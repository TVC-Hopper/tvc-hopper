classdef TelemetryViewerClient
    properties
        ip
        port
        tcpsock
    end
    
    methods
        function s = TelemetryViewerClient(ip, port)
            s.ip = ip;
            s.port = port;
            s.tcpsock = tcpclient(s.ip, s.port);

        end

        function RequestValue(s, id)
            packet = "val/%d";
            write(s.tcpsock, sprintf(packet, id))
        end

        function [id, sz, tstamp, value] = ReadValue(s, parser)
            buf = read(s.tcpsock);
            if ~isempty(buf)
                id = hex2dec(buf(1:4));
                sz = hex2dec(buf(5:6));
                tstamp = hex2dec(buf(7:14));
                value = parser(buf(15:(15+(2*sz)-1)));
            else
                id = 0;
                sz = 0;
                tstamp = 0;
                value = [];
            end
        end

        function StartStream(s, id, period)
            packet = "str/%d/%d";
            write(s.tcpsock, sprintf(packet, id, period));
        end


        function RequestProperty(s, id)
            packet = "get/%d";
            write(s.tcpsock, sprintf(packet, id))
        end

        function SetProperty(s, id, value)
            packet = "set/%d/%d";
            write(s.tcpsock, sprintf(packet, id, value))
        end
    end
    methods(Static)
        function arr = ParseU8Array(buffer)
            arr = zeros(1, length(buffer)/2);
            for i = 1:(length(buffer)/2)
                arr(i) = TelemetryViewerClient.ParseNumerical(buffer(2*i-1:2*i));
            end
        end

        function i = ParseNumerical(buffer)
            i = hex2dec(buffer);
        end
    end
end
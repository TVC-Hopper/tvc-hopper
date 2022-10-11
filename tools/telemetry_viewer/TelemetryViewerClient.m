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
            packet = append('val/', char(typecast(uint16(id), 'uint8')));
            write(s.tcpsock, packet);
        end

        function [id, sz, tstamp, value] = ReadValue(s)
            buf = read(s.tcpsock, 7);
            if length(buf) == 7
                id = typecast(buf(1:2), 'uint16');
                sz = typecast(buf(3), 'uint8');
                tstamp = typecast(buf(4:7), 'uint32');
                value = read(s.tcpsock, sz);
            else
                disp(buf);
                id = 0;
                sz = 0;
                tstamp = 0;
                value = [];
            end
        end

        function StartStream(s, id, period)
            packet = append('str/', char(typecast(uint16(id), 'uint8')), '/', char(typecast(uint16(period), 'uint8')));
            write(s.tcpsock, packet);
        end


        function RequestProperty(s, id)
            packet = append('get/', char(typecast(uint16(id), 'uint8')));
            write(s.tcpsock, packet)
        end

        function SetProperty(s, id, value)
            packet = append('set/', char(typecast(uint16(id), 'uint8')), '/', char(typecast(value, 'uint8')));
            write(s.tcpsock, packet)
        end
    end
end
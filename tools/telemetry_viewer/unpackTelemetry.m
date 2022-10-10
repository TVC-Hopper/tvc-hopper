function [p, v, a, att, alt] = unpackTelemetry(buffer)
        p = [];
        v = [];
        a = [];
        att = [];
        alt = [];
        
        if length(buffer) == 52
            p = double(typecast(uint8(buffer(1:12)), 'single'));
            v = double(typecast(uint8(buffer(13:24)), 'single'));
            a = double(typecast(uint8(buffer(25:36)), 'single'));
            att = double(typecast(uint8(buffer(37:48)), 'single'));
            alt = double(typecast(uint8(buffer(49:52)), 'single'));
        end
end
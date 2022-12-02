function [imu, gyro, lidar, esc] = unpackTelemetry(buffer)
        imu = [];
        gyro = [];
        lidar = [];
        esc = [];
        
        if length(buffer) == 32
            imu = double(typecast(uint8(buffer(1:12)), 'single'));
            gyro = double(typecast(uint8(buffer(13:24)), 'single'));
            lidar = double(typecast(uint8(buffer(25:28)), 'single'));
            esc = double(typecast(uint8(buffer(29:32)), 'single'));
        end
end

classdef TelemetryPlotter < handle
    properties
        tv;
        imuRPYAxLimY = [-5 5];
        imuGyroAxLimY = [-5 5];
        lidarAxLimY = [-180 180];
        escAxLimY = [900 2100];
        xRange_s = 20;

        stream_period_ms = 20;
        tickUpdateInterval = 20;
        dTick = 4000;
        
        telemetryId = 10;
        
        xRange_ms;
        tickBase;
        updateCounter = 0;
        
%         batt_t0
        telem_t0
        
        tim
        isStarted = false
        
        hfig
        f
        ax
        himu
        hgyro
        hlidar
        hesc
    end
    
    methods
        function s = TelemetryPlotter(tvc)
            s.tv = tvc;
            s.xRange_ms = s.xRange_s * 1000;
            s.tickBase = (0:s.dTick:s.xRange_ms);
        end
        
        function StartDataStream(s)
            s.tv.StartStream(s.telemetryId, s.stream_period_ms);
        end
        
        function s = ConfigurePlots(s)
            s.f = figure;
            s.f.Position = [100 100 1200 1200];
            subplotCount = 4;

            % imu (r,p,y)
            s.ax(1) = subplot(subplotCount,1,1);
            s.himu = [animatedline('Color', 'r', 'LineWidth', 1) ...
                    animatedline('Color', 'g', 'LineWidth', 1) ...
                    animatedline('Color', 'b', 'LineWidth', 1) ...
                  ];
            ylabel('rpy');
            legend('roll', 'pitch', 'yaw','AutoUpdate','off');
            set(s.ax(1), 'ylim', s.imuRPYAxLimY);

            % imu gyro (x,y,z)
            s.ax(2) = subplot(subplotCount,1,2);
            s.hgyro = [animatedline('Color', 'r', 'LineWidth', 1) ...
                    animatedline('Color', 'g', 'LineWidth', 1) ...
                    animatedline('Color', 'b', 'LineWidth', 1) ...
                  ];
            ylabel('gyro');
            legend('x', 'y', 'z','AutoUpdate','off');
            set(s.ax(2), 'ylim', s.imuGyroAxLimY);

            % lidar
            s.ax(3) = subplot(subplotCount,1,3);
            s.hlidar = [animatedline('Color', 'r', 'LineWidth', 1)];
            ylabel('altitude (lidar)');
            legend('altitude','AutoUpdate','off');
            set(s.ax(3), 'ylim', s.lidarAxLimY);

            % esc
            s.ax(4) = subplot(subplotCount,1,4);
            s.hesc = [animatedline('Color', 'r', 'LineWidth', 1)];
            ylabel('esc pulse width');
            legend('pw (us)','AutoUpdate','off'); 
            set(s.ax(4), 'ylim', s.escAxLimY);

            set(s.f,'defaultLegendAutoUpdate','off');
            s.hfig = axes(s.f, 'visible', 'off');
            s.hfig.Title.Visible = 'on';
            s.hfig.XLabel.Visible = 'on';
            xlabel(s.hfig, 'time (ms)');
            title(s.hfig, 'Telemetry Streams');
            linkaxes(s.ax,'x')
            set(s.hfig, 'xlim', [0 s.xRange_ms]);
            set(s.hfig, 'XTick', s.tickBase);
            % set(hfig,'LegendColorbarListeners',[]);
            setappdata(s.hfig,'LegendColorbarManualSpace',1);
            setappdata(s.hfig,'LegendColorbarReclaimSpace',1);

            pause(1);
            
        end
        
        function Stop(s)
            if s.isStarted
                stop(s.tim)
                delete(s.tim)
            end
            
            s.isStarted = false;
        end
        
        function Clear(s)
            for h = s.himu
                clearpoints(h);
            end
            
            for h = s.hgyro
                clearpoints(h);
            end
            
            for h = s.hlidar
                clearpoints(h);
            end
            
            for h = s.hesc
                clearpoints(h);
            end
        end
        
        function Plot(s)
            s.telem_t0 = 0;
            
            s.tim = timer('Period', 0.02, 'ExecutionMode', 'fixedRate');
            s.tim.TimerFcn = @(~, ~)s.PlotIteration;
                        
            for i = 1:5
                s.tv.RequestValue(s.telemetryId);
                [~, ~, s.telem_t0, ~] = s.tv.ReadValue();
                pause(0.1);
            end
            
            start(s.tim)
            s.isStarted = true;

        end
        
        function PlotIteration(s)
            s.tv.RequestValue(s.telemetryId);
            [~, ~, telem_tstamp, telem_value] = s.tv.ReadValue();
            [imu, gyro, lidar, esc] = unpackTelemetry(telem_value);

            t_tstamp = 0;


            if ~isempty(telem_value)
                t = double(telem_tstamp - s.telem_t0);
                t_tstamp = t;

                % imu
                addpoints(s.himu(1), t, imu(1));
                addpoints(s.himu(2), t, imu(2));
                addpoints(s.himu(3), t, imu(3));

                % gyro
                addpoints(s.hgyro(1), t, gyro(1));
                addpoints(s.hgyro(2), t, gyro(2));
                addpoints(s.hgyro(3), t, gyro(3));

                % lidar
                addpoints(s.hlidar(1), t, lidar);

                % esc
                addpoints(s.hesc(1), t, esc);
            end

            t = t_tstamp;
            lowerBound = max([0 (t - s.xRange_ms)]);
            upperBound = max([s.xRange_ms (t + s.xRange_ms)]);
            set(s.ax, 'xlim', [lowerBound upperBound]);

            if mod(s.updateCounter, s.tickUpdateInterval) == 0
                nearestTick = s.dTick * round(t / s.dTick);
                xTick = nearestTick + s.tickBase;
                set(s.hfig, 'XTick', xTick);
            end

            drawnow limitrate
            s.updateCounter = s.updateCounter + 1;
        end
    end
end


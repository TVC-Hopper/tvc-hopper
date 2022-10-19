classdef TelemetryPlotter < handle
    properties
        tv;
        posAxLimY = [-60 60];
        velAxLimY = [-10 10];
        accelAxLimY = [-20 20];
        attitudeAxLimY = [-180 180];
        altAxLimY = [-1 10];
        battVoltLimY = [0 25];
        xRange_s = 20;

        telemetryStream_period_ms = 80;
        battVoltStream_period_ms = 1000;
        tickUpdateInterval = 20;
        dTick = 4000;
        
        telemetryPropId = 3;
        battVoltPropId = 4;
        
        xRange_ms;
        tickBase;
        updateCounter = 0;
        
        batt_t0
        telem_t0
        
        tim
        isStarted = false
        
        hfig
        f
        ax
        hp
        hv
        ha
        hatt
        halt
        hb
    end
    
    methods
        function s = TelemetryPlotter(ip, port)
            s.tv = TelemetryViewerClient(ip, port);
            s.xRange_ms = s.xRange_s * 1000;
            s.tickBase = (0:s.dTick:s.xRange_ms);
        end
        
        function StartDataStream(s)
            s.tv.StartStream(s.telemetryPropId, s.telemetryStream_period_ms);
            s.tv.StartStream(s.battVoltPropId, s.battVoltStream_period_ms);
        end
        
        function s = ConfigurePlots(s)
            s.f = figure;
            s.f.Position = [100 100 1200 1200];
            subplotCount = 6;

            % position (x,y,z)
            s.ax(1) = subplot(subplotCount,1,1);
            s.hp = [animatedline('Color', 'r', 'LineWidth', 1) ...
                    animatedline('Color', 'g', 'LineWidth', 1) ...
                    animatedline('Color', 'b', 'LineWidth', 1) ...
                  ];
            ylabel('position');
            legend('x', 'y', 'z','AutoUpdate','off');
            set(s.ax(1), 'ylim', s.posAxLimY);

            % velocity (x,y,z)
            s.ax(2) = subplot(subplotCount,1,2);
            s.hv = [animatedline('Color', 'r', 'LineWidth', 1) ...
                    animatedline('Color', 'g', 'LineWidth', 1) ...
                    animatedline('Color', 'b', 'LineWidth', 1) ...
                  ];
            ylabel('velocity');
            legend('x', 'y', 'z','AutoUpdate','off');
            set(s.ax(2), 'ylim', s.velAxLimY);

            % acceleration (x,y,z)
            s.ax(3) = subplot(subplotCount,1,3);
            s.ha = [animatedline('Color', 'r', 'LineWidth', 1) ...
                    animatedline('Color', 'g', 'LineWidth', 1) ...
                    animatedline('Color', 'b', 'LineWidth', 1) ...
                  ];
            ylabel('acceleration');
            legend('x', 'y', 'z','AutoUpdate','off');
            set(s.ax(3), 'ylim', s.accelAxLimY);

            % attitude (x,y,z)
            s.ax(4) = subplot(subplotCount,1,4);
            s.hatt = [animatedline('Color', 'r', 'LineWidth', 1) ...
                    animatedline('Color', 'g', 'LineWidth', 1) ...
                    animatedline('Color', 'b', 'LineWidth', 1) ...
                  ];
            ylabel('attitude');
            legend('yaw', 'pitch', 'roll','AutoUpdate','off'); 
            set(s.ax(4), 'ylim', s.attitudeAxLimY);

            % altitiude (x,y,z)
            s.ax(5) = subplot(subplotCount,1,5);
            s.halt = [animatedline('Color', 'r', 'LineWidth', 1)];
            ylabel('altitude');
            set(s.ax(5), 'ylim', s.altAxLimY);


            % battery voltage (x,y,z)
            s.ax(6) = subplot(subplotCount,1,6);
            s.hb = [animatedline('Color', 'r', 'LineWidth', 1)];
            ylabel('voltage');
            set(s.ax(6), 'ylim', s.battVoltLimY);

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
            for h = s.hp
                clearpoints(h);
            end
            
            for h = s.hv
                clearpoints(h);
            end
            
            for h = s.ha
                clearpoints(h);
            end
            
            for h = s.halt
                clearpoints(h);
            end
            
            for h = s.hatt
                clearpoints(h);
            end
            
            for h = s.hb
                clearpoints(h);
            end
        end
        
        function Plot(s)
            s.telem_t0 = 0;
            s.batt_t0 = 0;
            
            s.tim = timer('Period', 0.02, 'ExecutionMode', 'fixedRate');
            s.tim.TimerFcn = @(~, ~)s.PlotIteration;
                        
            for i = 1:5
                s.tv.RequestValue(s.telemetryPropId);
                [~, ~, s.telem_t0, ~] = s.tv.ReadValue();

                s.tv.RequestValue(s.battVoltPropId);
                [~, ~, s.batt_t0, ~] = s.tv.ReadValue();
                pause(0.1);
            end
            
            start(s.tim)
            s.isStarted = true;

        end
        
        function PlotIteration(s)
            s.tv.RequestValue(s.telemetryPropId);
            [~, ~, telem_tstamp, telem_value] = s.tv.ReadValue();
            [p, v, a, att, alt] = unpackTelemetry(telem_value);

            s.tv.RequestValue(s.battVoltPropId);
            [~, ~, batt_tstamp, batt_value] = s.tv.ReadValue();

            t_tstamp = 0;
            b_tstamp = 0;

            if ~isempty(telem_value)
                t = double(telem_tstamp - s.telem_t0);
                t_tstamp = t;

                % position
                addpoints(s.hp(1), t, p(1));
                addpoints(s.hp(2), t, p(2));
                addpoints(s.hp(3), t, p(3));

                % velocity
                addpoints(s.hv(1), t, v(1));
                addpoints(s.hv(2), t, v(2));
                addpoints(s.hv(3), t, v(3));

                % acceleration
                addpoints(s.ha(1), t, a(1));
                addpoints(s.ha(2), t, a(2));
                addpoints(s.ha(3), t, a(3));

                % attitude
                addpoints(s.hatt(1), t, att(1));
                addpoints(s.hatt(2), t, att(2));
                addpoints(s.hatt(3), t, att(3));

                % altitude
                addpoints(s.halt(1), t, alt(1));
            end

            if ~isempty(batt_value)
                t = double(batt_tstamp - s.batt_t0);
                b_tstamp = t;
                voltage = double(typecast(uint8(batt_value), 'single'));
                addpoints(s.hb(1), t, voltage);
            end

            t = max([b_tstamp t_tstamp]);
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


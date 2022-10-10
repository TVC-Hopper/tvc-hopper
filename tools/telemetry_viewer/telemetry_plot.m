
tv = TelemetryViewerClient("127.0.0.1", 65123);

posAxLimY = [-60 60];
velAxLimY = [-10 10];
accelAxLimY = [-20 20];
attitudeAxLimY = [-180 180];
altAxLimY = [-1 10];
battVoltLimY = [0 25];

xRange_s = 30;

telemetryStream_period_ms = 20;
battVoltStream_period_ms = 20;

tickUpdateInterval = 20;
dTick = 2000;

telemetryPropId = 3;
battVoltPropId = 4;

xRange_ms = xRange_s * 1000;
tickBase = (0:dTick:xRange_ms);
updateCounter = 0;

%%

f = figure;
f.Position = [100 100 1200 1200];
subplotCount = 6;

% position (x,y,z)
ax(1) = subplot(subplotCount,1,1);
hp = [animatedline('Color', 'r', 'LineWidth', 1) ...
        animatedline('Color', 'g', 'LineWidth', 1) ...
        animatedline('Color', 'b', 'LineWidth', 1) ...
      ];
ylabel('position');
legend('x', 'y', 'z','AutoUpdate','off');
set(ax(1), 'ylim', posAxLimY);

% velocity (x,y,z)
ax(2) = subplot(subplotCount,1,2);
hv = [animatedline('Color', 'r', 'LineWidth', 1) ...
        animatedline('Color', 'g', 'LineWidth', 1) ...
        animatedline('Color', 'b', 'LineWidth', 1) ...
      ];
ylabel('velocity');
legend('x', 'y', 'z','AutoUpdate','off');
set(ax(2), 'ylim', velAxLimY);

% acceleration (x,y,z)
ax(3) = subplot(subplotCount,1,3);
ha = [animatedline('Color', 'r', 'LineWidth', 1) ...
        animatedline('Color', 'g', 'LineWidth', 1) ...
        animatedline('Color', 'b', 'LineWidth', 1) ...
      ];
ylabel('acceleration');
legend('x', 'y', 'z','AutoUpdate','off');
set(ax(3), 'ylim', accelAxLimY);

% attitude (x,y,z)
ax(4) = subplot(subplotCount,1,4);
hatt = [animatedline('Color', 'r', 'LineWidth', 1) ...
        animatedline('Color', 'g', 'LineWidth', 1) ...
        animatedline('Color', 'b', 'LineWidth', 1) ...
      ];
ylabel('attitude');
legend('yaw', 'pitch', 'roll','AutoUpdate','off'); 
set(ax(4), 'ylim', attitudeAxLimY);

% altitiude (x,y,z)
ax(5) = subplot(subplotCount,1,5);
halt = [animatedline('Color', 'r', 'LineWidth', 1)];
ylabel('altitude');
set(ax(5), 'ylim', altAxLimY);


% battery voltage (x,y,z)
ax(6) = subplot(subplotCount,1,6);
hb = [animatedline('Color', 'r', 'LineWidth', 1)];
ylabel('voltage');
set(ax(6), 'ylim', battVoltLimY);

set(f,'defaultLegendAutoUpdate','off');
hfig = axes(f, 'visible', 'off');
hfig.Title.Visible = 'on';
hfig.XLabel.Visible = 'on';
xlabel(hfig, 'time (ms)');
title(hfig, 'Telemetry Streams');
linkaxes(ax,'x')
set(hfig, 'xlim', [0 xRange_ms]);
set(hfig, 'XTick', tickBase);
% set(hfig,'LegendColorbarListeners',[]);
setappdata(hfig,'LegendColorbarManualSpace',1);
setappdata(hfig,'LegendColorbarReclaimSpace',1);

pause(1);

%%
tv.StartStream(telemetryPropId, telemetryStream_period_ms);
tv.StartStream(battVoltPropId, battVoltStream_period_ms);


%%

telem_t0 = 0;
batt_t0 = 0;

%% ensure values and streams have started, wait for valid timestamp
% since timestamp comes from SPP client, we can ensure these values have
% arrived from the client and are ready to be plotted
for i = 1:10
    tv.RequestValue(telemetryPropId);
    [~, ~, telem_t0, ~] = tv.ReadValue();
    
    tv.RequestValue(battVoltPropId);
    [~, ~, batt_t0, ~] = tv.ReadValue();
    pause(0.1);
end

profile on

while (updateCounter < 50)

    tv.RequestValue(telemetryPropId);
    [~, ~, telem_tstamp, telem_value] = tv.ReadValue();
    [p, v, a, att, alt] = unpackTelemetry(telem_value);
    
    tv.RequestValue(battVoltPropId);
    [~, ~, batt_tstamp, batt_value] = tv.ReadValue();
    
    t_tstamp = 0;
    b_tstamp = 0;
    
    if ~isempty(telem_value)
        t = double(telem_tstamp - telem_t0);
        t_tstamp = t;
        
        % position
        addpoints(hp(1), t, p(1));
        addpoints(hp(2), t, p(2));
        addpoints(hp(3), t, p(3));
                
        % velocity
        addpoints(hv(1), t, v(1));
        addpoints(hv(2), t, v(2));
        addpoints(hv(3), t, v(3));
        
        % acceleration
        addpoints(ha(1), t, a(1));
        addpoints(ha(2), t, a(2));
        addpoints(ha(3), t, a(3));
        
        % attitude
        addpoints(hatt(1), t, att(1));
        addpoints(hatt(2), t, att(2));
        addpoints(hatt(3), t, att(3));
        
        % altitude
        addpoints(halt(1), t, alt(1));
    end
    
    if ~isempty(batt_value)
        t = double(batt_tstamp - batt_t0);
        b_tstamp = t;
        voltage = double(typecast(uint8(batt_value), 'single'));
        addpoints(hb(1), t, voltage);
    end
    
    t = max([b_tstamp t_tstamp]);
    lowerBound = max([0 (t - xRange_ms)]);
    upperBound = max([xRange_ms (t + xRange_ms)]);
    set(ax, 'xlim', [lowerBound upperBound]);
    
    if mod(updateCounter, tickUpdateInterval) == 0
        nearestTick = dTick * round(t / dTick);
        xTick = nearestTick + tickBase;
        set(hfig, 'XTick', xTick);
    end
    
    drawnow limitrate
    updateCounter = updateCounter + 1;

    pause(0.015); 
end

profile viewer

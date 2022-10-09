
tv = TelemetryViewerClient("127.0.0.1", 65123);

tv.StartStream(3, 20);

%%

f = figure;
hx = animatedline('Color', 'r', 'LineWidth', 1);
hy = animatedline('Color', 'g', 'LineWidth', 1);
hz = animatedline('Color', 'b', 'LineWidth', 1);
xlabel('time');
ylabel('position');
legend('x', 'y', 'z');

%%

sz = 0;
t0 = 0;

while (t0 == 0)
    tv.RequestValue(3);
    [id, sz, tstamp, value] = tv.ReadValue(@TelemetryViewerClient.ParseU8Array);

    t0 = tstamp;
    pause(0.1);
end


while (true)
    tv.RequestValue(3);
    [id, sz, tstamp, value] = tv.ReadValue(@TelemetryViewerClient.ParseU8Array);
    
%     fprintf('%d %d %d', id, sz, tstamp);
    
    if ~isempty(value)
%         disp(value);

        t = tstamp - t0;
        addpoints(hx, t, value(1));
        addpoints(hy, t, value(2));
        addpoints(hz, t, value(3));
        drawnow;
    end

    pause(0.02)
end
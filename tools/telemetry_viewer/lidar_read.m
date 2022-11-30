tvc = TelemetryViewerClient("172.20.10.10", 65123);
pause(5)
while 1
    tvc.RequestProperty(50);
    pause(1);
    tvc.RequestValue(50);
    [id, sz, t, val] = tvc.ReadValue();
    disp(val);
end
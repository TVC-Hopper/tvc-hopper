tvc = TelemetryViewerClient("127.0.0.1", 65123);
pause(10)
while 1
    tvc.RequestProperty(51);
    pause(1);
    tvc.RequestValue(51);
    [id, sz, t, val] = tvc.ReadValue();
    disp(val);
end
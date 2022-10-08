

t = tcpclient("127.0.0.1", 65123);

% get request
% get/id
packet = "get/0";
write(t, packet)

% stream request
% str/id/period
% packet = "str/0/50";
% write(t, packet)

% set request
% set/id/value
% packet = "set/0/1";
% write(t, packet)

% get value
% get/id
% packet = "val/0";
% write(t, packet)

pause(1);
read(t)


% close connection
clear t




t = tcpclient("127.0.0.1", 65123);
%%

% get request
% get/id
% packet = "get/0";
% write(t, packet)
% 
% packet = "get/1";
% write(t, packet)
% 
% packet = "get/2";
% write(t, packet)
% 
% packet = "get/3";
% write(t, packet)

% stream request
% str/id/period
packet = "str/3/500";
write(t, packet)
%%

% set request
% set/id/value
% packet = "set/0/1";
% write(t, packet)

% get value
% get/id
packet = "val/3";
write(t, packet)
read(t)




%%

% close connection
clear t


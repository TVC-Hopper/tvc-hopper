% while 1

% end

clear r p y gx gy gz i fval i id sz t

r = animatedline('Color',"red");
p = animatedline('Color',"magenta");
y = animatedline('Color',"#A2142F");

gx = animatedline('Color',"green");
gy = animatedline('Color',"blue");
gz = animatedline('Color',"cyan");

i = 1;
tvc.RequestProperty(52);
pause(0.1);

legend('roll', 'pitch', 'yaw', 'gx', 'gy', 'gz');
while 1
    tvc.RequestValue(52);
    [id, sz, t, val] = tvc.ReadValue();
    fval = typecast(uint8(val), 'single');
    tvc.RequestProperty(52);
%     disp(fval);

    
    axis([max(i-500, 0) max(500 + i, i) -5 5])
    
    % first line
    addpoints(r,i,double(fval(1)));
    addpoints(p,i,double(fval(2)));
    addpoints(y,i,double(fval(3)));
    
    addpoints(gx,i,double(fval(4)));
    addpoints(gy,i,double(fval(5)));
    addpoints(gz,i,double(fval(6)));
    
    i = i + 1;
    
    
    % update screen
    drawnow limitrate
    pause(0.1);
end
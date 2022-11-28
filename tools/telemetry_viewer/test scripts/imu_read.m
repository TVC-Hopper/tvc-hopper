% while 1

% end

clear ax ay az gx gy gz i fval i id sz t

ax = animatedline('Color',"red");
ay = animatedline('Color',"magenta");
az = animatedline('Color',"#A2142F");

gx = animatedline('Color',"green");
gy = animatedline('Color',"blue");
gz = animatedline('Color',"cyan");

i = 1;
tvc.RequestProperty(51);
pause(0.1);
while 1
    tvc.RequestValue(51);
    [id, sz, t, val] = tvc.ReadValue();
    fval = typecast(uint8(val), 'single');
    tvc.RequestProperty(51);
%     disp(fval);

    
    axis([max(i-500, 0) max(500 + i, i) -5 5])
    
    % first line
    addpoints(ax,i,double(fval(1)));
    addpoints(ay,i,double(fval(2)));
    addpoints(az,i,double(fval(3)));
    
    addpoints(gx,i,double(fval(4)));
    addpoints(gy,i,double(fval(5)));
    addpoints(gz,i,double(fval(6)));
    
    i = i + 1;
    
    
    % update screen
    drawnow limitrate
    pause(0.1);
end
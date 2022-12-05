% while 1

% end

clear r p y gx gy gz esc i fval i id sz t

tvc = vc.tvc;

r = animatedline('Color',"red");
p = animatedline('Color',"magenta");
y = animatedline('Color',"#A2142F");

gx = animatedline('Color',"green");
gy = animatedline('Color',"blue");
gz = animatedline('Color',"cyan");

esc = animatedline('Color',"black");

i = 1;
tvc.RequestProperty(52);
tvc.RequestProperty(53);
pause(0.1);

legend('roll', 'pitch', 'yaw', 'gx', 'gy', 'gz', 'esc');
while 1
    tvc.RequestValue(52);
    [~, ~, ~, val_imu] = tvc.ReadValue();
    tvc.RequestValue(53);
    [id, sz, t, val_esc] = tvc.ReadValue();
    
    imu_val_f = typecast(uint8(val_imu), 'single');
    esc_val_f = typecast(uint8(val_esc), 'single');

    disp(esc_val_f);
    
    tvc.RequestProperty(52);
    tvc.RequestProperty(53);
%     disp(fval);

    
    axis([max(i-500, 0) max(500 + i, i) -5.1 5.1])
    
    % first line
    addpoints(r,i,double(imu_val_f(1)));
    addpoints(p,i,double(imu_val_f(2)));
    addpoints(y,i,double(imu_val_f(3)));
    
    addpoints(gx,i,double(imu_val_f(4)));
    addpoints(gy,i,double(imu_val_f(5)));
    addpoints(gz,i,double(imu_val_f(6)));
    
    addpoints(esc,i,double(esc_val_f));
    
    i = i + 1;
    
    % update screen
    drawnow limitrate
    pause(0.1);
end
measured_angle1 = 41.5;
measured_angle2 = 41;
measured_angle3 = 40;


realdot = @(u, v) u*transpose(v);
syms x y z
P = [x, y, z];

% 3 points on plane 1:
theta_Pl1 = (90 - measured_angle1) * pi/180;

Pl1_pt1 = [0,0,0];
Pl1_pt2 = [0,1,0];
Pl1_pt3 = [cos(theta_Pl1), 0, sin(theta_Pl1)];

normalPl1 = cross(Pl1_pt1 - Pl1_pt2, Pl1_pt1 - Pl1_pt3);
Plane1 = realdot(P - Pl1_pt1, normalPl1);
Plane1_eqn = Plane1 == 0;

% 3 points on plane 2:
theta_Pl2 = (90 - measured_angle2) * pi/180;

Pl2_pt1 = [0,0,0];
Pl2_pt2 = [1,0,0];
Pl2_pt3 = [0, cos(theta_Pl2), sin(theta_Pl2)];

normalPl2 = cross(Pl2_pt1 - Pl2_pt2, Pl2_pt1 - Pl2_pt3);
Plane2 = realdot(P - Pl2_pt1, normalPl2);
Plane2_eqn = Plane2 == 0;

% 3 points on plane 3:
theta_Pl3 = (90 - measured_angle3) * pi/180;

Pl3_pt1 = [9.5625, 0, 0];
Pl3_pt2 = [9.5625, 1, 0];
Pl3_pt3 = [9.5625 - cos(theta_Pl3), 0, sin(theta_Pl3)];

normalPl3 = cross(Pl3_pt1 - Pl3_pt2, Pl3_pt1 - Pl3_pt3);
Plane3 = realdot(P - Pl3_pt1, normalPl3);
Plane3_eqn = Plane3 == 0;

cm = solve([Plane1_eqn, Plane2_eqn, Plane3_eqn], x, y, z);
x1 = double(cm.x);
y1 = double(cm.y);
z1 = double(cm.z);
disp('x(inch): '); disp(x1);
disp('y(inch): '); disp(y1);
disp('z(inch): '); disp(z1);

disp('x(cm): '); disp(2.54 * x1);
disp('y(cm): '); disp(2.54 * y1);
disp('z(cm): '); disp(2.54 * z1);




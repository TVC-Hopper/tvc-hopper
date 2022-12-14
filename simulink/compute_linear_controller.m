clc
clear

syms x y z vx vy vz p q u wx wy wz      % States
syms Ft F1 F2 F3 F4 Fd a1 a2 a3 a4 wt   % Inputs 
syms Kt CL CD CF m Jx Jy Jz r l g       % Constants

%% Definitions
% Transformation matrix from body angular velocity to Tait-Bryan rates
W = [ 1, 0,      -sin(q)         ;
      0, cos(p),  cos(q)*sin(p)  ;
      0, -sin(p), cos(q)*cos(p) ];

Winv = simplify(inv(W));
  
% Rotation matrix from body to world frame, input: roll, pitch, yaw
R = [cos(q)*cos(u), sin(p)*sin(q)*cos(u)-cos(p)*sin(u), cos(p)*sin(q)*cos(u)+sin(p)*sin(u) ;
     cos(q)*sin(u), sin(p)*sin(q)*sin(u)+cos(p)*cos(u), cos(p)*sin(q)*sin(u)-sin(p)*cos(u) ;
     -sin(q),       sin(p)*cos(q),                      cos(p)*cos(q)                     ];
 

% Matrix of mass inertia
J = [Jx 0  0  ;
     0  Jy 0  ;
     0  0  Jz];
 
% Forces 
Ft = Kt * wt^2; % motor force
% in testbed, the value of wt is 0 when no roll/pitch stabilization is 
% needed, but this equation still holds but Ft is a constant in this 
% system's inputs
F1 = Ft * CL * a1;
F2 = Ft * CL * a2;
F3 = Ft * CL * a3;
F4 = Ft * CL * a4;
Fd = Ft * CD; % TODO verify that this is total drag and not per vane

% Body forces
fb = [ F2 + F4  ;
       F1 + F3  ;
       Ft - Fd ]; % TODO determine if total drag is calculated

% Body torques
tb = [ (F1 + F3)*l            ;
      -(F2 + F4)*l            ;
       (F1 - F2 - F3 + F4)*r ];
   
% State vectors used for derivation
nw = [p q u].';     % Attitude (world frame)
wb = [wx wy wz].';  % Angular velocity (body frame)
pw = [x y z].';     % Position (world frame)
vb = [vx vy vz].';  % Velocity (body frame)

% Total state vector
X = [nw; wb; pw; vb];
X_red = [nw; wb; pw(3); vb(3)]; % Reduced state vector (only attitude and altitude)
X_hor = [ pw(1); pw(2); vb(1); vb(2) ]; % Reduced state vector for horizontal movements


X_roll = [nw(1); wb(1); pw(1); vb(1)];

% Input vector 
U = [a1; a2; a3; a4; wt];

% Input vector for horizontal model
U_hor = [p; q];

% Roll 
U_roll = [a1; a2; a3; a4];

%% Rotational dynamics

nw_dot = Winv * wb;
wb_dot = inv(J) * (tb - cross(wb, J * wb)  );

%% Translational dynamics

pw_dot = R * vb;
vb_dot = 1/m * ( fb -  R.' * [0 0 m*g].');

% Translational dynamics in world
vw_dot = 1/m * R * fb - [0 0 m*g].';

%% Combined non-linear model

f = [ nw_dot  ;
      wb_dot  ;
      pw_dot  ;
      vb_dot ];

% Reduced non-linear model
f_red = [ nw_dot     ;
          wb_dot     ;
          pw_dot(3)  ;
          vb_dot(3) ];
      
% Horizontal non-linear model
f_hor = [ pw_dot(1) ;
          pw_dot(2) ;
          vb_dot(1) ;
          vb_dot(2)];
      
      
f_roll = [ nw_dot(1) ;
          wb_dot(1)  ;
          pw_dot(1)  ;
          vb_dot(1) ];
  
%% Linearization

% Using the Jacobian method, the set of nonlinear system equations are
% linearized around the hover point

A = jacobian(f, X);
B = jacobian(f, U);

% Reduced model (only z-axis in position)
A2 = jacobian(f_red, X_red);
B2 = jacobian(f_red, U);

% Horizontal model (only x- and y-direction)
A3 = jacobian( f_hor, X_hor );
B3 = jacobian( f_hor, U_hor );


% Single dimension model (roll/x axis)
A4 = jacobian( f_roll, X_roll );
B4 = jacobian( f_roll, U_roll );

% The A and B matrixes are now filled with partial derivatives, similar to
% an taylor expansion to approximate a nonlinear function/ODE
% We must insert the state- and input-values at the operating point

% All the states is zero at the hover point
x = 0; y = 0; z = 0; vx = 0; vy = 0; vz = 0; p = 0; q = 0; u = 0; wx = 0; wy = 0; wz = 0;

% subs(B)

% All input is not zero!
a1 = 0; a2 = 0; a3 = 0; a4 = 0; % TODO determine if radians or degrees
% TODO change wt according to thrust tests
wt = 20.062; % kRPM at hoverpoint
% in testbed, this will be some small value?
% do we need to back calculate from control force? or just pick a value
% that is small but large enough to generate a control force?

% Drone Constants ---------------------
% TODO replace values

Kt = 0.021952;      % scaling factor for wt^2 to yield thrust [N / (1/s^2)] Kf or Kt?
    
Aduct = 1;          % cross sectional area of exhaust [m^2]
Afin = 0.5;         % surface area of airfoil [m^2]

CLa = 0.008905;     % slope of lift coeff, same for all Joukovsky airfoils
CL = CLa * Afin / (2 * Aduct);
% CL = 0.008905;    % original value
% Can we estimate CL with a linear equation for larger angles of attack?

CD0 = 0.001054;     % constant bias of drag coeff, same for all Joukovsky airfoils
CD = CD0 * Afin / (2 * Aduct); % should this be x4 for total drag? within +/- 10 deg, CD is constant
% CD = 0.001054;    % original value

Jx = 0.01031759;    % x moment of inertia [Kg * m^2]
Jy = 0.01031865;    % y moment of inertia [Kg * m^2]
Jz = 0.00278832;    % z moment of inertia [Kg * m^2]

m = 0.92;           % mass of vehicle [Kg]
g = 9.807;          % gravitational acceleration [m/s^2]

l = 0.09471940;     % distance between COM and thrust vane joint [m]
r = 0.04;           % distance between z-axis and middle of thrust vane [m]
                    % Doesn't this value vary with alpha?

% ---------------------

% Now the A and B matrixes can be evaluted, yield the full linear model
% around the hover point.
A_sys = double(vpa(subs(A), 4));
B_sys = double(vpa(subs(B), 4));
C_sys = eye(12);
D_sys = zeros(12,5);

% Reduced model 
A_red = double(vpa(subs(A2), 4));
B_red = double(vpa(subs(B2), 4));
C_red = eye(8);
D_red = zeros(8,5);

% Horizontal model
A_hor = double(vpa(subs(A3),4));
B_hor = double(vpa(subs(B3),4));
C_hor = eye(4);
D_hor = zeros(4,2);

% Reduced model with integral action states
G_hov = [ 0 0 0 0 0 0 1 0 ]; % z
   
A_int = [A_red; G_hov];
A_int = [A_int zeros(9,1) ];
B_int = [B_red; zeros(1,5) ];
C_int = eye(9);
D_int = zeros(9,5);


% Horizontal model with integral action states
G_pos = [ 1 0 0 0; 
         0 1 0 0 ];

A_hint = [A_hor; G_pos];
A_hint = [A_hint zeros(6,2) ];
B_hint = [B_hor; zeros(2,2) ];
C_hint = eye(6);
D_hint = zeros(6,2);

     
%% Open Loop dynamics

sys = ss(A_sys,B_sys,C_sys,D_sys);
sys_red = ss(A_red,B_red,C_red,D_red);
sys_int = ss(A_int,B_int,C_int, D_int);
sys_hor = ss(A_hor, B_hor, C_hor, D_hor);
sys_hint = ss(A_hint, B_hint, C_hint, D_hint);

%% Design controller

% Bryson's Rule. 
% Max angle of 0.3 radians. Maximum angular rate of 5 rad/second
Q = [ 1/0.1^2     0        0        0      0      0      0        0    ;  % Roll
      0        1/0.1^2     0        0      0      0      0        0    ;  % Pitch
      0        0        1/1^2    0      0      0      0        0       ;  % Yaw
      0        0        0        1/1^2  0      0      0        0       ;  % omega_x
      0        0        0        0      1/1^2  0      0        0       ;  % omega_y
      0        0        0        0      0      1/2^2  0        0       ;  % omega_z
      0        0        0        0      0      0      1/0.5^2    0       ;  % z
      0        0        0        0      0      0      0        1/1^2  ]; % v_z

Q_red = Q;  
  
% Integral action  
Q(9,9) = [ 1/0.15^2 ]; % z
      
% Max actuation angle of +-10 degrees
R = [ 1/10^2   0       0       0       0       ; % a1
      0        1/10^2  0       0       0       ; % a2
      0        0       1/10^2  0       0       ; % a3
      0        0       0       1/10^2  0       ; % a4
      0        0       0       0       1/1^2  ]; % wt
  
% Compute "optimal" controller
K_hov = lqr(sys_int, Q, R);
K_red = lqr(sys_red, Q_red, R);

% Compute integral limit matching the steady-state motor velocity
int_lim = wt/K_hov(5,9) + wt*0.005;

sys_d = c2d(sys_int, 0.008, 'zoh' );

K_lqrd = dlqr(sys_d.A, sys_d.B, Q, R);

% matrix_to_cpp( K_hov )

% Calcuate closed loop system
% figure(1)
%cl_sys = ss((A_red - B_red*K_red), B_red, C_red, D_red );
sys_cl_hov = feedback( sys_red*K_red, eye(8));

figure(1)
pzmap(sys_cl_hov);
[p,z] = pzmap(sys_cl_hov);
grid on

Q_pos = [ 1/0.5^2  0         0        0        ; % max dev of x and y is 0.5 m
          0         1/0.5^2  0        0        ;
          0         0         1/2^2  0        ; % max vx and vy is 1 m/s
          0         0         0        1/2^2 ];

Q_hor = Q_pos;      
      
% TODO determine what these values represent
Q_pos(5:6,5:6) = [ 1/1^2  0
                   0        1/1^2];
     
R_pos = [ 1/0.05^2  0;          % max allowable actuation of 0.1 rad = 5 deg
          0          1/0.05^2]; % roll and pitch

K_pos = lqr(sys_hint, Q_pos, R_pos);
K_hor = lqr(sys_hor, Q_hor, R_pos);


sys_cl_pos = feedback( sys_hor*K_hor, eye(4));

figure(2)
pzmap(sys_cl_pos);
[p2,z2] = pzmap(sys_cl_pos);
grid on
% sys_total = series( sys_cl_pos, sys_cl_pos )

% matrix_to_cpp( K_pos )

%% Symbolic Discretization

% syms dt;
% M = expm([A_sym, B_sym; zeros(5,12), zeros(5,5) ]*dt);
% 
% Ad = M(1:12, 1:12);
% Bd = M(1:12, 13:17);


%% Functions 

function matrix_to_cpp( matrix )

    name = inputname(1);
    [m, n] = size(matrix);
    
    tol = 1.e-6;
    matrix(matrix<0 & matrix>-tol) = 0;
    
    fprintf('Matrix %s \n', name);
    
    for i = 1:m
        line = string();
        for j = 1:n
            str = sprintf('%.4f,', round( matrix(i,j), 4 ) );
            value = pad(str, 10, 'left');

            line = append( line, value );
        end
        disp(line);
    end
end
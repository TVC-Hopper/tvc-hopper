function ss_rep = getStateSpaceRep()
    syms x y z vx vy vz p q u wx wy wz      % States
    syms Ft F1 F2 F3 F4 Fd a1 a2 a3 a4 wt    % Inputs 
    syms Kt1 Kt2 CL CD CF m Jx Jy Jz r l g       % Constants
    
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
    %Ft = Kt * wt^2;
    Ft = Kt1*wt^2 + Kt2*wt;
    F1 = Ft * CL * a1;
    F2 = Ft * CL * a2;
    F3 = Ft * CL * a3;
    F4 = Ft * CL * a4;
    Fd = Ft * CD; 
    
    % Body forces
    fb = [ F2 + F4  ;
           F1 + F3  ;
           Ft - Fd ];
    
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
    %U = [a1; a2; a3; a4; wt];
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
    a1 = 0; a2 = 0; a3 = 0; a4 = 0; 
    wt = .5430;
    
    % Drone Constants
    Kt1 = 18.4626;      % N / (1/s^2)
    Kt2 = 18.4714;
    CL = .1081 * .0024265 / (2 * .0065698);
    CD = .0145 * .0024265 / (2 * .0065698);
    Jx = 0.0041;    % Kg * m^2
    Jy = 0.0074;    % Kg * m^2
    Jz = 0.0065;    % Kg * m^2
    m = 1.578;
    g = 9.807;          % m/s^2
    l = 0.0627;     % m
    r = 0.0128;           % m 
    
    
    % Reduced model 
    A_red = double(vpa(subs(A2), 4));
    B_red = double(vpa(subs(B2), 4));
    
    % Reduced model with integral action states
    G_hov = [ 0 0 0 0 0 0 1 0 ]; % z
       
    A_int = [A_red; G_hov];
    A_int = [A_int zeros(9,1) ];
    B_int = [B_red; zeros(1,5) ];
    C_int = eye(9);
    D_int = zeros(9,5);
         
    %% Open Loop dynamics
    ss_rep = ss(A_int,B_int,C_int, D_int);
end
Q = [ .001     0           0        0      0      0      0        0       ;  % Roll
      0     .001           0        0      0      0      0        0       ;  % Pitch
      0        0        .001        0      0      0      0        0       ;  % Yaw
      0        0           0     .001      0      0      0        0       ;  % omega_x
      0        0           0        0   .001      0      0        0       ;  % omega_y
      0        0           0        0      0   .001      0        0       ;  % omega_z
      0        0           0        0      0      0     .1        0       ;  % z
      0        0           0        0      0      0      0     .001      ];  % v_z


Q_red = Q;  
  
% Integral action  
% Q(9,9) = [ 1/0.15^2 ]; % z
Q(9,9) = [ .001 ]; % z
      
% Max actuation angle of +-10 degress
R = [ 1/10^2   0       0       0       0       ; % a1
      0        1/10^2  0       0       0       ; % a2
      0        0       1/10^2  0       0       ; % a3
      0        0       0       1/10^2  0       ; % a4
      0        0       0       0       1  ]; % wt
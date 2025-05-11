function [y_el, y_az, x_hat_el, x_hat_az, accel_el, accel_az, torque_el, torque_az] = ...
    control_system(time, theta_el_desired, theta_az_desired)

% --------- Global Parameters ----------
 
Vs = single(12);        % Supply Voltage [V]
I_el = single(0.02523);     % Moment of Inertia (EL)[kg.m^2]
I_az = single(0.02035);     % Moving Assy Mom. of Inertia about AZ Axis [kg/m^2]
 
% ---------- Elevation Motor Parameters (Transmotec) -------------
 
I_el_stall = single(0.24);    % nominal current[A] 
I_el_no_load = single(0.06);  % No load current [A]
omega_el_no_load = single(4900*2*pi/60); % [rad/s]
R_el_motor = single(Vs/I_el_stall);       % motor resistance [ohm]
gear_ratio_el = single(200); % Gear reduction
gear_eff_el = 0.343;
K_e_el = single(((Vs-(I_el_no_load * R_el_motor))*gear_ratio_el)/(omega_el_no_load)); % Back emf constant [V/(rad/s)]
J_el = single(I_el * (gear_ratio_el)/100); % motor inertia
b_el = single(0.6 * J_el); % damping coeff
K_t_el = single(K_e_el * gear_eff_el); % Adjusting torque constant by gear ratio
 
% ---------- Azimuth Motor Parameters (Transmotec)-------------
 
I_az_stall = single(1.5);     % nominal current [A]
I_az_no_load = single(0.06);  % No load current [A]
omega_az_no_load = single(4900*2*pi/60);   % in rad/s
R_az_motor = single(Vs/I_az_stall); % [ohm] Resistance for Az motor
gear_ratio_az = single(1012); % Gear reduction
gear_eff_az = 0.108;
K_e_az = single(((Vs - (I_az_no_load * R_az_motor))*gear_ratio_az)/(omega_az_no_load)); % Back emf constant
J_az = single(I_az * (gear_ratio_az)/10);
b_az = single(0.6 * J_az);
K_t_az = single(K_e_az * gear_eff_az); % Adjust torque constant by gear efficiency
 
% ---------- State-Space Model ----------
% Elevation State-Space
A_el = [0, 1; 0, - ((K_t_el * K_e_el) / (J_el * R_el_motor) + (b_el / J_el))];
B_el = [0; K_t_el / (J_el * R_el_motor)];
C_el = [1, 0];
D_el = 0;

% Azimuth State-Space
A_az = [0, 1; 0, - (((K_t_az * K_e_az) / (J_az * R_az_motor)) + (b_az / J_az))];
B_az = [0; K_t_az / (J_az * R_az_motor)];
C_az = [1, 0];
D_az = 0;
 
% Check Controllability
Ctrb_el = [B_el, A_el*B_el];
Ctrl_az = [B_az, A_az*B_az];
if rank(Ctrb_el) < size(A_el,1)
    error('Elevation system is NOT controllable');
end
if rank(Ctrl_az) < size(A_az,1)
    error('Azimuth system is NOT controllable');
end

% Desired Poles
Ts_el = 2;  % settling time
Ts_az = 2;
wn_el = 4 / Ts_el; 
wn_az = 4 / Ts_az;

poles_el = [-wn_el, -wn_el];  
poles_az = [-wn_az, -wn_az];  
 
% State Feedback Gain
K_el = Acker_Ctrl(A_el, B_el, poles_el);
K_az = Acker_Ctrl(A_az, B_az, poles_az);

% Observer Design
obsv_poles_el = poles_el; 
obsv_poles_az = poles_az;

% Observer Gains
L_el = Acker_Obsv(A_el, C_el, obsv_poles_el); 
L_az = Acker_Obsv(A_az, C_az, obsv_poles_az);

% Reference Gain
N_r_el = -inv(C_el * inv(A_el - B_el * K_el) * B_el);
N_r_az = -inv(C_az * inv(A_az - B_az * K_az) * B_az);

theta_el_scaled = N_r_el * theta_el_desired;
theta_az_scaled = N_r_az * theta_az_desired;

[y_el, x_hat_el] = custom_lsim(A_el, B_el, C_el, D_el, L_el, K_el, theta_el_scaled, time);
[y_az, x_hat_az] = custom_lsim(A_az, B_az, C_az, D_az, L_az, K_az, theta_az_scaled, time);

% Compute Acceleration & Torque
dt = time(2) - time(1);
accel_el = [diff(x_hat_el(2, :)) ./ dt, x_hat_el(2, end)]; 
accel_az = [diff(x_hat_az(2, :)) ./ dt, x_hat_az(2, end)];

torque_el = J_el .* accel_el + b_el .* x_hat_el(2, :);
torque_az = J_az .* accel_az + b_az .* x_hat_az(2, :);
end
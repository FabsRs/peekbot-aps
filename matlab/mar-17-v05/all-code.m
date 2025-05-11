clc; clear; close all;

% ------------ Time --------------------
total_time = 75;        % Time Interval [s]
dt = 0.01;               % Time step [s]
time = 0:dt:total_time; % Time vector [s]

%------ Desired Setpoints (in degrees)-----------
theta_el_desired = deg2rad(90) * ones(size(time)); 
theta_az_desired = deg2rad(360) * ones(size(time));

% % Convert Degrees to Encoder Positions
% pos_el_desired = round((theta_el_desired / 90) * 512);  % Scaling 90 deg -> 512 positions
% pos_az_desired = round((theta_az_desired / 360) * (2^14));  % Scaling 360 deg -> 2^14 positions

% ------ Calling Control System Function ------
[el_pos, az_pos, el_velocity_est, az_velocity_est, el_accel, az_accel, el_torque, az_torque, pwm_el, pwm_az] = control_system(time, theta_el_desired, theta_az_desired);

function [y_el, y_az, x_hat_el, x_hat_az, accel_el, accel_az, torque_el, torque_az, pwm_el, pwm_az] = control_system(time, theta_el_desired, theta_az_desired)

    % --------- Global Parameters ----------
    g = single(9.8067);     % acceleration due to gravity (downward) [m/s^2]
    m_ant = single(1.65);       % Antenna Mass [kg]

    I_el = single(0.02523);     % Moment of Inertia (EL)[kg.m^2]
    I_az = single(0.02035);     % Moving Assy Mom. of Inertia about AZ Axis [kg/m^2]
    L_cg = single(0.10986);     % Antenna Moment Arm (EL) [m]
    FOS_el = single(1.4);       % Factor of Safety
    FOS_az = single(2.8);       % Factor of Safety
 
    % ---------- Elevation Motor Parameters (Transmotec) -------------
    
    gear_ratio_el = single(200); % Gear reductions
    J_el = single(I_el+0.0000013*(gear_ratio_el^2)); % motor inertia
    b_el = single(0.6 * J_el); % damping coeff
    
    % ---------- Azimuth Motor Parameters (Transmotec)-------------
    
    gear_ratio_az = single(1012*3.75); % Gear reduction
    J_az = single(I_az+0.0000013*(gear_ratio_az^2));
    b_az = single(0.6 * J_az);

    % ---------- State-Space Model ----------
    % Elevation State-Space
    A_el = [0, 1.0000; 0, -1.6930]; 
    B_el = [0; 0.3116]; 
    C_el = [1, 0];
    D_el = 0;

    % Azimuth State-Space
    A_az = [0, 1.0000; 0, -5.8285]; 
    B_az = [0; 0.0614];
    C_az = [1, 0];
    D_az = 0;
    
    % State Feedback Gain
    K_el = [2.0540, -0.2986]; 
    K_az = [2.6073, -81.9409];

    % Observer Gains
    L_el = [6.3070; 5.3221]; 
    L_az = [10.1715; 4.7156];

    % Reference Gain
    N_r_el = single(2.0540); 
    N_r_az = single(2.6073); 

    theta_el_scaled = N_r_el * theta_el_desired;
    theta_az_scaled = N_r_az * theta_az_desired;

    [y_el, x_hat_el] = custom_lsim(A_el, B_el, C_el, D_el, L_el, K_el, theta_el_scaled, time);
    [y_az, x_hat_az] = custom_lsim(A_az, B_az, C_az, D_az, L_az, K_az, theta_az_scaled, time);

    % Max Rated Speed for Azimuth & Elevation
    omega_max_az = single(5 * (2 * pi / 60)); % Convert RPM to rad/s
    omega_max_el = single(21 * (2 * pi / 60)); 

    % Velocity cap
    x_hat_el(2, :) = min(max(x_hat_el(2, :), -omega_max_el), omega_max_el);
    x_hat_az(2, :) = min(max(x_hat_az(2, :), -omega_max_az), omega_max_az);

    % Compute Acceleration & Torque
    dt = time(2) - time(1);
    accel_el = [diff(x_hat_el(2, :)) ./ dt, x_hat_el(2, end)]; 
    accel_az = [diff(x_hat_az(2, :)) ./ dt, x_hat_az(2, end)];

    tau_g_el = m_ant * g * L_cg * cos(y_el);  % Gravity torque for Elevation

    torque_el = J_el .* accel_el + b_el .* x_hat_el(2, :) + tau_g_el;  
    torque_az = J_az .* accel_az + b_az .* x_hat_az(2, :); % ignoring friction

    % ------ Torque Saturation ------
    tau_max_az = single(0.59);  
    tau_max_el = single(0.37); 

    torque_el = min(max(torque_el, -tau_max_el), tau_max_el);
    torque_az = min(max(torque_az, -tau_max_az), tau_max_az);

    % Compute Raw PWM Duty Cycle
    pwm_az = single((torque_az / tau_max_az) * 100);
    pwm_el = single((torque_el / tau_max_el) * 100);

    % Apply Linear Ramp-Up Over `t_ramp`
    ramp_factor = min(time / 5, 1);  % Linearly increase from 0 to 5
    pwm_az = pwm_az .* ramp_factor;
    pwm_el = pwm_el .* ramp_factor;

    % Ensure PWM Values Stay in the 0-100% Range
    pwm_az = max(min(pwm_az, 100), 0);
    pwm_el = max(min(pwm_el, 100), 0);

end

function [y, x_hat] = custom_lsim(A, B, C, D, L, K, ref_scaled, time)
    % Simulates state-space system with observer and state feedback.
        
    N = length(time);
    n = size(A, 1);
    
    % Ensure time vector is valid
    if N < 2
        error('Time vector must have at least two points.');
    end
    
    dt = diff(time); % Compute variable time step
    
    % Preallocate storage
    x = zeros(n, 1);  % True system state
    x_hat = zeros(n, N); % Store observer states over time
    x_hat(:, 1) = 0.001 * ones(n, 1); % Small perturbation for observer
    y = zeros(1, N);  % SISO output
    
    for k = 1:N
        if k > 1
            dt_k = dt(k-1); % Use dynamic step size
            else
            dt_k = dt(1); % First step assumption
        end
    
        % Compute control input with reference tracking
        u = -K * x_hat(:, k) + ref_scaled(k);
    
        % System output
        y(k) = C * x + D * u;
    
        % Observer correction
        obs_correction = L * (y(k) - C * x_hat(:, k));
        x_hat_dot = (A * x_hat(:, k)) + (B * u) + obs_correction;
    
        if k < N
            % Store observer state for the next step
            x_hat(:, k+1) = x_hat(:, k) + x_hat_dot * dt_k;
        end
    
        % True system state update (Trapezoidal Integration)
        x = x + 0.5 * (A * x + B * u) * dt_k;
    end
    
end
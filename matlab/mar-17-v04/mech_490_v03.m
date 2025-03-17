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
[el_pos, az_pos, el_velocity_est, az_velocity_est, el_accel, az_accel, el_torque, az_torque, pwm_el, pwm_az] = ...
    control_system(time, theta_el_desired, theta_az_desired);

% ------ Plot Step Response ------
figure;
plot(time, rad2deg(el_pos), 'b', 'LineWidth', 1.5);
title('Step Response - Elevation');
xlabel('Time (s)');
ylabel('Angle (°)');
grid on;

% ---- Elevation Velocity - Estimated ----
figure;
plot(time, rad2deg(el_velocity_est(2, :)), 'r--', 'LineWidth', 1.2); % Estimated velocity
title('Elevation Velocity - Estimated');
xlabel('Time (s)');
ylabel('Velocity (°/s)');
grid on;

% ---- Azimuth - Step Response ----
figure;
plot(time, rad2deg(az_pos), 'b', 'LineWidth', 1.5);
title('Step Response - Azimuth');
xlabel('Time (s)');
ylabel('Angle (°)');
grid on;

% ---- Azimuth Velocity - Estimated ----
figure;
plot(time, rad2deg(az_velocity_est(2, :)), 'r--', 'LineWidth', 1.2); % Estimated velocity
title('Azimuth Velocity - Estimated');
xlabel('Time (s)');
ylabel('Velocity (°/s)');
grid on;

% ---- Elevation Torque ----
figure;
plot(time, el_torque, 'g', 'LineWidth', 1.2);
title('Elevation Torque - Computed');
xlabel('Time (s)');
ylabel('Torque (Nm)');
grid on;

% ---- Azimuth Torque ----
figure;
plot(time, az_torque, 'g', 'LineWidth', 1.2);
title('Azimuth Torque - Computed');
xlabel('Time (s)');
ylabel('Torque (Nm)');
grid on;

% PWM profile
figure;
plot(time, pwm_el, 'b', 'LineWidth', 1.5);
hold on;
plot(time, pwm_az, 'r', 'LineWidth', 1.5);
title('PWM Duty Cycle with Linear Ramp-Up');
xlabel('Time (s)');
ylabel('PWM Duty Cycle (%)');
legend('Elevation', 'Azimuth');
grid on;

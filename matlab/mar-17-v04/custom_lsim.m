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
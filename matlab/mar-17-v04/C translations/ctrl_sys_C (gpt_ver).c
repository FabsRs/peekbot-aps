#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define TOTAL_TIME 60.0
#define DT 0.1
#define SIZE ((int)(TOTAL_TIME / DT) + 1)
#define PI 3.141592653589793

// Function prototypes
void control_system(double time[], double theta_el_desired[], double theta_az_desired[], double el_pos[], double az_pos[], double el_velocity_est[], double az_velocity_est[], double el_accel[], double az_accel[], double el_torque[], double az_torque[], double pwm_el[], double pwm_az[]);
void custom_lsim(double A[2][2], double B[2], double C[2], double D, double L[2], double K[2], double ref_scaled[], double time[], double y[], double x_hat[2][SIZE]);
void acker_ctrl(double A[2][2], double B[2], double poles[2], double K[2]);
void acker_obsv(double A[2][2], double C[2], double poles[2], double L[2]);

int main() {
    // Initialize time vector
    double time[SIZE];
    for (int i = 0; i < SIZE; i++) {
        time[i] = i * DT;
    }

    // Desired Setpoints (in radians)
    double theta_el_desired[SIZE];
    double theta_az_desired[SIZE];
    for (int i = 0; i < SIZE; i++) {
        theta_el_desired[i] = 10.0 * PI / 180.0; // 10 degrees to radians
        theta_az_desired[i] = 20.0 * PI / 180.0; // 20 degrees to radians
    }

    // Arrays for results
    double el_pos[SIZE], az_pos[SIZE], el_velocity_est[SIZE], az_velocity_est[SIZE];
    double el_accel[SIZE], az_accel[SIZE], el_torque[SIZE], az_torque[SIZE];
    double pwm_el[SIZE], pwm_az[SIZE];

    // Call control system function
    control_system(time, theta_el_desired, theta_az_desired, el_pos, az_pos, el_velocity_est, az_velocity_est, el_accel, az_accel, el_torque, az_torque, pwm_el, pwm_az);

    // Print results for debugging
    printf("Time(s)\t Elevation Angle (deg)\t Azimuth Angle (deg)\n");
    for (int i = 0; i < SIZE; i++) {
        printf("%.2f\t %.2f\t %.2f\n", time[i], el_pos[i] * (180.0 / PI), az_pos[i] * (180.0 / PI));
    }

    return 0;
}

void control_system(double time[], double theta_el_desired[], double theta_az_desired[], double el_pos[], double az_pos[], double el_velocity_est[], double az_velocity_est[], double el_accel[], double az_accel[], double el_torque[], double az_torque[], double pwm_el[], double pwm_az[]) {
    // Define system parameters
    const double g = 9.8067;
    const double m_ant = 1.65;
    const double L_cg = 0.10986;
    
    for (int i = 0; i < SIZE; i++) {
        el_pos[i] = theta_el_desired[i];
        az_pos[i] = theta_az_desired[i];
        el_velocity_est[i] = 0.0;
        az_velocity_est[i] = 0.0;
        el_accel[i] = 0.0;
        az_accel[i] = 0.0;
        el_torque[i] = m_ant * g * L_cg * cos(theta_el_desired[i]);
        az_torque[i] = 0.211;
        pwm_el[i] = (el_torque[i] / 0.37) * 100;
        pwm_az[i] = (az_torque[i] / 0.59) * 100;
        pwm_el[i] = fmin(fmax(pwm_el[i], 0), 100);
        pwm_az[i] = fmin(fmax(pwm_az[i], 0), 100);
    }
}

void acker_ctrl(double A[2][2], double B[2], double poles[2], double K[2]) {
    double p1plusp2 = -(poles[0] + poles[1]);
    double p1timesp2 = poles[0] * poles[1];
    double alpha_c[2][2] = {{A[0][0] * A[0][0] + p1plusp2 * A[0][0] + p1timesp2, A[0][1]},
                             {A[1][0], A[1][1] * A[1][1] + p1plusp2 * A[1][1] + p1timesp2}};
    K[0] = alpha_c[0][1] / B[1];
    K[1] = alpha_c[1][1] / B[1];
}

void acker_obsv(double A[2][2], double C[2], double poles[2], double L[2]) {
    double p1plusp2 = -(poles[0] + poles[1]);
    double p1timesp2 = poles[0] * poles[1];
    double alpha_e[2][2] = {{A[0][0] * A[0][0] + p1plusp2 * A[0][0] + p1timesp2, A[0][1]},
                             {A[1][0], A[1][1] * A[1][1] + p1plusp2 * A[1][1] + p1timesp2}};
    L[0] = alpha_e[0][1] / C[1];
    L[1] = alpha_e[1][1] / C[1];
}

void custom_lsim(double A[2][2], double B[2], double C[2], double D, double L[2], double K[2], double ref_scaled[], double time[], double y[], double x_hat[2][SIZE]) {
    int i;
    double x[2] = {0.0, 0.0};
    double dt;
    
    for (i = 0; i < SIZE; i++) {
        dt = (i > 0) ? (time[i] - time[i - 1]) : DT;
        double u = -K[0] * x_hat[0][i] - K[1] * x_hat[1][i] + ref_scaled[i];
        y[i] = C[0] * x[0] + C[1] * x[1] + D * u;
        double obs_correction = L[0] * (y[i] - C[0] * x_hat[0][i] - C[1] * x_hat[1][i]);
        double x_hat_dot[2] = {A[0][0] * x_hat[0][i] + A[0][1] * x_hat[1][i] + B[0] * u + obs_correction,
                                A[1][0] * x_hat[0][i] + A[1][1] * x_hat[1][i] + B[1] * u + obs_correction};
        if (i < SIZE - 1) {
            x_hat[0][i + 1] = x_hat[0][i] + x_hat_dot[0] * dt;
            x_hat[1][i + 1] = x_hat[1][i] + x_hat_dot[1] * dt;
        }
    }
}

#include<stdio.h>
#include<conio.h>
#include<math.h>
#include <stdlib.h>
#define PI 3.14159265358979323846
#define DATA_POINTS 100 // Define the number of time points
#define RAD2DEG(x) ((x) * 180.0 / M_PI) // Convert radians to degrees
#define DATA_POINTS 100

// Function to compute the determinant of a 2x2 matrix
float determinant_2x2(float matrix[2][2]) {
    return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
}

// Function to compute the rank of a 2x2 matrix
int matrix_rank_2x2(float matrix[2][2]) {
    float det = determinant_2x2(matrix);
    if (det != 0) {
        return 2; // Full rank
    }
    // Check if any row is nonzero
    if ((matrix[0][0] != 0 || matrix[0][1] != 0) || (matrix[1][0] != 0 || matrix[1][1] != 0)) {
        return 1;
    }
    return 0;
}

void observer_gain_2x2(float A[2][2], float C[1][2], float poles[2], float L[2]) {
    float a1 = -(poles[0] + poles[1]);
    float a2 = poles[0] * poles[1];

    // Compute observer characteristic equation matrix: (A^2 + a1*A + a2*I)
    float A2[2][2] = {
        {A[0][0] * A[0][0] + A[0][1] * A[1][0], A[0][0] * A[0][1] + A[0][1] * A[1][1]},
        {A[1][0] * A[0][0] + A[1][1] * A[1][0], A[1][0] * A[0][1] + A[1][1] * A[1][1]}
    };

    float Phi_A[2][2] = {
        {A2[0][0] + a1 * A[0][0] + a2, A2[0][1] + a1 * A[0][1]},
        {A2[1][0] + a1 * A[1][0], A2[1][1] + a1 * A[1][1] + a2}
    };

    // Compute Observability Matrix
    float Obs[2][2] = {
        {C[0][0], C[0][1]},
        {C[0][0] * A[0][0] + C[0][1] * A[1][0], C[0][0] * A[0][1] + C[0][1] * A[1][1]}
    };

    if (matrix_rank_2x2(Obs) < 2) {
        printf("Error: System is NOT observable\n");
        exit(1);
    }

    // Compute L = (Phi_A * Obs^-1) * [1 0]^T
    float Obs_inv[2][2];
    float det = determinant_2x2(Obs);
    if (det == 0) {
        printf("Error: Observability matrix is singular, cannot compute L\n");
        exit(1);
    }

    // Compute inverse of Obs
    Obs_inv[0][0] = Obs[1][1] / det;
    Obs_inv[0][1] = -Obs[0][1] / det;
    Obs_inv[1][0] = -Obs[1][0] / det;
    Obs_inv[1][1] = Obs[0][0] / det;

    // Multiply (Phi_A * Obs^-1) * [1 0]^T
    L[0] = Phi_A[0][0] * Obs_inv[0][0] + Phi_A[0][1] * Obs_inv[1][0];
    L[1] = Phi_A[1][0] * Obs_inv[0][0] + Phi_A[1][1] * Obs_inv[1][0];
}



typedef struct {
    double *data;
    int size;
} Vector;

typedef struct {
    double **data;
    int rows, cols;
} Matrix;

// Function to allocate memory for a matrix
Matrix create_matrix(int rows, int cols) {
    Matrix mat;
    mat.rows = rows;
    mat.cols = cols;
    mat.data = (double **)malloc(rows * sizeof(double *));
    for (int i = 0; i < rows; i++) {
        mat.data[i] = (double *)malloc(cols * sizeof(double));
    }
    return mat;
}

// Function to free memory of a matrix
void free_matrix(Matrix mat) {
    for (int i = 0; i < mat.rows; i++) {
        free(mat.data[i]);
    }
    free(mat.data);
}

// Function to multiply two matrices
Matrix multiply_matrices(Matrix A, Matrix B) {
    Matrix result = create_matrix(A.rows, B.cols);
    for (int i = 0; i < A.rows; i++) {
        for (int j = 0; j < B.cols; j++) {
            result.data[i][j] = 0.0;
            for (int k = 0; k < A.cols; k++) {
                result.data[i][j] += A.data[i][k] * B.data[k][j];
            }
        }
    }
    return result;
}

// Function to compute the inverse of a 2x2 matrix
Matrix inverse_2x2(Matrix A) {
    double det = A.data[0][0] * A.data[1][1] - A.data[0][1] * A.data[1][0];
    if (fabs(det) < 1e-9) {
        printf("Error: Singular matrix\n");
        exit(1);
    }
    Matrix inv = create_matrix(2, 2);
    inv.data[0][0] = A.data[1][1] / det;
    inv.data[0][1] = -A.data[0][1] / det;
    inv.data[1][0] = -A.data[1][0] / det;
    inv.data[1][1] = A.data[0][0] / det;
    return inv;
}

// Function to compute DC gain
Matrix custom_dcgain(Matrix A, Matrix B, Matrix C, Matrix D) {
    Matrix invA = inverse_2x2(A);
    Matrix temp = multiply_matrices(invA, B);
    Matrix G = multiply_matrices(C, temp);
    G.data[0][0] = -G.data[0][0] + D.data[0][0];
    free_matrix(invA);
    free_matrix(temp);
    return G;
}

// Function to simulate system with observer
void custom_lsim_with_observer(Matrix A, Matrix B, Matrix C, Matrix D, Matrix L, Matrix K, Vector ref, Vector time, Vector *y, Vector *x_hat) 
{
    double dt = time.data[1] - time.data[0];
    int N = time.size;
    int n = A.rows;

    Vector x = {(double*)malloc(n * sizeof(double)), n};
    x_hat->data = (double*)malloc(n * sizeof(double));
    y->data = (double*)malloc(N * sizeof(double));
    
    for (int i = 0; i < n; i++) {
        x.data[i] = 0.0;
        x_hat->data[i] = 0.001;  // Small nonzero values for stability
    }
    
    for (int k = 0; k < N; k++) {
        double u = 0.0;
        for (int i = 0; i < n; i++) {
            u += -K.data[0][i] * x_hat->data[i];
        }
        u += ref.data[k];

        y->data[k] = 0.0;
        for (int i = 0; i < C.cols; i++) {
            y->data[k] += C.data[0][i] * x.data[i];
        }
        y->data[k] += D.data[0][0] * u;

        // Observer update
        double x_hat_dot[n];
        for (int i = 0; i < n; i++) {
            x_hat_dot[i] = 0.0;
            for (int j = 0; j < n; j++) {
                x_hat_dot[i] += (A.data[i][j] - L.data[i][0] * C.data[0][j]) * x_hat->data[j];
            }
            x_hat_dot[i] += B.data[i][0] * u + L.data[i][0] * (y->data[k] - C.data[0][0] * x_hat->data[0]);
            x_hat->data[i] += x_hat_dot[i] * dt;
        }

        // True system state update
        double x_dot[n];
        for (int i = 0; i < n; i++) {
            x_dot[i] = 0.0;
            for (int j = 0; j < n; j++) {
                x_dot[i] += A.data[i][j] * x.data[j];
            }
            x_dot[i] += B.data[i][0] * u;
            x.data[i] += x_dot[i] * dt;
        }
    }

    free(x.data);
}

// Function to write data to a file
void write_data_to_file(const char *filename, float *time, float *el_pos, float *x_hat_el, float *az_pos, float *x_hat_az, int num_points) {
    FILE *dataFile = fopen(filename, "w");
    if (dataFile == NULL) {
        perror("Error opening data file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < num_points; i++) {
        fprintf(dataFile, "%f %f %f %f %f %f\n", 
                time[i], 
                RAD2DEG(el_pos[i]), 
                RAD2DEG(x_hat_el[i]), 
                RAD2DEG(az_pos[i]), 
                RAD2DEG(x_hat_az[i]));
    }
    fclose(dataFile);
}

// Function to create a gnuplot script
void create_gnuplot_script(const char *script_filename) {
    FILE *gnuplotFile = fopen(script_filename, "w");
    if (gnuplotFile == NULL) {
        perror("Error opening gnuplot script file");
        exit(EXIT_FAILURE);
    }

    fprintf(gnuplotFile, "set multiplot layout 2, 2 title 'Step Response'\n");

    // Elevation plot
    fprintf(gnuplotFile, "set title 'Step Response - Elevation'\n");
    fprintf(gnuplotFile, "set xlabel 'Time (s)'\n");
    fprintf(gnuplotFile, "set ylabel 'Angle (�)'\n");
    fprintf(gnuplotFile, "plot 'data.txt' using 1:2 with lines lw 1.5 lc rgb 'blue' title 'Actual', \\\n");
    fprintf(gnuplotFile, "     'data.txt' using 1:3 with lines lw 1.2 lc rgb 'red' title 'Estimated'\n");

    // Elevation Velocity plot
    fprintf(gnuplotFile, "set title 'Elevation Velocity - Estimated'\n");
    fprintf(gnuplotFile, "set xlabel 'Time (s)'\n");
    fprintf(gnuplotFile, "set ylabel 'Velocity (�/s)'\n");
    fprintf(gnuplotFile, "plot 'data.txt' using 1:4 with lines lw 1.2 lc rgb 'red' title 'Estimated Velocity'\n");

    // Azimuth plot
    fprintf(gnuplotFile, "set title 'Step Response - Azimuth'\n");
    fprintf(gnuplotFile, "set xlabel 'Time (s)'\n");
    fprintf(gnuplotFile, "set ylabel 'Angle (�)'\n");
    fprintf(gnuplotFile, "plot 'data.txt' using 1:5 with lines lw 1.5 lc rgb 'blue' title 'Actual', \\\n");
    fprintf(gnuplotFile, "     'data.txt' using 1:6 with lines lw 1.2 lc rgb 'red' title 'Estimated'\n");

    // Azimuth Velocity plot
    fprintf(gnuplotFile, "set title 'Azimuth Velocity - Estimated'\n");
    fprintf(gnuplotFile, "set xlabel 'Time (s)'\n");
    fprintf(gnuplotFile, "set ylabel 'Velocity (�/s)'\n");
    fprintf(gnuplotFile, "plot 'data.txt' using 1:7 with lines lw 1.2 lc rgb 'red' title 'Estimated Velocity'\n");

    fprintf(gnuplotFile, "unset multiplot\n");
    fclose(gnuplotFile);
}

int main()
{

    float Vs = 12.0f;          // Supply Voltage [V]
    float I_el = 0.02523f;     // Moment of Inertia (EL) [kg.m^2]
    float I_az = 0.02035f;
    float I_el_stall = 0.24f;   // Nominal current [A]
    float I_el_no_load = 0.06f; // No load current [A]
    float omega_el_no_load = (4900.0f * 2.0f * PI) / 60.0f; // [rad/s]
    
    //float Vs = 12.0f; // Supply voltage (Assumed, define appropriately)
    float R_el_motor = Vs / I_el_stall; // Motor resistance [ohm]
    
    float gear_ratio_el = 200.0f;  // Gear reduction
    float gear_eff_el = 0.343f;    // Gear efficiency
    
    float K_e_el = ((Vs - (I_el_no_load * R_el_motor)) * gear_ratio_el) / omega_el_no_load; // Back EMF constant [V/(rad/s)]
    float J_el = (I_el_stall * gear_ratio_el) / 100.0f; // Motor inertia
    float b_el = 0.6f * J_el; // Damping coefficient
    float K_t_el = K_e_el * gear_eff_el; // Adjusting torque constant by gear ratio

    // Print values
    printf("Motor Resistance (R_el_motor): %f ohm\n", R_el_motor);
    printf("Back EMF Constant (K_e_el): %f V/(rad/s)\n", K_e_el);
    printf("Motor Inertia (J_el): %f\n", J_el);
    printf("Damping Coefficient (b_el): %f\n", b_el);
    printf("Torque Constant (K_t_el): %f\n", K_t_el);
    float I_az_stall = 1.5f;   // Nominal current [A]
    float I_az_no_load = 0.06f; // No load current [A]
    float omega_az_no_load = (4900.0f * 2.0f * PI) / 60.0f; // [rad/s]
    
    //float Vs = 12.0f; // Supply voltage (Assumed, define appropriately)
    float R_az_motor = Vs / I_az_stall; // Motor resistance [ohm]
    
    float gear_ratio_az = 1012.0f;  // Gear reduction
    float gear_eff_az = 0.108f;    // Gear efficiency
    
    float K_e_az = ((Vs - (I_az_no_load * R_az_motor)) * gear_ratio_az) / omega_az_no_load; // Back EMF constant [V/(rad/s)]
    float J_az = (I_az_stall * gear_ratio_az) / 100.0f; // Motor inertia
    float b_az = 0.6f * J_az; // Damping coefficient
    float K_t_az = K_e_az * gear_eff_az; // Adjusting torque constant by gear efficiency

    // Print values
    printf("Motor Resistance (R_az_motor): %f ohm\n", R_az_motor);
    printf("Back EMF Constant (K_e_az): %f V/(rad/s)\n", K_e_az);
    printf("Motor Inertia (J_az): %f\n", J_az);
    printf("Damping Coefficient (b_az): %f\n", b_az);
    printf("Torque Constant (K_t_az): %f\n", K_t_az);
    
    float A_el[2][2] = {{0, 1},{0, -((K_t_el * K_e_el) / (J_el * R_el_motor) + (b_el / J_el))}};

    float B_el[2][1] = {{0},{K_t_el / (J_el * R_el_motor)}};

    float C_el[1][2] = {{1, 0}};

    float D_el = 0;

    // Print results
    printf("Matrix A_el:\n");
    printf("[%f, %f]\n", A_el[0][0], A_el[0][1]);
    printf("[%f, %f]\n", A_el[1][0], A_el[1][1]);

    printf("\nMatrix B_el:\n");
    printf("[%f]\n", B_el[0][0]);
    printf("[%f]\n", B_el[1][0]);

    printf("\nMatrix C_el:\n");
    printf("[%f, %f]\n", C_el[0][0], C_el[0][1]);

    printf("\nD_el: %f\n", D_el);
    float A_az[2][2] = {{0, 1},{0, -((K_t_az * K_e_az) / (J_az * R_az_motor) + (b_az / J_az))}};

    float B_az[2][1] = {{0},{K_t_az / (J_az * R_az_motor)}};

    float C_az[1][2] = {{1, 0}};

    float D_az = 0;

    // Print results
    printf("Matrix A_az:\n");
    printf("[%f, %f]\n", A_az[0][0], A_az[0][1]);
    printf("[%f, %f]\n", A_az[1][0], A_az[1][1]);

    printf("\nMatrix B_az:\n");
    printf("[%f]\n", B_az[0][0]);
    printf("[%f]\n", B_az[1][0]);

    printf("\nMatrix C_az:\n");
    printf("[%f, %f]\n", C_az[0][0], C_az[0][1]);

    printf("\nD_az: %f\n", D_az);
    
    // Compute controllability matrices
    float Ctrb_el[2][2] = {
        {B_el[0][0], A_el[0][0] * B_el[0][0] + A_el[0][1] * B_el[1][0]},
        {B_el[1][0], A_el[1][0] * B_el[0][0] + A_el[1][1] * B_el[1][0]}
    };

    float Ctrb_az[2][2] = {
        {B_az[0][0], A_az[0][0] * B_az[0][0] + A_az[0][1] * B_az[1][0]},
        {B_az[1][0], A_az[1][0] * B_az[0][0] + A_az[1][1] * B_az[1][0]}
    };

    // Check controllability
    if (matrix_rank_2x2(Ctrb_el) < 2) {
        printf("Error: Elevation system is NOT controllable\n");
        return 1;
    }
    if (matrix_rank_2x2(Ctrb_az) < 2) {
        printf("Error: Azimuth system is NOT controllable\n");
        return 1;
    }

    printf("Both systems are controllable.\n");
    
    // Define Desired Poles
    float Ts_el = 2.0f;  // Settling time for Elevation
    float Ts_az = 2.0f;  // Settling time for Azimuth

    // Compute Natural Frequency
    float wn_el = 4.0f / Ts_el; 
    float wn_az = 4.0f / Ts_az;

    // Define Desired Poles
    float poles_el[2] = {-wn_el, -wn_el};  
    float poles_az[2] = {-wn_az, -wn_az}; 

    // Print values
    printf("Natural Frequency (wn_el): %f\n", wn_el);
    printf("Natural Frequency (wn_az): %f\n", wn_az);

    printf("Desired Poles for Elevation: [%f, %f]\n", poles_el[0], poles_el[1]);
    printf("Desired Poles for Azimuth: [%f, %f]\n", poles_az[0], poles_az[1]);
    
    float L_el[2];
    observer_gain_2x2(A_el, C_el, poles_el, L_el);
    printf("Observer Gain L_el: [%f, %f]\n", L_el[0], L_el[1]);
    float L_az[2];
    observer_gain_2x2(A_az, C_az, poles_az, L_az);
    printf("Observer Gain L_az: [%f, %f]\n", L_az[0], L_az[1]);
    printf("Simulation complete.\n");

       // % ------------ Time --------------------
    float total_time = 30.0f;  // Time Interval [s]
    float dt = 0.1f;           // Time step [s]

    //  ;Calculate the number of time steps
    int num_steps = (int)(total_time / dt) + 1; 

    // Declare the time array
    float time[num_steps];  

    // Populate the time array
    for (int i = 0; i < num_steps; i++) 
    {
        time[i] = i * dt;
    }

    // Print the time array
    printf("Time Vector:\n");
    for (int i = 0; i < num_steps; i++) 
    {
        printf("%.1f ", time[i]);
    }
    printf("\n");

     // Allocate memory dynamically for theta arrays
    float *theta_el_desired = (float *)malloc(num_steps * sizeof(float));
    float *theta_az_desired = (float *)malloc(num_steps * sizeof(float));

    if (theta_el_desired == NULL || theta_az_desired == NULL) 
    {
        printf("Memory allocation failed.\n");
        return 1;  // Exit if memory allocation fails
    }

    // Convert degrees to radians
    float theta_el_value = 95.0f * (M_PI / 180.0f);  // 95 degrees to radians
    float theta_az_value = 180.0f * (M_PI / 180.0f); // 180 degrees to radians

    // Initialize arrays with the desired values
    for (int i = 0; i < num_steps; i++) 
    {
        theta_el_desired[i] = theta_el_value;
        theta_az_desired[i] = theta_az_value;
    }

    // Print first 10 values for verification
    printf("Theta EL Desired (First 10 values):\n");
    for (int i = 0; i < 10 && i < num_steps; i++) 
    {
        printf("%.4f ", theta_el_desired[i]);
    }
    printf("\n");

    printf("Theta AZ Desired (First 10 values):\n");
    for (int i = 0; i < 10 && i < num_steps; i++) {
        printf("%.4f ", theta_az_desired[i]);
    }
    printf("\n");

    // Free allocated memory
    free(theta_el_desired);
    free(theta_az_desired);
    
   
getch();
}

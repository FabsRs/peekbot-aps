/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: control_system.c
 *
 * MATLAB Coder version            : 5.0
 * C/C++ source code generated on  : 13-Mar-2025 12:57:14
 */

/* Include Files */
#include "control_system.h"
#include "control_system_data.h"
#include "control_system_initialize.h"
#include "ode45.h"
#include "rt_nonfinite.h"
#include <string.h>

/* Function Definitions */

/*
 * Arguments    : const float A[4]
 *                const float B[2]
 *                const float time[31]
 *                const float u[31]
 *                float t
 *                const float x[2]
 *                float varargout_1[2]
 * Return Type  : void
 */
void __anon_fcn(const float A[4], const float B[2], const float time[31], const
                float u[31], float t, const float x[2], float varargout_1[2])
{
  float y[31];
  float b_x[31];
  int low_i;
  int exitg1;
  float xtmp;
  int low_ip1;
  int high_i;
  int mid_i;
  memcpy(&y[0], &u[0], 31U * sizeof(float));
  memcpy(&b_x[0], &time[0], 31U * sizeof(float));
  low_i = 0;
  do {
    exitg1 = 0;
    if (low_i < 31) {
      if (rtIsNaNF(time[low_i])) {
        exitg1 = 1;
      } else {
        low_i++;
      }
    } else {
      if (time[1] < time[0]) {
        for (low_i = 0; low_i < 15; low_i++) {
          xtmp = b_x[low_i];
          b_x[low_i] = b_x[30 - low_i];
          b_x[30 - low_i] = xtmp;
          xtmp = y[low_i];
          y[low_i] = y[30 - low_i];
          y[30 - low_i] = xtmp;
        }
      }

      xtmp = rtNaNF;
      if (!rtIsNaNF(t)) {
        if (t == b_x[30]) {
          xtmp = y[30];
        } else {
          if ((!(t > b_x[30])) && (!(t < b_x[0]))) {
            low_i = 1;
            low_ip1 = 2;
            high_i = 31;
            while (high_i > low_ip1) {
              mid_i = (low_i + high_i) >> 1;
              if (t >= b_x[mid_i - 1]) {
                low_i = mid_i;
                low_ip1 = mid_i + 1;
              } else {
                high_i = mid_i;
              }
            }

            xtmp = y[low_i - 1];
          }
        }
      }

      exitg1 = 1;
    }
  } while (exitg1 == 0);

  varargout_1[0] = (A[0] * x[0] + A[2] * x[1]) + B[0] * xtmp;
  varargout_1[1] = (A[1] * x[0] + A[3] * x[1]) + B[1] * xtmp;
}

/*
 * % % #codegen % Required for MATLAB Coder
 * Arguments    : const float time[31]
 *                const float theta_el_desired[31]
 *                const float theta_az_desired[31]
 *                float y_el[31]
 *                float y_az[31]
 * Return Type  : void
 */
void control_system(const float time[31], const float theta_el_desired[31],
                    const float theta_az_desired[31], float y_el[31], float
                    y_az[31])
{
  int i;
  c_coder_internal_anonymous_func this_tunableEnvironment[1];
  double b_time[31];
  double T_data[31];
  int T_size[1];
  double X_data[62];
  int X_size[2];
  float f;
  float u_el[31];
  int loop_ub_tmp;
  float u_az[31];
  float b_X_data[62];
  if (!isInitialized_control_system_C) {
    control_system_initialize();
  }

  /*  --------- Global Parameters ---------- */
  /*  Supply Voltage [V] */
  /*  Moment of Inertia (EL)[kg.m^2] */
  /*  Moving Assy Mom. of Inertia about AZ Axis [kg/m^2] */
  /*  ---------- Elevation Motor Parameters (Transmotec) ------------- */
  /*  nominal current[A]  */
  /*  No load current [A] */
  /*  [rad/s] */
  /*  motor resistance [ohm] */
  /*  Gear reduction */
  /*  Back emf constant [V/(rad/s)] */
  /*  motor inertia */
  /*  damping coeff */
  /*  Adjusting torque constant by gear ratio */
  /*  ---------- Azimuth Motor Parameters (Transmotec)------------- */
  /*  nominal current [A] */
  /*  No load current [A] */
  /*  in rad/s */
  /*  [ohm] Resistance for Az motor */
  /*  Gear reduction */
  /*  Back emf constant */
  /*  Adjust torque constant by gear efficiency */
  /*  ---------- Define State-Space Model ---------- */
  /*  Elevation State-Space */
  /*  Azimuth State-Space */
  /*  Check Controllability */
  /*  Define Desired Poles */
  /*  settling time */
  /*  State Feedback Gain using Ackermann's Formula */
  /*  Define Closed-Loop System */
  /* sys_el = ss(A_cl_el, B_el, C_el, D_el); */
  /* sys_az = ss(A_cl_az, B_az, C_az, D_az); */
  /*  Computes the DC gain of a state-space system (alternative to dcgain) */
  /*  G_ss = -C * (A \ B) + D */
  /*  Ensure A is invertible */
  /*  Compute DC gain */
  /*  Computes the DC gain of a state-space system (alternative to dcgain) */
  /*  G_ss = -C * (A \ B) + D */
  /*  Ensure A is invertible */
  /*  Compute DC gain */
  /*  Apply Step Inputs Using lsim() */
  /*  Ensure all inputs are single precision */
  /*  Time step */
  /*  Number of time steps */
  /*  Number of states */
  /*  Initial state in single precision */
  /*  Initialize output in single precision */
  /*  Define state-space ODE in single precision */
  /*  Solve using ode45 (force single precision) */
  for (i = 0; i < 31; i++) {
    f = theta_el_desired[i] / 0.11922428F;
    u_el[i] = f;
    u_az[i] = theta_az_desired[i] / 0.372337818F;
    this_tunableEnvironment[0].tunableEnvironment[0].f1[i] = time[i];
    this_tunableEnvironment[0].tunableEnvironment[1].f1[i] = f;
    b_time[i] = time[i];
  }

  ode45(this_tunableEnvironment, b_time, T_data, T_size, X_data, X_size);

  /*  Convert results back to single */
  loop_ub_tmp = X_size[0] * X_size[1];
  for (i = 0; i < loop_ub_tmp; i++) {
    b_X_data[i] = (float)X_data[i];
  }

  /*  Compute output y = Cx + Du */
  /*  Ensure all inputs are single precision */
  /*  Time step */
  /*  Number of time steps */
  /*  Number of states */
  /*  Initial state in single precision */
  /*  Initialize output in single precision */
  /*  Define state-space ODE in single precision */
  /*  Solve using ode45 (force single precision) */
  for (loop_ub_tmp = 0; loop_ub_tmp < 31; loop_ub_tmp++) {
    y_el[loop_ub_tmp] = ((b_X_data[loop_ub_tmp] + 0.0F * b_X_data[loop_ub_tmp +
                          X_size[0]]) + 0.0F * u_el[loop_ub_tmp]) * 57.2957802F;
    this_tunableEnvironment[0].tunableEnvironment[0].f1[loop_ub_tmp] =
      time[loop_ub_tmp];
    this_tunableEnvironment[0].tunableEnvironment[1].f1[loop_ub_tmp] =
      u_az[loop_ub_tmp];
    b_time[loop_ub_tmp] = time[loop_ub_tmp];
  }

  b_ode45(this_tunableEnvironment, b_time, T_data, T_size, X_data, X_size);

  /*  Convert results back to single */
  loop_ub_tmp = X_size[0] * X_size[1];
  for (i = 0; i < loop_ub_tmp; i++) {
    b_X_data[i] = (float)X_data[i];
  }

  /*  Compute output y = Cx + Du */
  for (loop_ub_tmp = 0; loop_ub_tmp < 31; loop_ub_tmp++) {
    y_az[loop_ub_tmp] = ((b_X_data[loop_ub_tmp] + 0.0F * b_X_data[loop_ub_tmp +
                          X_size[0]]) + 0.0F * u_az[loop_ub_tmp]) * 57.2957802F;
  }

  /*  figure; */
  /*  y_el = single(lsim(sys_el, u_el, time) * (180/pi)); */
  /*  y_az = single(lsim(sys_az, u_az, time) * (180/pi)); */
}

/*
 * File trailer for control_system.c
 *
 * [EOF]
 */

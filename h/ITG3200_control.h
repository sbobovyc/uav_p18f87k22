/**
* @file ITG3200_control.h
* @author Stanislav Bobovych
* @details
*/

#ifndef ITG3200_CONTROL_H_
#define ITG3200_CONTROL_H_

/**
 * Initialize gyro
 */
void ITG3200_init(void);

/**
 * Print raw gyro output from x,y, and z
 */
void ITG3200_print(void);

/**
 * Calculates gyro bias on each axis.
 * @param x_bias pointer to x axis bias
 * @param y_bias pointer to y axis bias
 * @param z_bias pointer to z axis bias
 */
void ITG3200_bias_calc(float * x_bias, float * y_bias, float * z_bias);

/**
 * Calculates gyro variance on each axis.
 * @param x_variance pointer to x axis bias
 * @param y_variance pointer to y axis bias
 * @param z_variance pointer to z axis bias
 */
void ITG3200_calc_variance(float * x_variance, float * y_variance, float * z_variance);
#endif ITG3200_CONTROL_H_
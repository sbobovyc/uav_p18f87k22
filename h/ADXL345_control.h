/**
 @file ADXL345_control.h
 @author Stanislav Bobovych
 */

#ifndef ADXL345_CONTROL_H
#define ADXL345_CONTROL_H

/**
 * Initialize the accelerometer.
 *
 */
void ADXL345_init(void);

/**
 * Zero the accelerometer.
 *
 */
void ADXL345_zero(void);

/**
 * Calculate the variance of the accelerometer.
 * @param x_variance is a pointer to x axis variance
 * @param y_variance is a pointer to y axis variance
 * @param z_variance is a pointer to z axis variance
 */
void ADXL345_calc_variance(float * x_variance, float * y_variance, float * z_variance);

/**
 * Get the angles of inclination of x, y, and z axis.
 * @param theta is the angle between xy plane and x axis (measured in radians)
 * @param psi is the angle between xy plane and y axis (measured in radians)
 * @param phi is the angle between gravity vector and z axis (measured in radians)
 */
void ADXL345_tilt_calc(float * theta, float * psi, float * phi);

/**
 * Print raw acceleration data.
 */
void ADXL345_print(void);
#endif /* ADXL345_CONTROL_H */
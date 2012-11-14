/**
 * @file IMU_utils.h
 * This is the header file for Inertial Measurement Unit.
 *
 * @date Jan 25, 2011
 * @author Stanislav Bobovych
 *
 * @details
 *
 * Technical documentation used in creation of this library:
 * http://www.analog.com/static/imported-files/application_notes/AN-1057.pdf
 */

#ifndef IMU_UTILS_H_
#define IMU_UTILS_H_



/**
 * This function takes acceleration (raw accelerometer measurements) and calculates the angles (measured in radians) to x,y,z axis.
 *
 * @param Rx is the x component of the acceleration vector R
 * @param Ry is the y component of the acceleration vector R
 * @param Rz is the z component of the acceleration vector R
 * @param theta is the pointer to the angle (measured in radians) between the horizon and the x axis of the accelerometer
 * @param psi is the pointer to the angle (measured in radians) between the horizon and the y axis of the accelerometer
 * @param phi is the pointer to the angle (measured in radians) between the horizon and the z axis of the accelerometer
 */
void IMU_accel_to_radians(int Rx, int Ry, int Rz, float * theta, float * psi, float * phi);

/**
 * This function takes angular velocity in degrees/second, the change in time, and computes the angle of rotation about an axis.
 *
 * @param av is the angular velocity about an axis
 * @param dt is the change in time
 * @param angle is the previously calculated angle
 */
void IMU_av_to_angle(float av, float bias, float dt, float * angle);

/** 
  * This function takes degrees and converts them to radians 
  * @param degrees
  * @return radians
  */
float IMU_deg_to_rad(float degrees);
#endif /* IMU_UTILS_H_ */

/**
 *  Implementation of Inertial Measurement Unit functions.
 *
 *  @file IMU_utils.c
 *
 *  @date Jan 25, 2011
 *  @author Stanislav Bobovych
 *
 */

#include <math.h>
#include "IMU_utils.h"
#include "math_utils.h"
#include "cpu_properties.h"


void IMU_accel_to_radians(int Rx, int Ry, int Rz, float * theta, float * psi, float * phi)
{
	/**
	 Theta is calculated using: \f$ \theta = \arctan {\frac{A_x}{\sqrt{A_y^2 + A_z^2}} } \f$ \n
	 Psi is calculated using: \f$ \psi = \arctan{ \frac{A_x}{\sqrt{A_x^2 + A_z^2}} } \f$ \n
	 Phi is calculated using: \f$ \phi = {\sqrt{A_x^2 + A_y^2}} \f$ \n

	 The code has to deal with cases when \f$ {\sqrt{A_y^2 + A_z^2}} \f$, \f$ {\sqrt{A_x^2 + A_z^2}} \f$ and \f$ \phi \f$ are 0,
	 since these cases would result in division by zero. \n

	 The solution is to detect these conditions and set \f$ \theta \f$ or \f$ \psi \f$  or \f$  \phi \f$ to +/- 1.57 (+/-90 degrees in radians).
	 */

	if(Ry*Ry + Rz*Rz == 0 && Rx > 0) {
		*theta = 1.57079633;
	} else if(Ry*Ry + Rz*Rz == 0 && Rx < 0) {
		*theta = -1.57079633;
	} else if(Rx == 0){
		*theta = 0.0;
	}else {
		*theta = atan( Rx / sqrt(Ry*Ry + Rz*Rz) );
	}

	if(Rx*Rx + Rz*Rz == 0 && Ry > 0) {
		*psi = 1.57079633;
	} else if(Rx*Rx + Rz*Rz == 0 && Ry < 0){
		*psi = -1.57079633;
	} else if(Ry == 0) {
		*psi = 0.0; 
	}	else {
		*psi = atan (Ry / sqrt(Rx*Rx + Rz*Rz) );
	}

	if(Rz != 0){
		*phi = atan( sqrt(Rx*Rx + Ry*Ry) / Rz);
	} else {
		*phi = 0;
	}
}

void IMU_av_to_angle(float av, float dt, float bias, float * angle)
{
	/**
	 * The gyroscope outputs angular velocity \f$ \omega \f$ in degrees/second.
	 * \f$ \omega = \textrm{gyroscope output = degrees/second}  = \frac{d\theta}{dt} \f$. \n
	 *
	 * To get the angle of rotation, integrate the angular velocity: \n
	 * \f$ \int \omega = \int \frac{d\theta}{dt} = \theta \f$
	 *
	 * In this implementation, the angle is accumulated with each successive call to this function: \n
	 * \f$ \theta (t) = \theta(t-1) + \omega * \Delta t \f$
	 *
	 * Since gyroscopes have bias, output from the gyro when it is NOT experiencing any rotation,
	 * we need to compensate for this.
	 *
	 * \f$ \theta (t) = \theta(t-1) - bias*\Delta t + \omega * \Delta t \f$ \n
	 * Keep in mind that this bias drifts, a filter is necassery to correct for this.
	 *
	 */
	
	*angle = *angle + (-bias * dt) + (av * dt);
}

float IMU_deg_to_rad(float degrees)
{
	return degrees * (M_PI / 180);
}

float IMU_rad_to_deg(float radians)
{
	return radians * (180 / M_PI);
}
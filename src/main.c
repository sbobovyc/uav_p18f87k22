/**
 *
 * @file main.c
 * Prototype for the IMU library.
 *
 * @date Feb 1, 2011
 * @author sbobovyc
 * @details
 *
 */
#include <p18f87k22.h>
#include <usart.h>
#include <stdio.h>
#include <stdlib.h>
#include <delays.h>
#include <math.h>
#include "ADXL345.h"
#include "ADXL345_control.h"
#include "LV_MaxSonar_Analog.h"
#include "ITG3200.h"
#include "ITG3200_control.h"
#include "Kalman.h"
#include "IMU_utils.h"
#include "serial_com.h"
#include "timer.h"
#include "cpu_properties.h"

#pragma config FOSC = HS1 //medium speed oscillator
#pragma config XINST = OFF //disable extended instruction set

/*
#pragma config OSC = HS //high speed oscillator
#pragma config DEBUG = ON
#pragma config CP0 = OFF //code protection block 0 off
#pragma config CP1 = OFF
#pragma config CP2 = OFF
#pragma config LVP = OFF //low voltage programming is off during debugging
#pragma config WDT = OFF //watch dog timer has to be off during debugging
#pragma config BOR = OFF //brown out reset is off
*/

void main(void)
{
	unsigned long count = 0;
	// accelermoter readings
	float accel_readings[3] = {0.0,0.0,0.0}; //{accel_theta, accel_psi, accel_phi}
	float accel_variance[3] = {0.0,0.0,0.0}; //{variance_x, variance_y, variance_z}

	// gyro readings
	float gyro_readings[3] = {0.0,0.0,0.0}; //{gyro_theta, gyro_psi, gyro_phi}
	float gyro_readings_rads[3] = {0.0,0.0,0.0}; //{gyro_theta, gyro_psi, gyro_phi} but in radians
	float gyro_bias[3] = {0.0,0.0,0.0};	//{x_bias(roll), y_bias(pitch), z_bias(yaw));
	float gyro_variance[3] = {0.0,0.0,0.0}; //{variance_x, variance_y, variance_z}
	float gyro_diff[3] = {0.0,0.0,0.0};
	// sonar readings
	int sonar_readings[6] = {0,0,0,0,0,0}; //{sonar0,sonar1,sonar2,sonar3,sonar4,sonar5},

	// fused readings
	float roll_angle;
	float pitch_angle;
	float yaw_angle;


	// kalman filters
	overlay struct Gyro1DKalman roll;
	overlay struct Gyro1DKalman pitch;
	//overlay struct Gyro1DKalman yaw;

	// difference in time
	float dt;

	// loop counter
	unsigned char i;	


	// Start initialization
	Delay100TCYx(10); //let the devices startup
	usart_init();
	i2c_init();
	ADXL345_init();
	LV_MAXSONAR_init();
	ITG3200_init();
	timer_init_ms();
	Delay10TCYx(0);
	// End initialization
	
	// Zero accelerometer
	ADXL345_zero();

    // Calculate accelerometer variances
	ADXL345_calc_variance(&accel_variance[0], &accel_variance[2], &accel_variance[3]);
	//printf("Accelerometer variances X: %i Y: %i Z: %i \r\n", (int)accel_variance[0], (int)accel_variance[2], (int)accel_variance[3]);

	// Calculate gyroscope bias for the last time
	ITG3200_bias_calc(&gyro_bias[0],&gyro_bias[1],&gyro_bias[2]);
	// Calculate gyroscope variance
	ITG3200_calc_variance(&gyro_variance[0], &gyro_variance[2], &gyro_variance[3]);

	/** Initialize Kalman filters \n
	  * use calculated accelerometer variances 
	  */
	Kalman_init(&roll, 0.06, gyro_variance[0], accel_variance[0]);
	roll.x_angle = 0; //initial angle is 0
	roll.x_bias = gyro_bias[0]; // set gyro bias

	Kalman_init(&pitch, 0, gyro_variance[1], accel_variance[1]);
	pitch.x_angle = 0; //initial angle is 0
	pitch.x_bias = gyro_bias[1]; // set gyro bias

	//Kalman_init(&yaw, 0, gyro_variance[2], accel_variance[3]);
	//yaw.x_angle = 0; //initial angle is 0
	//yaw.x_bias = gyro_bias[2]; // set gyro bias
/*
	while(1) 
	{
		dt = timer_dt();
		
		// calculate tilt using the accel
		ADXL345_tilt_calc(&accel_readings[0], &accel_readings[1], &accel_readings[2]);

		// get gyro readings 
		gyro_readings[0] = ITG3200_getX();
		gyro_readings[1] = ITG3200_getY();
		gyro_readings[2] = ITG3200_getZ();
		gyro_diff[0] = fabs(gyro_readings[0] - gyro_bias[0]);
		gyro_diff[1] = fabs(gyro_readings[1] - gyro_bias[1]);
		gyro_diff[2] = fabs(gyro_readings[2] - gyro_bias[2]);

		gyro_readings_rads[0] = IMU_deg_to_rad(gyro_readings[0] / ITG3200_SENSITIVITY);
		gyro_readings_rads[1] = IMU_deg_to_rad(gyro_readings[1] / ITG3200_SENSITIVITY);
		gyro_readings_rads[2] = IMU_deg_to_rad(gyro_readings[2] / ITG3200_SENSITIVITY);

		Kalman_predict(&roll, gyro_readings_rads[0], dt);
		roll_angle = Kalman_update(&roll, accel_readings[0]);	

		Kalman_predict(&pitch, gyro_readings_rads[1], dt);
		pitch_angle = Kalman_update(&pitch, accel_readings[1]);	

		Kalman_predict(&yaw, gyro_readings_rads[2], dt);
		yaw_angle = Kalman_update(&yaw, accel_readings[2]);	

		if(gyro_diff[0] < 0.5 && gyro_diff[1] < 0.5 && gyro_diff[2] < 0.5)
			break;
		gyro_bias[0] = gyro_readings[0];
		gyro_bias[1] = gyro_readings[1];
		gyro_bias[2] = gyro_readings[2];
	}
	Kalman_init(&roll, 0, 0, accel_variance[0]);
	roll.x_angle = 0; //initial angle is 0
	roll.x_bias = gyro_bias[0]; // set gyro bias

	Kalman_init(&pitch, 0, 0, accel_variance[1]);
	pitch.x_angle = 0; //initial angle is 0
	pitch.x_bias = gyro_bias[1]; // set gyro bias

	Kalman_init(&yaw, 0, 0, accel_variance[3]);
	yaw.x_angle = 0; //initial angle is 0
	yaw.x_bias = gyro_bias[2]; // set gyro bias
*/
	while(1)
	{
		dt = timer_dt();
		
		// calculate tilt using the accel
		ADXL345_tilt_calc(&accel_readings[0], &accel_readings[1], &accel_readings[2]);
		
		// sometimes, one get a NaN, so need to compensate for this
		if(ieeetomchp(accel_readings[0]) == 0xFF7FFFFF)
			accel_readings[0] = roll_angle;
		if(ieeetomchp(accel_readings[1]) == 0xFF7FFFFF)
			accel_readings[1] = pitch_angle;

		// get gyro readings and convert to degrees then to radians
		gyro_readings[0] = ITG3200_getX();
		gyro_readings[1] = ITG3200_getY();
		//gyro_readings[2] = ITG3200_getZ();

		gyro_readings_rads[0] = IMU_deg_to_rad(gyro_readings[0] / ITG3200_SENSITIVITY);
		gyro_readings_rads[1] = IMU_deg_to_rad(gyro_readings[1] / ITG3200_SENSITIVITY);
		//gyro_readings_rads[2] = IMU_deg_to_rad(gyro_readings[2] / ITG3200_SENSITIVITY);

		Kalman_predict(&roll, gyro_readings_rads[0], dt);
		roll_angle = Kalman_update(&roll, accel_readings[0]);	

		Kalman_predict(&pitch, gyro_readings_rads[1], dt);
		pitch_angle = Kalman_update(&pitch, accel_readings[1]);	

		//Kalman_predict(&yaw, gyro_readings_rads[2], dt);
		//yaw_angle = Kalman_update(&yaw, accel_readings[2]);	

		printf("Roll angle %d \r\n", (int)(roll_angle * 180 / 3.14));
		printf("Pitch angle %d \r\n", (int)(pitch_angle * 180 / 3.14));
		//printf("Yaw angle %d \r\n", (int)(yaw_angle * 180 / 3.14));
		
		//LV_MAXSONAR_measure(&sonar_readings, SONAR_INCHES_MODE);		

		//Delay1KTCYx(1); 
	
	}
}
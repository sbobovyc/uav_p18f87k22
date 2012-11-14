/**
 @file ADXL345_control.c
 @author Stanislav Bobovych

  @details
	To convert raw data into Gs, \n
	\f$ Gs = textrm{Measured value} \times (G-range / 2^bits) \f$ \n
	Example(+/-4 g, 10 bit resolution): \n
	\f$ 1G = textrm{128} \times (8 / 2^bits) \f$
 */  
 
#include "ADXL345.h"
#include "IMU_utils.h"
#include "math_utils.h"
#include <stdio.h>
#include <math.h>
#include <delays.h>

void ADXL345_init(void)
{
	printf("Initializing accelerometer \r\n");
	//Go into standby mode to configure the device.
	ADXL345_setPowerControl(0x00);
	//interrupt active low, 4 wire SPI(which in this case doesn't matter since we are using I2C),  right-justified mode with sign extension, full resolution, +/-16g, 4mg/LSB, 
	//ADXL345_setDataFormatControl(0x0B);
	//interrupt active low, 4 wire SPI(which in this case doesn't matter since we are using I2C),  right-justified mode with sign extension, full resolution, +/-2g, 4mg/LSB, 
	//ADXL345_setDataFormatControl(0x08);
	///Interrupt active low, 4 wire SPI(which in this case doesn't matter since we are using I2C),  right-justified mode with sign extension, 10 bit resolution, +/-4g, 7.8mg/LSB, 
	ADXL345_setDataFormatControl(0x01);
	///100 Hz data rate.
	ADXL345_setDataRate(ADXL345_100HZ);
	//set the device offset registers to zero the device
	ADXL345_setOffset(ADXL345_X, 0);
	ADXL345_setOffset(ADXL345_Y, 0);
	ADXL345_setOffset(ADXL345_Z, 0);
	//zero the data registers
	ADXL345_multiByteWrite(ADXL345_DATAX0_REG, 0, 3);
	///Put accelerometer in measurement mode.
	ADXL345_setPowerControl(0x08);	
}

void ADXL345_zero(void)
{
	// accelerometer is level, with positive z axis pointing upward from the ground
	// 1. take a number of readings
	// 2. average the x,y,z components
	// 3. zero the device by altering the offset registers
	int readings[VECT_ROWS][3];
	int x_avg = 0;
	int y_avg = 0;
	int z_avg = 0;
	int z_1g = 128;
	int * ptr = (int *)readings;
	unsigned char i = 0;
	printf("Zeroing accelerometer \r\n");
	// get a number of samples
	for(i=0; i<VECT_ROWS*3; i=i+3)
	{
		Delay1KTCYx(100);
		ADXL345_getOutput(ptr);
		//print what was read, debugging only
		printf("X: %d Y: %d Z:%d \r\n", ptr[0], ptr[1], ptr[2]);
		ptr = ptr+3;
	}
	
	math_vect_avg_int(&readings, &x_avg, &y_avg, &z_avg);
	
	//print out the averages, debugging only
	printf("AVG X: %d Y: %d Z:%d \r\n", x_avg, y_avg, z_avg);

	/**
	  * Set the device offset registers to zero the device.
	  * Offset registers have a resolution of 15.6 mg/LSB, we are using 7.8 mg/LSB, so scale our calculated offsets by 2.
	  * If the calculated offset is between -2 and 2, let the offset be 0.
      */
	if(x_avg > -2 && x_avg < 2){}
	else
		ADXL345_setOffset(ADXL345_X, -(char)x_avg/2);	
	if(y_avg > -2 && y_avg < 2){}
	else
		ADXL345_setOffset(ADXL345_Y, -(char)y_avg/2);
	if(z_avg > -2 && z_avg < 2){}
	else
		ADXL345_setOffset(ADXL345_Z, (char)((z_1g - z_avg)/2));


	printf("Offest registers: X:%d Y:%d Z:%d \r\n", (char)ADXL345_getOffset(ADXL345_X), (char)ADXL345_getOffset(ADXL345_Y), (char)ADXL345_getOffset(ADXL345_Z));
	printf("Done zeroing \r\n");
}

void ADXL345_calc_variance(float * x_variance, float * y_variance, float * z_variance)
{
	/** 
	 * Once the accelerometer is level, with positive z axis pointing upward from the ground: \n
	 * 1. take a number of readings \n
	 * 2. compute variance
	 */
	int x_readings[VECT_ROWS];
	int y_readings[VECT_ROWS];
	int z_readings[VECT_ROWS];
	int reading[3];
	unsigned char i = 0;

	// get a number of samples
	for(i=0; i<VECT_ROWS; i++)
	{
		Delay1KTCYx(100);
		ADXL345_getOutput((int *)reading);
		//print what was read, debugging only
		printf("X: %d Y: %d Z:%d \r\n", reading[0], reading[1], reading[2]);
		x_readings[i] = reading[0];
		y_readings[i] = reading[1];
		z_readings[i] = reading[2];
	}
	
	*x_variance = math_variance_i((int *)x_readings);
	*y_variance = math_variance_i((int *)y_readings);
	*z_variance = math_variance_i((int *)z_readings);
}


void ADXL345_tilt_calc(float * theta, float * psi, float * phi)
{
	int readings[3] = {0,0,0};

	ADXL345_getOutput((int *)readings);

	printf("Acceleration(Raw data): X: %i Y: %i Z: %i \r\n", readings[0], readings[1], readings[2]);

	// Noise from the accelerometer can cause inclination calculations to be incorrect in certain corner cases.
	// This can be fixed by having ranges of accelerometer readings that will not be passed into the calculation.
	// Accelerometer readings from -3 to 3 are ignored and counted as 0.	
	if(readings[0] <= 3 && readings[0] >= -3)
	{
		readings[0] = 0;
	}
	if(readings[1] <= 3 && readings[1] >= -3)
	{
		readings[1] = 0;
	}
	if(readings[2] <= 3 && readings[2] >= -3)
	{
		readings[2] = 0;
	}
	
	IMU_accel_to_radians(readings[0], readings[1], readings[2], theta, psi, phi);

	//printf("Angles of inclination (accel, in radians x 100): %i %i %i \r\n", (int)(*theta*100), (int)(*psi*100), (int)(*phi*100));
	printf("Angles of inclination (accel, in degrees): %i %i %i \r\n", (int)(*theta*180/3.14), (int)(*psi*180/3.14), (int)(*phi*180/3.14));
}

void ADXL345_print(void)
{
	int readings[3];
	ADXL345_getOutput((int *)readings);
	printf("Acceleration(Raw data): X: %i Y: %i Z: %i \r\n", readings[0], readings[1], readings[2]);
}
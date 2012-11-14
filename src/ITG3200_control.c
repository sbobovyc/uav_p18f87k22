#include <stdio.h>
#include <delays.h>
#include "ITG3200.h"
#include "math_utils.h"

extern long int count;

void ITG3200_init()
{
  ITG3200_write(ITG3200_REG_PWR_MGM, 0);	// into active mode
  ITG3200_write(ITG3200_REG_SMPLRT_DIV, 0xFF); // 3.90625 ms per sample (EB, 50, 80, 7F, DE, 23, 20, FF)
  ITG3200_write(ITG3200_REG_DLPF_FS, 0x18); // F_SEL = 3 and DLPF_CFG = 0, means using full range and 256Hz low-band pass filter bandwith
  ITG3200_write(ITG3200_REG_INT_CFG, 0x00);
}

void ITG3200_bias_calc(float * x_bias, float * y_bias, float * z_bias) 
{
	float readings[VECT_ROWS][3];
	float x_avg = 0;
	float y_avg = 0;
	float z_avg = 0;
	float * ptr = (float *)readings;
	unsigned char i = 0;
	unsigned char j = 0;
	//printf("Calculating gyro bias \r\n");
	// get a number of samples
	for(i=0; i<VECT_ROWS*3; i=i+3)
	{

		Delay1KTCYx(10);
		ptr[0] = ITG3200_getX();
		ptr[1] = ITG3200_getY();
		ptr[2] = ITG3200_getZ();
		//print what was read, debugging only
		//printf("Gyro bias: X: %d Y: %d Z:%d \r\n", ptr[0], ptr[1], ptr[2]);
		ptr = ptr+3;
	}
	
	math_vect_avg_float(&readings, &x_avg, &y_avg, &z_avg);
	*x_bias = (float)x_avg;
	*y_bias = (float)y_avg;
	*z_bias = (float)z_avg;
}

void ITG3200_calc_variance(float * x_variance, float * y_variance, float * z_variance)
{
	/** 
	 * Once the gyroscope is level, with positive z axis pointing upward from the ground: \n
	 * 1. take a number of readings \n
	 * 2. compute variance
	 */
	float x_readings[VECT_ROWS];
	float y_readings[VECT_ROWS];
	float z_readings[VECT_ROWS];
	float reading[3];
	unsigned char i = 0;

	// get a number of samples
	for(i=0; i<VECT_ROWS; i++)
	{
		Delay1KTCYx(100);
		reading[0] = ITG3200_getX();
		reading[1] = ITG3200_getY();
		reading[2] = ITG3200_getZ();
		//print what was read, debugging only
		//printf("X: %d Y: %d Z:%d \r\n", reading[0], reading[1], reading[2]);
		x_readings[i] = reading[0];
		y_readings[i] = reading[1];
		z_readings[i] = reading[2];
	}
	
	*x_variance = math_variance_f((int *)x_readings);
	*y_variance = math_variance_f((int *)y_readings);
	*z_variance = math_variance_f((int *)z_readings);
}


void ITG3200_print(void)
{
	float readings[3];
	readings[0]= ITG3200_getX();
	readings[1] = ITG3200_getY();
	readings[2] = ITG3200_getZ();
	printf("Gyro(Raw data): X: %i Y: %i Z: %i \r\n", (int)readings[0], (int)readings[1], (int)readings[2]);
}
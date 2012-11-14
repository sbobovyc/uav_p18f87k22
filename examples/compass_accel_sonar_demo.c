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
#include <p18F452.h>
#include <usart.h>
#include <stdio.h>
#include <delays.h>
#include <math.h>
#include "ADXL345.h"
#include "ADXL345_control.h"
#include "HMC6352.h"
#include "LV_MaxSonar.h"
#include "serial_com.h"

#pragma config OSC = HS //high speed oscillator
#pragma config DEBUG = ON
#pragma config CP0 = OFF //code protection block 0 off
#pragma config CP1 = OFF
#pragma config CP2 = OFF
#pragma config LVP = OFF //low voltage programming is off during debugging
#pragma config WDT = OFF //watch dog timer has to be off during debugging
#pragma config BOR = OFF //brown out reset is off

void main(void)
{
/*
	int i;

	Delay100TCYx(10); //let the device startup
	usart_init();
	i2c_init();
	// initialize accelerometer
	ADXL345_init();
	// initialize compass
	HMC6352_getSlaveAddress();
	Delay10TCYx(0);
	HMC6352_getOpMode();
	HMC6352_setOpMode(HMC6352_QUERY , 1, 20);
	HMC6352_getOutputMode();
	// initialize sonar
	LV_MAXSONAR_init();

*/
	int accel_readings[3] = {0,0,0};
	int compass_heading = 0;
	int sonar_distance = 0;
	int i;

	Delay100TCYx(10); //let the device startup
	usart_init();
	i2c_init();
	ADXL345_init();
	// initialize sonar
	LV_MAXSONAR_init();
	printf("Starting\r\n");
	// initialize compass
	HMC6352_getSlaveAddress();
	Delay10TCYx(0);
	HMC6352_getOpMode();
	HMC6352_setOpMode(HMC6352_QUERY , 1, 20);
	HMC6352_getOutputMode();
	

	while(1)
	{
		printf("X\t Y\t Z\t Heading \t Distance \r\n");
		for(i=0; i<10; i++)
		{
			ADXL345_getOutput(&accel_readings);
			compass_heading = HMC6352_sample();
			sonar_distance = LV_MAXSONAR_measure();
	
			printf("%i\t %i\t %i\t %i\t\t %i \r\n", accel_readings[0], accel_readings[1], accel_readings[2], compass_heading, sonar_distance);
			Delay1KTCYx(100); 
		}
	}
}
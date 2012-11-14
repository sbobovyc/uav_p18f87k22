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
#include <stdlib.h>
#include <delays.h>
#include "IMU_utils.h"

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
	int readings[3] = {0,-2,127};
	float theta, psi, phi;
	Delay100TCYx(10);
	usart_init();

	while(1)
	{

		IMU_accel_to_radians(readings[0], readings[1], readings[2], &theta, &psi, &phi);
		
		printf("Angles of inclination (accel, in radians x 100): %i %i %i \r\n", (int)(theta*100), (int)(psi*100), (int)(phi*100));
		printf("Angles of inclination (accel, in degrees): %i %i %i \r\n", (int)(theta*180/3.14), (int)(psi*180/3.14), (int)(phi*180/3.14));
		Delay10TCYx(1);
	}
}
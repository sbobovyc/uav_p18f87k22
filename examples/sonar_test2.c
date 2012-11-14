/*
		Sonic Sensor Test Code (ver2)
*/
#include <p18F452.h>
#include <usart.h>
#include <stdio.h>
#include <delays.h>
#include "serial_com.h"
#include "LV_MaxSonar.h"

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
	//vars
	int distance;

	Delay100TCYx(10); //let the device startup
	usart_init();
	LV_MAXSONAR_init();
	printf("Starting\r\n");

	while(1)
	{
		distance = LV_MAXSONAR_measure();
		printf("Distance is %i inches \r\n", distance);
	}
}
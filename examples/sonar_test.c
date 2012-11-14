/*
		Sonic Sensor Test Code
*/
#include <p18F452.h>
#include <usart.h>
#include <stdio.h>
#include <delays.h>
#include <i2c.h>
#include <timers.h>
#include <math.h>
#include "serial_com.h"
#include "HMC6352.h"
#include "ADXL345.h"

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
	//timer vars
	int result;
	float final_result;

	Delay100TCYx(10); //let the device startup
	usart_init();
	i2c_init();
	printf("Starting\r\n");


	// let sonar initialize, delay 250 ms
	Delay1KTCYx(625);
	TRISBbits.TRISB3 = 1;	// data pin input
	
	while(1)
	{
		while(PORTBbits.RB3 == 0);
		// configure timer0
		OpenTimer0(TIMER_INT_OFF & T0_16BIT & T0_SOURCE_INT & T0_PS_1_1);
 		WriteTimer0( 0 );         // restart timer
		while(PORTBbits.RB3 == 1);
		//Delay1TCY();
		result = ReadTimer0();    // read timer
		printf("Timer is %u \r\n", result);
		final_result = (float)result / (float)(2.5*147);
		printf("X / (4*147) = %i \r\n", (int)final_result);
		
	}
	
}
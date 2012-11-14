#include <p18F452.h>
#include <usart.h>
#include <stdio.h>
#include <delays.h>
#include <timers.h>
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
	//timer vars
	int result;

	Delay100TCYx(10); //let the device startup
	usart_init();
	printf("Starting\r\n");

	
	while(1)
	{
		// configure timer0
		OpenTimer0(TIMER_INT_OFF & T0_16BIT & T0_SOURCE_INT & T0_PS_1_1);
 		WriteTimer0( 0 );         // restart timer
		Delay10TCYx(15);
		result = ReadTimer0();    // read timer
		printf("Timer is %u \r\n", result);
		
	}
	
}
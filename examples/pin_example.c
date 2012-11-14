#include <p18f87k22.h>
#include <delays.h>

#pragma config FOSC = HS1 //medium speed oscillator
#pragma config XINST = OFF //disable extended instruction set
/*
#pragma config OSC = HS //high speed oscillator, 10 MHz
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
	//This uses a 10 MHz clock to generate a 416 kHz square wave signal
	TRISCbits.TRISC4 = 0;	// data pin output
	TRISCbits.TRISC3 = 0;	// data pin output
	while(1){
		PORTCbits.RC4 = 1;	// pin high
		PORTCbits.RC3 = 1;	// pin high
		Delay100TCYx(1);		// delay
		PORTCbits.RC4 = 0;	// pin low
		PORTCbits.RC3 = 0;	// pin low
		Delay100TCYx(1);		// delay
	}
	
}
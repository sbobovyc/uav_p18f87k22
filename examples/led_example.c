#include <p18F452.h>
#include <delays.h>

#pragma config OSC = HS //high speed oscillator
#pragma config DEBUG = ON
#pragma config CP0 = OFF //code protection block 0 off
#pragma config CP1 = OFF
#pragma config CP2 = OFF
#pragma config LVP = OFF //low voltage programming is off during debugging
#pragma config WDT = OFF //watch dog timer has to be off during debugging
#pragma config BOR = OFF //brown out reset is off


void main()
{
	TRISA = 0; //set as output
	while(1)
	{
		PORTAbits.RA4 = 0; //turn on
		PORTAbits.RA3 = 0; //turn off
		Delay1KTCYx(100);		
		PORTAbits.RA4 = 1; //turn off
		PORTAbits.RA3 = 1; //turn on
		Delay1KTCYx(100);		
	}

}
#include <p18F452.h>
#include <delays.h>
#include <sw_i2c.h>

#pragma config OSC = HS //high speed oscillator, 10 MHz
#pragma config DEBUG = ON
#pragma config CP0 = OFF //code protection block 0 off
#pragma config CP1 = OFF
#pragma config CP2 = OFF
#pragma config LVP = OFF //low voltage programming is off during debugging
#pragma config WDT = OFF //watch dog timer has to be off during debugging
#pragma config BOR = OFF //brown out reset is off


void main(void)
{
	//This uses a 10 MHz clock to 
	// generate a 416 kHz square wave signal 
	// for I2C communication
	CLOCK_LOW;	// data pin output
	//DATA_LOW;

	while(1){
		SCLK_PIN = 1;	// pin high
		Delay1TCY();		// delay
		SCLK_PIN = 0;	// pin low
		Delay1TCY();		// delay
	}
	
}
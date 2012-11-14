#include <p18F452.h>
#include <portb.h> 

#pragma config OSC = HS //high speed oscillator
#pragma config DEBUG = ON
#pragma config CP0 = OFF //code protection block 0 off
#pragma config CP1 = OFF
#pragma config CP2 = OFF
#pragma config LVP = OFF //low voltage programming is off during debugging
#pragma config WDT = OFF //watch dog timer has to be off during debugging
#pragma config BOR = OFF //brown out reset is off

void toggle_light(void);

/*
#pragma code LOW_INTERRUPT_VECTOR = 0x18
void Interrupt_low_vec(void)
{
	_asm 
		goto toggle_light
	_endasm
}
*/

#pragma code HIGH_INTERRUPT_VECTOR = 0x8
void high_ISR(void)
{
	_asm
		goto toggle_light
	_endasm
}

#pragma interrupt toggle_light
void toggle_light(void)
{
	PORTAbits.RA4 = ~PORTAbits.RA4; //toggle
}

void EnableInterrupts (void)
{
	INTCONbits.RBIE = 1; //Enable the PORTB interrupt-on-change (even though OpenPORTB call should do this)
	RCONbits.IPEN = 1; //Enable interrupt priorities
	INTCON2bits.RBIP = 1; //Set the PORTB interrupt-on-change as a high priority interrupt
} 



void main(void)
{
	EnableInterrupts();
	//initialize LED port
	TRISA = 0; //set as output
	PORTAbits.RA4 = 1; //turn off
	
	//initialize interrupt port
	TRISB = 0xf0; //Enable PORTB<4:7> as inputs
	OpenPORTB(PORTB_CHANGE_INT_ON & PORTB_PULLUPS_ON);
	INTCONbits.RBIF = 0; //Make sure that the PORTB interrupt is not already set 

}
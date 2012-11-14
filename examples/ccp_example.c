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
#include <p18f87k22.h>
#include <capture.h>
#include <timers.h>


#pragma config FOSC = HS1 //medium speed oscillator
#pragma config XINST = OFF //disable extended instruction set


unsigned int t4 = 0;
unsigned int t4_dt = 0;

#pragma interrupt check_CCP
void check_CCP(void)
{
	// is the CCP4 interrupt flag raised? 
	if(PIE4bits.CCP4IE == 1)
	{
		//****0101 means rising edge
		if(CCP4CONbits.CCP4M2 && CCP4CONbits.CCP4M0) 
		{
			t4 = ReadTimer1();		
			// reconfigure CCP4 to capture falling edge events
			OpenCapture4( C4_EVERY_FALL_EDGE & CAPTURE_INT_ON );
		} else {
			t4_dt = ReadTimer1() - t4;
			// reconfigure CCP4 to capture rising edge events
			OpenCapture4( C4_EVERY_RISE_EDGE & CAPTURE_INT_ON );
		}
	}
	
}

#pragma code HIGH_INTERRUPT_VECTOR = 0x8
void high_ISR(void)
{
	_asm
		goto check_CCP
	_endasm
}


void EnableInterrupts (void)
{
	RCONbits.IPEN = 1; //Enable interrupt priorities
} 

void main(void)
{
	unsigned int result = 0;

	//RC2 = ECCP1
	//RE7/RB3/RC1 = ECCP2
	//RG0 = ECCP3
	//RG3 = CCP4
	//RG4 = CCP5
	//RE6 = CCP6
	//RE5 = CCP7
	//RE4 = CCP8

  	// Enalge interrupts
	EnableInterrupts();
	TRISGbits.TRISG3 = 0;	// set CCP4 port as input

  	// Configure Timer1
	// turn off timer interrupt, use 16 bit mode, 1:1 prescaler, enable Timer1 oscilator, sync to external clock input
  	OpenTimer1( 0,TIMER_INT_OFF & T1_16BIT_RW & T1_PS_1_1  & T1_OSC1EN_ON & T1_SYNC_EXT_ON);
	WriteTimer1(0); //zero out timer

	while(1);
}
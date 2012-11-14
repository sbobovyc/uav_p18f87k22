#include <p18F452.h>
#include <timers.h>
#include <usart.h>
#include <stdio.h>
#include "serial_com.h"

void set_timer(void)
{
    OpenTimer0(TIMER_INT_ON & T0_16BIT & T0_SOURCE_INT & T0_PS_1_4);
     INTCONbits.GIEH = 1; //enable interrupts
}

//----------------------------------------------------------------------------
// High priority interrupt routine
#pragma code
#pragma interrupt InterruptHandlerHigh

void
InterruptHandlerHigh ()
{ //Every 0.026 secs
   if (INTCONbits.TMR0IF)
   { //check for TMR0 overflow
      INTCONbits.TMR0IF = 0; //clear interrupt flag
      WriteTimer0(0);
      //Code used by the interrupt goes here
   }
	printf("In interrupt handler \r\n");
}
#pragma code InterruptVectorHigh = 0x08
void
InterruptVectorHigh (void)
{
  _asm
   goto InterruptHandlerHigh //jump to interrupt routine
  _endasm
}


void main (void)
{
	int result;

	usart_init();
    set_timer();
	printf("Start \r\n");
    WriteTimer0(0);
    while(1){
		printf("In loop \r\n");
		result = ReadTimer0();    // read timer
		printf("Timer is %u \r\n", result);
      //interrupt will run now

    }
}
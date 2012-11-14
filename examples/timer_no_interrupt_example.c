#include <p18F452.h>
#include <timers.h>
#include <usart.h>
#include <stdio.h>
#include "serial_com.h"

void set_timer(void)
{
    OpenTimer0(TIMER_INT_OFF & T0_16BIT & T0_SOURCE_INT & T0_PS_1_1);
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
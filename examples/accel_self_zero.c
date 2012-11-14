#include <p18F452.h>
#include <usart.h>
#include <stdio.h>
#include <delays.h>
#include "ADXL345.h"
#include "ADXL345_control.h"
#include "HMC6352.h"
#include "LV_MaxSonar.h"
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
	int readings[3] = {0,0,0};
	int i;

	Delay100TCYx(10); //let the device startup
	usart_init();
	i2c_init();
	puts("Starting\r\n");
	ADXL345_init();

	for(i = 0; i<5; i++)
	{	
		ADXL345_getOutput(&readings);
		printf("%i %i %i \r\n", readings[0], readings[1], readings[2]);
	}

	ADXL345_zero();

	while(1){
		ADXL345_getOutput(&readings);
		printf("%i %i %i \r\n", readings[0], readings[1], readings[2]);
		Delay10KTCYx(100000); 
		//ADXL345_tilt_calc();
	}
	

}
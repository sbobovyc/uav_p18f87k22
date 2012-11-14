#include <p18F452.h>
#include <usart.h>
#include <stdio.h>
#include <delays.h>
#include <i2c.h>
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
	unsigned char data = 0;
	int data1 = 0;
	int data2 = 0;
	int data3 = 0;
	int readings[3] = {0,0,0};
	int compassSample;
	int i, j, k;

	Delay100TCYx(10); //let the device startup
	usart_init();
	i2c_init();
	printf("Starting\r\n");
	


/*	******************************* Compass code and ADSL Code ********************** */
	//Tests compass by getting slave address
	//it should be 0x21
	data2 = HMC6352_getSlaveAddress();
	Delay10TCYx(0);
	printf("Slave Address: 0x%02X \r\n", data2);

	data3 = HMC6352_getOpMode();
	printf("Op Mode: 0x%02X \r\n", data3);
	printf("Setting new op mode from 0x%02X to 0x51\r\n", data3);
	HMC6352_setOpMode(HMC6352_QUERY , 1, 20);

	data3 = HMC6352_getOutputMode();
	printf("Output Mode: 0x%02X \r\n", data3);
	
	while(1)
	{
		data2 = HMC6352_sample();
		if(data2 <275 || data2 > 3599)
			printf("North \r\n");

			
		printf("Heading: %i \r\n", data2);
	}

}
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
	unsigned char outputData[2];
	unsigned int result = 0;
	unsigned char data = 0;
	Delay100TCYx(10); //let the device startup
	usart_init();
	i2c_init();
	printf("Starting\r\n");
	


/*	******************************* Compass code and ADSL Code ********************** */
	//Tests compass by getting slave address
	//it should be 0x21
	Delay10TCYx(0);
	printf("Slave Address: 0x%02X \r\n", HMC6352_getSlaveAddress());
	HMC6352_setOpMode(HMC6352_CONTINUOUS , 1, 20);
	printf("Op Mode: 0x%02X \r\n", HMC6352_getOpMode());
	printf("Output Mode: 0x%02X \r\n", HMC6352_getOutputMode());
	

	while(1)
	{
		StartI2C();
		WriteI2C((HMC6352_I2C_ADDRESS << 1) | 1);
		outputData[0] = ReadI2C();
		NotAckI2C();
		outputData[1] = ReadI2C();
		NotAckI2C();    	
		StopI2C();

		result = outputData[0];
		result = result << 8;
		result = result | outputData[1];

		/*
		if(data2 <275 || data2 > 3599)
			printf("North \r\n");
		*/
			
		printf("Heading: %i \r\n", result-255);
	}

}
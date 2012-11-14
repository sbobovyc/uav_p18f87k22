#include <p18F87k22.h>
#include <usart.h>
#include <stdio.h>
#include <delays.h>
#include "ADXL345.h"
#include "serial_com.h"

#pragma config FOSC = HS1 //high speed oscillator
#pragma config XINST = OFF //disable extended instruction set
/*
#pragma config OSC = HS //high speed oscillator
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
	unsigned char devid = 0;
	int readings[3] = {0,0,0};

	Delay100TCYx(10); //let the device startup
	usart_init();
	i2c_init();
	puts("Starting\r\n");

	devid = ADXL345_getDevId();
	printf("Device ID is 0x%2X \r\n", devid);
	
	//Go into standby mode to configure the device.
	ADXL345_setPowerControl(0x00);
	//interrupt active low, 4 wire SPI(which in this case doesn't matter since we are using I2C),  right-justified mode with sign extension, full resolution, +/-16g, 4mg/LSB, 
	ADXL345_setDataFormatControl(0x0B);
	//100 Hz data rate.
	ADXL345_setDataRate(ADXL345_100HZ);
	//Measurement mode.
	ADXL345_setPowerControl(0x08);

	while(1){
		ADXL345_getOutput(&readings);
		printf("%i %i %i \r\n", readings[0], readings[1], readings[2]);
		Delay10KTCYx(100000); 
	}
	

}
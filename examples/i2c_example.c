#include <p18F87k22.h>
#include <delays.h>
#include <i2c.h>
#include <stdio.h>
#include <usart.h>

#define I2C_SCLK_LAT TRISCbits.TRISC3	///< I2C clock tristate bit
#define I2C_DATA_LAT TRISCbits.TRISC4	///< I2C data tristate bit

#pragma config FOSC = HS1 //high speed oscillator, 10 MHz
#pragma config XINST = OFF //disable extended instruction set
/*
#pragma config OSC = HS //high speed oscillator, 10 MHz
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
	unsigned char data = 0;
	signed char err0 = 0, err1 = 0, err2 = 0;
	Delay100TCYx(10); //let the device startup

	usart_init();
	//This uses a 10 MHz clock to 
	// generate a 416 kHz square wave signal 
	// for I2C communication

	//initialize section
	CloseI2C();
	I2C_SCLK_LAT = 1;	//SCLK is an input
	I2C_DATA_LAT = 1;	//DATA pin is an input
    OpenI2C(MASTER,SLEW_OFF); //400 kHz
	// SSPADD = ( (Fosc/4) / BiteRate )-1
	// SSPADD = (10000000/4 / 400000) -1 = 5
    SSP1ADD=((10000)/(4*100) - 1);	 
	//IdleI2C();

	printf("Var is %i\r\n", data);
	printf("Starting\r\n");
	printf("SSPCON1 is x%x\r\n", SSPCON1);
	printf("SSPADD is x%x\r\n", SSP1ADD);
	printf("SSPSTAT is x%x\r\n", SSPSTAT);

	while(1){
		//printf("Looping\r\n");

		
		StartI2C();
		printf("got here \r\n");
		
		err0 = WriteI2C(0x3A);  //adxl addr write
		printf("Err0 is %i\r\n", err0);
		/*
		//while(SSPCON2bits.ACKSTAT);
		err1 = WriteI2C(0x00);  //devid
		printf("Err1 is %i\r\n", err1);
		//while(SSPCON2bits.ACKSTAT);
		RestartI2C();
		err2 = WriteI2C(0x3B); //adxl addr read
		printf("Err2 is %i\r\n", err1);
		//while(SSPCON2bits.ACKSTAT);
		data = ReadI2C();
		NotAckI2C();
		*/
		StopI2C();
		printf("Var is %i\r\n", data);
		
		//while(1);
		
	}
	
}
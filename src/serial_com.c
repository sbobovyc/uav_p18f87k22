/**
 * Implementation of interfaces to serial devices. 
 * @file serial_com.c
 * @author Stanislav Bobovych
 */
#include <usart.h>
#include <sw_uart.h>
#include <spi.h>
#include <i2c.h>
#include <delays.h>
#include "serial_com.h"
#include "cpu_properties.h"


void DelayTXBitUART(void)
{
   char delay_time = (((((2*OSC)/(4*SW_UART_BAUD))+1)/2)-12)/10;
   Delay10TCYx (delay_time);
   return;
}
void DelayRXHalfBitUART(void)
{
   char delay_time = (((((2*OSC)/(8*SW_UART_BAUD))+1)/2)-9)/10;
   Delay10TCYx (delay_time);
   return;
}
void DelayRXBitUART(void)
{
   char delay_time = (((((2*OSC)/(4*SW_UART_BAUD))+1)/2)-14)/10;
   Delay10TCYx (delay_time);
   return;
} 

void usart_init(void)
{
	unsigned char config=0,spbrg=0;
	Close1USART();  //turn off usart if was previously on

	// configure USART
	config = USART_TX_INT_OFF &
	USART_RX_INT_OFF &
	USART_ASYNCH_MODE &
	USART_EIGHT_BIT &
	USART_CONT_RX &
	USART_BRGH_HIGH;
	
	/** 
	 * Here we want to use a baud rate of 9600 and a clock of 10 MHz. To detemine what should be written to the baud rate generator register: \n 
	 * \f$ 9600 = 10 MHz / (4 * (spbrg + 1)) \f$ \n
	 * \f$ spbrg = (10 MHz/9600*4) - 1 \f$ 
	 */
	spbrg = (OSC/9600*4) - 1;
	Open1USART(config, spbrg);
}

/**
 * This functions initializes the SPI unit
 */
void spi_init(void)
{

		//initialize ports
		SPI_CS_TRI = 0;	//chip select output
		SPI_CLK_TRI = 0; //clock is output
		SPI_SDI_TRI = 1; //SDI is input
		SPI_SDO_TRI = 0; //SDO is output

		OpenSPI(SPI_FOSC_16,MODE_11,SMPEND);

		/*
		_asm
			// Set up the SPI Support
			movlw 0x00
			movwf   TRISC,0       //all Outputs
			movlw 0x06
			movwf   ADCON1,0      //so PORTC can be used
	
			// Set up the SPI Port
			movlw   0x10        //SCK is output (Master), SDI is input, 
			movwf   TRISC,0     //SDO is output, all others output
			movlw   0x00        //Mode 1,1 SPI, sample on rising edge
			movwf   SSPSTAT,0   
			movlw	0x31
			movwf	SSPCON1,0	//Mode 1,1 SPI Master Mode, 1/16 Tosc bit time, clock idles high
		_endasm
		*/
}


void i2c_init(void)
{
	
	//initialize ports
	// for pic18f453
	//I2C_SCLK_LAT = 0;	//SCLK is an output
	//I2C_DATA_LAT = 0;	//DATA pin is an output

	// for pic18f87k22
	I2C_SCLK_LAT = 1;	//SCLK is an input
	I2C_DATA_LAT = 1;	//DATA pin is an input

	/**
	 * When the SSP is configured in Master mode, the lower seven bits of SSPADD act as the baud rate generator reload value. \n
	 * \f$ \textrm{I2C baud rate} = \frac{FOSC} {(4*(SSPADD +1))} \f$ \n
	 * so \f$ SSPADD = \frac{FOSC}{4 * \textrm{I2C baud rate}} - 1 \f$ \n
     * In this case, 100 kbps buad rate is used.
	 */

	// SSPADD=(FOSC / (4 * BAUD)) - 1; //This is the register on pic18f452
	SSP1ADD=(OSC / (4 * BAUD)) - 1; //This is the register on pic18f87k22
	OpenI2C(MASTER,SLEW_OFF); //100 kHz
	
	/*
	_asm
		//Setup I/O
		movlw	0x18  			// '00011000’ RC3, RC4 are inputs for PORTC
		movwf   TRISC,0         // Remaining PORTC I/O lines are outputs
		
		//Setup I2C
		//Configure MSSP module for Master Mode
		movlw	0x28  	// B'00101000’ Enables MSSP and uses appropriate 
						//PORTC pins for I2C mode (SSPEN set) AND
		//Enables I2C Master Mode (SSPMx bits)
		movwf   SSPCON1,0	//This is loaded into SSPCON

		//Configure Input Levels and slew rate as I2C Standard Levels
		movlw   0x80	//Slew Rate control (SMP) set for 100kHz
		movwf   SSPSTAT,0 // mode and input levels are I2C spec, loaded in SSPSTAT

		//Configure Baud Rate
		movlw   (FOSC / (4 * BAUD)) - 1  // Calculates SSPADD Setting for desired Baud rate and sets up SSPADD
		movwf   SSPADD,0          
	_endasm
	*/
}

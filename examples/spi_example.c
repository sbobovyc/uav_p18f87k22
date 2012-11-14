#include <p18F452.h>
#include <delays.h>
#include <spi.h>
#include <stdio.h>
#include <usart.h>

#pragma config OSC = HS //high speed oscillator, 10 MHz
#pragma config DEBUG = ON
#pragma config CP0 = OFF //code protection block 0 off
#pragma config CP1 = OFF
#pragma config CP2 = OFF
#pragma config LVP = OFF //low voltage programming is off during debugging
#pragma config WDT = OFF //watch dog timer has to be off during debugging
#pragma config BOR = OFF //brown out reset is off

void usart_init(void)
{
	unsigned char config=0,spbrg=0;
	//status light on
	TRISA = 0; //set as output
	PORTAbits.RA4 = 0; //turn on

	CloseUSART();  //turn off usart if was previously on

	// configure USART
	config = USART_TX_INT_OFF &
	USART_RX_INT_OFF &
	USART_ASYNCH_MODE &
	USART_EIGHT_BIT &
	USART_CONT_RX &
	USART_BRGH_HIGH;
	
	// 9600 = 10 MHz / (4 * (spbrg + 1))
	// spbrg = (10 MHz/9600*4) - 1
	spbrg = (10000000/9600*4) - 1;
	OpenUSART(config, spbrg);
}

void spi_init(void)
{

		//initialize ports
		TRISCbits.TRISC2 = 0;	//chip select output
		TRISCbits.TRISC3 = 0; //clock is output
		TRISCbits.TRISC4 = 1; //SDI is input
		TRISCbits.TRISC5 = 0; //SDI is output

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

void main(void)
{
	unsigned char data = 0;
	signed char err0 = 0, err1 = 0, err2 = 0;
	Delay100TCYx(10); //let the device startup

	//initialize uart
	usart_init();
	spi_init();
	printf("Var is %i\r\n", data);
	printf("Starting\r\n");
	printf("SSPCON1 is x%x\r\n", SSPCON1);
	printf("SSPADD is x%x\r\n", SSPADD);
	printf("SSPSTAT is x%x\r\n", SSPSTAT);

	PORTCbits.RC2 = 1;	// pin high
	while(1){
		PORTCbits.RC2 = 0;	// CS low
		Delay1TCY();		// delay at least 5 ns
		WriteSPI(0x80);		// b'10000000, read single byte 000000
		//WriteSPI(0xB2);		// b'10101100, read single byte 
		data = ReadSPI();
		PORTCbits.RC2 = 1;	// CS high
		Delay1TCY();		// delay at least 5 ns
		//data = data>>1;
		printf("Data is 0x%02X \r\n", data);
	}	
}
#include <p18F452.h>
#include <usart.h>

#pragma config OSC = HS //high speed oscillator
#pragma config DEBUG = ON
#pragma config CP0 = OFF //code protection block 0 off
#pragma config CP1 = OFF
#pragma config CP2 = OFF
#pragma config LVP = OFF //low voltage programming is off during debugging
#pragma config WDT = OFF //watch dog timer has to be off during debugging
#pragma config BOR = OFF //brown out reset is off

unsigned char Txdata[] = "MICROCHIP_USART\r\n";
unsigned char config=0,spbrg=0;

/*
config
A bitmask that is created by performing a bitwise AND operation (‘&’) 
with a value from each of the categories listed below. These values are 
defined in the file usart.h.
Interrupt on Transmission:
USART_TX_INT_ON Transmit interrupt ON
USART_TX_INT_OFF Transmit interrupt OFF
Interrupt on Receipt:
USART_RX_INT_ON Receive interrupt ON
USART_RX_INT_OFF Receive interrupt OFF
USART Mode:
USART_ASYNCH_MODE Asynchronous Mode
USART_SYNCH_MODE Synchronous Mode
Transmission Width:
USART_EIGHT_BIT 8-bit transmit/receive
USART_NINE_BIT 9-bit transmit/receive
Slave/Master Select*:
USART_SYNC_SLAVE Synchronous Slave mode
USART_SYNC_MASTER Synchronous Master mode
Reception mode:
USART_SINGLE_RX Single reception
USART_CONT_RX Continuous reception
Baud rate:
USART_BRGH_HIGH High baud rate
USART_BRGH_LOW Low baud rate
* Applies to Synchronous mode only
*/
/*
spbrg
This is the value that is written to the baud rate generator register which 
determines the baud rate at which the USART operates. The formulas 
for baud rate are:
Asynchronous mode, high speed: 
      FOSC / (16 * (spbrg + 1))
Asynchronous mode, low speed: 
      FOSC / (64 * (spbrg + 1))
Synchronous mode:
      FOSC / (4 * (spbrg + 1))
Where FOSC is the oscillator frequency.
*/

void main(void)
{
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

	//------USART Transmission ----
	while(1)
	{
		while(BusyUSART());             //Check if Usart is busy or not
		putsUSART((char *)Txdata);      //transmit the string which is stored in program memmory
	}
}

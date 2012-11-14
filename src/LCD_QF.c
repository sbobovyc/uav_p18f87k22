/*
	QwikFlash LCD
*/
#include<delays.h>
#include<string.h>
#include "LCD_QF.h"

unsigned char lcdtext[16+1];

void LCD_print(void)
{
	unsigned char temp;
	unsigned char i;
	RS_SIG = 0;
	E_SIG = 1;
	LATD &= 0b00001111;
	LATD |= 0x80;
	E_SIG = 0;
	E_SIG = 1;
	LATD &= 0b00001111;
	LATD |= 0x00;
	E_SIG = 0;
	T40;

	RS_SIG = 1;
	for( i = 0; (i < 8) && (lcdtext[i] != 0x00); i++)
	{
		E_SIG = 1;
		temp = lcdtext[i];
		temp &= 0b11110000;
		LATD &= 0b00001111;
		LATD |= temp;
		E_SIG = 0;
		E_SIG = 1;
		temp = lcdtext[i] << 4;
		temp &= 0b11110000;
		LATD &= 0b00001111;
		LATD |= temp;
		E_SIG = 0;
		T40;
	}	
	RS_SIG = 0;
	E_SIG = 1;
	LATD &= 0b00001111;
	LATD |= 0xC0;
	E_SIG = 0;
	E_SIG = 1;
	LATD &= 0b00001111;
	LATD |= 0x00;
	E_SIG = 0;
	T40;
	RS_SIG = 1;

for( i = 8; (i < 16) && (lcdtext[i] != 0x00); i++)
	{
		E_SIG = 1;
		temp = lcdtext[i];
		temp &= 0b11110000;
		LATD &= 0b00001111;
		LATD |= temp;
		E_SIG = 0;
		E_SIG = 1;
		temp = lcdtext[i] << 4;
		temp &= 0b11110000;
		LATD &= 0b00001111;
		LATD |= temp;
		E_SIG = 0;
		T40;
	}	
	
}	
void LCD_clear(void)
{
	
	unsigned char temp;
	RS_SIG = 0;
	E_SIG = 1;
	temp = LCD_CLEAR;
	temp &= 0b11110000;
	LATD &= 0b00001111;
	LATD |= temp;
	E_SIG = 0;
	E_SIG = 1;
	temp = LCD_CLEAR << 4;
	temp &= 0b11110000;
	LATD &= 0b00001111;
	LATD |= temp;
	E_SIG = 0;
	T40;

	
}	


void LCD_init(void)
{
	unsigned char inits[7] = {0x33, 0x32, 0x28, 0x01, 0x0c, 0x06, 0x00};
	unsigned char temp;
	char i;
	
	TRISD &= 0b00001111;	//set high nibble of portd to output
	RS_SIG = 0;
	E_SIG = 0;
	E_TRIS = 0;
	RS_TRIS = 0;
	
	memset(lcdtext, ' ', sizeof(lcdtext)-1);
	Delay10KTCYx(25);
	
	for( i = 0; i < 7; i++ )
	{
		E_SIG = 1;
		temp = inits[i];
		temp &= 0b11110000;
		LATD &= 0b00001111;
		LATD |= temp;
		E_SIG = 0;
		Delay1KTCYx(25);
		E_SIG = 1;
		temp = inits[i] << 4;
		temp &= 0b11110000;
		LATD &= 0b00001111;
		LATD |= temp;
		E_SIG = 0;
		Delay1KTCYx(25);
	}	
	
}	
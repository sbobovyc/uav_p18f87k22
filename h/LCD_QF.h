/*
	QwikFlash LCD
*/
#include<delayS.h>

#define NIBBLE PORTD
#define E_SIG LATEbits.LATE1
#define E_TRIS	TRISEbits.TRISE1
#define RS_SIG LATEbits.LATE0
#define RS_TRIS	TRISEbits.TRISE0

#define LCD_HOME 0b00000010
#define LCD_CLEAR 0b00000001
#define T40 Delay100TCYx(1)

extern unsigned char lcdtext[16 + 1];
void LCD_print(void);
void LCD_clear(void);
void LCD_init(void);

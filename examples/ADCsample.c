/*****************************************************************
*james morris

********************************************************************/

/************************ HEADERS **********************************/
#include "adc.h"





/************************ VARIABLES ********************************/


#define VREF ADCON0bits.VCFG0
#define VDDREF 0
#define INPUTREF 1	//AN3 is external ref

#define VCHAN ADCON0bits.CHS
#define BG 0b1111

	#define BOARD_POT 0x00	//picdem 18 board
	#define RESVAL 3235.4		//load divider low-side resistor val (current calc)
	#define UNDIVIDE 1.24622		//to calculate Vout from measured voltage divider

#define CSENSE 0b0010	//channel 2
#define CSENSE_IO ANCON0bits.PCFG2
#define BATT 0b0011

#define VBGSTABLE 

#define EXPVDD 2.9959 //3.348	//measured voltage of pic18 explorer's VAR voltage (or chip's VDD); used to confirm band gap.  not necessary, but measure for each board if you use it


unsigned int getVbgmV(void);
unsigned int getVoutmV(void);
unsigned int getCurrentuA(void);
unsigned int getVddmV(void);
int getADCVal(void);
void calibrateADC(void);
float getVdd(void);
BYTE getPotInput(void);

unsigned int mVbg, mVbat, current, tcurrent;
float Vdd;
BYTE TimeInt = 0;
BYTE dPot = 0;
BYTE potVal;







/*********************************************************************
* Function:         void main(void)
********************************************************************/
void main(void)
{   
    BYTE ByteIn;
    int adcval, valdiv;
     
    int ticks;
    
    
    
   
   
    /*******************************************************************/
    // Initialize the system
    /*******************************************************************/
    OSCTUNEbits.PLLEN = 1; //96MHz PLL
    InitSymbolTimer();
    
    ADC_ERR = 0;
    
    
    
     
     
	
	//Initialize ADC:
	ADCON0bits.ADON = 1; 	//ADC on
	ADCON0bits.CHS = 0;		//channel 0
	ADCON0bits.VCFG = 0;	//Vss/Vdd serves as voltage ref
	
	ADCON1bits.ADFM = 1;	//right justified
	ADCON1bits.ADCAL = 0;	//no calibration
	ADCON1bits.ACQT = 0b101;	//12 TAD for acquisition 
	ADCON1bits.ADCS = 0b110;	//conversion clock is FOSC/64 (48MHz clock)
	
	//set current sense input and an3 as analog inputs
	CSENSE_IO = 0;
	ANCON0bits.PCFG3 = 0;
	TRISAbits.TRISA3 = 1;
	TRISCbits.TRISC2 = 1;
	
	ANCON1bits.VBGEN = 1;	//enable band gap reference
	delay_ms(10);			//allow bg to stabilize
	calibrateADC();		//run self-calibration on pic
	mVbg = 1200; //getVbgmV();	// for current chip using getVbgmV();
	
	
    while(1)		//Main Loop
	{
		
			current = getCurrentuA();
			
				Vdd = getVdd();
				mVbat = getVoutmV();
				current = getCurrentuA();
				if(getVddmV() < 2300)
					sprintf((char *)&(LCDText[26]), (far rom char*)"LOWBAT");
				else
					sprintf((char *)&(LCDText[26]), (far rom char*)"      ");
				
  		


  		
 	}     
  
}



BYTE getPotInput(void)
{
	BYTE value;
	
	ADCON1bits.ADFM = 0;  //Left Justified; just want MS8b
	VREF = VDDREF;
	VCHAN = BOARD_POT;
	ADCON0bits.GO_DONE = 1;
	while(ADCON0bits.GO_DONE);
	ADCON1bits.ADFM = 1;  //back to Right Justified
	return (255 - ADRESH);	//invert pot value (makes more sense on custom board)
	
}	

unsigned int getVbgmV(void)
{
	int adresult;
	float voltage;
	unsigned int mV;
	
	VCHAN = BG;
	VREF = VDDREF;

	while(!HLVDCONbits.BGVST);	//wait for stable BG voltage
	adresult = getADCVal();
  	
  	VREF = INPUTREF;
    voltage = (float)adresult/1024 * EXPVDD;
    mV = (unsigned int)(1000*voltage + 0.5);
    return mV;
	

}

unsigned int getVddmV(void)
{
	int adresult = 1024;		//avoid div/0
	float voltage;
	unsigned int mV;
	VCHAN = BG;
	VREF = VDDREF;
	while(!HLVDCONbits.BGVST);	//wait for stable BG voltage
	ADCON0bits.GO_DONE = 1;
	while(ADCON0bits.GO_DONE);
   	adresult = ADRESH;
   	adresult = adresult << 8;
	adresult |= ADRESL;
	VREF = INPUTREF;
	voltage = mVbg*1024.0/adresult;
	mV = (unsigned int)(voltage + 0.5);
	return mV;
}

float getVdd(void)
{
	int adresult = 1024;		//avoid div/0
	float voltage;
	unsigned int mV;
	VCHAN = BG;
	VREF = VDDREF;
	while(!HLVDCONbits.BGVST);	//wait for stable BG voltage
	ADCON0bits.GO_DONE = 1;
	while(ADCON0bits.GO_DONE);
   	adresult = ADRESH;
   	adresult = adresult << 8;
	adresult |= ADRESL;
	VREF = INPUTREF;
	
	voltage = (((float) mVbg)/1000)*1024.0/adresult;
	
	return voltage;
}

unsigned int getVoutmV(void)
{
	int adresult = 1024;		//avoid div/0
	float voltage;
	unsigned int mV;
	VCHAN = BATT;	
	VREF = VDDREF;
	
   	adresult = getADCVal();
	
	voltage = ((float)adresult)/1024 * Vdd * UNDIVIDE * 1000;
	//voltage = ((float)adresult)/1024 * EXPVDD * UNDIVIDE * 1000;
	mV = (unsigned int)(voltage + 0.5);
	return mV;
}

unsigned int getCurrentuA(void)
{
	unsigned int uA;
	float voltage;
	int adresult;
	VCHAN = CSENSE;
	VREF = VDDREF;
	
   	adresult = getADCVal();
	
	voltage = ((float)adresult)/1024*Vdd*1000;
	uA = (unsigned int)(1000*voltage / RESVAL + 0.5);
	
	return uA;
}

int getADCVal(void)
{
	int adresult = 1024;		//avoid div/0
	float voltage;
	unsigned int mV;
	
	
	ADCON0bits.GO_DONE = 1;
	while(ADCON0bits.GO_DONE);
   	adresult = ADRESH;
   	adresult = adresult << 8;
	adresult |= ADRESL;
	
	return adresult;
}

void calibrateADC(void)
{
	ADCON1bits.ADCAL = 1;
	
	ADCON0bits.GO_DONE = 1;
	while(ADCON0bits.GO_DONE);
   	
	ADCON1bits.ADCAL = 0;
	
	return;
}
	

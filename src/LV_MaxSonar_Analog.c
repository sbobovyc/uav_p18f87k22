#include <delays.h>
#include <math.h>
#include <timers.h>
#include <adc.h>
#include "LV_MaxSonar_Analog.h"
#include "cpu_properties.h"

int LV_MAXSONAR_raw_to_inches(int reading)
{
		float temp;
		temp = ADC_REF_VOLTAGE*(float)reading / ADC_MAX;
		return (int)((float)temp * 1000 / SONAR_MV_PER_INCH);
}

void LV_MAXSONAR_init(void)
{
	// let sonar initialize, delay 250 ms
	Delay1KTCYx(625);

	SONAR0_READ_TRI = 1;	// data pin input
	SONAR0_MEASURE = 0;	// 0 stops sonar pulse, 1 starts sonar pulse
	SONAR0_MEASURE_TRI = 0; // measure signal pin output

	//SONAR1_READ_TRI = 1;	// data pin input
	//SONAR1_MEASURE = 0;	// 0 stops sonar pulse, 1 starts sonar pulse
	//SONAR1_MEASURE_TRI = 0; // measure signal pin output
	
}

void LV_MAXSONAR_measure(int * distances, unsigned char mode)
{

	//unsigned char config1 = ADC_FOSC_RC & ADC_RIGHT_JUST & ADC_8ANA_0REF; // on pic18f452
	unsigned char config1 = ADC_FOSC_RC & ADC_RIGHT_JUST; // on pic18f87k22
	unsigned char config2 = ADC_CH7 & ADC_INT_OFF;
	unsigned char portconfig = 15; // on pic18f87k22
	/*
	ADCON0bits.ADCS = 0b11;	//set timer to internal RC (best for ADC)
	ADCON0bits.CHS = SONAR_ADC_CHAN;	//analog input at E2
	ADCON1bits.ADFM = 1;	//right-justified
	ADCON1bits.PCFG = 0;	//only config with Vdd ref and E2 enabled
	ADCON0bits.ADON = 1;	//enable ADC
	*/
	int temp_result[6] = {0,0,0,0,0,0};
	float temp;
		

	OpenADC(config1, config2, portconfig);
	// read Sonar 0
	SetChanADC(SONAR0_ADC_CHAN);	// set the channel
	SONAR0_MEASURE = 0;	// allow measurement
	Delay10TCYx(9);	//20 uS for sig to sensor; 16 for ADC
	ConvertADC();         // Start conversion
	while( BusyADC() );   // Wait for completion
	SONAR0_MEASURE = 1;		// stop measurement
	temp_result[0] = ReadADC();		// raw output from adc

	/*
	// read Sonar 1
	SetChanADC(SONAR1_ADC_CHAN);	// set the channel
	SONAR0_MEASURE = 0;	// allow measurement
	Delay10TCYx(9);	//20 uS for sig to sensor; 16 for ADC
	ConvertADC();         // Start conversion
	while( BusyADC() );   // Wait for completion
	SONAR0_MEASURE = 1;		// stop measurement
	temp_result[1] = ReadADC();		// raw output from adc

	// read Sonar 2
	SetChanADC(SONAR2_ADC_CHAN);	// set the channel
	SONAR0_MEASURE = 0;	// allow measurement
	Delay10TCYx(9);	//20 uS for sig to sensor; 16 for ADC
	ConvertADC();         // Start conversion
	while( BusyADC() );   // Wait for completion
	SONAR0_MEASURE = 1;		// stop measurement
	temp_result[2] = ReadADC();		// raw output from adc

	// read Sonar 3
	SetChanADC(SONAR3_ADC_CHAN);	// set the channel
	SONAR0_MEASURE = 0;	// allow measurement
	Delay10TCYx(9);	//20 uS for sig to sensor; 16 for ADC
	ConvertADC();         // Start conversion
	while( BusyADC() );   // Wait for completion
	SONAR0_MEASURE = 1;		// stop measurement
	temp_result[3] = ReadADC();		// raw output from adc

	// read Sonar 4
	SetChanADC(SONAR4_ADC_CHAN);	// set the channel
	SONAR0_MEASURE = 0;	// allow measurement
	Delay10TCYx(9);	//20 uS for sig to sensor; 16 for ADC
	ConvertADC();         // Start conversion
	while( BusyADC() );   // Wait for completion
	SONAR0_MEASURE = 1;		// stop measurement
	temp_result[4] = ReadADC();		// raw output from adc

	// read Sonar 5
	SetChanADC(SONAR5_ADC_CHAN);	// set the channel
	SONAR0_MEASURE = 0;	// allow measurement
	Delay10TCYx(9);	//20 uS for sig to sensor; 16 for ADC
	ConvertADC();         // Start conversion
	while( BusyADC() );   // Wait for completion
	SONAR0_MEASURE = 1;		// stop measurement
	temp_result[5] = ReadADC();		// raw output from adc
	*/
	CloseADC();

	if(mode == SONAR_INCHES_MODE)
	{
		distances[0] = LV_MAXSONAR_raw_to_inches(temp_result[0]);
		distances[1] = LV_MAXSONAR_raw_to_inches(temp_result[1]);
		distances[2] = LV_MAXSONAR_raw_to_inches(temp_result[2]);
		distances[3] = LV_MAXSONAR_raw_to_inches(temp_result[3]);
		distances[4] = LV_MAXSONAR_raw_to_inches(temp_result[4]);
		distances[5] = LV_MAXSONAR_raw_to_inches(temp_result[5]);
	} 
	else 
	{
		distances[0] = temp_result[0];
		distances[1] = temp_result[1];
		distances[2] = temp_result[2];
		distances[3] = temp_result[3];
		distances[4] = temp_result[4];
		distances[5] = temp_result[5];
	}
}
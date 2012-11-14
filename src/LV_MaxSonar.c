#include <delays.h>
#include <math.h>
#include <timers.h>
#include "LV_MaxSonar.h"

void LV_MAXSONAR_init(void)
{
	// let sonar initialize, delay 250 ms
	Delay1KTCYx(625);
	SONIC_SENSOR_PW_TRI = 1;	// data pin input
}

int LV_MAXSONAR_measure(void)
{
	//timer vars
	unsigned int result;
	float temp_result;
	while(1)
	{
		while(SONIC_SENSOR_PW_PIN == 0);
		// configure timer0
		OpenTimer0(TIMER_INT_OFF & T0_16BIT & T0_SOURCE_INT & T0_PS_1_1);
 		WriteTimer0( 0 );         // restart timer
		while(PORTBbits.RB3 == 1);
		result = ReadTimer0();    // read timer
		temp_result = (float)result / (float)(TIMER_SCALE_FACTOR*147);
		break;
	}
	result = (int)temp_result;
	return result;
}
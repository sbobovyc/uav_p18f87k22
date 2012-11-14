/**
 * @section LICENSE
 *
 * Written by Stanislav Bobovych
 *
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * @section DESCRIPTION
 *
 * LV-Max-Sonar-EZ1, ranger finder.
 *
 * Datasheet:
 *
 * www.maxbotix.com/uploads/LV-MaxSonar-EZ1-Datasheet.pdf 
 */  
#ifndef LV_MAXSONAR_H
#define LV_MAXSONAR_H

//TODO Set pins for different sonar sensors
/**
 * Defines
 */
#define SONAR_RAW_MODE		0
#define SONAR_INCHES_MODE	1

#define SONAR0_READ			LATEbits.LATE2
#define SONAR0_READ_TRI		TRISEbits.TRISE2
#define SONAR0_MEASURE		PORTBbits.RB3
#define SONAR0_MEASURE_TRI	TRISBbits.TRISB3

#define SONAR1_READ			LATEbits.LATE2
#define SONAR1_READ_TRI		TRISEbits.TRISE2
#define SONAR1_MEASURE		PORTBbits.RB3
#define SONAR1_MEASURE_TRI	TRISBbits.TRISB3

#define SONAR2_READ			LATEbits.LATE2
#define SONAR2_READ_TRI		TRISEbits.TRISE2
#define SONAR2_MEASURE		PORTBbits.RB3
#define SONAR2_MEASURE_TRI	TRISBbits.TRISB3

#define SONAR3_READ			LATEbits.LATE2
#define SONAR3_READ_TRI		TRISEbits.TRISE2
#define SONAR3_MEASURE		PORTBbits.RB3
#define SONAR3_MEASURE_TRI	TRISBbits.TRISB3

#define SONAR4_READ			LATEbits.LATE2
#define SONAR4_READ_TRI		TRISEbits.TRISE2
#define SONAR4_MEASURE		PORTBbits.RB3
#define SONAR4_MEASURE_TRI	TRISBbits.TRISB3

#define SONAR5_READ			LATEbits.LATE2
#define SONAR5_READ_TRI		TRISEbits.TRISE2
#define SONAR5_MEASURE		PORTBbits.RB3
#define SONAR5_MEASURE_TRI	TRISBbits.TRISB3

#define SONAR0_ADC_CHAN		ADC_CH7
#define SONAR1_ADC_CHAN		ADC_CH6
#define SONAR2_ADC_CHAN		ADC_CH7
#define SONAR3_ADC_CHAN		ADC_CH6
#define SONAR4_ADC_CHAN		ADC_CH7
#define SONAR5_ADC_CHAN		ADC_CH7

#ifndef THREE_VOLT_SYS
	#define SONAR_MV_PER_INCH 9.8
#else
	#define SONAR_MV_PER_INCH 6.8	
#endif

/// ADC scale factor
#define SONAR0_ADC_SCALE_FACTOR
#define SONAR1_ADC_SCALE_FACTOR
#define SONAR2_ADC_SCALE_FACTOR
#define SONAR3_ADC_SCALE_FACTOR
#define SONAR4_ADC_SCALE_FACTOR
#define SONAR5_ADC_SCALE_FACTOR

/**
 * Initialize the all sonar range finders.
 *
 */
void LV_MAXSONAR_init(void);

/**
 * Measure distance to object.
 * @param distances pointer to array that contains sonar readings (measured in inches)
 * @param mode of measurement 
 */
void LV_MAXSONAR_measure(int * distances, unsigned char mode);
#endif /* ADXL345_H */
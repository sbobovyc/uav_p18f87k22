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
 * LV-Max-Sonar-EZ0, ranger finder.
 *
 * Datasheet:
 *
 * www.maxbotix.com/uploads/LV-MaxSonar-EZ1-Datasheet.pdf 
 */  
#ifndef LV_MAXSONAR_H
#define LV_MAXSONAR_H


/**
 * Defines
 */

#define SONIC_SENSOR_PW_PIN	PORTBbits.RB3
#define SONIC_SENSOR_PW_TRI	TRISBbits.TRISB3

// This measurement is in microseconds and should be close to execution time
// of one instruction cycle.
#define TIMER_SCALE_FACTOR	2.3	

// This measurement is in microseconds.
// PW pin outputs a pulse width representation of range.  
// The distance can be calculated using the scale factor of 147uS per inch.
// Adjust as necassery. 
#define PW_SCALE_FACTOR	147


/**
 * Initialize the sonar range finder.
 *
 */
void LV_MAXSONAR_init(void);

/**
 * Measure distance to object.
 *
 * @return Distance to object in inches.
 */
int LV_MAXSONAR_measure(void);
#endif /* ADXL345_H */
/**
 * This file contains the hardware specifics of the microcontroller.
 * @file cpu_properties.h
 *
 * @date Jan 25, 2011
 * @author Stanislav Bobovych
 * @details
 * TODO: put the link to the PIC's data sheet here
 */

#ifndef CPU_PROPERTIES_H_
#define CPU_PROPERTIES_H_

/**
 * @details
 * If a PIC uses a 12 bit ADC, 2^12 - 1 = 4095
 * If a PIC uses a 10 bit ADC, 2^10 - 1 = 1023
 */
#define ADC_MAX 1023
#define ADC_REF_VOLTAGE 5	///< This PIC uses a 5 volt power supply.
#define OSC 10000000		///< Operating frequency in Hz, in this case 10 MHz
#define CLK_PERIOD 0.0000001	///< Period in seconds

#define M_PI 3.14	///< Definition of PI
#endif /* CPU_PROPERTIES_H_ */

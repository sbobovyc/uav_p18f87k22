/**
 * @author Aaron Berk
 * Edited for C by Brian Wolff 
 *
 * @section LICENSE
 *
 * Copyright (c) 2010 ARM Limited
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
 * Honeywell HMC6352 digital compass.
 *
 * Datasheet:
 *
 * http://www.ssec.honeywell.com/magnetic/datasheets/HMC6352.pdf
 */

/**
 * Includes
 */
#include "HMC6352.h"
#include "i2c.h"

int operationMode_ = 0;
int data = 0;

/* int HMC6352_getSlaveAddress()
 *
 * returns the address of the compass
 *
*/
int HMC6352_getSlaveAddress(){
	StartI2C();
	WriteI2C((HMC6352_I2C_ADDRESS << 1) & 0xFE);
	WriteI2C(HMC6352_EEPROM_READ);
	WriteI2C(HMC6352_SLAVE_ADDR);
	RestartI2C();
	WriteI2C((HMC6352_I2C_ADDRESS << 1) | 1);
	data = ReadI2C();
	NotAckI2C();
	StopI2C();
	return data;
}

/* int HM6352_sample()
*
* returns the Heading dependent on axis
*/
int HMC6352_sample(void) {

	int outputData[2];
	
	StartI2C();
    	if (operationMode_ == HMC6352_STANDBY || operationMode_ == HMC6352_QUERY) {

		WriteI2C((HMC6352_I2C_ADDRESS << 1) & 0xFE);
		WriteI2C(HMC6352_GET_DATA);
		RestartI2C();
		WriteI2C((HMC6352_I2C_ADDRESS << 1) | 1);
		outputData[0] = ReadI2C();
		AckI2C();
		outputData[1] = ReadI2C();
		NotAckI2C();
    	}
	StopI2C();

    return ((outputData[0] << 8) | outputData[1]);

}

/*
*HMC6352_getOutputMode(void)
*
* sends the 'a' command to read from the ram address
* 0x41 which is the Output Mode register
* 
* returns the current Output Mode
*/
int HMC6352_getOutputMode(void){
	int outputData;
	StartI2C();
	WriteI2C((HMC6352_I2C_ADDRESS<<1) & 0xFE);
	WriteI2C(HMC6352_RAM_READ);
	WriteI2C(HMC6352_RAM_OUTPUT);
	RestartI2C();
	WriteI2C((HMC6352_I2C_ADDRESS << 1) | 1);
	data = ReadI2C();
	NotAckI2C();
	StopI2C();
	return data;	
}

/* HMC6352_setOutputMode(int mode)
 *
 * sets the Output Mode to mode
 *
 * mode = 	0x00 -> Heading mode (default)
 *		0x01 -> Raw magnetometer X mode
 *		0x02 -> Raw magnetometer Y mode
 * 		0x03 -> Magnetometer X mode
 *		0x04 -> Magnetometer Y mode
*/
void HMC6352_setOutputMode(int mode){
	StartI2C();
	WriteI2C((HMC6352_I2C_ADDRESS << 1) & 0xFE);
	WriteI2C(HMC6352_RAM_WRITE);
	WriteI2C(HMC6352_RAM_OUTPUT);
	WriteI2C(mode);
	StopI2C();
	operationMode_ = HMC6352_getOpMode();
}


/* int HMC6352_getOpMode()
 * 
 * returns the current Operation Mode
*/
int HMC6352_getOpMode(){
	StartI2C();
	WriteI2C((HMC6352_I2C_ADDRESS << 1) & 0xFE);
	WriteI2C(HMC6352_EEPROM_READ);
	WriteI2C(HMC6352_OPMODE);
	RestartI2C();
	WriteI2C((HMC6352_I2C_ADDRESS << 1) | 1);
	data = ReadI2C();
	NotAckI2C();
	StopI2C();
	operationMode_ = data;
	return data;
	
}


/* Set the operation mode.
 *
 * @param mode 0x00 -> Standby mode
 *             0x01 -> Query mode
 *             0x02 -> Continuous mode
 * @param periodicSetReset 0x00 -> No periodic set/reset
 *                         0x01 -> Periodic set/reset
 * @measurementRate Measurement rate in Hz for continuous rate.
 *                  Possible rates: {1, 5, 10, 20}Hz.
*/
void HMC6352_setOpMode(int mode, int periodicSetReset, int measurementRate) {

    char opModeByte = mode;

    if (periodicSetReset == 1) {
        opModeByte |= HMC6352_PERIODIC_SR;
    }

    if (measurementRate == 5) {
        opModeByte |= HMC6352_CM_MR_5HZ;
    } else if (measurementRate == 10) {
        opModeByte |= HMC6352_CM_MR_10HZ;
    } else if (measurementRate == 20) {
        opModeByte |= HMC6352_CM_MR_20HZ;
    }

	StartI2C();
	WriteI2C((HMC6352_I2C_ADDRESS << 1) & 0xFE);
	WriteI2C(HMC6352_RAM_WRITE);
	WriteI2C(HMC6352_RAM_OPMODE);
	WriteI2C(opModeByte);
	StopI2C();
	HMC6352_saveOpMode();
	//printf("mode:0x%02H Periodic Set/Reset:0x%02H Measurement:0x%02H", mode, periodicSetReset, measurementRate);
    operationMode_ = mode;
}

/* HMC6352_saveOpMode()
 *
 * saves the current RAM value of Operation Mode into EEPROM
 *
*/
void HMC6352_saveOpMode(){
	StartI2C();
	WriteI2C((HMC6352_I2C_ADDRESS << 1) & 0xFE);
	WriteI2C(HMC6352_SAVE_OPMODE);
	StopI2C();
}
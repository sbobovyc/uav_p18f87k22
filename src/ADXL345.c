/**
 * @file ADXL345.c
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
 * ADXL345, triple axis, digital interface, accelerometer.
 *
 * Datasheet:
 *
 * http://www.analog.com/static/imported-files/data_sheets/ADXL345.pdf
 */  
 
/**
 * Includes
 */
#include "ADXL345.h"
#include "serial_com.h"
#include <i2c.h>
#include <spi.h>
#include <delays.h>
#include <stdio.h>

unsigned char ADXL345_getDevId(void) 
{
    return ADXL345_oneByteRead(ADXL345_DEVID_REG);
}

unsigned char ADXL345_getTapThreshold(void) 
{
    return ADXL345_oneByteRead(ADXL345_THRESH_TAP_REG);
}

void ADXL345_setTapThreshold(unsigned char threshold) 
{
    ADXL345_oneByteWrite(ADXL345_THRESH_TAP_REG, threshold);
}

unsigned char ADXL345_getOffset(unsigned char axis) {

    int address = 0;

    if (axis == ADXL345_X) {
        address = ADXL345_OFSX_REG;
    } else if (axis == ADXL345_Y) {
        address = ADXL345_OFSY_REG;
    } else if (axis == ADXL345_Z) {
        address = ADXL345_OFSZ_REG;
    }

    return ADXL345_oneByteRead(address);

}

void ADXL345_setOffset(unsigned char axis, unsigned char offset) {

    int address = 0;

    if (axis == ADXL345_X) {
        address = ADXL345_OFSX_REG;
    } else if (axis == ADXL345_Y) {
        address = ADXL345_OFSY_REG;
    } else if (axis == ADXL345_Z) {
        address = ADXL345_OFSZ_REG;
    }

    ADXL345_oneByteWrite(address, offset);

}

int ADXL345_getTapDuration(void) {

    return ADXL345_oneByteRead(ADXL345_DUR_REG)*625;

}

void ADXL345_setTapDuration(int duration_us) {

    int tapDuration = duration_us / 625;

    ADXL345_oneByteWrite(ADXL345_DUR_REG, tapDuration);

}

float ADXL345_getTapLatency(void) {

    return ADXL345_oneByteRead(ADXL345_LATENT_REG)*1.25;

}

void ADXL345_setTapLatency(int latency_ms) {

    int tapLatency = latency_ms / 1.25;

    ADXL345_oneByteWrite(ADXL345_LATENT_REG, tapLatency);

}

float ADXL345_getWindowTime(void) {

    return ADXL345_oneByteRead(ADXL345_WINDOW_REG)*1.25;

}

void ADXL345_setWindowTime(int window_ms) {

    int windowTime = window_ms / 1.25;

    ADXL345_oneByteWrite(ADXL345_WINDOW_REG, windowTime);

}

int ADXL345_getActivityThreshold(void) {

    return ADXL345_oneByteRead(ADXL345_THRESH_ACT_REG);

}

void ADXL345_setActivityThreshold(unsigned char threshold) {

    ADXL345_oneByteWrite(ADXL345_THRESH_ACT_REG, threshold);

}

int ADXL345_getInactivityThreshold(void) {

    return ADXL345_oneByteRead(ADXL345_THRESH_INACT_REG);

}

void ADXL345_setInactivityThreshold(unsigned char threshold) {

    ADXL345_oneByteWrite(ADXL345_THRESH_INACT_REG, threshold);

}

int ADXL345_getTimeInactivity(void) {

    return ADXL345_oneByteRead(ADXL345_TIME_INACT_REG);

}

void ADXL345_setTimeInactivity(unsigned char timeInactivity) {

    ADXL345_oneByteWrite(ADXL345_TIME_INACT_REG, timeInactivity);

}

int ADXL345_getActivityInactivityControl(void) {

    return ADXL345_oneByteRead(ADXL345_ACT_INACT_CTL_REG);

}

void ADXL345_setActivityInactivityControl(int settings) {

    ADXL345_oneByteWrite(ADXL345_ACT_INACT_CTL_REG, settings);

}

int ADXL345_getFreefallThreshold(void) {

    return ADXL345_oneByteRead(ADXL345_THRESH_FF_REG);

}

void ADXL345_setFreefallThreshold(int threshold) {

    ADXL345_oneByteWrite(ADXL345_THRESH_FF_REG, threshold);

}

int ADXL345_getFreefallTime(void) {

    return ADXL345_oneByteRead(ADXL345_TIME_FF_REG)*5;

}

void ADXL345_setFreefallTime(int freefallTime_ms) {

    int freefallTime = freefallTime_ms / 5;

    ADXL345_oneByteWrite(ADXL345_TIME_FF_REG, freefallTime);

}

int ADXL345_getTapAxisControl(void) {

    return ADXL345_oneByteRead(ADXL345_TAP_AXES_REG);

}

void ADXL345_setTapAxisControl(int settings) {

    ADXL345_oneByteWrite(ADXL345_TAP_AXES_REG, settings);

}

int ADXL345_getTapSource(void) {

    return ADXL345_oneByteRead(ADXL345_ACT_TAP_STATUS_REG);

}

void ADXL345_setPowerMode(char mode) {

    //Get the current register contents, so we don't clobber the rate value.
    char registerContents = ADXL345_oneByteRead(ADXL345_BW_RATE_REG);

    registerContents = (mode << 4) | registerContents;

    ADXL345_oneByteWrite(ADXL345_BW_RATE_REG, registerContents);

}

int ADXL345_getPowerControl(void) {

    return ADXL345_oneByteRead(ADXL345_POWER_CTL_REG);

}

void ADXL345_setPowerControl(unsigned char settings) {

    ADXL345_oneByteWrite(ADXL345_POWER_CTL_REG, settings);

}

int ADXL345_getInterruptEnableControl(void) {

    return ADXL345_oneByteRead(ADXL345_INT_ENABLE_REG);

}

void ADXL345_setInterruptEnableControl(unsigned char settings) {

    ADXL345_oneByteWrite(ADXL345_INT_ENABLE_REG, settings);

}

int ADXL345_getInterruptMappingControl(void) {

    return ADXL345_oneByteRead(ADXL345_INT_MAP_REG);

}

void ADXL345_setInterruptMappingControl(int settings) {

    ADXL345_oneByteWrite(ADXL345_INT_MAP_REG, settings);

}

int ADXL345_getInterruptSource(void){

    return ADXL345_oneByteRead(ADXL345_INT_SOURCE_REG);

}

int ADXL345_getDataFormatControl(void){

    return ADXL345_oneByteRead(ADXL345_DATA_FORMAT_REG);

}

void ADXL345_setDataFormatControl(unsigned char settings){

    ADXL345_oneByteWrite(ADXL345_DATA_FORMAT_REG, settings);

}

void ADXL345_setDataRate(int rate) {

    //Get the current register contents, so we don't clobber the power bit.
    char registerContents = ADXL345_oneByteRead(ADXL345_BW_RATE_REG);

    registerContents &= 0x10;
    registerContents |= rate;

    ADXL345_oneByteWrite(ADXL345_BW_RATE_REG, registerContents);

}

void ADXL345_getOutput(int * readings){

    unsigned char buffer[6];
    
    ADXL345_multiByteRead(ADXL345_DATAX0_REG, buffer, 6);
    
    readings[0] = (int)buffer[1] << 8;
	readings[0] |= (int)buffer[0];
    readings[1] = ((int)buffer[3] << 8);
	readings[1] |= (int)buffer[2];
    readings[2] = ((int)buffer[5] << 8); 
	readings[2] |= (int)buffer[4];

}

int ADXL345_getFifoControl(void){

    return ADXL345_oneByteRead(ADXL345_FIFO_CTL);

}

void ADXL345_setFifoControl(int settings){

    ADXL345_oneByteWrite(ADXL345_FIFO_STATUS, settings);

}

char ADXL345_getFifoStatus(void){

    return ADXL345_oneByteRead(ADXL345_FIFO_STATUS);

}


unsigned char ADXL345_oneByteRead(unsigned char address) {
	unsigned char data = 0;

	#if I2C
	    StartI2C();
		WriteI2C(ADXL343_ADDR_WRITE); 
		WriteI2C(address);
		RestartI2C();
		WriteI2C(ADXL343_ADDR_READ);
		data = ReadI2C();
		NotAckI2C();
		StopI2C();
		return data;
	#elif SPI
		SPI_CS_PIN = 0;	//CS pin low, ie enable chip
		Delay1TCY();		// delay at least 5 ns
		address = address | ADXL345_SPI_READ;
		WriteSPI(address);		// read bit, multibyte bit, A5-A0 address
		data = ReadSPI();
		SPI_CS_PIN = 1;	//CS pin high, ie disable chip
		Delay1TCY();		// delay at least 5 ns
		return data;
	#endif
}


void ADXL345_oneByteWrite(unsigned char address, unsigned char data) 
{
	#if I2C
		StartI2C();
		WriteI2C(ADXL343_ADDR_WRITE); // control byte
		WriteI2C(address); // word address
		WriteI2C(data); // data
		StopI2C();
	#elif SPI
		SPI_CS_PIN = 0;	//CS pin low, ie enable chip
		Delay1TCY();		// delay at least 5 ns
		address = address | ADXL345_SPI_WRITE;
		WriteSPI(address);		// write bit, multibyte bit, A5-A0 address
		WriteSPI(data);		
		SPI_CS_PIN = 1;	//CS pin high, ie disable chip
		Delay1TCY();		// delay at least 5 ns
	#endif
}

void ADXL345_multiByteRead(unsigned char startAddress, char* buffer, unsigned char size) {

    
	#if I2C
		unsigned char i;
		StartI2C();
		WriteI2C(ADXL343_ADDR_WRITE); 
		WriteI2C(startAddress);
		RestartI2C();
		WriteI2C(ADXL343_ADDR_READ);

	    for (i = 0; i < size; i++) {
	        buffer[i] = ReadI2C();	//keep the clock pulsing
			// if not last byte, send ack
			// if last byte, send nack
			if(i < size-1)
			{
				AckI2C();
			} 
			else
			{
				NotAckI2C();
			}
	    }
		StopI2C();
    #elif SPI
		unsigned char tx = (ADXL345_SPI_READ | ADXL345_MULTI_BYTE | (startAddress & 0x3F));	// the &0x3F restricts reading from only the XYZ data registers
		unsigned char i;
		SPI_CS_PIN = 0;	//CS pin low, ie enable chip
		Delay1TCY();	// delay at least 5 ns
		WriteSPI(tx);	//Send address to start reading from.
		

	    for (i = 0; i < size; i++) {
	        buffer[i] = ReadSPI();	//keep the clock pulsing
	    }

		SPI_CS_PIN = 1;	//CS pin high, ie disable chip
	#endif
}

void ADXL345_multiByteWrite(unsigned char startAddress, char* buffer, unsigned char size) {

	#if I2C
		unsigned char i;
		StartI2C();
		WriteI2C(ADXL343_ADDR_WRITE); 
		WriteI2C(startAddress);


	    for (i = 0; i < size; i++) {
			WriteI2C(buffer[i]);
	    }
		StopI2C();
	#elif SPI
    	unsigned char tx = (ADXL345_SPI_WRITE | ADXL345_MULTI_BYTE | (startAddress & 0x3F));
		unsigned char i;
		SPI_CS_PIN = 0;	//CS pin low, ie enable chip
		Delay1TCY();	// delay at least 5 ns
		WriteSPI(tx);	//Send starting write address.
		

	    for (i = 0; i < size; i++) {
	        WriteSPI(buffer[i]);
	    }

		SPI_CS_PIN = 1;	//CS pin high, ie disable chip
	#endif
}

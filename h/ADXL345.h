/**
 * @file ADXL345.h
 * @section LICENSE
 *
 * Ported to C by Stanislav Bobovych
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

#ifndef ADXL345_H
#define ADXL345_H

/**
 * Defines
 */
// I2C addresses
#ifdef ADXL345_ALT_ADDR_IS_HIGH
	#define ADXL345_ALT_ADDR		0x1D	///<I2C address of device when SDO/ALT ADDRESS pin is high
	#define ADXL343_ADDR_WRITE		0x3A ///<I2C address for write
	#define ADXL343_ADDR_READ		0x3B ///<I2C address for read
#else
	#define ADXL343_ALT_ADDR		0x53 ///<I2C address of device when SDO/ALT ADDRESS pin is low
	#define ADXL343_ADDR_WRITE		0xA6 ///<I2C address for write
	#define ADXL343_ADDR_READ		0xA7 ///<I2C address for read
#endif

//Registers.
#define ADXL345_DEVID_REG          0x00
#define ADXL345_THRESH_TAP_REG     0x1D
#define ADXL345_OFSX_REG           0x1E
#define ADXL345_OFSY_REG           0x1F
#define ADXL345_OFSZ_REG           0x20
#define ADXL345_DUR_REG            0x21
#define ADXL345_LATENT_REG         0x22
#define ADXL345_WINDOW_REG         0x23
#define ADXL345_THRESH_ACT_REG     0x24
#define ADXL345_THRESH_INACT_REG   0x25
#define ADXL345_TIME_INACT_REG     0x26
#define ADXL345_ACT_INACT_CTL_REG  0x27
#define ADXL345_THRESH_FF_REG      0x28
#define ADXL345_TIME_FF_REG        0x29
#define ADXL345_TAP_AXES_REG       0x2A
#define ADXL345_ACT_TAP_STATUS_REG 0x2B
#define ADXL345_BW_RATE_REG        0x2C
#define ADXL345_POWER_CTL_REG      0x2D
#define ADXL345_INT_ENABLE_REG     0x2E
#define ADXL345_INT_MAP_REG        0x2F
#define ADXL345_INT_SOURCE_REG     0x30
#define ADXL345_DATA_FORMAT_REG    0x31
#define ADXL345_DATAX0_REG         0x32
#define ADXL345_DATAX1_REG         0x33
#define ADXL345_DATAY0_REG         0x34
#define ADXL345_DATAY1_REG         0x35
#define ADXL345_DATAZ0_REG         0x36
#define ADXL345_DATAZ1_REG         0x37
#define ADXL345_FIFO_CTL           0x38
#define ADXL345_FIFO_STATUS        0x39

//Data rate codes.
#define ADXL345_3200HZ      0x0F
#define ADXL345_1600HZ      0x0E
#define ADXL345_800HZ       0x0D
#define ADXL345_400HZ       0x0C
#define ADXL345_200HZ       0x0B
#define ADXL345_100HZ       0x0A
#define ADXL345_50HZ        0x09
#define ADXL345_25HZ        0x08
#define ADXL345_12HZ5       0x07
#define ADXL345_6HZ25       0x06

#define ADXL345_SPI_READ    0x80
#define ADXL345_SPI_WRITE   0x00
#define ADXL345_MULTI_BYTE  0x60

#define ADXL345_X           0x00
#define ADXL345_Y           0x01
#define ADXL345_Z           0x02


/**
 * Read the device ID register on the device.
 *
 * @return The device ID code [0xE5]
 */
unsigned char ADXL345_getDevId(void);

/**
 * Read the tap threshold on the device.
 *
 * @return The tap threshold as an 8-bit number with a scale factor of
 *         62.5mg/LSB.
 */
unsigned char ADXL345_getTapThreshold(void);

/**
 * Set the tap threshold.
 *
 * @param The tap threshold as an 8-bit number with a scale factor of
 *        62.5mg/LSB.
 */
void ADXL345_setTapThreshold(unsigned char threshold);

/**
 * Get the current offset for a particular axis.
 *
 * @param axis 0x00 -> X-axis
 *             0x01 -> Y-axis
 *             0x02 -> Z-axis
 * @return The current offset as an 8-bit 2's complement number with scale
 *         factor 15.6mg/LSB.
 */
unsigned char ADXL345_getOffset(unsigned char axis);

/**
 * Set the offset for a particular axis.
 *
 * @param axis 0x00 -> X-axis
 *             0x01 -> Y-axis
 *             0x02 -> Z-axis
 * @param offset The offset as an 8-bit 2's complement number with scale
 *               factor 15.6mg/LSB.
 */
void ADXL345_setOffset(unsigned char axis, unsigned char offset);

/**
 * Get the tap duration required to trigger an event.
 *
 * @return The max time that an event must be above the tap threshold to
 *         qualify as a tap event, in microseconds.
 */
int ADXL345_getTapDuration(void);

/**
 * Set the tap duration required to trigger an event.
 *
 * @param duration_us The max time that an event must be above the tap
 *                    threshold to qualify as a tap event, in microseconds.
 *                    Time will be normalized by the scale factor which is
 *                    625us/LSB. A value of 0 disables the single/double
 *                    tap functions.
 */
void ADXL345_setTapDuration(int duration_us);

/**
 * Get the tap latency between the detection of a tap and the time window.
 *
 * @return The wait time from the detection of a tap event to the start of
 *         the time window during which a possible second tap event can be
 *         detected in milliseconds.
 */
float ADXL345_getTapLatency(void);

/**
 * Set the tap latency between the detection of a tap and the time window.
 *
 * @param latency_ms The wait time from the detection of a tap event to the
 *                   start of the time window during which a possible
 *                   second tap event can be detected in milliseconds.
 *                   A value of 0 disables the double tap function.
 */
void ADXL345_setTapLatency(int latency_ms);

/**
 * Get the time of window between tap latency and a double tap.
 *
 * @return The amount of time after the expiration of the latency time
 *         during which a second valid tap can begin, in milliseconds.
 */
float ADXL345_getWindowTime(void);

/**
 * Set the time of the window between tap latency and a double tap.
 *
 * @param window_ms The amount of time after the expiration of the latency
 *                  time during which a second valid tap can begin,
 *                  in milliseconds.
 */
void ADXL345_setWindowTime(int window_ms);

/**
 * Get the threshold value for detecting activity.
 *
 * @return The threshold value for detecting activity as an 8-bit number.
 *         Scale factor is 62.5mg/LSB.
 */
int ADXL345_getActivityThreshold(void);

/**
 * Set the threshold value for detecting activity.
 *
 * @param threshold The threshold value for detecting activity as an 8-bit
 *                  number. Scale factor is 62.5mg/LSB. A value of 0 may
 *                  result in undesirable behavior if the activity
 *                  interrupt is enabled.
 */
void ADXL345_setActivityThreshold(unsigned char threshold);

/**
 * Get the threshold value for detecting inactivity.
 *
 * @return The threshold value for detecting inactivity as an 8-bit number.
 *         Scale factor is 62.5mg/LSB.
 */
int ADXL345_getInactivityThreshold(void);

/**
 * Set the threshold value for detecting inactivity.
 *
 * @param threshold The threshold value for detecting inactivity as an
 *                  8-bit number. Scale factor is 62.5mg/LSB.
 */
void ADXL345_setInactivityThreshold(unsigned char threshold);

/**
 * Get the time required for inactivity to be declared.
 *
 * @return The amount of time that acceleration must be less than the
 *         inactivity threshold for inactivity to be declared, in
 *         seconds.
 */
int ADXL345_getTimeInactivity(void);

/**
 * Set the time required for inactivity to be declared.
 *
 * @param inactivity The amount of time that acceleration must be less than
 *                   the inactivity threshold for inactivity to be
 *                   declared, in seconds. A value of 0 results in an
 *                   interrupt when the output data is less than the
 *                   threshold inactivity.
 */
void ADXL345_setTimeInactivity(unsigned char timeInactivity);

/**
 * Get the activity/inactivity control settings.
 *
 *      D7            D6             D5            D4
 * +-----------+--------------+--------------+--------------+
 * | ACT ac/dc | ACT_X enable | ACT_Y enable | ACT_Z enable |
 * +-----------+--------------+--------------+--------------+
 *
 *        D3             D2               D1              D0
 * +-------------+----------------+----------------+----------------+
 * | INACT ac/dc | INACT_X enable | INACT_Y enable | INACT_Z enable |
 * +-------------+----------------+----------------+----------------+
 *
 * See datasheet for details.
 *
 * @return The contents of the ACT_INACT_CTL register.
 */
int ADXL345_getActivityInactivityControl(void);

/**
 * Set the activity/inactivity control settings.
 *
 *      D7            D6             D5            D4
 * +-----------+--------------+--------------+--------------+
 * | ACT ac/dc | ACT_X enable | ACT_Y enable | ACT_Z enable |
 * +-----------+--------------+--------------+--------------+
 *
 *        D3             D2               D1              D0
 * +-------------+----------------+----------------+----------------+
 * | INACT ac/dc | INACT_X enable | INACT_Y enable | INACT_Z enable |
 * +-------------+----------------+----------------+----------------+
 *
 * See datasheet for details.
 *
 * @param settings The control byte to write to the ACT_INACT_CTL register.
 */
void ADXL345_setActivityInactivityControl(int settings);

/**
 * Get the threshold for free fall detection.
 *
 * @return The threshold value for free-fall detection, as an 8-bit number,
 *         with scale factor 62.5mg/LSB.
 */
int ADXL345_getFreefallThreshold(void);

/**
 * Set the threshold for free fall detection.
 *
 * @return The threshold value for free-fall detection, as an 8-bit number,
 *         with scale factor 62.5mg/LSB. A value of 0 may result in 
 *         undesirable behavior if the free-fall interrupt is enabled.
 *         Values between 300 mg and 600 mg (0x05 to 0x09) are recommended.
 */
void ADXL345_setFreefallThreshold(int threshold);

/**
 * Get the time required to generate a free fall interrupt.
 *
 * @return The minimum time that the value of all axes must be less than
 *         the freefall threshold to generate a free-fall interrupt, in
 *         milliseconds.
 */
int ADXL345_getFreefallTime(void);

/**
 * Set the time required to generate a free fall interrupt.
 *
 * @return The minimum time that the value of all axes must be less than
 *         the freefall threshold to generate a free-fall interrupt, in
 *         milliseconds. A value of 0 may result in undesirable behavior
 *         if the free-fall interrupt is enabled. Values between 100 ms 
 *         and 350 ms (0x14 to 0x46) are recommended.
 */
void ADXL345_setFreefallTime(int freefallTime_ms);

/**
 * Get the axis tap settings.
 *
 *      D3           D2            D1             D0
 * +----------+--------------+--------------+--------------+
 * | Suppress | TAP_X enable | TAP_Y enable | TAP_Z enable |
 * +----------+--------------+--------------+--------------+
 *
 * (D7-D4 are 0s).
 *
 * See datasheet for more details.
 *
 * @return The contents of the TAP_AXES register.
 */ 
int ADXL345_getTapAxisControl(void);

/**
 * Set the axis tap settings.
 *
 *      D3           D2            D1             D0
 * +----------+--------------+--------------+--------------+
 * | Suppress | TAP_X enable | TAP_Y enable | TAP_Z enable |
 * +----------+--------------+--------------+--------------+
 *
 * (D7-D4 are 0s).
 *
 * See datasheet for more details.
 *
 * @param The control byte to write to the TAP_AXES register.
 */
void ADXL345_setTapAxisControl(int settings);

/**
 * Get the source of a tap.
 *
 * @return The contents of the ACT_TAP_STATUS register.
 */
int ADXL345_getTapSource(void);

/**
 * Set the power mode.
 *
 * @param mode 0 -> Normal operation.
 *             1 -> Reduced power operation.
 */
void ADXL345_setPowerMode(char mode);

/**
 * Set the data rate.
 *
 * @param rate The rate code (see #defines or datasheet).
 */
void ADXL345_setDataRate(int rate);

/**
 * Get the power control settings.
 *
 * See datasheet for details.
 *
 * @return The contents of the POWER_CTL register.
 */
int ADXL345_getPowerControl(void);

/**
 * Set the power control settings.
 *
 * See datasheet for details.
 *
 * @param The control byte to write to the POWER_CTL register.
 */
void ADXL345_setPowerControl(unsigned char settings);

/**
 * Get the interrupt enable settings.
 *
 * @return The contents of the INT_ENABLE register.
 */
int ADXL345_getInterruptEnableControl(void);

/**
 * Set the interrupt enable settings.
 *
 * @param settings The control byte to write to the INT_ENABLE register.
 */
void ADXL345_setInterruptEnableControl(unsigned char settings);

/**
 * Get the interrupt mapping settings.
 *
 * @return The contents of the INT_MAP register.
 */
int ADXL345_getInterruptMappingControl(void);

/**
 * Set the interrupt mapping settings.
 *
 * @param settings The control byte to write to the INT_MAP register.
 */
void ADXL345_setInterruptMappingControl(int settings);

/**
 * Get the interrupt source.
 *
 * @return The contents of the INT_SOURCE register.
 */
int ADXL345_getInterruptSource(void);

/**
 * Get the data format settings.
 *
 * @return The contents of the DATA_FORMAT register.
 */
int ADXL345_getDataFormatControl(void);

/**
 * Set the data format settings.
 *
 * @param settings The control byte to write to the DATA_FORMAT register.
 */
void ADXL345_setDataFormatControl(unsigned char settings);

/**
 * Get the output of all three axes.
 *
 * @param Pointer to a buffer to hold the accelerometer value for the
 *        x-axis, y-axis and z-axis [in that order].
 */
void ADXL345_getOutput(int * readings);

/**
 * Get the FIFO control settings.
 *
 * @return The contents of the FIFO_CTL register.
 */
int ADXL345_getFifoControl(void);

/**
 * Set the FIFO control settings.
 *
 * @param The control byte to write to the FIFO_CTL register.
 */
void ADXL345_setFifoControl(int settings);

/**
 * Get FIFO status.
 *
 * @return The contents of the FIFO_STATUS register.
 */
char ADXL345_getFifoStatus(void);



//TODO These function have to be reimplemented for I2C

/**
 * Read one byte from a register on the device.
 *
 * @param address Address of the register to read.
 *
 * @return The contents of the register address.
 */
unsigned char ADXL345_oneByteRead(unsigned char address);

/**
 * Write one byte to a register on the device.
 *
 * @param address Address of the register to write to.
 * @param data The data to write into the register.
 */
void ADXL345_oneByteWrite(unsigned char address, unsigned char data);

/**
 * Read several consecutive bytes on the device.
 *
 * @param startAddress The address of the first register to read from.
 * @param buffer Pointer to a buffer to store data read from the device.
 * @param size The number of bytes to read.
 */
void ADXL345_multiByteRead(unsigned char startAddress, char* buffer, unsigned char size);

/**
 * Write several consecutive bytes on the device.
 *
 * @param startAddress The address of the first register to write to.
 * @param buffer Pointer to a buffer which contains the data to write.
 * @param size The number of bytes to write.
 */
void ADXL345_multiByteWrite(unsigned char startAddress, char* buffer, unsigned char size);


#endif /* ADXL345_H */

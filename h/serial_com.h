/**
 * @file serial_com.h
 *
 * Provides interaction with serial communication units.
 * @author Stan Bobovych
 */

#ifndef SERIAL_COM_H
#define SERIAL_COM_H

//SPI
#define SPI_CLK_TRI TRISCbits.TRISC3	///< SPI clock tristate bit
#define SPI_SDO_TRI TRISCbits.TRISC5	///< SPI data out tristate bit
#define SPI_SDI_TRI TRISCbits.TRISC4	///< SPI data in tristate bit
#define SPI_CS_TRI	TRISCbits.TRISC2	///< SPI chip select tristate bit
#define SPI_CS_PIN	PORTCbits.RC2		///< SPI chip select pin

//I2C
#define I2C_SCLK_LAT TRISCbits.TRISC3	///< I2C clock tristate bit
#define I2C_DATA_LAT TRISCbits.TRISC4	///< I2C data tristate bit
#define I2C_SCLK_PIN PORTCbits.RC3		///< I2C clock pin
#define I2C_DATA_PIN PORTCbits.RC4		///< I2C data pin

//Generic
#define BAUD 100000		///<Baude rate in Hz
#define SW_UART_BAUD 9600 ///<software uart baude rate in bps

/** Software uart function */
void DelayTXBitUART(void);
/** Software uart function */
void DelayRXHalfBitUART(void);
/** Software uart function */
void DelayRXBitUART(void);

/**
 * Initializes the USART unit
 */
void usart_init(void);
/**
 * Initializes the SPI unit
 */
void spi_init(void);
/**
 * Initializes the I2C unit
 */
void i2c_init(void);

#endif /* SERIAL_COM_H */

/**
* @file ITG32000.c
*
* Copyright (C) 2010 Ricardo Arturo Cabral <ing dot cabral dot mejia at gmail dot com>. All rights reserved.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. See <license.txt>, if not, see
* <http://www.gnu.org/licenses/>.
*/
#include <i2c.h>
#include "ITG3200.h"


float ITG3200_getX(void) {
  return (float)(ITG3200_read(ITG3200_REG_X_L) | ITG3200_read(ITG3200_REG_X_H)<<8)/ITG3200_SENSITIVITY;
}

float ITG3200_getY(void) {
  return (float)(ITG3200_read(ITG3200_REG_Y_L) | ITG3200_read(ITG3200_REG_Y_H)<<8)/ITG3200_SENSITIVITY;
}

float ITG3200_getZ(void) {
  return (float)(ITG3200_read(ITG3200_REG_Z_L) | ITG3200_read(ITG3200_REG_Z_H)<<8)/ITG3200_SENSITIVITY;
}

float ITG3200_getTemperature(void){
  return (((float)((ITG3200_read(ITG3200_REG_TEMP_L) | ITG3200_read(ITG3200_REG_TEMP_H)<<8) + ITG3200_TEMP_OFFSET))/ITG3200_TEMP_SENSITIVITY) + ITG3200_TEMP_OFFSET_CELSIUS;
}

void ITG3200_reset(void) {
  ITG3200_write(ITG3200_REG_PWR_MGM, ITG3200_RESET);
}

void ITG3200_sleep(void) {
  unsigned char t = ITG3200_read(ITG3200_REG_PWR_MGM);
  ITG3200_write(ITG3200_REG_PWR_MGM, t | ITG3200_SLEEP);
}

void ITG3200_wake(void){
  unsigned char t = ITG3200_read(ITG3200_REG_PWR_MGM);
  ITG3200_write(ITG3200_REG_PWR_MGM, t & ~ITG3200_SLEEP);
}

void ITG3200_standBy(unsigned char axis) {
  unsigned char t = ITG3200_read(ITG3200_REG_PWR_MGM);
  ITG3200_write(ITG3200_REG_PWR_MGM, t & ~axis);
}

unsigned char ITG3200_getAddress(void)
{
  return ITG3200_read(ITG3200_REG_WHOAMI);
}

void ITG3200_setAddress(unsigned char newAddress)
{
  ITG3200_write(ITG3200_REG_WHOAMI, newAddress);
}

void ITG3200_setInterruptConfig(unsigned char config)
{
  // bit 3 and 1 must be zero
  ITG3200_write(ITG3200_REG_INT_CFG, 0xF5 & config);
}

unsigned char ITG3200_isInterruptRawDataReady(void)
{
  unsigned char result = ITG3200_read(ITG3200_REG_INT_STS);
  if((result & ITG3200_INT_DATA) == ITG3200_INT_DATA)
	return 1;
  else
	return 0;	
}

unsigned char ITG3200_isInterruptReady(void)
{
  unsigned char result = ITG3200_read(ITG3200_REG_INT_STS);
  if((result & ITG3200_INT_READY) == ITG3200_INT_READY)
	return 1;
  else
	return 0;
}

unsigned char ITG3200_getInterruptConfig(void)
{
  return ITG3200_read(ITG3200_REG_INT_CFG);
}

void ITG3200_setClockSource(unsigned char clockSource)
{
  if (clockSource >= 6) // 6 and 7 are reserved
    return;
  ITG3200_write(ITG3200_REG_PWR_MGM, 0xF8 & clockSource);
}

void ITG3200_write(unsigned char reg, unsigned char val) {
	StartI2C();
	WriteI2C(ITG3200_ADDR_WRITE);
	WriteI2C(reg);
	WriteI2C(val);
	StopI2C();
}

unsigned char ITG3200_read(unsigned char reg) {
	unsigned char buf;
	
	StartI2C();
	WriteI2C(ITG3200_ADDR_WRITE);
	WriteI2C(reg);
	RestartI2C();
	WriteI2C(ITG3200_ADDR_READ);
	buf = ReadI2C();
	NotAckI2C();
	StopI2C();
	
	return buf;
}

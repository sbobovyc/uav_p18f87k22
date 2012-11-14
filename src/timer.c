/***********************************************************************
 *                                                                     *
 * This file contains the implementation of timer.h                    *
 *                                                                     *
 ***********************************************************************
 *                                                                     * 
 *    Author:         Tom Pycke                                        *
 *    Filename:       timer.c                                          *
 *    Date:           13/10/2007                                       *
 *    File Version:   1.00                                             *
 *    Other Files Required: timer.h                                    *
 *                          common.h                                   *
 *                                                                     *
 ***********************************************************************
 *                                                                     *
 * Other Comments:                                                     *
 *  Implements TIMER1 from dsPic to return time in seconds since last  *
 *  call.                                                              *
 *                                                                     *
 ***********************************************************************/
 
#include <timers.h>
#include "cpu_properties.h"

static const float ticks_per_second = (float)OSC;

void timer_init_ms()
{
	OpenTimer0(TIMER_INT_ON & T0_16BIT & T0_SOURCE_INT & T0_PS_1_1); 
	WriteTimer0(0); //zero out timer
}


void timer_reset(void)
{
	WriteTimer0(0); //zero out timer
}

/* 
 * Returns the time in seconds since the last call 
 * of this function (or last reset) 
 */
float timer_dt(void)
{
	float ticks = (float)ReadTimer0();
	timer_reset();	
	return ticks/ticks_per_second;
}

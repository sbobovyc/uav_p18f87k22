/**
 @file timer.h
 @author Stanislav Bobovych
 */

#ifndef TIMER_H
#define TIMER_H


/** Initialze the timer */
void timer_init_ms(void);

/** Resets the timer's counter */
void timer_reset(void);

/** Returns the time in seconds since the last call of this function (or last reset) 
  * @return seconds = ticks/ticks_per_second
  */
float timer_dt(void);

#endif TIMER_H
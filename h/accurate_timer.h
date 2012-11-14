/**
 @file ADXL345_control.h
 @author Stanislav Bobovych
 */

#ifndef ACCURATE_TIMER_H
#define ACCURATE_TIMER_H

/**
 * Initialize timer.
 */
void timer_init(void);

/**
 * Start Timer0.
 */
void timer_start(void);

/**
 * Stop Timer0.
 */
void timer_stop(void);

/**
 * Clear Timer0.
 */
void timer_clear(void);

/**
 * Read the timer and the rollover count.
 * @return clocks since timer was started
 */
int timer_getOutput();

#endif ACCURATE_TIMER_H

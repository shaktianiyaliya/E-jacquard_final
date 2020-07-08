/*
 * delay.h
 *
 *  Created on: 14-Jun-2017
 *      Author: Devsys
 */

#ifndef DELAY_H_
#define DELAY_H_

/*****************************************************************************/
#include <stm32f4xx.h>

/* = 1MHZ -> timer runs in microseconds */
#define TIM3_CLK_FREQ 99
/* timer states */
typedef enum {TIMER_RUNNING, TIMER_OVER}TIMER_STATE;


void tim3_delay_us(uint16_t uSecs);
void tim3_start_us_timer(uint16_t uSecs);
uint32_t tim3_check_us_timer(void);
void tim3_stop_us_timer(void);

/*****************************************************************************/
#endif /* DELAY_H_ */


/*
 * delay.c
 *
 *  Created on: 14-Jun-2017
 *      Author: Devsys
 */
/*****************************************************************************/
#include "delay.h"

/*****************************************************************************/


/**
 * _init_tim3() inits timer3 for time-base mode where it starts from 0 to
 * Auto reload value and stops(using One Pulse Mode). It sets update flag when
 * counter reaches ARR value
 * @param mSecs :  delay in ms
 */
static void _init_tim3(uint16_t uSecs)
{

	uint16_t PrescalerValue;
	// Enable RCC clock for TIM3
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);

	/* Compute the prescaler value to get 1000000Hz freq*/
	 PrescalerValue = (uint16_t) (SystemCoreClock  / (TIM3_CLK_FREQ + 1));
	// Time base configuration
	TIM_TimeBaseInitTypeDef TIM;
	TIM_TimeBaseStructInit(&TIM);
	TIM.TIM_Prescaler = PrescalerValue;
	TIM.TIM_Period = uSecs;
	TIM.TIM_ClockDivision = 0;
	TIM.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3,&TIM);

	/* set up update flag related settings */
	TIM_UpdateRequestConfig(TIM3,TIM_UpdateSource_Global);
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);
	TIM_SelectOnePulseMode(TIM3, TIM_OPMode_Single);

	// Enable counter for TIM3
	TIM_Cmd(TIM3,ENABLE);
}



/**
 * _stop_timer() stops the timer3 enabled by _init_timer()
 */
static void _stop_timer(void)
{

	TIM_Cmd(TIM3,DISABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,DISABLE);
}



/**
 * tim3_delay_ms() is used to generate millisecond delay using tim3.
 * The max delay accepted by this call is 0xffff ms. When 1 ms is passed it will
 * generate delay of 2 ms as timer doesnot work with zero value.
 * (actual value passed to timer is mSecs-1, so when mSecs is 1 we still pass 1 so we get 2 ms).
 * For avoiding this function from blocking forever, it exits when mSecs is 0.
 * @param mSecs
 */
void tim3_delay_us(uint16_t uSecs)
{

	/* Init and start timer */
	if(uSecs > 1)
	{
		uSecs -= 1;
	}
	else if(uSecs == 0)
	{
		return;
	}
	// init tim3 with msecs delay
	_init_tim3(uSecs);

	/* wait while overflow happens */
	while(TIM_GetFlagStatus(TIM3,TIM_FLAG_Update) == RESET);
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);

	// Stop timer
	_stop_timer();
}




/**
 * void tim3_start_ms_timer(uint16_t mSecs) is used to start timer with mSecs (milliseconds).
 * Once started, timer status can be checked using tim3_check_ms_timer().
 *
 * @param mSecs
 */
void tim3_start_us_timer(uint16_t uSecs)
{

	/* Init and start timer */
	if(uSecs > 1)
	{
		uSecs -= 1;
	}
	else if(uSecs == 0)
	{
		return;
	}
	// init tim3 with usecs delay
	_init_tim3(uSecs);
}




/**
 * tim3_check_ms_timer(void) is used to check whether timer set by tim3_start_ms_timer()
 * is over or still running. It returns TIMER_OVER when timer has timeed out and clears flags
 * and stops timer, otherwise it returns TIMER_RUNNING.
 *
 * @return TIMER_STATE
 */
uint32_t tim3_check_us_timer(void)
{
	uint32_t timer_state;

	/* wait while overflow happens */
	if(TIM_GetFlagStatus(TIM3,TIM_FLAG_Update) == SET)
	{
		TIM_ClearFlag(TIM3,TIM_FLAG_Update);
		// Stop timer
		_stop_timer();
		timer_state = TIMER_OVER;
	}
	else
	{
		timer_state = TIMER_RUNNING;
	}

	return timer_state;
}




/**
 * tim3_stop_ms_timer(void) is used to stop timer started by tim3_start_ms_timer().
 */
void tim3_stop_us_timer(void)
{
	_stop_timer();
}



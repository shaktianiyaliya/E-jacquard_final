/*
 * extender.c
 *
 *  Created on: 03-Feb-2018
 *      Author: Santosh P N
 */

#include "extender.h"

volatile uint8_t sensor_key = 1;
volatile ejac_mode Mode;
extern void action_feed_data(void);

char data_pins[16][5] = {"PC00"/*1*/,"PA00"/*3*/,"PC02"/*2*/,"PA02"/*4*/,"PA06"/*5*/,"PC05"/*6*/,"PB13"/*7*/,"PB15"/*8*/,"PC07"/*9*/,"PC09"/*10*/,"PA11"/*11*/,"PC10"/*12*/,"PC12"/*13*/,"PB03"/*14*/,"PB05"/*15*/,"PB07"/*16*/};
char clock_pins[16][5] = {"PC13"/*1*/,"PC03"/*3*/,"PC01"/*2*/,"PA01"/*4*/,"PA03"/*5*/,"PC04"/*6*/,"PB10"/*7*/,"PB14"/*8*/,"PC06"/*9*/,"PC08"/*10*/,"PA08"/*11*/,"PA12"/*12*/,"PC11"/*13*/,"PD02"/*14*/,"PB04"/*15*/,"PB06"/*16*/};

//char data_pins[16][5] = {"PC00"/*1*/,"PC02"/*2*/,"PA00"/*3*/,"PA02"/*4*/,"PA06"/*5*/,"PC05"/*6*/,"PB13"/*7*/,"PB15"/*8*/,"PC07"/*9*/,"PC09"/*10*/,"PA11"/*11*/,"PC10"/*12*/,"PC12"/*13*/,"PB03"/*14*/,"PB05"/*15*/,"PB07"/*16*/};
//char clock_pins[16][5] = {"PC13"/*1*/,"PC01"/*2*/,"PC03"/*3*/,"PA01"/*4*/,"PA03"/*5*/,"PC04"/*6*/,"PB10"/*7*/,"PB14"/*8*/,"PC06"/*9*/,"PC08"/*10*/,"PA08"/*11*/,"PA12"/*12*/,"PC11"/*13*/,"PD02"/*14*/,"PB04"/*15*/,"PB06"/*16*/};

GPIO_TypeDef* GPIOxport[] = {GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH};
uint16_t GPIO_Pin_xpin[] = {GPIO_Pin_0, GPIO_Pin_1, GPIO_Pin_2, GPIO_Pin_3, GPIO_Pin_4, GPIO_Pin_5, GPIO_Pin_6, GPIO_Pin_7, GPIO_Pin_8, GPIO_Pin_9, GPIO_Pin_10, GPIO_Pin_11, GPIO_Pin_12, GPIO_Pin_13, GPIO_Pin_14, GPIO_Pin_15};

void Mydelay(int loops)
{
	unsigned int i = 0;
	while (i++ < loops * 1000);
}

void disable_sensors(void)
{
	if(EXTI_GetITStatus(EXTI_Line0) == SET)
	{
		EXTI_ClearITPendingBit(EXTI_Line0);
	}

	if(EXTI_GetITStatus(EXTI_Line1) == SET)
	{
		EXTI_ClearITPendingBit(EXTI_Line1);
	}
	NVIC_DisableIRQ(EXTI0_IRQn);
	NVIC_DisableIRQ(EXTI1_IRQn);
}



void extender_init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIOA_InitStruct;
	GPIOA_InitStruct.GPIO_Pin = SDATA_PIN | SCLOCK_PIN | OE_PIN | GPIO_Pin_8 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_6;
	GPIOA_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIOA_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIOA_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIOA_InitStruct.GPIO_Speed = GPIO_Fast_Speed;
	GPIO_Init(GPIOA, &GPIOA_InitStruct);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	//Data and Clock Pins from PORT B
	GPIO_InitTypeDef GPIOB_InitStruct;
	GPIOB_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_10 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 | GPIO_Pin_9;
	GPIOB_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIOB_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIOB_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIOB_InitStruct.GPIO_Speed = GPIO_Medium_Speed;
	GPIO_Init(GPIOB, &GPIOB_InitStruct);

	GPIOB_InitStruct.GPIO_Pin = CL_OE_PIN;
	GPIOB_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIOB_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIOB_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIOB_InitStruct.GPIO_Speed = GPIO_Medium_Speed;
	GPIO_Init(GPIOB, &GPIOB_InitStruct);

	GPIO_InitTypeDef GPIOB_Sensors_InitStruct;
	GPIOB_Sensors_InitStruct.GPIO_Pin = SENSOR1_PIN | SENSOR2_PIN | PUSH_BUTTON;
	GPIOB_Sensors_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIOB_Sensors_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIOB_Sensors_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIOB_Sensors_InitStruct.GPIO_Speed = GPIO_High_Speed;
	GPIO_Init(GPIOB, &GPIOB_Sensors_InitStruct);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_InitTypeDef GPIOC_InitStruct;
	GPIOC_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIOC_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIOC_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIOC_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIOC_InitStruct.GPIO_Speed = GPIO_Fast_Speed;
	GPIO_Init(GPIOC, &GPIOC_InitStruct);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	//Data and Clock Pins from PORT D
	GPIO_InitTypeDef GPIOD_InitStruct;
	GPIOD_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIOD_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIOD_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIOD_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIOD_InitStruct.GPIO_Speed = GPIO_Medium_Speed;
	GPIO_Init(GPIOD, &GPIOD_InitStruct);
}

void sensor1_pnp_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource0);
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line = EXTI_Line0;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);

/*	NVIC_InitTypeDef NVIC_InitEXTI;
	NVIC_InitEXTI.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitEXTI.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitEXTI.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitEXTI.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_Init(&NVIC_InitEXTI); */
//	NVIC_EnableIRQ(EXTI0_IRQn);
}

void sensor1_npn_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource0);
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line = EXTI_Line0;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);

	/*NVIC_InitTypeDef NVIC_InitEXTI;
	NVIC_InitEXTI.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitEXTI.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitEXTI.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitEXTI.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_Init(&NVIC_InitEXTI);*/
//	NVIC_EnableIRQ(EXTI0_IRQn);
}

void sensor2_pnp_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource1);
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line = EXTI_Line1;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);

	/*NVIC_InitTypeDef NVIC_InitEXTI;
	NVIC_InitEXTI.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitEXTI.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitEXTI.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitEXTI.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_Init(&NVIC_InitEXTI);*/
//	NVIC_EnableIRQ(EXTI1_IRQn);
}

void sensor2_npn_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource1);
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line = EXTI_Line1;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);

/*	NVIC_InitTypeDef NVIC_InitEXTI;
	NVIC_InitEXTI.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitEXTI.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitEXTI.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitEXTI.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_Init(&NVIC_InitEXTI); */
//	NVIC_EnableIRQ(EXTI1_IRQn);
}
/*
void sensor3_pnp_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource2);
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line = EXTI_Line2;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);

	NVIC_InitTypeDef NVIC_InitEXTI;
	NVIC_InitEXTI.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitEXTI.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitEXTI.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitEXTI.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_Init(&NVIC_InitEXTI);
	NVIC_EnableIRQ(EXTI2_IRQn);
	brake_on = 1;
	brake_off = 0;
}

void sensor3_npn_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource2);
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line = EXTI_Line2;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);

	NVIC_InitTypeDef NVIC_InitEXTI;
	NVIC_InitEXTI.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitEXTI.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitEXTI.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitEXTI.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_Init(&NVIC_InitEXTI);
	NVIC_EnableIRQ(EXTI2_IRQn);
	brake_on = 0;
	brake_off = 1;
}
*/
void EXTI0_IRQHandler (void)
{
    if (Mode == SETTING)
    {
    	EXTI_ClearITPendingBit(EXTI_Line0);
    	return; // MJN
    }
    else if (EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		if((sensor_key == 1) && (config_hw.Sensor1.Sensor_Usage == ON))
		{
			GPIO_SetBits(GPIOB, CL_OE_PIN);
			sensor_key = 0;
			feed_next_data = 1;
		}
		else if((sensor_key == 0) && (config_hw.Sensor1.Sensor_Usage == OFF))
		{
			GPIO_ResetBits(GPIOB, CL_OE_PIN);
			//feed_next_data = 1;
			sensor_key = 1;
			action_feed_data();
		//	g_curr_line++;
		//	currline_response(SUCCES);
		}
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}

void EXTI1_IRQHandler (void)
{
    if (Mode == SETTING)
    {
    	EXTI_ClearITPendingBit(EXTI_Line1);
    	return; // MJN
    }
	if (EXTI_GetITStatus(EXTI_Line1) != RESET)
	{
		if((sensor_key == 0) && (config_hw.Sensor2.Sensor_Usage == OFF))
		{
			GPIO_ResetBits(GPIOB, CL_OE_PIN);
			//feed_next_data = 1;
			sensor_key = 1;
			action_feed_data();
		//	g_curr_line++;
		//	currline_response(SUCCES);
		}
		else if((sensor_key == 1) && (config_hw.Sensor2.Sensor_Usage == ON))
		{
			GPIO_SetBits(GPIOB, CL_OE_PIN);
			sensor_key = 0;
			feed_next_data = 1;
		}
		EXTI_ClearITPendingBit(EXTI_Line1);
	}
}

/*void EXTI2_IRQHandler (void)
{
	if (EXTI_GetITStatus(EXTI_Line2) != RESET)
	{
		if((Mode == RUNNING) && (config_hw.Sensor3.Sensor_Usage == BREAK) && (GPIO_ReadInputDataBit(GPIOB,PUSH_BUTTON) == brake_on))
		{
			Mode = SETTING;
			prev_flag = action_flag;
			action_flag = IDLE;
		}
		else if((Mode == SETTING) && (GPIO_ReadInputDataBit(GPIOB,PUSH_BUTTON) == brake_off))
		{
			Mode = RUNNING;
			action_flag = prev_flag;
		}
		EXTI_ClearITPendingBit(EXTI_Line2);
	}
}*/

void latch_addr_byte(uint8_t byte)
{
	latch_addr_bit(byte & (1<<7));
	latch_addr_bit(byte & (1<<6));
	latch_addr_bit(byte & (1<<5));
	latch_addr_bit(byte & (1<<4));
	latch_addr_bit(byte & (1<<3));
	latch_addr_bit(byte & (1<<2));
	latch_addr_bit(byte & (1<<1));
	latch_addr_bit(byte & (1<<0));
}

void latch_addr_bit(uint8_t bit)
{
	SDATA_BIT(bit);
	Mydelay(3);
	SCLOCK_HIGH;
	Mydelay(3);
	SCLOCK_LOW;
	Mydelay(1);
}

void latch_data_byte(uint8_t byte,uint8_t data_port,uint8_t data_pin,uint8_t clock_port,uint8_t clock_pin)
{
	latch_data_bit(byte & (1<<7),data_port,data_pin,clock_port,clock_pin);
	latch_data_bit(byte & (1<<6),data_port,data_pin,clock_port,clock_pin);
	latch_data_bit(byte & (1<<5),data_port,data_pin,clock_port,clock_pin);
	latch_data_bit(byte & (1<<4),data_port,data_pin,clock_port,clock_pin);
	latch_data_bit(byte & (1<<3),data_port,data_pin,clock_port,clock_pin);
	latch_data_bit(byte & (1<<2),data_port,data_pin,clock_port,clock_pin);
	latch_data_bit(byte & (1<<1),data_port,data_pin,clock_port,clock_pin);
	latch_data_bit(byte & (1<<0),data_port,data_pin,clock_port,clock_pin);
}

void latch_data_bit(uint8_t bit,uint8_t data_port,uint8_t data_pin,uint8_t clock_port,uint8_t clock_pin)
{
	DATA_BIT(bit,data_port,data_pin);
	Mydelay(3);
	CLOCK_HIGH(clock_port,clock_pin);
	Mydelay(3);
	CLOCK_LOW(clock_port,clock_pin);
	Mydelay(5);
}

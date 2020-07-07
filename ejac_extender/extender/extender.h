/*
 * extender.h
 *
 *  Created on: 03-Feb-2018
 *      Author: Santosh P N
 */

#ifndef EXTENDER_H_
#define EXTENDER_H_

#include "stm32f4xx.h"
#include "delay.h"
#include "uart.h"

#define SDATA_PIN		GPIO_Pin_7  /* PA7 */
#define SCLOCK_PIN		GPIO_Pin_5  /* PA5 */
#define OE_PIN			GPIO_Pin_4  /* PA4 */
#define DATA_PIN		GPIO_Pin_3  /* PC3 */
#define CLOCK_PIN		GPIO_Pin_10  /* PB10 */
#define CL_OE_PIN		GPIO_Pin_12  /* PB12 */

#define SENSOR1_PIN     GPIO_Pin_0   /* PB0 */
#define SENSOR2_PIN		GPIO_Pin_1   /* PB1 */
#define PUSH_BUTTON  	GPIO_Pin_2   /* PB2 */

#define MUX_EN_Pin      GPIO_Pin_9
#define MUX_EN_GPIO_Port GPIOB

#define MUX_HIGH		GPIO_WriteBit(MUX_EN_GPIO_Port,MUX_EN_Pin,Bit_SET)
#define MUX_LOW			GPIO_WriteBit(MUX_EN_GPIO_Port,MUX_EN_Pin,Bit_RESET)


#if 0
#define SDATA_HIGH				(GPIO_SetBits(GPIOA, SDATA_PIN))
#define SDATA_LOW				(GPIO_ResetBits(GPIOA, SDATA_PIN))
#define SDATA_BIT(bit)			((bit != 0) ? (SDATA_HIGH) : (SDATA_LOW))

#define DATA_HIGH				(GPIO_SetBits(GPIOC, DATA_PIN))
#define DATA_LOW				(GPIO_ResetBits(GPIOC, DATA_PIN))
#define DATA_BIT(bit)			((bit != 0) ? (DATA_HIGH) : (DATA_LOW))

#define SCLOCK_HIGH	   		    (GPIO_SetBits(GPIOA, SCLOCK_PIN))
#define SCLOCK_LOW	   		    (GPIO_ResetBits(GPIOA, SCLOCK_PIN))

#define CLOCK_HIGH	 	        (GPIO_SetBits(GPIOB, CLOCK_PIN))
#define CLOCK_LOW	  	        (GPIO_ResetBits(GPIOB, CLOCK_PIN))
#endif

#define SDATA_HIGH				(GPIO_WriteBit(GPIOA, SDATA_PIN, Bit_SET))
//#define SDATA_LOW				(GPIO_ResetBits(GPIOA, SDATA_PIN))
#define SDATA_LOW				(GPIO_WriteBit(GPIOA, SDATA_PIN, Bit_RESET))
#define SDATA_BIT(bit)			((bit != 0) ? (SDATA_HIGH) : (SDATA_LOW))

//#define DATA_HIGH				(GPIO_SetBits(GPIOC, DATA_PIN))
#define DATA_HIGH(port,pin)		(GPIO_WriteBit(GPIOxport[port-'A'],GPIO_Pin_xpin[pin], Bit_SET))
//#define DATA_LOW				(GPIO_ResetBits(GPIOC, DATA_PIN))
#define DATA_LOW(port,pin)		(GPIO_WriteBit(GPIOxport[port-'A'],GPIO_Pin_xpin[pin], Bit_RESET))
#define CLOCK_HIGH(port,pin)	(GPIO_WriteBit(GPIOxport[port-'A'],GPIO_Pin_xpin[pin], Bit_SET))
//#define CLOCK_LOW	  	        (GPIO_ResetBits(GPIOB, CLOCK_PIN))
#define CLOCK_LOW(port,pin)	    (GPIO_WriteBit(GPIOxport[port-'A'],GPIO_Pin_xpin[pin], Bit_RESET))

#define DATA_BIT(bit, port, pin) ((bit != 0) ? (DATA_HIGH(port,pin)) : (DATA_LOW(port, pin)))

//#define SCLOCK_HIGH	   		    (GPIO_SetBits(GPIOA, SCLOCK_PIN))
#define SCLOCK_HIGH	   		    (GPIO_WriteBit(GPIOA, SCLOCK_PIN, Bit_SET))
//#define SCLOCK_LOW	   		    (GPIO_ResetBits(GPIOA, SCLOCK_PIN))
#define SCLOCK_LOW	   		    (GPIO_WriteBit(GPIOA, SCLOCK_PIN, Bit_RESET))

//#define CLOCK_HIGH	 	        (GPIO_SetBits(GPIOB, CLOCK_PIN))



#define MASK(x) ((uint8_t)(0x01)<<x)

void extender_init(void);
void sensor1_pnp_init(void);
void sensor1_npn_init(void);
void sensor2_pnp_init(void);
void sensor2_npn_init(void);
void sensor3_pnp_init(void);
void sensor3_npn_init(void);

void latch_addr_bit(uint8_t bit);
void latch_addr_byte(uint8_t byte);

void latch_data_bit(uint8_t bit,uint8_t data_port,uint8_t data_pin,uint8_t clock_port,uint8_t clock_pin);
void latch_data_byte(uint8_t byte,uint8_t data_port,uint8_t data_pin,uint8_t clock_port,uint8_t clock_pin);
void disable_sensors(void);

uint8_t brake_on, brake_off;
extern char data_pins[16][5];
extern char clock_pins[16][5];
extern GPIO_TypeDef* GPIOxport[];
extern uint16_t GPIO_Pin_xpin[];
#endif /* EXTENDER_H_ */

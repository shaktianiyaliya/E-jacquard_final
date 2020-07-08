#ifndef __EJAC_TRACE_H__
#define __EJAC_TRACE_H__

#include "stm32f4xx.h"
#include <stdint.h>

#define EJAC_TRACE_BUFFER_MAX_SIZE	256
#define EJAC_TRACE_UART_TX_PIN 		GPIO_Pin_9
#define EJAC_TRACE_UART_RX_PIN     	GPIO_Pin_10

#define EJAC_TRACE_UART_EJACQUARD  	USART1

void ejac_trace_uart_init(void);
void ejac_trace(uint8_t  *msg, int32_t len);

#endif

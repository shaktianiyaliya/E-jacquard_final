#include "ejacTrace.h"

uint8_t ejacTraceRecvBuffer[EJAC_TRACE_BUFFER_MAX_SIZE];
uint8_t ejacTraceSendBuffer[EJAC_TRACE_BUFFER_MAX_SIZE];
volatile uint32_t ejacRecvCount = 0;

void ejac_trace_uart_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = EJAC_TRACE_UART_TX_PIN | EJAC_TRACE_UART_RX_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Medium_Speed;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_Mode = (USART_Mode_Tx | USART_Mode_Rx);
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(EJAC_TRACE_UART_EJACQUARD, &USART_InitStruct);

#if 0
	NVIC_InitTypeDef NVIC_InitUSART;
	NVIC_InitUSART.NVIC_IRQChannel =  USART1_IRQn;
	NVIC_InitUSART.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitUSART.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitUSART.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_Init(&NVIC_InitUSART);
#endif
	// enable EIE interrupt.
	USART_ITConfig(EJAC_TRACE_UART_EJACQUARD, USART_IT_ERR, ENABLE);
	USART_ITConfig(EJAC_TRACE_UART_EJACQUARD, USART_IT_RXNE, ENABLE);
	USART_Cmd(EJAC_TRACE_UART_EJACQUARD, ENABLE);

	NVIC_EnableIRQ(USART1_IRQn);
}

void ejac_trace(uint8_t  *msg, int32_t len)
{

	while(len)
	{
		if (USART_GetFlagStatus(EJAC_TRACE_UART_EJACQUARD, USART_FLAG_TXE) == SET)
		{
			USART_SendData(EJAC_TRACE_UART_EJACQUARD, (uint16_t)(*msg));
			len--;
			msg++;
		}
	}
}


#if 0
void USART1_IRQHandler(void)
{
	__disable_irq();
#if 1
	uint32_t isr_flags = EJAC_TRACE_UART_EJACQUARD->SR;
	uint32_t cr1_its = EJAC_TRACE_UART_EJACQUARD->CR1;
	uint32_t error_flags;
	uint8_t dummy = dummy;

	    /* If no error occurs */
	error_flags = (isr_flags & (uint32_t) (USART_SR_ORE |  USART_SR_NE | USART_SR_FE |  USART_SR_PE ));

	if (error_flags == RESET)
	{
	        if ( ((isr_flags & USART_SR_RXNE) != RESET) && ((cr1_its & USART_CR1_RXNEIE) != RESET) )    /* Receive Data Ready */
	        {
	          /* If no error on ISR, normal data ready, save into the data buffer. */
	          /* Note: read from RDR will clear the interrupt */
	        	ejacTraceSendBuffer[ejacRecvCount] =  (uint8_t)USART_ReceiveData(EJAC_TRACE_UART_EJACQUARD);
	        	ejacRecvCount++;
	        }
	        if(ejacRecvCount >= EJAC_TRACE_BUFFER_MAX_SIZE)
	        {
	        	ejacRecvCount = 0;
	        }
	 }
	 else
	 {
	   	         /* USART frame error interrupt occurred --------------------------------------*/
	         if(((isr_flags & USART_SR_FE) != RESET))
	         {
	             /*Clear the Frame error Flag*/
	             USART_ClearFlag(EJAC_TRACE_UART_EJACQUARD, USART_FLAG_FE);
	         }
	         /* USART noise error interrupt occurred --------------------------------------*/
	         else if(((isr_flags & USART_SR_NE) != RESET) )
	         {
	             /*Clear the noise error Flag*/
	             USART_ClearFlag(EJAC_TRACE_UART_EJACQUARD, USART_FLAG_NE);
	         }
	         /* USART Over-Run interrupt occurred -----------------------------------------*/
	         else if(((isr_flags & USART_SR_ORE) != RESET))
	         {
	             /*Clear the Over-Run error Flag*/
	        	 //ore_count++;
	             USART_ClearFlag(EJAC_TRACE_UART_EJACQUARD, USART_FLAG_ORE);
	        	 //error_flags = UART_EJACQUARD->SR;

	         }
	    	 dummy = USART_ReceiveData(EJAC_TRACE_UART_EJACQUARD);

	 }
#endif
	 // re-enable irqs
	 __enable_irq();
}
#endif

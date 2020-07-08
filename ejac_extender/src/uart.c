/*
 * uart.c
 *
 *  Created on: 31-Jan-2018
 *      Author: Santosh P N
 */

#include "uart.h"
extern ejac_mode Mode;

void uart_init(void)
{
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = UART_TX_PIN | UART_RX_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Medium_Speed;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	//GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	//GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
	USART_ClearFlag(UART_EJACQUARD, USART_FLAG_TC);

	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_Mode = (USART_Mode_Tx | USART_Mode_Rx);
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(UART_EJACQUARD, &USART_InitStruct);

	// enable EIE interrupt.
	USART_ITConfig(UART_EJACQUARD, USART_IT_ERR, ENABLE);
	USART_ITConfig(UART_EJACQUARD, USART_IT_RXNE, ENABLE);
	USART_Cmd(UART_EJACQUARD, ENABLE);

	NVIC_EnableIRQ(USART1_IRQn);
}

void USART1_IRQHandler(void)
{
	__disable_irq();
#if 1
	uint32_t isr_flags = UART_EJACQUARD->SR;
	uint32_t cr1_its = UART_EJACQUARD->CR1;
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
              save_buffer[cmd_count] =  (uint8_t)USART_ReceiveData(UART_EJACQUARD);
	          cmd_count++;
	    }
		if(cmd_count >= PAGE_SIZE)
		{
			cmd_count = 0;
		}
	 }
	 else
	 {
			 /* USART frame error interrupt occurred --------------------------------------*/
		 if(((isr_flags & USART_SR_FE) != RESET))
		 {
			 /*Clear the Frame error Flag*/
			 USART_ClearFlag(UART_EJACQUARD, USART_FLAG_FE);
		 }
		 /* USART noise error interrupt occurred --------------------------------------*/
		 else if(((isr_flags & USART_SR_NE) != RESET) )
		 {
			 /*Clear the noise error Flag*/
			 USART_ClearFlag(UART_EJACQUARD, USART_FLAG_NE);
		 }
		 /* USART Over-Run interrupt occurred -----------------------------------------*/
		 else if(((isr_flags & USART_SR_ORE) != RESET))
		 {
			 /*Clear the Over-Run error Flag*/
			 ore_count++;
			 USART_ClearFlag(UART_EJACQUARD, USART_FLAG_ORE);
			 //error_flags = UART_EJACQUARD->SR;

		 }
		 dummy = USART_ReceiveData(UART_EJACQUARD);

	 }
#endif
	 // re-enable irqs
	 __enable_irq();
}

void sel_action(void)
{
	ejac_cmd_request_t cmd;
	memcpy(&cmd, save_buffer, SIZE_CMD);
	if ((cmd.MagicNumber != MAGIC_NO) && (cmd.Uid != UID))
	{
		cmd_count = 0;
		memset(save_buffer, 0, PAGE_SIZE);
		response(FAIL);
	}

	if ((cmd.Action == STATUS))
	{
		action_flag = STATUS;
		//prev_action_flag = STATUS;
	}
	else if (cmd.Action == LOAD)
	{
		action_flag = LOAD;
	}
	else if (cmd.Action == START)
	{
		action_flag = START;
		Mode = RUNNING;
		//prev_action_flag = START;
	}
	else if ((cmd.Action == STOP) && (buff_len == 0))
	{
		action_flag = STOP;
		//prev_action_flag = STOP;
	}
	/*else if (cmd.Action == JUMP)
	{
		action_flag = JUMP;
		//prev_action_flag = JUMP;
	}*/
	else if (cmd.Action == IMAGE_CONFIG)
	{
		action_flag = IMAGE_CONFIG;
		//prev_action_flag = IMAGE_CONFIG;
	}
	else if (cmd.Action == HARDWARE_CONFIG)
	{
		action_flag = HARDWARE_CONFIG;
		//prev_action_flag = HARDWARE_CONFIG;
	}
	else if (cmd.Action == IMAGE_INFO)
	{
		action_flag = IMAGE_INFO;

	}
	else if ((cmd.Action == IMAGE_DETAIL_STATUS))
	{
		action_flag = IMAGE_DETAIL_STATUS;
	}
	else if ((cmd.Action == HARDWARE_DETAIL_STATUS))
	{
		action_flag = HARDWARE_DETAIL_STATUS;
	}
	else if ((cmd.Action == TEST_PATTERN))
	{
		action_flag = TEST_PATTERN;
	}
}

void response(ejac_status_type status_code)
{
	uint8_t count = 0, * resp_ptr;
	ejac_cmd_response_t  resp;
	ejac_cmd_image_config_t config_resp;
	ejac_cmd_hardware_settings_t hw_resp;
	(resp).MagicNumber = MAGIC_NO;
	(resp).Uid = UID;
	(resp).Action = action_flag;
	(resp).Status = status_code;
	if (status_code == SUCCES)
	{
	    if((action_flag == IMAGE_DETAIL_STATUS))
		{
			uint8_t * config_addr_ptr = (uint8_t *)0x08010000; //CONFIG_ADDR;
			memcpy(&config_resp, config_addr_ptr, CONFIG_RESP_SIZE);
		}
	    else if((action_flag == HARDWARE_DETAIL_STATUS))
	    {
	    	uint8_t * hw_addr_ptr = (uint8_t *)0x0800C000;
	    	memcpy(&hw_resp, hw_addr_ptr, sizeof(ejac_cmd_hardware_settings_t));
	    }

	}
	resp_ptr = (uint8_t *)&resp;
	count = SIZE_RESP;
	while(count)
	{
		if (USART_GetFlagStatus(UART_EJACQUARD, USART_FLAG_TXE) == SET)
		{
			USART_SendData(UART_EJACQUARD, (*resp_ptr));
			count--;
			resp_ptr++;
		}
	}

	 while(USART_GetFlagStatus(UART_EJACQUARD, USART_FLAG_TC) == SET)
	 {
		         ;
     }


	if((action_flag == IMAGE_DETAIL_STATUS))
	{
		resp_ptr = (uint8_t *)&config_resp;
		count = CONFIG_RESP_SIZE;
		while (count)
		{
			if (USART_GetFlagStatus(UART_EJACQUARD, USART_FLAG_TXE) == SET)
			{
				USART_SendData(UART_EJACQUARD, (*resp_ptr));
				count--;
				resp_ptr++;
			}
		}
		while(USART_GetFlagStatus(UART_EJACQUARD, USART_FLAG_TC) == SET)
		{
			;
		}
	}
	else if((action_flag == HARDWARE_DETAIL_STATUS))
	{
		resp_ptr = (uint8_t *)&hw_resp;
		count = sizeof(ejac_cmd_hardware_settings_t);
		while (count)
		{
			if (USART_GetFlagStatus(UART_EJACQUARD, USART_FLAG_TXE) == SET)
			{
				USART_SendData(UART_EJACQUARD, (*resp_ptr));
				count--;
				resp_ptr++;
			}
		}
		while(USART_GetFlagStatus(UART_EJACQUARD, USART_FLAG_TC) == SET)
		{
				;
		}
	}

	if ((action_flag != START) && (action_flag != LOAD))
	{
		action_flag = IDLE;
		//prev_action_flag = IDLE;
		memset(save_buffer, 0, PAGE_SIZE);
		cmd_count = 0;
	}
}

void currline_response(ejac_status_type status_code)
{
	ejac_cmd_resp_t curr_resp;
	uint8_t * curr_resp_ptr, count = 0;

	curr_resp.CurrLine1 = line_count1;
	curr_resp.Itr1 = itr1;
	curr_resp.CurrLine2 = line_count2;
	curr_resp.Itr2 = itr2;
	/*curr_resp.CurrLine3 = line_count3;
	curr_resp.Itr3 = itr3;
	curr_resp.CurrLine4 = line_count4;
	curr_resp.Itr4 = itr4;*/
	curr_resp.GlobalCurr = g_curr_line;
	curr_resp_ptr = (uint8_t *)&curr_resp;
	count = SIZE_CURR_RESP;

	while(count)
	{
		if (USART_GetFlagStatus(UART_EJACQUARD, USART_FLAG_TXE) == SET)
		{
			USART_SendData(UART_EJACQUARD, (*curr_resp_ptr));
			count--;
			curr_resp_ptr++;
		}
	}

	 while(USART_GetFlagStatus(UART_EJACQUARD, USART_FLAG_TC) == SET)
	 {
		         ;
	 }

	feed_next_data = 0;
}

ejac_status_type erase_flash(void)
{
	FlashState state;
	state = flash_sector_erase(FLASH_Sector_5);
	if(state == FLASH_FAIL)
	{
		return(FAIL_FLASH);
	}
	state = flash_sector_erase(FLASH_Sector_6);
	if(state == FLASH_FAIL)
	{
		return(FAIL_FLASH);
	}
	state = flash_sector_erase(FLASH_Sector_7);
	if(state == FLASH_FAIL)
	{
		return(FAIL_FLASH);
	}
	state = flash_sector_erase(FLASH_Sector_8);
	if(state == FLASH_FAIL)
	{
		return(FAIL_FLASH);
	}
	state = flash_sector_erase(FLASH_Sector_9);
	if(state == FLASH_FAIL)
	{
		return(FAIL_FLASH);
	}
	if(state == FLASH_PASS)
	{
		return(SUCCES);
	}
	else
	{
		return(FAIL_FLASH);
	}

}

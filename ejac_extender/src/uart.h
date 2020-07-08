/*
 * uart.h
 *
 *  Created on: 31-Jan-2018
 *      Author: Santosh P N
 */

#ifndef UART_H_
#define UART_H_

#include "stm32f4xx.h"
#include "protocol.h"
#include "string.h"
#include "flash.h"

#define UART_TX_PIN 	GPIO_Pin_9
#define UART_RX_PIN     GPIO_Pin_10
//#define UART_TX_PIN 	GPIO_Pin_2
//#define UART_RX_PIN     GPIO_Pin_3

#define UART_EJACQUARD  USART1
//#define UART_EJACQUARD  USART2

#define MAGIC_NO  		0x0B0B
#define UID				0x01

#define CONFIG_ADDR    	  FLASH_Sector_4
#define HW_ADDR			  FLASH_Sector_3
#define IMAGE_ORDER 	  FLASH_Sector_11
#define SIZE_CMD		  (sizeof(ejac_cmd_request_t))
#define SIZE_RESP		  (sizeof(ejac_cmd_response_t))
#define SIZE_CMD_IMG_CONFIG   (sizeof(ejac_cmd_image_config_t) + sizeof(ejac_cmd_request_t))
#define SIZE_CMD_HW_CONFIG 	  (sizeof(ejac_cmd_hardware_settings_t) + sizeof(ejac_cmd_request_t))
#define PAGE_SIZE 		  (3072)
#define CONFIG_RESP_SIZE  sizeof(ejac_cmd_image_config_t)
#define SIZE_CURR_RESP    (sizeof(ejac_cmd_resp_t))

void uart_init(void);
void sel_action(void);
ejac_status_type erase_flash(void);
void response(ejac_status_type status_code);
void currline_response(ejac_status_type status_code);

volatile uint8_t action_flag, prev_flag, feed_next_data;
uint32_t flash_start_addr[8], flash_end_addr[8], buff_len,  curr_line1, curr_line2, curr_line3, curr_line4, jump_line, current_load_size, load_count;
uint8_t file_id, img_count, save_buffer[PAGE_SIZE], receive_input_data, border_count, body_count;
volatile uint32_t itr1,itr2,itr3,itr4,g_curr_line;
volatile uint32_t  cmd_count,ore_count;
volatile int32_t line_count1, line_count2, line_count3, line_count4;
uint8_t act_cols, act_rows;
ejac_cmd_hardware_settings_t config_hw;
ejac_cmd_image_config_t config_data;
ejac_cmd_load_t load_cmd;
ejac_cmd_start_t start_cmd;
ejac_image_extra_config_t extra_config;
ejac_cmd_test_pattern_t test_pattern;
ejac_cmd_image_order_t image_order;

#endif /* UART_H_ */

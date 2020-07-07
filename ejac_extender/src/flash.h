/*
 * flash.h
 *
 *  Created on: 31-Jan-2018
 *      Author: DevSys
 */

#ifndef FLASH_H_
#define FLASH_H_

#include "stm32f4xx.h"
#include "stm32f4xx_flash.h"

typedef enum {FLASH_PASS, FLASH_FAIL}FlashState;

int flash_write(uint8_t *flash_data, uint16_t size, uint32_t address);
FlashState flash_sector_erase(uint32_t page_addr);

#endif /* FLASH_H_ */

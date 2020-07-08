/*
 * flash.c
 *
 *  Created on: 31-Jan-2018
 *      Author: DevSys
 */

#include "flash.h"

volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;

/**
 * flash_write() writes into flash memory from buffer of given size.
 *
 * @param flash_data	: data to be programmed into flash
 * @param size			: size of data
 * @param address		: address at which data to be written in flash memory
 * @return	uint32_t FLAH_PASS/FLASH_FAIL
 */
int flash_write(uint8_t *flash_data, uint16_t size, uint32_t address)
{
	//uint16_t data = 0;
	uint8_t data = 0;
	int byte_index = 0;

	/* Unlock the Flash Bank1 Program Erase controller */
	FLASH_Unlock();

	for(byte_index = 0; byte_index < size; byte_index++)
	{
#if 0
		if(byte_index % 2)
		{
			data |= (flash_data[byte_index]) << 8;
		}
		else
		{
			data = (flash_data[byte_index]);
			continue;
		}
#endif

		data = flash_data[byte_index];
		//FLASHStatus = FLASH_ProgramHalfWord(address, data);
		FLASHStatus = FLASH_ProgramByte(address, data);
		address += 1;
		if (FLASHStatus != FLASH_COMPLETE)
		{
			/* lock flash again */
			FLASH_Lock();
			return FLASH_FAIL;
		}
	}

#if 0
	if(size % 2)
	{
		FLASHStatus = FLASH_ProgramHalfWord(address, data);
		if (FLASHStatus != FLASH_COMPLETE)
		{
			/* lock flash again */
			FLASH_Lock();
			return FLASH_FAIL;
		}
	}
#endif
	/* lock flash again */
	FLASH_Lock();

	return FLASH_PASS;
}

/**
 * flash_page_erase() erases the given page from flash memory.
 * It succeeds in erasing page only when address passed it start of page.
 *
 * @param page_addr : start address of the page
 *
 * @return FLASH_PASS/FLASH_FAIL
 */
FlashState flash_sector_erase(uint32_t page_addr)
{
	FlashState state = FLASH_FAIL;
	uint8_t VoltRange = VoltageRange_3;
	/* Unlock the Flash Bank1 Program Erase controller */
	FLASH_Unlock();
	/* erase the specified page of flash memory */
	if(FLASH_EraseSector(page_addr, VoltRange) == FLASH_COMPLETE)
	{
		state = FLASH_PASS;
	}
	/* lock flash again */
	FLASH_Lock();
	return state;
}

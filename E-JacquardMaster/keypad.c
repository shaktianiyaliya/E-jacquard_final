#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "display.h"
#include "usb.h"

#define E_KEY_MAIN  	0x02
#define E_KEY_ENTER  	0x04
#define E_KEY_UP	0x08
#define E_KEY_DOWN  	0x10
#define E_KEY_PALLU  	0x12
#define E_KEY_BODY  	0x14


#define KEY_NUMBER_0	0x40
#define KEY_NUMBER_9	0x49
static unsigned int gsKeyNumber = 1;
static unsigned int gsKeyIndex = 0;

#define RESET_KEY_NUMBERS	gsKeyNumber = 1; gsKeyIndex = 0;
void keypad_handler(UINT8_T key)
{
	/* Do Nothing during the RUNNING STATE */
	if (GET_STATE() == EJACQUARD_STATE_RUNNING)  {
		if ( key = E_KEY_MAIN ) CHANGE_STATE(EJACQUARD_STATE_IDLE);
		return;
	}
	switch(key)	
	{
		case E_KEY_MAIN:
			CHANGE_STATE(EJACQUARD_STATE_RUNNING);
			RESET_KEY_NUMBERS;
			break;
		case E_KEY_ENTER:
			if (GET_STATE() == EJACQUARD_STATE_USB_CONFIG)
				usb_select_file();	
			else 
				printf("TODO on config  mode\n");
			RESET_KEY_NUMBERS;
			break;
		case E_KEY_UP:
			/* In usb mode this up key will move the usb file list up */
			if (GET_STATE() == EJACQUARD_STATE_USB_CONFIG)
				usb_scroll_up();
			else 
				/* increment the Pallu/borader number */
				process_increament_lines(gsKeyNumber);
			RESET_KEY_NUMBERS;
			break;
		case E_KEY_DOWN:
			/* In usb mode this up key will move the usb file list up */
			if (GET_STATE() == EJACQUARD_STATE_USB_CONFIG)
				usb_scroll_down();		
			else 
				/* Decrement the Pallu/borader number */
				process_decrement_lines(gsKeyNumber);
			RESET_KEY_NUMBERS;
			break;
		case E_KEY_PALLU:
			/* Set the Display-2  screen type should be Pallu */
			CHANGE_FILE_TYPE(EJACQUARD_FILE_TYPE_PALLU);
			RESET_KEY_NUMBERS;
			break;
		case E_KEY_BODY: 
			/* Set the Display-2  screen type should be Body */
			CHANGE_FILE_TYPE(EJACQUARD_FILE_TYPE_BODY);
			RESET_KEY_NUMBERS;
			break;
		default:
			if ( key >= KEY_NUMBER_0 && key <= KEY_NUMBER_9 && gsKeyIndex < 2/* only 2 digit number is valid*/)
			{
				gsKeyNumber = (gsKeyNumber *  gsKeyIndex) + key;	
				gsKeyIndex ++;
			}
			break;
	}
	return;	
}


int keypad_init(void)
{
	int ret = 0;

	/* initialize the keypad_handler into the keypad intrrupt line */
	return 0;
}

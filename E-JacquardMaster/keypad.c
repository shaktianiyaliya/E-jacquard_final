
/**
  ******************************************************************************
  * @file    keypad.c
  * @author  Devsys embedded team
  * @version V1.1.0
  * @date    18-April-2020
  * @brief   This file provides keypad functions to manage the following 
  *          functionalities of the keypad:
  *           + 
  *           + 
  *
 
  ******************************************************************************
  */




/* Includes ------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "display.h"
#include "usb.h"

#define E_KEY_MAIN  	0x02  // Defining MAIN key
#define E_KEY_ENTER  	0x04  // Defining ENTER key
#define E_KEY_UP	0x08  // Defining UP key
#define E_KEY_DOWN  	0x10  // Defining DOWN key
#define E_KEY_PALLU  	0x12  // Defining BORDER key
#define E_KEY_BODY  	0x14  // Defining BODY key


#define KEY_NUMBER_0	0x40 // Defining KEY_NUMBER_0 as hex value
#define KEY_NUMBER_9	0x49 // Defining KEY_NUMBER_9 as hex value

static unsigned int gsKeyNumber = 1; /* Declaring a positive integer of static to preserves it's previous value */
static unsigned int gsKeyIndex = 0;  /* Declaring a positive integer of static to preserves it's previous value */
 
#define RESET_KEY_NUMBERS	gsKeyNumber = 1; gsKeyIndex = 0;// doubt this syntax


/**
  * @brief  Configures Keypad, when ever user press perticular key that corresponding operation will perform.
  * @note   If the current state is EJACQUARD_STATE_RUNNING and if the key pressed is MAIN key- 
  *         The state will change to idle state. 
  *        
  * @note   Validate each and every key pressed by user inside switch. 
  *         If the switch key is E_KEY_MAIN, ie pressed by MAIN key, mechine state will change to Running state.
  *         After that,the reset key reset by RESET_KEY_NUMBERS.

  * @note   If Enter key is pressed, check whether the state is Usb_config, if it is Usb_config.

  * @note   copy the image inside the usb stick into device else will print messege then Reset the Resetkey.
  *         usb_select_file() will select the file in /dev/ttyusb device file.

  * @note   If Enter key is pressed, check whether the state is Usb_config, if it is Usb_config 	
  *	    copy the image inside the usb stick into device else will print messege then Reset the Resetkey. 

  * @note   If Enter UP key is pressed scroll up the 16*2 display screen Increment the  sgUsbFileDisplayIndex and call the display-1 screen. 
  * 	    In usb mode this Up key will move the usb file list up, when GET_STATE() becomes EJACQUARD_STATE_USB_CONFIG.	
  *	    In usb mode this Down key Decrement the displayList[] and call the display-1 screen, when GET_STATE() becomes 
  *         EJACQUARD_STATE_USB_CONFIG.       
  * @param  UINT8_T key: charector key.
  * @retval None
  */


void keypad_handler(UINT8_T key)
{
	/* Do Nothing during the RUNNING STATE */

	if (GET_STATE() == EJACQUARD_STATE_RUNNING)  
	{
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
			if (GET_STATE() == EJACQUARD_STATE_USB_CONFIG)
				usb_scroll_up();
			else 
				/* increment the Pallu/borader number */
				process_increament_lines(gsKeyNumber);
			RESET_KEY_NUMBERS;
			break;
		case E_KEY_DOWN:
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

/**
  * @brief  initialize the keypad_handler into the keypad intrrupt line.
  * @note   Initialize UP,DOWN,MAIN keys.
  * @param  No parameter.
  * @retval zeo
  */



int keypad_init(void)
{
	int ret = 0;

	/* initialize the keypad_handler into the keypad intrrupt line */
	return 0;
}

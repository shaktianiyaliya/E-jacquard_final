/****************************************************************************

* Copyright (C) 2020 by Devsystech                                    *

* @version V1.0                             *

* This is the main part of the code which contain functional blocks of project *

****************************************************************************/

/**

* @file main.c

* @author Jayaram

* @date 17 Sep 2020

* @brief   Default main function.

* @brief File containing header files for file processing, usb drive access and dispaying in LCD.

*/


#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "display.h"
#include "comm.h"
#include "usb.h"
#include "process.h"
extern int keypad_init(void);


unsigned int  gState 	= EJACQUARD_STATE_IDLE;
unsigned int  gFileType	= EJACQUARD_FILE_TYPE_PALLU; /* Assuming default FileType is pallu */

/**

Main entry point of the program.

*/

int main(void)
{
	int ret = 0;

	ret = display_init();
	if (ret != 0 ) return -1;
	
	ret = comm_init();
	if (ret != 0 ) return -2;

	ret = usb_init();
	if (ret != 0 ) return -3;

	ret = keypad_init();
	if (ret != 0 ) return -4;

	ret = process_init();
	if (ret != 0 ) return -5;

	while (1) {

		switch(gState)
		{
			case EJACQUARD_STATE_IDLE:
				/* Do Nothing in Keypad intrrupt mode*/
				/* else we need to poll for keypad events*/
				break;
			case EJACQUARD_STATE_USB:
				usb_process_files();
				CHANGE_STATE(EJACQUARD_STATE_USB_CONFIG);
				break;
			case EJACQUARD_STATE_USB_CONFIG:
				/* Do Nothing in Keypad intrrupt mode*/
				/* else we need to poll for keypad events*/
				break;
			case EJACQUARD_STATE_RUNNING:
				ret = comm_process_cmd();
				if (ret != 0) {
				 	// TODO : Need to thing error case
					// We may restart the comm_reinit and check
					printf("Comm read failed , may need to re init\n");
				}
				break;
			default:
				break;
		}
	}
	return 0;
}

int is_file_exist(char *filename)
{
	int fd = open(filename , O_RDONLY); 
    	if (fd != -1 )
    	{
		close(fd);
		return 1; 
    	} 
	return 0;
}	

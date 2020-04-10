#include <stdio.h>
#include <string.h>
#include "main.h"
#include "usb.h"
#include "display.h"
#include "process.h"

#define USB_FILE_MAX_SIZE   	64
#define USB_MAX_FILES		5
#define USB_DEVICE_PATH 	"/dev/USB0"

static unsigned char sgUsbDisplayList[USB_MAX_FILES][USB_FILE_MAX_SIZE];
static unsigned int  sgUsbFileDisplayIndex = 0;
static unsigned int  sgUsbFilesCount = 0;

static char *sgDeviceFile = USB_DEVICE_PATH;
#define LIST_INC(Indx)	(Indx + 1) % (sgUsbFilesCount - 1)
#define LIST_DEC(Indx)	((Indx) == 0) ? Indx : ((Indx - 1) % (sgUsbFilesCount - 1))

/* Intrrupt/Event handler during the usb pulgin */
static void usb_plugin_handler(void *arg)
{
	/* We should mount the USB */
	usb_mount();
}

/* Intrrupt/Event handler during the usb pulgout */
static void usb_plugout_handler(void *arg)
{
	/* We may need to create usb unmount function during pulgout intrrupt */
	usb_unmount();
}

/* We may need to create usb mount function during pulgin intrrupt */
int usb_mount(void)
{
	int ret = 0;
	/* Change the gState into USB mode */
	CHANGE_STATE( EJACQUARD_STATE_USB );

	/* TODO: Mount the USB into the hardcode path */

	/* If mount fails we should CHANGE the state in to CONFIG */
	//CHANGE_STATE( EJACQUARD_STATE_IDLE );
	return ret;
}

int usb_unmount(void) 
{
	int ret = 0;

	/* TODO: UN Mount the USB  */
	/* Change the State Even USB unmount pass or fail */
	if (GET_STATE()  == EJACQUARD_STATE_USB || GET_STATE()  == EJACQUARD_STATE_USB_CONFIG ) 
		CHANGE_STATE (EJACQUARD_STATE_IDLE);
	return ret;
}

/* Initialize the USB handlers to the controller during usb_init */
int usb_init(void)
{
	int ret = 0;

	/* TODO Register the usb_plugout_handler and 
		usb_plugin_handler to the MCU usb interrupt line */

	sgUsbFileDisplayIndex = 0;
	sgUsbFilesCount  = 0;				
	memset(sgUsbDisplayList, 0 , USB_FILE_MAX_SIZE * USB_MAX_FILES);
	return ret;
}

/* Read Usb device and list the files and update the displayList[] */
int usb_process_files(void)
{
	int ret = 0;
	int fd =0 ;

	/* Re -set the variable before use :MJN: need to check if this need here or not */
	sgUsbFileDisplayIndex = 0;
	sgUsbFilesCount  = 0;				
	memset(sgUsbDisplayList, 0 , USB_FILE_MAX_SIZE * USB_MAX_FILES);

	 /* open a sgDeviceFile in read moide */
  	fd = open(sgDeviceFile , "r");
	if (fd < 0) { 
		printf("Device open error\n");
		return -1;
	}

	/* TODO: Read the file list from the USB device and fill the sgUsbDisplayList */	
	/* TODO: Update the sgUsbFilesCount on each file read */

	/* Display the files in the screan-1 */
	display_lines( sgUsbDisplayList[sgUsbFileDisplayIndex], sgUsbDisplayList[sgUsbFileDisplayIndex +1] );
	/* Display the content of the File in the screan2 */
	display_file(sgUsbDisplayList[sgUsbFileDisplayIndex]);

	return ret;
}
/* Increment the  sgUsbFileDisplayIndex and call the display-1 screen */
void usb_scroll_up(void)
{
	/* Circular loop increament in the sgUsbFileDisplayIndex */
	LIST_INC(sgUsbFileDisplayIndex);
	
	/* Call the display_lines() function call from the Display module */
	display_lines( sgUsbDisplayList[sgUsbFileDisplayIndex], sgUsbDisplayList[sgUsbFileDisplayIndex +1] );

	/* View the files in the Diplay screen-2 */
	display_file(sgUsbDisplayList[sgUsbFileDisplayIndex]);
}
  
/* Decrement the displayList[] and call the display-1 screen */
void usb_scroll_down(void)
{
	/* Circular loop increament in the sgUsbFileDisplayIndex */
	LIST_DEC(sgUsbFileDisplayIndex);
	
	/* Call the display_lines() function call from the Display module */
	display_lines( sgUsbDisplayList[sgUsbFileDisplayIndex], sgUsbDisplayList[sgUsbFileDisplayIndex +1] );
	
	/* View the files in the Diplay screen-2 */
	display_file(sgUsbDisplayList[sgUsbFileDisplayIndex]);
}


/* Copy the given image file in to the device */
int usb_select_file(void)
{
	int ret ;
	char *filename = sgUsbDisplayList[sgUsbFileDisplayIndex];
	ret = process_reload_bitmap(GET_FILE_TYPE());
	if (ret == 0 ) {
		/* copy the given file from the USB to the system in the predefine PATH */
		printf ( "Copy the current file into the back path \n");
		printf("Copy the current file into the BMP path\n");
	}
	return ret;
}


#ifndef _USB_H_
#define _USB_H_ 



/* We may need to create usb mount function during pulgin intrrupt */
int usb_mount();
/* We may need to create usb unmount function during pulgout intrrupt */
int usb_unmount();

/* Read Usb device and list the files and update the displayList[] */
int usb_read_files(void);

/* Increment the displayList[] and call the display-1 screen */
void usb_scroll_up(void);
  
/* Decrement the displayList[] and call the display-1 screen */
void usb_scroll_down(void);

/* Copy the given image file in to the device */
int usb_select_file(void);

/* Initialize the USB handlers to the controller during usb_init */
int usb_init(void);

#endif //#ifndef _USB_H_

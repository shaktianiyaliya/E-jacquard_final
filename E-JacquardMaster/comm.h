#ifndef _COMM_H
#define _COMM_H

#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>

#define EJACQUARD_SERIAL_UART 	"/dev/ttyUSB0"
#define EJACQUARD_BAUD_RATE 	B115200


/* Initialize the communication channel with extended board */
int comm_init(void);

/* Send the the Data to the Extended board thru Serial port */
int comm_send_data(UINT8_T *buf, UINT32_T size);

/* comm serial wait for the commnds to server for the extended board */
int comm_read_cmd(void);

#endif /* _COMM_H */

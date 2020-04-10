#include <stdio.h>
#include <string.h>
#include "main.h"
#include "comm.h"
#include "process.h"
#define COMM_START	'*'
#define COMM_STOP	'&'
#define COMM_NEXT	'?'
#define COMM_PAUSE	'!'


static int sgFileDesc = 0;
static char comm_buf[256];
/*
 *
 *  Configures the serial terminal settings.
 */
static int serial_init(int fd, UINT32_T baudRate)
{
	struct termios term;

	if(-1 == tcgetattr(fd, &term))
	{
		perror("init_termios");
		fprintf(stderr, "could not get terminal settings for ttyS0\n");
		return 1;
	}

	/* clear ISIG, ICANON, IEXTEN flags */
	term.c_lflag &= ~(ISIG | ICANON | IEXTEN | ECHO);
	/* clear ICRNL, IXON flags */
	term.c_iflag &= ~(ICRNL | IXON);
	/* clear HUPCL flag */
	term.c_cflag &= ~(HUPCL);
	/* clear OPOST flag */
	term.c_oflag &= ~(OPOST);

	/* set baud-rate to SERIAL_BAUD_RATE */
	if(-1 == cfsetspeed( &term, baudRate))
	{
		fprintf(stderr, "could not set up baud rate for ttyS0\n");
		return 2;
	}
	if(-1 == tcsetattr(fd, TCSANOW, &term))
	{
		fprintf(stderr, "could not set up terminal settings for ttyS0\n");
		return 3;
	}

	return 0;
}


/* Initialize the communication channel with extended board */
int comm_init(void)
{
	int fd;  
	struct termios SPortSettings;
	fd = open(EJACQUARD_SERIAL_UART, O_RDWR | O_NOCTTY);

	if (fd < 0)
	{
		printf("Error opening Serial Port!\n");
		return fd;
	}
	if(serial_init(fd, EJACQUARD_BAUD_RATE))
	{
		return -1;
	}

	sgFileDesc = fd;
	return 0;
}


/* Send the the Data to the Extended board thru Serial port */
int comm_send_data(UINT8_T *buf, UINT32_T size)
{
	UINT32_T len = 0;
	int ret = 1;
	/* Write the  given binary data in to the serial porth */
	/* size --> size of the data that we should write into the serial port */
	while ( ret < 0 && len < size ) { 
		ret  = write(sgFileDesc, buf, size);
		len++;	
	}
	if ( len < size ) {
		printf(" Communication error \n");
		return -1;
	}
	printf(" The data of size = %d is wrriten in the Serial port\n",size);
	return 0;
}

/* comm serial wait for the commnds to server for the extended board */
int comm_process_cmd(void)
{
	int ret = 0;
	char  cmd = ' ';
	unsigned int size = 0;
	/* Read the cmds from the extended board and change the appropriate state */
	ret = read(sgFileDesc , &cmd, 1);
	if(ret > 0 )
	{
		switch (cmd)
		{
			case COMM_START: 
				memset(&comm_buf, 0 , 256);
				/* read the data from process and send in comm port */
				size = process_read_data(comm_buf);
				if (size > 0 ) 
					ret = comm_send_data(comm_buf, size); 	
				
				break;
			case COMM_STOP:	
				/* Change to Idle State */
				CHANGE_STATE( EJACQUARD_STATE_IDLE );
				/* Close the close the channel */
				 // TODO: close(sgFileDesc);
				/* memset the comminucation channel */
				memset(&comm_buf, 0 , 256);
				break;
			case COMM_NEXT:	
				/* Increament the Pallu and Borader Counter */
				process_increament_lines(1);

				memset(&comm_buf, 0 , 256);
				/* read the data from process and send in comm port */
				size = process_read_data(comm_buf);
				if (size > 0 ) 
					ret = comm_send_data(comm_buf, size); 	
				break;
			case COMM_PAUSE:	
				/* Increament the Pallu and Borader Counter */
				process_increament_lines(1);
				/* Change to Idle State */
				CHANGE_STATE( EJACQUARD_STATE_IDLE );
				/* memset the comminucation channel */
				memset(&comm_buf, 0 , 256);
				break;
			default: 
				break;/* Just ignore */
		}
	}
	return ret; 

}

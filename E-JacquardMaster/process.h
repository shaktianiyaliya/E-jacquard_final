#ifndef _PROCESS_H_
#define _PROCESS_H_

/*****************************************************************************
*
* Data types.
*
* INT8 is an integer of at least 8 bits wide.
* INT16 is an integer of at least 16 bits wide.
* INT32 is an integer of at least 32 bits wide.
*
* UINT8 is an unsigned INT8
* UINT16 is an unsigned INT16
* UINT32 is an unsigned INT32
*/

#if 0
typedef int8_t          INT8;
typedef int16_t         INT16;
typedef int32_t         INT32;
typedef uint8_t         UINT8;
typedef uint16_t        UINT16;
typedef uint32_t        UINT32;

#else

typedef signed char          INT8;
typedef signed short int     INT16;
typedef signed int           INT32;
typedef unsigned char        UINT8;
typedef unsigned short int   UINT16;
typedef unsigned int         UINT32;
#endif


typedef struct ejacqaurd_file_info
{
    unsigned char *bitmapImage;
    unsigned int  TotalColumns;
    unsigned int  TotalLines;
    unsigned int  CurrentLine;
}EJACQAURD_FILE_INFO_T; 


extern int process_reload_bitmap(EJACQUARD_FILE_TYPE_T type);
extern int process_init(void);
extern int process_read_data(unsigned char *buf);
extern int process_update_line(void);
extern int process_increament_line(unsigned int no);
extern int process_decrement_lines(unsigned int no);
//extern void process_display_lines(void);
#endif //__PROCESS_H_

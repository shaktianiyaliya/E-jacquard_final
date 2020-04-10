#ifndef _MAIN_H_
#define _MAIN_H_
#if 1
typedef unsigned int  	UINT32_T;
typedef unsigned char  	UINT8_T;
typedef int 	INT32_T;
typedef char  	INT8_T;
#endif

typedef enum {
        EJACQUARD_STATE_IDLE = 0,
        EJACQUARD_STATE_RUNNING ,
        EJACQUARD_STATE_USB , 
	EJACQUARD_STATE_USB_CONFIG,
        EJACQUARD_STATE_MAX 
} EJACQUARD_STATE_T;

extern UINT32_T gState;
#define CHANGE_STATE(changeState) gState = (changeState)
#define GET_STATE() gState

typedef enum {
        EJACQUARD_FILE_TYPE_PALLU = 0,
        EJACQUARD_FILE_TYPE_BODY = 1,
        EJACQUARD_FILE_TYPE_MAX
} EJACQUARD_FILE_TYPE_T;

extern UINT32_T gFileType;
#define CHANGE_FILE_TYPE(fType)  gFileType = (fType)
#define GET_FILE_TYPE()	gFileType


#define EJACQUARD_FILE_PATH "/tmp/"
#define EJACQUARD_PALLU_FILE "pallu.bmp"
#define EJACQUARD_BODY_FILE  "body.bmp"

int is_file_exist(char *filename);
#endif

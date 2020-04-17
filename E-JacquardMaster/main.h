* @file    main.h

#ifndef _MAIN_H_
#define _MAIN_H_
#if 1
typedef unsigned int  	UINT32_T;/* Declaring user defined data type name UINT32_T of type unsigned int */
typedef unsigned char  	UINT8_T; /* Declaring user defined data type name UINT8_T of type unsigned int */
typedef int 	INT32_T; /* Declaring user defined data type name INT32_T of type unsigned int */
typedef char  	INT8_T;  /* Declaring user defined data type name INT8_T of type unsigned int */
#endif

/**
 * @brief "EJACQUARD_STATE_T" is a user defined data type.
 * which holds different state of EJACQUARD mechine.
 * apart from that it is an enamurated type, the value of
 * members inside the "typedef enum" will varries from 0 to 4.
 */


typedef enum {
        EJACQUARD_STATE_IDLE = 0, /**< Idle state of the mechine . */
        EJACQUARD_STATE_RUNNING , /**< Running state of the mechine . */
        EJACQUARD_STATE_USB ,   /**< Usb state of the mechine . */
	EJACQUARD_STATE_USB_CONFIG, /**< Usb config state of the mechine . */
        EJACQUARD_STATE_MAX   /**< Maximum state of the mechine . */
} EJACQUARD_STATE_T;

extern UINT32_T gState;
#define CHANGE_STATE(changeState) gState = (changeState)  // Used for changing the state of the mechine time to tme.
#define GET_STATE() gState // Used for getting the updated state of the mechine.

/**
 * @brief "EJACQUARD_FILE_TYPE_T" is a user defined data type.
 * which holds different file types of EJACQUARD mechine.
 * apart from that it is an enamurated type, the value of
 * members inside the "typedef enum" will varries from 0 to 3.
 */



typedef enum {
        EJACQUARD_FILE_TYPE_PALLU = 0,/**< Border file indicator . */
        EJACQUARD_FILE_TYPE_BODY = 1,/**< Body file indicator . */
        EJACQUARD_FILE_TYPE_MAX  /**< Max file indicator . */
} EJACQUARD_FILE_TYPE_T;

extern UINT32_T gFileType; /* Declaring a variable name: gFileType, of user defined data type UINT32_T  */
#define CHANGE_FILE_TYPE(fType)  gFileType = (fType) // Used for changing the file type inside the usb time to time.
#define GET_FILE_TYPE()	gFileType // Used for getting the updated file inside usb.


#define EJACQUARD_FILE_PATH "/tmp/" // Used for getting file path of usb inserted,by default which will inside tmp directory.
#define EJACQUARD_PALLU_FILE "pallu.bmp" // Used for accessing border file.
#define EJACQUARD_BODY_FILE  "body.bmp" // Used for accessing body file.

int is_file_exist(char *filename); /* function declaration definition @ main.c  */
#endif

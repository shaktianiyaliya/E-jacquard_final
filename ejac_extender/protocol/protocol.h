/*
 * protocol.h
 *
 *  Created on: 01-Feb-2018
 *      Author: Santosh P N
 */

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#include "stm32f4xx.h"

#define L_COLS  (16)
#define L_ROWS  (16)
//********************** Action Types *********************************//
typedef enum {STATUS=0,LOAD,START,STOP,JUMP,IMAGE_CONFIG, HARDWARE_CONFIG, IMAGE_INFO, IMAGE_DETAIL_STATUS,HARDWARE_DETAIL_STATUS, FEED_DATA, IDLE, BUSY, TEST_PATTERN}ejac_action_type;
typedef enum {SUCCES=0, SUCCESS_MORE_DATA, FAIL, FAIL_FLASH, IMG_WIDTH_ERROR}ejac_status_type;
typedef enum {PNP=0,NPN}ejac_sensor_type;
typedef enum {BORDER=40,BODY}ejac_BB_type;
typedef enum {OFF=0,ON,BREAK}ejac_sensor_usage;
typedef enum {SINGLE = 1, CONCATENATE, MERGE}ejac_operation_type;
typedef enum {RUNNING = 0, SETTING}ejac_mode;
typedef enum {VALID = 0, INVALID}ejac_validity;
typedef enum {EJAC_ENABLE = 0, EJAC_DISABLE}ejac_enable_disable;

// ******************** Action cmd structure ****************************//
typedef struct __attribute__((__packed__))
{
	uint16_t MagicNumber;
	uint8_t Uid;
	uint8_t Action;
}ejac_cmd_request_t;

typedef struct __attribute__((__packed__))
{
	uint8_t FileId;
	uint32_t Height;
	uint32_t Width;
//	uint32_t Start;
//	uint32_t End;
}ejac_cmd_load_t;

typedef struct __attribute__((__packed__))
{
	uint32_t Line1;
	uint32_t Itr1;
	uint32_t Line2;
	uint32_t Itr2;
	/*uint32_t Line3;
	uint32_t Itr3;
	uint32_t Line4;
	uint32_t Itr4;*/
	uint32_t GlobalCurr;
}ejac_cmd_start_t;


typedef struct __attribute__((__packed__))
{
	uint8_t FileID;
	uint32_t Iteration;
	uint8_t BBType;
}ejac_cmd_image_info_t;

typedef struct __attribute__((__packed__))
{
	ejac_cmd_image_info_t ImgOrder[10];
}ejac_cmd_image_order_t;
// ******************** Response packet structure **************************//

typedef struct __attribute__((__packed__))
{
	uint16_t MagicNumber;
	uint8_t Uid;
	uint8_t Action;
	uint8_t Status;
}ejac_cmd_response_t;

typedef struct __attribute__((__packed__))
{
	uint32_t CurrLine1;
	uint32_t Itr1;
	uint32_t CurrLine2;
	uint32_t Itr2;
	uint32_t CurrLine3;
	uint32_t Itr3;
	uint32_t CurrLine4;
	uint32_t Itr4;
	uint32_t GlobalCurr;

}ejac_cmd_resp_t;

// ******************** Config cmd structure **************************//

typedef struct __attribute__((__packed__))
{
	uint8_t Sensor_Type;
	uint8_t Sensor_Usage;
	uint8_t Sensor_Status;
}ejac_sensor_t;


typedef struct __attribute__((__packed__))
{
	uint8_t FileID;
	uint8_t Iterator;
	uint32_t Height;
	uint32_t Width;
	uint32_t BytesPerRow;
//	uint32_t Start;
//	uint32_t End;
	uint32_t FlStartAdr;
	uint32_t FlEndAdr;
}ejac_image_t;


typedef struct __attribute__((__packed__))
{
	uint8_t ModeType;
	uint8_t ImageCount;
	uint8_t LogicColumns [L_COLS];
}ejac_cmd_image_config_t;

typedef struct __attribute__((__packed__))
{
	ejac_cmd_image_config_t ImageConfig;
	ejac_image_t ExtraConfig1;
	ejac_image_t ExtraConfig2;
	//ejac_image_t ExtraConfig3;
	//ejac_image_t ExtraConfig4;

}ejac_image_extra_config_t;

typedef struct __attribute__((__packed__))
{
	uint8_t MaxRows;
	uint8_t MaxColumns;
	ejac_sensor_t Sensor1;
	ejac_sensor_t Sensor2;
	//ejac_sensor_t Sensor3;
	uint8_t Distribution;
	uint8_t ActiveRows [L_ROWS];
	uint8_t ActiveColumns [L_COLS];
}ejac_cmd_hardware_settings_t;

typedef struct __attribute__((__packed__))
{
	uint8_t Pattern[256];
	uint8_t Rep;
	uint8_t testType;
	uint16_t modularNum;
}ejac_cmd_test_pattern_t;

#endif /* PROTOCOL_H_ */

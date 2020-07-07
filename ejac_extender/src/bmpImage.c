#include "bmpImage.h"
#include "ejacTrace.h"
#include "protocol.h"
#include <string.h>

#if (MERGE_IMAGE_TEST == 1)
ejac_image_extra_config_t test_config;
ejac_cmd_image_order_t test_image_order;
extern volatile int32_t line_count1, line_count2, line_count3, line_count4;
extern volatile uint32_t itr1,itr2,itr3,itr4,g_curr_line;
volatile uint8_t action_flag;

void test_image_init(void)
{
	ejac_trace_uart_init();
}


void write_headerinfo(uint32_t  width, uint32_t height)
{
	BmpImageHeader imgHeader;
	uint32_t BytesPerRow, palettesize;
	imgHeader.bitmapInfoHeader.biSize = SIZE_OF_INFO_HEADER;
	imgHeader.bitmapInfoHeader.biWidth = (width * 8);
	imgHeader.bitmapInfoHeader.biHeight = height;
	imgHeader.bitmapInfoHeader.biPlanes = 1;
	imgHeader.bitmapInfoHeader.biBitCount = 1;
	imgHeader.bitmapInfoHeader.biCompression = 0;
	imgHeader.bitmapInfoHeader.biXPelsPerMeter = 0;
	imgHeader.bitmapInfoHeader.biYPelsPerMeter = 0;
	imgHeader.bitmapInfoHeader.biClrUsed = 2;
	imgHeader.bitmapInfoHeader.biClrImportant = 0;

	BytesPerRow = ((((width * 8)* imgHeader.bitmapInfoHeader.biBitCount) + 31) / 32) * 4;
	palettesize = 8;
	uint8_t  padBytes = 0;

	if( ((width % 32) == 0) || ((width % 32) > 24))
	{
		padBytes = 0;
	}
	else if ((width % 32) <= 8)
	{
		padBytes = 3;
	}
	else if ((width % 32) <= 16)
	{
		padBytes = 2;
	}
	else
	{
		padBytes = 1;
	}

    imgHeader.bitmapFileHeader.bfType = TYPE_BMP;
    imgHeader.bitmapFileHeader.bfSize = SIZE_OF_HEADER + palettesize + ((BytesPerRow + padBytes) * imgHeader.bitmapInfoHeader.biHeight);;
    imgHeader.bitmapFileHeader.bfReserved1 = 0;
    imgHeader.bitmapFileHeader.bfReserved2 = 0;
    imgHeader.bitmapFileHeader.bOffBits = (sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + palettesize);
    imgHeader.bitmapInfoHeader.biSizeImage = ((BytesPerRow + padBytes) * height);

    memset(imgHeader.palleteBuffer, 0, PALETTE_SIZE);
    imgHeader.palleteBuffer[4]=0xff;
    imgHeader.palleteBuffer[5]=0xff;
    imgHeader.palleteBuffer[6]=0xff;
    ejac_trace((uint8_t *)&imgHeader, sizeof(BmpImageHeader));
}

void merge_image(void)
{
	uint32_t activeRowSize = 10 * 12;
	uint8_t cl_line[256];
	uint8_t borderLine[256];
	uint8_t bodyLine[256];
	uint32_t i;

	memset(&cl_line[0], 0, activeRowSize);
	memset(&borderLine[0], 0, activeRowSize );
	memset(&bodyLine[0], 0, activeRowSize);

	if(line_count1 == test_config.ExtraConfig1.Height)
	{
		line_count1 = 0;
		itr1++;
		action_flag = IDLE;
		return;
	}
	if(line_count2 == test_config.ExtraConfig2.Height)
	{
		line_count2 = 0;
		itr2++;
	}

	//action_flag = START;
	if(test_image_order.ImgOrder[0].BBType == BORDER)
	{
		memcpy(&borderLine[0],(uint32_t *)(test_config.ExtraConfig1.FlStartAdr +(line_count1*test_config.ExtraConfig1.BytesPerRow)), test_config.ExtraConfig1.BytesPerRow);
		line_count1++;
	}

	 if(test_image_order.ImgOrder[1].BBType == BODY)
	{
		memcpy(&bodyLine[0],(uint32_t *)(test_config.ExtraConfig2.FlStartAdr +(line_count2*test_config.ExtraConfig2.BytesPerRow)), test_config.ExtraConfig2.BytesPerRow);
		line_count2++;
	}

	 activeRowSize = 120;
	if( (bodyLine[test_config.ExtraConfig2.BytesPerRow-1] & 0xff) != 0)
	{
		for (i =0 ; i<activeRowSize; i++)
		{
			cl_line[i] = bodyLine[i] & borderLine[i];
			//cl_line[i] = (bodyLine[i] | borderLine[i]);
		}
	}
	else
	{
		for (i = 0 ; i<activeRowSize; i++)
		{
			cl_line[i] = borderLine[i];
		}
	}
	//vert_distribution(&cl_line[0], 120); //MJN

	ejac_trace((uint8_t *)cl_line, activeRowSize);
}

void send_merge_image_data(void)
{
	static uint8_t sentHeader = 0;

	if(!sentHeader)
	{
		memcpy(&test_config, (uint32_t *)0x08010000, sizeof(ejac_image_extra_config_t));
		memcpy(&test_image_order, (uint32_t *)0x080E0000, sizeof(ejac_cmd_image_order_t));
		write_headerinfo(test_config.ExtraConfig1.Width,test_config.ExtraConfig1.Height);
		sentHeader = 1;
	}
	else
	{
		merge_image();
	}
}
#endif

#ifndef __BMPIMAGE_H__
#define __BMPIMAGE_H__

#include <stdint.h>

#define SIZE_OF_HEADER          54
#define SIZE_OF_INFO_HEADER     40


#define MASK(x) ((uint8_t)(0x01)<<x)
#define TEST_IMAGE_WEAVING		20
#define TYPE_BMP            (0x4d42)   /* 'BM' */
#define PALETTE_SIZE	8
/*****************************************************************************
*
* Structures.
*/

/*
 * BITMAPFILEHEADER contains the info about the filsesize and the offset from
 * where the image data starts.
 *
 * A BITMAPINFOHEADER structure is always concatenated to the end of a
 * BITMAPFILEHEADER structure.
 */

typedef struct __attribute__((__packed__))
{
    uint16_t  bfType;  //specifies the file type
    uint32_t  bfSize;  //specifies the size in bytes of the bitmap file
    int16_t   bfReserved1;  //reserved; must be 0
    int16_t   bfReserved2;  //reserved; must be 0
    uint32_t  bOffBits;  //species the offset in bytes from the bitmapfileheader to the bitmap bits
}BITMAPFILEHEADER;


/*
 * BITMAPINFOHEADER defines the properties of a bitmap.
 *
 * A color table is concatenated to this structure.  The number of elements in
 * the color table determined by the bit-depth of the image.
 *
 */

typedef struct __attribute__((__packed__))
{
    uint32_t  biSize;  //specifies the number of bytes required by the struct
    uint32_t  biWidth;  //specifies width in pixels
    uint32_t  biHeight;  //species height in pixels
    uint16_t  biPlanes; //specifies the number of color planes, must be 1
    uint16_t  biBitCount; //specifies the number of bit per pixel
    uint32_t  biCompression;//spcifies the type of compression
    uint32_t  biSizeImage;  //size of image in bytes
    uint32_t  biXPelsPerMeter;  //number of pixels per meter in x axis
    uint32_t  biYPelsPerMeter;  //number of pixels per meter in y axis
    uint32_t  biClrUsed;  //number of colors used by th ebitmap
    uint32_t  biClrImportant;  //number of colors that are important */
}BITMAPINFOHEADER;

typedef struct __attribute__((__packed__))
{
	BITMAPFILEHEADER 	bitmapFileHeader;
	BITMAPINFOHEADER	bitmapInfoHeader;
	uint8_t palleteBuffer[PALETTE_SIZE];
}BmpImageHeader;

#if (MERGE_IMAGE_TEST == 1)
void send_merge_image_data(void);
void test_image_init(void);
#endif
#endif

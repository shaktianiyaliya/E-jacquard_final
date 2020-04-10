#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"
#include "process.h"


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

#pragma pack(push, 1)
typedef struct tagBITMAPFILEHEADER
{
    UINT16  bfType;  //specifies the file type
    UINT32  bfSize;  //specifies the size in bytes of the bitmap file
    INT16   bfReserved1;  //reserved; must be 0
    INT16   bfReserved2;  //reserved; must be 0
    UINT32  bfOffBits;  //species the offset in bytes from the bitmapfileheader to the bitmap bits
}BITMAPFILEHEADER;
#pragma pack(pop)

/*
 * BITMAPINFOHEADER defines the properties of a bitmap.
 *
 * A color table is concatenated to this structure.  The number of elements in
 * the color table determined by the bit-depth of the image.
 *
 */

#pragma pack(push, 1)
typedef struct tagBITMAPINFOHEADER
{
    UINT32  biSize;  //specifies the number of bytes required by the struct
    UINT32  biWidth;  //specifies width in pixels
    UINT32  biHeight;  //species height in pixels
    UINT16  biPlanes; //specifies the number of color planes, must be 1
    UINT16  biBitCount; //specifies the number of bit per pixel
    UINT32  biCompression;//spcifies the type of compression
    UINT32  biSizeImage;  //size of image in bytes
    UINT32  biXPelsPerMeter;  //number of pixels per meter in x axis
    UINT32  biYPelsPerMeter;  //number of pixels per meter in y axis
    UINT32  biClrUsed;  //number of colors used by th ebitmap
    UINT32  biClrImportant;  //number of colors that are important */
}BITMAPINFOHEADER;
#pragma pack(pop)

// Declare the static global variable

static EJACQAURD_FILE_INFO_T gPallu[EJACQUARD_FILE_TYPE_MAX];
static char gBmpFiles[EJACQUARD_FILE_TYPE_MAX][64] ;

   
static unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
{
    FILE *filePtr; //our file pointer
    int imageIdx=0;  //image index counter
    unsigned char *bitmapImage = NULL;  //store image data
    BITMAPFILEHEADER bitmapFileHeader; //our bitmap file header

    //unsigned char tempRGB;  //our swap variable
    //open filename in read binary mode
    filePtr = fopen(filename,"rb");
    if (filePtr == NULL)
        return NULL;

    //read the bitmap file header
    fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER),1,filePtr);

    //verify that this is a bmp file by check bitmap id
    if (bitmapFileHeader.bfType !=0x4D42)
    {
        fclose(filePtr);
        return NULL;
    }

    //read the bitmap info header
    fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER),1,filePtr); // small edit. forgot to add the closing bracket at sizeof

    //move file point to the begging of bitmap data
    fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

    //allocate enough memory for the bitmap image data
    bitmapImage = (unsigned char*)malloc(bitmapInfoHeader->biSizeImage);

    //verify memory allocation
    if (!bitmapImage)
    {
        free(bitmapImage);
        fclose(filePtr);
        return NULL;
    }

    //read in the bitmap image data
    fread(bitmapImage,bitmapInfoHeader->biSizeImage, 1 ,filePtr);

    //make sure bitmap image data was read
    if (bitmapImage == NULL)
    {
        fclose(filePtr);
        return NULL;
    }

#if 0
    //swap the r and b values to get RGB (bitmap is BGR)
    for (imageIdx = 0;imageIdx < bitmapInfoHeader->biSizeImage;imageIdx+=3) // fixed semicolon
    {
        tempRGB = bitmapImage[imageIdx];
        bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
        bitmapImage[imageIdx + 2] = tempRGB;
    }
#endif
    //close file and return bitmap image data
    fclose(filePtr);
    return bitmapImage;
}


static void process_display_lines(void)
{
	unsigned char line1[16] = {0};
	unsigned char line2[16] = {0};

	sprintf(line1, "PALLU %d / %d", 
		gPallu[EJACQUARD_FILE_TYPE_PALLU].CurrentLine, gPallu[EJACQUARD_FILE_TYPE_PALLU].TotalLines);
	sprintf(line1, "BODY %d / %d", 
		gPallu[EJACQUARD_FILE_TYPE_BODY].CurrentLine, gPallu[EJACQUARD_FILE_TYPE_BODY].TotalLines);

	display_lines(line1, line2);
}


int process_reload_bitmap(EJACQUARD_FILE_TYPE_T type)
{
	unsigned char *bitmapImage =  NULL;
	BITMAPINFOHEADER infoHeader = {0};

	if ( type > EJACQUARD_FILE_TYPE_MAX) {
		printf(" Unnknow Bmp file Type \n");
		return -1;
	}

	bitmapImage = LoadBitmapFile(gBmpFiles[type], &infoHeader);

	if (bitmapImage == NULL ) {
		printf(" bmpImageParser Error\n");
		return -1;
	}
	free (gPallu[type].bitmapImage);
	gPallu[type].bitmapImage = bitmapImage;
	gPallu[type].TotalLines = infoHeader.biHeight;
	gPallu[type].TotalColumns = infoHeader.biWidth;
	gPallu[type].CurrentLine = 0;
	
	return 0;
}


int process_increament_lines(unsigned int no)
{
	int i = 0;

	if ( /* Some Hard code max line*/ 99 < no)
		return -1; 
	
	for (i = 0; i < EJACQUARD_FILE_TYPE_MAX; i++)
	{
		gPallu[i].CurrentLine = (gPallu[i].CurrentLine + no) % gPallu[i].TotalLines;
	}
	process_display_lines();
	return 0;
}


int process_decrement_lines(unsigned int no)
{
	int i = 0;

	if ( /* Some Hard code max line*/ 99 < no)
		return -1; 

	for (i = 0; i < EJACQUARD_FILE_TYPE_MAX; i++)
	{
		gPallu[i].CurrentLine = (gPallu[i].CurrentLine >= no ) ? 
						(gPallu[i].CurrentLine - no) : 
						(gPallu[i].TotalLines - (no - gPallu[i].CurrentLine));
	}
	process_display_lines();
	return 0;
}

int process_read_data(unsigned char *buf)
{
	unsigned int indx = 0;
	int i = 0;
	
	for (i = 0; i < EJACQUARD_FILE_TYPE_MAX; i++)
	{
		memcpy(buf + indx, 
			gPallu[i].bitmapImage + (gPallu[i].CurrentLine * gPallu[i].TotalColumns),
			gPallu[i].TotalColumns );
		indx = indx + gPallu[i].TotalColumns;
	}

	return indx;
}

int process_init(void)
{
	int i = 0;
	BITMAPINFOHEADER infoHeader = {0};
	/* Initialize the pallu and body files */
	sprintf(gBmpFiles[EJACQUARD_FILE_TYPE_PALLU], "%s%s",  EJACQUARD_FILE_PATH , EJACQUARD_PALLU_FILE);
	sprintf(gBmpFiles[EJACQUARD_FILE_TYPE_BODY], "%s%s",  EJACQUARD_FILE_PATH , EJACQUARD_BODY_FILE);

	for (i = 0; i < EJACQUARD_FILE_TYPE_MAX; i++)
	{
		/* memst the Pallu and body files info structures*/
		memset(&gPallu[i], 0, sizeof(EJACQAURD_FILE_INFO_T));

		/* Load the file into the bitstream */
		gPallu[i].bitmapImage = LoadBitmapFile(gBmpFiles[i], &infoHeader);

		if (gPallu[i].bitmapImage == NULL ) {
			printf(" bmpImageParser Error\n");
			return -1;
		}
		gPallu[i].TotalLines = infoHeader.biHeight;
		gPallu[i].TotalColumns = infoHeader.biWidth;
		gPallu[i].CurrentLine = 0;
	}	
	process_display_lines();
	return 0;


}


#if 0
void print_header_details(BITMAPFILEHEADER *FileHeader, BITMAPINFOHEADER *InfoHeader)
{
    printf("\nType: %#X", FileHeader->bfType);
    printf("\nSize: %u", FileHeader->bfSize);
    printf("\nReserved 1: %hd", FileHeader->bfReserved1);
    printf("\nReserved 2: %hd", FileHeader->bfReserved2);
    printf("\nOffset: %d", FileHeader->bOffBits);

    printf("\n\nSize: %u", InfoHeader->biSize);
    printf("\nWidth: %d", InfoHeader->biWidth);
    printf("\nHeight: %d", InfoHeader->biHeight);
    printf("\nPlanes: %hd", InfoHeader->biPlanes);
    printf("\nBits per pixel: %d", InfoHeader->biBitCount);
    printf("\nCompression: %d", InfoHeader->biCompression);
    printf("\nImageSize: %d", InfoHeader->biSizeImage);
    printf("\nxResolution: %d", InfoHeader->biXPelsPerMeter);
    printf("\nyResolution: %d", InfoHeader->biYPelsPerMeter);
    printf("\nColors: %d", InfoHeader->biClrUsed);
    printf("\nImportantColors: %d", InfoHeader->biClrImportant);
    printf("\n");
}
#endif


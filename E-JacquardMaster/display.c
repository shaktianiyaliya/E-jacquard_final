#include <stdio.h>
#include "main.h"
#include "display.h"

/* Name: Display_lines
 * 
 *
 *
 */
int display_init(void)
{
	int ret = 0;
	/* Initilize the 12 x 2 segment : Display1: */
	/* initilize the display2: */
	printf ("\n Display - 1 Initialized\n");
	printf ("\n Display - 2 Initialized\n");
	return ret;
}

int display1_clear(void) 
{
	int ret = 0;
 	/* Clear the Display1 or clear the lines 12 x 2 segment display*/
	printf(" Clear the display1\n");
	return ret;
}
	
int display2_clear(void) 
{
	int ret = 0;
 	/* Clear the Display-2/ Clear the images */
	printf(" Clear the display2\n");
	return ret;
}

int display_lines(UINT8_T *line1, UINT8_T *line2)
{ 
	int ret = 0;
	display1_clear();
	/* Display Lines in the 12 x 2 segment display-1 */
	printf ("\n %s \n ", (unsigned char *) line1);
	printf ("\n %s \n ", (unsigned char *) line2);

	return ret;
}

int display_file(char *filename)
{
	int ret = 0;
	
	display2_clear();
	/* Display the new file content in the Display -2 */

	return ret;
}

void display_flashlight(UINT32_T lineno)
{
	/* Display the flash light on the given line number of the screen */
	printf("Display flash on the line no %d\n", lineno);
}

int display_image_with_highlight(UINT8_T *buf, UINT32_T len, UINT32_T markno)
{
	int ret = 0;
	
	display2_clear();

	/* Display the image data content in the Display -2 and highlight the line*/
	
	/* Call the flashlight on the given markno */
	display_flashlight(markno);	

	return ret;
}


#ifndef DISPLAY_H
#define DISPLAY_H

/* Initilize the 12 x 2 segment : Display1: */
/* Initilize the display2: */
int display_init(void);

/* Clear the Display1 or clear the lines 12 x 2 segment display*/
int display1_clear(void) ;

/* Clear the Display-2/ Clear the images */
int display2_clear(void) ;

/* Display Lines in the 12 x 2 segment display */
int display_lines(UINT8_T *line1, UINT8_T *line2);

/* Display the new file content in the Display -2 */
int display_file(char *filename);

/* Display the flash light on the given line number */
void display_flashlight(UINT32_T lineno);

/* Display the image on the display-2 and Highlight/flashlight on the given line/markno */
int display_image_with_highlight(UINT8_T *buf, UINT32_T len, UINT32_T markno);
 
#endif

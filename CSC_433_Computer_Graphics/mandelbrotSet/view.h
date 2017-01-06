/*
 * view.h
 *
 * This file has functions concerning the mandelbrot set itslef
 */

#ifndef __VIEW_H
#define __VIEW_H

#include <unistd.h>

/* GLOBAL VARABLES */
// Window stuff
extern int width, height, zoomCount, mandelColor, juliaColor, zoomLimit;
extern float xMid, yMid, xJul, yJul, scale, interval, range;
extern bool rotateFlag;


/*
 * Function: recenter
 *
 * Authors: Elliott Rarden & Katie Macmillan
 *
 * Description: Takes in an x and y coordinate and sets them to the
 * 				new mid point of the image
 *
 * Parameters:
 *		[in] x - The current x coordinate (complex plane) to be the new
 *				  xMid
 *		[in] y - The current y coordinate (complex plane) to be the new
 *				  yMid
 *
 */
void recenter(float x, float y) {
	x = (2*x)/width - 1;
	y = (2*y)/height - 1;

	xMid = xMid + (x/scale);
	yMid = yMid - (y/scale);
}

/*
 * Function: panClick
 *
 * Authors: Elliott Rarden & Katie Macmillan
 *
 * Description: Takes in an x and y coordinate and sets them to the
 * 				new mid point of the image
 *
 * Parameters:
 *		[in] x - The current x coordinate (complex plane) to be the new
 *				  xMid
 *		[in] y - The current y coordinate (complex plane) to be the new
 *				  yMid
 *
 */
void panClick(float x, float y) {
	x = (x)/width;
	y = (y)/height;

	xMid -= x/scale;
	yMid += y/scale;
}

/*
 * Function: panButton
 *
 * Authors: Elliott Rarden & Katie Macmillan
 *
 * Description: Takes in an x and y coordinate and sets them to the
 * 				 new mid point of the image
 *
 * Parameters:
 *		[in] c - The direction in which the user wishes to move
 *
 */
void panButton(char d) {
	switch (d){
		case 'u':
			yMid = yMid + (0.2/scale);
		break;

		case 'd':
			yMid = yMid - (0.2/scale);
		break;

		case 'r':
			xMid = xMid + (0.2/scale);
		break;

		case 'l':
			xMid = xMid - (0.2/scale);
		break;

	}
}

/*
 * Function: zoomIn
 *
 * Authors: Elliott Rarden & Katie Macmillan
 *
 * Description:
 * zoomIn() changes our interval, scale, and range so that we are showing 2 times
 * more detail in the image. 
 */
void zoomIn(){
	// Update interval, scale and range
	range = range/2.0;
	interval = interval/2.0;
	scale = scale*2.0;
	// Increment zoom count
	zoomCount++;
}

/*
 * Function: zoomOut
 *
 * Authors: Elliott Rarden & Katie Macmillan
 *
 * Description:
 * zoomOut() changes our interval, scale, and range so that we are showing 1/2 as
 * much detail in the image.
 */
void zoomOut(){
	// Update interval, scale and range
	interval = interval*2.0;
	scale = scale/2.0;
	range = range*2.0;
	// Decrement zoom count
	zoomCount--;
}

/*
 * Function: rotateColor
 *
 * Authors: Elliott Rarden & Katie Macmillan
 *
 * Description:
 * rotateColor() changes the colour scheme once every second.
 */
void rotateColor(){
	// Check that rotate flag is on
	if(rotateFlag){
		// Prevent epileptic seizurs
		sleep(1);
		// Move mandel color and julia color up by 1
		mandelColor = (mandelColor+1)%numColourSchemes;
		juliaColor = (juliaColor+1)%numColourSchemes;
		glutPostRedisplay();
	}
}
#endif

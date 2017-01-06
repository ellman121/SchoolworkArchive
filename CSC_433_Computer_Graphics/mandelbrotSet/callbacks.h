/*
 * callbacks.h
 *
 * This file has the functions used as registered callbacks for GLUT
 */

#ifndef __CALLBACKS_H
#define __CALLBACKS_H

#define GLUT_WHEEL_UP 3
#define GLUT_WHEEL_DOWN 4
extern bool cudaFlag;
extern int zoomLimit, zoomCount;
int wheelZoom = 0;

/*******************************************************************************
 * Function: reshapeCallback
 *
 * Authors: Elliott Rarden & Katie Macmillan
 *
 * Description:
 * GLUT callback to reshape the world and sets scaling factors. The height and
 * width are intentionally set to the starting  defaults on an attempted resize
 * in order to avoid scaling  and distortion of the image, while keeping it a
 * reasonable viewing size.
 *
 * Parameters:
 *		[in] w - The new width, which is ignored
 * 		[in] h - The new height, which is ignored
 ******************************************************************************/
void reshapeCallback( int w, int h ) {
	glutReshapeWindow( width, height);

	glMatrixMode( GL_PROJECTION );			// Use an orthographic projection
	glLoadIdentity();						// Initialize transformation matrix
	gluOrtho2D( -1, 1, -1, 1 );				// Window coordinate domain and range
	glViewport( 0, 0, width, height );		// Keep the viewport size at 700 x 700
}

/*******************************************************************************
 * Function: displayCallback
 *
 * Authors: Elliott Rarden & Katie Macmillan
 *
 * Description: 
 * Display callback for GLUT which will first check the info flag to determine
 * if the info screen should be drawn. If the info flag is not set, then the
 * displayCallback() function calculates and draws the mandelbrot set. It then
 * checks to see if the julia flag is set. If so, the julia set is calculated
 * and drawn in th upper left octant of the viewport.
 ******************************************************************************/
void displayCallback() {
	// Clear the display
	glClear( GL_COLOR_BUFFER_BIT );

	// Draw only info screen if info flag is set
	if (infoFlag){
		drawInfoScreen();
	} else {
		// Select parallel or sequential calculation and draw
		if (cudaFlag){
			drawMandelbrotCuda();
		} else {
			// Calculate and draw mandelbrot set
			drawMandelbrot();
		}
		// Calculate and draw julia set if julia flag is set
		if (juliaFlag){
			if (cudaFlag){
				drawJuliaCuda();
			} else {
				drawJulia();
			}
		}
	}

	// Swap buffers and flush display
	glutSwapBuffers();
	glFlush();
}

/*******************************************************************************
 * Function: mouseCallback
 *
 * Authors: Elliott Rarden & Katie Macmillan
 *
 * Description: 
 * mouseCallback() is the GLUT callback for mouse button activity. If the info
 * flag is set the mouse controls are disabled.
 *
 * mouseCallback() only reacts to left button or mouse wheel activity. Left
 * button activity can be either a simple click or a click, drag, and release
 * action.
 *
 * On a simple click the new point is made the new center point of the viewport.
 * On a click and drag action the image is moved the distance and in the
 * direction of the click and drag action.
 *
 * On mouse wheel activity the image will zoom in or out of the image. Since 
 * glut registers two activities for each click of the wheel mouse, the zoom
 * functions are halved, and a wheel activity counter is kept. For every two
 * zoom functions called by the wheel mouse action, the zoomCount variable is
 * decremented by 1 (for zoomIn) or incremented by 1 (for zoomOut) to
 * compensate for the double incrementation or decrementation by the mouse
 * wheel.
 *
 * Parameters:
 * 		[in] button - The button which is active
 *		[in]  state	- The current state (up or down) of the button
 *		[in] 	  x - The x location of the mouse action
 * 		[in] 	  y - The y location of the mouse action
 ******************************************************************************/
void mouseCallback (int button, int state, int x, int y){
	// Variables to hold initial click cooridnates on mouse down
	static int recentClickX, recentClickY;

	// Check that the mouse is within the window boundaries
	if (x > width or y > height)
	{
		// If it's not, just ignore it
		return;
	}

	// Mouse controls are off when julia set and info screen are being viewed
	if (!infoFlag){
		if (button == GLUT_LEFT_BUTTON){
			// Store x and y position when left button clicked down
			if (state == GLUT_DOWN) {
				// Make sure mouse is not disabled
				mouseFlag = false;
				recentClickX = x;
				recentClickY = y;
			}
			// If left click and drag, calculate delta (x,y) and move image
			else if (!mouseFlag && state == GLUT_UP && x != recentClickX && y != recentClickY){
				panClick(x - recentClickX, y - recentClickY);
			}
			// If just a plain left click, make clicked point new center
			else if (!mouseFlag && state == GLUT_UP && x == recentClickX && y == recentClickY) {
				recenter(x, y);
			}
		} else if (button == GLUT_WHEEL_UP){
			// Zoom in half the distance of button press
			if((wheelZoom%2 == 0) && (zoomCount < zoomLimit)){
				recenter(x,y);
				zoomIn();

			}
			// Wheels do double movement so track it
			wheelZoom++;
		} else if (button == GLUT_WHEEL_DOWN){
			if((wheelZoom%2 == 0) && (zoomCount > -6 )){
				recenter(x,y);
				zoomOut();

			}
			wheelZoom--;
		}
	}
	// Redraw image
	glutPostRedisplay();
}

/*******************************************************************************
 * Function: mouseCallback
 *
 * Authors: Elliott Rarden & Katie Macmillan
 *
 * Description: 
 * motionCallback() is the GLUT callback for passive mouse motion. 
 *
 * The function first checks to see if the julia flag is set. If so, the 
 * current position of the mouse is translated to model coordinates and stored
 * as the current julia coordinate, which is used to calculate the julia set.
 *
 * Parameters:
 *		[in] 	  x - The x location of the mouse
 * 		[in] 	  y - The y location of the mouse
 ******************************************************************************/
void motionCallback(int x, int y){
	if (juliaFlag){
		// Translate to window coordinates
		y = height - y;
		// Store current mouse coordinate as julia set coordinates
		xJul = xMid+ (((2.0*x)/width - 1.0)/scale);
		yJul = yMid+(((2.0*y)/height - 1.0)/scale);
		
		glutPostRedisplay();		
	}
}

/*******************************************************************************
 * Function: keyboardCallback
 *
 * Authors: Elliott Rarden & Katie Macmillan
 *
 * Description:
 * GLUT callback for standard keybaord activity.
 *
 * keyboardCallback() first checks to see if the info flag is set. If so, only
 * the default commands are valid which will toggle the info screen, quit the
 * program, or switch between sequential and parallel computing.
 *
 * If the info flag is not set, then the image movement and color functions
 * become available allowing the user to zoom in and out of the image, pan,
 * switch color schemes and reset the image back to the default position.
 *
 * Parameters:
 * 		[in] key - The character (in ASCII) for what key was pressed
 *		[in]   x - The x location of the mouse when the key was pressed
 * 		[in]   y - The y location of the mouse when the key was pressed
 ******************************************************************************/
void keyboardCallback (unsigned char key, int x, int y){
	// Check that the mouse is within the window boundaries
	if (x > width or y > height)
	{
		// If it's not, just ignore it
		return;
	}

	// Only allow default commands if info flag is set
	if (!infoFlag) {
		switch(key) {
			// Zoom commands
			case '+':
				if(zoomCount < zoomLimit) {
					// Recenter to mouse position and zoom
					recenter(x, y);
					zoomIn();
				}
			break;

			case '-':
				if(zoomCount > -6) {
					// Recenter to mouse position and zoom
					recenter(x, y);
					zoomOut();
				}
			break;

			// Reset image to default
			case 'R':
			case 'r':
			case '0':
				reset();
			break;

			// Manually rotate through color book
			case 'C':
			case 'c':
				// Ensure rotate flag is not set
				rotateFlag = false;
				// Move color indexes forward
				mandelColor = (mandelColor+1)%numColourSchemes;
				juliaColor = (juliaColor+1)%numColourSchemes;
			break;
		
			// Automatically rotate through color book
			case 'A':
			case 'a':
				// Set rotate flag
				rotateFlag = !rotateFlag;
				// Register idle function
				glutIdleFunc(rotateColor);
			break;

			// Toggle julia set view
			case 'J':
			case 'j':
				juliaFlag = !juliaFlag;
			break;

			default:
			break;
		}
	}
	// Default commands
	switch(key) {
		// Toggle info screen
		case 'I':
		case 'i':
			infoFlag = !infoFlag;
		break;

		// Toggle Paralel Computing
		case 'P':
		case 'p':
			cudaFlag = true;
			zoomLimit = 22;
		break;
		
		// Toggle Sequential Computing
		case 'S':
		case 's':
			cudaFlag = false;
			zoomLimit = 16;
		break;

		// Exit program
		case 'Q':
		case 'q':
		case 27:
			exit(0);
		break;

		default:
		break;
	}

	// Ask GLUT to redraw our window once it has time
	glutPostRedisplay();
}

/*******************************************************************************
 * Function: keyboardCallback
 *
 * Authors: Elliott Rarden & Katie Macmillan
 *
 * Description:
 * GLUT callback for special keybaord activity.
 *
 * specialKeyCallback() first checks to see if the julia flag is set. If so,
 * no special keys are accepted as input. If the julia flag is not set then
 * only arrow keys are accepted as commands.
 *
 * Each arrow key will cause the image to pan in the direction which
 * corresponds with the selected arrow.
 *
 * Parameters:
 * 		[in] key - The key that was pressed
 *		[in]   x - The x location of the mouse when the key was pressed
 * 		[in]   y - The y location of the mouse when the key was pressed
 ******************************************************************************/
void specialKeyCallback (int key, int x, int y){
	// Only allow commands if julia flag is not set
	if(!infoFlag){
		// Pan commands executed on arrow key presses
		switch(key) {
			case GLUT_KEY_UP:
				panButton('u');
			break;

			case GLUT_KEY_DOWN:
				panButton('d');
			break;

			case GLUT_KEY_LEFT:
				panButton('l');
			break;

			case GLUT_KEY_RIGHT:
				panButton('r');
			break;

			default:
			break;
		}
	}
	// Ask GLUT to redraw our window once it has time
	glutPostRedisplay();		
}

#endif

#include <GL/glut.h>	// GLUT header includes gl.h and glu.h
#include <GL/freeglut.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <unistd.h>

// Our headers
#include "colorArrays.h"
#include "mandelcuda.h"
#include "view.h"
#include "draw.h"
#include "menus.h"
#include "callbacks.h"

using namespace std;

/* FUNCTION PROTOTYPES */

// Init functions
void glutSetup();
void reset();

/* GLOBAL VARABLES */
// Window stuff
int width, height;
float xMid, yMid, xJul, yJul, scale, interval, range;
bool juliaFlag, infoFlag, colorFlag, mouseFlag, rotateFlag, cudaFlag;

// Utility Variables and Flags
int zoomCount = 0;
int mandelColor = 0, juliaColor = 1;
int ct = 0;
int zoomLimit;

/*
 * Function: Main
 *
 * Authors: Elliott Rarden & Katie Macmillan
 *
 * Description: Main function will initialize GLUT and then drop into the main loop
 *
 * Parameters:
 *		[in] argc - number of command line arguments
 * 		[in] argv - Array of strings for command line args
 *

 * Returns: [integer] status of the program on exit
 */
int main( int argc, char *argv[] ){
	if ((argc > 1) && (argv[1][1] == 's')){
		cudaFlag = false;
		zoomLimit = 16;
	} else {
		cudaFlag = true;
		zoomLimit = 22;
	}
			
	// width and height of the window
	width = height = 700;

	glutInit(&argc, argv);
	glutSetup();
	setupMenus();

	// go into GL main loop... FOREVER!!!
	glutMainLoop();

	// "STOP Whining!"
	return 0;
}

/*
 * Function: glutSetup
 *
 * Authors: Elliott Rarden & Katie Macmillan
 *
 * Description: Setup the GLUT parameters, window size, and callbacks
 */
void glutSetup( void ){
	reset();
	infoFlag = true;
	/**base window setup**/
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE );
	glutInitWindowSize( width, height );
	glutInitWindowPosition( 200, 0 );
	glutCreateWindow( "Fractals Are Beautiful" );
	glClearColor( 0.0, 0.0, 0.0, 1.0 );

	/**callbacks**/
	glutDisplayFunc( displayCallback );
	glutReshapeFunc( reshapeCallback );
	glutMouseFunc(mouseCallback);
	glutKeyboardFunc( keyboardCallback );
	glutPassiveMotionFunc(motionCallback);
	glutSpecialFunc(specialKeyCallback);

}

/*
 * Function: reset
 *
 * Authors: Elliott Rarden & Katie Macmillan
 *
 * Description: Sets all points, flags and view dimensions to
 *				the default settings, bringing the image back
 *				to the starting position
 */
void reset(){
	xMid = 0;
	yMid = 0;
	xJul = 0;
	yJul = 0;
	range = 2.0;
	scale = 0.5;
	interval = 0.00200;

	juliaFlag = false;
	zoomCount = 0;
}

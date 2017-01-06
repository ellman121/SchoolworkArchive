/*
* orbit.cpp
*
* Program to simulate the solar system and allow eht user to fly around, viewing
* individual planets.  
*
* Authors: Elliott Rarden && Katie MacMillan
*
* This code is based on the code provided by Dr. Weiss at 
* www.mds.sdsmt.edu/csc344/Assignments/PA3
*
* Compile and run using 
*
*   $ make
*   $ ./solar
*
* If on an Apple Mac Computer, use
*
*   $ make osx
*   $ ./solar
*
* Notes:
*	  	If program is closed from comandline there will be a mem leak from 
*		dynamically allocated bmp images
*
*	Moons - 
*		If a value could not be found to define a moon's day length, the default
*		was set to that of Luna, the Earth's moon.
*
*		For Jupiters moons, we used only the Inner and Gallilean moons, as many
* 		of the other moons had very small diameters < 10 km and were fairly 
*		inconcequential.
*
*		Saturn has a total of 62 satellite bodies, however, 7 of them comprise 
*		99.96% of the mass which orbits Saturn. For this reason we used only the 
*		primary 7 moons in this model.
*
*		For Uranus we used only the five major moons. These moons are quite dark
*		with very low reflectivity coefficients.
*
*		Neptune has 14 moons. Triton is the largest making up 99.59% of the mass
*		orbiting Neptune. The next two largest moons are __ and __ which together
*		comprise 0.36% of the orbiting mass. The remaining 11 moons  make up the
*		final 0.05% of the mass. For this reason we modeled only Triton.
*
*/

// C Libraries
#include <cstdlib>
#include <cmath>
#include <ctime>

// C++ Libraries
#include <iostream>
#include <algorithm>
#include <string>
#include <map>

// Steve Jobs made me
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

// Our headers
// #include "globals.h"
#include "planet.h"
#include "rings.h"
#include "imageReader.h"
#include "initSystem.h"
#include "setFuncs.h"
#include "menu.h"
#include "drawInfo.h"
#include "drawObjects.h"
#include "callbacks.h"

using namespace std;

// function prototypes
void OpenGLInit( void );
void ResizeWindow( int w, int h );
void initLighting();
void initMenus();

// Global things
bool infoFlag = true;
bool pauseFlag = false;
bool lightFlag = true;
bool solidFlag = false;
bool smoothFlag = false;
bool texFlag = false;
bool bodyLabelFlag = true;
bool moonLabelFlag = false;
bool velocityFlag = false;

int width = 1200;
int height = 720;

float hourSpeed = 0.1;
float xTranslate = 0.0;
float yTranslate = 0.0;
float zTranslate = -200;
float xVelocity = 0;
float yVelocity = 0;
float zVelocity = 0;
float xRotate = 0.0;
float yRotate = 0.0;
float mouseX, mouseY;

/* initMenus()
 *
 * Initialize the menus for our program
 *
 * Parameters:
 *
 * Returns:
 *
 */
void initMenus() {
	int primaryMenu;
	int speedSelectSubmenu;
	int planetSelectSubmenu;

	// Create the speed selection submenu
	speedSelectSubmenu = glutCreateMenu(setSpeedSelectSubmenuOption);
	glutAddMenuEntry("Minimum Speed", 0);
	glutAddMenuEntry("1 Hour/Frame", 1);
	glutAddMenuEntry("12 Hours/Frame", 2);
	glutAddMenuEntry("1 Day/Frame", 3);
	glutAddMenuEntry("2 Days/Frame", 4);

	// Create planet selection submenu
	planetSelectSubmenu = glutCreateMenu(setPlanetSelectSubmenuOption);
	glutAddMenuEntry("Sun", 0);
	glutAddMenuEntry("Mercury", 1);
	glutAddMenuEntry("Venus", 2);
	glutAddMenuEntry("Earth", 3);
	glutAddMenuEntry("Mars", 4);
	glutAddMenuEntry("Jupiter", 5);
	glutAddMenuEntry("Saturn", 6);
	glutAddMenuEntry("Uranus", 7);
	glutAddMenuEntry("Neptune", 8);

	// Create our primary menu
	primaryMenu = glutCreateMenu(setMenuOption);
	glutAddSubMenu("Select Speed", speedSelectSubmenu);
	glutAddSubMenu("Planet Focus", planetSelectSubmenu);
	glutAddMenuEntry("Toggle Lights", 0);
	glutAddMenuEntry("Toggle Planet Labels", 1);
	glutAddMenuEntry("Toggle Moon Labels", 2);
	glutAddMenuEntry("Reset Position", 3);
	glutAddMenuEntry("Show Info Screen", 4);
	glutAddMenuEntry("Exit", 5);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}


/* initLighting()
 *
 * Initialize OpenGL's lighting features for our solar system
 *
 * Parameters:
 *
 * Returns:
 *
 */
void initLighting() {
	// Specify material properties for info screen
    GLfloat mat_ambient[] = { 0.6, 0.6, 1.0, 0.10 };
    GLfloat mat_diffuse[] = { 0.6, 0.6, 1.0, 1.0 };
    GLfloat mat_specular[] = { 0.6, 0.6, 1.0, 1.0 };
    GLfloat mat_shininess = { 100.0 };
    
    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient );
    glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse );
    glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular );
    glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess );
    
    // Specify light properties
    GLfloat light_position[] = { 0.0, 400.0, 0.0, 1.0 };
    GLfloat light_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

    // Enable a light source
    glEnable( GL_LIGHT0 );
    glLightfv( GL_LIGHT0, GL_POSITION, light_position );
    glLightfv( GL_LIGHT0, GL_AMBIENT, light_ambient );
    glLightfv( GL_LIGHT0, GL_DIFFUSE, light_diffuse );
    glLightfv( GL_LIGHT0, GL_SPECULAR, light_specular );

    // Set shading model
    glShadeModel( GL_FLAT );

    glEnable( GL_DEPTH_TEST );  // enable depth buffer for hidden-surface elimination
    glEnable( GL_NORMALIZE );   // automatic normalization of normals
    // Hide back of objects
    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );

}

/* drawLightSource()
 *
 * Initalize OpenGL
 *
 * Parameters:
 *
 * Returns:
 *
 */
void OpenGLInit( void )
{
	glShadeModel( GL_FLAT );
	glClearColor( 0.0, 0.0, 0.0, 0.0 );
	glClearDepth( 1.0 );
	glEnable( GL_DEPTH_TEST );
}


/* ResizeWindow()
 *
 * Funciton that is called when the user resizes the window for GLUT
 *
 * Parameters:
 *		int w - The new width of the window
 *		int h - The new height of the window
 * Returns:
 *
 */
void ResizeWindow( int w, int h )
{
	float aspectRatio;
	height = ( h == 0 ) ? 1 : h;
	width = ( w == 0 ) ? 1 : w;
	glViewport( 0, 0, w, h );	// View port uses whole window
	aspectRatio = ( float ) width / ( float ) height;

	// Set up the projection view matrix (not very well!)
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( 60.0, aspectRatio, 1.0, 1000000.0 );

	// Select the Modelview matrix
	glMatrixMode( GL_MODELVIEW );
}

// Main routine
// Set up OpenGL, hook up callbacks, and start the main loop

/* main()
 *
 * Main will initailze OpenGL, GLUT, the Solar System, and drop into GlutMainLoop();
 *
 * Parameters:
 *		int argc 	- Argument count
 *		char** argv - Argument values
 * Returns:
 *
 */
int main( int argc, char** argv )
{
	// Need to double buffer for animation
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );

	// Create and position the graphics window
	glutInitWindowPosition( 0, 0 );
	glutInitWindowSize( width, height );
	glutCreateWindow( "Solar System Demo" );

	// Initialize OpenGL.
	OpenGLInit();
	initMenus();
	setPlanets();
	setMoons();
	setRings();
	setTexImage();
	// setAstroidBelt();
	// Set up the callback function for resizing windows
	glutReshapeFunc( ResizeWindow );

    // Set up lighting
    initLighting();
    glEnable(GL_LIGHTING);

    // Set initial wireframe draw mode
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    glShadeModel( GL_FLAT );


	// Callback for graphics image redrawing
	glutDisplayFunc( displayCallback );
	glutKeyboardFunc(keyboardCallback);
	glutMouseFunc(mouseCallback);
	glutMotionFunc(NULL);
	// glutPassiveMotionFunc(passiveMouseCallback);
	glutSpecialFunc(specialKeyCallback);
	// Start the main loop.  glutMainLoop never returns.
	glutMainLoop( );

	return 0;
}

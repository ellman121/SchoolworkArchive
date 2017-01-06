// #include <GL/glut.h>	// GLUT header includes gl.h and glu.h
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "tank.h"
#include "terrain.h"
#include "drawRoutines.h"
using namespace std;

/* FUNCTION PROTOTYPES */

// Init functions
void glutSetup();

// GLUT functions
void reshape(int w, int h );

// GLUT Callbacks
void drawDisplayCallback();
void keyboardCallback(unsigned char key, int x, int y);

// Utility Functions
void moveTank(int direction);
bool checkGas();
bool wasHit(int x, int y);
bool checkClimb(int d);
void newGame();
void endTurn();

/* GLOBAL VARABLES */
// Window stuff
int width, height;

// Players
tank players[2];
int pCurr = 0;
int playerGas[2] = {100, 100};
char pNames[2][20] = {"Sub-Zero", "Terminator"};

// Utility Variables and Flags
bool fired = false; // A player fired a shot
bool infoScreen = true; // The info screen is being shown
char screenMsg[80] = ""; // A place to store a message to display in the message box
bool gameOver; // If the game is over or not

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
	// width and height of the window
	width = height = 900;

	glutInit(&argc, argv);
	glutSetup();

	// We are in a new game state
	newGame();

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

	glutInitDisplayMode( GLUT_RGBA | GLUT_SINGLE );		// set color and buffer mode
	glutInitWindowSize( width, height );				// set initial window size
	glutInitWindowPosition( 100, 50 );					// set initial window position
	glutCreateWindow( "TANK WARS!!!" );   				// create window with title
	glClearColor( 0.0, 0.0, 0.0, 1.0 );					// use black for clear

	/**callbacks**/
	glutDisplayFunc( drawDisplayCallback );		// how to display & redisplay
	glutReshapeFunc( reshape );					// how to resize window
	glutKeyboardFunc( keyboardCallback );		//what to do with keyboard input
}

/*
 * Function: newGame
 *
 * Authors: Elliott Rarden & Katie Macmillan
 *
 * Description: Resets the game so a new one can begin
 */
void newGame(){
	// The game isn't over when it starts
	gameOver = false;
	clearTerrain();
	pCurr = 0;
	// Create player 1
	tank_color p1_color;
	p1_color.red = 0;
	p1_color.grn = 0;
	p1_color.blu = 255;
	players[0] = tank(p1_color, xMin + 50);
	players[0].setOrientation(1);
	playerGas[0] = players[0].getGasPercentage();
	sprintf(pNames[0], "Sub-Zero");

	// Create player 2
	tank_color p2_color;
	p2_color.red = 255;
	p2_color.grn = 0;
	p2_color.blu = 0;
	players[1] = tank(p2_color, xMax - 50);
	playerGas[1] = players[1].getGasPercentage();
	sprintf(pNames[1], "Terminator");
	// Generate the terrain
	generateTerrain();

	// Display start game message
	sprintf(screenMsg, "It's Show Time!");
}

/*
 * Function: inTerrain
 *
 * Authors: Elliott Rarden & Katie Macmillan
 *
 * Description: Checks if the given point is inside the enemy tank and returns
 *		true if it is
 *
 * Parameters:
 *		[in] x - The current x location of the shot (world coordinates)
 * 		[in] y - The current y location of the shot (world coordinates)
 *
 * Returns: [boolean] true/false if the non-current player's tank was hit
 */
bool wasHit(int x, int y) {
	tank enemy_tnk = players[(pCurr + 1) % 2];

	// Bounds for tank to be hit
	int x_body_min = enemy_tnk.getPosition() - 10; // Tank is 20 units long, position returns the midpoint
	int x_body_max = x_body_min + 20;
	int y_body_min = terrainY[x_body_min];
	int y_body_max = y_body_min + 10; // Body is 10 units tall

	// The "lid" of the tank is the part that rotates on top
	int x_lid_min = x_body_min + 5;
	int x_lid_max = x_body_min + 15;
	int y_lid_max = y_body_min + 15;
	int y_lid_min = y_body_min + 10;

	// Check a lid hit
	if(y < y_lid_min && y > y_lid_max) {
		if(x > x_lid_min && x < x_lid_max) {
			return true;
		}
	}

	// Check a body hit
	if(y > y_body_min && y < y_body_max) {
		if(x > x_body_min && x < x_body_max) {
			return true;
		}
	}

	return false;
}

/*
 * Function: reshape
 *
 * Authors: Elliott Rarden & Katie Macmillan
 *
 * Description: GLUT callback to reshape the world and sets scaling factors
 *
 * Parameters:
 *		[in] w - The new width
 * 		[in] h - The new height
 */
void reshape( int w, int h ) {
	// store new window dimensions globally
	width = w;
	height = h;
	// how to project 3-D scene onto 2-D
	glMatrixMode( GL_PROJECTION );		// use an orthographic projection
	glLoadIdentity();					// initialize transformation matrix
	gluOrtho2D( 0, terrainScale-1, 0, terrainScale-1 );		// window coords: (0,0) to (100,100)
	glViewport( 0, 0, w, h );			// adjust viewport to actual display window dimensions
}

/*
 * Function: drawDisplayCallback
 *
 * Authors: Elliott Rarden & Katie Macmillan
 *
 * Description: Display callback for GLUT
 */
void drawDisplayCallback() {
	bool stalemate = false;

	// Clear the display
	glClear( GL_COLOR_BUFFER_BIT );

	// Draw a white line at the base of the window
	glColor3fv(White);
	glLineWidth(1);
	glBegin(GL_LINES);
		glVertex2i(xMin, yMin-1);
		glVertex2i(xMax, yMin-1);
	glEnd();

	// Draw terrain
	drawTerrain();

	// Draw our tanks
	drawTank(players[0]);
	drawTank(players[1]);

	// If a player fired, draw the parabola for their shot
	if(fired){
		drawParabola(players[pCurr]);
		endTurn();
	} else {
		// IF we're in easy mode, draw 1/2 the parabola for shot estimating
		drawParabola(players[pCurr]);
	}

	drawInfoPanel();

	// If a player is dead, draw some fire on their tank
	for (int i = 0; i < 2; ++i) {
		if(!players[i].isStillAlive()) {
			drawExplosion(players[pCurr].getPosition(), terrainY[players[pCurr].getPosition()]);
		}
	}

	// Call satlemates
	if(!players[0].getAmmo() && !players[1].getAmmo()) {
		stalemate = true;
	}

	if(stalemate) {
		drawText("I'm the Party Pooper (stalemate)", 300, 150, White, 0.22, 0.22);
	} else {
		if(screenMsg[0] != '\0'){
			drawText(screenMsg, 300, 150, Green, 0.22, 0.22);
			sprintf(screenMsg, " ");
		}
	}

	// Flush the display
	glFlush();
}

/*
 * Function: endTurn
 *
 * Authors: Elliott Rarden & Katie Macmillan
 *
 * Description: Does all the stuff we need to do when a turn is over
 */
void endTurn(){
	fired = false;
	players[pCurr].useAmmo();
	pCurr = (pCurr + 1) % 2;
	playerGas[pCurr] = players[pCurr].getGasPercentage();
}

/*
 * Function: keyboardCallback
 *
 * Authors: Elliott Rarden & Katie Macmillan
 *
 * Description: GLUT callback for keybaord activity
 *
 * Parameters:
 * 		[in] key - the character (in ASCII) for what key was pressed
 *		[in] x - The current x location of the shot (world coordinates)
 * 		[in] y - The current y location of the shot (world coordinates)
 *
 * Returns: [boolean] true/false if the non-current player's tank was hit
 */
void keyboardCallback (unsigned char key, int x, int y){
	if(!gameOver){
		switch(key) {
			case 'z':
			case 'Z':
				players[pCurr].setOrientation(players[pCurr].getOrientation() +1);
			break;

			case 'a':
			case 'A':
				if(checkGas()) {
					if(	checkClimb(-1)) {
						moveTank(-1); // Move tank left 1 pixel
					} else {
						sprintf(screenMsg, "STICK AROUND!");
					}
				} else {
					sprintf(screenMsg,"Do it!  Do it now!");
				}
			break;

			case 'd':
			case 'D':
				if(checkGas()) {
					if(	checkClimb(1)) {
						moveTank(1); // Move tank right 1 pixel
					} else {
						sprintf(screenMsg, "STICK AROUND!");
					}
				} else {
					sprintf(screenMsg,"Do it!  Do it now!");
				}
			break;

			case 'q':
			case 'Q':
				if(players[pCurr].getPower() > __TANK_MIN_POWER)
					players[pCurr].setPower(players[pCurr].getPower() - 1); // Decrement power
			break;

			case 'e':
			case 'E':
				if(players[pCurr].getPower() < __TANK_MAX_POWER)
					players[pCurr].setPower(players[pCurr].getPower() + 1); // Increment power
			break;

			case 'w':
			case 'W':
				if(players[pCurr].getAngle() < __TANK_MAX_ANGLE)
					players[pCurr].setAngle(players[pCurr].getAngle() + 1); // Increment angle
			break;

			case 's':
			case 'S':
				if(players[pCurr].getAngle() > __TANK_MIN_ANGLE)
					players[pCurr].setAngle(players[pCurr].getAngle() - 1); // Decrement angle
			break;

			case ' ':
				fired = true;
			break;
		}
	}
	switch(key) {
		case 'n':
		case 'N':
			newGame();
		break;

		case 27: // Escape key quits
			cout << "I'll be back" << endl;
			exit(0);
		break;

		default:
		break;
	}

	// Ask GLUT to redraw our window once it has time
	glutPostRedisplay();
}

/*
 * Function: moveTank
 *
 * Authors: Elliott Rarden & Katie Macmillan
 *
 * Description: Moves the current player's tank in the given direction
 *
 * Parameters:
 *		[in] direciton - + means right, - means left.
 */
void moveTank(int direction){
	// Find the new x
	int newX = players[pCurr].getPosition() + (5 * direction);

	// If the new x is out of bounds, don't move them
	if (newX > xMax)
	{
		players[pCurr].setPosition(xMax);
	}
	else if (newX < xMin)
	{
		players[pCurr].setPosition(xMin);
	}
	else
	{
		players[pCurr].setPosition(newX);
	}
	playerGas[pCurr] = playerGas[pCurr] - 5;
}

/*
 * Function: checkClimb
 *
 * Authors: Elliott Rarden & Katie Macmillan
 *
 * Description: Checks if the given point is inside the enemy tank and returns
 *		true if it is
 *
 * Parameters:
 *		[in] d - The direction (+ is right, - is left)
 *
 * Returns: [boolean] true if the tank can go in that direction
 */
bool checkClimb(int d){
	int x = players[pCurr].getPosition();
	int y1 = terrainY[ x ];
	int y2 = terrainY[ x + (5 * d) ];
	int deltaY = abs(y2 - y1);
	if (deltaY > climbMax){
		return false;
	}
	return true;
}

/*
 * Function: checkGas
 *
 * Authors: Elliott Rarden & Katie Macmillan
 *
 * Description: Checks the current player's gas and returns if they still
 *		have any in the tank
 *
 * Returns: [boolean] true/false if the player has enough gas to move
 */
bool checkGas(){
	if (playerGas[pCurr] == 0){
		return false;
	}
	return true;
}

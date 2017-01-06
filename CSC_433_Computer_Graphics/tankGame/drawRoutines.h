#include "tank.h"

#define GRAVITY_CONST 3

// Stuff defined in other files
extern tank players[2];
extern int pCurr;
extern int playerGas[2];
extern char pNames[2][20];
extern bool fired;
extern bool infoScreen;
extern char screenMsg[80];
extern bool gameOver;

extern bool wasHit(int x, int y);

// Drawing function prototypes
void drawParabola(tank tnk);
void drawGas();
void drawTank(tank p);
void drawPower();
void drawTerrain();
void drawExplosion(int x, int y);
void drawInfoPanel();
void drawText(const char *string, float x, float y, const float color[], float scale1, float scale2);

const char *keys[10] = {"A - Move Left", "Q - Power Down", "S - Gun Down", \
"SPACE - Fire Weapon", "N - New Game", "D - Move Right", "E - Power Up", \
"W - Gun Up", "Z - Switch Orientation", "ESC - Quit"}; // Strings to display in info box at bottom

// Color vectors
const float White[3] = {1.0, 1.0, 1.0};
const float Red[3] = {1.0, 0.0, 0.0};
const float Green[3] = {0.0, 1.0, 0.0};
const float Blue[3] = {0.0, 0.0, 1.0};
const float Teal[3] = {0.0, 1.0, 1.0};
const float Purple[3] = {0.7, 0.2, 1.0};
const float Yellow[3] = {1.0, 0.8, 0.0};

// Some math constants
const double DEG_TO_RAD = 3.14159265/180;

/*
 * Function: inTerrain
 *
 * Authors: Elliott Rarden & Katie Macmillan
 *
 * Description: Draw an explosion centered on the given point
 *
 * Parameters:
 *		[in] x - The x point of the explosion (in world coordinates)
 * 		[in] y - The y point of the explosion (in world coordinates)
 */
void drawExplosion(int x, int y) {
	int x_start = x - 10;
	int y_start = y;

	// Draw a bright orange outline
	GLfloat bright_orange[3] = {1.0, 0.2, 0.2};
	glColor3fv(bright_orange);
	glLineWidth(0.5);

	glBegin(GL_LINE_LOOP);
		glVertex2i(x_start, y_start);
		glVertex2i(x_start - 5, y_start + 15);
		glVertex2i(x_start, y_start + 10);
		glVertex2i(x_start - 5, y_start + 25);
		glVertex2i(x_start + 5, y_start + 15);
		glVertex2i(x_start + 10, y_start + 25);
		glVertex2i(x_start + 15, y_start + 15);
		glVertex2i(x_start + 25, y_start + 25);
		glVertex2i(x_start + 20, y_start + 10);
		glVertex2i(x_start + 25, y_start + 15);
		glVertex2i(x_start + 20, y_start);
	glEnd();

	// Draw a yellow line inside for flare
	GLfloat fire_yellow[3] = {1.0, 1.0, 0.2};
	glColor3fv(fire_yellow);
	glLineWidth(0.5);

	glBegin(GL_LINE_LOOP);
		glVertex2i(x_start, y_start);
		glVertex2i(x_start - 3, y_start + 13);
		glVertex2i(x_start, y_start + 10);
		glVertex2i(x_start - 3, y_start + 23);
		glVertex2i(x_start + 3, y_start + 13);
		glVertex2i(x_start + 10, y_start + 23);
		glVertex2i(x_start + 13, y_start + 13);
		glVertex2i(x_start + 23, y_start + 23);
		glVertex2i(x_start + 20, y_start + 10);
		glVertex2i(x_start + 23, y_start + 13);
		glVertex2i(x_start + 20, y_start);
	glEnd();
}

/*
 * Function: inTerrain
 *
 * Authors: Elliott Rarden & Katie Macmillan
 *
 * Description: Draws the gas meters for both players in the info panel
 */
void drawGas(){

	// Set colour to yellow to draw meter fills
	glColor3fv( Yellow);
	glLineWidth(1);
	glBegin( GL_POLYGON);
		glVertex2i(30, 15);
		glVertex2i(30, 15+playerGas[0]);
		glVertex2i(80, 15+playerGas[0]);
		glVertex2i(80, 15);
	glEnd();
	glBegin( GL_POLYGON);
		glVertex2i(970, 15);
		glVertex2i(970, 15+playerGas[1]);
		glVertex2i(920, 15+playerGas[1]);
		glVertex2i(920, 15);
	glEnd();

	// Set color to white to draw outlines of meters
	glColor3fv( White );
	glLineWidth(2);
	glBegin( GL_LINE_LOOP);
		glVertex2i(30, 15);
		glVertex2i(30, 115);
		glVertex2i(80, 115);
		glVertex2i(80, 15);
	glEnd();
	glBegin( GL_LINE_LOOP);
		glVertex2i(970, 15);
		glVertex2i(970, 115);
		glVertex2i(920, 115);
		glVertex2i(920, 15);
	glEnd();
}

/*
 * Function: drawInfoPanel
 *
 * Authors: Elliott Rarden & Katie Macmillan
 *
 * Description: Draws the info panel
 */
void drawInfoPanel(){
	int x = 15, y = 145;
	if (pCurr == 1){
		x = 845;
	}
	GLfloat color[3];
	color[0] = players[pCurr].getColor().red / 255.0;
	color[1] = players[pCurr].getColor().grn / 255.0;
	color[2] = players[pCurr].getColor().blu / 255.0;
	glColor3fv(color);
	glLineWidth(4);
	glBegin( GL_LINES);
		glVertex2i(x, y);
		glVertex2i(x+135, y);
	glEnd();

	drawGas();
	drawPower();

	char name[20];
	sprintf(name, "%s", pNames[0]);
	drawText(name, 20, 150, White, 0.2, 0.2);
	sprintf(name, "%s", pNames[1]);
	drawText(name, 850, 150, White, 0.2, 0.2);


	char msg[80];
	char hits[4];
	for (int i = 0; i < 4; i++){
		if (i < players[0].getHits()){
			hits[i] = '*';
		}
		else {
			hits[i] = '\0';
		}
	}
	sprintf(msg, "Hits: %s", hits);
	drawText(msg, 90, 80, White, 0.15, 0.15);

	for (int i = 0; i < 4; i++){
		if (i < players[1].getHits()){
			hits[i] = '*';
		}
		else {
			hits[i] = '\0';
		}
	}
	sprintf(msg, "Hits: %s", hits);
	drawText(msg, 800, 80, White, 0.15, 0.15);

	sprintf(msg, "Ammo: %d", players[0].getAmmo());
	drawText(msg, 90, 100, White, 0.15, 0.15);
	sprintf(msg, "Ammo: %d", players[1].getAmmo());
	drawText(msg, 800, 100, White, 0.15, 0.15);


	sprintf(msg, "Gun: %d Deg.", players[0].getAngle());
	drawText(msg, 90, 60, White, 0.13, 0.13);
	sprintf(msg, "Gun: %d Deg.", players[1].getAngle());
	drawText(msg, 800, 60, White, 0.13, 0.13);

	drawText("Power", 90, 40, White, 0.15, 0.15);
	drawText("Power", 800, 40, White, 0.15, 0.15);

	float y2 = 100;
	for (int i = 0; i < 5; i++){
		drawText(keys[i], 300, y2, White, 0.13, 0.13);
		drawText(keys[i+5], 550, y2, White, 0.13, 0.13);
		y2 = y2-20;
	}
}

/*
 * Function: drawParabola
 *
 * Authors: Elliott Rarden & Katie Macmillan
 *
 * Description: Draws a parabola given a tank.  The function extracts the power,
 * 		angle, and position of the gun fron the tank, and will draw a parabola
 * 		using a parametric version of Newton's Equations
 *
 * Parameters:
 *		[in] tnk - tank from which we are drawing the parabola
 * 		[in] halfParabola - If true, only draw the first 1/2 of the parabola
 */
void drawParabola(tank tnk){
	// Define variables
	int angle = tnk.getAngle();
	int x;
	int y;
	int x_start = tnk.getPosition();
	x_start += (tnk.getOrientation() == 0 ? -1 : 1) *  tnk.getGunLength() * cos(angle*DEG_TO_RAD);
	int y_start = terrainY[tnk.getPosition()] + 10 + tnk.getGunLength() * sin(angle*DEG_TO_RAD);
	int v_x = tnk.getPower() * (cos(angle*DEG_TO_RAD));
	int v_y = tnk.getPower() * (sin(angle*DEG_TO_RAD));

	// X velocity is positive or negative depending on orientation
	// 0 is left, 1 is right
	if(tnk.getOrientation() == 0)
		v_x *= -1;
	else
		v_x *= 1;

	// Set color for the line
	GLfloat color[3];
	color[0] = tnk.getColor().red / 255.0;
	color[1] = tnk.getColor().grn / 255.0;
	color[2] = tnk.getColor().blu / 255.0;
	glColor3fv(color);
	glLineWidth(1);

	// Draw the parabola as a set of line_strips using
	// Newton's equatoins parametrically
	glBegin(GL_LINE_STRIP);
		float t = 0;
		while(true)
		{
			// Newton's Equations
			x = x_start + v_x * t;
			y = y_start + ((-GRAVITY_CONST) * (t * t / 2)) + (v_y * t);

			// If we are in the terrain or off screen, we can stop drawing
			if(x < 0 || x > terrainScale-1 || y < yMin || inTerrain(x, y))
				break;

			// Place our point
			glVertex2i(x, y);

			// If the point was a hit, register it and stop drawing
			if(wasHit(x, y))
			{
				// Register a hit on the other player
				players[(pCurr + 1) % 2].takeHit();
				if(players[(pCurr + 1) % 2].isStillAlive()) {
					sprintf(screenMsg, "WELL THAT HIT THE SPOT!");
				} else {
					int winner = (pCurr + 1) % 2;
					sprintf(screenMsg, "Consider that a divorce!");

					// Change the loosers name appropriately
					if(winner == 0) {
						sprintf(pNames[0], "Plain-Zero");
					} else {
						sprintf(pNames[1], "Terminated");
					}

					gameOver = true;
				}
				break;
			}

			// Step our parametric equation parameter
			t += 0.01;
		}
	glEnd();
}

/*
 * Function: drawPower
 *
 * Authors: Elliott Rarden & Katie Macmillan
 *
 * Description: Draws power meters for each player in the info panel
 */
void drawPower(){

	// Set colour to yellow to draw meter fills
	glColor3fv( Purple );
	glLineWidth(1);
	glBegin( GL_POLYGON);
		glVertex2i(90, 15);
		glVertex2i(90+players[0].getPower(), 15);
		glVertex2i(90+players[0].getPower(), 30);
		glVertex2i(90, 30);
	glEnd();
	glBegin( GL_POLYGON);
		glVertex2i(800, 15);
		glVertex2i(800+players[1].getPower(), 15);
		glVertex2i(800+players[1].getPower(), 30);
		glVertex2i(800, 30);
	glEnd();

	// Set color to white to draw outlines of meters
	glColor3fv( White );
	glLineWidth(2);
	glBegin( GL_LINE_LOOP);
		glVertex2i(90, 15);
		glVertex2i(90, 30);
		glVertex2i(190, 30);
		glVertex2i(190, 15);
	glEnd();
	glBegin( GL_LINE_LOOP);
		glVertex2i(900, 15);
		glVertex2i(900, 30);
		glVertex2i(800, 30);
		glVertex2i(800, 15);
	glEnd();
}

/*
 * Function: drawTank
 *
 * Authors: Elliott Rarden & Katie Macmillan
 *
 * Description: Draw a given tank on the screen.
 *
 * Parameters:
 *		[in] tnk - The tank to be drawn
 */
void drawTank(tank tnk){
	// Set color
	GLfloat color[3];
	color[0] = tnk.getColor().red / 255;
	color[1] = tnk.getColor().grn / 255;
	color[2] = tnk.getColor().blu / 255;
	glColor3fv(color);
	glLineWidth(1);
	// Set initial position
	int x = tnk.getPosition();
	int y = terrainY[x];

	// Draw the tank
	glBegin( GL_POLYGON );
		glVertex2i(x, y);
		glVertex2i(x+10, y);
		glVertex2i(x+10, y+10);
		glVertex2i(x+5, y+10);
		glVertex2i(x+5, y+15);
		glVertex2i(x-5, y+15);
		glVertex2i(x-5, y+10);
		glVertex2i(x-10, y+10);
		glVertex2i(x-10, y);
	glEnd();

	// Get angle for gun
	int gun_y_offset = 10;
	int angle = tnk.getAngle();
	int len_y = tnk.getGunLength() * sin(angle * DEG_TO_RAD);
	int len_x = tnk.getGunLength() * cos(angle * DEG_TO_RAD);
	len_x *= (tnk.getOrientation() ? 1 : -1); // Set orientation (left/right)

	// Draw tank's gun
	glLineWidth(3.0);
	glBegin(GL_LINE_STRIP);
		glVertex2i(x, y + gun_y_offset);
		glVertex2i(x + len_x, y + gun_y_offset + len_y);
	glEnd();
}

/*
 * Function: drawText
 *
 * Authors: Elliott Rarden & Katie Macmillan
 *
 * Description: Draw the given message at ponit x, y using the color vector vector
 * 		passed in with a given scale
 *
 * Parameters:
 *		[in] msg - The message to be drawn
 *		[in] x - The x location to draw the text
 * 		[in] y - The y location to draw the text
 * 		[in] color - A 3 dimensional vector of floats for the color of the text (in RGB)
 * 		[in] scale1 - 1st scale parameter to be used on glScalef
 * 		[in] scale2 - 2nd scale parameter to be used on glScalef
 */
void drawText(const char *msg, float x, float y, const float color[], float scale1, float scale2){
	glColor3fv( color );
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef( x, y, 0 );
	glScalef(scale1,scale2,1);
	// while ( *string ) glutStrokeCharacter( GLUT_STROKE_ROMAN, *string++ );
	// glutStrokeString( GLUT_STROKE_ROMAN, (const unsigned char *)msg );
	glPopMatrix();
}

/*
 * Function: drawTerrain
 *
 * Authors: Elliott Rarden & Katie Macmillan
 *
 * Description: Draw the terrain on the map
 */
void drawTerrain(){
	// Draw our terrain in teal
	glColor3fv( Teal );
	glLineWidth(1);
	glBegin( GL_LINE_STRIP );
		for(int i = 0; i < terrainScale; i++){
			glVertex2i(i, terrainY[i]);
		}
	glEnd();
}

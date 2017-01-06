/******************************************************************************
 * File: drawInfo.h
 *
 * Authors: Elliott Rarden & Katie MacMillan
 *
 * Description:
 *		Contains draw routines for different parts of the solar system
 ******************************************************************************/
extern void initLighting();


/* drawBodyName()
 *
 * Draw the name of a celestial body near it
 *
 * Parameters:
 *		string sName - The name of the body in question
 *		float radius - The radius of the body in question
 * Returns:
 *
 */
void drawBodyName(string sName, float radius) {
	const char* name = sName.c_str();
	if (lightFlag) {
		// Set material property values to max
		GLfloat labelMat[] = { 1.0, 1.0, 1.0, 1.0 };
		glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, labelMat );
		glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, labelMat );
		glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, labelMat );
	}

	glColor3f(1.0, 1.0, 1.0);

	// Position label above body and draw
	glRasterPos3f(0, radius + (radius * 0.1), 0);
	glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char *)name);
}

/* drawBodyName()
 *
 * Draw the status of the program in the top left corner (i.e. current speed,
 * current camera velocity)
 *
 * Parameters:
 *
 * Returns:
 *
 */
void drawStatus() {

	//Set ortho view
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	// change to ortho view for 2D drawing
	glOrtho(-1, 1, -1, 1, 0.0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// create variables
	char xV [40];
	char yV [40];
	char zV [40];
	char speed[40];
	float color[3] = {1.0, 1.0, 1.0};

	// set speed text
	(pauseFlag) ? sprintf(speed, "Speed:  PAUSED") : sprintf(speed, "Speed:  %f  hours / frame", hourSpeed);

	glColor3fv(color);

	glRasterPos3f(-0.97, 0.9, 0);
	glutBitmapString(GLUT_BITMAP_HELVETICA_10, (const unsigned char*)speed);

	glRasterPos3f(-0.95, 0.85, 0);
	glutBitmapString(GLUT_BITMAP_HELVETICA_10, (const unsigned char*)"Velocity");

	glRasterPos3f(-0.98, 0.83, 0);
	glutBitmapString(GLUT_BITMAP_HELVETICA_10, (const unsigned char*)"<--------->");

	// Print velocities if velocity is engaged
	if (velocityFlag) {
		// set velocity strings
		sprintf(xV, "X:  %d", (int)(-1 * xVelocity));
		sprintf(yV, "Y:  %d", (int)(-1 * yVelocity));
		sprintf(zV, "Z:  %d", (int)(zVelocity));

		// draw strings
		glRasterPos3f(-0.95, 0.8, 0);
		glutBitmapString(GLUT_BITMAP_HELVETICA_10, (const unsigned char*)xV);

		glRasterPos3f(-0.95, 0.75, 0);
		glutBitmapString(GLUT_BITMAP_HELVETICA_10, (const unsigned char*)yV);

		glRasterPos3f(-0.95, 0.7, 0);
		glutBitmapString(GLUT_BITMAP_HELVETICA_10, (const unsigned char*)zV);

	} else {
		// Let user know that velocity is off
		glRasterPos3f(-0.95, 0.8, 0);
		glutBitmapString(GLUT_BITMAP_HELVETICA_10, (const unsigned char*)"OFF");
	}
}

/* drawText()
 *
 * Draw some text at a given location with a given font
 *
 * Parameters:
 *		char* msg  - The message to be drawn
 *		float x    - The x location to draw at
 *		float y    - The y locatino to draw at
 * 		void* font - The font to use
 * Returns:
 *
 */
void drawText(const char *msg, float x, float y, void *font) {
	glRasterPos2f(x, y);
	glutBitmapString(font, (const unsigned char *)msg);
}

/* drawInfoScreen()
 *
 * Draw the splash screen
 *
 * Parameters:
 *
 * Returns:
 *
 */void drawInfoScreen() {
	initLighting();

	float leftCol1 = -0.70;
	float leftCol2 = -0.45;
	float rightCol1 = 0.20;
	float rightCol2 = 0.35;



	//Set ortho view
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	// change to ortho view for 2D drawing
	glOrtho(-1, 1, -1, 1, 0.0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glLineWidth(1.5);

	drawText("Solar System Simulator", -0.25, 0.80, GLUT_BITMAP_HELVETICA_18);


	drawText("Mouse Controls", leftCol1 + 0.1, 0.65, GLUT_BITMAP_HELVETICA_18);

	drawText("Right Click", leftCol1, 0.60, GLUT_BITMAP_HELVETICA_12);
	drawText("Left Click & Drag", leftCol1, 0.55, GLUT_BITMAP_HELVETICA_12);
	drawText("Wheel Up", leftCol1, 0.50, GLUT_BITMAP_HELVETICA_12);
	drawText("Wheel Down", leftCol1, 0.45, GLUT_BITMAP_HELVETICA_12);

	drawText("Display Menu", leftCol2, 0.60, GLUT_BITMAP_HELVETICA_12);
	drawText("Rotate Scene", leftCol2, 0.55, GLUT_BITMAP_HELVETICA_12);
	drawText("Move Forward", leftCol2, 0.50, GLUT_BITMAP_HELVETICA_12);
	drawText("Move Backward", leftCol2, 0.45, GLUT_BITMAP_HELVETICA_12);

	drawText("Motion Control Keys", leftCol1 + 0.1, 0.35, GLUT_BITMAP_HELVETICA_18);

	drawText("Arrow Keys", leftCol1, 0.30, GLUT_BITMAP_HELVETICA_12);
	drawText("+", leftCol1, 0.25, GLUT_BITMAP_HELVETICA_12);
	drawText("-", leftCol1, 0.20, GLUT_BITMAP_HELVETICA_12);
	drawText("W", leftCol1, 0.15, GLUT_BITMAP_HELVETICA_12);
	drawText("S", leftCol1, 0.10, GLUT_BITMAP_HELVETICA_12);
	drawText("A", leftCol1, 0.05, GLUT_BITMAP_HELVETICA_12);
	drawText("D", leftCol1, 0.00, GLUT_BITMAP_HELVETICA_12);
	drawText("Q", leftCol1, -0.05, GLUT_BITMAP_HELVETICA_12);
	drawText("Z", leftCol1, -0.10, GLUT_BITMAP_HELVETICA_12);

	drawText("Rotate Scene", leftCol2, 0.30, GLUT_BITMAP_HELVETICA_12);
	drawText("Increase Speed", leftCol2, 0.25, GLUT_BITMAP_HELVETICA_12);
	drawText("Decrease Speed", leftCol2, 0.20, GLUT_BITMAP_HELVETICA_12);
	drawText("Move Forward", leftCol2, 0.15, GLUT_BITMAP_HELVETICA_12);
	drawText("Move Backward", leftCol2, 0.10, GLUT_BITMAP_HELVETICA_12);
	drawText("Move Left", leftCol2, 0.05, GLUT_BITMAP_HELVETICA_12);
	drawText("Move Right", leftCol2, 0.0, GLUT_BITMAP_HELVETICA_12);
	drawText("Move Up", leftCol2, -0.05, GLUT_BITMAP_HELVETICA_12);
	drawText("Move Down", leftCol2, -0.10, GLUT_BITMAP_HELVETICA_12);

	drawText("Draw Mode Keys", rightCol1 + 0.1, 0.65, GLUT_BITMAP_HELVETICA_18);

	drawText("1", rightCol1, 0.60, GLUT_BITMAP_HELVETICA_12);
	drawText("2", rightCol1, 0.55, GLUT_BITMAP_HELVETICA_12);
	drawText("3", rightCol1, 0.50, GLUT_BITMAP_HELVETICA_12);
	drawText("4", rightCol1, 0.45, GLUT_BITMAP_HELVETICA_12);

	drawText("Wire Frame", rightCol2, 0.60, GLUT_BITMAP_HELVETICA_12);
	drawText("Solid With Flat Shading", rightCol2, 0.55, GLUT_BITMAP_HELVETICA_12);
	drawText("Solid With Smooth Shading", rightCol2, 0.50, GLUT_BITMAP_HELVETICA_12);
	drawText("Texture Mapped", rightCol2, 0.45, GLUT_BITMAP_HELVETICA_12);

	drawText("Toggle Keys", rightCol1 + 0.1, 0.35, GLUT_BITMAP_HELVETICA_18);

	drawText("B", rightCol1, 0.30, GLUT_BITMAP_HELVETICA_12);
	drawText("M", rightCol1, 0.25, GLUT_BITMAP_HELVETICA_12);
	drawText("P", rightCol1, 0.20, GLUT_BITMAP_HELVETICA_12);
	drawText("L", rightCol1, 0.15, GLUT_BITMAP_HELVETICA_12);
	drawText("V", rightCol1, 0.10, GLUT_BITMAP_HELVETICA_12);
	drawText("I", rightCol1, 0.05, GLUT_BITMAP_HELVETICA_12);
	drawText("R", rightCol1, 0.0, GLUT_BITMAP_HELVETICA_12);
	drawText("ESC", rightCol1, -0.05, GLUT_BITMAP_HELVETICA_12);

	drawText("Display Planet Body Names", rightCol2, 0.30, GLUT_BITMAP_HELVETICA_12);
	drawText("Display Moon Names", rightCol2, 0.25, GLUT_BITMAP_HELVETICA_12);
	drawText("Pause Time", rightCol2, 0.20, GLUT_BITMAP_HELVETICA_12);
	drawText("Turn Lighting On/Off", rightCol2, 0.15, GLUT_BITMAP_HELVETICA_12);
	drawText("Turn Velocity Travel On/Off", rightCol2, 0.10, GLUT_BITMAP_HELVETICA_12);
	drawText("Display Info Screen", rightCol2, 0.05, GLUT_BITMAP_HELVETICA_12);
	drawText("Reset View To Starting Position", rightCol2, 0.0, GLUT_BITMAP_HELVETICA_12);
	drawText("Exit program", rightCol2, -0.05, GLUT_BITMAP_HELVETICA_12);


	drawText("Press 'I' to close this screen", -0.14, -0.30, GLUT_BITMAP_HELVETICA_12);
	glBegin(GL_LINES);
	glVertex2f(0.0, 0.75);
	glVertex2f(0.0, -0.20);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(-0.85, 0.75);
	glVertex2f(0.85, 0.75);
	glEnd();
}

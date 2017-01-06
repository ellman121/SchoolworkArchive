/******************************************************************************
 * File: drawInfo.h
 *
 * Authors: Elliott Rarden & Katie MacMillan
 *
 * Description: 
 * 		This file is a collection of OpenGL and GLUT callbacks
 ******************************************************************************/


// Defined constants
#define GLUT_WHEEL_UP 3
#define GLUT_WHEEL_DOWN 4

// Global things
extern bool infoFlag;
extern float  mouseX, mouseY;
string relative = "Sun";			// Initial relative body is the sun


/* displayCallback()
 *
 * Parameters:
 *
 * Returns: 
 */
void displayCallback( void ){
	float aspectRatio = ( float ) width / ( float ) height;
	// Set up the projection view matrix (not very well!)
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( 60.0, aspectRatio, 1.0, 1000000.0 );

	// Select the Modelview matrix
	glMatrixMode( GL_MODELVIEW );
	// Clear the redering window
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glLineWidth(1);

	if(!infoFlag){
		// Progress frame objects and variables to the next hour
		setNextFrame();
		// Clear the current matrix
		glLoadIdentity();

		// Move to current view point
		glTranslatef ( xTranslate, yTranslate, zTranslate );

		// Rotate the scene according to user specifications
		glRotatef( xRotate, 1.0, 0.0, 0.0 );
		glRotatef( yRotate, 0.0, 1.0, 0.0 );

		// If viewing system relative to a planet, move scene negated distance and orbit of planet
		if (relative != "Sun"){
			float sRadius = planetMap.at("Sun")->getRadius();
			float rRadius = planetMap.at(relative)->getRadius();
			// how much bigger the sun is than relative body
			float percent = 1 - (rRadius/sRadius);

			// translate to position of planet center in x position
			// translate in z direction forward or back depending on how big the planet is
			glTranslatef (-1 * (sRadius + rRadius + planetMap.at(relative)->getDistance()), 0, 200*percent);
			glRotatef( -1 * planetMap.at(relative)->getOrbit(), 0.0, 1.0, 0.0 );
		}

		// Draw each planet
		for (auto& p: planetMap){
			(p.second->getName() != "Sun") ? drawBody(p.second, false) : drawSun(p.second);
		}

		drawLighSource();
		drawStatus();
	}else {
		drawInfoScreen();
	}
	// Flush pipeline, swap buffers, and redraw
	glFlush();
	glutSwapBuffers();
	glutPostRedisplay();
}

/* passiveMouseCallback()
 *
 * Parameters:
 *		int x - the x location of the mouse
 * 		int y - the y location of the mouse
 * Returns: 
 */
void passiveMouseCallback(int x, int y){
	// get  current mouse position on screen
	float deltaX = x - ( width / 2.0 );
	float deltaY = height - y - ( height / 2.0 );

	// subtract stored coordinates from current and add to rotate
	xRotate += (deltaY-mouseY);
	yRotate += (deltaX-mouseX);

	// Store current position
	mouseX = deltaX;
	mouseY = deltaY;

	glutPostRedisplay();
}

/* mouseCallback()
 *
 * Parameters:
 *		int button - The button that was pressed
 *		int state  - The state of the button
 *		int x 	   - The x location of the mouse
 *		int y 	   - The y location of the mouse
 * Returns: 
 */
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
			if (state == GLUT_DOWN){
				// Store coordinates where button was pressed
				mouseX = x - ( width / 2.0 );
				mouseY = height - y - ( height / 2.0 );
				// Register mouse callback
				glutMotionFunc(passiveMouseCallback);
			} else {
				// Unregister mouse callback
				glutMotionFunc(NULL);
			}
	    } else if (button == GLUT_WHEEL_UP){
	    	(velocityFlag) ? zVelocity++ : zTranslate += 5;
		} else if (button == GLUT_WHEEL_DOWN){
			(velocityFlag) ? zVelocity-- : zTranslate -= 5;
		}
	}
	// Redraw image
	glutPostRedisplay();
}

/* keyboardCallback()
 *
 * Parameters:
 * 		uchar key - The key that was pressed
 *		int x 	  - The x location of the mouse
 * 		int y     - The y locatino of the mouse
 * Returns: 
 */
void keyboardCallback(unsigned char key, int x, int y){

	if (!infoFlag){
		switch(key){

			// Increase hours per frame
			case '+':
				(hourSpeed >= 1) ? hourSpeed++ : hourSpeed += 0.1;
			break;

			// Decrement by 1 until speed is 1 hour per frame then by 0.1
			case '-':
				// Don't go below 0.1 hour per frame
				(hourSpeed > 1) ? hourSpeed-- : (hourSpeed > 0.1) ? hourSpeed -= 0.1 : hourSpeed += 0;
			break;

			// Increase x direction velocity or move left in scene
	        case 'A':
			case 'a':
				(velocityFlag)? xVelocity++ : xTranslate += 5;
			break;
			// Decrease x direction velocity or move right in scene
			case 'D':
			case 'd':
				(velocityFlag) ? xVelocity-- : xTranslate -= 5;
			break;
			// Increase y direction velocity or move downward in scene
			case 'Z':
			case 'z':
				(velocityFlag) ? yVelocity++ : yTranslate += 5;
			break;
			// Decrease y direction velocity or move upward in scene
			case 'Q':
			case 'q':
				(velocityFlag) ? yVelocity-- : yTranslate -= 5;
			break;
			// Increase z direction velocity or move forward in scene
			case 'W':
			case 'w':
				(velocityFlag) ? zVelocity++ : zTranslate += 5;
			break;
			// Decrease z direction velocity or move backward in scene
			case 'S':
			case 's':
				(velocityFlag) ? zVelocity-- : zTranslate -= 5;
			break;

			// toggle velocity travel, set velocity values to 0
			case 'V':
			case 'v':
				velocityFlag = !velocityFlag;
				if (velocityFlag){
					xVelocity = yVelocity = zVelocity = 0;
				}
			break;

			//Toggle lighting
			case 'L':
			case 'l':
				lightFlag = !lightFlag;
				(lightFlag) ? glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING);
			break;

			// toggle labels for planetary bodies
			case 'B':
			case 'b':
				bodyLabelFlag = !bodyLabelFlag;
			break;

			// toggle labels for moons
			case 'M':
			case 'm':
				moonLabelFlag = !moonLabelFlag;
			break;
		}

	}
	switch(key){
		// Pause animation
        case 'P':
		case 'p':
			pauseFlag = !pauseFlag;
		break;
		// Pause animation
        case 'I':
		case 'i':
			infoFlag = !infoFlag;
		break;
		// Reset scene to original position
		case 'R':
		case 'r':
			resetView();
		break;

        // Set drawing modes 1 -> wireframe, 2 -> flat, 3 -> smooth, 4 ->image
        case '1':
            setDrawMode(wire);
        break;
        case '2':
            setDrawMode(flat);
        break;
        case '3':
            setDrawMode(smooth);
        break;
        case '4':
            setDrawMode(image);
        break;

        // Escape key to quit
        case 27:
            exit(0);
        break;
	}
	glutPostRedisplay();
}

/* specialKeyCallback()
 *
 * Parameters:
 *		int key - The key that was pressed
 *		int x   - The x location of the mouse
 *		int y   - The y location of the mouse
 * Returns: 
 */
void specialKeyCallback(int key, int x, int y){
	switch(key){
		// Rotate counter clockwise about the x axis
        case GLUT_KEY_UP:
			xRotate+= 0.5;
		break;

		// Rotate clockwise about the x axis
        case GLUT_KEY_DOWN:
			xRotate-= 0.5;
		break;

		// Rotate counter clockwise about the y axis
        case GLUT_KEY_RIGHT:
			yRotate+= 0.5;
		break;

		// Rotate clockwise about the y axis
        case GLUT_KEY_LEFT:
			yRotate-= 0.5;
		break;

	}
}


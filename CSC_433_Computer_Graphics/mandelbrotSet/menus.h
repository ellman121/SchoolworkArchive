/*
 * menus.h
 *
 * This file has functions utilized to create the popup menu
 */

#ifndef __MENUS_H
#define __MENUS_H

// Bring in the reset() function
extern void reset ();

/*******************************************************************************
 * Function: colourMenu
 *
 * Authors: Elliott Rarden & Katie Macmillan
 *
 * Description: 
 * colourMenu() takes a user selected index which represents a selected color
 * map, and sets it to be the index into the color book for the mandelbrot set.
 *
 * It then sets the color map index in the color book for the julia set to the
 * next color map following the selected map for the mandelbrot set.
 *
 * Parameters:
 *		[in] num - index to selected color scheme map
 ******************************************************************************/
void colourMenu(int num) {
	// Disable the mouse click so image doesn't pan
 	mouseFlag = true;
 	// Set mandelbrot image color to selected color map
	mandelColor = num;
	// Set julia color to color map following mandelbrot color
	juliaColor = (num+1)%numColourSchemes;
	glutPostRedisplay();
}

/*******************************************************************************
 * Function: mainMenu
 *
 * Authors: Elliott Rarden & Katie Macmillan
 *
 * Description:
 * mainMenu() takes in an integer representing a character which is used in a
 * switch statement to determine which action should be executed based on the
 * user selection.
 *
 * Parameters:
 *		[in] op - option selected by the user
 ******************************************************************************/
void mainMenu(int op) {
 	// Whenever a menu is selected, disable the mouse callback so we don't accidentally pan
 	mouseFlag = true;

	switch(op) {
		// Toggle the julia set
		case 'j':
			juliaFlag = !juliaFlag;
		break;
		// Auto rotate the color schemes
		case 'a':
    		rotateFlag = !rotateFlag;
    		glutIdleFunc(rotateColor);
		break;
		// Toggle the info page
		case 'i':
			infoFlag = !infoFlag;
		break;
		// Reset the image to defaults
		case 'r':
			reset();
		break;
		// Exit the program
		case 'q':
			exit(0);
	}
	// Refresh the view
	glutPostRedisplay();
}

/*******************************************************************************
 * Function: setupMenus
 *
 * Authors: Elliott Rarden & Katie Macmillan
 *
 * Description:
 * setupMenus() sets up a menu to be triggered by a right mouse button click.
 *
 * First the color scheme sub menu is created by adding an entry for each color
 * map in the color book. Then the sub menu and other menu options are added to
 * a main menu which is set to be triggered on a right mouse button click.
 ******************************************************************************/
void setupMenus () {
	// create a sub menu for color schemes
	int clrMenu = glutCreateMenu(colourMenu);
	// Add color names to sub menu
	for (int i = 0; i < numColourSchemes; ++i)
	{
		char temp[17];
		sprintf(temp, "%s", colorNames[i]);
		glutAddMenuEntry(temp, i);
 	}

	// create main "right click" menu
	glutCreateMenu(mainMenu);
	glutAddSubMenu("Colour Scheme", clrMenu);
	glutAddMenuEntry("Toggle Julia Mode (j)", 'j');
	glutAddMenuEntry("Auto Rotate Colour (a)", 'a');
	glutAddMenuEntry("Reset Image (r)", 'r');
	glutAddMenuEntry("Info Screen (i)", 'i');
	glutAddMenuEntry("Quit (q)", 'q');
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

#endif

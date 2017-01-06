/******************************************************************************
 * File: menu.h
 *
 * Authors: Elliott Rarden & Katie MacMillan
 *
 * Description: This file contains the code for the right click menu.
 *
 ******************************************************************************/

// If we are showing the info screen or not
extern bool infoFlag;

// Set the functions for the Speed Selection submenu
void setSpeedSelectSubmenuOption(int option) {
    switch (option) {
    case 0: // Minimum speed
        hourSpeed = 0.1;
    break;

    case 1: // 1 Hour/Step
        hourSpeed = 1;
    break;

    case 2: // 12 Hour/Step
        hourSpeed = 12;
    break;

    case 3: // 1 Day/Step
        hourSpeed = 24;
    break;

    case 4: // 2 Day/Step
        hourSpeed = 48;
    break;
    }

    glutPostRedisplay();
}

// Set the functions for the Planet Focus Selection submenu
void setPlanetSelectSubmenuOption(int option) {
    switch (option) {
    // Planets in order
    // "Marys Virgin Explanation Made Joseph 
    // Suspect Upstairs Neighbour"
    // 
    // Source: xkcd.com/992
    case 0:
    relative = 
        relative = "Sun";
        resetView();
    break;

    case 1:
        relative = "Mercury";
        resetView();
    break;

    case 2:
        relative = "Venus";
        resetView();
    break;

    case 3:
        relative = "Earth";
        resetView();
    break;

    case 4:
        relative = "Mars";
        resetView();
    break;

    case 5:
        relative = "Jupiter";
        resetView();
    break;

    case 6:
        relative = "Saturn";
        resetView();
    break;

    case 7:
        relative = "Uranus";
        resetView();
    break;

    case 8:
        relative = "Neptune";
        resetView();
    break;
    }

    glutPostRedisplay();
}

// Set the options for the main menu
void setMenuOption(int option) {
    switch (option) {
    case 0: // Toggle lighting
        lightFlag = !lightFlag;
        (lightFlag) ? glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING);
    break;

    case 1: // Toggle planatary labels
        bodyLabelFlag = !bodyLabelFlag;
    break;

    case 2: // Toggle moon labels
        moonLabelFlag = !moonLabelFlag;
    break;

    case 3: // Reset position
        resetView();
    break;

    case 4: // Info Screen
        infoFlag = !infoFlag;
    break;

    case 5: // Exit
        exit(0);
    break;
    }

    glutPostRedisplay();
}

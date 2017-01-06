#include "tank.h"

// Construct a tank with default values
tank::tank()
{
	// Default color is white
	tank_color c;
	c.red = 255;
	c.grn = 255;
	c.blu = 255;

	// Default values are position = (0,0)
	uint16_t p = 0;

	// Set our private variables
	hits = 0;			// We start with no hits
	orientation = 0; 	// Start facing left
	clr = c;			// Set our oclor
	pos = p;			// Set our position
	angle = 45;			// Start at 45 deg angle
	power = 50;			// Start with 50% power
}

// Construct a tank given a color and position
tank::tank(tank_color c, uint16_t p)
{
	// Set our private variables
	hits = 0;			// We start with no hits
	orientation = 0; 	// Start facing left
	clr = c;			// Set our oclor
	pos = p;			// Set our position
	angle = 45;			// Start at 45 deg angle
	power = 50;			// Start with 50% power
	ammo = 20;			// Start with 20 ammo
}

// Deconstruct a tank
tank::~tank()
{
}

/*
 * GETTERS
 */
// Get a tank's  color
tank_color tank::getColor()
{
	return clr;
}

// Get a tank's orientation
// Left is 0, right is 1
uint8_t tank::getOrientation()
{
	return orientation;
}

// Get a tank's position
uint16_t tank::getPosition()
{
	return pos;
}

// Get a tank's gas percentage
uint8_t tank::getGasPercentage()
{
	switch (hits) {
	case 0:
		return 100;
	case 1:
		return 50;
	case 2:
		return 25;
	default:
		return 0;
	}
}

// Get a tank's angle
uint8_t tank::getAngle()
{
	return angle;
}

// Get a tank's power level
uint8_t tank::getPower()
{
	return power;
}

// Get a tank's gun length
uint8_t tank::getGunLength()
{
	return 15; // It's always 15
}

// Return the number of times the tank has been hit
uint8_t tank::getHits()
{
	return hits;
}

uint8_t tank::getAmmo()
{
	return ammo;
}

/*
 *	Setters
 */
// Set a tank's color
bool tank::setColor(tank_color newColor)
{
	// Since we define the struct as uint8_t's
	// we don't have to check bounds
	clr = newColor;
	return true;
}

// Set a tank's orientation
// Even numbers => 0 => Left
// Odd  numbers => 1 => Right
bool tank::setOrientation(uint8_t newOrient)
{
	orientation = newOrient & 0x01; // Get the even/odd bit
	return true;
}

// Set a tank's position
bool tank::setPosition(uint16_t x)
{
	// If the position is within our bounds, set and return true
	if(x <= 1000)
	{
		pos = x;
		return true;
	}
	// Otherwise return false
	return false;
}

// Set a tank's firing angle
bool tank::setAngle(uint8_t newAngle)
{
	// If the new angle is within our bounds (You can't fire more than straight
	// up, and you can't fire lower than 0 degrees)
	if(newAngle < 90)
	{
		angle = newAngle;
		return true;
	}
	// Otherwise return false
	return false;
}

// Set a tank's power level (0-255)
bool tank::setPower(uint8_t newPower)
{
	power = newPower;
	return true;
}

/*
 * Tank Class Methods
 */
// Use one ammo
void tank::useAmmo()
{
	ammo--;
}

// Take a hit
void tank::takeHit()
{
	hits++;
}

// We take 4 hits.
// 1 looses 1/2 your gas
// 2 looses 3/4 your gas
// 3 looses all your gas (you can't move anymore)
// 4 you die
bool tank::isStillAlive()
{
	return hits < 3;
}

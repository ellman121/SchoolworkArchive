/* tank.h
 *
 * Class and public API definition for a tank
 *
 * Authors: Elliott Rarden & Katie MacMillan
 */

 // HEADER GUARD
#ifndef __tank_h
#define __tank_h

#include <stdint.h>
#include <stdio.h>

#define __TANK_MAX_POWER 255
#define __TANK_MIN_POWER 0
#define __TANK_MAX_ANGLE 90
#define __TANK_MIN_ANGLE 0

// Struct to hold a tank's color 
typedef struct tank_color_s {
	uint8_t red; // 0-255
	uint8_t grn; // 0-255
	uint8_t blu; // 0-255
} tank_color;

// Public decleration of tank class
class tank
{
public:
	// Constructors, destructors
	tank();
	tank(tank_color initColor, uint16_t initPos); // Initialize a tank with color and position 
	~tank();

	// Getters
	tank_color getColor();
	uint8_t getOrientation();
	uint16_t getPosition();
	uint8_t getGasPercentage();
	uint8_t getAngle();
	uint8_t getPower();
	uint8_t getGunLength();
	uint8_t getHits();
	uint8_t getAmmo();

	// Setters (all return true if successful)
	bool setColor(tank_color newColor);
	bool setOrientation(uint8_t newOrient);
	bool setPosition(uint16_t newPos);
	bool setAngle(uint8_t newAngle);
	bool setPower(uint8_t newPower);

	// Class methods
	void useAmmo();
	void takeHit();
	bool isStillAlive(); 
	void debuginfo();

private:
	uint8_t ammo;			// Amount of ammo a tank has
	uint8_t hits;			// Number of times the tank has been hit
	uint8_t orientation;	// 0 is left, 1 is right
	tank_color clr;			// Struct containing .red, .grn, and .blue as uint8_ts
	uint16_t pos;			// X position of the tank
	uint8_t angle;			// 0-90 degrees
	uint8_t power;			// 0-255 in m/s
};

#endif

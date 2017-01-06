#include "rings.h"
#include <cmath>
#include <iostream>
using namespace std;

/*
 * Constructors
 ***************/

// Use this constructor
ring::ring(string group, string planet, float iRadius, float oRadius, float incline, float hoursPerDay, float albedo, float color[3], Image_s img) {
	_group = group;
	_planet = planet;
	_innerRadius = iRadius / 1000;			// scale down radius
	_outerRadius = oRadius / 1000;			// scale down radius

	_rotation = 0;							// start at 0 degree rotation position
	if (hoursPerDay == 0) {
		_rotSpeed = 0;
	} else {
		_rotSpeed = 360.0 / hoursPerDay;	// rotational degree change per hour
	}

	_incline = incline;						// the angle in y at which the body orbits
	_albedo = albedo;						// the reflectivity coefficient of the body


	_color[0] = color[0];
	_color[1] = color[1];
	_color[2] = color[2];
	_img = img;

}

/*
 * Destructors
 ***************/
ring::~ring() {}

/*
 * Getters
 ***************/

// Get the inner radius of a ring
float ring::getInnerRadius() {
	return _innerRadius;
}

// Get the outer radius of a ring
float ring::getOuterRadius() {
	return _outerRadius;
}


// The number of degrees around self axis we've rotated
float ring::getRotation() {
	return _rotation;
}

// Get the orbit incline angle
float ring::getIncline() {
	return _incline;
}

// Get the orbit incline angle
float ring::getAlbedo() {
	return _albedo;
}

// Get the color from the ring
void ring::getColor(float *returned) {
	returned[0] = _color[0];
	returned[1] = _color[1];
	returned[2] = _color[2];
}

// Return a pionter to the bitmap image for the ring
Image_s ring::getImage() {
	return _img;
}

// Get the name of the ring group
string ring::getGroup() {
	return _group;
}

// Get the name of the parent ring
string ring::getPlanet() {
	return _planet;
}


/*
 * Setters
 ***************/
// sets the image for the ring
void ring::setImage(Image_s image) {
	_img = image;
}

/*
 * Activities
 ***************/

// Simulate the ring's movements in 1 hour
void ring::step(float speed)
{
	_rotation = fmod( _rotation + (_rotSpeed * speed), 360);
}

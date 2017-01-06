#include "planet.h"
#include <cmath>
#include <iostream>
using namespace std;
/*
 * Constructors
 ***************/

// No-Arg constructor
// Don't use this one
planet::planet() {
	_name = "";
	_radius = 0;
	_rotation =0;

}

// Use this constructor
planet::planet(string name, string parent, float radius, float distance, float daysPerYear, float hoursPerDay, float incline, float tilt, float albedo, float color[3], Image_s img, vector<string> satellites, vector<string> rings){
	_name = name;
	_parent = parent;
	_radius = radius/1000;								//scale down radius
	_distance = distance/200000;							//scale down distance
	_rotation = 0;										//start at 0 degree rotation position
	if (hoursPerDay == 0){
		_rotSpeed = 0;
	} else {
		_rotSpeed = 360.0/hoursPerDay;						//rotational degree change per hour		
	}

	_orbit = 0;											//start at 0 degree orbit position
	if (daysPerYear == 0){
		_orbitalSpeed = 0;
	} else {
		_orbitalSpeed = 360.0/(24.0*daysPerYear);				//orbital degree change per hour
	}
	
	
	_incline = incline;									// the angle in y at which the body orbits
	_axialTilt = tilt;									// the angle at which the body rotates
	_albedo = albedo;									// the reflectivity coefficient of the body


	_color[0] = color[0];
	_color[1] = color[1];
	_color[2] = color[2];
	_img = img;
	_satellites = satellites;							// the list of satellite names
	_rings = rings;	
}

/*
 * Destructors
 ***************/
planet::~planet(){}

/*
 * Getters
 ***************/

// Get the radius of a planet
float planet::getRadius(){
	return _radius;
}

// The distance from parent planet
float planet::getDistance(){
	return  _distance;
}

// The number of degrees around self axis we've rotated
float planet::getRotation(){
	return _rotation;
}

// The number of degrees around parent we've rotated
float planet::getOrbit(){
	return _orbit;
}

// Get the orbit incline angle
float planet::getTilt(){
	return _axialTilt;
}

// Get the orbit incline angle
float planet::getIncline(){
	return _incline;
}

// Get the orbit incline angle
float planet::getAlbedo(){
	return _albedo;
}

// Get the color from the planet
void planet::getColor(float *returned){
	returned[0] = _color[0];
	returned[1] = _color[1];
	returned[2] = _color[2];
}

// Return a pionter to the bitmap image for the planet
Image_s planet::getImage(){
	return _img;
}

// Get the name of the planet
string planet::getName(){
	return _name;
}

// Get the name of the parent planet
string planet::getParent(){
	return _parent;
}

// Get the names of the orbiting satellites
vector<string> planet::getSatellites(){
	return _satellites;
}

// Get the groups of the orbiting rings
vector<string> planet::getRings(){
	return _rings;
}


/*
 * Setters
****************/
// Return a pionter to the bitmap image for the planet
void planet::setImage(Image_s image){
	_img = image;
}

/*
 * Activities
 ***************/

// Simulate the planet's movements in 1 hour
void planet::step(float speed){
	_rotation = fmod( _rotation + (_rotSpeed * speed), 360);
	_orbit = fmod( _orbit + (_orbitalSpeed * speed), 360);
}

#ifndef __GLOBALS_H_
#define __GLOBALS_H_
#include <map>
#include <string>

using namespace std;

// Image structure
struct Image_s {
	int nRows;
	int nCols;
	unsigned char *img;
};

// Position
struct Position_s {
	float x;
	float y;
	float z;
};

// Current draw mode
enum Mode {wire, flat, smooth, image};

#endif

// imageReader.h
//
// Header file to to provide access to functinos to read
// uncmopressed bithmap images

#ifndef __IMAGE_READER_H__
#define __IMAGE_READER_H__

// Function defs
bool LoadBmpFile( string nameOfPlanet, int &NumRows, int &NumCols, unsigned char* &ImagePtr );

#endif

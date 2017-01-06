// Routines to read image files from uncompressed bitmap files.
//
// This code is a modified version of the code found at
// http://www.mcs.sdsmt.edu/csc433/Code/TexturedObjects/bmpRead.cpp

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <string.h>

using namespace std;

static short readShort( FILE* infile );
static int readLong( FILE* infile );
static void skipChars( FILE* infile, int numChars );
static inline int GetNumBytesPerRow( int NumCols );

// Take the name of the planet, look for it in the assets folder,
// and return the image in memory if we find one
bool LoadBmpFile( string nameOfPlanet, int &NumRows, int &NumCols, unsigned char* &ImagePtr )
{
    char* filename;

    // Convert the string name to all lower case and add
    // prefix/suffix for filename
    // e.g. "Sun" -> "./assets/sun.bmp"
    transform(nameOfPlanet.begin(), nameOfPlanet.end(), nameOfPlanet.begin(), ::tolower);
    string filepath = "./assets/" + nameOfPlanet + ".bmp";
    filename = (char *) malloc( sizeof(char) * filepath.size() + 1);

    // Copy the c++ string to a c string so we can use it
    for(int i = 0; i < filepath.size() + 1; i++)
        filename[i] = 0;
    strcpy(filename, filepath.c_str());

    FILE* infile = fopen( filename, "rb" ); // Open for reading binary data
    if ( !infile )
    {
        fprintf( stderr, "LoadBmpFile(): unable to open file: %s\n", filename );
        return false;
    }

    bool fileFormatOK = false;
    int bChar = fgetc( infile );
    int mChar = fgetc( infile );
    if ( bChar == 'B' && mChar == 'M' )
    {			// If starts with "BM" for "BitMap"
        skipChars( infile, 4 + 2 + 2 + 4 + 4 );			// Skip 4 fields we don't care about
        NumCols = readLong( infile );
        NumRows = readLong( infile );
        skipChars( infile, 2 );					// Skip one field
        int bitsPerPixel = readShort( infile );
        skipChars( infile, 4 + 4 + 4 + 4 + 4 + 4 );		// Skip 6 more fields

        if ( NumCols > 0 && NumCols <= 100000 && NumRows > 0 && NumRows <= 100000
                && bitsPerPixel == 24 && !feof( infile ) )
        {
            fileFormatOK = true;
        }
    }

    if ( !fileFormatOK )
    {
        fclose ( infile );
        fprintf( stderr, "Not a valid 24-bit bitmap file: %s.\n", filename );
        return false;
    }

    // Allocate memory
    ImagePtr = new unsigned char[ NumRows * GetNumBytesPerRow( NumCols ) ];
    if ( !ImagePtr )
    {
        fclose ( infile );
        fprintf( stderr, "Unable to allocate memory for %i x %i bitmap: %s.\n",
                 NumRows, NumCols, filename );
        return false;
    }

    unsigned char* cPtr = ImagePtr;
    for ( int i = 0; i < NumRows; i++ )
    {
        int j;
        for ( j = 0; j < NumCols; j++ )
        {
            *( cPtr + 2 ) = fgetc( infile );	// Blue color value
            *( cPtr + 1 ) = fgetc( infile );	// Green color value
            *cPtr = fgetc( infile );		// Red color value
            cPtr += 3;
        }
        int k = 3 * NumCols;					// Num bytes already read
        for ( ; k < GetNumBytesPerRow( NumCols ); k++ )
        {
            fgetc( infile );				// Read and ignore padding;
            *( cPtr++ ) = 0;
        }
    }

    if ( feof( infile ) )
    {
        fclose ( infile );
        fprintf( stderr, "Premature end of file: %s.\n", filename );
        return false;
    }

    // Close the file
    fclose( infile );

    // Free memory
    free(filename);

    return true;
}

// Rows are word aligned, this function find teh number
// of bytes in a row given the number of columns
static inline int GetNumBytesPerRow( int NumCols )
{
    return ( ( 3 * NumCols + 3 ) >> 2 ) << 2;
}

// read a 16-bit integer from the input file
short readShort( FILE* infile )
{
    unsigned char lowByte, hiByte;
    lowByte = fgetc( infile );          // Read the low order byte (little endian form)
    hiByte = fgetc( infile );           // Read the high order byte

    // Pack together
    short ret = hiByte;
    ret <<= 8;
    ret |= lowByte;
    return ret;
}

// read a 32-bit integer from the input file
int readLong( FILE* infile )
{
    unsigned char byte0, byte1, byte2, byte3;
    byte0 = fgetc( infile );            // Read bytes, low order to high order
    byte1 = fgetc( infile );
    byte2 = fgetc( infile );
    byte3 = fgetc( infile );

    // Pack together
    int ret = byte3;
    ret <<= 8;
    ret |= byte2;
    ret <<= 8;
    ret |= byte1;
    ret <<= 8;
    ret |= byte0;
    return ret;
}

// skip over given number of bytes in input file
void skipChars( FILE* infile, int numChars )
{
    for ( int i = 0; i < numChars; i++ )
    {
        fgetc( infile );
    }
}

# Makefile for TankGame

#-----------------------------------------------------------------------

# GNU C/C++ compiler and linker:
CC = gcc
CXX = g++
LINK = g++

# Turn on optimization and warnings (add -g for debugging with gdb):
CFLAGS = -O -Wall
CXXFLAGS = -O -Wall

# OpenGL libraries for Linux:
GL_LIBS = -lglut -lGLU -lGL -lm

#-----------------------------------------------------------------------

default: orbit.cpp planet.o imageReader.o rings.o globals.h
	g++ -o solar orbit.cpp planet.o imageReader.o rings.o -lglut -lGLU -lGL -lm -std=c++11

osx: orbit.cpp planet.o imageReader.o globals.h
	g++ -o solar orbit.cpp planet.o imageReader.o -framework OpenGL -framework GLUT -framework Cocoa -Wno-deprecated-declarations -std=c++11

planet.o: planet.cpp planet.h
	g++ -o planet.o planet.cpp -c -std=c++11

rings.o: rings.cpp rings.h
	g++ -o rings.o rings.cpp -c -std=c++11

imageReader.o: imageReader.cpp imageReader.h
	g++ -o imageReader.o imageReader.cpp -c -std=c++11

# utility targets
clean:
	@rm -f *.o *~ core
	@rm solar

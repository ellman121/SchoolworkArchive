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

default: Tankgame.c tank.cpp tank.h
	g++ -o tankwars Tankgame.c tank.cpp -lglut -lGLU -lGL -std=c++11

# utility targets
clean:
	@rm -f *.o *~ core
	@rm tankwars

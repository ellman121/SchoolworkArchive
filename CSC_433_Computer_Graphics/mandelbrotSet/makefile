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
GL_MAC_LIBS = -I/usr/local/include -framework OpenGL 

#-----------------------------------------------------------------------

default: mandelbrot.o mandelcuda.o
# Link our files together
	nvcc -o mandelbrot -lglut -lGLU -lGL mandelbrot.o mandelcuda.o

debug: mandelbrot.o mandelcuda.o
# Link our files together
	nvcc -o mandelbrot -lglut -lGLU -lGL -g mandelbrot.o mandelcuda.o

mandelbrot.o: mandelbrot.cpp
# Compile our C++ code
	g++ -c -Wall -O3 -std=c++11 -lglut -lGLU -lGL -g mandelbrot.cpp -o mandelbrot.o

mandelcuda.o: mandelcuda.cu
# Compile our CUDA code
	nvcc -c -O3 -std=c++11 --expt-relaxed-constexpr -g mandelcuda.cu -o mandelcuda.o

macos: mandelbrot.cpp
	g++ -o mandelbrot -v -framework OpenGL --std=c++11 mandelbrot.cpp

# utility targets
clean:
	@rm -f *.o *~ core
	@rm mandelbrot

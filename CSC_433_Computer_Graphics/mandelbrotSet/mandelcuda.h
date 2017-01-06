// Headerguard
#ifndef __mandelcuda_h
#define __mandelcuda_h

// Cuda Function Prototypes
void cudaCalcMandelbrot(double *setOfReals, double *setOfImags, int *iterations, int len);
void cudaCalcJulia(double zReal, double zImag, double *setOfReals, double *setOfImags, int *iterations, int len);

#endif

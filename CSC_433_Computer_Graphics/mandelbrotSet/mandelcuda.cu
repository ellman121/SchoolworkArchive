#include <complex>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cuComplex.h>

// Kernel Definitions

/******************************************************************************
 * Function: CUDAisInMandelbrotSet
 *
 * Authors: Elliott Rarden & Katie Macmillan
 *
 * Description: 
 * CUDAisInMandelbrotSet() takes in cuDoubleComplex number, c, and an int pointer
 * to return the number of iterations
 *
 * Parameters:		[in]	c
 *				[in-out]	iterations
 ******************************************************************************/
__device__ void CUDAisInMandelbrotSet(cuDoubleComplex c, int *iterations){
	double zr = 0;
	double zi = 0;
	cuDoubleComplex z = make_cuDoubleComplex(zr, zi);
	int i = 0;

	for (i = 0; i < 100 && cuCabs(z) < 2.0; ++i){
		z = cuCadd(cuCmul(z, z), c); // z = z*z + c;
	}

	*iterations = i;
}

/******************************************************************************
 * Function: CUDAisInMandelbrotSet
 *
 * Authors: Elliott Rarden & Katie Macmillan
 *
 * Description: 
 * CUDAisInJuliaSet() takes in two cuDoubleComplex numbers, c & z, and an int
 * pointer to return the number of iterations
 *
 * Parameters:		[in]	z
 *					[in]	c
 *				[in-out]	iterations
 ******************************************************************************/
__device__ void CUDAisInJuliaSet(cuDoubleComplex z, cuDoubleComplex c, int *iterations){
	int i = 0;

	for (i = 0; i < 100 && cuCabs(z) < 2.0; ++i){
		z = cuCadd(cuCmul(z, z), c); // z = z*z + c;
	}

	*iterations = i;
}

/******************************************************************************
 * Function: mandelCalc
 *
 * Authors: Elliott Rarden & Katie Macmillan
 *
 * Description: 
 * mandelCalc() is a kernel which takes in a set of real and imaginary values
 * and will calcualte in parallel wether or not each point is in or out of the
 * mandelbrot set.
 *
 * Parameters:		[in]	cReals
 *					[in]	cImags
 *				[in-out]	iterations
 *					[in]	len
 ******************************************************************************/
__global__ void mandelCalc(double *cReals, double *cImags, int *iterations, int len) {
	int i = blockDim.x * blockIdx.x + threadIdx.x;
	cuDoubleComplex num;

	if(i < len){
		num = make_cuDoubleComplex(cReals[i], cImags[i]);
		CUDAisInMandelbrotSet(num, &iterations[i]);
	}
}

/******************************************************************************
 * Function: juliaCalc
 *
 * Authors: Elliott Rarden & Katie Macmillan
 *
 * Description: 
 * juliaCalc() is a kernel which takes in a set of real and imaginary values as
 * well as a real and imaginary number (as doubles), and will calcualte in
 * parallel wether or not each point in the arrays is in or out of the Julia set
 * associated with the given point
 *
 * Parameters:		[in]	zReal
 *					[in]	zImag
 *					[in]	cReals
 *					[in]	cImags
 *				[in-out]	iterations
 *					[in]	len
 ******************************************************************************/
__global__ void juliaCalc(double zReal, double zImag, double *cReals, double *cImags, int *iterations, int len) {
	int i = blockDim.x * blockIdx.x + threadIdx.x;
	cuDoubleComplex num;
	cuDoubleComplex z;

	if(i < len){
		num = make_cuDoubleComplex(cReals[i], cImags[i]);
		z = make_cuDoubleComplex(zReal, zImag);
		CUDAisInJuliaSet(num, z, &iterations[i]);
	}
}

/******************************************************************************
 * Function: cudaCalcMandelbrot
 *
 * Authors: Elliott Rarden & Katie Macmillan
 *
 * Description: 
 * cudaCalcMandelbrot() is a function which takes in a set of real and imaginary
 * values, and will reutrn if each point is in our out of the mandelbrot set using
 * the in-out iterations parameter
 *
 * Parameters:		[in]	setOfReals
 *					[in]	setOfImags
 *				[in-out]	iterations
 *					[in]	len
 ******************************************************************************/
__host__ void cudaCalcMandelbrot(double *setOfReals, double *setOfImags, int *iterations, int len) {
	// Block management
	int n = len;

	// Device arrays
	double *d_set_reals;
	double *d_set_imags;
	int *d_iterations;

	// Allocate our memory on the device
	cudaMalloc(&d_set_reals,	len * sizeof(double));
	cudaMalloc(&d_set_imags,	len * sizeof(double));
	cudaMalloc(&d_iterations,	len * sizeof(int)); 

	// Copy our stuff to the device
	cudaMemcpy(d_set_reals,		setOfReals,	len * sizeof(double),	cudaMemcpyHostToDevice);
	cudaMemcpy(d_set_imags,		setOfImags,	len * sizeof(double),	cudaMemcpyHostToDevice);
	cudaMemcpy(d_iterations,	iterations,	len * sizeof(int),	cudaMemcpyHostToDevice);

	// Run the code on the GPU
	int nThreads = 128; // should be multiple of 32 (up to 1024)
	int nBlocks = ( n + nThreads - 1 ) / nThreads;
	mandelCalc<<<nBlocks, nThreads>>>(d_set_reals, d_set_imags, d_iterations, len);

	// Copy stuff from the GPU to our host
	cudaMemcpy(setOfReals,	d_set_reals,	len * sizeof(double),	cudaMemcpyDeviceToHost);
	cudaMemcpy(setOfImags,	d_set_imags,	len * sizeof(double),	cudaMemcpyDeviceToHost);
	cudaMemcpy(iterations,	d_iterations,	len * sizeof(int),	cudaMemcpyDeviceToHost);

	// Free the device memory
	cudaFree(d_set_reals);
	cudaFree(d_set_imags);
	cudaFree(d_iterations);
}

/******************************************************************************
 * Function: cudaCalcJulia
 *
 * Authors: Elliott Rarden & Katie Macmillan
 *
 * Description: 
 * cudaCalcJulia() is a function which takes in a real and imaginary z value and
 * a set of real and imaginary values, c, and will reutrn if each point in c is
 * in the Julia set associated with the point z
 *
 * Parameters:		[in]	zReal
 *					[in]	zImag
 *					[in]	cReals
 *					[in]	cImags
 *				[in-out]	iterations
 *					[in]	len
 ******************************************************************************/
__host__ void cudaCalcJulia(double zReal, double zImag, double *setOfReals, double *setOfImags, int *iterations, int len) {
	// Local variables
	int n = len;

	// Device variables
	double *d_set_reals;
	double *d_set_imags;
	int *d_iterations;

	// Allocate our memory on the device
	cudaMalloc(&d_set_reals,	len * sizeof(double));
	cudaMalloc(&d_set_imags,	len * sizeof(double));
	cudaMalloc(&d_iterations,	len * sizeof(int));

	// Copy our stuff to the device
	cudaMemcpy(d_set_reals,		setOfReals,		len * sizeof(double),			cudaMemcpyHostToDevice);
	cudaMemcpy(d_set_imags,		setOfImags,		len * sizeof(double),			cudaMemcpyHostToDevice);
	cudaMemcpy(d_iterations,	iterations,		len * sizeof(int),				cudaMemcpyHostToDevice);

	// Run the code on the GPU
	int nThreads = 128; // should be multiple of 32 (up to 1024)
	int nBlocks = ( n + nThreads - 1 ) / nThreads;
	juliaCalc<<<nBlocks, nThreads>>>(zReal, zImag, d_set_reals, d_set_imags, d_iterations, len);

	// Copy stuff from the GPU to our host
	cudaMemcpy(iterations,	d_iterations,	len * sizeof(int),	cudaMemcpyDeviceToHost);

	// Free the device memory
	cudaFree(d_set_reals);
	cudaFree(d_set_imags);
	cudaFree(d_iterations);
}

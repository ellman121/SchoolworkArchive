/*
 * mandelbrot.h
 *
 * This file has functions for calculating whether a point is in the mandelbrot
 * and julia sets.
 */

#ifndef __MANDELBROT_H
#define __MANDELBROT_H

#include <cmath>
#include <complex>

using namespace std;

/******************************************************************************
 * Function: isInMandelbrotSet
 *
 * Authors: Elliott Rarden & Katie Macmillan
 *
 * Description: 
 * isInMandelbrotSet() takes in complex double, which is the compex parameter.
 * It will return true or false wether the point is in the Mandelbrot Set.
 *
 * Parameters:	    [in]          c
 *				[in-out] iterations
 * Returns:		 True - The z value is in the julia set for c
 *				False - The z value is not in the julia set for c
 ******************************************************************************/
bool isInMandelbrotSet(complex<double> c, int &iterations){
	complex<double> z = 0;
	unsigned i;

	for (i = 0; i < 100 && abs(z) < 2.0; ++i)
		z = z*z + c;

	iterations = i;
	return (i >= 100) ? true : false;
}

/******************************************************************************
 * Function: isInJuliaSet
 *
 * Authors: Elliott Rarden & Katie Macmillan
 *
 * Description: 
 * isInJuliaSet () takes in two complex doubles, on as c, which is a specified
 * point that will remain consistent through the evaluation of all points in 
 * the complex plane.
 *
 * The value z, whch is passed in is a point in the complex plain being
 * evaluated.
 *
 * Parameters:	    [in]          c
 *				    [in]          z
 *				[in-out] iterations
 * Returns:		 True - The z value is in the julia set for c
 *				False - The z value is not in the julia set for c
 ******************************************************************************/
bool isInJuliaSet(complex<double> c, complex<double> z, int &iterations){
	unsigned i;

	for (i = 0; i < 100 && abs(z) < 2.0; ++i)
		z = z*z + c;
	
	iterations = i;
	return (i >= 100) ? true : false;
}

#endif

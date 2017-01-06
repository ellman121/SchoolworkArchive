/*
 * draw.h
 *
 * This file has functions concerning the mandelbrot set itslef
 */

#ifndef __DRAW_H
#define __DRAW_H

/* GLOBAL VARABLES */
// Window stuff
extern bool juliaFlag, infoFlag, mouseFlag, cudaFlag;
//extern UNCALCULATED;
#define UNCALCULATED -1


//-------------------------------------------------------------------------
//  Drawing Functions
//-------------------------------------------------------------------------

/******************************************************************************
 * Function: drawText
 *
 * Authors: Elliott Rarden & Katie Macmillan
 *
 * Description: 
 * This function takes a string and draws it at point x, y with the given color
 * and font.
 *
 * Parameters:	    [in]          msg
 *				    [in]          x
 *				    [in]          y
 *				    [in]          color
 *				    [in]          font
 ******************************************************************************/
void drawText(const char *msg, float x, float y, const float color[], void *font){
	glColor3fv( color );
  	glRasterPos2f(x, y);
	glutBitmapString(font, (const unsigned char *)msg);
}

/******************************************************************************
 * Function: drawInfoScreen
 *
 * Authors: Elliott Rarden & Katie Macmillan
 *
 * Description: 
 * This function draws the information/splash screen
 ******************************************************************************/
void drawInfoScreen(){
	float rightCol1 = 0.15;
	float rightCol2 = 0.45;
	float leftCol1 = -0.85;
	float leftCol2 = -0.50;

	glLineWidth(1.5);
	drawText("Fractals Are Beautiful...", -0.25, 0.80, Teal, GLUT_BITMAP_HELVETICA_18);

	drawText("Mouse Controls", leftCol1, 0.65, Teal, GLUT_BITMAP_HELVETICA_18);
	
	drawText("Left Click", leftCol1, 0.55, Teal, GLUT_BITMAP_HELVETICA_12);
	drawText("Left Click & Drag", leftCol1, 0.45, Teal, GLUT_BITMAP_HELVETICA_12);
	drawText("Right Click", leftCol1, 0.35, Teal, GLUT_BITMAP_HELVETICA_12);
	drawText("Wheel Up", leftCol1, 0.25, Teal, GLUT_BITMAP_HELVETICA_12);
	drawText("Wheel Down", leftCol1, 0.15, Teal, GLUT_BITMAP_HELVETICA_12);
	
	drawText("Recenter image at selected point", leftCol2, 0.55, Teal, GLUT_BITMAP_HELVETICA_12);
	drawText("Pan image", leftCol2, 0.45, Teal, GLUT_BITMAP_HELVETICA_12);
	drawText("Display Menu", leftCol2, 0.35, Teal, GLUT_BITMAP_HELVETICA_12);
	drawText("Zoom In", leftCol2, 0.25, Teal, GLUT_BITMAP_HELVETICA_12);
	drawText("Zoom Out", leftCol2, 0.15, Teal, GLUT_BITMAP_HELVETICA_12);


	drawText("Keyboard Controls", rightCol1, 0.65, Teal, GLUT_BITMAP_HELVETICA_18);
	
	drawText("Arrow Keys", rightCol1, 0.55, Teal, GLUT_BITMAP_HELVETICA_12);
	drawText("+", rightCol1, 0.45, Teal, GLUT_BITMAP_HELVETICA_12);
	drawText("-", rightCol1, 0.35, Teal, GLUT_BITMAP_HELVETICA_12);
	drawText("C", rightCol1, 0.25, Teal, GLUT_BITMAP_HELVETICA_12);
	drawText("A", rightCol1, 0.15, Teal, GLUT_BITMAP_HELVETICA_12);
	drawText("J", rightCol1, 0.05, Teal, GLUT_BITMAP_HELVETICA_12);
	drawText("P", rightCol1, -0.05, Teal, GLUT_BITMAP_HELVETICA_12);
	drawText("S", rightCol1, -0.15, Teal, GLUT_BITMAP_HELVETICA_12);
	drawText("R", rightCol1, -0.25, Teal, GLUT_BITMAP_HELVETICA_12);
	drawText("I", rightCol1, -0.35, Teal, GLUT_BITMAP_HELVETICA_12);
	drawText("ESC or Q", rightCol1, -0.45, Teal, GLUT_BITMAP_HELVETICA_12);
	
	drawText("Pan image", rightCol2, 0.55, Teal, GLUT_BITMAP_HELVETICA_12);
	drawText("Zoom In", rightCol2, 0.45, Teal, GLUT_BITMAP_HELVETICA_12);
	drawText("Zoom Out", rightCol2, 0.35, Teal, GLUT_BITMAP_HELVETICA_12);
	drawText("Switch color scheme", rightCol2, 0.25, Teal, GLUT_BITMAP_HELVETICA_12);
	drawText("Auto rotate color schemes", rightCol2, 0.15, Teal, GLUT_BITMAP_HELVETICA_12);
	drawText("Toggle Julia set", rightCol2, 0.05, Teal, GLUT_BITMAP_HELVETICA_12);
	drawText("Compute in parallel", rightCol2, -0.05, Teal, GLUT_BITMAP_HELVETICA_12);
	drawText("Compute sequentially", rightCol2, -0.15, Teal, GLUT_BITMAP_HELVETICA_12);
	drawText("Reset image to default", rightCol2, -0.25, Teal, GLUT_BITMAP_HELVETICA_12);
	drawText("Toggle info screen", rightCol2, -0.35, Teal, GLUT_BITMAP_HELVETICA_12);
	drawText("Exit program", rightCol2, -0.45, Teal, GLUT_BITMAP_HELVETICA_12);

	drawText("Press 'I' to close this screen", -0.14, -0.60, Teal, GLUT_BITMAP_HELVETICA_12);

	if (cudaFlag){
		drawText("**Computations are currently being ran in parallel on the GPU**", -0.40, -0.70, Teal, GLUT_BITMAP_HELVETICA_12);
	} else {
		drawText("**Computations are currently being ran sequentially**", -0.35, -0.70, Teal, GLUT_BITMAP_HELVETICA_12);
	}
	glBegin(GL_LINES);
		glVertex2f(0.08, 0.75);
		glVertex2f(0.08, -0.50);
	glEnd();	
	glBegin(GL_LINES);
		glVertex2f(-0.85, 0.75);
		glVertex2f(0.85, 0.75);
	glEnd();
}

/******************************************************************************
 * Function: drawMandelbrot
 *
 * Authors: Elliott Rarden & Katie Macmillan
 *
 * Description: 
 * This function draws the mandelbrot set image using sequential (non-gpu) code
 ******************************************************************************/
void drawMandelbrot(){
	int iterations;
	float xMax = xMid + range;
	float yMax = yMid + range;
	// go through each row of x values and each column of y values for each x
	for (float x = xMid-range; x <= xMax; x += interval){
		for (float y = yMid-range; y <= yMax; y += interval){
			// look to see if the (x,y) coordinate is in the mandelbrot set
			isInMandelbrotSet(complex<double>(x,y),iterations);
			// color points not in the set based on iterations it took to diverge
			glColor3fv(colorBook[mandelColor][iterations]);
			glBegin(GL_POINTS);
				glVertex2f((x-xMid)*scale, (y-yMid)*scale);
			glEnd();
		}
	}
}

/******************************************************************************
 * Function: drawJulia
 *
 * Authors: Elliott Rarden & Katie Macmillan
 *
 * Description: 
 * This function draws the julia set for a point using sequential (non-gpu) code
 ******************************************************************************/
void drawJulia(){
	int iterations;
	float xMax = xMid + range;
	float yMax = yMid + range;
	float xPt, yPt;

	// go through each row of x values and each column of y values for each x
	for (float x = xMid-range; x <= xMax; x += interval){
		for (float y = yMid-range; y <= yMax; y += interval){
			xPt = -0.75 + (((x-xMid)*scale)/4);
			yPt = 0.75 + (((y-yMid)*scale)/4);
			// look to see if the (x,y) coordinate is in the mandelbrot set
			isInJuliaSet(complex<double>(xJul,yJul),complex<double>(x,y),iterations);
			// color points not in the set based on iterations it took to diverge
			glColor3fv(colorBook[juliaColor][iterations]);
			glBegin(GL_POINTS);
				glVertex2f(xPt, yPt);
			glEnd();
		}
	}
}

/******************************************************************************
 * Function: drawMandelbrotCuda
 *
 * Authors: Elliott Rarden & Katie Macmillan
 *
 * Description: 
 * This function draws the mandelbrot set using the parallel (CUDA) code
 ******************************************************************************/
void drawMandelbrotCuda(){
	// Iterator
	int pos = 0;

	// int iterations;
	// double xPt, yPt;
	double xMin = xMid - range;
	double yMin = yMid - range;
	double xMax = xMid + range;
	double yMax = yMid + range;

	static vector<double> ptsReal;	// An array to hold the real values of all of our points
	static vector<double> ptsImag;	// An array to hold the imag values of all of our points
	// bool *inSet;				// An array to hold if each point is in mandelbrot set or not
	int *iters;					// An array to hold the number of iterations for each pont

	ptsReal.clear();
	ptsImag.clear();
	// Create a 1D array of all points
	for (double x = xMid-range; x <= xMax; x += interval){
		for (double y = yMid-range; y <= yMax; y += interval){
			ptsReal.push_back(x);
			ptsImag.push_back(y);
		}
	}

	// Allocate memory and initialize
	// inSet = new bool[ptsReal.size()];
	iters = new int[ptsReal.size()];
	for (unsigned i = 0; i < ptsReal.size(); ++i)
	// {
		// inSet[i] = false;
		iters[i] = UNCALCULATED;
	// }

	// Go calculate our points
	// cudaCalcMandelbrot(&ptsReal[0], &ptsImag[0], inSet, iters, ptsReal.size());
	cudaCalcMandelbrot(&ptsReal[0], &ptsImag[0], iters, ptsReal.size());

	// Colour in our pixels
	pos = 0;
	for (double x = xMin; x <= xMax; x += interval){
		for (double y = yMin; y <= yMax; y += interval){
			// If we calculated the point, fill it in.
			int temp = iters[pos];
			if(temp != UNCALCULATED) {
				glColor3fv(colorBook[mandelColor][iters[pos]]);
				glBegin(GL_POINTS);
					glVertex2f((x-xMid)*scale, (y-yMid)*scale);
				glEnd();
			}
			pos++;			
		}
	}

	// Free our memory
	// delete[] inSet;
	delete[] iters;
}

/******************************************************************************
 * Function: drawJuliaCuda
 *
 * Authors: Elliott Rarden & Katie Macmillan
 *
 * Description: 
 * This function draws the julia set for a point using the parallel (CUDA) code
 ******************************************************************************/
void drawJuliaCuda(){
	// Iterator
	int pos = 0;
	double xMax = xMid + range;
	double yMax = yMid + range;

	// Vectors to hold the real values, imaginary values, and iterations of each point
	static vector<double> ptsReal;	
	static vector<double> ptsImag;
	int *iters;

	// Clear our vectors
	ptsReal.clear();
	ptsImag.clear();

	// Create a 1D array of all points
	for (double x = xMid-range; x <= xMax; x += interval){
		for (double y = yMid-range; y <= yMax; y += interval){
			ptsReal.push_back(x);
			ptsImag.push_back(y);
		}
	}

	// Allocate memory and initialize to uncalculated value
	iters = new int[ptsReal.size()];
	for (unsigned i = 0; i < ptsReal.size(); ++i)
		iters[i] = UNCALCULATED;

	// Go calculate our points
	cudaCalcJulia(xJul, yJul, &ptsReal[0], &ptsImag[0], iters, ptsReal.size());

	// Colour in our pixels
	pos = 0;
	for (double x = xMid-range; x <= xMax; x += interval){
		for (double y = yMid-range; y <= yMax; y += interval){
			// If we calculated the point, fill it in.
			int temp = iters[pos];
			if(temp != UNCALCULATED) {
				glColor3fv(colorBook[juliaColor][iters[pos]]);
				glBegin(GL_POINTS);
					glVertex2f(-0.75+(((x-xMid)*scale)/4), 0.75+(((y-yMid)*scale)/4));
				glEnd();
			}
			pos++;			
		}
	}

	// Free our memory
	delete[] iters;
}


#endif

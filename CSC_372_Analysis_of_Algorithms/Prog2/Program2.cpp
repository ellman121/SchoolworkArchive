// Program 2
// Written by Elliott Rarden
// Analysis of Algorithms
// Dr. Logar + Dr. Corwin
// 
// This program uses several modified functions from geometry.C from the CS372 website

#include <iostream>   // cin, cout
#include <fstream>
#include <stdint.h>   // Standardized int sizes
#include <math.h>     // fmin() and fmax()
#include <iomanip>
#include <vector>

using namespace std;

// Point struct.
struct point
{
	double x; // X-coordinate Point
	double y; // Y-coordinate Point
};
typedef point vctr;

//
// Function Prototypes
// 
double area(vector<point> p);
double crossProduct(vctr a, vctr b);
double direction(point a, point b, point c);
bool linesDoIntersect(point a, point b, point c, point d);
bool isConvex(vector<point> verticies);
bool onSegment(point a, point b, point c);

// int main()
// 
// Description:  This program reads in a number of cases consisting of sets
// of points from an input file, then finds and outputs the x coordinate
// where a verticle line will cut the shape in half by area.
// 
// Params:
// 		Input:	int argc - number of arguments passed on the command line
// 				char** argv - argument values
// 				
// 	Return: Error code.  0 means no errors.  
int main(int argc, char const *argv[])
{
	ifstream fin;
	ofstream fout;
	int numPoints;
	int caseNum = 0;
	double x;
	double left;
	double right;
	double height;
	vector<point> verticies;
	string ifile = argv[1];
	string ofile = ifile.substr(0, ifile.find_last_of(".")) + ".out";

	fin.open(ifile.c_str());
	fout.open(ofile.c_str());

	while(fin >> numPoints)
	{
		caseNum++;
		verticies.clear();
		for(int i = 0; i < numPoints; i++)
		{
			point temp;
			fin >> temp.x;
			fin >> temp.y;
			verticies.push_back(temp);
		}

		if (!isConvex(verticies))
		{
			fout << "Case " << caseNum << ": No Solution" << endl;
			continue;
		}

		// Find the leftmost and rightmost points as well as the highest point
		left = verticies[0].x;
		right = verticies[0].x;
		for (int i = 1; i < verticies.size(); ++i) // We don't hae to look at i = 0 from above
		{
			double test = verticies[i].x;
			if(test < left)
			{
				left = test;
			}
			else if (test > right)
			{
				right = test;
			}
			if (verticies[i].y > height)
			{
				height = verticies[i].y;
			}
		}

		// The middle x line
		height += 1.0;
			
		vector<point> testPolygon = verticies;
		for (int j = 0; j < 300; ++j)
		{
			x = (left + right) / 2.0;

			// Cut the polygon at x
			for (int i = 0; i < testPolygon.size(); ++i)
			{
				point bot = {x, 0};
				point top = {x, height};
				point left = testPolygon[i];
				point right = testPolygon[(i+1)%testPolygon.size()];
				if (left.x == right.x || left.x == x || right.x == x)
				{
					// The two points are in a verticle line.  Just do nothing, it actually works
				}
				else if (linesDoIntersect(bot, top, testPolygon[i], testPolygon[(i+1)%testPolygon.size()]))
				{
					// Insert a point at the intersection.  I solved for y = mx + b here
					// m = ((right.y - left.y)/(right.x - left.x))
					// x = (text_x - left.x)
					// b = left.y
					point newPoint = {x, ((right.y - left.y)/(right.x - left.x)) * (x-left.x) + left.y};
					testPolygon.insert(testPolygon.begin() + i + 1, 1, newPoint);
					i++;
				}
			}

			// Create "Left" and "Right" polygons
			vector<point> leftPolygon;
			vector<point> rightPolygon;
			for (int i = 0; i < testPolygon.size(); ++i)
			{
				point p = testPolygon[i];
				if (testPolygon[i].x <= x)
				{
					leftPolygon.push_back(p);
				}
				if (testPolygon[i].x >= x)
				{
					rightPolygon.push_back(p);
				}
			}

			// If area's are equal, output and break
			if (fabs(area(leftPolygon) - area(rightPolygon)) < 0.00001 || j == 299)
			{
				fout << fixed << "Case " << caseNum << ": " << setprecision(5) << x << endl;
				break;
			}
			else if (area(leftPolygon) > area(rightPolygon))
			{
				right = x;
			}
			else
			{
				left = x;
			}
		}
	}

	return 0;
}

// double area(vector<point> p)
//
// Description: Returns the area of a polygon
//
// Params:
// 		Input:	vector<point> p - A vector containing the verticies of a polygon
// 				
// 	Return: A IEEE floating point value with the area of the polygon
double area(vector<point> p)
{
	int numSides = p.size();
	int j = 0;
	double result = 0;

	result = 0;
	for (int i = 0; i < numSides; i++)
	{
		j = (i + 1) % numSides;
		result += p[i].x * p[j].y;
		result -= p[i].y * p[j].x;
	}

	return fabs(result / 2);
}

// double crossProduct()
//
// Description: Return the 2D cross product of two TD vectors
//
// Params:
// 		Input:	vctr a - the vector from (0,0) to a.x, a.y
// 				vctr b - the vector from (0,0) to b.x, b.y
//
// 	Return: A double specifying the cross product value
double crossProduct(vctr a, vctr b) // vctr is typedefed to point
{
	return a.x * b.y - a.y * b.x;
}

// double direction()
// 
// Description: Given a "walk" from a to b to c, return positive, negative, or 
// 		zero depeinding on if a left, right, or no turn (180 deg) was taken at b
// 		
// 	Params:
// 		Input:	point a - Start point of "walk"
// 				point b - Middle point of "walk"
// 				point c - End point of "walk"
// 				
double direction(point a, point b, point c)
{
	double retval; // Return value
	point ab; // Vector from a to b
	point bc; // Vector from b to c

	// Fill our vectors
	ab.x = b.x - a.x;
	ab.y = b.y - a.y;
	bc.x = c.x - b.x;
	bc.y = c.y - b.y;

	// Get cross product
	retval =  crossProduct(ab, bc);

	// If were close to 0, set it to 0.  Man, wouldn't it be nice to have precision?
	if (fabs(retval) < 1.0e-6)
		retval = 0.0;

	return retval;
}

// bool linesDoIntersect()
//
// Description: Returns true if segment ab intersects with segment cd
//
// Params:
// 		Input:	point a - "start" point on segment AB
// 				point b - "end" point on segment AB
// 				point c - "start" point on segment CD
// 				point d - "end" point on segment CD
//
// 	Return: True/false if the lines intersect
bool linesDoIntersect(point a, point b, point c, point d)
{
	double da, db, dc, dd; // Directions

	da = direction(c, d, a); // Left or right turn when walking c to d to a
	db = direction(c, d, b); // Left or right turn when walking c to d to b
	dc = direction(a, b, c); // Were not talking about circuits here
	dd = direction(a, b, d); // Left or right turn when walking a to b to d	

	if ((((da > 0) && (db < 0)) || ((da < 0) && (db > 0))) &&
		(((dc > 0) && (dd < 0)) || ((dc < 0) && (dd > 0))))
	{
		return true;
	}

	// Check the no turn cases
	if ((da == 0) && onSegment(c, d, a)) // a is on line cd
	{
		return true;
	}
	if ((db == 0) && onSegment(c, d, b)) // b is on line cd
	{
		return true;
	}
	if ((dc == 0) && onSegment(a, b, c)) // c is on line ab
	{
		return true;
	}
	if ((dd == 0) && onSegment(a, b, d)) // d is on line ab
	{
		return true;
	}

	// Return (0,0) if no intersect is found
	return false;
}

// bool isConvex()
//
// Description: Returns true if the given polygon is convex
// 
// Reference:  Modified from the code found at
// https://stackoverflow.com/questions/471962/how-do-determine-if-a-polygon-is-complex-convex-nonconvex
//
// Params:
// 		Input:	vector<point> verticies - the polygon's verticies as a std::vector
//
// 	Return: If the polygon is convex, return true, else return false
bool isConvex(vector<point> verticies)
{
	// Triangles and lines are convex
	if (verticies.size() < 4)
	{
		return true;
	}

	bool sign = false;  // Negative
	int n = verticies.size(); // Number of points

	for(int i = 0; i < n; i++)
	{
		vctr d1, d2;
		d1.x = verticies[(i+2) % n].x - verticies[(i + 1) % n].x;
		d1.y = verticies[(i+2) % n].y - verticies[(i + 1) % n].y;
		d2.x = verticies[ (i)  % n].x - verticies[(i + 1) % n].x;
		d2.y = verticies[ (i)  % n].y - verticies[(i + 1) % n].y;

		double cp = crossProduct(d1, d2);

		if (cp == 0)
		{
			vector<point> temp = verticies;
			temp.erase(temp.begin() + (i+1));
			cout << "Removed point (" << verticies[(i+1) % n].x << ", " << verticies[(i+1) % n].y << ")" << endl;
			return isConvex(temp);
		}

		if(i == 0)
		{
			sign = cp > 0;
		}
		else
		{
			if (sign != (cp > 0))
			{
				return false;
			}
		}
	}
	return true;
}

// bool onSegment()
//
// Description: Returns true if point c is on the segment from a to b, false
// 		otherwise.  This algorithm is as found in Chapter 33 of CLRS Algorithms
//
// Params:
// 		Input:	point a - one end point on segment AB
// 				point b - the other end point on segment AB
// 				point c - the point to test
//
// 	Return: A bool, true if c is on segment AB, false otherwise
bool onSegment(point a, point b, point c)
{
	if (((fmin(a.x, b.x) <= c.x) && (c.x <= fmax(a.x, b.x))) &&
		((fmin(a.y, b.y) <= c.y) && (c.y <= fmax(a.y, b.y))))
		return true;

	return false;
}

// Terrain function prototypes
bool inTerrain(int x, int y);
void generateTerrain();
void smoothTerrain(int x1, int y1, int x2, int y2);
void clearTerrain();

// Terrain Variables
int terrainScale = 1001;
int xMin = 0, xMax = terrainScale, yMin = 200, yMax = 800;
int terrainX[21] = {};
int terrainY[1001] = {};
int climbMax = 10;

/*
 * Function: inTerrain
 *
 * Authors: Elliott Rarden & Katie Macmillan
 *
 * Description: Returns wether or not a given point is in the terrain
 *
 * Parameters:
 *		[in] x - The x location of the point (in world coordinates)
 * 		[in] y - The y location of the point (in world coordinates)
 *
 * Returns: [boolean] true/false if the point is in the terrain
 */
bool inTerrain(int x, int y){
	if(y <= terrainY[x])
	{
		return true;
	}

	return false;
}

/*
 * Function: clearTerrain
 *
 * Authors: Elliott Rarden & Katie Macmillan
 *
 * Description: Resets the terrain array so we don't draw garbage on new game
 */
void clearTerrain(){
	for(int i = 0; i < 21; i++){
		terrainX[i] = 0;
		terrainY[i] = 0;
	}
	for (int i = 21; i < terrainScale; i++){
		terrainY[i] = 0;
	}
}

/*
 * Function: generateTerrain
 *
 * Authors: Elliott Rarden & Katie Macmillan
 *
 * Description: Generate the terrain
 */
void generateTerrain (){
	int xIncr = (int)(xMax/21);
	int yIncr = (int)(yMax/100);
	int i;

	// Set up random nums
	srand(time(NULL));

	terrainX[0] = xMin;
	terrainX[20] = xMax;
	for (i = 1; i < 20; i++)
	{
		terrainX[i] = (i - 1) * xIncr + (int)(rand() % xIncr);
	}
	terrainY[ terrainX[0] ] = yMin + (int)(rand() % (20));
	terrainY[ terrainX[20] ] = yMin + (int)(rand() % (20));
	for(i = 1; i < 4; i++)
	{
		terrainY[ terrainX[i] ] = terrainY[ terrainX[i-1] ] + (int)(rand() % (i * 10));
		terrainY[ terrainX[20-i] ] = terrainY[ terrainX[20-i+1] ] + (int)(rand() % (i * 10));
	}
	for (i = 4; i < 9; i++)
	{
		terrainY[ terrainX[i] ] = terrainY[ terrainX[i-1] ] + (int)(rand() % (i * 20));
		terrainY[ terrainX[20-i] ] = terrainY[ terrainX[20-i+1] ] + (int)(rand() % (i * 20));
	}
	for (i = 9; i < 12; ++i)
	{
		terrainY[ terrainX[i] ] = terrainY[ terrainX[8] ] + (int)(rand() % (i*40));
	}

	for(i = 0; i < 20; i++)
	{
		smoothTerrain(terrainX[i],terrainY[(int)terrainX[i]], terrainX[i+1], terrainY[(int)terrainX[i+1]]);
	}
	// check parameters of all y coordinates
	for (i = 0; i < terrainScale; i++)
	{
		// if a y coordinate got skipped during smoothing, set it to previous
		if ((terrainY[i] == 0) && (i > 0))
		{
			terrainY[i] = terrainY[i-1];
		}
		// if a y coordinate is lower than yMin, set to yMin
		else if (terrainY[i] < yMin)
		{
			terrainY[i] = yMin;
		}
		// if a y coordinate exceeds yMax, set to yMax
		else if (terrainY[i] > yMax)
		{
			terrainY[i] = yMax;
		}
	}
}

/*
 * Function: inTerrain
 *
 * Authors: Elliott Rarden & Katie Macmillan
 *
 */
void smoothTerrain(int x1, int y1, int x2, int y2){
	//seed random
	srand(time(NULL));
	// calculate deltaX and deltaY
	int dX = x2 - x1;
	int dY = abs(y2 - y1);

	//if deltaX or deltaY is 1 return
	if((dX == 1) || (dY == 1)){
		return;
	}
	else {
		//get random offset
		int offset = rand()%2;
		// find floor midpoint of line segment
		int xM = x1 + floor(dX/2);
		int yM = floor(dY/2);
		if (y1 < y2) {
			yM = y1 + yM;
		} else {
			yM = y1 - yM;
		}
		// get random positive or negative for offset
		if (rand()%2 == 1){
			offset = offset*-1;
		}
		// set y value for x coordinate based on line segment slope
		terrainY[xM] = yM + offset;
		// split new line segment and smooth each side
		smoothTerrain(x1, y1, xM, yM);
		smoothTerrain(xM, yM, x2, y2);

	}
}

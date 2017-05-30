// Color vectors
const float White[3] = {1.0, 1.0, 1.0};
const float Red[3] = {1.0, 0.0, 0.0};
const float Green[3] = {0.0, 1.0, 0.0};
const float Blue[3] = {0.0, 0.0, 1.0};
const float Teal[3] = {0.0, 0.8, 1.0};
const float PowerGreen[3] = {0.0, 1.0, 0.4};
const float Yellow[3] = {1.0, 0.8, 0.0};
const float Orange[3] = {1.0, 0.5, 0.0};
const float FireYellow[3] = {1.0, 1.0, 0.2};
const float BrightOrange[3] = {1.0, 0.2, 0.2};
const float FireRed[3] = {1.0, 0.5, 0.2};

// List of names for each color scheme
const char *colorNames[10] = {"Red", "Green", "Blue", "Yellow", "Aqua", "Magenta",
"Dark Red","Dark Green","Dark Blue", "Greyscale"};

// The colour scheme arrays
const int numColourSchemes = 10;
float colorBook[10][101][3] = {
	// ColorMap0 - red
	{
		{   1,    0,    0},
		{0.95,    0,    0},
		{0.90,    0,    0},
		{0.85,    0,    0},
		{0.80,    0,    0},
		{0.75,    0,    0},
		{0.70,    0,    0},
		{0.65,    0,    0},
		{0.60,    0,    0},
		{0.55,    0,    0},
		{0.50,    0,     0},
		{0.45,    0,     0},
		{0.40,    0,     0},
		{0.35,    0,     0},
		{0.30,    0,     0},
		{0.25,    0,     0},
		{0.20,    0,     0},
		{0.15,    0,     0},
		{0.10,    0,     0},
		{0.05,    0,     0},
		{   1,    1,     0},
		{0.95, 0.95,    0},
		{0.90, 0.90,    0},
		{0.85, 0.85,    0},
		{0.80, 0.80,    0},
		{0.75, 0.75,    0},
		{0.70, 0.70,    0},
		{0.65, 0.65,    0},
		{0.60, 0.60,    0},
		{0.55, 0.55,    0},
		{0.50, 0.50,    0},
		{0.45, 0.45,    0},
		{0.40, 0.40,    0},
		{0.35, 0.35,    0},
		{0.30, 0.30,    0},
		{0.25, 0.25,    0},
		{0.20, 0.20,    0},
		{0.15, 0.15,    0},
		{0.10, 0.10,    0},
		{0.05, 0.05,    0},
		{   0,    1,    0},
		{   0, 0.95,    0},
		{   0, 0.90,    0},
		{   0, 0.85,    0},
		{   0, 0.80,    0},
		{   0, 0.75,    0},
		{   0, 0.70,    0},
		{   0, 0.65,    0},
		{   0, 0.60,    0},
		{   0, 0.55,    0},
		{   0, 0.50,    0},
		{   0, 0.45,    0},
		{   0, 0.40,    0},
		{   0, 0.35,    0},
		{   0, 0.30,    0},
		{   0, 0.25,    0},
		{   0, 0.20,    0},
		{   0, 0.15,    0},
		{   0, 0.10,    0},
		{   0, 0.05,    0},
		{   0,    1,    1},
		{   0, 0.95, 0.95},
		{   0, 0.90, 0.90},
		{   0, 0.85, 0.85},
		{   0, 0.80, 0.80},
		{   0, 0.75, 0.75},
		{   0, 0.70, 0.70},
		{   0, 0.65, 0.65},
		{   0, 0.60, 0.60},
		{   0, 0.55, 0.55},
		{   0, 0.50, 0.50},
		{   0, 0.45, 0.45},
		{   0, 0.40, 0.40},
		{   0, 0.35, 0.35},
		{   0, 0.30, 0.30},
		{   0, 0.25, 0.25},
		{   0, 0.20, 0.20},
		{   0, 0.15, 0.15},
		{   0, 0.10, 0.10},
		{   0, 0.05, 0.05},
		{   0,    0,    1},
		{   0,    0, 0.95},
		{   0,    0, 0.90},
		{   0,    0, 0.85},
		{   0,    0, 0.80},
		{   0,    0, 0.75},
		{   0,    0, 0.70},
		{   0,    0, 0.65},
		{   0,    0, 0.60},
		{   0,    0, 0.55},
		{   0,    0, 0.50},
		{   0,    0, 0.45},
		{   0,    0, 0.40},
		{   0,    0, 0.35},
		{   0,    0, 0.30},
		{   0,    0, 0.25},
		{   0,    0, 0.20},
		{   0,    0, 0.15},
		{   0,    0, 0.10},
		{   0,    0, 0.05},
		{0,0,0}
	},
	// ColorMap1 - green
	{
		{   0,    1,    0},
		{   0, 0.95,    0},
		{   0, 0.90,    0},
		{   0, 0.85,    0},
		{   0, 0.80,    0},
		{   0, 0.75,    0},
		{   0, 0.70,    0},
		{   0, 0.65,    0},
		{   0, 0.60,    0},
		{   0, 0.55,    0},
		{   0, 0.50,    0},
		{   0, 0.45,    0},
		{   0, 0.40,    0},
		{   0, 0.35,    0},
		{   0, 0.30,    0},
		{   0, 0.25,    0},
		{   0, 0.20,    0},
		{   0, 0.15,    0},
		{   0, 0.10,    0},
		{   0, 0.05,    0},
		{   0,    1,    1},
		{   0, 0.95, 0.95},
		{   0, 0.90, 0.90},
		{   0, 0.85, 0.85},
		{   0, 0.80, 0.80},
		{   0, 0.75, 0.75},
		{   0, 0.70, 0.70},
		{   0, 0.65, 0.65},
		{   0, 0.60, 0.60},
		{   0, 0.55, 0.55},
		{   0, 0.50, 0.50},
		{   0, 0.45, 0.45},
		{   0, 0.40, 0.40},
		{   0, 0.35, 0.35},
		{   0, 0.30, 0.30},
		{   0, 0.25, 0.25},
		{   0, 0.20, 0.20},
		{   0, 0.15, 0.15},
		{   0, 0.10, 0.10},
		{   0, 0.05, 0.05},
		{   0,    0,    1},
		{   0,    0, 0.95},
		{   0,    0, 0.90},
		{   0,    0, 0.85},
		{   0,    0, 0.80},
		{   0,    0, 0.75},
		{   0,    0, 0.70},
		{   0,    0, 0.65},
		{   0,    0, 0.60},
		{   0,    0, 0.55},
		{   0,    0, 0.50},
		{   0,    0, 0.45},
		{   0,    0, 0.40},
		{   0,    0, 0.35},
		{   0,    0, 0.30},
		{   0,    0, 0.25},
		{   0,    0, 0.20},
		{   0,    0, 0.15},
		{   0,    0, 0.10},
		{   0,    0, 0.05},
		{   1,    0,    1},
		{0.95,    0, 0.95},
		{0.90,    0, 0.90},
		{0.85,    0, 0.85},
		{0.80,    0, 0.80},
		{0.75,    0, 0.75},
		{0.70,    0, 0.70},
		{0.65,    0, 0.65},
		{0.60,    0, 0.60},
		{0.55,    0, 0.55},
		{0.50,    0, 0.50},
		{0.45,    0, 0.45},
		{0.40,    0, 0.40},
		{0.35,    0, 0.35},
		{0.30,    0, 0.30},
		{0.25,    0, 0.25},
		{0.20,    0, 0.20},
		{0.15,    0, 0.15},
		{0.10,    0, 0.10},
		{0.05,    0, 0.05},
		{   1,    0,    0},
		{0.95,    0,    0},
		{0.90,    0,    0},
		{0.85,    0,    0},
		{0.80,    0,    0},
		{0.75,    0,    0},
		{0.70,    0,    0},
		{0.65,    0,    0},
		{0.60,    0,    0},
		{0.55,    0,    0},
		{0.50,    0,    0},
		{0.45,    0,    0},
		{0.40,    0,    0},
		{0.35,    0,    0},
		{0.30,    0,    0},
		{0.25,    0,    0},
		{0.20,    0,    0},
		{0.15,    0,    0},
		{0.10,    0,    0},
		{0.05,    0,    0},
		{0,0,0}
	},
	// ColorMap2 - blue
	{
		{   0,    0,    1},
		{   0,    0, 0.95},
		{   0,    0, 0.90},
		{   0,    0, 0.85},
		{   0,    0, 0.80},
		{   0,    0, 0.75},
		{   0,    0, 0.70},
		{   0,    0, 0.65},
		{   0,    0, 0.60},
		{   0,    0, 0.55},
		{   0,    0, 0.50},
		{   0,    0, 0.45},
		{   0,    0, 0.40},
		{   0,    0, 0.35},
		{   0,    0, 0.30},
		{   0,    0, 0.25},
		{   0,    0, 0.20},
		{   0,    0, 0.15},
		{   0,    0, 0.10},
		{   0,    0, 0.05},
		{   1,    0,    1},
		{0.95,    0, 0.95},
		{0.90,    0, 0.90},
		{0.85,    0, 0.85},
		{0.80,    0, 0.80},
		{0.75,    0, 0.75},
		{0.70,    0, 0.70},
		{0.65,    0, 0.65},
		{0.60,    0, 0.60},
		{0.55,    0, 0.55},
		{0.50,    0, 0.50},
		{0.45,    0, 0.45},
		{0.40,    0, 0.40},
		{0.35,    0, 0.35},
		{0.30,    0, 0.30},
		{0.25,    0, 0.25},
		{0.20,    0, 0.20},
		{0.15,    0, 0.15},
		{0.10,    0, 0.10},
		{0.05,    0, 0.05},
		{   1,    0,    0},
		{0.95,    0,    0},
		{0.90,    0,    0},
		{0.85,    0,    0},
		{0.80,    0,    0},
		{0.75,    0,    0},
		{0.70,    0,    0},
		{0.65,    0,    0},
		{0.60,    0,    0},
		{0.55,    0,    0},
		{0.50,    0,    0},
		{0.45,    0,    0},
		{0.40,    0,    0},
		{0.35,    0,    0},
		{0.30,    0,    0},
		{0.25,    0,    0},
		{0.20,    0,    0},
		{0.15,    0,    0},
		{0.10,    0,    0},
		{0.05,    0,    0},
		{   1,    1,    0},
		{0.95, 0.95,    0},
		{0.90, 0.90,    0},
		{0.85, 0.85,    0},
		{0.80, 0.80,    0},
		{0.75, 0.75,    0},
		{0.70, 0.70,    0},
		{0.65, 0.65,    0},
		{0.60, 0.60,    0},
		{0.55, 0.55,    0},
		{0.50, 0.50,    0},
		{0.45, 0.45,    0},
		{0.40, 0.40,    0},
		{0.35, 0.35,    0},
		{0.30, 0.30,    0},
		{0.25, 0.25,    0},
		{0.20, 0.20,    0},
		{0.15, 0.15,    0},
		{0.10, 0.10,    0},
		{0.05, 0.05,    0},
		{   0,    1,    0},
		{   0, 0.95,    0},
		{   0, 0.90,    0},
		{   0, 0.85,    0},
		{   0, 0.80,    0},
		{   0, 0.75,    0},
		{   0, 0.70,    0},
		{   0, 0.65,    0},
		{   0, 0.60,    0},
		{   0, 0.55,    0},
		{   0, 0.50,    0},
		{   0, 0.45,    0},
		{   0, 0.40,    0},
		{   0, 0.35,    0},
		{   0, 0.30,    0},
		{   0, 0.25,    0},
		{   0, 0.20,    0},
		{   0, 0.15,    0},
		{   0, 0.10,    0},
		{   0, 0.05,    0},
		{0,0,0}
	},
	// ColorMap3 - yellow
	{
		{   1,    1, 0.05},
		{0.95, 0.95,    0},
		{0.90, 0.90,    0},
		{0.85, 0.85,    0},
		{0.80, 0.80,    0},
		{0.75, 0.75,    0},
		{0.70, 0.70,    0},
		{0.65, 0.65,    0},
		{0.60, 0.60,    0},
		{0.55, 0.55,    0},
		{0.50, 0.50,    0},
		{0.45, 0.45,    0},
		{0.40, 0.40,    0},
		{0.35, 0.35,    0},
		{0.30, 0.30,    0},
		{0.25, 0.25,    0},
		{0.20, 0.20,    0},
		{0.15, 0.15,    0},
		{0.10, 0.10,    0},
		{0.05, 0.05,    1},
		{   0,    1,    1},
		{   0, 0.95, 0.95},
		{   0, 0.90, 0.90},
		{   0, 0.85, 0.85},
		{   0, 0.80, 0.80},
		{   0, 0.75, 0.75},
		{   0, 0.70, 0.70},
		{   0, 0.65, 0.65},
		{   0, 0.60, 0.60},
		{   0, 0.55, 0.55},
		{   0, 0.50, 0.50},
		{   0, 0.45, 0.45},
		{   0, 0.40, 0.40},
		{   0, 0.35, 0.35},
		{   0, 0.30, 0.30},
		{   0, 0.25, 0.25},
		{   0, 0.20, 0.20},
		{   0, 0.15, 0.15},
		{   0, 0.10, 0.10},
		{   1, 0.05, 0.05},
		{   1,    0,    0},
		{0.95,    0,    0},
		{0.90,    0,    0},
		{0.85,    0,    0},
		{0.80,    0,    0},
		{0.75,    0,    0},
		{0.70,    0,    0},
		{0.65,    0,    0},
		{0.60,    0,    0},
		{0.55,    0,    5},
		{0.50,    0,    5},
		{0.45,    0,    0},
		{0.40,    0,    0},
		{0.35,    0,    0},
		{0.30,    0,    0},
		{0.25,    0,    0},
		{0.20,    0,    0},
		{0.15,    0,    0},
		{0.10,    0,    0},
		{0.05,    1,    0},
		{   0,    1,    0},
		{   0, 0.95,    0},
		{   0, 0.90,    0},
		{   0, 0.85,    0},
		{   0, 0.80,    0},
		{   0, 0.75,    0},
		{   0, 0.70,    0},
		{   0, 0.65,    0},
		{   0, 0.60,    0},
		{   0, 0.55,    0},
		{   0, 0.50,    0},
		{   0, 0.45,    0},
		{   0, 0.40,    0},
		{   0, 0.35,    0},
		{   0, 0.30,    0},
		{   0, 0.25,    0},
		{   0, 0.20,    0},
		{   0, 0.15,    0},
		{   0, 0.10,    0},
		{   0, 0.05,    1},
		{   0,    0,    1},
		{   0,    0, 0.95},
		{   0,    0, 0.90},
		{   0,    0, 0.85},
		{   0,    0, 0.80},
		{   0,    0, 0.75},
		{   0,    0, 0.70},
		{   0,    0, 0.65},
		{   0,    0, 0.60},
		{   0,    0, 0.55},
		{   0,    0, 0.50},
		{   0,    0, 0.45},
		{   0,    0, 0.40},
		{   0,    0, 0.35},
		{   0,    0, 0.30},
		{   0,    0, 0.25},
		{   0,    0, 0.20},
		{   0,    0, 0.15},
		{   0,    0, 0.10},
		{   1,    1, 0.05},
		{0,0,0}
	},
	// ColorMap4 - aqua
	{
		{0.05,    1,    1},
		{   0, 0.95, 0.95},
		{   0, 0.90, 0.90},
		{   0, 0.85, 0.85},
		{   0, 0.80, 0.80},
		{   0, 0.75, 0.75},
		{   0, 0.70, 0.70},
		{   0, 0.65, 0.65},
		{   0, 0.60, 0.60},
		{   0, 0.55, 0.55},
		{   0, 0.50, 0.50},
		{   0, 0.45, 0.45},
		{   0, 0.40, 0.40},
		{   0, 0.35, 0.35},
		{   0, 0.30, 0.30},
		{   0, 0.25, 0.25},
		{   0, 0.20, 0.20},
		{   0, 0.15, 0.15},
		{   0, 0.10, 0.10},
		{   1, 0.05, 0.05},
		{   1,    0,    1},
		{0.95,    0, 0.95},
		{0.90,    0, 0.90},
		{0.85,    0, 0.85},
		{0.80,    0, 0.80},
		{0.75,    0, 0.75},
		{0.70,    0, 0.70},
		{0.65,    0, 0.65},
		{0.60,    0, 0.60},
		{0.55,    0, 0.55},
		{0.50,    0, 0.50},
		{0.45,    0, 0.45},
		{0.40,    0, 0.40},
		{0.35,    0, 0.35},
		{0.30,    0, 0.30},
		{0.25,    0, 0.25},
		{0.20,    0, 0.20},
		{0.15,    0, 0.15},
		{0.10,    0, 0.10},
		{0.05,    1, 0.05},
		{   0,    1,    0},
		{   0, 0.95,    0},
		{   0, 0.90,    0},
		{   0, 0.85,    0},
		{   0, 0.80,    0},
		{   0, 0.75,    0},
		{   0, 0.70,    0},
		{   0, 0.65,    0},
		{   0, 0.60,    0},
		{   5, 0.55,    0},
		{   5, 0.50,    0},
		{   0, 0.45,    0},
		{   0, 0.40,    0},
		{   0, 0.35,    0},
		{   0, 0.30,    0},
		{   0, 0.25,    0},
		{   0, 0.20,    0},
		{   0, 0.15,    0},
		{   0, 0.10,    0},
		{   0, 0.05,    1},
		{   0,    0,    1},
		{   0,    0, 0.95},
		{   0,    0, 0.90},
		{   0,    0, 0.85},
		{   0,    0, 0.80},
		{   0,    0, 0.75},
		{   0,    0, 0.70},
		{   0,    0, 0.65},
		{   0,    0, 0.60},
		{   0,    0, 0.55},
		{   0,    0, 0.50},
		{   0,    0, 0.45},
		{   0,    0, 0.40},
		{   0,    0, 0.35},
		{   0,    0, 0.30},
		{   0,    0, 0.25},
		{   0,    0, 0.20},
		{   0,    0, 0.15},
		{   0,    0, 0.10},
		{   1,    0, 0.05},
		{   1,    0,    0},
		{0.95,    0,    0},
		{0.90,    0,    0},
		{0.85,    0,    0},
		{0.80,    0,    0},
		{0.75,    0,    0},
		{0.70,    0,    0},
		{0.65,    0,    0},
		{0.60,    0,    0},
		{0.55,    0,    0},
		{0.50,    0,    0},
		{0.45,    0,    0},
		{0.40,    0,    0},
		{0.35,    0,    0},
		{0.30,    0,    0},
		{0.25,    0,    0},
		{0.20,    0,    0},
		{0.15,    0,    0},
		{0.10,    0,    0},
		{0.05,    1,    1},
		{0,0,0}
	},
	// ColorMap5 - magenta
	{
		{   1, 0.05,    1},
		{0.95,    0, 0.95},
		{0.90,    0, 0.90},
		{0.85,    0, 0.85},
		{0.80,    0, 0.80},
		{0.75,    0, 0.75},
		{0.70,    0, 0.70},
		{0.65,    0, 0.65},
		{0.60,    0, 0.60},
		{0.55,    0, 0.55},
		{0.50,    0, 0.50},
		{0.45,    0, 0.45},
		{0.40,    0, 0.40},
		{0.35,    0, 0.35},
		{0.30,    0, 0.30},
		{0.25,    0, 0.25},
		{0.20,    0, 0.20},
		{0.15,    0, 0.15},
		{0.10,    0, 0.10},
		{0.05,    1, 0.05},
		{   1,    1,    0},
		{0.95, 0.95,    0},
		{0.90, 0.90,    0},
		{0.85, 0.85,    0},
		{0.80, 0.80,    0},
		{0.75, 0.75,    0},
		{0.70, 0.70,    0},
		{0.65, 0.65,    0},
		{0.60, 0.60,    0},
		{0.55, 0.55,    0},
		{0.50, 0.50,    0},
		{0.45, 0.45,    0},
		{0.40, 0.40,    0},
		{0.35, 0.35,    0},
		{0.30, 0.30,    0},
		{0.25, 0.25,    0},
		{0.20, 0.20,    0},
		{0.15, 0.15,    0},
		{0.10, 0.10,    0},
		{0.05, 0.05,    1},
		{   0,    0,    1},
		{   0,    0, 0.95},
		{   0,    0, 0.90},
		{   0,    0, 0.85},
		{   0,    0, 0.80},
		{   0,    0, 0.75},
		{   0,    0, 0.70},
		{   0,    0, 0.65},
		{   0,    0, 0.60},
		{   0,    5, 0.55},
		{   0,    5, 0.50},
		{   0,    0, 0.45},
		{   0,    0, 0.40},
		{   0,    0, 0.35},
		{   0,    0, 0.30},
		{   0,    0, 0.25},
		{   0,    0, 0.20},
		{   0,    0, 0.15},
		{   0,    0, 0.10},
		{   1,    0, 0.05},
		{   1,    0,    0},
		{0.95,    0,    0},
		{0.90,    0,    0},
		{0.85,    0,    0},
		{0.80,    0,    0},
		{0.75,    0,    0},
		{0.70,    0,    0},
		{0.65,    0,    0},
		{0.60,    0,    0},
		{0.55,    0,    0},
		{0.50,    0,    0},
		{0.45,    0,    0},
		{0.40,    0,    0},
		{0.35,    0,    0},
		{0.30,    0,    0},
		{0.25,    0,    0},
		{0.20,    0,    0},
		{0.15,    0,    0},
		{0.10,    0,    0},
		{0.05,    1,    0},
		{   0,    1,    0},
		{   0, 0.95,    0},
		{   0, 0.90,    0},
		{   0, 0.85,    0},
		{   0, 0.80,    0},
		{   0, 0.75,    0},
		{   0, 0.70,    0},
		{   0, 0.65,    0},
		{   0, 0.60,    0},
		{   0, 0.55,    0},
		{   0, 0.50,    0},
		{   0, 0.45,    0},
		{   0, 0.40,    0},
		{   0, 0.35,    0},
		{   0, 0.30,    0},
		{   0, 0.25,    0},
		{   0, 0.20,    0},
		{   0, 0.15,    0},
		{   0, 0.10,    0},
		{   1, 0.05,    1},
		{0,0,0}
	},
	// ColorMap6 - dark red
	{
		{0.05,    0,    0 },
		{0.10,    0,    0 },
		{0.15,    0,    0 },
		{0.20,    0,    0 },
		{0.25,    0,    0 },
		{0.30,    0,    0 },
		{0.35,    0,    0 },
		{0.40,    0,    0 },
		{0.45,    0,    0 },
		{0.50,    0,    0 },
		{0.55, 0.05,    0 },
		{0.60, 0.10,    0 },
		{0.65, 0.15,    0 },
		{0.70, 0.20,    0 },
		{0.75, 0.25,    0 },
		{0.80, 0.30,    0 },
		{0.85, 0.35,    0 },
		{0.90, 0.40,    0 },
		{0.95, 0.45,    0 },
		{   1, 0.50,    0 },
		{   1, 0.55, 0.05 },
		{   1, 0.60, 0.10 },
		{   1, 0.65, 0.15 },
		{   1, 0.70, 0.20 },
		{   1, 0.75, 0.25 },
		{   1, 0.80, 0.30 },
		{   1, 0.85, 0.35 },
		{   1, 0.90, 0.40 },
		{   1, 0.95, 0.45 },
		{   1,    1, 0.50 },
		{   1,    1, 0.55 },
		{   1,    1, 0.60 },
		{   1,    1, 0.65 },
		{   1,    1, 0.70 },
		{   1,    1, 0.75 },
		{   1,    1, 0.80 },
		{   1,    1, 0.85 },
		{   1,    1, 0.90 },
		{   1,    1, 0.95 },
		{   1,    1,    1 },
		{0.95,    1,    1 },
		{0.90,    1,    1 },
		{0.85,    1,    1 },
		{0.80,    1,    1 },
		{0.75,    1,    1 },
		{0.70,    1,    1 },
		{0.65,    1,    1 },
		{0.60,    1,    1 },
		{0.55,    1,    1 },
		{0.50,    1,    1 },
		{0.45,    1,    1 },
		{0.40,    1,    1 },
		{0.35,    1,    1 },
		{0.30,    1,    1 },
		{0.25,    1,    1 },
		{0.20,    1,    1 },
		{0.15,    1,    1 },
		{0.10,    1,    1 },
		{0.05,    1,    1 },
		{   0,    1,    1 },
		{0.05, 0.95,    1 },
		{0.10, 0.90,    1 },
		{0.15, 0.85,    1 },
		{0.20, 0.80,    1 },
		{0.25, 0.75,    1 },
		{0.30, 0.70,    1 },
		{0.35, 0.65,    1 },
		{0.40, 0.60,    1 },
		{0.45, 0.55,    1 },
		{0.50, 0.50,    1 },
		{0.55, 0.45,    1 },
		{0.60, 0.40,    1 },
		{0.65, 0.35,    1 },
		{0.70, 0.30,    1 },
		{0.75, 0.25,    1 },
		{0.80, 0.20,    1 },
		{0.85, 0.15,    1 },
		{0.90, 0.10,    1 },
		{0.95, 0.05,    1 },
		{   1,    0,    1 },
		{0.95, 0.05, 0.95 },
		{0.90, 0.10, 0.90 },
		{0.85, 0.15, 0.85 },
		{0.80, 0.20, 0.80 },
		{0.75, 0.25, 0.75 },
		{0.70, 0.30, 0.70 },
		{0.65, 0.35, 0.65 },
		{0.60, 0.40, 0.60 },
		{0.55, 0.45, 0.55 },
		{0.50, 0.50, 0.50 },
		{0.45, 0.55, 0.45 },
		{0.40, 0.60, 0.40 },
		{0.35, 0.65, 0.35 },
		{0.30, 0.70, 0.30 },
		{0.25, 0.75, 0.25 },
		{0.20, 0.80, 0.20 },
		{0.15, 0.85, 0.15 },
		{0.10, 0.90, 0.10 },
		{0.05, 0.95, 0.05 },
		{   0,    1,    0 },
		{0,0,0}
	},
	// ColorMap7 - dark green
	{
		{   0, 0.05, 0},
		{   0, 0.10, 0},
		{   0, 0.15, 0},
		{   0, 0.20, 0},
		{   0, 0.25, 0},
		{   0, 0.30, 0},
		{   0, 0.35, 0},
		{   0, 0.40, 0},
		{   0, 0.45, 0},
		{   0, 0.50, 0},
		{   0, 0.55, 0.05},
		{   0, 0.60, 0.10},
		{   0, 0.65, 0.15},
		{   0, 0.70, 0.20},
		{   0, 0.75, 0.25},
		{   0, 0.80, 0.30},
		{   0, 0.85, 0.35},
		{   0, 0.90, 0.40},
		{   0, 0.95, 0.45},
		{   0,    1, 0.50},
		{0.05,    1, 0.55},
		{0.10,    1, 0.60},
		{0.15,    1, 0.65},
		{0.20,    1, 0.70},
		{0.25,    1, 0.75},
		{0.30,    1, 0.80},
		{0.35,    1, 0.85},
		{0.40,    1, 0.90},
		{0.45,    1, 0.95},
		{0.50,    1, 1},
		{0.55,    1, 1},
		{0.60,    1, 1},
		{0.65,    1, 1},
		{0.70,    1, 1},
		{0.75,    1, 1},
		{0.80,    1, 1},
		{0.85,    1, 1},
		{0.90,    1, 1},
		{0.95,    1, 1},
		{   1,    1, 1},
		{   1, 0.95, 1},
		{   1, 0.90, 1},
		{   1, 0.85, 1},
		{   1, 0.80, 1},
		{   1, 0.75, 1},
		{   1, 0.70, 1},
		{   1, 0.65, 1},
		{   1, 0.60, 1},
		{   1, 0.55, 1},
		{   1, 0.50, 1},
		{   1, 0.45, 1},
		{   1, 0.40, 1},
		{   1, 0.35, 1},
		{   1, 0.30, 1},
		{   1, 0.25, 1},
		{   1, 0.20, 1},
		{   1, 0.15, 1},
		{   1, 0.10, 1},
		{   1, 0.05, 1},
		{   1,    0,  1},
		{   1, 0.05, 0.95},
		{   1, 0.10, 0.90},
		{   1, 0.15, 0.85},
		{   1, 0.20, 0.80},
		{   1, 0.25, 0.75},
		{   1, 0.30, 0.70},
		{   1, 0.35, 0.65},
		{   1, 0.40, 0.60},
		{   1, 0.45, 0.55},
		{   1, 0.50, 0.50},
		{   1, 0.55, 0.45},
		{   1, 0.60, 0.40},
		{   1, 0.65, 0.35},
		{   1, 0.70, 0.30},
		{   1, 0.75, 0.25},
		{   1, 0.80, 0.20},
		{   1, 0.85, 0.15},
		{   1, 0.90, 0.10},
		{   1, 0.95, 0.05},
		{   1,    1, 0},
		{0.95, 0.95, 0.05},
		{0.90, 0.90, 0.10},
		{0.85, 0.85, 0.15},
		{0.80, 0.80, 0.20},
		{0.75, 0.75, 0.25},
		{0.70, 0.70, 0.30},
		{0.65, 0.65, 0.35},
		{0.60, 0.60, 0.40},
		{0.55, 0.55, 0.45},
		{0.50, 0.50, 0.50},
		{0.45, 0.45, 0.55},
		{0.40, 0.40, 0.60},
		{0.35, 0.35, 0.65},
		{0.30, 0.30, 0.70},
		{0.25, 0.25, 0.75},
		{0.20, 0.20, 0.80},
		{0.15, 0.15, 0.85},
		{0.10, 0.10, 0.90},
		{0.05, 0.05, 0.95},
		{   0,    0, 1},
		{0,0,0}
	},
	// ColorMap8 - dark blue 
	{
		{   0,    0, 0.05 },
		{   0,    0, 0.10 },
		{   0,    0, 0.15 },
		{   0,    0, 0.20 },
		{   0,    0, 0.25 },
		{   0,    0, 0.30 },
		{   0,    0, 0.35 },
		{   0,    0, 0.40 },
		{   0,    0, 0.45 },
		{   0,    0, 0.50 },
		{0.05,    0, 0.55 },
		{0.10,    0, 0.60 },
		{0.15,    0, 0.65 },
		{0.20,    0, 0.70 },
		{0.25,    0, 0.75 },
		{0.30,    0, 0.80 },
		{0.35,    0, 0.85 },
		{0.40,    0, 0.90 },
		{0.45,    0, 0.95 },
		{0.50,    0,    1 },
		{0.55, 0.05,    1 },
		{0.60, 0.10,    1 },
		{0.65, 0.15,    1 },
		{0.70, 0.20,    1 },
		{0.75, 0.25,    1 },
		{0.80, 0.30,    1 },
		{0.85, 0.35,    1 },
		{0.90, 0.40,    1 },
		{0.95, 0.45,    1 },
		{   1, 0.50,    1 },
		{   1, 0.55,    1 },
		{   1, 0.60,    1 },
		{   1, 0.65,    1 },
		{   1, 0.70,    1 },
		{   1, 0.75,    1 },
		{   1, 0.80,    1 },
		{   1, 0.85,    1 },
		{   1, 0.90,    1 },
		{   1, 0.95,    1 },
		{   1,    1,    1 },
		{   1,    1, 0.95 },
		{   1,    1, 0.90 },
		{   1,    1, 0.85 },
		{   1,    1, 0.80 },
		{   1,    1, 0.75 },
		{   1,    1, 0.70 },
		{   1,    1, 0.65 },
		{   1,    1, 0.60 },
		{   1,    1, 0.55 },
		{   1,    1, 0.50 },
		{   1,    1, 0.45 },
		{   1,    1, 0.40 },
		{   1,    1, 0.35 },
		{   1,    1, 0.30 },
		{   1,    1, 0.25 },
		{   1,    1, 0.20 },
		{   1,    1, 0.15 },
		{   1,    1, 0.10 },
		{   1,    1, 0.05 },
		{   1,    1,    0 },
		{0.95,    1, 0.05 },
		{0.90,    1, 0.10 },
		{0.85,    1, 0.15 },
		{0.80,    1, 0.20 },
		{0.75,    1, 0.25 },
		{0.70,    1, 0.30 },
		{0.65,    1, 0.35 },
		{0.60,    1, 0.40 },
		{0.55,    1, 0.45 },
		{0.50,    1, 0.50 },
		{0.45,    1, 0.55 },
		{0.40,    1, 0.60 },
		{0.35,    1, 0.65 },
		{0.30,    1, 0.70 },
		{0.25,    1, 0.75 },
		{0.20,    1, 0.80 },
		{0.15,    1, 0.85 },
		{0.10,    1, 0.90 },
		{0.05,    1, 0.95 },
		{   0,    1,    1 },
		{ 0.05, 0.95, 0.95 },
		{ 0.10, 0.90, 0.90 },
		{ 0.15, 0.85, 0.85 },
		{ 0.20, 0.80, 0.80 },
		{ 0.25, 0.75, 0.75 },
		{ 0.30, 0.70, 0.70 },
		{ 0.35, 0.65, 0.65 },
		{ 0.40, 0.60, 0.60 },
		{ 0.45, 0.55, 0.55 },
		{ 0.50, 0.50, 0.50 },
		{ 0.55, 0.45, 0.45 },
		{ 0.60, 0.40, 0.40 },
		{ 0.65, 0.35, 0.35 },
		{ 0.70, 0.30, 0.30 },
		{ 0.75, 0.25, 0.25 },
		{ 0.80, 0.20, 0.20 },
		{ 0.85, 0.15, 0.15 },
		{ 0.90, 0.10, 0.10 },
		{ 0.95, 0.05, 0.05 },
		{    1,    0,    0 },
		{0,0,0}
	},
	// ColorMap9 - GreyScale
	{
		{ 0.01, 0.01, 0.01 },
		{ 0.02, 0.02, 0.02 },
		{ 0.03, 0.03, 0.03 },
		{ 0.04, 0.04, 0.04 },
		{ 0.05, 0.05, 0.05 },
		{ 0.06, 0.06, 0.06 },
		{ 0.07, 0.07, 0.07 },
		{ 0.08, 0.08, 0.08 },
		{ 0.09, 0.09, 0.09 },
		{ 0.10, 0.10, 0.10 },
		{ 0.11, 0.11, 0.11 },
		{ 0.12, 0.12, 0.12 },
		{ 0.13, 0.13, 0.13 },
		{ 0.14, 0.14, 0.14 },
		{ 0.15, 0.15, 0.15 },
		{ 0.16, 0.16, 0.16 },
		{ 0.17, 0.17, 0.17 },
		{ 0.18, 0.18, 0.18 },
		{ 0.19, 0.19, 0.19 },
		{ 0.20, 0.20, 0.20 },
		{ 0.21, 0.21, 0.21 },
		{ 0.22, 0.22, 0.22 },
		{ 0.23, 0.23, 0.23 },
		{ 0.24, 0.24, 0.24 },
		{ 0.25, 0.25, 0.25 },
		{ 0.26, 0.26, 0.26 },
		{ 0.27, 0.27, 0.27 },
		{ 0.28, 0.28, 0.28 },
		{ 0.29, 0.29, 0.29 },
		{ 0.30, 0.30, 0.30 },
		{ 0.31, 0.31, 0.31 },
		{ 0.32, 0.32, 0.32 },
		{ 0.33, 0.33, 0.33 },
		{ 0.34, 0.34, 0.34 },
		{ 0.35, 0.35, 0.35 },
		{ 0.36, 0.36, 0.36 },
		{ 0.37, 0.37, 0.37 },
		{ 0.38, 0.38, 0.38 },
		{ 0.39, 0.39, 0.39 },
		{ 0.40, 0.40, 0.40 },
		{ 0.41, 0.41, 0.41 },
		{ 0.42, 0.42, 0.42 },
		{ 0.43, 0.43, 0.43 },
		{ 0.44, 0.44, 0.44 },
		{ 0.45, 0.45, 0.45 },
		{ 0.46, 0.46, 0.46 },
		{ 0.47, 0.47, 0.47 },
		{ 0.48, 0.48, 0.48 },
		{ 0.49, 0.49, 0.49 },
		{ 0.50, 0.50, 0.50 },
		{ 0.51, 0.51, 0.51 },
		{ 0.52, 0.52, 0.52 },
		{ 0.53, 0.53, 0.53 },
		{ 0.54, 0.54, 0.54 },
		{ 0.55, 0.55, 0.55 },
		{ 0.56, 0.56, 0.56 },
		{ 0.57, 0.57, 0.57 },
		{ 0.58, 0.58, 0.58 },
		{ 0.59, 0.59, 0.59 },
		{ 0.60, 0.60, 0.60 },
		{ 0.61, 0.61, 0.61 },
		{ 0.62, 0.62, 0.62 },
		{ 0.63, 0.63, 0.63 },
		{ 0.64, 0.64, 0.64 },
		{ 0.65, 0.65, 0.65 },
		{ 0.66, 0.66, 0.66 },
		{ 0.67, 0.67, 0.67 },
		{ 0.68, 0.68, 0.68 },
		{ 0.69, 0.69, 0.69 },
		{ 0.70, 0.70, 0.70 },
		{ 0.71, 0.71, 0.71 },
		{ 0.72, 0.72, 0.72 },
		{ 0.73, 0.73, 0.73 },
		{ 0.74, 0.74, 0.74 },
		{ 0.75, 0.75, 0.75 },
		{ 0.76, 0.76, 0.76 },
		{ 0.77, 0.77, 0.77 },
		{ 0.78, 0.78, 0.78 },
		{ 0.79, 0.79, 0.79 },
		{ 0.80, 0.80, 0.80 },
		{ 0.81, 0.81, 0.81 },
		{ 0.82, 0.82, 0.82 },
		{ 0.83, 0.83, 0.83 },
		{ 0.84, 0.84, 0.84 },
		{ 0.85, 0.85, 0.85 },
		{ 0.86, 0.86, 0.86 },
		{ 0.87, 0.87, 0.87 },
		{ 0.88, 0.88, 0.88 },
		{ 0.89, 0.89, 0.89 },
		{ 0.90, 0.90, 0.90 },
		{ 0.91, 0.91, 0.91 },
		{ 0.92, 0.92, 0.92 },
		{ 0.93, 0.93, 0.93 },
		{ 0.94, 0.94, 0.94 },
		{ 0.95, 0.95, 0.95 },
		{ 0.96, 0.96, 0.96 },
		{ 0.97, 0.97, 0.97 },
		{ 0.98, 0.98, 0.98 },
		{ 0.99, 0.99, 0.99 },
		{0,0,0}
	}
};

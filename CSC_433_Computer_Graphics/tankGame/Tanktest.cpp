#include "tank.h"
#include <stdio.h>

int main(int argc, char const *argv[])
{
	// Create a tank
	tank tnk;

	tank_position pos;
	pos.x = 35;
	pos.y = 50;
	tnk.setPosition(pos);

	tank_color clr;
	clr.red = 0;
	clr.grn = 38;
	clr.blu = 249;
	tnk.setColor(clr);

	tnk.setAngle(35);
	tnk.setPower(80);

	tnk.debuginfo();

	tnk.takeHit();
	tnk.takeHit();

	return 0;
}

#include "Wall.h"
#include <cmath>

Wall::Wall()
	:ax(0), az(0), bx(0), bz(0), dir(0, 0)
{
	dirX = dirZ = maxT = 0.0;
}

Wall::Wall(GLdouble ax, GLdouble az, GLdouble bx, GLdouble bz)
	: ax(ax), az(az), bx(bx), bz(bz)
{
	dirX = bx - ax;
	dirZ = bz - az;
	maxT = sqrt(dirX * dirX + dirZ * dirZ);
	// Normalize the direction vector (dirX, dirZ)
	dirX /= maxT;
	dirZ /= maxT;
	dir = Vector2d(dirX, dirZ);
	dir.normalize();
}

Wall::~Wall()
{
}

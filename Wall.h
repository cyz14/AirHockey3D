#pragma once
// Wall of table in X-Z plane
#include <Windows.h>
#include <GL\GL.h>
#include <GL\GLU.h>
#include "Vector2d.h"

class Wall
{
	// Parameters to represent the segment
	GLdouble maxT;
	GLdouble dirX, dirZ; // Direction vector
public:
	Wall();
	Wall(GLdouble ax, GLdouble az, GLdouble bx, GLdouble bz);
	~Wall();

	// Two end points of the wall
	GLdouble ax, az, bx, bz;
	Vector2d dir;
	GLdouble getDirX() { return dirX; }
	GLdouble getDirZ() { return dirZ; }
	GLdouble getMaxT() { return maxT; }
	Vector2d getDir()  { return dir;  }
};


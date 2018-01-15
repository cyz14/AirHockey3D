#pragma once
#include <Windows.h>
#include <GL\GL.h>
#include <GL\GLU.h>
#include <vector>
#include "Mallet.h"
#include "Wall.h"

#define NORM(x, y) sqrt((x) * (x)+(y)*(y))

class Puck
{
	GLfloat x, y, z;
	GLfloat r, g, b;
	GLfloat radius;
	GLfloat height;
	GLfloat dx, dz;
	GLUquadricObj* objCylinder, *objDisk;
	std::vector < Mallet* > mallets;
	std::vector < Wall* > walls, goals;

	GLint SLICES_NUMBER = 20, STACKS_NUMBER = 10;
	GLfloat SPEED = 0.03f;
	const GLdouble EPS = 1e-6;

public:
	Puck();
	~Puck();

	void draw();
	int move();
	GLfloat getX() { return x; }
	GLfloat getY() { return y; }
	GLfloat getZ() { return z; }
	GLfloat getRadius() { return radius; }
	GLfloat getSpeed() { return NORM(dx, dz) / 16; }
	Vector2d getMoveDirection() { return Vector2d(dx, dz).getUnitVector(); }
	
	void setPosition(GLfloat ax, GLfloat ay, GLfloat az);
	void setParameter(GLfloat aRadius, GLfloat aHeight);
	void setColor(GLdouble a_r, GLdouble a_g, GLdouble a_b);
	void resetDirection();

	void addMallet(Mallet* mallet);
	void addWall(Wall* wall);
	void addGoal(Wall* goal);

	bool collide(Wall* w);
	void reflect(Wall* w);
};


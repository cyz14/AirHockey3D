#pragma once
#include <Windows.h>
#include <GL\GL.h>
#include <GL\GLU.h>

class Puck
{
	GLfloat x, y, z;
	GLfloat r, g, b;
	GLfloat radius;
	GLfloat height;
	GLUquadricObj* objCylinder, *objDisk;

	GLint SLICES_NUMBER = 20, STACKS_NUMBER = 10;

public:
	Puck();
	~Puck();

	void draw();
	GLfloat getX() { return x; }
	GLfloat getY() { return y; }
	GLfloat getZ() { return z; }
	GLfloat getRadius() { return radius; }
	void setPosition(GLfloat ax, GLfloat ay, GLfloat az);
	void setParameter(GLfloat aRadius, GLfloat aHeight);
	void setColor(GLdouble a_r, GLdouble a_g, GLdouble a_b);
};


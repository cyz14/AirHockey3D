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
	GLfloat dx, dz;
	GLUquadricObj* objCylinder, *objDisk;

	GLint SLICES_NUMBER = 20, STACKS_NUMBER = 10;
	GLfloat SPEED = 0.03f;

public:
	Puck();
	~Puck();

	void draw();
	void move();
	GLfloat getX() { return x; }
	GLfloat getY() { return y; }
	GLfloat getZ() { return z; }
	GLfloat getRadius() { return radius; }
	void setPosition(GLfloat ax, GLfloat ay, GLfloat az);
	void setParameter(GLfloat aRadius, GLfloat aHeight);
	void setColor(GLdouble a_r, GLdouble a_g, GLdouble a_b);
};


#pragma once
#include <Windows.h>
#include <GL\GL.h>

class Puck
{
	GLfloat x, y, z;
	GLfloat r, g, b;
	GLfloat radius;
	GLfloat height;

public:
	Puck();
	~Puck();

	void draw();
	GLfloat getX() { return x; }
	GLfloat getY() { return y; }
	GLfloat getZ() { return z; }
	GLfloat getRadius() { return radius; }
	void setPosition(GLfloat ax, GLfloat ay, GLfloat az);
};


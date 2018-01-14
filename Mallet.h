#pragma once
#include <Windows.h>
#include <GL\GL.h>
#include <GL\GLU.h>
class Mallet
{
	GLfloat x, y, z;
	GLfloat radius;
	GLfloat height;
	GLdouble r, g, b;
	GLUquadricObj* objCylinder, *objDisk;

    GLint SLICES_NUMBER = 20, STACKS_NUMBER = 10;

public:
	Mallet();
	Mallet(GLfloat a_y);
	Mallet(GLfloat a_x, GLfloat a_y, GLfloat a_z);
	~Mallet();

	void setParameter(GLfloat aRadius, GLfloat aHeight);
	void setPosition(GLfloat a_x, GLfloat a_z);
	void setPosition(GLfloat a_x, GLfloat a_y, GLfloat a_z);
	void setColor(GLdouble a_r, GLdouble a_g, GLdouble a_b);
	void draw();
	void update(GLfloat newx, GLfloat newz, GLfloat px, GLfloat pz, GLfloat pr);
};


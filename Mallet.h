#pragma once
#include <Windows.h>
#include <GL\GL.h>
#include <GL\GLU.h>
class Mallet
{
	GLfloat x, y, z;
	GLfloat radius;
	GLfloat height;
	GLubyte r, g, b;
	GLUquadricObj* objCylinder, *objDisk0, *objDisk1;
public:
	Mallet();
	Mallet(GLfloat a_x, GLfloat a_y, GLfloat a_z);
	~Mallet();

	void setPosition(GLfloat a_x, GLfloat a_y, GLfloat a_z);
	void setColor(GLubyte a_r, GLubyte a_g, GLubyte a_b);
	void draw();
	void update(GLfloat newx, GLfloat newz, GLfloat px, GLfloat pz, GLfloat pr);
};


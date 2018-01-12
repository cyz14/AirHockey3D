#include "Mallet.h"

int SLICES_NUMBER = 10;
int STACKS_NUMBER = 10;

Mallet::Mallet()
	: x(0.0f), y(0.0f), z(0.0f),
	objCylinder (NULL), objDisk0(NULL), objDisk1(NULL) {}

Mallet::Mallet(GLfloat a_x, GLfloat a_y, GLfloat a_z)
	: x(a_x), y(a_y), z(a_z), objCylinder(NULL), objDisk0(NULL), objDisk1(NULL) {}

Mallet::~Mallet()
{
	if (objCylinder != NULL) gluDeleteQuadric(objCylinder);
	if (objDisk0 != NULL) gluDeleteQuadric(objDisk0);
	if (objDisk1 != NULL) gluDeleteQuadric(objDisk1);
}

void Mallet::setPosition(GLfloat a_x, GLfloat a_y, GLfloat a_z) {
	x = a_x;
	y = a_y;
	z = a_z;
}

void Mallet::setColor(GLubyte a_r, GLubyte a_g, GLubyte a_b) {
	r = a_r;
	g = a_g;
	b = a_b;
}


void Mallet::draw() {
	glMatrixMode(GL_MODELVIEW);
	objCylinder = gluNewQuadric();
	objDisk0 = gluNewQuadric();
	// objDisk1 = gluNewQuadric();
	glColor3b(r, g, b);
	glPushMatrix();
	glNormal3d(0, 1, 0);
	glTranslated(x, y, z);
	gluCylinder(objCylinder, radius, radius, height, SLICES_NUMBER, STACKS_NUMBER);
	gluDisk(objDisk0, 0.0f, radius, SLICES_NUMBER, STACKS_NUMBER);
	glPopMatrix();
}

void Mallet::update(GLfloat newx, GLfloat newz, GLfloat px, GLfloat pz, GLfloat pr) {
	double L2 = (newx - px) * (newx - px) + (newz - pz) * (newz - pz);
	if (L2 < (pr + radius) * (pr + radius)) return;
	x = newx;
	z = newz;
}
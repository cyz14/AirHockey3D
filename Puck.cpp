#include <cmath>
#include "Puck.h"

Puck::Puck()
    : x(0.0f), y(0.0f), z(0.0f), dx(0.0f), dz(0.0f),
    objCylinder(NULL), objDisk(NULL) {
    objCylinder = gluNewQuadric();
    objDisk = gluNewQuadric();
    radius = height = 0.0f;
    r = g = b = 0.3;
	dz = -SPEED;
}


Puck::~Puck() {
    if (objCylinder != NULL) gluDeleteQuadric(objCylinder);
    if (objDisk != NULL) gluDeleteQuadric(objDisk);
}

void Puck::draw() {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
        glColor3d(r, g, b);
        glTranslated(x, y, z);
        glNormal3d(0, 1, 0);
        glRotated(90, 1, 0, 0);
        gluCylinder(objCylinder, radius, radius, height, SLICES_NUMBER, STACKS_NUMBER);
        gluDisk(objDisk, 0.0f, radius, SLICES_NUMBER, STACKS_NUMBER);
    glPopMatrix();
}

void Puck::move() {
	// Collision detection with mallets
	for (Mallet *m : mallets) {
		GLfloat mx = m->getX();
		GLfloat mz = m->getZ();
		GLfloat mradius = m->getRadius();
		GLfloat L2 = (mx - x) * (mx - x) + (mz - z) * (mz - z);
		if (L2 < (radius + mradius) * (radius + mradius)) {
			dx = x - mx;
			dz = z - mz;
			dx = dx * SPEED / sqrt(L2);
			dz = dz * SPEED / sqrt(L2);
		}
	}

	// Move on
    x += dx;
    z += dz;
}

void Puck::setPosition(GLfloat ax, GLfloat ay, GLfloat az) {
    x = ax;
    y = ay + height;
    z = az;
}

void Puck::setParameter(GLfloat aRadius, GLfloat aHeight) {
    radius = aRadius;
    height = aHeight;
}

void Puck::setColor(GLdouble a_r, GLdouble a_g, GLdouble a_b) {
    r = a_r;
    g = a_g;
    b = a_b;
}

void Puck::addMallet(Mallet *mallet) {
	mallets.push_back(mallet);
}

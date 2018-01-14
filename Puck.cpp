#include "Puck.h"

Puck::Puck()
    : x(0.0f), y(0.0f), z(0.0f),
    objCylinder(NULL), objDisk(NULL) {
    objCylinder = gluNewQuadric();
    objDisk = gluNewQuadric();
    radius = height = 0.0f;
    r = g = b = 0.3;
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

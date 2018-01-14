#include <cmath>
#include "Puck.h"
#include "Vector2d.h"

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

	// Collision detection with walls
	for (Wall *w : walls) {
		if (this->collide(w)) {
			this->reflect(w);
			break;
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

void Puck::addWall(Wall* wall) {
	walls.push_back(wall);
}

bool Puck::collide(Wall* w) {
	GLdouble px = w->ax - x;
	GLdouble pz = w->az - z;
	GLdouble b = -(px * w->getDirX() + pz * w->getDirZ());
	GLdouble det = b * b - (px * px + pz * pz) + radius * radius;

	if (det > 1e-6) {
		det = sqrt(det);
		GLdouble t1 = b - det, t2 = b + det;
		if (t1 > 1e-6 && t1 < w->getMaxT()) {
			return true;
		}
		if (t2 > 1e-6 && t2 < w->getMaxT()){
			return true;
		}
	}
	return false;
}

void Puck::reflect(Wall* w) {
	Vector2d L = Vector2d(x - w->ax, z - w->az);
	Vector2d N = L - L.dot(w->getDir()) * w->getDir();
	N.normalize();
	double ln = N.dot(w->getDir());
	Vector2d pDir = Vector2d(dx, dz);
	pDir.normalize();
	pDir = pDir - 2 * pDir.dot(N) * N;
	pDir.normalize();
	dx = pDir.getX() * SPEED;
	dz = pDir.getY() * SPEED;
}
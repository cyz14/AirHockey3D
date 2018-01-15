#include "Mallet.h"
#include "Puck.h"

extern const GLfloat MAX_MALLET_X;
extern const GLfloat TABLE_LENGTH;
extern const GLfloat MALLET_DIAMETER;

const GLfloat MAX_AI_X = MAX_MALLET_X;
const GLfloat MIN_AI_X = -MAX_AI_X;
const GLfloat MAX_AI_Z = 0 - MALLET_DIAMETER;
const GLfloat MIN_AI_Z = (-TABLE_LENGTH + MALLET_DIAMETER) / 2;

void aiMove(GLfloat time, Mallet* ai, Puck* puck) {
	Vector2d puckDir = puck->getMoveDirection();
	GLfloat puckSpeed = puck->getSpeed();
	GLfloat step = 0.0f;
	GLfloat x = ai->getX();
	GLfloat z = ai->getZ();

	if (puck->getX() > x) {
		step = puckSpeed * time;
	}
	else
	{
		step = -puckSpeed * time;
	}
	x += step * (rand() % 10) / 10;
	if (x > MAX_AI_X) x = MAX_AI_X;
	if (x < MIN_AI_X) x = MIN_AI_X;
	z += step * (rand() % 10) / 10;
	if (z > MAX_AI_Z) z = MAX_AI_Z;
	if (z < MIN_AI_Z) z = MIN_AI_Z;
	ai->update(x, z, puck->getX(), puck->getZ(), puck->getRadius());
}

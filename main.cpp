
#include "Mallet.h"
#include "Puck.h"
#include "BMP.h"

#include <windows.h>
#include <GL\GL.h>
#include <GL\GLU.h>
#include <GL\glut.h>

#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <ctime>
#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

const GLuint SCREEN_WIDTH = 480;
const GLuint SCREEN_HEIGHT = 480;

extern const GLfloat TABLE_WIDTH = 2.0f;  // x
extern const GLfloat TABLE_HEIGHT = 4.0f; // y
extern const GLfloat TABLE_LENGTH = 4.0f; // z

GLfloat eye[] = { 0.0, TABLE_HEIGHT * 1.5, 6.0 };
GLfloat to[] = { 0.0, 0, -TABLE_LENGTH / 2.0 };
GLfloat up[] = { 0.0f, 3.0f, -1.0f };
GLfloat xAngle = 0.0;
GLfloat yAngle = 0.0;

const GLfloat MAX_X_ANGLE = 30.0;
const GLfloat MIN_X_ANGLE = 0.0;

const GLfloat GOAL_LENGTH = 0.8f;

const GLfloat WALL_THICK = 0.1f;
const GLfloat WALL_HEIGHT = 0.1f;

const GLfloat PUCK_DIAMETER = 0.2f;
const GLfloat PUCK_HEIGHT = 0.1f;

extern const GLfloat MALLET_DIAMETER = 0.3f;
const GLfloat MALLET_HEIGHT = 0.1f;

GLfloat light_ambient[] = {0.5f, 0.5f, 0.5f, 1.0f};
GLfloat light_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat light_position[] = { 0.0f, 2.0f, 3.0f, 0.0f };
GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat mat_ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };
GLfloat mat_diffuse[] = { 0.7f, 0.1f, 0.2f, 1.0f };
GLfloat mat_shininess[] = { 150.0f };

Puck* puck;
Mallet* player;
Mallet* aiPlayer;
GLfloat playerX, playerZ;

// Valid range of mallet's moving area
extern const GLfloat MAX_MALLET_X = (TABLE_WIDTH - MALLET_DIAMETER) / 2;
extern const GLfloat MIN_MALLET_X = - MAX_MALLET_X;

extern const GLfloat MAX_MALLET_Z = (TABLE_LENGTH - MALLET_DIAMETER) / 2;
extern const GLfloat MIN_MALLET_Z = (MALLET_DIAMETER) / 2;

GLuint floorTexture;
BMP* floorBMP;

size_t gameEnd = 0;
const int PLAYER_LOSE = 1;
const int PLAYER_WIN = 2;
const char* winLabel = "You Win!";
const char* loseLabel = "You Lose!";
const char* tryAgainLabel = "Press Enter to try again";

GLfloat wallPoints[12][2] = {
{ TABLE_WIDTH / 2, TABLE_LENGTH / 2},
{ TABLE_WIDTH / 2, -TABLE_LENGTH / 2 },
{ -TABLE_WIDTH / 2, TABLE_LENGTH / 2 },
{ -TABLE_WIDTH / 2, -TABLE_LENGTH / 2},
{ TABLE_WIDTH / 2, TABLE_LENGTH / 2 },
{ GOAL_LENGTH / 2, TABLE_LENGTH / 2 },
{ -TABLE_WIDTH / 2, TABLE_LENGTH / 2 },
{ -GOAL_LENGTH / 2, TABLE_LENGTH / 2 },
{ TABLE_WIDTH / 2, -TABLE_LENGTH / 2 },
{ GOAL_LENGTH / 2, -TABLE_LENGTH / 2 },
{ -TABLE_WIDTH / 2, -TABLE_LENGTH / 2 },
{ -GOAL_LENGTH / 2, -TABLE_LENGTH / 2 },
};

GLfloat goalPoints[4][2] = {
{GOAL_LENGTH / 2, TABLE_LENGTH / 2},
{-GOAL_LENGTH / 2, TABLE_LENGTH / 2},
{GOAL_LENGTH / 2, -(TABLE_LENGTH / 2)},
{-GOAL_LENGTH / 2, -(TABLE_LENGTH / 2)},
};

void gameInit() {
	aiPlayer = new Mallet();
	aiPlayer->setColor(0.7, 0.2, 0.2);
	aiPlayer->setParameter(MALLET_DIAMETER / 2, MALLET_HEIGHT);

	player = new Mallet();
	player->setColor(0.2, 0.7, 0.2);
	player->setParameter(MALLET_DIAMETER / 2, MALLET_HEIGHT);

	puck = new Puck();
	puck->setColor(0.2, 0.2, 0.7);
	puck->setParameter(PUCK_DIAMETER / 2, PUCK_HEIGHT);
	puck->addMallet(player);
	puck->addMallet(aiPlayer);

	for (size_t i = 0; i < 12; i+=2)
	{
		Wall* wall = new Wall(wallPoints[i][0], wallPoints[i][1], wallPoints[i+1][0], wallPoints[i+1][1]);
		puck->addWall(wall);
	}

	for (size_t i = 0; i < 4; i += 2)
	{
		Wall* goal = new Wall(goalPoints[i][0], goalPoints[i][1], goalPoints[i + 1][0], goalPoints[i + 1][1]);
		puck->addGoal(goal);
	}
}

void glInit() {
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glGenTextures(1, &floorTexture);
	floorBMP = new BMP;
	floorBMP->Input("floor.bmp");
	GLubyte * pixels = floorBMP->GetPixels();

	if (pixels != NULL) {
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, floorBMP->GetW(), floorBMP->GetH(), 
			0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
		glShadeModel(GL_FLAT);
	}

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT1);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
}

void gameRestart() {
	gameEnd = 0;
	aiPlayer->setPosition(0, TABLE_HEIGHT / 2, MALLET_DIAMETER - TABLE_LENGTH / 2);
	player->setPosition(0, TABLE_HEIGHT / 2, TABLE_LENGTH / 2 - MALLET_DIAMETER);
	puck->setPosition(0, TABLE_HEIGHT / 2, 0);
	puck->resetDirection();
}

void drawCube(GLfloat x, GLfloat y, GLfloat z) {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glScaled(x, y, z);
	glutSolidCube(1.0);
	glPopMatrix();
}

void drawWall() {
	
	// Left Wall
	glPushMatrix();
	// table wall on the ground
	glColor3d(0.2, 0.2, 0.2);
	glTranslated( (TABLE_WIDTH + WALL_THICK)/2.0, 0, 0);
	drawCube(WALL_THICK, TABLE_HEIGHT, TABLE_LENGTH);
	// table edge on the wall
	glColor3d(0.6, 0.6, 0.6);
	glTranslated(0.0, (TABLE_HEIGHT + WALL_THICK) / 2.0, 0.0);
	drawCube(WALL_THICK, WALL_THICK, TABLE_LENGTH);
	glPopMatrix();

	glPushMatrix();
	// table wall on the ground
	glColor3d(0.2, 0.2, 0.2);
	glTranslated(-(TABLE_WIDTH + WALL_THICK) / 2.0, 0, 0);
	drawCube(WALL_THICK, TABLE_HEIGHT, TABLE_LENGTH);
	// table edge on the wall
	glColor3d(0.6, 0.6, 0.6);
	glTranslated(0, (TABLE_HEIGHT + WALL_THICK) / 2.0, 0);
	drawCube(WALL_THICK, WALL_THICK, TABLE_LENGTH);
	glPopMatrix();

	// length of wall beside the goal
	GLdouble length = (TABLE_WIDTH - GOAL_LENGTH) / 2.0 + WALL_THICK;

	glPushMatrix();
	// table wall on the ground
	glColor3d(0.2, 0.2, 0.2);
	glTranslated(0, 0, (TABLE_LENGTH + WALL_THICK) / 2.0);
	drawCube(TABLE_WIDTH + 2 * WALL_THICK, TABLE_HEIGHT, WALL_THICK);
	// table edge on the wall with Goal
	glColor3d(0.6, 0.6, 0.6);
	glTranslated((GOAL_LENGTH + length)/2, (TABLE_HEIGHT + WALL_THICK) / 2.0, 0);
	drawCube(length, WALL_THICK, WALL_THICK);
	glTranslated(-(GOAL_LENGTH + length), 0, 0);
	drawCube(length, WALL_THICK, WALL_THICK);
	glPopMatrix();

	glPushMatrix();
	// table wall on the ground
	glColor3d(0.2, 0.2, 0.2);
	glTranslated(0, 0, -(TABLE_LENGTH + WALL_THICK) / 2.0);
	drawCube(TABLE_WIDTH + 2 * WALL_THICK, TABLE_HEIGHT, WALL_THICK);
	// table edge on the wall with Goal
	glColor3d(0.6, 0.6, 0.6);
	glTranslated((GOAL_LENGTH + length)/2, (TABLE_HEIGHT + WALL_THICK) / 2.0, 0);
	drawCube(length, WALL_THICK, WALL_THICK);
	glTranslated(-(GOAL_LENGTH + length), 0, 0);
	drawCube(length, WALL_THICK, WALL_THICK);
	glPopMatrix();
}

void drawFloor() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, floorTexture);

	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3d(-16, -1, -16);
	glTexCoord2f(0, 1); glVertex3d(16, -1, -16);
	glTexCoord2f(1, 1); glVertex3d(16, -1, 16);
	glTexCoord2f(1, 0); glVertex3d(-16, -1, 16);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void drawTable() {
	glPushMatrix();
	glColor3d(0.2, 0.7, 0.2);
	drawCube(TABLE_WIDTH, TABLE_HEIGHT, TABLE_LENGTH);
	glPopMatrix();
}

void drawLabel() {
	// glViewport(0, 0, SCREEN_WIDTH, SCREEN_WIDTH);
	if (gameEnd == 0) {
		return;
	}
	glPushMatrix();
	if (gameEnd == PLAYER_LOSE) {
		glRasterPos3d(-0.5, TABLE_HEIGHT * 0.75, 1);
		glColor3d(0.1, 0.8, 0.1);
		for (int i = 0; i < strlen(loseLabel); i++) {
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, loseLabel[i]);
		}
	}
	else if (gameEnd == PLAYER_WIN) {
		glRasterPos3f(-0.5, TABLE_HEIGHT * 0.75, 1);
		glColor3d(0.8, 0.1, 0.1);
		for (int i = 0; i < strlen(winLabel); i++) {
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, winLabel[i]);
		}
	}

	glRasterPos3f(-0.6, TABLE_HEIGHT * 0.75 - 0.2, 1);
	for (int i = 0; i < strlen(tryAgainLabel); i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, tryAgainLabel[i]);
	}
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	return;
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	
	glLoadIdentity();
	gluLookAt(eye[0], eye[1], eye[2], to[0], to[1], to[2], up[0], up[1], up[2]);
	glRotatef(yAngle, 0, 1, 0);
	glRotatef(xAngle, 1, 0, 0);
	drawFloor();
	drawTable();
	drawWall();
	drawLabel();
	player->draw();
	aiPlayer->draw();
	puck->draw();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, SCREEN_WIDTH / SCREEN_HEIGHT, 1, 100);

	glFlush();
	glutSwapBuffers();
}

void reshape(int width, int height) {
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	if (height == 0) height = 1;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (GLfloat)width / height, 0.1, 30);
}

void keyboardFunc(unsigned char key, int x, int y) {
	switch (key)
	{
	case 27: case 'q': case 'Q':
		exit(0);
		break;
	case 'g': case 'G':
		gameEnd = PLAYER_LOSE; // GG
		break;
	case 13:
		gameRestart();
		break;
	case 'a':
		yAngle += 10.0f;
		if (yAngle > 90.0) yAngle = 90.0;
		break;
	case'd':
		yAngle -= 10.0f;
		if (yAngle < -90.0) yAngle = -90.0;
		break;
	case 'w':
		xAngle += 10.0f;
		if (xAngle > MAX_X_ANGLE) xAngle = MAX_X_ANGLE;
		break;
	case 's':
		xAngle -= 10.0f;
		if (xAngle < MIN_X_ANGLE) xAngle = MIN_X_ANGLE;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

// http://www.yovisun.com/archive/opengl-gluunproject-original.html
void mouseFunc(int x, int y) {
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;

	glPushMatrix();
	// repear operations in display()
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye[0], eye[1], eye[2], to[0], to[1], to[2], up[0], up[1], up[2]);
	glRotatef(yAngle, 0, 1, 0);
	glRotatef(xAngle, 1, 0, 0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, SCREEN_WIDTH / SCREEN_HEIGHT, 1, 100);

	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glPopMatrix();

	winX = x;
	winY = viewport[3] - (GLfloat)y; // viewport[3] is screen height
	glReadPixels((int)winX, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
	playerX = posX;
	playerZ = posZ;

	playerX = max(playerX, MIN_MALLET_X);
	playerX = min(playerX, MAX_MALLET_X);
	playerZ = max(playerZ, MIN_MALLET_Z);
	playerZ = min(playerZ, MAX_MALLET_Z);
	
	if (gameEnd == 0) {
		player->update(playerX, playerZ, puck->getX(), puck->getY(), puck->getRadius());
	}
	glutPostRedisplay();
}

void idleFunc() {

}

extern void aiMove(GLfloat, Mallet*, Puck*);

void onTimer(int) {
	if (gameEnd == 0) {
		gameEnd = puck->move();
		if (gameEnd) {
			yAngle = 0;
			xAngle = 10;
		}
 		aiMove(16.0f, aiPlayer, puck);
	}
	glutPostRedisplay();
	glutTimerFunc(16, onTimer, 1);
}

int main(int argc, char **argv) {
	srand((unsigned)time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutCreateWindow("Air Hockey 3D");
	glInit();
	gameInit();
	gameRestart();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboardFunc);
	glutPassiveMotionFunc(mouseFunc);
	glutIdleFunc(idleFunc);
	glutTimerFunc(16, onTimer, 1);
	glutMainLoop();
	return 0;
}

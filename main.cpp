
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

using namespace std;

GLfloat eye[] = { 5.0, 6.0, 5.0 };
GLfloat to[] = { 0.0, 4.0, 0.0 };
GLfloat up[] = { -2, 1, -2 };

const GLuint SCREEN_WIDTH = 640;
const GLuint SCREEN_HEIGHT = 480;

const GLfloat TABLE_WIDTH = 2.0f;  // x
const GLfloat TABLE_LENGTH = 4.0f; // y
const GLfloat TABLE_HEIGHT = 4.0f; // z

const GLfloat GOAL_LENGTH = 0.8f;

const GLfloat WALL_THICK = 0.1f;
const GLfloat WALL_HEIGHT = 0.1f;

const GLfloat PUCK_DIAMETER = 0.2f;
const GLfloat PUCK_HEIGHT = 0.1f;

const GLfloat MALLET_DIAMETER = 0.3f;
const GLfloat MALLET_HEIGHT = 0.1f;

GLfloat light_position[] = { 0.0f, 2.0f, 3.0f, 0.0f };
GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat mat_ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };
GLfloat mat_diffuse[] = { 0.7f, 0.1f, 0.2f, 1.0f };
GLfloat mat_shininess[] = { 150.0f };

Puck* puck;
Mallet* player;
Mallet* aiPlayer;

GLuint floorTexture;
BMP* floorBMP;

void init() {
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
			0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
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
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
}

void drawCube(GLfloat x, GLfloat y, GLfloat z) {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glScaled(x, y, z);
	glutSolidCube(1.0);
	glPopMatrix();
}

void drawWall() {
	glColor3f(0.6f, 0.6f, 0.6f);

	glPushMatrix();
	glTranslated(TABLE_WIDTH / 2.0, 2.0, 0);
	drawCube(WALL_THICK, WALL_THICK, TABLE_LENGTH + 2 * WALL_THICK);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-TABLE_WIDTH / 2.0, 2.0, 0);
	drawCube(WALL_THICK, WALL_THICK, TABLE_LENGTH + 2 * WALL_THICK);
	glPopMatrix();

}

void drawFloor() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, floorTexture);

	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3d(-16, -1, -10);
	glTexCoord2f(0, 1); glVertex3d(10, -1, -10);
	glTexCoord2f(1, 1); glVertex3d(10, -1, 4);
	glTexCoord2f(1, 0); glVertex3d(-16, -1, 4);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void drawTable() {
	glPushMatrix();
	drawCube(TABLE_WIDTH, TABLE_HEIGHT, TABLE_LENGTH);
	glPopMatrix();

	drawWall();
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	
	glLoadIdentity();
	gluLookAt(eye[0], eye[1], eye[2], to[0], to[1], to[2], up[0], up[1], up[2]);
	
	drawTable();
	drawWall();
	drawFloor();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40, 1, 1, 100);
	glMatrixMode(GL_MODELVIEW);

	player->draw();
	aiPlayer->draw();
	puck->draw();

	glFlush();
	glutSwapBuffers();
}

void reshape(int width, int height) {
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	if (height == 0) height = 1;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, (GLfloat)width / height, 0.1, 30);
}

void keyboardFunc(unsigned char key, int x, int y) {
	switch (key)
	{
	case 27: case 'q': case 'Q':
		exit(0);
		break;

	default:
		break;
	}
	glutPostRedisplay();
}

void mouseFunc(int , int , int , int) {

}

void idleFunc() {

}

int main(int argc, char **argv) {
	srand((unsigned)time(NULL));
	player = new Mallet();
	aiPlayer = new Mallet();
	puck = new Puck();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutCreateWindow("Air Hockey 3D");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboardFunc);
	glutMouseFunc(mouseFunc);
	glutIdleFunc(idleFunc);
	glutMainLoop();
	return 0;
}
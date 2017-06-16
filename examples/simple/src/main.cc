#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <GL/glut.h>
#include "vrtk/vrtk.h"

void display();
void reshape(int x, int y);
void key_press(unsigned char key, int x, int y);
void key_release(unsigned char key, int x, int y);
void mbutton(int bn, int st, int x, int y);
void mmotion(int x, int y);

float cam_theta, cam_phi, cam_dist = 10;

vrtk::Button *bn;

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(1024, 768);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutCreateWindow("vrtk test");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(key_press);
	glutKeyboardUpFunc(key_release);
	glutMouseFunc(mbutton);
	glutMotionFunc(mmotion);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_CULL_FACE);

	float ldir[] = {-1, 1, 1, 0};
	glLightfv(GL_LIGHT0, GL_POSITION, ldir);

	bn = new vrtk::Button;

	glutMainLoop();
	return 0;
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -cam_dist);
	glRotatef(cam_phi, 1, 0, 0);
	glRotatef(cam_theta, 0, 1, 0);

	bn->draw();

	glutSwapBuffers();
	assert(glGetError() == GL_NO_ERROR);
}

void reshape(int x, int y)
{
	glViewport(0, 0, x, y);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50, (float)x / (float)y, 0.2, 500.0);
}

void key_press(unsigned char key, int x, int y)
{
	switch(key) {
	case 27:
		exit(0);
	}
}

void key_release(unsigned char key, int x, int y)
{
}

static bool bnstate[8];
static int prev_x, prev_y;

void mbutton(int bn, int st, int x, int y)
{
	bnstate[bn - GLUT_LEFT_BUTTON] = st == GLUT_DOWN;
	prev_x = x;
	prev_y = y;
}

void mmotion(int x, int y)
{
	int dx = x - prev_x;
	int dy = y - prev_y;
	prev_x = x;
	prev_y = y;

	if((dx | dy) == 0) return;

	if(bnstate[0]) {
		cam_theta += dx * 0.5;
		cam_phi += dy * 0.5;

		if(cam_phi < -90) cam_phi = -90;
		if(cam_phi > 90) cam_phi = 90;
		glutPostRedisplay();
	}
	if(bnstate[2]) {
		cam_dist += dy * 0.1;
		if(cam_dist < 0) cam_dist = 0;
		glutPostRedisplay();
	}
}

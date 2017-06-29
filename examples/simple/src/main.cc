#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include "vrtk/vrtk.h"

void display();
void reshape(int x, int y);
void key_press(unsigned char key, int x, int y);
void key_release(unsigned char key, int x, int y);
void mbutton(int bn, int st, int x, int y);
void mmotion(int x, int y);
void passive(int x, int y);
void sball_motion(int x, int y, int z);
void sball_rotate(int x, int y, int z);
void sball_button(int bn, int st);
void proc_mouse();
void proc_sball();

float cam_theta, cam_phi, cam_dist = 10;
bool mouse_pending, sball_pending;
Vec3 sbpos;
Quat sbrot;

vrtk::WidgetGroup *wgroup;

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
	glutPassiveMotionFunc(passive);
	glutSpaceballMotionFunc(sball_motion);
	glutSpaceballRotateFunc(sball_rotate);
	glutSpaceballButtonFunc(sball_button);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_CULL_FACE);

	float ldir[] = {-1, 1, 1, 0};
	glLightfv(GL_LIGHT0, GL_POSITION, ldir);

	wgroup = new vrtk::WidgetGroup;

	vrtk::Button *bn = new vrtk::Button;
	wgroup->add_widget(bn);

	glutMainLoop();
	return 0;
}

void display()
{
	proc_mouse();
	proc_sball();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -cam_dist);
	glRotatef(cam_phi, 1, 0, 0);
	glRotatef(cam_theta, 0, 1, 0);

	wgroup->draw();

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

	vrtk::input_keyboard(key, true);
}

void key_release(unsigned char key, int x, int y)
{
	vrtk::input_keyboard(key, false);
}

static bool bnstate[8];
static int prev_x, prev_y;
static int modkeys;

void mbutton(int bn, int st, int x, int y)
{
	int bidx = bn - GLUT_LEFT_BUTTON;
	bool pressed = st == GLUT_DOWN;

	bnstate[bidx] = pressed;
	prev_x = x;
	prev_y = y;

	modkeys = glutGetModifiers();

	proc_mouse();
	proc_sball();

	vrtk::input_button(bidx, pressed);
}

void mmotion(int x, int y)
{
	int dx = x - prev_x;
	int dy = y - prev_y;
	prev_x = x;
	prev_y = y;

	if((dx | dy) == 0) return;

	if(modkeys & (GLUT_ACTIVE_ALT | GLUT_ACTIVE_CTRL)) {
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
	} else {
		mouse_pending = true;
	}
}

void passive(int x, int y)
{
	mouse_pending = true;
	prev_x = x;
	prev_y = y;
}

void sball_motion(int x, int y, int z)
{
	sbpos = Vec3(x, y, z);
	sball_pending = true;
}

void sball_rotate(int x, int y, int z)
{
	// TODO
}

void sball_button(int bn, int st)
{
	vrtk::input_button(bn, st == GLUT_DOWN);
}

void proc_mouse()
{
	int vp[4];
	double view[16], proj[16];
	double pnear[3], pfar[3];

	if(!mouse_pending) return;

	glGetIntegerv(GL_VIEWPORT, vp);
	glGetDoublev(GL_MODELVIEW_MATRIX, view);
	glGetDoublev(GL_PROJECTION_MATRIX, proj);

	gluUnProject(prev_x, prev_y, 0, view, proj, vp, pnear, pnear + 1, pnear + 2);
	gluUnProject(prev_x, prev_y, 1, view, proj, vp, pfar, pfar + 1, pfar + 2);

	Vec3 orig = Vec3(pnear[0], pnear[1], pnear[2]);
	Vec3 dir = Vec3(pfar[0], pfar[1], pfar[2]) - orig;

	vrtk::input_ray_pointer(orig, dir);
	mouse_pending = false;
}

void proc_sball()
{
	if(!sball_pending) return;

	vrtk::input_3d_pointer(sbpos);
	sball_pending = false;
}

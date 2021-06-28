#include <windows.h>
#include <GL/glut.h>
#include "tutorial4.h"
#include "texture.h"
#include "3dsloader.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>
using namespace std;
int screen_width = 640;
int screen_height = 480;
GLfloat angle = 0;

double rotation_x = 0, rotation_x_increment = 0;
double rotation_y = 0, rotation_y_increment = 0;
double rotation_z = 180, rotation_z_increment = 0;
double xx[3] = { 0 };
double yy[3] = { 150,150,150 };
int filling = 1;
int blood = 3;
double dd = 1;
obj_type object;

typedef struct
{
	vertex_type vertex[MAX_VERTICES];
	polygon_type polygon[MAX_POLYGONS];
	mapcoord_type mapcoord[MAX_VERTICES];
	int id_texture;
} obj_type2, * obj_type2_ptr;

obj_type2 back_ground = {
	{
		-300,-300,-350,
		300 ,-300,-350,
		300 , 300,-350,
		-300, 300,-350
	},
	{
		0,1,2,
		2,3,0
	},
	{
		0,0,1,0,1,1,0,1
	},
	0,
};

void background()
{
	glLoadIdentity();
	glPushMatrix();
	glTranslatef(0, -angle, 0);
	glBindTexture(GL_TEXTURE_2D, back_ground.id_texture);
	glBegin(GL_TRIANGLES);
	for (int l_index = 0; l_index < 2; l_index++)
	{
		glTexCoord2f(back_ground.mapcoord[back_ground.polygon[l_index].a].u, back_ground.mapcoord[back_ground.polygon[l_index].a].v);
		glVertex3f(back_ground.vertex[back_ground.polygon[l_index].a].x, back_ground.vertex[back_ground.polygon[l_index].a].y, back_ground.vertex[back_ground.polygon[l_index].a].z);

		glTexCoord2f(back_ground.mapcoord[back_ground.polygon[l_index].b].u, back_ground.mapcoord[back_ground.polygon[l_index].b].v);
		glVertex3f(back_ground.vertex[back_ground.polygon[l_index].b].x, back_ground.vertex[back_ground.polygon[l_index].b].y, back_ground.vertex[back_ground.polygon[l_index].b].z);

		glTexCoord2f(back_ground.mapcoord[back_ground.polygon[l_index].c].u, back_ground.mapcoord[back_ground.polygon[l_index].c].v);
		glVertex3f(back_ground.vertex[back_ground.polygon[l_index].c].x, back_ground.vertex[back_ground.polygon[l_index].c].y, back_ground.vertex[back_ground.polygon[l_index].c].z);
	}
	glEnd();
	glPopMatrix();
}

void init(void)
{
	xx[0] = (rand() % screen_width - (screen_width / 2)) / 2;
	xx[1] = (rand() % screen_width - (screen_width / 2)) / 2;
	xx[2] = (rand() % screen_width - (screen_width / 2)) / 2;

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);

	glViewport(0, 0, screen_width, screen_height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)screen_width / (GLfloat)screen_height, 10.0f, 10000.0f);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_TEXTURE_2D);

	Load3DS(&object, "spaceship.3ds");

	object.id_texture = LoadBitmap("spaceshiptexture.bmp");
	if (object.id_texture == -1)
	{
		MessageBox(NULL, "Image file: spaceshiptexture.bmp not found", "Zetadeck", MB_OK | MB_ICONERROR);
		exit(0);
	}

	back_ground.id_texture = LoadBitmap(strdup("texture.bmp"));
	if (back_ground.id_texture == -1)
	{
		MessageBox(NULL, "Image file: texture.bmp not found", "error", MB_OK | MB_ICONERROR);
		return;
	}
	MessageBox(NULL, "game start, blood= 3 ", "information", MB_OK | MB_OKCANCEL);
}

void resize(int width, int height)
{
	screen_width = width;
	screen_height = height;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, screen_width, screen_height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)screen_width / (GLfloat)screen_height, 10.0f, 10000.0f);

	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{

	switch (key)
	{

	case ' ':
		rotation_x_increment = 0;
		rotation_y_increment = 0;
		rotation_z_increment = 0;
		break;
	case 'r': case 'R':
		if (filling == 0)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			filling = 1;
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			filling = 0;
		}

	}
}

void keyboard_s(int key, int x, int y)
{

	switch (key)
	{
	case GLUT_KEY_UP:
		rotation_y_increment = rotation_y_increment + 1;
		break;
	case GLUT_KEY_DOWN:
		rotation_y_increment = rotation_y_increment - 1;
		break;
	case GLUT_KEY_LEFT:
		rotation_x_increment = rotation_x_increment - 1;
		break;
	case GLUT_KEY_RIGHT:
		rotation_x_increment = rotation_x_increment + 1;
		break;
	case 'Q':
		rotation_x++;
		cout << rotation_x << endl;
		break;
	case 'E':
		rotation_x--;
		break;
	}
}

void make_sphere(int xx, int yy, int in)
{
	cout << xx << '\t';
	glTranslatef(xx, yy, -270);
	glPushMatrix();
	glColor3f(0, 1, 1);
	GLUquadricObj* quadricPtr;
	quadricPtr = gluNewQuadric();
	gluQuadricDrawStyle(quadricPtr, GLU_LINE);
	gluQuadricNormals(quadricPtr, GLU_NONE);
	gluQuadricTexture(quadricPtr, GL_FALSE);
	gluSphere(quadricPtr, in, in * 1.5, in * 2);
	glPopMatrix();
}

void bloodd(int blood)
{
	if (blood == 3)
	{
		MessageBox(NULL, "hey guy, blood= 3 ", "information", MB_OK | MB_OKCANCEL);

	}
	else if (blood == 2)
	{
		MessageBox(NULL, "oh, blood= 2 ", "information", MB_OK | MB_OKCANCEL);

	}
	else if (blood == 1)
	{
		MessageBox(NULL, "oh no!, blood= 1 ", "information", MB_OK | MB_OKCANCEL);

	}
	else
	{
		MessageBox(NULL, "game over..., blood= 0 ", "information", MB_OK | MB_OKCANCEL);
		exit(0);
	}
}

void display(void)
{
	int l_index;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	make_sphere(xx[0], yy[0], 15);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	make_sphere(xx[1], yy[1], 15);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	make_sphere(xx[2], yy[2], 15);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(rotation_x_increment, rotation_y_increment, -300);

	rotation_x = rotation_x;
	rotation_y = rotation_y;
	rotation_z = rotation_z;

	if (rotation_x > 359) rotation_x = 0;
	if (rotation_y > 359) rotation_y = 0;
	if (rotation_z > 359) rotation_z = 0;

	glRotatef(rotation_x, 1.0, 0.0, 0.0);
	glRotatef(rotation_y, 0.0, 1.0, 0.0);
	glRotatef(rotation_z, 0.0, 0.0, 1.0);

	glBindTexture(GL_TEXTURE_2D, object.id_texture);
	glScalef(0.2, 0.2, 0.2);
	glBegin(GL_TRIANGLES);
	for (l_index = 0; l_index < object.polygons_qty; l_index++)
	{
		//----------------- FIRST VERTEX -----------------
		// Texture coordinates of the first vertex
		glTexCoord2f(object.mapcoord[object.polygon[l_index].a].u,
			object.mapcoord[object.polygon[l_index].a].v);
		// Coordinates of the first vertex
		glVertex3f(object.vertex[object.polygon[l_index].a].x,
			object.vertex[object.polygon[l_index].a].y,
			object.vertex[object.polygon[l_index].a].z); //Vertex definition

//----------------- SECOND VERTEX -----------------
// Texture coordinates of the second vertex
		glTexCoord2f(object.mapcoord[object.polygon[l_index].b].u,
			object.mapcoord[object.polygon[l_index].b].v);
		// Coordinates of the second vertex
		glVertex3f(object.vertex[object.polygon[l_index].b].x,
			object.vertex[object.polygon[l_index].b].y,
			object.vertex[object.polygon[l_index].b].z);

		//----------------- THIRD VERTEX -----------------
		// Texture coordinates of the third vertex
		glTexCoord2f(object.mapcoord[object.polygon[l_index].c].u,
			object.mapcoord[object.polygon[l_index].c].v);
		// Coordinates of the Third vertex
		glVertex3f(object.vertex[object.polygon[l_index].c].x,
			object.vertex[object.polygon[l_index].c].y,
			object.vertex[object.polygon[l_index].c].z);
	}
	glEnd();
	background();
	glFlush();
	glutSwapBuffers();
}

void idle()
{
	Sleep(5);
	cout << rotation_x_increment << '\t';
	if (angle < 155)	angle += 0.1;
	else
	{
		if(blood==1)		MessageBox(NULL, "you win!! ,blood: 1", "win!!!!!", MB_OK | MB_OKCANCEL);
		else if(blood==2)	MessageBox(NULL, "you win!! ,blood: 2", "win!!!!!", MB_OK | MB_OKCANCEL);
		else				MessageBox(NULL, "you win!! ,blood: 3", "win!!!!!", MB_OK | MB_OKCANCEL);
		exit(0);
	}
	if (yy[0] > -155) yy[0]-= dd;
	else
	{
		yy[0] = 155;
		xx[0] = (rand() % screen_width - (screen_width / 2)) / 2;
	}
	cout << yy[0] << '\t';
	if (yy[1] > -155) yy[1]-= dd;
	else
	{
		yy[1] = 155;
		xx[1] = (rand() % screen_width - (screen_width / 2)) / 2;
		dd += 0.2;
	}
	cout << yy[1] << '\t';
	if (yy[2] > -155) yy[2]-= dd;
	else
	{
		yy[2] = 155;
		xx[2] = (rand() % screen_width - (screen_width / 2)) / 2;
	}
	cout << yy[2] << endl;
	if (	(fabs(xx[0] - rotation_x_increment)<10|| fabs(xx[1] - rotation_x_increment) < 10|| fabs(xx[2] - rotation_x_increment) < 10)
		&&	((fabs(yy[0]-rotation_y_increment)<38)|| (fabs(yy[1] - rotation_y_increment) < 38)|| (fabs(yy[2] - rotation_y_increment) < 38)))
	{
		bloodd(--blood);
		yy[0] -= 90;
		yy[1] -= 90;
		yy[2] -= 90;
	}

	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(screen_width, screen_height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("www.spacesimulator.net - 3d engine tutorials: Tutorial 4");
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboard_s);
	glutIdleFunc(idle);
	init();
	glutMainLoop();

	return(0);
}

#include <windows.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "lib/stdafx.h"
#include "lib/glm.h"

#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500

//Header:
void renderScene(void);
void reshape(int w, int h);
void keyEvent(unsigned char key, int x, int y);
void specialEvent(int key, int x, int y);
void mouseEvent(int button, int state, int x, int y);
void motionEvent(int x, int y);
void setup(void);
void drawScene(void);
void drawAirplane(void);
GLuint glmLoadTexture(char *filename, GLboolean alpha, GLboolean repeat, GLboolean filtering, GLboolean mipmaps, GLfloat *texcoordwidth, GLfloat *texcoordheight);

//Variaveis globais:
GLMmodel* pmodel1 = NULL;

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition (15, 15);
	glutCreateWindow("Simulador");

	glutDisplayFunc(renderScene);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyEvent);
	glutSpecialFunc(specialEvent);
	glutMouseFunc(mouseEvent);
	glutMotionFunc(motionEvent);

	setup();

	glutMainLoop();

	return 0;
}

void renderScene(void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	drawScene();
	//drawAirplane();

	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
	gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 0.01, 20.1);
	gluLookAt(0.0, 0.1, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glMatrixMode(GL_MODELVIEW);
}

void keyEvent(unsigned char key, int x, int y)
{
}

void specialEvent(int key, int x, int y)
{
}

void mouseEvent(int button, int state, int x, int y)
{
}

void motionEvent(int x, int y)
{
}

void setup(void)
{
	glClearColor (1.0f, 1.0f, 1.0f, 1.0f);   
	glShadeModel (GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
}

void drawScene(void)
{
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	float w, h;

	//Desenha esfera
	GLuint textureSky = glmLoadTexture("sky.tga", GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE, &w, &h);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureSky);

	GLUquadric *qobj = gluNewQuadric();
	gluQuadricTexture(qobj, GL_TRUE);
	gluSphere(qobj, 1, 50, 50);
	gluDeleteQuadric(qobj);

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_2D);

	//Desenha plano
	GLuint textureGrass = glmLoadTexture("grass.tga", GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE, &w, &h);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureGrass);

	glBegin(GL_QUADS);
	glTexCoord2d(0,0); glVertex3f(-1.1f, 0.0f, 1.1f);
	glTexCoord2d(50,0); glVertex3f(-1.1f, 0.0f, -1.1f);
	glTexCoord2d(50,50); glVertex3f(1.1f, 0.0f, -1.1f);
	glTexCoord2d(0,50); glVertex3f(1.1f, 0.0f, 1.1f);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void drawAirplane(void)
{
	if (!pmodel1) 
	{
		// this is the call that actualy reads the OBJ and creates the model object
        pmodel1 = glmReadOBJ("airplane.obj");	
        if (!pmodel1) exit(0);
        glmUnitize(pmodel1);
        glmFacetNormals(pmodel1);        
		glmVertexNormals(pmodel1, 90.0);
    }
    glmDraw(pmodel1, GLM_SMOOTH | GLM_TEXTURE);
}
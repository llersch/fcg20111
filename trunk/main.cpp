#include <windows.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "lib/stdafx.h"
#include "lib/glm.h"
#include "Camera.h"

//CONSTANTES
#define PERSPECTIVE 1
#define ORTOGRAPHIC 2
#define MAXPITCH 45
#define TRANSLATEINC 0.1
#define ROTATEINCCAM 1.5
#define ROTATEINCOBJ 4

//area de visualizacao da camera ORTHO
#define XMIN -100
#define XMAX 100
#define YMIN -100
#define YMAX 100

//altura maxima do cenario
#define MAXHEIGHT 100

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
void setPerspectiveView(void);
void setOrtographicView(void);

//Variaveis globais:
GLMmodel* pmodel1 = NULL;
static float xpoz = 0,ypoz = 0, zpoz = 0, pitch = 0; //determina o quanto o objeto rotaciona nos eixos
int cameraType = PERSPECTIVE; //inicializa a camera como perspectiva
int viewPortHeight;
int viewPortWidth;
float translateX = 0,translateY = -1,translateZ = 0, rotateX = 0, rotateY = 0;
int firstTime=1;
GLfloat luzAmbiente[4]={1,1,1,0.5}; 
GLfloat luzDifusa[4]={1.0,1.0,1.0,1.0};		 // "cor" 
GLfloat luzEspecular[4]={1.0, 1.0, 1.0, 1.0};// "brilho" 
GLfloat posicaoLuz[4]={10.0, 10.0, 10.0, 0.0};   // inicial


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
	if (firstTime==1)
	{
		glTranslatef(25,1.0,0.0);	
		//glScalef(5.0f,5.0f,5.0f);
		//glRotatef(180, 0.0, 1.0, 0.0); 
		firstTime=0;
	}
	glColor3f(1.0f, 1.0f, 1.0f);
	drawAirplane();
	
	glPushMatrix();	
	glLoadIdentity();

	drawScene();
	glPopMatrix();

	
	glPushMatrix();	
	glLoadIdentity();

	glColor3f(1.0, 0.0, 0.0);
	glTranslatef(-20.0, 0.0, -20.0);
	glutSolidCube(25.0);

	glLoadIdentity();

	glColor3f(0.0, 1.0, 0.0);
	glTranslatef(-20.0, 0.0, 20.0);
	glutSolidCube(25.0);

	glLoadIdentity();

	glColor3f(0.0, 0.0, 1.0);
	glTranslatef(20.0, 0.0, -20.0);
	glutSolidCube(25.0);

	glLoadIdentity();

	glColor3f(1.0, 1.0, 0.0);
	glTranslatef(20.0, 0.0, 20.0);
	glutSolidCube(25.0);
	glPopMatrix();

	glutSwapBuffers();
}

void reshape(int w, int h)
{
	viewPortHeight = h;
	viewPortWidth = w;
    if(cameraType == PERSPECTIVE)
		setPerspectiveView();    
	else
		setOrtographicView();
   
	glMatrixMode (GL_MODELVIEW);
}

void keyEvent(unsigned char key, int x, int y)
{
	switch (key) 
		{
		//ESC - encerra o programa
		case 27:
			exit(0);
			break;
		//W - Gira em torno do eixo x
		case 'w':         
			if(pitch<MAXPITCH)
			{
				pitch = pitch + ROTATEINCOBJ;
				glRotatef(ROTATEINCOBJ,1,0,0);
			}
			break;

		//S - Gira em torno do eixo x
		case 's':         
			if(pitch>-MAXPITCH)
			{
				pitch = pitch -ROTATEINCOBJ;
				glRotatef(-ROTATEINCOBJ,1,0,0);
			}
			break;
	  
		//Q - Gira em torno do eixo y
		case 'q':         
			glRotatef(ROTATEINCOBJ,0,1,0);
			break;

		//E - Gira em torno do eixo y
		case 'e':         
			glRotatef(-ROTATEINCOBJ,0,1,0);
			break;
		//D - Gira em torno do eixo z
		case 'd':         
			glRotatef(ROTATEINCOBJ,0,0,1);
			break;	
		
		//A - Gira em torno do eixo z
		case 'a':         
			glRotatef(-ROTATEINCOBJ,0,0,1);
			break;	
		
		//C - Alterna as cameras
		case 'c':         
			if (cameraType==ORTOGRAPHIC)
				setPerspectiveView();
			else
				setOrtographicView();
			break;

		
		case 'k':         
			if (cameraType == PERSPECTIVE)
			{
				translateZ-=TRANSLATEINC;
				setPerspectiveView();
			}
			break;

		case 'i':
			if (cameraType == PERSPECTIVE)
			{
				translateZ+=TRANSLATEINC;
				setPerspectiveView();
			}
			break;
	  
		case 'l':         
			if (cameraType == PERSPECTIVE)
			{
				translateX-=TRANSLATEINC;
				setPerspectiveView();
			}
			break;

		case 'j':
			if (cameraType == PERSPECTIVE)
			{
				translateX+=TRANSLATEINC;
				setPerspectiveView();
			}
			break;
  		}

	
	glutPostRedisplay();
}

void specialEvent(int key, int x, int y)
{
	switch (key)
	{
		case GLUT_KEY_LEFT:
			rotateY+=ROTATEINCCAM;
			break;
		case GLUT_KEY_RIGHT:
			rotateY-=ROTATEINCCAM;
			break;
		case GLUT_KEY_UP:
			rotateX+=ROTATEINCCAM;
			break;
		case GLUT_KEY_DOWN:
			rotateX-=ROTATEINCCAM;
			break;
	}
	setPerspectiveView();

	glutPostRedisplay();
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
   
	//ILUMINACAO
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHT0);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0);
	glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz );
	//glLightfv(GL_LIGHT0, GL_AMBIENT,luzAmbiente);
	glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular );
	glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa );

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
	gluSphere(qobj, 100.0f, 50, 50);//se aumentar o raio da esfera, DEVE-SE aumentar as coordenadas do plano abaixo assim como a constante "MAXHEIGHT"
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
	glTexCoord2d(0,0); glVertex3f(-100.0f, 0.0f, 100.0f);
	glTexCoord2d(50,0); glVertex3f(-100.0f, 0.0f, -100.0f);
	glTexCoord2d(50,50); glVertex3f(100.0f, 0.0f, -100.0f);
	glTexCoord2d(0,50); glVertex3f(100.0f, 0.0f, 100.0f);
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

void setPerspectiveView(void)
{
	glViewport (0, 0, (GLsizei) viewPortWidth, (GLsizei) viewPortHeight); 
	cameraType=PERSPECTIVE; //seta o tipo de camera atual
	glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(60.0, (GLfloat) viewPortWidth/(GLfloat) viewPortHeight, 0.1, 200.0);
	glTranslatef(translateX,translateY,translateZ);
	glRotatef(rotateY,0,1,0);
	glRotatef(rotateX,1,0,0);
	glMatrixMode (GL_MODELVIEW);
}

void setOrtographicView(void)

{
	float orthoPoints;
	glViewport (0, 0, (GLsizei) viewPortWidth, (GLsizei) viewPortHeight); 
	cameraType=ORTOGRAPHIC; //seta o tipo de camera atual
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	orthoPoints = MAXHEIGHT/2;
	glOrtho(-orthoPoints,orthoPoints,-orthoPoints,orthoPoints,MAXHEIGHT*30/100,MAXHEIGHT+1);
	gluLookAt (0,MAXHEIGHT,0,0,0,0,1,0,1);
	glMatrixMode (GL_MODELVIEW);
}
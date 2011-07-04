#include <windows.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "lib/stdafx.h"
#include "lib/glm.h"
#include <math.h>

//CONSTANTES
#define PERSPECTIVE 1
#define ORTOGRAPHIC 2
#define THIRDPERSON 3
#define ANOTHERPLANE 4
#define ROTATEINCOBJ 3.0
#define X 0
#define Y 1
#define Z 2
#define PI 3.14159265
#define FPS 17 //Esse valor e definido por 1000/60
#define MAXSPEED 0.5 //Velocidade maxima do aviao
#define MINSPEED 0.3 //Velocidade para alcar voo
#define SPEEDINC 0.001 //Aceleracao
#define BUILDINGSCALE 35.0

//area de visualizacao da camera ORTHO
#define XMIN -100
#define XMAX 100
#define YMIN -100
#define YMAX 100

//altura maxima do cenario
#define MAXHEIGHT 1000

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
void drawHangar(void);
void drawBank(void);
void drawStadium(void);
void drawBuilding1(void);
void drawBuilding3(void);
void drawBuilding4(void);
GLuint glmLoadTexture(char *filename, GLboolean alpha, GLboolean repeat, GLboolean filtering, GLboolean mipmaps, GLfloat *texcoordwidth, GLfloat *texcoordheight);
void setPerspectiveView(void);
void setOrtographicView(void);
void movePlane(void);
void idleFunc(void);
void setThirdPersonView(void);
void setAnotherPlaneView(void);
void refreshCamera(void);


//Variaveis globais:
int cameraType = PERSPECTIVE; //inicializa a camera como perspectiva
int viewPortHeight;
int viewPortWidth;
int firstTime=1;
GLfloat luzAmbiente[4]={0.5,0.5,0.5,0.5};	//luz ambiente 
GLfloat luzDifusa[4]={1.0,1.0,1.0,1.0};		 // "cor" 
GLfloat luzEspecular[4]={1.0, 1.0, 1.0, 1.0};// "brilho" 
GLfloat posicaoLuz[4]={50.0, 99.0, 0.0, 0.0};   // inicial
double planePosition[3]={0,1,697};
double pointVector[3]={0,0,1};
double planeSpeed=0.0;
float rotateAngle[3]={0,180,0};
//float upVector[3]={0,1,0};
//Modelos:
GLMmodel* airplane = NULL;
GLMmodel* hangar = NULL;
GLMmodel* bank = NULL;
GLMmodel* building1 = NULL;
GLMmodel* building3 = NULL;
GLMmodel* building4 = NULL;
GLMmodel* stadium = NULL;

//Texturas:
GLuint textureSky;
GLuint textureGrass;
GLuint textureLane;
GLuint textureAsfalt;

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
	//glutMouseFunc(mouseEvent);
	//glutMotionFunc(motionEvent);
	glutIdleFunc( idleFunc );	

	setup();

	glutMainLoop();

	return 0;
}



void renderScene(void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor3f(1.0f, 1.0f, 1.0f);

	//Desenha o Aviao
	glLoadIdentity();
	glTranslated(planePosition[X], planePosition[Y], planePosition[Z]);
	glRotatef(rotateAngle[Y],0,1,0);
	glRotatef(-rotateAngle[X],1,0,0);
	glRotatef(rotateAngle[Z],0,0,1);
	drawAirplane();
	
	//Desenha a Cena
	glPushMatrix();	
	glLoadIdentity();
	drawScene();
	glPopMatrix();


	//Comandos para acompanhar o comportamento do vôo
	//system("cls");
	printf("Velocidade: %f/%f\n",planeSpeed,MAXSPEED);
	printf("Direcao: %f,%f,%f\n",pointVector[X],pointVector[Y],pointVector[Z]);
	printf("Posicao: %f,%f,%f\n\n",planePosition[X],planePosition[Y],planePosition[Z]);


	glutSwapBuffers();
}

void reshape(int w, int h)
{
	viewPortHeight = h;
	viewPortWidth = w;
    refreshCamera();
	glMatrixMode (GL_MODELVIEW);
}

void movePlane(void)
{
	if (planeSpeed<MINSPEED && planePosition[Y]>1) //Simulacao de gravidade
		{
			planePosition[Y]-=(MINSPEED-planeSpeed)*1;
			if( rotateAngle[X]>-90 && rotateAngle[X]<=90)
				rotateAngle[X]-=ROTATEINCOBJ*10/100;
			if( rotateAngle[X]<=-90 && rotateAngle[X]>90)
				rotateAngle[X]+=ROTATEINCOBJ*10/100;
		}


	//Set new camera Up Vector //Tentativa de rotacionar a camera
//	upVector[X]=sin(rotateAngle[Z]*PI/180)*cos(rotateAngle[Y]*PI/180);
//	upVector[Y]=cos(rotateAngle[Z]*PI/180)*cos(rotateAngle[X]*PI/180);
//	upVector[Z]=sin(rotateAngle[X]*PI/180)*sin(rotateAngle[Y]*PI/180);

	
	//Set new plane Point Vector
	//pointVector[X]=cos(rotateAngle[Z]*PI/180)*sin(rotateAngle[Y]*PI/180);
	//pointVector[Y]=sin(rotateAngle[X]*PI/180)*cos(rotateAngle[Z]*PI/180);
	pointVector[X]=sin(rotateAngle[Y]*PI/180)*cos(rotateAngle[X]*PI/180);
	pointVector[Y]=sin(rotateAngle[X]*PI/180);
	pointVector[Z]=cos(rotateAngle[Y]*PI/180)*cos(rotateAngle[X]*PI/180);

	//Setup new Plane Position
	planePosition[X]+= pointVector[X]*planeSpeed;
	planePosition[Y]+= pointVector[Y]*planeSpeed;
	planePosition[Z]+= pointVector[Z]*planeSpeed;

	if (planePosition[Y]<=1) //Simulacao de atrito e ajuste do aviao no solo
	{
		planeSpeed-=SPEEDINC*10/100;
		if(planeSpeed<0)
			planeSpeed=0;
		if( (rotateAngle[X]>-90 && rotateAngle[X]<0) || (rotateAngle[X]>90 && rotateAngle[X]<180))
			rotateAngle[X]+=ROTATEINCOBJ*10/100;
		if( (rotateAngle[X]<=-90 && rotateAngle[X]>-180) || (rotateAngle[X]<=90 && rotateAngle[X]>0))
			rotateAngle[X]-=ROTATEINCOBJ*10/100;

	}


	if (planePosition[Y]<1) //Colisao com o solo
		planePosition[Y]=1;
}
void idleFunc(void)
{
		movePlane();
		refreshCamera();
		glutPostRedisplay();
		Sleep(FPS);
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
		case 'W':
		case 'w':
			if(planePosition[Y]>1 && planeSpeed>MINSPEED)
			{
			glRotatef(-ROTATEINCOBJ,1,0,0);
			rotateAngle[X]-=ROTATEINCOBJ;
			if(rotateAngle[X]<=-360)
				rotateAngle[X]+=360.0;
			movePlane();
			}
			break;

		//S - Gira em torno do eixo x
		case 'S':
		case 's':

				if(planeSpeed>MINSPEED)
				{
				glRotatef(ROTATEINCOBJ,1,0,0);
				rotateAngle[X]+=ROTATEINCOBJ;
				if(rotateAngle[X]>=360)
					rotateAngle[X]-=360.0;
				movePlane();
				}

			break;
	  
		//A - Gira em torno do eixo Y
		case 'A':
		case 'a':
			if(planeSpeed>0)
			{
			rotateAngle[Y]+=ROTATEINCOBJ;
			if(rotateAngle[Y]>=360)
				rotateAngle[Y]-=360.0;

			movePlane();
			glRotatef(ROTATEINCOBJ,0,1,0);
			}
			break;

		//D - Gira em torno do eixo Y
		case 'D':
		case 'd':
			if(planeSpeed>0)
			{
			rotateAngle[Y]-=ROTATEINCOBJ;
			if(rotateAngle[Y]<=-360)
				rotateAngle[Y]+=360.0;
			movePlane();
			glRotatef(-ROTATEINCOBJ,0,1,0);
			}
			break;

		//E - Gira em torno do eixo Z
		case 'E':
		case 'e':
			if(planeSpeed>0 && planePosition[Y]>1)
			{
			rotateAngle[Z]+=ROTATEINCOBJ;
			if(rotateAngle[Z]>=360)
				rotateAngle[Z]-=360.0;
			movePlane();
			glRotatef(ROTATEINCOBJ,0,0,1);
			}
			break;	
		
		//Q - Gira em torno do eixo Z
		case 'Q':
		case 'q':
			if(planeSpeed>0 && planePosition[Y]>1)
			{
			rotateAngle[Z]-=ROTATEINCOBJ;
			if(rotateAngle[Z]<=-360)
				rotateAngle[Z]+=360.0;
			movePlane();
			glRotatef(-ROTATEINCOBJ,0,0,1);
			}
			break;	

		case '+':
			planeSpeed+=SPEEDINC;
			if (planeSpeed>MAXSPEED)
				planeSpeed=MAXSPEED;
			break;
		case '-':
			planeSpeed-=SPEEDINC;
			if(planeSpeed<0)
				planeSpeed=0;
			break;

  		}

	refreshCamera();	
	glutPostRedisplay();
}

void specialEvent(int key, int x, int y)
{
	switch(key)
	{
	case GLUT_KEY_F1:
			cameraType=PERSPECTIVE;
			break;

	case GLUT_KEY_F2:
			cameraType=ORTOGRAPHIC;
			break;

	case GLUT_KEY_F3:         
			cameraType=THIRDPERSON;
			break;

	case GLUT_KEY_F4:         
			cameraType=ANOTHERPLANE;
			break;
	}
	refreshCamera();	
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
	glEnable(GL_NORMALIZE);
   
	//ILUMINACAO
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHT0);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0);
	glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz );
	glLightfv(GL_LIGHT0, GL_AMBIENT,luzAmbiente);
	glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular );
	glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa );

	//Carrega arquivos de textura
	float w, h;
	textureSky = glmLoadTexture("textures//sky.tga", GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE, &w, &h);
	textureGrass = glmLoadTexture("textures//grass.tga", GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE, &w, &h);
	textureLane = glmLoadTexture("textures//lane.tga", GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE, &w, &h);
	textureAsfalt = glmLoadTexture("textures//asfalt.tga", GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE, &w, &h);
}

void drawScene(void)
{
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//======================================== ESFERA ========================================
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureSky);

	GLUquadric *qobj = gluNewQuadric();
	gluQuadricTexture(qobj, GL_TRUE);
	gluSphere(qobj, 1000.0f, 50, 50);//se aumentar o raio da esfera, DEVE-SE aumentar as coordenadas do plano abaixo assim como a constante "MAXHEIGHT"
	gluDeleteQuadric(qobj);

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_2D);

	//======================================== PLANO ========================================
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureGrass);

	glBegin(GL_QUADS);
	glTexCoord2d(0,0); glVertex3f(-1000.0f, 0.0f, 1000.0f);
	glTexCoord2d(50,0); glVertex3f(-1000.0f, 0.0f, -1000.0f);
	glTexCoord2d(50,50); glVertex3f(1000.0f, 0.0f, -1000.0f);
	glTexCoord2d(0,50); glVertex3f(1000.0f, 0.0f, 1000.0f);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	//========================================= PISTA =========================================
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureLane);

	glBegin(GL_QUADS);
	glTexCoord2d(0,3); glVertex3f(5.0f,0.1f,700.f);
	glTexCoord2d(0,0); glVertex3f(5.0f,0.1f,550.0f);
	glTexCoord2d(1,0); glVertex3f(-5.0f,0.1f,550.0f);
	glTexCoord2d(1,3); glVertex3f(-5.0f,0.1f,700.0f);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	//========================================= CIDADE1 =========================================
	
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureAsfalt);

	glBegin(GL_QUADS);
	glTexCoord2d(0,0); glVertex3f(-400.0f,0.1f,-70.f);
	glTexCoord2d(5,0); glVertex3f(500.0f,0.1f,-70.0f);
	glTexCoord2d(5,5); glVertex3f(500.0f,0.1f,-400.0f);
	glTexCoord2d(0,5); glVertex3f(-400.0f,0.1f,-400.0f);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	//======================================== PREDIOS ========================================
	
	glLoadIdentity();
	glTranslatef(-300.0f, 7.0f, -280.0f);
	glScalef(BUILDINGSCALE, BUILDINGSCALE, BUILDINGSCALE);
	drawBuilding1();

	glLoadIdentity();
	glTranslatef(-320.0f, 7.0f, -150.0f);	
	glRotatef(45, 0.0f, 1.0f, 0.0f);
	glScalef(BUILDINGSCALE, BUILDINGSCALE, BUILDINGSCALE);
	drawBuilding1();

	glLoadIdentity();
	glTranslatef(-160.0f, 7.0f, -210.0f);
	glRotatef(-20, 0.0f, 1.0f, 0.0f);
	glScalef(BUILDINGSCALE, BUILDINGSCALE, BUILDINGSCALE);
	drawBuilding1();

	glLoadIdentity();
	glTranslatef(-160.0f, 7.0f, -290.0f);
	glScalef(BUILDINGSCALE, BUILDINGSCALE, BUILDINGSCALE);
	drawBuilding3();

	glLoadIdentity();
	glTranslatef(-85.0f, 7.0f, -200.0f);
	glRotatef(45, 0.0f, 1.0f, 0.0f);
	glScalef(BUILDINGSCALE, BUILDINGSCALE, BUILDINGSCALE);
	drawBuilding3();

	glLoadIdentity();
	glTranslatef(-240.0f, 7.0f, -250.0f);	
	glRotatef(-20, 0.0f, 1.0f, 0.0f);
	glScalef(BUILDINGSCALE, BUILDINGSCALE, BUILDINGSCALE);
	drawBuilding3();

	glLoadIdentity();
	glTranslatef(-240.0f, 7.0f, -380.0f);
	glScalef(BUILDINGSCALE, BUILDINGSCALE, BUILDINGSCALE);
	drawBank();

	glLoadIdentity();
	glTranslatef(-180.0f, 7.0f, -120.0f);	
	glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
	glScalef(BUILDINGSCALE, BUILDINGSCALE, BUILDINGSCALE);
	drawBank();

	glLoadIdentity();
	glTranslatef(-90.0f, 7.0f, -320.0f);
	glRotatef(-20.0f, 0.0f, 1.0f, 0.0f);
	glScalef(BUILDINGSCALE, BUILDINGSCALE, BUILDINGSCALE);
	drawBank();












	//======================================== PREDIOS ========================================
	
	glLoadIdentity();
	glTranslatef(200.0f, 7.0f, -280.0f);
	glScalef(BUILDINGSCALE, BUILDINGSCALE, BUILDINGSCALE);
	drawBuilding1();

	glLoadIdentity();
	glTranslatef(180.0f, 7.0f, -150.0f);	
	glRotatef(45, 0.0f, 1.0f, 0.0f);
	glScalef(BUILDINGSCALE, BUILDINGSCALE, BUILDINGSCALE);
	drawBuilding1();

	glLoadIdentity();
	glTranslatef(340.0f, 7.0f, -210.0f);
	glRotatef(-20, 0.0f, 1.0f, 0.0f);
	glScalef(BUILDINGSCALE, BUILDINGSCALE, BUILDINGSCALE);
	drawBuilding1();

	glLoadIdentity();
	glTranslatef(340.0f, 7.0f, -290.0f);
	glScalef(BUILDINGSCALE, BUILDINGSCALE, BUILDINGSCALE);
	drawBuilding3();

	glLoadIdentity();
	glTranslatef(415.0f, 7.0f, -200.0f);
	glRotatef(45, 0.0f, 1.0f, 0.0f);
	glScalef(BUILDINGSCALE, BUILDINGSCALE, BUILDINGSCALE);
	drawBuilding3();

	glLoadIdentity();
	glTranslatef(260.0f, 7.0f, -250.0f);	
	glRotatef(-20, 0.0f, 1.0f, 0.0f);
	glScalef(BUILDINGSCALE, BUILDINGSCALE, BUILDINGSCALE);
	drawBuilding3();

	glLoadIdentity();
	glTranslatef(260.0f, 7.0f, -380.0f);
	glScalef(BUILDINGSCALE, BUILDINGSCALE, BUILDINGSCALE);
	drawBank();

	glLoadIdentity();
	glTranslatef(320.0f, 7.0f, -120.0f);	
	glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
	glScalef(BUILDINGSCALE, BUILDINGSCALE, BUILDINGSCALE);
	drawBank();

	glLoadIdentity();
	glTranslatef(410.0f, 7.0f, -320.0f);
	glRotatef(-20.0f, 0.0f, 1.0f, 0.0f);
	glScalef(BUILDINGSCALE, BUILDINGSCALE, BUILDINGSCALE);
	drawBank();












	glLoadIdentity();
	glTranslatef(0.0f, 1.0f, 697.0f);
	glScalef(3.0f, 3.0f, 3.0f);
	glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
	drawHangar();
}

void drawAirplane(void)
{
	if (!airplane) 
	{
		// this is the call that actualy reads the OBJ and creates the model object
        airplane = glmReadOBJ("models//airplane//airplane.obj");	
        if (!airplane) exit(0);
        glmUnitize(airplane);
        glmFacetNormals(airplane);        
		glmVertexNormals(airplane, 90.0);
    }
		glmDraw(airplane, GLM_SMOOTH | GLM_TEXTURE);
		glEnable(GL_COLOR_MATERIAL);
}

void drawHangar(void)
{
	if (!hangar) 
	{
        hangar = glmReadOBJ("models//hangar//hangar.obj");	
        if (!hangar) exit(0);
        glmUnitize(hangar);
        glmFacetNormals(hangar);        
		glmVertexNormals(hangar, 90.0);
    }
    glmDraw(hangar, GLM_SMOOTH | GLM_TEXTURE | GLM_MATERIAL);
	glEnable(GL_COLOR_MATERIAL);
}

void drawBank(void)
{
	if (!bank) 
	{
        bank = glmReadOBJ("models//bank//bank.obj");	
        if (!bank) exit(0);
        glmUnitize(bank);
        glmFacetNormals(bank);        
		glmVertexNormals(bank, 90.0);
    }

    glmDraw(bank, GLM_SMOOTH | GLM_TEXTURE | GLM_MATERIAL);
	glEnable(GL_COLOR_MATERIAL);
}

void drawStadium(void)
{
	if (!stadium) 
	{
        stadium = glmReadOBJ("models//stadium//stadium.obj");	
        if (!stadium) exit(0);
        glmUnitize(stadium);
        glmFacetNormals(stadium);        
		glmVertexNormals(stadium, 90.0);
    }
    glmDraw(stadium, GLM_SMOOTH | GLM_TEXTURE | GLM_MATERIAL);
	glEnable(GL_COLOR_MATERIAL);
}

void drawBuilding1(void)
{
	if (!building1) 
	{
        building1 = glmReadOBJ("models//building1//equitable.obj");	
        if (!building1) exit(0);
        glmUnitize(building1);
        glmFacetNormals(building1);        
		glmVertexNormals(building1, 90.0);
    }
    glmDraw(building1, GLM_SMOOTH | GLM_TEXTURE | GLM_MATERIAL);
	glEnable(GL_COLOR_MATERIAL);
}

void drawBuilding3(void)
{
	if (!building3) 
	{
        building3 = glmReadOBJ("models//building3//building3.obj");	
        if (!building3) exit(0);
        glmUnitize(building3);
        glmFacetNormals(building3);        
		glmVertexNormals(building3, 90.0);
    }
    glmDraw(building3, GLM_SMOOTH | GLM_TEXTURE | GLM_MATERIAL);
	glEnable(GL_COLOR_MATERIAL);
}

void drawBuilding4(void)
{
	if (!building4) 
	{
        building4 = glmReadOBJ("models//building4//building4.obj");	
        if (!building4) exit(0);
        glmUnitize(building4);
        glmFacetNormals(building4);        
		glmVertexNormals(building4, 90.0);
    }
    glmDraw(building4, GLM_SMOOTH | GLM_TEXTURE | GLM_MATERIAL);
	glEnable(GL_COLOR_MATERIAL);
}

void refreshCamera(void)
{
	switch(cameraType)
	{
		case PERSPECTIVE:
			setPerspectiveView();
			break;
		
		case ORTOGRAPHIC:
			setOrtographicView();
			break;

		case THIRDPERSON:
			setThirdPersonView();
			break;

		case ANOTHERPLANE:
			setAnotherPlaneView();
			break;
	}

}

void setThirdPersonView(void)
{
	float upAxis;
	if((rotateAngle[X]>90 && rotateAngle[X]<=270) || (rotateAngle[X]<-90 && rotateAngle[X]>=-270))
		upAxis=-1;
	else
		upAxis=1;

	glViewport (0, 0, (GLsizei) viewPortWidth, (GLsizei) viewPortHeight); 
	glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(60.0, (GLfloat) viewPortWidth/(GLfloat) viewPortHeight, 0.1, 2000.0);
	glTranslatef(0,0,-2);
	gluLookAt(planePosition[X],planePosition[Y],planePosition[Z],planePosition[X]+pointVector[X],planePosition[Y]+pointVector[Y], planePosition[Z]+pointVector[Z],0,upAxis,0);
	glMatrixMode (GL_MODELVIEW);
}

void setAnotherPlaneView(void)
{
	glViewport (0, 0, (GLsizei) viewPortWidth, (GLsizei) viewPortHeight); 
	glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(60.0, (GLfloat) viewPortWidth/(GLfloat) viewPortHeight, 0.1, 2000.0);
	glTranslatef(0,0,-10);
	gluLookAt(planePosition[X],planePosition[Y],planePosition[Z],planePosition[X],planePosition[Y]-1, planePosition[Z],0,0,1); //Camera acompanha o aviao
	//gluLookAt(planePosition[X],planePosition[Y],planePosition[Z],planePosition[X],planePosition[Y]-1, planePosition[Z],pointVector[X],0,pointVector[Z]); //Camera acompanha o aviao e gira com ele
	glMatrixMode (GL_MODELVIEW);
}

void setPerspectiveView(void)
{
	float upAxis;
	if((rotateAngle[X]>90 && rotateAngle[X]<=270) || (rotateAngle[X]<-90 && rotateAngle[X]>=-270))
		upAxis=-1;
	else
		upAxis=1;

	glViewport (0, 0, (GLsizei) viewPortWidth, (GLsizei) viewPortHeight); 
	glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(60.0, (GLfloat) viewPortWidth/(GLfloat) viewPortHeight, 0.1, 2000.0);
	glTranslatef(0,0,1);
	//glRotatef(rotateAngle[Z],pointVector[X],pointVector[Y],pointVector[Z]); //Tentativa de fazer a camera girar
	//gluLookAt(planePosition[X],planePosition[Y],planePosition[Z],planePosition[X]+pointVector[X],planePosition[Y]+pointVector[Y], planePosition[Z]+pointVector[Z],upVector[X],upVector[Y],upVector[Z]); //Tentativa de fazer a camera girar
	gluLookAt(planePosition[X],planePosition[Y],planePosition[Z],planePosition[X]+pointVector[X],planePosition[Y]+pointVector[Y], planePosition[Z]+pointVector[Z],0,upAxis,0);
	glMatrixMode (GL_MODELVIEW);
}

void setOrtographicView(void)

{
	float orthoPoints;
	glViewport (0, 0, (GLsizei) viewPortWidth, (GLsizei) viewPortHeight); 
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	orthoPoints = MAXHEIGHT/2;
	glOrtho(-orthoPoints,orthoPoints,-orthoPoints,orthoPoints,MAXHEIGHT*30/100,MAXHEIGHT+1);
	gluLookAt (0,MAXHEIGHT,0,0,0,0,1,0,1);
	glMatrixMode (GL_MODELVIEW);
}
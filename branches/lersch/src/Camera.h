#include <GL/glut.h>

#pragma once
class Camera
{
private:
	GLfloat eye[3];
	GLfloat up[3];
	GLfloat side[3];
	GLfloat look[3];

public:
	Camera(void);
	~Camera(void);
	void Update_glMatrix(void);
	void Translate(float x, float y, float z);
};


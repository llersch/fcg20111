#include "Camera.h"


Camera::Camera(void)
{
	eye[0] = 0.0f; eye[1] = 0.1f; eye[2] = 0.0f;

	  up[0] = 0.0f;   up[1] = 1.0f;   up[2] = 0.0f;
	side[0] = 1.0f; side[1] = 0.0f; side[2] = 0.0f;
	look[0] = 1.0f; look[1] = 0.0f; look[2] = 0.0f;

}


Camera::~Camera(void)
{
}

void Camera::Update_glMatrix(void)
{
	glLoadIdentity();
	gluLookAt( eye[0], eye[1], eye[2],
			   look[0], look[1], look[2],
			   up[0]-eye[0], up[1]-eye[1], up[2]-eye[2]);
}

void Camera::Translate(float x, float y, float z)
{
	GLfloat matrix[3][3] = {
		{side[0]-eye[0], up[0]-eye[0], look[0]-eye[0]},
		{side[1]-eye[1], up[1]-eye[1], look[1]-eye[1]},
		{side[2]-eye[2], up[2]-eye[2], look[2]-eye[2]}
	};

	GLfloat vector[3] = {x,y,-z};

	GLfloat result[3];
	result[0] = matrix[0][0]*vector[0] + matrix[0][1]*vector[1] + matrix[0][2]*vector[2];
	result[1] = matrix[1][0]*vector[0] + matrix[1][1]*vector[1] + matrix[1][2]*vector[2];
	result[2] = matrix[2][0]*vector[0] + matrix[2][1]*vector[1] + matrix[2][2]*vector[2];

	eye[0] += result[0];
	eye[1] += result[1];
	eye[2] += result[2];

	up[0] += result[0];
	up[1] += result[1];
	up[2] += result[2];

	side[0] += result[0];
	side[1] += result[1];
	side[2] += result[2];

	look[0] += result[0];
	look[1] += result[1];
	look[2] += result[2];

	this->Update_glMatrix();
}


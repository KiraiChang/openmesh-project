#ifndef _M_OPEN_GL_H_
#define _M_OPEN_GL_H_

#include <windows.h>
#include <GL/glew.h>
#include <GL/glut.h>
#define GLM_SWIZZLE
#include <glm\glm.hpp>
#include <math.h>
/*####################################################################*/
/*############################   Math   ##############################*/
/*####################################################################*/

#define MATH_PI 3.141592653f

template <class T>
void sincos (const T &radin, T &sinValue, T &cosValue)
{
	sinValue = sin (radin);
	cosValue = cos (radin);
}

/*####################################################################*/
/*###########################   MOpenGL   ############################*/
/*####################################################################*/

class MOpenGL
{
public:
	MOpenGL(void); 
	~MOpenGL(void);
	bool init(HDC hdc);
	void initOpenGL(void);
	void initScene(int x, int y, int width, int height);
	void renderScene(void);
	void swapBuffers(void);
	void calcCamera(float mx, float my);
public:
	glm::vec3 m_eye;
	glm::vec3 m_lookat;
	glm::vec3 m_up;
	float m_near;
	float m_far;
private:
	HDC m_hDC; // device context handler
	HGLRC m_rc; // render context handler

	// -------------------- lights
	GLfloat gLightDiffuse[4]; // light diffuse
	GLfloat gLightPosition[4]; // light position

public:
	time_t m_currentTime;
	float m_theta;
	float m_phi;
	int m_currentX;
	int m_currentY;
};

/*####################################################################*/
/*#############################   End   ##############################*/
/*####################################################################*/

#endif
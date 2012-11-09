#include "stdafx.h"
#include "m_open_gl.h"
#include <iostream>
#include <time.h>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform2.hpp>

/*####################################################################*/
/*###########################   MOpenGL   ############################*/
/*####################################################################*/

// `���r��Ϊ�8����u`
glm::vec4 g_vertices[] = 
{
	glm::vec4(-1.0f, 1.0f,-1.0f, 1.0),
	glm::vec4(-1.0f,-1.0f,-1.0f, 1.0),

	glm::vec4(-1.0f,-1.0f,-1.0f, 1.0),
	glm::vec4( 1.0f,-1.0f,-1.0f, 1.0),

	glm::vec4( 1.0f,-1.0f,-1.0f, 1.0),
	glm::vec4( 1.0f, 1.0f,-1.0f, 1.0),

	glm::vec4( 1.0f, 1.0f,-1.0f, 1.0),
	glm::vec4(-1.0f, 1.0f,-1.0f, 1.0),

	glm::vec4( 0.0f, 0.0f, 1.0f, 1.0),
	glm::vec4(-1.0f, 1.0f,-1.0f, 1.0),

	glm::vec4( 0.0f, 0.0f, 1.0f, 1.0),
	glm::vec4(-1.0f,-1.0f,-1.0f, 1.0),

	glm::vec4( 0.0f, 0.0f, 1.0f, 1.0),
	glm::vec4( 1.0f,-1.0f,-1.0f, 1.0),

	glm::vec4( 0.0f, 0.0f, 1.0f, 1.0),
	glm::vec4( 1.0f, 1.0f,-1.0f, 1.0),
};

// `���Y��m`
glm::vec3 g_eye(0.0f, 3.0f, 3.0f); 
// `���Y��Ǫ��I`
glm::vec3 g_lookat(0.0f, 0.0f, 0.0f); 
// `���Y���W�誺��V`
glm::vec3 g_up(0.0f, -1.0f, 1.0f);

//Grid size(left, bottom, right, up)
glm::vec4 g_gridSize( -50.0f, -50.0f, 50.0f, 50.0);

MOpenGL::MOpenGL():m_rc(NULL),
	m_near(-1.0),
	m_far(1.0),
	m_theta(-MATH_PI * 0.5f),
	m_phi(0.0f),
	m_hDC(NULL)
{

}

MOpenGL::~MOpenGL(void)
{
	if(m_rc != NULL)
	{
		wglMakeCurrent(0,0);
		wglDeleteContext(m_rc);
	}
}

bool MOpenGL::init(HDC hdc)
{
	m_hDC = hdc;

	// Set pixel format
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;

	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int cpf = ChoosePixelFormat(m_hDC, &pfd);
	if(!cpf)
	{
		std::cerr << "Choose Pixel Format Error\n";
		return false;
	}

	SetPixelFormat(m_hDC, cpf, &pfd);

	m_rc = wglCreateContext(m_hDC);
	if(!m_rc)
	{
		std::cerr << "Create Render Context Error\n";
		return false;
	}

	if(!wglMakeCurrent(m_hDC, m_rc))
	{
		std::cerr << "wglMakeCurrent Error\n";
		return false;
	}

	GLenum err = glewInit();
	if(GLEW_OK != err)
	{
		fprintf(stderr, "Error initializing GLEW :%s\n", glewGetErrorString(err));
	}

	return true;
}

void MOpenGL::initOpenGL()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // White Background
	//glColor3f(0.0, 0.0, 0.0); // ���e�C��: ��

	/*******************************************************/
	// �����]�w
	// light diffuse (���g��): ����
	gLightDiffuse[0] = 1.0;
	gLightDiffuse[1] = 0.0;
	gLightDiffuse[2] = 0.0;
	gLightDiffuse[3] = 1.0;

	// light position (������m)
	gLightPosition[0] = 1.0;
	gLightPosition[1] = 1.0;
	gLightPosition[2] = 0.0;
	gLightPosition[3] = 1.0;

	// �ҥβĤ@�ӥ����]�w
	glLightfv(GL_LIGHT0, GL_DIFFUSE, gLightDiffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, gLightPosition);
	glEnable(GL_LIGHT0);
	/*******************************************************/

	glClearDepth(1.0f); // Depth Buffer Setup
	glEnable(GL_DEPTH_TEST); // Enables Depth Testing
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH); // Enable Smooth Shading

	glDepthFunc(GL_LEQUAL); // The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void MOpenGL::initScene(int x, int y, int w, int h)
{
	glViewport(x, y, w, h);

	// `�p��ó]�w�����ഫ�x�}`
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0f, (GLdouble)w/h, 1.0f, 100.0f);
	//gluOrtho2D(-(GLdouble)h/h, (GLdouble) h/h, -(GLdouble) h/h, (GLdouble) h/h);

	// `�p��ó]�w���Y�ഫ�x�}`
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		g_eye[0], g_eye[1], g_eye[2],
		g_lookat[0], g_lookat[1], g_lookat[2],
		g_up[0], g_up[1], g_up[2]);
}

void MOpenGL::renderScene()
{
	// Clear Screen And Depth Buffer    
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    
	//glLoadIdentity(); // Reset The Current Modelview Matrix

	// -------------------- �ҥΥ����]�w
	glEnable(GL_LIGHTING);

	glColor3f(1.0, 0.0, 0.0);
	glLineWidth(1);
	// `�e�X���r��8����u`
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(4, GL_FLOAT, sizeof(glm::vec4), g_vertices);
	glDrawArrays(GL_LINES, 0, 16);

	// 3. �A�첾
	//glTranslatef(3.0f,0.0f,0.0f);    // Move Right 3 Units

	// 2. �A���ਤ��
	//m_rAngle = m_rAngle + 1;
	//if(360 < m_rAngle) m_rAngle -= 360;
	//glRotatef(m_rAngle, 0.0f, 1.0f, 0.0f); // �� Y �b(�����b)�@����

	// 1. ���e��
	//glPointSize(1.0f);
	//glColor3f(0.4, 0.6, 0.7);
	//glutWireTeapot(0.5); 

	// -------------------- ���Υ����]�w
	glDisable(GL_LIGHTING);

	/* Render grid over 0..rows, 0..columns. */
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINES);
	/* Horizontal lines. */
	for (int i=g_gridSize.x; i<=g_gridSize.z; i++) {
		glVertex2f(g_gridSize.y, i);
		glVertex2f(g_gridSize.w, i);
	}
	/* Vertical lines. */
	for (int i=g_gridSize.y; i<=g_gridSize.w; i++) {
		glVertex2f(i, g_gridSize.x);
		glVertex2f(i, g_gridSize.z);
	}
	glEnd();
}

void MOpenGL::swapBuffers(void)
{
	SwapBuffers(m_hDC);
}
void MOpenGLMouseMove(float mx, float my)
{

}
//void MOpenGL::calcCamera(float mx, float my)
//{
//	time_t ti;
//	time(&ti);
//	float time_diff = ti - m_currentTime;
//	time_diff /= 10000;
//	m_currentTime = ti;
//
//	float moving_speed = 2.0f * time_diff;
//	float rotation_speed = 1.0 * time_diff;
//
//	// `���y�Шt��`
//	m_theta += (m_currentX - mx )* rotation_speed;
//	m_currentX = mx;
//	m_phi -= (m_currentY - my) * rotation_speed;
//	m_currentY = my;
//	float sin_phi, cos_phi;
//	float sin_theta, cos_theta;
//
//	sincos<float>(m_phi, sin_phi, cos_phi);
//	sincos<float>(m_theta, sin_theta, cos_theta);
//
//	// `�p�����Y�����V`
//	glm::vec3 camera_facing;
//
//	camera_facing[0] = cos_phi * cos_theta;
//	camera_facing[1] = sin_phi;
//	camera_facing[2] = cos_phi * sin_theta;
//
//	// `�p�����Y���W�誺�b�V`
//	glm::vec3 camera_up;
//	sincos<float>(m_phi + MATH_PI*0.5f, sin_phi, cos_phi);
//
//	camera_up[0] = cos_phi * cos_theta;
//	camera_up[1] = sin_phi;
//	camera_up[2] = cos_phi * sin_theta;
//
//	// `���o�譱�k�誺��V`
//	glm::vec3 camera_right = glm::cross(camera_facing, camera_up);
//
//	// `�p��X���Y��Ǫ��I, �������Y�ഫ�x�}�ɷ|�Ψ�.`
//	g_lookat = g_eye + camera_facing;
//	// `�]���O��2�Ӷb���, �ݭn��s���Y�¤W���b.`
//	g_up = camera_up;
//	// `�p��ó]�w���Y�ഫ�x�}`
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//	gluLookAt(
//		g_eye[0], g_eye[1], g_eye[2],
//		g_lookat[0], g_lookat[1], g_lookat[2],
//		g_up[0], g_up[1], g_up[2]);
//}

/*####################################################################*/
/*#############################   End   ##############################*/
/*####################################################################*/
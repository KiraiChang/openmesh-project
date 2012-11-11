#pragma once
#ifndef _DP_H_
#define _DP_H_

#include "GUA_OM.h"
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "GLCamera.h"
#include "XForm.h"
#define MAX_LAYERS 10
#include <vector>

typedef std::vector<int> VECTOR_LAYER;

class DP_COM
{

public:
	float*					m_pZBuffer;			//�x�s�C�@���h��ZBuffer��       
	bool*					m_ValidBuffer;		//��Pixel�O�_�s�bmesh���P�_buffer�A�P�_�[�t��
	VECTOR_LAYER			m_vLayer;			//���X�h�i�H�]���[�����I
	int						m_curPixelX, m_curPixelY;
	int						m_sceneWidth, m_sceneHeight;

	//---------------function---------------//
	DP_COM();
	~DP_COM();
	bool Scene_Init( );
	void Set_BufferObj(  int, int  );
	void Clear_Buffer( );
	void Peeling_layer(  int, int, int,  Tri_Mesh*, GLCamera, xform) ;
	void Check_Layers(int x, int y);
	size_t Get_Layer_Count();
	float Get_Depth(int selectLayer);
	void Get_Pixel_Position(float &x, float &y);

protected:
private:

};


#endif
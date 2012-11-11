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
	float*					m_pZBuffer;			//儲存每一階層的ZBuffer值       
	bool*					m_ValidBuffer;		//此Pixel是否存在mesh的判斷buffer，判斷加速用
	VECTOR_LAYER			m_vLayer;			//有幾層可以設骨架控制點
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
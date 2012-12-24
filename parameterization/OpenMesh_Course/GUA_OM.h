#pragma once
#ifndef _GUA_OM_H_
#define _GUA_OM_H_

#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <OpenMesh/Tools/Utils/getopt.h>
#include <OpenMesh/Core/Geometry/QuadricT.hh>

#include <windows.h>
#include "glew.h"
#include <gl/gl.h>
#include <gl/glu.h>
//#include <gl/glut.h>
//#include <gl/glaux.h>
#include <highgui.h> 
//#include "GLCamera.h"

#define INIT_DIST 9999.9f

struct TextureInfo
{
	std::vector<OpenMesh::Vec2d> UVs;
	GLuint texGID[2];	//for 2 panel
	cv::Mat	imgMat;
	TextureInfo(){}
};

struct TextureForFace
{
	TextureInfo* texInfo;
	int uvmap[3];
	TextureForFace* nextTexture;
	TextureForFace() : texInfo(NULL), nextTexture(NULL)
	{
		uvmap[0] = uvmap[1] = uvmap[2] = -1;
	}
};

struct Face_InnerAngle
{
	double Vertex_Angle[3];
};

class AllPairHarVal_class
{
public:  
	std::vector<double> Har_val;
};

namespace OMT//OpenMesh Triangle mesh
{
	using namespace std;
	/*----------------------------------------------------------------------*/

	/*定義使用的精準度和基本屬性*/
	struct MyTraits : OpenMesh::DefaultTraits
	{
		// let Point and Normal be a vector made from doubles
		typedef OpenMesh::Vec3d Point;
		typedef OpenMesh::Vec3d Normal;

		// add normal property to vertices and faces
		VertexAttributes(OpenMesh::Attributes::Normal|OpenMesh::Attributes::Status);
		FaceAttributes  (OpenMesh::Attributes::Normal|OpenMesh::Attributes::Status);
		EdgeAttributes  (OpenMesh::Attributes::Status);

		// Already defined in OpenMesh::DefaultTraits
		// HalfedgeAttributes( OpenMesh::Attributes::PrevHalfedge );

		// Uncomment next line to disable attribute PrevHalfedge
		// HalfedgeAttributes( OpenMesh::Attributes::None );
		//
		// or
		//
		// HalfedgeAttributes( 0 );
	};
	/*----------------------------------------------------------------------*/

	/*定義常用type*/
	typedef OpenMesh::TriMesh_ArrayKernelT<MyTraits>	    MyMesh	;
	typedef OpenMesh::Vec3d									Vector3d;	//Vec3D type
	typedef MyMesh::Scalar									Scalar	;	//Scalar type
	typedef MyMesh::Point									Point	;	//Point type
	typedef MyMesh::Normal									Normal	;	//Normal type
	typedef MyMesh::VertexHandle							VHandle	;	//VertexHandle type
	typedef MyMesh::HalfedgeHandle							HEHandle;	//HalfedgeHandle type
	typedef MyMesh::EdgeHandle							    EHandle ;	//edgeHandle type
	typedef MyMesh::FaceHandle								FHandle	;	//FaceHandle type
	//-------------Vertex iterators & circulators-------------
	typedef MyMesh::VertexIter								VIter	;	//VertexIter type
	typedef MyMesh::VertexVertexIter						VVIter	;	//VertexVertexIter type
	typedef MyMesh::VertexEdgeIter							VEIter	;	//VertexEdgeIter type
	typedef MyMesh::VertexFaceIter							VFIter	;	//VertexFaceIter type
	typedef MyMesh::EdgeIter								EIter	;	//EdgeIterT	type
	typedef MyMesh::FaceIter								FIter	;	//FaceIter type
	typedef MyMesh::FaceVertexIter							FVIter	;	//FaceVertexIter type
	typedef MyMesh::FaceEdgeIter							FEIter	;	//FaceEdgeIter type
	typedef MyMesh::FaceFaceIter							FFIter	;	//FaceFaceIter type
	typedef MyMesh::VertexOHalfedgeIter						VOHEIter;	//VertexOutHalfEdge type
	typedef MyMesh::ConstVertexVertexIter					CVVIter	;	//ConstVertexVertexIter type
	/*----------------------------------------------------------------------*/
	
	/*定義額外資料結構*/
	using namespace OpenMesh;
	//指定特別畫出面的資料結構
	struct sp_f 
	{
		FHandle fh;
		float r, g, b;
	};
	//指定特別畫出頂點的資料結構
	struct sp_v 
	{
		VHandle vh;
		float r, g, b;
	};
	//指定另外畫出位置的資料結構
	struct sp_p
	{
		Point pt;
		float r, g, b;
	};
	//
	typedef std::vector<VHandle> V_ONE_RING_HANLDE;
	struct DELETE_HISTORY
	{
		VHandle fromVertex;
		V_ONE_RING_HANLDE fromOnering;
		VHandle toVertex;
		V_ONE_RING_HANLDE toOnering;
		VHandle newVertex;
	};
	typedef std::vector<DELETE_HISTORY> V_DELETE_HISTORY;

	typedef std::vector< sp_p >			SP_POINT_LIST;
	typedef std::vector< sp_v >			SP_VERTEX_LIST;
	typedef std::vector< sp_v >			SP_EDGE_LIST;
	typedef std::vector< sp_f >			SP_FACE_LIST;

	typedef std::map<size_t, 
		OpenMesh::Geometry::Quadricd >	QUADRICES;
	typedef std::pair<int, int>			PAIR;
	typedef std::map<PAIR, double>		ERRORS;
	/*----------------------------------------------------------------------*/
	bool sameSide(const Point &p1, const Point &p2, const Point &p3, const Point &p4);
	bool pointInTrangle(const Point &p1, const Point &tv1, const Point &tv2, const Point &tv3);
	double distance(const Point &p1, const Point &p2);
	double area(const Point &v1, const Point &v2);
	double distanceL(const Point &p, const Point &from, const Point &to);
	double det(OpenMesh::Geometry::Quadricd q, int a11, int a12, int a13,
												int a21, int a22, int a23,
												int a31, int a32, int a33);
	double vertexError(OpenMesh::Geometry::Quadricd q, double x, double y, double z);
	/*定義類別*/
	const size_t MAX_TEXTURE = 1;
	class Model:public MyMesh
	{
	private:
		SP_POINT_LIST					sp_p_list;
		SP_VERTEX_LIST					sp_v_list;
		SP_EDGE_LIST					sp_e_list;
		SP_FACE_LIST					sp_f_list;
		SP_FACE_LIST					sp_mapping_f_list;
		SP_VERTEX_LIST					sp_uv_mapping_list;
		QUADRICES						quadrics;
		ERRORS							errors;
		V_DELETE_HISTORY				vDeleteHistory;
		size_t							number_of_face;

		//GLuint							m_uiTexture[MAX_TEXTURE];				// Storage For One Texture
		////IplImage *						m_pImage[MAX_TEXTURE];
		//cv::Mat							m_matImage[MAX_TEXTURE];
		OpenMesh::FPropHandleT<bool>	f_bIsSelect;
		OpenMesh::VPropHandleT<Vec2d>	v_vec2dTexcoord;
		std::vector<TextureInfo*>		m_TexInfos;
		TextureInfo*					m_CurEditTex;
		unsigned int					m_uiCurVertex;

	public:
										Model();//constructor
										~Model();//de-constructor

		void							initQuadrices();//初始化QuadricMatrix.

		void							RenderSpecifiedPoint();//畫出指定位置的點
		void							RenderSpecifiedVertex();	//畫出指定的頂點
		void							RenderSpecifiedEdge();	//畫出指定的頂點
		void							RenderSpecifiedFace();		//畫出指定的面

		void							clear_sp_p();//清空額外畫出的點
		void							clear_sp_v();//清空額外畫出的頂點
		void							clear_sp_e();//清空額外畫出的邊
		void							clear_sp_f();//清空額外畫出的面

		void							add_sp_p(const Point &_p, float _r, float _g, float _b);//指定額外畫出的點
		void							add_sp_v(const VHandle &_v, float _r, float _g, float _b);//指定額外畫出的頂點
		void							add_sp_e(const VHandle &_p1, const VHandle &_p2, float _r, float _g, float _b);//指定額外畫出的邊
		void							add_sp_f(const FHandle &_f, float _r, float _g, float _b);//指定額外畫出的面

		double							findVertex(const Point &p, VHandle &handle);
		double							findHalfEdge(const Point &p, HEHandle &handle);
		double							findFace(const Point &p, FHandle &handle);

		/*---------------------------------SIMPLIFY-------------------------------------*/
		bool							isConvex(vector<VHandle> &polygon);
		double							calculateError(int id_v1, int id_v2, double* vx = NULL, double* vy = NULL, double* vz = NULL);
		void							selectPair();
		DELETE_HISTORY					undoSimplification(void);
		void							undoSimplification(int target_num_faces);
		void							simplification(double rate);
		void							simplification(int target_num_faces);
		int								simplification(int id_v1, int id_v2, double vx, double vy, double vz);
		void							updateErrors(int idx);

		/*---------------------------------TEXTURE--------------------------------------*/
		//AUX_RGBImageRec *				LoadBMP(const char *filename);
		//IplImage*						LoadImage(const char* filename, int flags);
		int								LoadGLTextures(const std::string &tex_name);
		int								LoadImage(const std::string &tex_name, size_t image_id);
		int								GenTextures(int panelID);
		int								GenTextures(size_t texture_id, size_t image_id);
		void							RenderTexture(void);
		void							RenderUVMapping(void);
		void							drawRect(int _x,  int _y, float _w, float _h, GLint viewport[4], GLdouble modelview[16], GLdouble projection[16]);
		void							drawCircle(float _x,  float _y, float _radin, float _r, float _g, float _b, int _n);
		void							VertexMapping(const FHandle &_f);
		void							clear_sp_mapping(void);
		void							add_sp_mapping(const VHandle &_v, float _r, float _g, float _b);
		void							selectFace(int _x,  int _y, float _u, float _r, float _d, float _l, GLint viewport[4], GLdouble modelview[16], GLdouble projection[16]);
		void							SelectNring(int n, int _x,  int _y,  float radin);
		void							Selected(void);
		void							RenderSelectVertex(void);
		void							RenderSelectFace(void);
		void							add_mapping_face(FHandle &_f);
		void							clear_mapping_face(void);
		bool							selectUVVertex(float u, float v);
		void							moveUVVertex(float u, float v);
		void							renderSelectPoint(void);

		/*---------------------------------PARAMETERIZATION-----------------------------*/
	private:
		OpenMesh::FPropHandleT<int>       SelRingID;
		OpenMesh::VPropHandleT<int>		  SelVID;		//positive : inside, negative : bound, other : not selected
		OpenMesh::FPropHandleT<TextureForFace*>	fTex;
		std::vector<int> centerVid;
		vector< FHandle > sel_faces;
		vector< VHandle > bound_Vex;
		vector< Vec2d > BoundVexIn2D;
		vector< Vec2d > CenterVexIn2D;
	public:
		void InitModelProperty();
		double RayTraceFace(FHandle& fh, Point& p, Vector3d& rayDir);
		FHandle FindFace( Point& p, Vector3d& rayDir );
		void SelectNring(int n, Point& p, Vector3d& rayDir);
		void ClearSelectFaces();
		void FindBound();
		void FixBoundShape();
		void MapBoundTo2D();
		void FillCenter();
		void Parameterization();
		double calCotWeight(VHandle vh1, VHandle vh2);
		void RenderBound2D( float r, float g, float b );
		void RenderTextureToModel();
		bool IsBoundEdge(EHandle eh)
		{
			HEHandle heh = halfedge_handle(eh, 0);
			FHandle fh1 = face_handle( heh );
			FHandle fh2 = opposite_face_handle( heh );
			if ( property(SelRingID, fh1) * property(SelRingID, fh2)<0 )
				return true;

			return false;
		}
	};
}
/*======================================================================*/

namespace OMP//OpenMesh Polygonal mesh
{
	using namespace std;
	/*----------------------------------------------------------------------*/

	/*定義使用的精準度和基本屬性*/
	struct MyTraits : OpenMesh::DefaultTraits
	{
		// let Point and Normal be a vector made from doubles
		typedef OpenMesh::Vec3d Point;
		typedef OpenMesh::Vec3d Normal;

		// add normal property to vertices and faces
		VertexAttributes(OpenMesh::Attributes::Normal);
		FaceAttributes  (OpenMesh::Attributes::Normal);

		// Already defined in OpenMesh::DefaultTraits
		// HalfedgeAttributes( OpenMesh::Attributes::PrevHalfedge );

		// Uncomment next line to disable attribute PrevHalfedge
		// HalfedgeAttributes( OpenMesh::Attributes::None );
		//
		// or
		//
		// HalfedgeAttributes( 0 );
	};
	/*----------------------------------------------------------------------*/

	/*定義常用type*/
	typedef OpenMesh::PolyMesh_ArrayKernelT<MyTraits>	    MyMesh	;
	typedef OpenMesh::Vec3d									Vector3d;	//Vec3D type
	typedef MyMesh::Scalar									Scalar	;	//Scalar type
	typedef MyMesh::Point									Point	;	//Point type
	typedef MyMesh::Normal									Normal	;	//Normal type
	typedef MyMesh::VertexHandle							VHandle	;	//VertexHandle type
	typedef MyMesh::HalfedgeHandle							HEHandle;	//HalfedgeHandle type
	typedef MyMesh::EdgeHandle							    EHandle ;	//edgeHandle type
	typedef MyMesh::FaceHandle								FHandle	;	//FaceHandle type
	//-------------Vertex iterators & circulators-------------
	typedef MyMesh::VertexIter								VIter	;	//VertexIter type
	typedef MyMesh::VertexVertexIter						VVIter	;	//VertexVertexIter type
	typedef MyMesh::VertexEdgeIter							VEIter	;	//VertexEdgeIter type
	typedef MyMesh::VertexFaceIter							VFIter	;	//VertexFaceIter type
	typedef MyMesh::EdgeIter								EIter	;	//EdgeIterT	type
	typedef MyMesh::FaceIter								FIter	;	//FaceIter type
	typedef MyMesh::FaceVertexIter							FVIter	;	//FaceVertexIter type
	typedef MyMesh::FaceEdgeIter							FEIter	;	//FaceEdgeIter type
	typedef MyMesh::FaceFaceIter							FFIter	;	//FaceFaceIter type
	typedef MyMesh::VertexOHalfedgeIter						VOHEIter;	//VertexOutHalfEdge type
	typedef MyMesh::ConstVertexVertexIter					CVVIter	;	//ConstVertexVertexIter type
	/*----------------------------------------------------------------------*/

	/*定義額外資料結構*/
	using namespace OpenMesh;
	//指定特別畫出面的資料結構
	struct sp_f 
	{
		FHandle fh;
		float r, g, b;
	};
	//指定特別畫出頂點的資料結構
	struct sp_v 
	{
		VHandle vh;
		float r, g, b;
	};
	//指定另外畫出位置的資料結構
	struct sp_p
	{
		Point pt;
		float r, g, b;
	};
	/*----------------------------------------------------------------------*/
	struct srtPath
	{
		std::vector<OMP::VHandle> path;
	};

	/*定義類別*/
	class Model:public MyMesh
	{
		//資料成員
	public:
		MyMesh Mesh;													//OpenMesh instance
	private:
		vector< sp_p > sp_p_list;
		vector< sp_v > sp_v_list;
		vector< sp_f > sp_f_list;
		//函式成員
	public:
		Model();//constructor
		~Model();//de-constructor

		bool ReadFile(std::string _fileName);//讀取mesh資料
		bool SaveFile(std::string _fileName);//儲存mesh資料

		void Render_solid();			//solid rendering
		void Render_wireframe();		//wireframe rendering

		void RenderSpecifiedPoint();	//畫出指定位置的點
		void RenderSpecifiedVertex();	//畫出指定的頂點
		void RenderSpecifiedFace();		//畫出指定的面

		void add_sp_p(Point   _p, float _r, float _g, float _b);//指定額外畫出的點
		void add_sp_v(VHandle _v, float _r, float _g, float _b);//指定額外畫出的頂點
		void add_sp_f(FHandle _f, float _r, float _g, float _b);//指定額外畫出的面
		void clear_sp_p();//清空額外畫出的點
		void clear_sp_v();//清空額外畫出的頂點
		void clear_sp_f();//清空額外畫出的面

		VHandle addVertex(Point _p);										//在model上增加新的頂點
		FHandle addFace(VHandle _v0, VHandle _v1, VHandle _v2, VHandle _v3);//在model上增加新的面
		void deleteFace(FHandle _f);										//在model上刪除面
		void deleteFace(VHandle _v0, VHandle _v1, VHandle _v2, VHandle _v3);//在model上刪除面

		bool IsVertexVertex( VHandle _vj, VHandle _vi);	//檢查兩頂點是否相鄰

		int quad_subdivision1(int _face_id);//1to4 quadrilateral subdivision
		int quad_subdivision2(int _face_id);//1to9 quadrilateral subdivision

	private:
		int findVertex(Point _p);//找出位置_p是否已經有頂點存在
	};
}
/*======================================================================*/
class Tri_Mesh:public OMT::Model
{
public:
	Tri_Mesh()
	{
	
	
	}
	//-------Edit Flag-------//
    bool                                       Delete_Flag;
	
	int                                        Constrain_num;
	int                                        Boundary_num ;
	OMT::VHandle                               start_vh,end_vh;
	OMT::VHandle                               ExtremeVh[2];
	int                                        PatchType;

	std::vector<OMT::VHandle>                  Pluspt      ;
	std::vector<OMT::VHandle>                  Minuspt     ;
	std::vector<OMT::VHandle>                  Extrme_Pt   ;


	void Render_Solid();
	void Render_No_Lighting_Solid();
	void Render_SolidWireframe();
	void Render_Wireframe();
	void Render_Point();
private:
};

///*======================================================================*/
/*======================================================================*/
bool ReadFile(std::string _fileName,Tri_Mesh *); //讀取mesh資料
//	bool SaveFile(std::string _fileName);//儲存mesh資料
/*初始化view port設定函式*/

#endif


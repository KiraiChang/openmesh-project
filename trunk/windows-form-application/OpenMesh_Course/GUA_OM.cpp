#include "stdafx.h"
#include "GUA_OM.h"





namespace OMT
{
	bool sameSide(const Point &p1, const Point &p2, const Point &p3, const Point &p4)
	{
		Point cp1 = cross(p4 - p3, p1 - p3);
		Point cp2 = cross(p4 - p3, p2 - p3);
		if(dot(cp1, cp2) >= 0)
			return true;
		return false;
	}

	bool pointInTrangle(const Point &p1, const Point &tv1, const Point &tv2, const Point &tv3)
	{
		if(sameSide(p1, tv1, tv2, tv3) && sameSide(p1, tv2, tv3, tv1) && sameSide(p1, tv3, tv1, tv2))
			return true;
		return false;
	}

	double distance(const Point &p1, const Point &p2)
	{
		//return	( p1[0] - p2[0]) * ( p1[0] - p2[0]) +
		//		( p1[1] - p2[1]) * ( p1[1] - p2[1]) +
		//		( p1[2] - p2[2]) * ( p1[2] - p2[2]);
		return (p1-p2).length();
	}

	double area(const Point &v1, const Point &v2)
	{
		Point co = cross(v1, v2);
		return co[0] + co[1] + co[2];
	}

	double distanceL(const Point &p, const Point &from, const Point &to)
	{
		Point v1 = p - from, v2 = to - from;
		return fabs(area(v1, v2))/v2.length();
	}

	/*======================================================================*/
	Model::Model()
	{
		request_vertex_status();
		request_edge_status();
		request_face_status();
	}
	Model::~Model()
	{
		release_vertex_status();
		release_edge_status();
		release_face_status();
	}

	void Model::RenderSpecifiedPoint()
	{
		glPushAttrib(GL_LIGHTING_BIT);
		glDisable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
		glPointSize(5.0f);
		glBegin(GL_POINTS);
		SP_POINT_LIST::iterator p_itr = sp_p_list.begin();
		for (p_itr; p_itr != sp_p_list.end(); ++p_itr)
		{
			glColor3f(p_itr->r, p_itr->g, p_itr->b);
			glVertex3dv(&p_itr->pt[0]);
		}
		glEnd();
		glEnable(GL_LIGHTING);
		glDisable(GL_POLYGON_OFFSET_FILL);
	}

	void Model::RenderSpecifiedVertex()
	{
		glPushAttrib(GL_LIGHTING_BIT);
		glDisable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
		glPointSize(5.0f);
		glBegin(GL_POINTS);
		SP_VERTEX_LIST::iterator v_itr = sp_v_list.begin();
		for (v_itr; v_itr != sp_v_list.end(); ++v_itr)
		{
			glColor3f(v_itr->r, v_itr->g, v_itr->b);
			glVertex3dv(&point(v_itr->vh)[0]);
		}
		glEnd();
		glEnable(GL_LIGHTING);
		glDisable(GL_POLYGON_OFFSET_FILL);
	}

	void Model::RenderSpecifiedEdge()
	{
		glDisable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		glPushAttrib(GL_LIGHTING_BIT);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(0.5, 1.0);
		glLineWidth(3.f);
		glDisable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);

		glBegin(GL_LINES);

		SP_EDGE_LIST::iterator v_itr = sp_e_list.begin();
		for (v_itr; v_itr != sp_e_list.end(); v_itr+=2)
		{
			glColor3f(v_itr->r, v_itr->g, v_itr->b);
			glVertex3dv(&point(v_itr->vh)[0]);
			glVertex3dv(&point( (v_itr+1)->vh)[0]);
		}

		glEnd();		
		glEnable(GL_LIGHTING);
		glDisable(GL_POLYGON_OFFSET_FILL);
		glPolygonMode(GL_FRONT,GL_FILL);
		glEnable(GL_CULL_FACE);
		glLineWidth(1.f);
	}

	void Model::RenderSpecifiedFace()
	{
		glDisable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		glPushAttrib(GL_LIGHTING_BIT);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(0.5, 1.0);
		glDisable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
		glBegin(GL_TRIANGLES);
		FVIter fv_itr;
		SP_FACE_LIST::iterator f_itr;
		for (f_itr = sp_f_list.begin(); f_itr != sp_f_list.end(); ++f_itr)
		{
			glColor3f(f_itr->r, f_itr->g, f_itr->b);
			for (fv_itr=fv_iter(f_itr->fh); fv_itr; ++fv_itr)
			{						
				glNormal3dv(&normal(fv_itr.handle())[0]);
				glVertex3dv(&point(fv_itr.handle())[0]);
			}
		}
		glEnd();		
		glEnable(GL_LIGHTING);
		glDisable(GL_POLYGON_OFFSET_FILL);
		glPolygonMode(GL_FRONT,GL_FILL);
		glEnable(GL_CULL_FACE);
	}

	void Model::clear_sp_p()
	{
		sp_p_list.clear();
	}

	void Model::clear_sp_v()
	{
		sp_v_list.clear();
	}
	
	void Model::clear_sp_e()
	{
		sp_e_list.clear();
	}

	void Model::clear_sp_f()
	{
		sp_f_list.clear();
	}

	void Model::add_sp_p(const Point &_p, float _r, float _g, float _b)
	{
		sp_p input_data;
		input_data.pt = _p;
		input_data.r = _r;
		input_data.g = _g;
		input_data.b = _b;
		sp_p_list.push_back(input_data);
	}

	void Model::add_sp_v(const VHandle &_v, float _r, float _g, float _b)
	{
		sp_v input_data;
		input_data.vh = _v;
		input_data.r = _r;
		input_data.g = _g;
		input_data.b = _b;
		sp_v_list.push_back(input_data);
	}

	void Model::add_sp_e(const VHandle &_p1, const VHandle &_p2, float _r, float _g, float _b)
	{
		sp_v input_data;
		input_data.vh = _p1;
		input_data.r = _r;
		input_data.g = _g;
		input_data.b = _b;
		sp_e_list.push_back(input_data);

		input_data.vh = _p2;
		sp_e_list.push_back(input_data);
	}

	void Model::add_sp_f(const FHandle &_f, float _r, float _g, float _b)
	{
		sp_f input_data;
		input_data.fh = _f;
		input_data.r = _r;
		input_data.g = _g;
		input_data.b = _b;
		sp_f_list.push_back(input_data);
	}

	double Model::findVertex(const Point &p, VHandle &handle)
	{
		float mDist = INIT_DIST;
		float dist;
		OMT::VIter mV;
		for (OMT::VIter v_it = vertices_begin() ; v_it != vertices_end() ; ++v_it)
		{	
			//計算找到的點與vertex之間的距離
			dist =	OMT::distance(point( v_it.handle()), p);
			if( dist < mDist )
			{
				//距離比較近的記錄下來,接著一一比較
				handle = v_it.handle();
				mDist = dist;
			}
		}
		printf("#%d ", handle.idx());
		return mDist;
	}

	double Model::findHalfEdge(const Point &p, HEHandle &handle)
	{
		float dist, mDist = INIT_DIST;
		OMT::FHandle fH;
		if( findFace(p, fH) < INIT_DIST)
		{
			//找到面了，接下來在面的三個邊中找最近的線, 從one ring edge開始找
			OMT::FEIter e_it;
			for(e_it = fe_iter(fH);e_it;++e_it)
			{
				OMT::HEHandle hedge = halfedge_handle(e_it.handle(),1);
				Point from = point(from_vertex_handle(hedge));
				Point to = point(to_vertex_handle(hedge));
				dist = distanceL(p, from, to);
				if(dist < mDist)
				{
					handle = hedge;
					mDist = dist;
				}
			}
		}
		return mDist;
	}

	double Model::findFace(const Point &p, FHandle &handle)
	{
		VHandle vHandle;
		float mDist = INIT_DIST;
		OMT::FIter mf;
		OMT::Point co, fv[3];
		int i = 0;
		for (OMT::FIter f_it = faces_begin() ; f_it != faces_end() ; ++f_it)
		{
			co[0] = co[1] = co[2] = 0.0;
			i = 0;
			for(OMT::FVIter fv_it = fv_iter(f_it.handle()); fv_it ; ++fv_it, ++i) 
			{
				fv[i] = point(fv_it.handle());
				co += fv[i];
			}
			co /= 3.0f;//重心位置
			float	dist =	OMT::distance(co, p);		
			if(dist < mDist)
			{
				//檢查是否在三角面內
				if(OMT::pointInTrangle(p, fv[0], fv[1], fv[2]))
				{
					handle = f_it.handle();
					mDist = dist;
				}
			}
		}
		////找出最接近的點
		//if(findVertex(p, vHandle) < INIT_DIST)
		//{
		//	VFIter vf_ite;
		//	//尋找點的one ring
		//	for(vf_ite = vf_iter(vHandle);vf_ite;++vf_ite)
		//	{
		//		co[0] = co[1] = co[2] = 0.0;
		//		i = 0;
		//		for(OMT::FVIter fv_it = fv_iter(vf_ite.handle()); fv_it ; ++fv_it, ++i) 
		//		{
		//			fv[i] = point(fv_it.handle());
		//			co += fv[i];
		//		}
		//		//檢查是否在三角面內
		//		co /= 3.0f;//重心位置
		//		float	dist =	OMT::distance(co, p);
		//		if(dist < mDist)
		//		{
		//			if(OMT::pointInTrangle(p, fv[0], fv[1], fv[2]))
		//			{
		//				handle = vf_ite.handle();
		//				mDist = dist;
		//			}
		//		}
		//	}
		//}

		return mDist;
	}

	void Model::deleteVertex(VHandle &handle)
	{
		delete_vertex(handle, false);
		garbage_collection();
	}

	void Model::deleteEdge(HEHandle &handle)
	{
		delete_vertex(from_vertex_handle(handle), false);
		delete_vertex(to_vertex_handle(handle), false);
		garbage_collection();
	}

	void Model::deleteFace(FHandle &handle)
	{
		delete_face(handle, false);
		garbage_collection();
	}

	void Model::simplificationEdge(HEHandle &handle)
	{ 
		//vector<VHandle> face_vhandles;
		vector<VHandle> record_from_vhandles;
		vector<VHandle> record_to_vhandles;
		int size = 0;
		int index = 0;
		int offset_from = 0;
		int offset_to = 0;
		OMT::VFIter vf_it;
		OMT::VVIter vv_it;
		set<FHandle> shandles;
		set<FHandle>::const_iterator ite;


		VHandle opp = opposite_vh(handle);
		VHandle fv = from_vertex_handle(handle);
		VHandle tv = to_vertex_handle(handle);

		printf("From#%d, To#%d\n", fv.idx(), tv.idx());
		//Record the vertex order of erase face
		for(vv_it = vv_iter(fv);vv_it;++vv_it, index++)
		{
			if(vv_it.handle() == tv)
			{
				offset_from = index + 1;
				printf("Offset#%d ", offset_from);
			}
			record_from_vhandles.push_back(vv_it.handle());
			printf("#%d ", vv_it.handle().idx());
		}
		printf("\n");

		index = 0;
		for(vv_it = vv_iter(tv);vv_it;++vv_it, index++)
		{
			if(vv_it.handle() == fv)
			{
				offset_to = index + 1;
				printf("Offset#%d ", offset_to);
			}
			record_to_vhandles.push_back(vv_it.handle());
			printf("#%d ", vv_it.handle().idx());
		}
		printf("\n");

		//Record the face handle
		for(vf_it = vf_iter(fv);vf_it;++vf_it)
		{
			shandles.insert(vf_it.handle());
		}

		for(vf_it = vf_iter(tv);vf_it;++vf_it)
		{
			shandles.insert(vf_it.handle());
		}

		Point fp = point(fv);
		Point tp = point(tv);

		//delete_vertex(fv);
		//delete_vertex(tv);
		//garbage_collection();
		for(ite = shandles.begin();ite != shandles.end();++ite)
		{
			delete_face(*ite, false);
		}
		garbage_collection();


		Point add((fp[0] + tp[0])/2, (fp[1] + tp[1])/2, (fp[2] + tp[2])/2);
		VHandle addHandle = add_vertex(add);
		//for(ite = vhandles.begin(); ite != vhandles.end(); ++ite)
		//{
		//	face_vhandles.push_back(*ite);
		//	if(face_vhandles.size() == 2)
		//	{
		//		face_vhandles.push_back(addHandle);
		//		add_face(face_vhandles);
		//		face_vhandles.clear();
		//		face_vhandles.push_back(*ite);
		//	}
		//}
		size = record_from_vhandles.size();
		printf("From Start#%d, size:%d\n", record_from_vhandles[(offset_from)%size], size);
		for(int i = 0; i < size - 2; i++)
		{
			//face_vhandles.push_back(record_from_vhandles[(i+offset_from)%size]);
			//printf("#%d ", record_from_vhandles[(i+offset_from)%size].idx());
			//if(face_vhandles.size() == 2)
			//{
			//	face_vhandles.push_back(addHandle);
			//	add_face(face_vhandles);
			//	face_vhandles.clear();
			//	face_vhandles.push_back(record_from_vhandles[(i+offset_from)%size]);
			//}
			//face_vhandles.push_back(addHandle);
			printf("#%d ", addHandle.idx());
			//face_vhandles.push_back(record_from_vhandles[(i+1+offset_from)%size]);
			printf("#%d ", record_from_vhandles[(i+1+offset_from)%size].idx());
			//face_vhandles.push_back(record_from_vhandles[(i+offset_from)%size]);
			printf("#%d \n", record_from_vhandles[(i+offset_from)%size].idx());
			//add_face(face_vhandles);
			add_face(addHandle, record_from_vhandles[(i+1+offset_from)%size], record_from_vhandles[(i+offset_from)%size]);
			//face_vhandles.clear();
		}
		
		
		size = record_to_vhandles.size();
		printf("To Start#%d, size:%d\n", record_to_vhandles[(offset_to)%size], size);
		for(int i = 0; i < size - 2; i++)
		{
			//face_vhandles.push_back(record_to_vhandles[(i+offset_to)%size]);
			//printf("#%d ", record_to_vhandles[(i+offset_to)%size].idx());
			//if(face_vhandles.size() == 2)
			//{
			//	face_vhandles.push_back(addHandle);
			//	add_face(face_vhandles);
			//	face_vhandles.clear();
			//	face_vhandles.push_back(record_to_vhandles[(i+offset_to)%size]);
			//}
			//face_vhandles.push_back(addHandle);
			printf("#%d ", addHandle.idx());
			//face_vhandles.push_back(record_to_vhandles[(i+1+offset_to)%size]);
			printf("#%d ", record_to_vhandles[(i+1+offset_to)%size].idx());
			//face_vhandles.push_back(record_to_vhandles[(i+offset_to)%size]);
			printf("#%d \n", record_to_vhandles[(i+offset_to)%size].idx());
			//add_face(face_vhandles);
			add_face(addHandle, record_to_vhandles[(i+1+offset_to)%size], record_to_vhandles[(i+offset_to)%size]);
			//face_vhandles.clear();
		}
		printf("\n");
	}
}
/*======================================================================*/
namespace OMP
{
	Model::Model()
	{
		Mesh.request_vertex_status();
		Mesh.request_edge_status();
		Mesh.request_face_status();
	}
	Model::~Model()
	{
		Mesh.release_vertex_status();
		Mesh.release_edge_status();
		Mesh.release_face_status();
	}
	/*======================================================================*/
	bool Model::ReadFile(std::string _fileName)
	{
		bool isRead = false;
		OpenMesh::IO::Options opt;
		if ( OpenMesh::IO::read_mesh(Mesh, _fileName, opt) )
		{
			//read mesh from filename OK!
			isRead = true;
		}
		if(isRead)
		{
			// If the file did not provide vertex normals and mesh has vertex normal ,then calculate them
			if (!opt.check( OpenMesh::IO::Options::VertexNormal ) && Mesh.has_vertex_normals())
			{
				Mesh.update_normals();
			}
		}
		return isRead;
	}
	bool Model::SaveFile(std::string _fileName)
	{
		bool isSave = false;
		OpenMesh::IO::Options opt;
		if ( OpenMesh::IO::write_mesh(Mesh, _fileName, opt) )
		{
			//read mesh from filename OK!
			isSave = true;
		}
		return isSave;
	}
	/*======================================================================*/
	void Model::Render_solid()
	{
		FIter f_it;
		FVIter	fv_it;
		glPushAttrib(GL_LIGHTING_BIT);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glEnable(GL_DEPTH_TEST);
		glPolygonOffset(2.0, 2.0);
		glBegin(GL_POLYGON);
		//glColor4f(1.0, 0.5, 1.0, 0.5);
		for (f_it = Mesh.faces_begin(); f_it != Mesh.faces_end(); ++f_it) 
		{
			for (fv_it = Mesh.fv_iter( f_it ); fv_it; ++fv_it)
			{						
				glNormal3dv(&Mesh.normal(fv_it.handle())[0]);
				glVertex3dv(&Mesh.point(fv_it.handle())[0]);
		
			}
		}
		glEnd();		
		glDisable(GL_POLYGON_OFFSET_FILL);
	}
	void Model::Render_wireframe()
	{
		MyMesh::HalfedgeHandle _hedge;
		EIter e_it=Mesh.edges_begin();

		glDisable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
		glColor3f(0.0, 0.0, 0.0);
		glLineWidth(1);
		glBegin(GL_LINES);
		for(e_it=Mesh.edges_begin(); e_it != Mesh.edges_end(); ++e_it)
		{
			_hedge = Mesh.halfedge_handle(e_it.handle(),1);

			glVertex3dv(&Mesh.point(Mesh.from_vertex_handle(_hedge))[0]);
			glVertex3dv(&Mesh.point(Mesh.to_vertex_handle(_hedge))[0]);			
		}
		glEnd();
		glEnable(GL_LIGHTING);
	}
	/*======================================================================*/
	void Model::RenderSpecifiedPoint()
	{
		glPushAttrib(GL_LIGHTING_BIT);
		glDisable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
		glPointSize(5.0f);
		glBegin(GL_POINTS);
		vector<sp_p>::iterator p_itr = sp_p_list.begin();
		for (p_itr; p_itr != sp_p_list.end(); ++p_itr)
		{
			glColor3f(p_itr->r, p_itr->g, p_itr->b);
			glVertex3dv(&p_itr->pt[0]);
		}
		glEnd();
		glEnable(GL_LIGHTING);
		glDisable(GL_POLYGON_OFFSET_FILL);
	}
	void Model::RenderSpecifiedVertex()
	{
		glPushAttrib(GL_LIGHTING_BIT);
		glDisable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
		glPointSize(5.0f);
		glBegin(GL_POINTS);
		vector< sp_v >::iterator v_itr = sp_v_list.begin();
		for (v_itr; v_itr != sp_v_list.end(); ++v_itr)
		{
			glColor3f(v_itr->r, v_itr->g, v_itr->b);
			glVertex3dv(&Mesh.point(v_itr->vh)[0]);
		}
		glEnd();
		glEnable(GL_LIGHTING);
		glDisable(GL_POLYGON_OFFSET_FILL);
	}
	void Model::RenderSpecifiedFace()
	{
		glDisable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		glPushAttrib(GL_LIGHTING_BIT);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(0.5, 1.0);
		glDisable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
		glBegin(GL_QUADS);
		FVIter fv_itr;
		vector< sp_f >::iterator f_itr;
		for (f_itr = sp_f_list.begin(); f_itr != sp_f_list.end(); ++f_itr)
		{
			glColor3f(f_itr->r, f_itr->g, f_itr->b);
			for (fv_itr=Mesh.fv_iter(f_itr->fh); fv_itr; ++fv_itr)
			{						
				glNormal3dv(&Mesh.normal(fv_itr.handle())[0]);
				glVertex3dv(&Mesh.point(fv_itr.handle())[0]);
			}
		}
		glEnd();		
		glEnable(GL_LIGHTING);
		glDisable(GL_POLYGON_OFFSET_FILL);
		glPolygonMode(GL_FRONT,GL_FILL);
		glEnable(GL_CULL_FACE);
	}

	/*======================================================================*/
	void Model::add_sp_p(Point   _p, float _r, float _g, float _b)
	{
		sp_p input_data;
		input_data.pt = _p;
		input_data.r = _r;
		input_data.g = _g;
		input_data.b = _b;
		sp_p_list.push_back(input_data);
	}
	void Model::add_sp_v(VHandle _v, float _r, float _g, float _b)
	{
		sp_v input_data;
		input_data.vh = _v;
		input_data.r = _r;
		input_data.g = _g;
		input_data.b = _b;
		sp_v_list.push_back(input_data);
	}
	void Model::add_sp_f(FHandle _f, float _r, float _g, float _b)
	{
		sp_f input_data;
		input_data.fh = _f;
		input_data.r = _r;
		input_data.g = _g;
		input_data.b = _b;
		sp_f_list.push_back(input_data);
	}
	void Model::clear_sp_p()
	{
		sp_p_list.clear();
	}
	void Model::clear_sp_v()
	{
		sp_v_list.clear();
	}
	void Model::clear_sp_f()
	{
		sp_f_list.clear();
	}

	/*======================================================================*/
	VHandle Model::addVertex(Point _p)
	{
		int find_result = findVertex(_p);
		if (find_result != -1)
		{
			return Mesh.vertex_handle(find_result);
		}
		else
		{
			return Mesh.add_vertex(_p);
		}
	}
	FHandle Model::addFace(VHandle _v0, VHandle _v1, VHandle _v2, VHandle _v3)
	{
		vector<VHandle> face_vhandles;

		face_vhandles.clear();
		face_vhandles.push_back(_v0);
		face_vhandles.push_back(_v1);
		face_vhandles.push_back(_v2);
		face_vhandles.push_back(_v3);
		return Mesh.add_face(face_vhandles);
	}
	void Model::deleteFace(FHandle _f)
	{
		Mesh.delete_face(_f);
		Mesh.garbage_collection();
	}
	void Model::deleteFace(VHandle _v0, VHandle _v1, VHandle _v2, VHandle _v3)
	{
		/* 
		v1				v0
		*<--------------*
		|				|
		|				|
		|				|
		|		f		|
		|				|
		|				|
		|				|
		* --------------*
		v2				v3
		*/

		HEHandle v0v1 = Mesh.find_halfedge(_v0, _v1);
		if (v0v1.is_valid())
		{
			FHandle fh = Mesh.face_handle(v0v1);
			if (fh.is_valid())
			{
				Mesh.delete_face(fh);
				Mesh.garbage_collection();
			}
		}
	}
	/*======================================================================*/
	bool Model::IsVertexVertex( VHandle _vj, VHandle _vi)
	{
		for( VVIter vvit = Mesh.vv_iter(_vi) ; vvit ; ++vvit )
			if( vvit.handle() == _vj )
				return true;
		return false;
	}
	/*======================================================================*/
	int Model::quad_subdivision1(int _face_id)
	{
		/*----------------------------------------------------------------------*/
		//這段是為了解決index問題
		VHandle vq, vw, vt, vr;
		vq	 = addVertex(Point(0, 0, 100));
		vw	 = addVertex(Point(1, 0, 100));
		vt	 = addVertex(Point(1, 1, 100));
		vr	 = addVertex(Point(0, 1, 100));
		addFace(vq, vw, vt, vr);
		/*----------------------------------------------------------------------*/
		/*收集需要subdivision的face*/
		//初始table
		bool *table = new bool[Mesh.n_faces()];
		for (unsigned i = 0; i < Mesh.n_faces(); i++)
		{
			table[i] = false;
		}

		vector< FHandle > candidate_faces, last_found_face;
		last_found_face.push_back(Mesh.face_handle(_face_id));
		table[_face_id] = true;

		while(last_found_face.size() != 0)
		{
			vector< FHandle > new_found_faces;
			for (vector< FHandle >::iterator crnt_f = last_found_face.begin(); crnt_f != last_found_face.end(); ++crnt_f)
			{
				for (FFIter ff_itr = Mesh.ff_iter(*crnt_f); ff_itr; ++ff_itr)
				{
					if (table[ff_itr.handle().idx()] != true)
					{
						new_found_faces.push_back(ff_itr.handle());
						table[ff_itr.handle().idx()] = true;
					}
				}
			}
			for (vector< FHandle >::iterator f_itr = last_found_face.begin(); f_itr != last_found_face.end(); ++f_itr)
			{
				candidate_faces.push_back(*f_itr);
			}
			last_found_face.assign(new_found_faces.begin(), new_found_faces.end());
		}
		delete table;
		/*----------------------------------------------------------------------*/
		/*對candidate faces做subdivision*/
		/*
			v0		vd		v3
			*-------*-------*
			|		|		|	 
			|		|		|
			|	  ve|		|
		  va*-------*-------*vc
			|		|		|
			|		|		|
			|		|		|
			*-------*-------*
			v1		vb		v2		
		*/
		for (vector< FHandle >::iterator face_itr = candidate_faces.begin(); face_itr != candidate_faces.end(); ++face_itr)
		{
			VHandle v[4], va, vb, vc, vd, ve;
			FVIter fv_itr = Mesh.fv_iter(*face_itr);
			for (int i = 0; fv_itr; ++fv_itr)
			{
				v[i++] = fv_itr.handle();
			}

			deleteFace(v[0], v[1], v[2], v[3]);
			va	 = addVertex((Mesh.point(v[0])+Mesh.point(v[1]))/2);
			vb	 = addVertex((Mesh.point(v[1])+Mesh.point(v[2]))/2);
			vc	 = addVertex((Mesh.point(v[2])+Mesh.point(v[3]))/2);
			vd	 = addVertex((Mesh.point(v[3])+Mesh.point(v[0]))/2);
			ve	 = addVertex((Mesh.point(v[0])+Mesh.point(v[1])+Mesh.point(v[2])+Mesh.point(v[3]))/4);
			addFace(vd, v[0], va, ve);
			addFace(va, v[1], vb, ve);
			addFace(vb, v[2], vc, ve);
			addFace(vc, v[3], vd, ve);
		}
		/*----------------------------------------------------------------------*/
		deleteFace(vq, vw, vt, vr);//這行只是為了解決index問題
		/*----------------------------------------------------------------------*/
		return 0;
	}
	int Model::quad_subdivision2(int _face_id)
	{
		/*----------------------------------------------------------------------*/
		//這段是為了解決index問題
		VHandle vq, vw, vt, vr;
		vq	 = addVertex(Point(0, 0, 100));
		vw	 = addVertex(Point(1, 0, 100));
		vt	 = addVertex(Point(1, 1, 100));
		vr	 = addVertex(Point(0, 1, 100));
		addFace(vq, vw, vt, vr);
		/*----------------------------------------------------------------------*/
		/*收集需要subdivision的face*/
		//初始table
		bool *table = new bool[Mesh.n_faces()];
		for (unsigned i = 0; i < Mesh.n_faces(); i++)
		{
			table[i] = false;
		}

		vector< FHandle > candidate_faces, last_found_face;
		last_found_face.push_back(Mesh.face_handle(_face_id));
		table[_face_id] = true;

		while(last_found_face.size() != 0)
		{
			vector< FHandle > new_found_faces;
			for (vector< FHandle >::iterator crnt_f = last_found_face.begin(); crnt_f != last_found_face.end(); ++crnt_f)
			{
				for (FFIter ff_itr = Mesh.ff_iter(*crnt_f); ff_itr; ++ff_itr)
				{
					if (table[ff_itr.handle().idx()] != true)
					{
						new_found_faces.push_back(ff_itr.handle());
						table[ff_itr.handle().idx()] = true;
					}
				}
			}
			for (vector< FHandle >::iterator f_itr = last_found_face.begin(); f_itr != last_found_face.end(); ++f_itr)
			{
				candidate_faces.push_back(*f_itr);
			}
			last_found_face.assign(new_found_faces.begin(), new_found_faces.end());
		}
		delete table;
		/*----------------------------------------------------------------------*/
		/*對candidate faces做subdivision*/
		/*
			v0		vh		vg		v3
			*-------*-------*-------*
			|		|		|		|
			|		|		|		|
			|	  vi|	  vl|		|
		 va *-------*-------*-------*vf
			|		|		|		|
			|		|		|		|
			|	  vj|	  vk|		|
		 vb *-------*-------*-------*ve
			|		|		|		|
			|		|		|		|
			|		|		|		|
			*-------*-------*-------*
			v1		vc		vd		v2		
		*/
		for (vector< FHandle >::iterator face_itr = candidate_faces.begin(); face_itr != candidate_faces.end(); ++face_itr)
		{
			VHandle v[4], va, vb, vc, vd, ve, vf, vg, vh, vi, vj, vk, vl;
			FVIter fv_itr = Mesh.fv_iter(*face_itr);
			for (int i = 0; fv_itr; ++fv_itr)
			{
				v[i++] = fv_itr.handle();
			}

			deleteFace(v[0], v[1], v[2], v[3]);
			va	 = addVertex((Mesh.point(v[0])*2+Mesh.point(v[1])  )/3);
			vb	 = addVertex((Mesh.point(v[0])  +Mesh.point(v[1])*2)/3);
			vc	 = addVertex((Mesh.point(v[1])*2+Mesh.point(v[2])  )/3);
			vd	 = addVertex((Mesh.point(v[1])  +Mesh.point(v[2])*2)/3);
			ve	 = addVertex((Mesh.point(v[2])*2+Mesh.point(v[3])  )/3);
			vf	 = addVertex((Mesh.point(v[2])  +Mesh.point(v[3])*2)/3);
			vg	 = addVertex((Mesh.point(v[3])*2+Mesh.point(v[0])  )/3);
			vh	 = addVertex((Mesh.point(v[3])  +Mesh.point(v[0])*2)/3);

			vi	 = addVertex((Mesh.point(vh)*2+Mesh.point(vc)  )/3);
			vj	 = addVertex((Mesh.point(vh)  +Mesh.point(vc)*2)/3);
			vk	 = addVertex((Mesh.point(vd)*2+Mesh.point(vg)  )/3);
			vl	 = addVertex((Mesh.point(vd)  +Mesh.point(vg)*2)/3);
			addFace(v[0], va, vi, vh);
			addFace(va, vb, vj, vi);
			addFace(vb, v[1], vc, vj);
			addFace(vc, vd, vk, vj);
			addFace(vd, v[2], ve, vk);
			addFace(ve, vf, vl, vk);
			addFace(vf, v[3], vg, vl);
			addFace(vg, vh, vi, vl);
			addFace(vi, vj, vk, vl);
		}
		/*----------------------------------------------------------------------*/
		deleteFace(vq, vw, vt, vr);//這行只是為了解決index問題
		/*----------------------------------------------------------------------*/
		return 0;
	}
	/*======================================================================*/
	int Model::findVertex(Point _p)
	{
		for( VIter v_itr = Mesh.vertices_begin(); v_itr != Mesh.vertices_end(); ++v_itr)
			if( Mesh.point(v_itr) == _p )
				return v_itr.handle().idx();
		return -1;
	}
	/*======================================================================*/
};
/*======================================================================*/
void Tri_Mesh::Render_Solid()
{
		FIter f_it;
		FVIter	fv_it;
		//glPushAttrib(GL_LIGHTING_BIT);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
		glPolygonOffset(2.0, 2.0);
		glBegin(GL_TRIANGLES);
		glColor4f(0.81, 0.74, 0.33, 0.5);
		for (f_it = faces_begin(); f_it != faces_end(); ++f_it) 
		{
			for (fv_it = fv_iter( f_it ); fv_it; ++fv_it)
			{						
				glNormal3dv(&normal(fv_it.handle())[0]);
				glVertex3dv(&point(fv_it.handle())[0]);
			}
		}
		glEnd();	
		
		glDisable(GL_POLYGON_OFFSET_FILL);
}

void Tri_Mesh::Render_SolidWireframe()
{
	FIter f_it;
	FVIter	fv_it;

	/*glPointSize(8.0);
	glBegin(GL_POINTS);
	glColor4f(1.0, 0.0, 0.0, 1.0);
	glVertex3dv(point(vertex_handle(0)));
	glEnd();*/
    glDisable(GL_LIGHTING);
	glPushAttrib(GL_LIGHTING_BIT);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glEnable(GL_DEPTH_TEST);
	glPolygonOffset(2.0, 2.0);
	glBegin(GL_TRIANGLES);
	glColor4f(1.0, 0.96, 0.49, 1.0);
	for (f_it = faces_begin(); f_it != faces_end(); ++f_it) 
	{
		for (fv_it = fv_iter( f_it ); fv_it; ++fv_it)
		{						
			//glNormal3dv(normal(fv_it.handle()));
			glVertex3dv(&point(fv_it.handle())[0]);
		}
	}
	glEnd();

	
	//glDisable(GL_POLYGON_OFFSET_FILL);


	glPushAttrib(GL_LIGHTING_BIT);	
	glDisable(GL_LIGHTING);
	glLineWidth(1.0);
	glColor3f(0.0, 0.0, 0.0);	
	glBegin(GL_LINES);
	for(OMT::EIter e_it = edges_begin(); e_it != edges_end(); ++e_it)
	{
		OMT::HEHandle _hedge = halfedge_handle(e_it.handle(),1);

		OMT::Point curVertex  = point(from_vertex_handle(_hedge));
		glVertex3dv(&curVertex[0]);
		
		curVertex = point(to_vertex_handle(_hedge));
		glVertex3dv(&curVertex[0]);			
	}
	glEnd();
	glPopAttrib();
}

void Tri_Mesh::Render_Wireframe()
{
	//glPushAttrib(GL_LIGHTING_BIT);	
	glDisable(GL_LIGHTING);
	glLineWidth(1.0);
	
	glColor3f(0.0, 0.0, 0.0);	

	glBegin(GL_LINES);
	for(OMT::EIter e_it = edges_begin(); e_it != edges_end(); ++e_it)
	{
		OMT::HEHandle _hedge = halfedge_handle(e_it.handle(),1);

		OMT::Point curVertex  = point(from_vertex_handle(_hedge));
		glVertex3dv(&curVertex[0]);

		curVertex = point(to_vertex_handle(_hedge));
		glVertex3dv(&curVertex[0]);			
	}
	glEnd();
	
}

void Tri_Mesh::Render_Point()
{
	glPointSize ( 8.0 ) ;				  
	glColor3f( 1.0, 0.0, 0.0 ) ;
	glBegin(GL_POINTS);
	for (OMT::VIter v_it = vertices_begin() ; v_it != vertices_end() ; ++v_it)
	{
		  glVertex3dv(&point(v_it)[0]);
	}
	glEnd();
}

bool ReadFile(std::string _fileName,Tri_Mesh *_mesh)
{
	bool isRead = false;
	OpenMesh::IO::Options opt;
	if ( OpenMesh::IO::read_mesh(*_mesh, _fileName, opt) )
	{
			//read mesh from filename OK!
			isRead = true;
	}
	if(isRead)
	{
			// If the file did not provide vertex normals and mesh has vertex normal ,then calculate them
		if (!opt.check( OpenMesh::IO::Options::VertexNormal ) && _mesh->has_vertex_normals())
		{
				_mesh->update_normals();
		}
	}
	return isRead;
}

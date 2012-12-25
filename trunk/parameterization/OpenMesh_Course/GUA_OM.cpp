#include "stdafx.h"
#include "GUA_OM.h"
#define PI 3.141592653589793
#define THRESHOLD 1.5

#include <LinearSystemLib.h>
#include <math.h>
#include <set>


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

	double det(OpenMesh::Geometry::Quadricd q, int a11, int a12, int a13,
		int a21, int a22, int a23,
		int a31, int a32, int a33)
	{
		double m[16];
		m[0] = q.a();
		m[4] = m[1] = q.b();
		m[8] = m[2] = q.c();
		m[3] = q.d();
		m[5] = q.e();
		m[9] = m[6] = q.f();
		m[7] = q.g();
		m[10] = q.h();
		m[11] = q.i();
		m[15] = q.j();
		m[12] = m[13] = m[14] = 0;
		double d = m[a11]*m[a22]*m[a33] + m[a13]*m[a21]*m[a32] + m[a12]*m[a23]*m[a31] 
		- m[a13]*m[a22]*m[a31] - m[a11]*m[a23]*m[a32] - m[a12]*m[a21]*m[a33]; 
		return d;
	}

	double vertexError(OpenMesh::Geometry::Quadricd q, double x, double y, double z)
	{
		return q.a()*x*x + 2*q.b()*x*y + 2*q.c()*x*z + 2*q.d()*x + q.e()*y*y
			+ 2*q.f()*y*z + 2*q.g()*y + q.h()*z*z + 2*q.i()*z + q.j();
	}

	/*======================================================================*/
	Model::Model()
	{
		request_vertex_status();
		request_edge_status();
		request_face_status();
		add_property(f_bIsSelect);
		add_property(v_vec2dTexcoord);
		add_property(SelRingID);
		add_property(SelVID);
		add_property(fTex);
		//property(f_bIsSelect, face_handle);
		m_uiCurVertex = -1;
		m_CurEditTex = NULL;
		//for(int i = 0; i < MAX_TEXTURE; i++)
		//{
		//	m_uiTexture[i] = NULL;
		//	//m_pImage[i] = NULL;
		//}
	}
	Model::~Model()
	{
		release_vertex_status();
		release_edge_status();
		release_face_status();

		//for(int i = 0; i < MAX_TEXTURE; i++)
		//{
		//	if(m_uiTexture[i] != NULL)
		//	{
		//		glDeleteTextures(1, &m_uiTexture[i]);
		//		m_uiTexture[i] = NULL;
		//	}

		//	//if(m_pImage[i] != NULL)
		//	//{
		//	//	cvReleaseImage(&m_pImage[i]);
		//	//	m_pImage[i] = NULL;
		//	//}

		//	if(m_matImage[i].data != NULL)
		//		m_matImage[i].release();
		//}
	}

	void Model::initQuadrices()
	{
		VIter v_ite;
		FIter f_ite;
		FVIter fv_ite;
		double a, b, c, d, M;
		Point p[3];
		size_t vid[3];
		int i;
		number_of_face = n_faces();
		quadrics.clear();
		errors.clear();
		for (v_ite = vertices_begin(); v_ite != vertices_end(); ++v_ite)
		{
			quadrics.insert(QUADRICES::value_type(v_ite.handle().idx(), OpenMesh::Geometry::Quadricd(0.0)));
		}

		for (f_ite = faces_begin(); f_ite != faces_end(); ++f_ite)
		{
			for(fv_ite = fv_iter(f_ite.handle()), i = 0; fv_ite; ++fv_ite, ++i)
			{
				vid[i] = fv_ite.handle().idx();
				p[i] = point(fv_ite);
			}
			a = (p[1][1]-p[0][1])*(p[2][2]-p[0][2]) - (p[1][2]-p[0][2])*(p[2][1]-p[0][1]);   /* a1*b2 - a2*b1;        */
			b = (p[1][2]-p[0][2])*(p[2][0]-p[0][0]) - (p[1][0]-p[0][0])*(p[2][2]-p[0][2]);   /* a2*b0 - a0*b2;        */
			c = (p[1][0]-p[0][0])*(p[2][1]-p[0][1]) - (p[1][1]-p[0][1])*(p[2][0]-p[0][0]);   /* a0*b1 - a1*b0;        */
			M = sqrt(a*a + b*b + c*c);
			a = a/M;
			b = b/M;
			c = c/M;
			OpenMesh::Geometry::Quadricd q(a, b, c, -1*(a*p[0][0] + b*p[0][1] + c*p[0][2]));
			for(i = 0; i < 3; i++)
			{
				quadrics[vid[i]] += q;
			}
		}
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
		return mDist;
	}

	/*---------------------------------SIMPLIFY-------------------------------------*/
	bool Model::isConvex(vector<VHandle> &polygon)
	{
		return true;
		float totalAngle = 0;
		size_t size = polygon.size();
		for(int i = 0; i < size; i++)
		{
			Point p1 = point(polygon[i]);
			Point p2 = point(polygon[(i+1)%size]);
			Point p3 = point(polygon[(i+2)%size]);
			Point vec1 = p1 - p2;
			Point vec2 = p2 - p3;
			vec1.normalize();
			vec2.normalize();
			float angle = acos(dot(vec1,vec2));
#ifdef _DEBUG
			printf("%f ", angle);
#endif
			totalAngle +=  angle;
		}
#ifdef _DEBUG
		printf("Total:%f\n", totalAngle);
#endif
		if(totalAngle <= 2 * PI + THRESHOLD && totalAngle >  2 * PI - THRESHOLD)
			return true;
		return false;
	}

	double Model::calculateError(int id_v1, int id_v2, double* vx, double* vy, double* vz)
	{
		double min_error;
		OpenMesh::Geometry::Quadricd q_bar;
		OpenMesh::Geometry::Quadricd q_delta;
		bool isReturnVertex = true;
		if (vx == NULL) { vx = new double; isReturnVertex = false; }
		if (vy == NULL) { vy = new double; }
		if (vz == NULL) { vz = new double; }

		/* computer quadric of virtual vertex vf */
		q_bar = quadrics[id_v1];
		q_bar += quadrics[id_v2];

		q_delta.set(q_bar.a(),q_bar.b(),q_bar.c(),q_bar.d(),q_bar.e(),q_bar.f(),q_bar.g(),q_bar.h(),q_bar.i(),1);

		/* if q_delta is invertible */
		if ( double d = det(q_delta, 0, 1, 2, 4, 5, 6, 8, 9, 10) )		/* note that det(q_delta) equals to M44 */
		{
			*vx = -1/d*det(q_delta, 1, 2, 3, 5, 6, 7, 9, 10, 11);	/* vx = A41/det(q_delta) */
			*vy =  1/d*det(q_delta, 0, 2, 3, 4, 6, 7, 8, 10, 11);	/* vy = A42/det(q_delta) */
			*vz = -1/d*det(q_delta, 0, 1, 3, 4, 5, 7, 8, 9, 11);		/* vz = A43/det(q_delta) */	
		}

		/*
		* if q_delta is NOT invertible, select 
		* vertex from v1, v2, and (v1+v2)/2 
		*/
		else
		{
			Point p1 = point(vertex_handle(id_v1));
			Point p2 = point(vertex_handle(id_v2));
			Point p3 = (p1+p1)/2;

			double error1 = vertexError(q_bar, p1[0], p1[1], p1[2]);
			double error2 = vertexError(q_bar, p2[0], p2[1], p2[2]);
			double error3 = vertexError(q_bar, p3[0], p3[1], p3[2]);

			if(error2 > error3)
				min_error = error3;
			else
				min_error = error2;

			if(min_error > error1)
				min_error = error1;

			if (error1 == min_error) { *vx = p1[0]; *vy = p1[1], *vz = p1[2]; }
			if (error2 == min_error) { *vx = p2[0]; *vy = p2[1], *vz = p2[2]; }
			if (error3 == min_error) { *vx = p3[0]; *vy = p3[1], *vz = p3[2]; }
		}

		min_error = vertexError(q_bar, *vx, *vy, *vz);

		if (isReturnVertex == false) { delete vx; delete vy; delete vz; }
		return min_error;
	}

	void Model::selectPair()
	{
		const double t = 0.12;
		FIter f_ite;
		FVIter fv_ite;
		int max_vid;
		int min_vid;
		int i, j;
		VHandle v1;
		VHandle v2;

		/* (v1, v2) is an edge */
		/* id_v1 < id_v2*/
		for(f_ite = faces_begin(); f_ite != faces_end();++f_ite)
		{
			fv_ite = fv_iter(f_ite.handle());
			v1 = fv_ite.handle();
			if(fv_ite)
				++fv_ite;
			else
				continue;

			for(; fv_ite; ++fv_ite)
			{
				v2 = fv_ite.handle();
				if(v1.idx() > v2.idx())
				{
					max_vid = v1.idx();
					min_vid = v2.idx();
				}
				else
				{
					min_vid = v1.idx();
					max_vid = v2.idx();
				}

				if ( errors.find(PAIR(min_vid, max_vid)) == errors.end() )
				{
					/* (faces[i].id_vertex[0], faces[i].id_vertex[1]) is an edge */
					errors.insert(ERRORS::value_type(PAIR(min_vid, max_vid), calculateError(min_vid, max_vid)));
				}
				v1 = v2;
			}
		}
	}

	DELETE_HISTORY Model::undoSimplification(void)
	{
		DELETE_HISTORY history;
		if(vDeleteHistory.size() > 0)
		{
			history = vDeleteHistory.back();
			VFIter vf_ite;
			size_t size;
			delete_vertex(history.newVertex);
			garbage_collection();
			//for(vf_ite = vf_iter(history.newVertex); vf_ite;++vf_ite)
			//{
			//	delete_face(vf_ite.handle(), false);
			//}

			size = history.fromOnering.size();
			for(int i = 0; i < size; i++)
			{
#ifdef _DEBUG
				printf("#%d ", history.fromVertex.idx());
				printf("#%d ", history.fromOnering[(i+1)%size].idx());
				printf("#%d \n", history.fromOnering[(i)%size].idx());
#endif
				add_face(history.fromVertex, history.fromOnering[(i+1)%size], history.fromOnering[(i)%size]);
			}
			size = history.toOnering.size();
			for(int i = 0; i < size; i++)
			{
#ifdef _DEBUG
				printf("#%d ", history.toVertex.idx());
				printf("#%d ", history.toOnering[(i+1)%size].idx());
				printf("#%d \n", history.toOnering[(i)%size].idx());
#endif
				add_face(history.toVertex, history.toOnering[(i+1)%size], history.toOnering[(i)%size]);
			}
			vDeleteHistory.pop_back();
		}
		return history;
	}

	void Model::undoSimplification(int target_num_faces)
	{
		int delete_id;
		PAIR p;
		std::pair<ERRORS::iterator, bool> pr;
		ERRORS::iterator e_ite;
		OMT::VVIter vv_it;
		int max_id, min_id;
		int id_v1, id_v2;
		while(n_faces() < target_num_faces)
		{
			if(vDeleteHistory.empty())
				return;
			DELETE_HISTORY history = undoSimplification();
			delete_id = history.newVertex.idx();
			id_v1 = history.fromVertex.idx();
			id_v2 = history.toVertex.idx();
			//delete error have delete_id
			for (e_ite = errors.begin(); e_ite != errors.end();)
			{
				p = e_ite->first;
				if (p.first == delete_id || p.second == delete_id)
				{
					errors.erase(e_ite++);
				}
				else
					++e_ite;
			}
			quadrics.erase(quadrics.find(delete_id));

			//recover the vertex error
			for(vv_it = vv_iter(history.fromVertex);vv_it;++vv_it)
			{
				if(vv_it.handle().idx() == id_v2)
					continue;
				if(history.fromVertex.idx() > vv_it.handle().idx())
				{
					max_id = history.fromVertex.idx();
					min_id = vv_it.handle().idx();
				}
				else
				{
					min_id = history.fromVertex.idx();
					max_id = vv_it.handle().idx();
				}
				pr = errors.insert(ERRORS::value_type( PAIR(min_id, max_id), 0.0 ));
#ifdef _DEBUG
				if (pr.second == true)
					printf("QUADRIC: insert  pair(%d, %d)\n", min_id, max_id);
				else
					printf("QUADRIC: insert  pair(%d, %d) FAIL..duplicate\n", min_id, max_id);
#endif
			}

			for(vv_it = vv_iter(history.toVertex);vv_it;++vv_it)
			{
				if(history.toVertex.idx() > vv_it.handle().idx())
				{
					max_id = history.toVertex.idx();
					min_id = vv_it.handle().idx();
				}
				else
				{
					min_id = history.toVertex.idx();
					max_id = vv_it.handle().idx();
				}
				pr = errors.insert(ERRORS::value_type( PAIR(min_id, max_id), 0.0 ));
#ifdef _DEBUG
				if (pr.second == true)
					printf("QUADRIC: insert  pair(%d, %d)\n", min_id, max_id);
				else
					printf("QUADRIC: insert  pair(%d, %d) FAIL..duplicate\n", min_id, max_id);
#endif
			}
			updateErrors(id_v1);
			updateErrors(id_v2);
		}
	}

	void Model::simplification(double rate)
	{
		int target_num_faces = rate * number_of_face;
		if(n_faces() > target_num_faces)
		{
			simplification(target_num_faces);
		}
		else
		{
			undoSimplification(target_num_faces);
		}
	}

	void Model::simplification(int target_num_faces)
	{
		/* calculate initial error for each valid pair*/
		selectPair();
		int max_id, min_id;
		int id_v1, id_v2, new_id;
		double vx, vy, vz;

		/* contract vertices and generate vsplits */
		while (n_faces() > target_num_faces)
		{
			/* find least-error pair */
			double min_error = INT_MAX;
			ERRORS::iterator iter_min_error;
			for (ERRORS::iterator iter = errors.begin(); iter != errors.end(); iter++)
			{
				if (iter -> second < min_error)
				{
					min_error = iter -> second;
					iter_min_error = iter;
				}
				const PAIR &p = iter -> first;
			}
			PAIR pair_min_error = iter_min_error -> first;
			id_v1 = pair_min_error.first;
			id_v2 = pair_min_error.second;
			if(id_v1 == id_v2)
				int i = 0;

			/* add to vsplits; */
			calculateError(id_v1, id_v2, &vx, &vy,&vz);	/* get coordinate of vf */
			//v.v1.x = vertices[id_v1].x;
			//v.v1.y = vertices[id_v1].y;
			//v.v1.z = vertices[id_v1].z;
			//v.v2.x = vertices[id_v2].x;
			//v.v2.y = vertices[id_v2].y;
			//v.v2.z = vertices[id_v2].z;
			//v.vf.x = vx;
			//v.vf.y = vy;
			//v.vf.z = vz;
			//vsplits.push_back(v);

			///* update coordinate of v1 */
			//vertices[id_v1].x = vx;
			//vertices[id_v1].y = vy;
			//vertices[id_v1].z = vz;

			/* update quadric of v1 */
			//quadrics[id_v1] += quadrics[id_v2];
			new_id = simplification(id_v1, id_v2, vx, vy, vz);
			if(new_id == -1)
				return;
			quadrics.insert(QUADRICES::value_type(new_id, OpenMesh::Geometry::Quadricd(0.0)));
			quadrics[new_id] = quadrics[id_v1];
			quadrics[new_id] += quadrics[id_v2];
			///////////////////////////////////////////////////////////////////////////////////////////////
			//                                                                                           //
			//                                        WARNING:                                           //
			// Removing faces and edges contaning v2 can be more efficient. Major performance issue here //
			///////////////////////////////////////////////////////////////////////////////////////////////


			///* replace v2 with v1 in faces */
			///* remove faces that has an edge of (v1, v2) */
			//for (Faces::iterator iter = faces.begin(); iter != faces.end(); )
			//{
			//	for (int j = 0; j < 3; j++)
			//	{
			//		if (iter->id_vertex[j] == id_v2)
			//		{
			//			if (iter->id_vertex[0] == id_v1 || iter->id_vertex[1] == id_v1 || iter->id_vertex[2] == id_v1)
			//			{
			//				iter = faces.erase(iter);
			//			}
			//			else
			//			{
			//				iter->id_vertex[j] = id_v1;
			//				iter++;
			//			}
			//			break;
			//		}
			//		else if(j == 2)
			//			iter++;
			//	}
			//}

			///* remove v2 in vertices */
			//vertices.erase(id_v2);

			/* merge pairs of v2 to v1 */
			PAIR p;
			std::pair<ERRORS::iterator, bool> pr;
			for (ERRORS::iterator iter = errors.begin(); iter != errors.end(); )
			{
				//printf("%d\n", errors.size());
				//Errors::iterator iter_back = iter;
				p = iter -> first;
				if (p.first == id_v2 && p.second != id_v1)
				{
					//erase
					errors.erase(iter++);

					if(new_id > p.second)
					{
						max_id = new_id;
						min_id = p.second;
					}
					else
					{
						min_id = new_id;
						max_id = p.second;
					}
					
					//insert
					//duplicate is not possible in map
					pr = errors.insert(ERRORS::value_type( PAIR(min_id, max_id), 0.0 ));
#ifdef _DEBUG
					if (pr.second == true)
						printf("QUADRIC: insert  pair(%d, %d)\n", min_id, max_id);
					else
						printf("QUADRIC: insert  pair(%d, %d) FAIL..duplicate\n", min_id, max_id);
#endif
				}
				else if (p.second == id_v2 && p.first != id_v1)
				{
					//erase
					errors.erase(iter++);

					if(new_id > p.first)
					{
						max_id = new_id;
						min_id = p.first;
					}
					else
					{
						min_id = new_id;
						max_id = p.first;
					}

					//insert
					//duplicate is not possible in map
					pr = errors.insert(ERRORS::value_type( PAIR(min_id, max_id), 0.0 ));
#ifdef _DEBUG
					if (pr.second == true)
						printf("QUADRIC: insert  pair(%d, %d)\n", min_id, max_id);
					else
						printf("QUADRIC: insert  pair(%d, %d) FAIL..duplicate\n", min_id, max_id);
#endif
					//iter = iter_back;
				}
				else if (p.first == id_v1 && p.second != id_v2)
				{
						//erase
						errors.erase(iter++);

						if(new_id > p.second)
						{
							max_id = new_id;
							min_id = p.second;
						}
						else
						{
							min_id = new_id;
							max_id = p.second;
						}

						//insert
						//duplicate is not possible in map
						pr = errors.insert(ERRORS::value_type( PAIR(min_id, max_id), 0.0 ));
#ifdef _DEBUG
						if (pr.second == true)
							printf("QUADRIC: insert  pair(%d, %d)\n", min_id, max_id);
						else
							printf("QUADRIC: insert  pair(%d, %d) FAIL..duplicate\n", min_id, max_id);
#endif
					}
					else if (p.second == id_v1 && p.first != id_v2)
					{
						//erase
						errors.erase(iter++);

						if(new_id > p.first)
						{
							max_id = new_id;
							min_id = p.first;
						}
						else
						{
							min_id = new_id;
							max_id = p.first;
						}

						//insert
						//duplicate is not possible in map
						pr = errors.insert(ERRORS::value_type( PAIR(min_id, max_id), 0.0 ));
#ifdef _DEBUG
						if (pr.second == true)
							printf("QUADRIC: insert  pair(%d, %d)\n", min_id, max_id);
						else
							printf("QUADRIC: insert  pair(%d, %d) FAIL..duplicate\n", min_id, max_id);
#endif
						//iter = iter_back;
					}
					else
					iter++;
			}


			///////////////////////////////////////////////////////////////////////////////////////////////
			//                                                                                           //
			//                                        END WARNING                                        //
			///////////////////////////////////////////////////////////////////////////////////////////////

			/* remove pair (v1, v2) */
			errors.erase(iter_min_error);

			/* update error of pairs involving v1 */
			updateErrors(new_id);
		}
	}

	int Model::simplification(int id_v1, int id_v2, double vx, double vy, double vz)
	{
		DELETE_HISTORY history;

		vector<VHandle> polygon;
		int size = 0;
		int index = 0;
		int offset_from = 0;
		int offset_to = 0;
		OMT::VFIter vf_it;
		OMT::VVIter vv_it;
		set<FHandle> shandles;
		set<FHandle>::const_iterator ite;
		int new_id;


		history.fromVertex = vertex_handle(id_v1);
		history.toVertex = vertex_handle(id_v2);
#ifdef _DEBUG
		printf("\nFrom#%d, To#%d\n", history.fromVertex.idx(), history.toVertex.idx());
#endif
		//Record the vertex order of erase face
		for(vv_it = vv_iter(history.fromVertex);vv_it;++vv_it, index++)
		{
			if(vv_it.handle() == history.toVertex)
			{
				offset_from = index + 1;
#ifdef _DEBUG
				printf("Offset#%d ", offset_from);
#endif
			}
			history.fromOnering.push_back(vv_it.handle());
#ifdef _DEBUG
			printf("#%d ", vv_it.handle().idx());
#endif
		}
#ifdef _DEBUG
		printf("\n");
#endif

		index = 0;
		for(vv_it = vv_iter(history.toVertex);vv_it;++vv_it, index++)
		{
			if(vv_it.handle() == history.fromVertex)
			{
				offset_to = index + 1;
#ifdef _DEBUG
				printf("Offset#%d ", offset_to);
#endif
			}
			history.toOnering.push_back(vv_it.handle());
#ifdef _DEBUG
			printf("#%d ", vv_it.handle().idx());
#endif
		}
#ifdef _DEBUG
		printf("\n");
#endif

		size = history.fromOnering.size();
		for(int i = 0; i < size - 2; i++)
		{
			polygon.push_back(history.fromOnering[(i+offset_from)%size]);
#ifdef _DEBUG
			printf("#%d ", history.fromOnering[(i+offset_from)%size].idx());
#endif
		}

		size = history.toOnering.size();
		for(int i = 0; i < size - 2; i++)
		{
			polygon.push_back(history.toOnering[(i+offset_to)%size]);
#ifdef _DEBUG
			printf("#%d ", history.toOnering[(i+offset_to)%size].idx());
#endif
		}
#ifdef _DEBUG
		printf("\n ");
#endif
		if(!isConvex(polygon))
			return -1;

		//Record the face handle
		for(vf_it = vf_iter(history.fromVertex);vf_it;++vf_it)
		{
			shandles.insert(vf_it.handle());
		}

		for(vf_it = vf_iter(history.toVertex);vf_it;++vf_it)
		{
			shandles.insert(vf_it.handle());
		}

		for(ite = shandles.begin();ite != shandles.end();++ite)
		{
			delete_face(*ite, false);
		}
		garbage_collection();


		Point add(vx, vy, vz);
		history.newVertex = add_vertex(add);
		new_id = history.newVertex.idx();
		size = history.fromOnering.size();
		printf("From Start#%d, size:%d\n", history.fromOnering[(offset_from)%size], size);
		for(int i = 0; i < size - 2; i++)
		{
#ifdef _DEBUG
			printf("#%d ", history.newVertex.idx());
			printf("#%d ", history.fromOnering[(i+1+offset_from)%size].idx());
			printf("#%d \n", history.fromOnering[(i+offset_from)%size].idx());
#endif
			add_face(history.newVertex, history.fromOnering[(i+1+offset_from)%size], history.fromOnering[(i+offset_from)%size]);
		}


		size = history.toOnering.size();
		printf("To Start#%d, size:%d\n", history.toOnering[(offset_to)%size], size);
		for(int i = 0; i < size - 2; i++)
		{
#ifdef _DEBUG
			printf("#%d ", history.newVertex.idx());
			printf("#%d ", history.toOnering[(i+1+offset_to)%size].idx());
			printf("#%d \n", history.toOnering[(i+offset_to)%size].idx());
#endif
			add_face(history.newVertex, history.toOnering[(i+1+offset_to)%size], history.toOnering[(i+offset_to)%size]);
		}
		printf("\n");
		vDeleteHistory.push_back(history);
		return new_id;
	}

	void Model::updateErrors(int idx)
	{
		PAIR p;
		ERRORS::iterator ite;
		for (ite = errors.begin(); ite != errors.end(); ite++)
		{
			p = ite -> first;
			if (p.first == idx)
			{
				ite -> second = calculateError(idx, p.second);
			}
			if (p.second == idx)
			{
				ite -> second = calculateError(idx, p.first);
			}
		}
	}

	/*---------------------------------TEXTURE--------------------------------------*/
	//AUX_RGBImageRec *LoadBMP(const char *filename)	// Loads A Bitmap Image
	//{
	//	FILE *file=NULL;			// File Handle

	//	if (!filename)				// Make Sure A Filename Was Given
	//	{
	//		return NULL;				// If Not Return NULL
	//	}

	//	file=fopen(filename,"r");			// Check To See If The File Exists

	//	if (file)				// Does The File Exist?
	//	{
	//		fclose(file);				// Close The Handle
	//		return auxDIBImageLoadA(filename);		// Load The Bitmap And Return A Pointer
	//	}

	//	return NULL;				// If Load Failed Return NULL
	//}

	IplImage* LoadCVImage(const char* filename, int flags)
	{
		IplImage* iplImg = cvLoadImage(filename, flags);
		return iplImg;
	}

	int	Model::LoadGLTextures(const std::string &tex_name)
	{
		int   Status=FALSE;		                     // Status Indicator

		////AUX_RGBImageRec *TextureImage[1];		// Create Storage Space For The Texture
		//IplImage *TextureImage[1];
		////memset(TextureImage,0,sizeof(void *)*1);             // Set The Pointer To NULL

		//// Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit
		////if (TextureImage[0]=LoadBMP(tex_name.c_str()))
		//if (TextureImage[0]=LoadCVImage(tex_name.c_str(), CV_LOAD_IMAGE_COLOR))
		//{
		//	Status = TRUE;			// Set The Status To TRUE

		//	if(m_uiTexture[0] != NULL)
		//		glDeleteTextures(1, &m_uiTexture[0]);
		//	glGenTextures(1, &m_uiTexture[0]);		// Create The Texture

		//	// Typical Texture Generation Using Data From The Bitmap
		//	glBindTexture(GL_TEXTURE_2D, m_uiTexture[0]);

		//	//glTexImage2D(GL_TEXTURE_2D, 
		//	//	0, 
		//	//	3, 
		//	//	TextureImage[0]->sizeX,
		//	//	TextureImage[0]->sizeY,
		//	//	0, 
		//	//	GL_RGB,
		//	//	GL_UNSIGNED_BYTE,
		//	//	TextureImage[0]->data
		//	//	); 

		//	glTexImage2D(GL_TEXTURE_2D, 
		//		0, 
		//		3, 
		//		TextureImage[0]->width,
		//		TextureImage[0]->height,
		//		0, 
		//		GL_RGB,
		//		GL_UNSIGNED_BYTE,
		//		TextureImage[0]->imageData
		//		); 


		//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		//}
		////if (TextureImage[0])				// If Texture Exists
		////{
		////	if (TextureImage[0]->data)		// If Texture Image Exists
		////	{
		////		free(TextureImage[0]->data);		// Free The Texture Image Memory
		////	}

		////	free(TextureImage[0]);			// Free The Image Structure
		////}
		//cvReleaseImage(&TextureImage[0]);
		return Status;				// Return The Status
	}

	int	Model::LoadImage(const std::string &tex_name, size_t image_id)
	{
		//if(image_id < MAX_TEXTURE)
		//{
		//	//if(m_pImage[image_id] != NULL)
		//	//{
		//	//	cvReleaseImage(&m_pImage[image_id]);
		//	//	m_pImage[image_id] = NULL;
		//	//}
		//	if(m_matImage[image_id].data != NULL)
		//	{
		//		m_matImage[image_id].release();
		//	}

		//	//if (m_pImage[image_id]=LoadCVImage(tex_name.c_str(), CV_LOAD_IMAGE_COLOR))
		//	cv::Mat image=cv::imread(tex_name.c_str(), cv::IMREAD_ANYCOLOR);

		//	if(image.data != NULL)
		//	{
		//		cv::flip(image, m_matImage[image_id], 0);//des(i, j) = src(i,src.cols-j-1)
		//		//cv::flip(image, m_matImage[image_id], 1);//des(i, j) = src(src.rows-i-1,j)
		//		//cv::flip(image, m_matImage[image_id], -1);//des(i, j) = src(src.rows-i-1,src.cols-j-1)
		//		image.release();
		//		return TRUE;
		//	}
		//}
		m_uiCurVertex = -1;
		m_CurEditTex = new TextureInfo;
		cv::Mat image = cv::imread(tex_name.c_str(), cv::IMREAD_ANYCOLOR);
		printf("%d, %d\n", image.cols, image.rows);
		if ( image.data != NULL )
		{
			printf("load image success!\n");
			cv::flip(image, m_CurEditTex->imgMat, 0);
			image.release();
			m_CurEditTex->img_path = tex_name;
			m_TexInfos.push_back(m_CurEditTex);
			return TRUE;
		}
		else
		{
			printf("load image %s error!\n", tex_name.c_str());
			delete m_CurEditTex;
			m_CurEditTex = NULL;
		}
		return FALSE;
	}

	int	Model::GenTextures(size_t texture_id, size_t image_id)
	{
		//if(texture_id < MAX_TEXTURE && image_id < MAX_TEXTURE)
		//{
		//	if(m_uiTexture[texture_id] != NULL)
		//		glDeleteTextures(1, &m_uiTexture[texture_id]);	// Release Old Texture
		//	glGenTextures(1, &m_uiTexture[texture_id]);			// Create The Texture

		//	// Typical Texture Generation Using Data From The Bitmap
		//	glBindTexture(GL_TEXTURE_2D, m_uiTexture[texture_id]);

		//	//glTexImage2D(GL_TEXTURE_2D, 
		//	//	0, 
		//	//	3, 
		//	//	m_pImage[image_id]->width,
		//	//	m_pImage[image_id]->height,
		//	//	0, 
		//	//	GL_RGB,
		//	//	GL_UNSIGNED_BYTE,
		//	//	m_pImage[image_id]->imageData
		//	//	); 


		//	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		//	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

		//	//glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);
		//	//glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		//	//glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		//	//glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
		//	//glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
		//	//gluBuild2DMipmaps(GL_TEXTURE_2D, 
		//	//	3,
		//	//	m_pImage[image_id]->width,
		//	//	m_pImage[image_id]->height,
		//	//	GL_RGB,
		//	//	GL_UNSIGNED_BYTE,
		//	//	m_pImage[image_id]->imageData
		//	//	);
		//	//return TRUE;

		//	//glTexImage2D(
		//	//	GL_TEXTURE_2D, 
		//	//	0, 
		//	//	3, 
		//	//	m_matImage[image_id].cols, 
		//	//	m_matImage[image_id].rows, 
		//	//	0, 
		//	//	GL_BGR_EXT, 
		//	//	GL_UNSIGNED_BYTE, 
		//	//	m_matImage[image_id].data);

		//	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
		//	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
		//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
		//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
		//	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); 
		//	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE ); 

		//	if(m_matImage[image_id].channels() == 3 ) 
		//	{   
		//		glTexImage2D( 
		//			GL_TEXTURE_2D, 
		//			0, 
		//			GL_RGB, 
		//			m_matImage[image_id].cols, 
		//			m_matImage[image_id].rows, 
		//			0, 
		//			GL_BGR_EXT, 
		//			GL_UNSIGNED_BYTE, 
		//			m_matImage[image_id].data 
		//		);  
		//	} 
		//	else if( m_matImage[image_id].channels() == 4 ) 
		//	{   
		//		glTexImage2D( 
		//		GL_TEXTURE_2D, 
		//		0, 
		//		GL_RGBA, 
		//		m_matImage[image_id].cols, 
		//		m_matImage[image_id].rows, 
		//		0, 
		//		GL_BGRA_EXT, 
		//		GL_UNSIGNED_BYTE, 
		//		m_matImage[image_id].data 
		//		);  
		//	} 

		//	return TRUE;
		//}
		return FALSE;
	}

	int Model::GenTextures( int panelID )
	{
		if ( !m_CurEditTex )
			return false;
		glGenTextures(1, &(m_CurEditTex->texGID[panelID]) );			// Create The Texture
		glBindTexture(GL_TEXTURE_2D, m_CurEditTex->texGID[panelID]);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); 
		if(m_CurEditTex->imgMat.channels() == 3 ) 
		{   
			glTexImage2D( 
				GL_TEXTURE_2D, 
				0, 
				GL_RGB, 
				m_CurEditTex->imgMat.cols, 
				m_CurEditTex->imgMat.rows, 
				0, 
				GL_BGR_EXT, 
				GL_UNSIGNED_BYTE, 
				m_CurEditTex->imgMat.data 
				);  
		} 
		else if( m_CurEditTex->imgMat.channels() == 4 ) 
		{   
			glTexImage2D( 
				GL_TEXTURE_2D, 
				0, 
				GL_RGBA, 
				m_CurEditTex->imgMat.cols, 
				m_CurEditTex->imgMat.rows, 
				0, 
				GL_BGRA_EXT, 
				GL_UNSIGNED_BYTE, 
				m_CurEditTex->imgMat.data 
				);  
		} 

		return TRUE;
	}

	void Model::RenderTexture(void)
	{
		//if(m_uiTexture[0] != NULL)
		if(m_CurEditTex != NULL)
		{
			glEnable(GL_TEXTURE_2D);	// Enable Texture Mapping ( NEW )
			glShadeModel(GL_SMOOTH);	// Enable Smooth Shading
			glDisable(GL_LIGHTING);
			glDisable(GL_DEPTH_TEST);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			// 清除屏幕及深度緩存
			glLoadIdentity();
			glTranslatef(-0.5f, -0.5f, -1.25f);
			//glBindTexture(GL_TEXTURE_2D, m_uiTexture[0]);				// 選擇紋理
			glBindTexture(GL_TEXTURE_2D, m_CurEditTex->texGID[1]);
			glBegin(GL_QUADS);											//  繪製正方形
			glColor4f(1, 1, 1, 1);
			glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.0f,  0.0f,  0.0f);	// 紋理和四邊形的左下
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  0.0f,  0.0f);	// 紋理和四邊形的右下
			glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  0.0f);	// 紋理和四邊形的右上
			glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.0f,  1.0f,  0.0f);	// 紋理和四邊形的左上
			glEnd();													// 正方形繪製結束
			glDisable(GL_TEXTURE_2D);
		}
	}

	void Model::RenderUVMapping(void)
	{
		glPushAttrib(GL_LIGHTING_BIT);
		glPointSize(5.0f);
		glBegin(GL_LINE_STRIP);
		Point p;
		p[2] = 0.0001;
		SP_VERTEX_LIST::iterator v_itr = sp_uv_mapping_list.begin();
		for (v_itr; v_itr != sp_uv_mapping_list.end(); ++v_itr)
		{
			glColor3f(v_itr->r, v_itr->g, v_itr->b);
			p[0] = property(v_vec2dTexcoord, v_itr->vh)[0];
			p[1] = property(v_vec2dTexcoord, v_itr->vh)[1];
			glVertex3f(p[0], p[1], p[2]);
		}
		glEnd();
		glEnable(GL_LIGHTING);
		glDisable(GL_POLYGON_OFFSET_FILL);
	}

	void Model::drawRect(int _x,  int _y, float _w, float _h, GLint viewport[4], GLdouble modelview[16], GLdouble projection[16])
	{
		float l = _x - _w, r = _x + _w, u = _y - _h, d = _y + _h;
		l /= viewport[2]; r /= viewport[2];
		u /= viewport[3]; d /= viewport[3];
		glPushMatrix();
		glLoadIdentity();
		glTranslatef(-0.5f, -0.5f, -1.25f);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glBegin(GL_LINES);
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(l, u, 0);
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(r, u, 0);
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(r, u, 0);
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(r, d, 0);
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(r, d, 0);
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(l, d, 0);
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(l, d, 0);
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(l, u, 0);
		glEnd();
		glEnable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
		glPopMatrix();
	}

	void Model::drawCircle(float _x,  float _y, float _radin, float _r, float _g, float _b, int _n)
	{
		GLfloat winX, winY;
		GLint viewport[4];
		glPushMatrix();
		glMatrixMode(GL_VIEWPORT); //glMultMatrixd(xf.memptr());
		glGetIntegerv( GL_VIEWPORT, viewport );
		winX = (float)_x;
		winY = (float)viewport[3] - (float)_y;
		winX /= viewport[2];
		winY/= viewport[3];

		float pre_x, pre_y, cur_x, cur_y, a = 360.0/_n;
		int i;
		pre_x = winX+2*_radin*std::sin(0.001/360);
		pre_y = winY-2*_radin*std::cos(0.001/360);
		glPushMatrix();
		glLoadIdentity();
		glTranslatef(-0.5f, -0.5f, -1.25f);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glBegin(GL_LINES);
		for(i=1;i<=_n;i++)
		{
			cur_x = winX+2*_radin*std::sin(a*i*2*PI/360);
			cur_y = winY-2*_radin*std::cos(a*i*2*PI/360);
			glColor3f(_r, _g, _b);
			glVertex3f(pre_x, pre_y, 0);
			glColor3f(_r, _b, _b);
			glVertex3f(cur_x, cur_y, 0);
			pre_x = cur_x;
			pre_y = cur_y;
		}
		glEnd();
		glEnable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
		glPopMatrix();
	}

	void Model::VertexMapping(const FHandle &_f)
	{
		FVIter fv_ite;
		int i = 0;
		for(fv_ite = fv_iter(_f);fv_ite;++fv_ite)
		{
			if(i == 0)
			{
				property(v_vec2dTexcoord, fv_ite)[0] = 1.0;
				property(v_vec2dTexcoord, fv_ite)[1] = 0.5;
			}
			else if(i == 1)
			{
				property(v_vec2dTexcoord, fv_ite)[0] = 0.5;
				property(v_vec2dTexcoord, fv_ite)[1] = 1.0;
			}
			else if(i == 2)
			{
				property(v_vec2dTexcoord, fv_ite)[0] = 0.0;
				property(v_vec2dTexcoord, fv_ite)[1] = 0.5;
			}

			i++;
			add_sp_mapping(fv_ite, 1.0, 0.0, 0.0);
		}
	}

	void Model::clear_sp_mapping(void)
	{
		sp_uv_mapping_list.clear();
	}

	void Model::add_sp_mapping(const VHandle &_v, float _r, float _g, float _b)
	{
		sp_v v;
		v.vh = _v;
		v.r = _r;
		v.g = _g;
		v.b = _b;
		sp_uv_mapping_list.push_back(v);
	}

	void Model::selectFace(int _x, int _y, float _u, float _r, float _d, float _l, GLint viewport[4], GLdouble modelview[16], GLdouble projection[16])
	{
		Point _p[3];
		GLdouble x, y, z;
		float up = _y - _u;
		float down = _y + _d;
		float left = _x - _l;
		float right = _x + _r;
		FaceIter f_ite;
		FVIter fv_ite;
		bool isSelected;
		Vec3d view_dir;
		view_dir[0] = modelview[8];
		view_dir[1] = modelview[9];
		view_dir[2] = modelview[10];
		int index;

		for(f_ite = faces_begin();f_ite != faces_end();++f_ite)
		{
			isSelected = false;
			index = 0;
			//Vec3d nor = normal(f_ite);
			//if(dot(view_dir, nor) < 0)
			//	continue;
			for(fv_ite = fv_iter(f_ite);fv_ite; ++fv_ite, ++index)
			{
				_p[index] = point(fv_ite);
				gluProject( _p[index][0], _p[index][1], _p[index][2], modelview, projection, viewport, &x, &y, &z);
				if(x > left && x < right && y > up && y < down)
				{
					//veiw ray dot normal < 0 才要設定
					isSelected = true;
				}
				else
					continue;
			}
			if(isSelected)
			{
				Vec3d nor = cross(_p[0] - _p[1], _p[1] - _p[2]);
				if(dot(view_dir, nor) > 0)
				{
					add_mapping_face(f_ite.handle());
					add_sp_f(f_ite.handle(), 1.0, 0.5, 0.0);
				}
			}
		}
	}

	void Model::SelectNring(int n, int _x,  int _y, float radin)
	{
		GLfloat winX, winY, winZ;
		GLint viewport[4];
		GLdouble modelview[16];
		GLdouble projection[16];
		GLdouble x, y, z;
		Point curPoint;

		glPushMatrix();
		glMatrixMode(GL_MODELVIEW);	//glMultMatrixd(xf.memptr());
		glGetDoublev( GL_MODELVIEW_MATRIX, modelview );

		glMatrixMode(GL_PROJECTION_MATRIX);	//glMultMatrixd(xf.memptr());
		glGetDoublev( GL_PROJECTION_MATRIX, projection );

		glMatrixMode(GL_VIEWPORT); //glMultMatrixd(xf.memptr());
		glGetIntegerv( GL_VIEWPORT, viewport );

		winX = (float)_x;
		winY = (float)viewport[3] - (float)_y;

		Vec2d screen(winX, winY);
		float max_radin = radin * viewport[2];
		glReadPixels( int(winX), int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

		if(winZ>=0.99999f)
		{
			//std::cerr << "Click on background (z= " << winZ << ")" << std::endl;
			glPopMatrix();
			return;
		}

		gluUnProject( winX, winY, winZ, modelview, projection, viewport, &x, &y, &z);
		Point p(x, y, z);

		gluUnProject( winX, winY, 0, modelview, projection, viewport, &x, &y, &z);

		Point rayDir = p - Point(x, y, z);

		ClearSelectFaces();
		FHandle org_fid = FindFace(p, rayDir);
		if (!is_valid_handle(org_fid))
			return ;
		FHandle org_fh = org_fid;
		property(SelRingID, org_fh) = 0;
		for (FVIter fv_it = fv_iter(org_fh); fv_it; ++fv_it)
			property( SelVID, fv_it.handle() ) = 1;
		sel_faces.push_back( org_fh );
		float max_dist = 0.0;
		int curRing = 0;
		int NextStartID = 0;
		while(curRing < n)
		{
			int curSize = sel_faces.size();
			for (int i = NextStartID; i < curSize; i++)
			{
				for (FFIter ff_it = ff_iter( sel_faces[i] ); ff_it; ++ff_it)
				{
					if ( property( SelRingID, ff_it.handle() ) < 0)
					{
						for (FVIter fv_it = fv_iter(ff_it.handle()); fv_it; ++fv_it)
						{
							if(property( SelVID, fv_it.handle() ) != 1)
							{
								curPoint = point(fv_it.handle());
								gluProject( curPoint[0], curPoint[1], curPoint[2], modelview, projection, viewport, &x, &y, &z);
								float dist = (screen - Vec2d(x, y)).length();
								//printf("radin:%f, distance:%f, vertex:%f,%f,%f\n", max_radin, dist, x, y, z);
								if(dist > max_dist)
								{
									max_dist = dist;
								}
								property( SelVID, fv_it.handle() ) = 1;
							}
						}
						property( SelRingID, ff_it.handle() ) = curRing+1;
						sel_faces.push_back( ff_it.handle() );
					}
				}
			}
			NextStartID = curSize;
			curRing++;
			if(max_dist > max_radin)
				break;
		}
		glPopMatrix();
	}

	void Model::Selected(void)
	{
		FindBound();
		FixBoundShape();
		MapBoundTo2D();
		FillCenter();
	}

	void Model::RenderSelectVertex(void)
	{
		VIter v_ite;
		glPushAttrib(GL_LIGHTING_BIT);
		glDisable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
		glPointSize(5.0f);
		glBegin(GL_POINTS);
		glColor3f(1.0, 0.0, 0.0);
		for (v_ite = vertices_begin(); v_ite != vertices_end(); ++v_ite)
		{
			if(property(SelVID, v_ite) == 1)
				glVertex3dv(&point(v_ite)[0]);
		}
		glEnd();
		glEnable(GL_LIGHTING);
		glDisable(GL_POLYGON_OFFSET_FILL);
	}

	void Model::RenderSelectFace(void)
	{
		glDisable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		glPushAttrib(GL_LIGHTING_BIT);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(0.5, 1.0);
		glDisable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
		glBegin(GL_QUADS);
		FVIter fv_ite;
		FIter f_ite;
		glColor3f(1.0, 0.0, 0.0);
		Point vertex[3];
		Point nor[3];
		bool bIsDraw;
		int i;
		for (f_ite = faces_begin(); f_ite != faces_end(); ++f_ite)
		{
			bIsDraw = true;
			for (fv_ite=fv_iter(f_ite), i = 0; fv_ite; ++fv_ite, i++)
			{						
				if(property(SelVID, fv_ite) == 1)
				{
					vertex[i] = point(fv_ite.handle());
					nor[i] = normal(fv_ite.handle());
				}
				else
				{
					bIsDraw = false;
					break;
				}
			}
			if(bIsDraw)
			{
				for(i = 0; i < 3; i++)
				{
					glNormal3dv(&nor[i][0]);
					glVertex3dv(&vertex[i][0]);
				}
			}
		}
		glEnd();		
		glEnable(GL_LIGHTING);
		glDisable(GL_POLYGON_OFFSET_FILL);
		glPolygonMode(GL_FRONT,GL_FILL);
		glEnable(GL_CULL_FACE);
	}

	void Model::add_mapping_face(FHandle &_f)
	{
		sp_f f;
		f.fh = _f;
		property(f_bIsSelect, _f) = true;
		sp_mapping_f_list.push_back(f);
	}

	void Model::clear_mapping_face(void)
	{
		SP_FACE_LIST::iterator ite;
		for(ite = sp_mapping_f_list.begin(); ite != sp_mapping_f_list.end();++ite)
		{
			property(f_bIsSelect, ite->fh) = false;
		}
		sp_mapping_f_list.clear();
	}

	bool Model::selectUVVertex(float u, float v)
	{
		//printf("u:%f, v:%f\n", u, v);
		Vec2d uv(u, v);
		float min_dist = 1000.0;
		float threshold = 0.01;
		for(unsigned int i = 0; i < m_CurEditTex->UVs.size();++i)
		{
			float dist = (m_CurEditTex->UVs[i] - uv).length();
			
			if(dist < min_dist)
			{
				//printf("dist:%f, u:%f, v:%f\n", dist, m_CurEditTex->UVs[i][0], m_CurEditTex->UVs[i][1]);
				min_dist = dist;
				if(min_dist < threshold)
				{
					printf("dist:%f, u:%f, v:%f\n", dist, m_CurEditTex->UVs[i][0], m_CurEditTex->UVs[i][1]);
					m_uiCurVertex = i;
					break;
				}
			}
		}
		return m_uiCurVertex != -1;
	}

	void Model::moveUVVertex(float u, float v)
	{
		if(m_CurEditTex && m_uiCurVertex < m_CurEditTex->UVs.size())
		{
			//printf("u:%f, v:%f\n", u, v);
			m_CurEditTex->UVs[m_uiCurVertex][0] = u;
			m_CurEditTex->UVs[m_uiCurVertex][1] = v;
		}
	}

	void Model::scaleUVVertex(float scalex, float scaley)
	{
		if(m_CurEditTex)
		{
			printf("scalex:%f, %f", scalex, scaley);
			unsigned int index;
			Vec2d center(0.5, 0.5);
			Vec2d scale(scalex, scaley);
			for(index = 0;index < m_CurEditTex->UVs.size();++index)
			{
				m_CurEditTex->UVs[index] = (m_CurEditTex->UVs[index]- center) * scale + center;
			}
		}
	}

	void Model::rotationUVVertex(float angle)
	{
		if(m_CurEditTex)
		{
			float theta = angle/360*2*PI;
			printf("angle:%f", theta);
			unsigned int index;
			Vec2d center(0.5, 0.5);
			//Vec2d rotation(std::cos(theta), std::sin(theta));
			Vec2d diff;
			Vec2d newVec;
			for(index = 0;index < m_CurEditTex->UVs.size();++index)
			{
				diff = m_CurEditTex->UVs[index]- center;
				newVec[0] = diff[0] * cos(theta) - diff[1] * sin(theta);
				newVec[1] = diff[0] * sin(theta) + diff[1] * cos(theta);
				m_CurEditTex->UVs[index] = newVec + center;
			}
		}
	}

	void Model::renderSelectPoint(void)
	{
		if(m_CurEditTex != NULL && m_uiCurVertex < m_CurEditTex->UVs.size())
		{
			glPushAttrib(GL_LIGHTING_BIT);
			glDisable(GL_LIGHTING);
			glEnable(GL_DEPTH_TEST);
			glPointSize(10.0f);

			glBegin(GL_POINTS);
			glColor3f(1, 1, 0);

			
			glVertex3d( m_CurEditTex->UVs[m_uiCurVertex][0], m_CurEditTex->UVs[m_uiCurVertex][1], 0 );
			glEnd();

			glEnable(GL_LIGHTING);
			glDisable(GL_POLYGON_OFFSET_FILL);
		}
	}

	/*---------------------------------PARAMETERIZATION-----------------------------*/
	double Model::RayTraceFace(OMP::FHandle& f, OMP::Point& p, OMP::Vector3d& rayDir)
	{
		double dir = -1;
		bool positiveSide = true;
		bool negativeSide = true;

		int i = 0;
		OMP::Point tp[3];
		OMP::Vector3d v[3];
		OMP::Vector3d tNormal;
		for (OMP::FVIter fv_itr=fv_iter(f); fv_itr; ++fv_itr, i++)
		{
			tp[i] = point(fv_itr.handle());
		}
		tNormal = cross( tp[1] - tp[0], tp[2] - tp[0] );
		// 			tNormal[0]/=3;
		// 			tNormal[1]/=3;
		// 			tNormal[2]/=3;

		double denom = dot( tNormal, rayDir );
		if ( denom > + std::numeric_limits<double>::epsilon() )
		{
			if (!negativeSide)	return -1;
		}
		else if ( denom < - std::numeric_limits<double>::epsilon() )
		{
			if (!positiveSide)  return -1;
		}
		else
		{
			// Parallel or triangle area is close to zero when
			// the plane normal not normalised.
			return -1;
		}

		dir = dot( tNormal, tp[0] - p ) / denom;
		if ( dir < 0 )
			return -1;


		double n0 = fabs( tNormal[0] );
		double n1 = fabs( tNormal[1] );
		double n2 = fabs( tNormal[2] );
		// Calculate the largest area projection plane in X, Y or Z.
		int i0 = 1, i1 = 2;
		if (n1 > n2)
		{
			if (n1 > n0) i0=0;
		}
		else
		{
			if (n2>n0) i1=0;
		}

		double u1 = tp[1][i0] - tp[0][i0];
		double v1 = tp[1][i1] - tp[0][i1];
		double u2 = tp[2][i0] - tp[0][i0];
		double v2 = tp[2][i1] - tp[0][i1];
		double u0 = dir * rayDir[i0] + p[i0] - tp[0][i0];
		double v0 = dir * rayDir[i1] + p[i1] - tp[0][i1];

		double alpha = u0 * v2 - u2 * v0;
		double beta  = u1 * v0 - u0 * v1;
		double area  = u1 * v2 - u2 * v1;

		const double EPSILON = 1e-6f;
		double tolerance = -EPSILON*area;
		if (area > 0)
		{
			if (alpha < tolerance || beta < tolerance || alpha+beta > area-tolerance)
				return -1;
		}
		else
		{
			if (alpha > tolerance || beta > tolerance || alpha+beta < area-tolerance)
				return -1;
		}

		return dir;
	}
	/*======================================================================*/
	FHandle Model::FindFace( Point& p, Vector3d& rayDir )
	{
		OMP::FIter f_it;
		double minDir = 1000;
		OMP::FHandle fh = faces_begin().handle();
		for (f_it = faces_begin(); f_it != faces_end(); ++f_it) 
		{
			double dir = RayTraceFace(f_it.handle(), p, rayDir) ;
			if (dir > 0 && dir<minDir) {
				minDir = dir;
				fh = f_it.handle();
				//mesh->add_sp_f(f_it.handle(), 0.0f, 1.0f, 0.0f);
				//return f_it.handle();
			}
		}
		return fh;
	}

	void Model::SelectNring( int n, Point& p, Vector3d& rayDir )
	{
		ClearSelectFaces();
		FHandle org_fid = FindFace(p, rayDir);
		if (!is_valid_handle(org_fid))
			return ;
		FHandle org_fh = org_fid;
		property(SelRingID, org_fh) = 0;
		for (FVIter fv_it = fv_iter(org_fh); fv_it; ++fv_it)
			property( SelVID, fv_it.handle() ) = 1;
		sel_faces.push_back( org_fh );
		int curRing = 0;
		int NextStartID = 0;
		while(curRing < n)
		{
			int curSize = sel_faces.size();
			for (int i = NextStartID; i < curSize; i++)
			{
				for (FFIter ff_it = ff_iter( sel_faces[i] ); ff_it; ++ff_it)
				{
					if ( property( SelRingID, ff_it.handle() ) < 0 )
					{
						property( SelRingID, ff_it.handle() ) = curRing+1;
						sel_faces.push_back( ff_it.handle() );
						for (FVIter fv_it = fv_iter(ff_it.handle()); fv_it; ++fv_it)
							property( SelVID, fv_it.handle() ) = 1;
					}
				}
			}
			NextStartID = curSize;
			curRing++;
		}


		FindBound();
		FixBoundShape();
		MapBoundTo2D();
		FillCenter();
	}

	void Model::ClearSelectFaces()
	{
		sel_faces.clear();
		for (FIter f_it = faces_begin(); f_it != faces_end(); ++f_it)
		{
			property(SelRingID, f_it) = -1;
		}
		for (VIter v_it = vertices_begin(); v_it != vertices_end(); ++v_it)
		{
			property(SelVID, v_it) = 0;
		}
	}

	void Model::FindBound()
	{
		EHandle boundE;
		for (int i = sel_faces.size()-1; i>=0; i++)
		{
			for ( FEIter fe_it = fe_iter(sel_faces[i]); fe_it; ++fe_it)
			{
				if ( IsBoundEdge( fe_it.handle() ) )
				{
					boundE = fe_it.handle();
					break;
				}
			}
			if ( is_valid_handle(boundE) )
				break;
		}
		HEHandle startHEH = halfedge_handle(boundE, 0);
		VHandle startV = from_vertex_handle(startHEH);
		VHandle lastV = startV;
		VHandle nextV = to_vertex_handle(startHEH);
		bound_Vex.clear();
		bound_Vex.push_back(nextV);
		property( SelVID, nextV ) = -1;
		int nextBoundID = -2;
		while( startV != nextV )
		{
			for (VOHIter voh_it = voh_iter(nextV) ;voh_it ; ++voh_it )
			{
				EHandle eh = edge_handle( voh_it.handle() );
				if ( IsBoundEdge(eh) )
				{
					VHandle vh = to_vertex_handle(voh_it.handle());
					if (vh != lastV)
					{
						lastV = nextV;
						nextV = vh;
						property( SelVID, vh ) = nextBoundID;
						nextBoundID--;
						bound_Vex.push_back(vh);
						break;
					}
				}
			}
		}
	}

	void Model::Parameterization()
	{

	}

	void Model::MapBoundTo2D()
	{
		double boundLen = 0;
		int boundVNum = bound_Vex.size();
		BoundVexIn2D.resize( boundVNum );
		for (int i=1; i<boundVNum; i++)
		{
			boundLen+= (point( bound_Vex[i] ) - point(bound_Vex[i-1]) ).length();
		}
		BoundVexIn2D[0] = Vec2d(1, 0.5);
		double curLen = 0;
		for (int i=1; i<boundVNum; i++)
		{
			curLen += (point( bound_Vex[i] ) - point(bound_Vex[i-1]) ).length();
			double angle = 2*3.1415926*curLen/boundLen;
			BoundVexIn2D[i] = Vec2d(cos(angle), sin(angle))*0.5+Vec2d(0.5, 0.5);
		}
// 		GLint	  glViewport[4];
// 		GLdouble  glModelview[16];
// 		GLdouble  glProjection[16];
// 		double    pos[3];
// 		GLdouble sx, sy, sz;
// 		float alpha = 0.9;
// 		float maxx = 0, minx = 100, maxy = 0, miny = 100;
// 
// 		glMatrixMode( GL_PROJECTION );
// 		glGetDoublev( GL_PROJECTION_MATRIX, glProjection );
// 		glMatrixMode( GL_MODELVIEW );
// 		glGetDoublev( GL_MODELVIEW_MATRIX, glModelview );
// 		glGetIntegerv( GL_VIEWPORT, glViewport );
// 
// 		for (int i=0; i<bound_Vex.size(); i++)
// 		{
// 			Point curPoint = point( bound_Vex[i] );
// 			gluProject(curPoint[0], curPoint[1], curPoint[2], glModelview, glProjection, glViewport, &sx, &sy, &sz);
// 			BoundVexIn2D[i][0] = sx/glViewport[2];
// 			BoundVexIn2D[i][1] = sy/glViewport[3];
// 
// 			if(BoundVexIn2D[i][0] > maxx)
// 				maxx = BoundVexIn2D[i][0];
// 			if(BoundVexIn2D[i][0] < minx)
// 				minx = BoundVexIn2D[i][0];
// 			if(BoundVexIn2D[i][1] > maxy)
// 				maxy = BoundVexIn2D[i][1];
// 			if(BoundVexIn2D[i][1] < miny)
// 				miny = BoundVexIn2D[i][1];
// 			printf("x:%f y:%f\n", BoundVexIn2D[i][0], BoundVexIn2D[i][1]);
// 		}

// 		for(int i=0; i<bound_Vex.size(); i++)
// 		{
// 			BoundVexIn2D[i][0] -= minx;
// 			BoundVexIn2D[i][0] /= (maxx - minx);
// 
// 			BoundVexIn2D[i][1] -= miny;
// 			BoundVexIn2D[i][1] /= (maxy - miny);
// 		}
		
	}

	void Model::FixBoundShape()
	{
		int sizeN = sel_faces.size();
		for (int i=0; i<sizeN; i++)
		{
			for (FFIter ff_it = ff_iter(sel_faces[i]); ff_it; ++ff_it)
			{
				if ( property(SelRingID, ff_it) >=0 )
					continue;
				int BoundEdgeN = 0;
				for (FEIter fe_it = fe_iter(ff_it); fe_it; ++fe_it)
				{
					if ( IsBoundEdge(fe_it) )
						BoundEdgeN++;
				}
				if (BoundEdgeN>=2)
				{
					sel_faces.push_back(ff_it);
					property(SelRingID, ff_it) = property(SelRingID, sel_faces.back());
				}
			}
		}
	}

	void Model::FillCenter()
	{
		std::set<int> centerVidSet;
		for (int i=0; i<sel_faces.size(); i++)
		{
			for (FVIter fv_it = fv_iter(sel_faces[i]); fv_it; ++fv_it)
			{
				if ( property(SelVID, fv_it.handle()) == 1 )
					centerVidSet.insert(fv_it.handle().idx());
			}
		}
		int nextVid = 1;
		centerVid = std::vector<int>(centerVidSet.begin(), centerVidSet.end());

		for (int i=0; i<centerVid.size(); i++)
		{
			property( SelVID, vertex_handle(centerVid[i]) ) = nextVid;
			nextVid++;
		}

		using namespace LinearSystemLib;
		GeneralSparseMatrix SM;
		int unkown_v_num = centerVid.size();

		printf("unknow num:%d\n", unkown_v_num);
		SM.Create( unkown_v_num, unkown_v_num );
		const int dim = 2;
		double** B = new double*[dim];
		B[0] = new double[unkown_v_num];
		B[1] = new double[unkown_v_num];
		int i=0, j=0;
		for (i=0; i<unkown_v_num; i++)
		{
			B[0][i] = 0;
			B[1][i] = 0;
		}

		for (i=0; i<unkown_v_num; i++)
		{
			double w_sum = 0;
			VHandle curVh = vertex_handle(centerVid[i]);
			B[0][i] = 0;
			B[1][i] = 0;
			for ( VVIter vv_it = vv_iter(curVh); vv_it; ++vv_it )
			{
				int mapID = property(SelVID, vv_it.handle());
				if ( mapID == 0 )
				{
					printf("selected vertex may be not center!\n");
					continue;
				}
				double w = calCotWeight(curVh, vv_it.handle());
				w_sum+=w;
				if ( mapID < 0 )
				{
					B[0][i] += w*BoundVexIn2D[-mapID-1][0];
					B[1][i] += w*BoundVexIn2D[-mapID-1][1];
				}
				else if ( mapID>0 )
				{
					SM.SetElement(i, mapID-1, -w);
					printf( "[%d, %d]=%f, ", i, mapID-1, float(-w) );
				}

			}
			SM.SetElement(i, i, w_sum);
		}

		SparseLinearSystem sls( new StableSparseMatrix(SM), B, 2 );
		double** xxx = 0; 
		bool result = GeneralSparseLSSolver::GetInstance()->Solve( &sls, xxx );
		CenterVexIn2D.clear();
		for (i=0; i<unkown_v_num; i++)
		{
			Vec2d newP(xxx[0][i], xxx[1][i]);
			CenterVexIn2D.push_back(newP);
			printf("center%d %f %f\n", i, xxx[0][i], xxx[1][i]);
		}

		for (i=0; i<dim; i++)
			delete[] xxx[i];
		delete[] xxx;

		int cntVNum = CenterVexIn2D.size();
		int totalVnum = BoundVexIn2D.size() + CenterVexIn2D.size();
		std::vector<int> AllSelVhSet;
		AllSelVhSet.resize(totalVnum);
		m_CurEditTex->UVs.resize(totalVnum);
		for ( int i=0; i<cntVNum; i++ )
			m_CurEditTex->UVs[i] = CenterVexIn2D[i];
		for (int i=cntVNum; i<totalVnum; i++)
			m_CurEditTex->UVs[i] = BoundVexIn2D[i-cntVNum];

		m_CurEditTex->FV_UV.clear();
		for (int i=0; i<sel_faces.size(); i++)
		{
			TextureForFace* tff = new TextureForFace;
			tff->texInfo = m_CurEditTex;
			TextureForFace* curFaceBackTff = property(fTex, sel_faces[i]);
			if ( curFaceBackTff == NULL )
			{
				property(fTex, sel_faces[i]) = tff;
			}
			else
			{
				while( curFaceBackTff->nextTexture != NULL )
					curFaceBackTff = curFaceBackTff->nextTexture;
				curFaceBackTff->nextTexture = tff;
			}

			int vhid = 0;
			for (FVIter fv_it = fv_iter(sel_faces[i]); fv_it; ++fv_it)
			{
				if ( property(SelVID, fv_it.handle()) < 0 )
					tff->uvmap[vhid] = -property(SelVID, fv_it.handle()) + cntVNum - 1;
				else if ( property(SelVID, fv_it.handle()) > 0 )
					tff->uvmap[vhid] = property(SelVID, fv_it.handle()) - 1;
				m_CurEditTex->FV_UV.push_back(tff->uvmap[vhid]);
				vhid++;
			}
		}
	}

	double Model::calCotWeight( VHandle vh1, VHandle vh2 )
	{
		HEHandle heh = find_halfedge(vh1, vh2);
		VHandle o_vh1 = opposite_vh(heh);
		VHandle o_vh2 = opposite_he_opposite_vh(heh);

		Point vh1_p = point(vh1);
		Point vh2_p = point(vh2);
		Point o_vh1_p, o_vh2_p;
		double angle_a, angle_b;

		if(o_vh1.is_valid())
		{
			o_vh1_p = point(o_vh1);
			Vec3d vh11 = vh1_p - o_vh1_p;
			Vec3d vh12 = vh2_p - o_vh1_p;

			angle_a = dot(vh11,vh12) / (vh11.length()*vh12.length()) ;
			angle_a = acos(angle_a);
		}

		if(o_vh2.is_valid())
		{
			o_vh2_p = point(o_vh2);
			Vec3d vh21 = vh1_p-o_vh2_p; 
			Vec3d vh22 = vh2_p-o_vh2_p;

			angle_b = dot(vh21,vh22) / ( vh21.length()*vh22.length() );
			angle_b = acos(angle_b);
		}
		double h_weight = 0;
		if(o_vh1.is_valid() && o_vh2.is_valid())
			h_weight = 0.5*((1.0/tan(angle_a))+(1.0/tan(angle_b))) ;
		if(o_vh1.is_valid() && !o_vh2.is_valid())
			h_weight = 0.5*((1.0/tan(angle_a))) ;
		if(!o_vh1.is_valid() && o_vh2.is_valid())
			h_weight = 0.5*((1.0/tan(angle_b))) ;

		return h_weight;
	}

	void Model::RenderBound2D( float r, float g, float b )
	{		
		if(m_CurEditTex != NULL)
		{
			glPushAttrib(GL_LIGHTING_BIT);
			glDisable(GL_LIGHTING);
			glEnable(GL_DEPTH_TEST);
			glPointSize(5.0f);
			//glBegin(GL_LINE_STRIP);

			//glColor3f(r, g, b);
			//for (int i=0; i<BoundVexIn2D.size(); i++)
			//{
			//	glVertex3d( BoundVexIn2D[i][0], BoundVexIn2D[i][1], 0 );
			//}
			//glEnd();

			glBegin(GL_POINTS);
			glColor3f(1, 0, 0);
			//for (int i=0; i<CenterVexIn2D.size(); i++)
			//{
			//	glVertex3d( CenterVexIn2D[i][0], CenterVexIn2D[i][1], 0 );
			//}
			for (int i=0; i<m_CurEditTex->UVs.size(); i++)
			{
				glVertex3d( m_CurEditTex->UVs[i][0], m_CurEditTex->UVs[i][1], 0 );
			}
			glEnd();
			glEnable(GL_LIGHTING);
			glDisable(GL_POLYGON_OFFSET_FILL);
		}
	}

	void Model::RenderTextureToModel()
	{
		// 		if(m_uiTexture[0] == NULL)
		// 			return;
		glEnable(GL_TEXTURE_2D);	// Enable Texture Mapping ( NEW )
		glShadeModel(GL_SMOOTH);	// Enable Smooth Shading
		glDisable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		for (FIter f_it = faces_begin(); f_it!=faces_end(); ++f_it)
		{
			TextureForFace* tff = property(fTex, f_it);
			int curTime = 0;
			while( tff!=NULL )
			{
				if (curTime==0)
				{
					glDisable(GL_BLEND);
				}
				else
				{
					glEnable(GL_BLEND);
					glBlendEquation(GL_FUNC_ADD);
					//glDisable(GL_DEPTH_TEST);
					glBlendFunc(GL_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_COLOR);
					glBlendColor(1.0/(curTime+2.0), 1.0/(curTime+2.0), 1.0/(curTime+2.0), 1);
				}
				glBindTexture(GL_TEXTURE_2D, tff->texInfo->texGID[0]);
				glBegin(GL_TRIANGLES);
				glColor3f(1, 1, 1);
				int cur_v_count = 0;
				for (FVIter fv_it = fv_iter(f_it); fv_it; ++fv_it)
				{
					int texUvMapID = tff->uvmap[cur_v_count];
					glTexCoord2dv( &(tff->texInfo->UVs[texUvMapID][0]) );
					glNormal3dv(&normal(fv_it.handle())[0]);
					glVertex3dv(&point(fv_it.handle())[0]);
					cur_v_count++;
				}
				glEnd();
				tff = tff->nextTexture;
				curTime++;
			}
		}
		glDepthFunc(GL_LESS);
		glDisable(GL_TEXTURE_2D);	// Enable Texture Mapping ( NEW )
		glDisable(GL_BLEND);
	}

	void Model::InitModelProperty()
	{
		for (FIter f_it = faces_begin(); f_it!=faces_end(); ++f_it)
		{
			property( fTex , f_it.handle()) = NULL;
		}
	}

	void Model::ClearAllTexture()
	{
		for (int i=0; i<m_TexInfos.size(); i++)
		{
			TextureInfo* tf = m_TexInfos[i];
			tf->imgMat.release();

			if(tf->texGID[0] != NULL)
				glDeleteTextures(1, &tf->texGID[0]);
			if(tf->texGID[1] != NULL)
				glDeleteTextures(1, &tf->texGID[1]);

			m_TexInfos[i]->UVs.clear();
			delete m_TexInfos[i];
		}
		m_TexInfos.clear();

		for (FIter f_it = faces_begin(); f_it!=faces_end(); ++f_it)
		{
			TextureForFace* tff = property(fTex, f_it);
			while(tff)
			{
				TextureForFace* next_tff = tff->nextTexture;
				delete tff;
				tff = next_tff;
			}
			property(fTex, f_it) = NULL;
		}
		m_CurEditTex = NULL;
	}

	void Model::SaveTextureInfoToFile( const std::string &f_name )
	{
		FILE* fp = fopen(f_name.c_str(), "w");
		fprintf(fp, "%d\n", m_TexInfos.size());
		for (int i=0; i<m_TexInfos.size(); i++)
		{
			fprintf(fp, "%s\n", m_TexInfos[i]->img_path);
			int uvNum = m_TexInfos[i]->UVs.size();
			fprintf(fp, "%d\n", uvNum);
			for (int j=0; j<uvNum; j++ )
			{
				fprintf(fp, "%f %f ", (float)m_TexInfos[i]->UVs[j][0], (float)m_TexInfos[i]->UVs[j][1] );
			}
			fprintf(fp, "\n");
		}

		for (FIter f_it = faces_begin(); f_it != faces_end(); ++f_it)
		{
			TextureForFace* tff = property(fTex, f_it);
			while(tff!=NULL)
			{
				fprintf( fp, "%d ", f_it.handle().idx() );
				for (int i=0; i<m_TexInfos.size(); i++)
				{
					if (m_TexInfos[i] == tff->texInfo)
					{
						fprintf( fp, "%d\n", i );
						break;
					}
				}
				for (int i=0; i<3; i++)
				{
					fprintf( fp, "%d ", tff->uvmap[i] );
				}
				tff = tff->nextTexture;
			}
		}
		fclose(fp);
	}

	bool Model::LoadTextureInfoFromFile( const std::string &f_name )
	{
		char tex_path[128];
		int texNum = -1;
		ClearAllTexture();
		FILE* fp = fopen(f_name.c_str(), "r");
		if (!fp)
			return false;
		fscanf(fp, "%d\n", &texNum);
		if (texNum<0)
			return false;
		for (int i=0; i<texNum; i++)
		{
			fgets(tex_path, 128, fp);
			for (int j=0; j<128; j++)
			{
				if (tex_path[j]=='\0' || tex_path[j]=='\n')
				{
					if (tex_path[j]=='\n')
						tex_path[j] = '\0';
					break;
				}
			}
			LoadImage(tex_path, 0);
			m_CurEditTex->img_path = tex_path;
			int uvNum = -1;
			fscanf(fp, "%d\n", &uvNum);
			float uvX, uvY;
			for (int j=0; j<uvNum; j++)
			{
				fscanf(fp, "%f %f ", &uvX, &uvY);
				m_CurEditTex->UVs.push_back( OpenMesh::Vec2d(uvX, uvY) );
			}
		}
		int fid = -1;
		while( fscanf(fp, "%d", &fid) == 1 )
		{
			if (fid<0)
				break;
			FHandle fh = face_handle(fid);
			if (!is_valid_handle(fh))
			{
				printf("have valid id!\n");
				break;
			}
			int mapTexId = -1;
			fscanf(fp, "%d", &mapTexId);
			if (mapTexId<0 || mapTexId>=m_TexInfos.size())
			{
				printf("have valid map tex id!\n");
				break;
			}
			TextureForFace* tff = new TextureForFace;
			tff->texInfo = m_TexInfos[mapTexId];
			tff->nextTexture = NULL;
			int uvID[3];
			fscanf(fp, "%d %d %d", uvID, uvID+1, uvID+2);
			for (int i=0; i<3; i++)
			{
				tff->uvmap[i] = uvID[i];
				tff->texInfo->FV_UV.push_back(tff->uvmap[i]);
			}

			TextureForFace* cur_tff = property(fTex, fh);
			if (cur_tff == NULL)
			{
				property(fTex, fh) = tff;
			}
			else
			{
				while(cur_tff->nextTexture!=NULL)
					cur_tff = cur_tff->nextTexture;
				cur_tff->nextTexture = tff;
			}
		}
		fclose(fp);
		return true;
	}


	void Model::GenAllTex(unsigned int paneID )
	{
		for (int i=0; i < m_TexInfos.size(); i++)
		{
			m_CurEditTex = m_TexInfos[i];
			GenTextures(paneID);
		}
	}

	void Model::RenderUVPoint( float r, float g, float b )
	{
		if (!m_CurEditTex)
			return ;
		glDisable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_DEPTH_TEST);
		glBegin(GL_TRIANGLES);
		glColor3f(r, g, b);
		for (int i=0; i<m_CurEditTex->FV_UV.size(); i+=3)
		{
			int fvuv = m_CurEditTex->FV_UV[i];
			glVertex2dv( &(m_CurEditTex->UVs[fvuv][0]) );
			fvuv = m_CurEditTex->FV_UV[i+1];
			glVertex2dv( &(m_CurEditTex->UVs[fvuv][0]) );
			fvuv = m_CurEditTex->FV_UV[i+2];
			glVertex2dv( &(m_CurEditTex->UVs[fvuv][0]) );
// 			FHandle fh = m_CurEditTex->FV_UV[i];
// 			TextureForFace* tff = property(fTex, fh);
// 			while(tff!=NULL)
// 			{
// 				if (tff->texInfo == m_CurEditTex)
// 				{
// 					for (int i=0; i<3; i++)
// 					{
// 						int v_to_uv_id = tff->uvmap[i];
// 						glVertex2dv( &(m_CurEditTex->UVs[v_to_uv_id][0]) );
// 					}
// 				}
// 				tff = tff->nextTexture;
// 			}
		}
		glEnd();
		glPolygonMode(GL_FRONT, GL_FILL);
		glEnable(GL_CULL_FACE);

// 		glBegin(GL_POINTS);
// 		for (int i=0; i<m_CurEditTex->UVs.size(); i++)
// 		{
// 			glVertex2dv( &(m_CurEditTex->UVs[i][0]) );
// 		}
// 		glEnd();

	}

	void Model::ChangeCurEditTex( unsigned int tid )
	{
		if (tid < m_TexInfos.size())
			m_CurEditTex = m_TexInfos[tid];
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

void Tri_Mesh::Render_No_Lighting_Solid()
{
	FIter f_it;
	FVIter	fv_it;
	//glPushAttrib(GL_LIGHTING_BIT);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
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
	glEnable(GL_LIGHTING);
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
		_mesh->initQuadrices();
		_mesh->InitModelProperty();
	}
	return isRead;
}

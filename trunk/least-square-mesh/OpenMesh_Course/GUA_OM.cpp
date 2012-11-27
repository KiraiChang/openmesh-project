#include "stdafx.h"
#include "GUA_OM.h"
#include <algorithm>
#define PI 3.141592653589793
#define THRESHOLD 1.5


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
	}
	Model::~Model()
	{
		release_vertex_status();
		release_edge_status();
		release_face_status();
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

	void Model::Render_Least_Square_Solid(SELECT_CONTROL_POINT_TYPE type)
	{
		FIter f_it;
		FVIter	fv_it;
		glPushMatrix();
		float t = 1;
		if(type == eQuadricd)
			t = 2;
		//glPushAttrib(GL_LIGHTING_BIT);
		glTranslatef(t,0.0f, 0.0f);
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
				if(type == eQuadricd)
				{
					if(least_square_quadrics.count(fv_it.handle().idx()) > 0)
					{
						glNormal3dv(&normal(fv_it.handle())[0]);
						glVertex3dv(&least_square_quadrics[fv_it.handle().idx()][0]);
					}
				}
				else
				{
					if(least_square_random.count(fv_it.handle().idx()) > 0)
					{
						glNormal3dv(&normal(fv_it.handle())[0]);
						glVertex3dv(&least_square_random[fv_it.handle().idx()][0]);
					}
				}
			}
		}
		glEnd();	
		glPopMatrix();
		glDisable(GL_POLYGON_OFFSET_FILL);
	}

	void Model::Render_Least_Square_No_Lighting_Solid(SELECT_CONTROL_POINT_TYPE type)
	{
		FIter f_it;
		FVIter	fv_it;
		glPushMatrix();
		//glPushAttrib(GL_LIGHTING_BIT);
		float t = 1;
		if(type == eQuadricd)
			t = 2;
		glTranslatef(t, 0.0f, 0.0f);
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
				if(type == eQuadricd)
				{
					if(least_square_quadrics.count(fv_it.handle().idx()) > 0)
					{
						glNormal3dv(&normal(fv_it.handle())[0]);
						glVertex3dv(&least_square_quadrics[fv_it.handle().idx()][0]);
					}
				}
				else
				{
					if(least_square_random.count(fv_it.handle().idx()) > 0)
					{
						glNormal3dv(&normal(fv_it.handle())[0]);
						glVertex3dv(&least_square_random[fv_it.handle().idx()][0]);
					}
				}
			}
		}
		glEnd();	
		glPopMatrix();
		glDisable(GL_POLYGON_OFFSET_FILL);
		glEnable(GL_LIGHTING);
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

	void Model::clearBuffer(void)
	{
	}

	void Model::randomIndex(int control_number)
	{
		control_point_idx.clear();
		size_t n = n_vertices();
		int i;
		std::vector<int> idx;
		idx.resize(n);
		for(i = 0; i < n; i++)
		{
			idx[i] = i;
		}

		std::random_shuffle(idx.begin(), idx.end());
		for(int i = 0; i < control_number; i++)
		{
			control_point_idx.insert(idx[i]);
			//printf("index:%d, id:%d\n", i, idx[i]);
		}
	}

	void Model::quadricdIndex(int control_number)
	{
		control_point_idx.clear();
		selectPair();
		std::map< double, PAIR> errorQueue;
		std::map< double, PAIR>::reverse_iterator reve_ite;
		std::map< double, PAIR>::iterator ite;
		PAIR pair;
		for (ERRORS::iterator iter = errors.begin(); iter != errors.end(); iter++)
		{
			errorQueue.insert(std::map< double, PAIR>::value_type(iter->second, iter->first));
		}

		for (reve_ite = errorQueue.rbegin(); reve_ite != errorQueue.rend(); reve_ite++)
		{
			//printf("Error:%f\n", reve_ite->first);
			pair = reve_ite->second;
			//printf("min idx:%d, max idx:%d\n", pair.first, pair.second);
			control_point_idx.insert(pair.first);
			if(control_point_idx.size() >= control_number)
				return;
			control_point_idx.insert(pair.second);
			if(control_point_idx.size() >= control_number)
				return;
		}

		//for (ite = errorQueue.begin(); ite != errorQueue.end(); ite++)
		//{
		//	//printf("Error:%f\n", ite->first);
		//	pair = ite->second;
		//	//printf("min idx:%d, max idx:%d\n", pair.first, pair.second);
		//	control_point_idx.insert(pair.first);
		//	if(control_point_idx.size() >= control_number)
		//		return;
		//	control_point_idx.insert(pair.second);
		//	if(control_point_idx.size() >= control_number)
		//		return;
		//}
	}

	void Model::leastSquareMesh(int m, SELECT_CONTROL_POINT_TYPE type)
	{
		int i, cp_count, one_ring_count, cp_index;
		Point p;
		size_t n = n_vertices();
		if(m > n || m <= 0)
		{
			printf("Control Point(%d) less than all vertices size(%d)...\n", m, n);
			return ;
		}

		if(type == eQuadricd)
		{
			quadricdIndex(m);
			least_square_quadrics.clear();
			printf("Begin process Least Square Mesh (Quadricd) ...\n");
		}
		else
		{
			randomIndex(m);
			least_square_random.clear();
			printf("Begin process Least Square Mesh (Random) ...\n");
		}
		LinearSystemLib::GeneralSparseMatrix GA;//Ax=B;
		VIter v_ite;
		VVIter vv_ite;
		cp_count = 0;




		GA.Create(n + m, n);
		int dim = 3;
		double **B = new double * [dim];
		for(i = 0; i < dim; i++)
		{
			B[i] = new double [n+m];
			memset(B[i], 0, (n+m) * sizeof(double *));
		}

		for(v_ite = vertices_begin(); v_ite != vertices_end(); ++v_ite)
		{
			one_ring_count = 0;
			for(vv_ite = vv_iter(v_ite.handle());vv_ite;++vv_ite)
			{
				one_ring_count++;
			}

			float rate = -1.0 / one_ring_count;
			for(vv_ite = vv_iter(v_ite.handle());vv_ite;++vv_ite)
			{
				GA.SetElement( v_ite.handle().idx(), vv_ite.handle().idx(), rate );
			}
			GA.SetElement( v_ite.handle().idx(),  v_ite.handle().idx(), 1);

			if(control_point_idx.count(v_ite.handle().idx()) > 0)
			{
				cp_index = n + cp_count;
				GA.SetElement(cp_index ,  v_ite.handle().idx(), 1);
				p = point(v_ite.handle());
				B[0][cp_index] = p[0];
				B[1][cp_index] = p[1];
				B[2][cp_index] = p[2];
				cp_count ++;
			}
		}
		LinearSystemLib::SparseLinearSystem sls( new LinearSystemLib::StableSparseMatrix(GA), B, dim );
		try
		{
			// the solution will be stored here.
			double** x = 0; 

			// solve it !!
			bool result = LinearSystemLib::LeastSquareSparseLSSolver::GetInstance()->Solve( &sls, x );

			const int num_vars = sls.GetA()->GetNumCols();

			// output the solution..!!
			//-------------------------------------------------

			if( !result )
				cerr << "solve error!!! check your matrix." << endl;
			//else
			//{
			//	cout << "The solution is : " << endl;

			//	cout << "X = ";
			//	copy( &x[0][0], &x[0][num_vars], ostream_iterator<double>( cout, " " ) );
			//	cout << endl << "bye bye" << endl;
			//}

			// 須要自行 release solution memory, A，B 不用!!
			//-------------------------------------------------
			i = 0;
			for(v_ite = vertices_begin(); v_ite != vertices_end(); ++v_ite, i++)
			{
				//point(v_ite.handle()) = Point(x[0][i], x[1][i], x[2][i]); 
				if(type == eQuadricd)
				{
					least_square_quadrics.insert(LEAST_SQUARE_MESH::value_type(v_ite.handle().idx(), Point(x[0][i], x[1][i], x[2][i])));
				}
				else
				{
					least_square_random.insert(LEAST_SQUARE_MESH::value_type(v_ite.handle().idx(), Point(x[0][i], x[1][i], x[2][i])));
				}
			}
			for( int i = 0 ; i < dim ; ++i )
				delete[] x[i];
			delete[] x;
			x = 0;
		} 
		catch( exception e )
		{
			cerr << e.what() << endl;
		}
		printf("Least Square Mesh Ok...\n");

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
	}
	return isRead;
}

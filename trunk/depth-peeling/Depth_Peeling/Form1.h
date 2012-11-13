#pragma once


#include "DotNetUtilities.h"
#include "Mesh/GUA_OM.h"
#include "Mesh/DP.h"


Tri_Mesh *mesh;

DP_COM *dp_com ;

xform xf;
GLCamera camera;
float fov = 0.7f;

bool peeling_state ;


static const Mouse::button physical_to_logical_map[] = {
	Mouse::NONE, Mouse::ROTATE, Mouse::MOVEXY, Mouse::MOVEZ,
	Mouse::MOVEZ, Mouse::MOVEXY, Mouse::MOVEXY, Mouse::MOVEXY,
};

Mouse::button Mouse_State = Mouse::ROTATE;

#pragma comment(linker, "/subsystem:console /entry:WinMainCRTStartup")
namespace Depth_Peeling {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();

			point center;

			center[0] = 0.0 ;
			center[1] = 0.0 ;
			center[2] = 0.0 ;
			xf = xform::trans( 0, 0,  -3.5f / fov * 0.8 ) *
				xform::trans(center);

			mesh = NULL ;

			peeling_state = true ;
			dp_com = new DP_COM ;
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::GroupBox^  groupBox1;
	protected: 
	private: System::Windows::Forms::Button^  DepthPeel_button;
	private: System::Windows::Forms::Button^  OpenMesh_button;
	private: System::Windows::Forms::GroupBox^  groupBox2;
	private: HKOGLPanel::HKOGLPanelControl^  hkoglPanelControl1;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
	private: System::Windows::Forms::ListBox^  listSelectLayer;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			HKOGLPanel::HKCOGLPanelCameraSetting^  hkcoglPanelCameraSetting1 = (gcnew HKOGLPanel::HKCOGLPanelCameraSetting());
			HKOGLPanel::HKCOGLPanelPixelFormat^  hkcoglPanelPixelFormat1 = (gcnew HKOGLPanel::HKCOGLPanelPixelFormat());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->listSelectLayer = (gcnew System::Windows::Forms::ListBox());
			this->DepthPeel_button = (gcnew System::Windows::Forms::Button());
			this->OpenMesh_button = (gcnew System::Windows::Forms::Button());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->hkoglPanelControl1 = (gcnew HKOGLPanel::HKOGLPanelControl());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->groupBox1->SuspendLayout();
			this->groupBox2->SuspendLayout();
			this->SuspendLayout();
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->DepthPeel_button);
			this->groupBox1->Controls->Add(this->OpenMesh_button);
			this->groupBox1->Dock = System::Windows::Forms::DockStyle::Right;
			this->groupBox1->Location = System::Drawing::Point(715, 0);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(150, 720);
			this->groupBox1->TabIndex = 1;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Mesh_OP";
			// 
			// listSelectLayer
			// 
			this->listSelectLayer->BackColor = System::Drawing::SystemColors::InactiveCaption;
			this->listSelectLayer->FormattingEnabled = true;
			this->listSelectLayer->ItemHeight = 12;
			this->listSelectLayer->Location = System::Drawing::Point(352, 320);
			this->listSelectLayer->Name = L"listSelectLayer";
			this->listSelectLayer->Size = System::Drawing::Size(60, 16);
			this->listSelectLayer->TabIndex = 1;
			this->listSelectLayer->Visible = false;
			this->listSelectLayer->Click += gcnew System::EventHandler(this, &Form1::listSelectLayer_Click);
			// 
			// DepthPeel_button
			// 
			this->DepthPeel_button->Dock = System::Windows::Forms::DockStyle::Top;
			this->DepthPeel_button->Location = System::Drawing::Point(3, 61);
			this->DepthPeel_button->Name = L"DepthPeel_button";
			this->DepthPeel_button->Size = System::Drawing::Size(144, 43);
			this->DepthPeel_button->TabIndex = 1;
			this->DepthPeel_button->Text = L"Depth Peeling";
			this->DepthPeel_button->UseVisualStyleBackColor = true;
			this->DepthPeel_button->Click += gcnew System::EventHandler(this, &Form1::DepthPeel_button_Click);
			// 
			// OpenMesh_button
			// 
			this->OpenMesh_button->Dock = System::Windows::Forms::DockStyle::Top;
			this->OpenMesh_button->Location = System::Drawing::Point(3, 18);
			this->OpenMesh_button->Name = L"OpenMesh_button";
			this->OpenMesh_button->Size = System::Drawing::Size(144, 43);
			this->OpenMesh_button->TabIndex = 0;
			this->OpenMesh_button->Text = L"OpenMesh";
			this->OpenMesh_button->UseVisualStyleBackColor = true;
			this->OpenMesh_button->Click += gcnew System::EventHandler(this, &Form1::OpenMesh_button_Click);
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->listSelectLayer);
			this->groupBox2->Controls->Add(this->hkoglPanelControl1);
			this->groupBox2->Dock = System::Windows::Forms::DockStyle::Fill;
			this->groupBox2->Location = System::Drawing::Point(0, 0);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(715, 720);
			this->groupBox2->TabIndex = 2;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"OpenGL_Panel";
			// 
			// hkoglPanelControl1
			// 
			hkcoglPanelCameraSetting1->Far = 1000;
			hkcoglPanelCameraSetting1->Fov = 45;
			hkcoglPanelCameraSetting1->Near = -1000;
			hkcoglPanelCameraSetting1->Type = HKOGLPanel::HKCOGLPanelCameraSetting::CAMERATYPE::ORTHOGRAPHIC;
			this->hkoglPanelControl1->Camera_Setting = hkcoglPanelCameraSetting1;
			this->hkoglPanelControl1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->hkoglPanelControl1->Location = System::Drawing::Point(3, 18);
			this->hkoglPanelControl1->Name = L"hkoglPanelControl1";
			hkcoglPanelPixelFormat1->Accumu_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
			hkcoglPanelPixelFormat1->Alpha_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_32;
			hkcoglPanelPixelFormat1->Stencil_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
			this->hkoglPanelControl1->Pixel_Format = hkcoglPanelPixelFormat1;
			this->hkoglPanelControl1->Size = System::Drawing::Size(709, 699);
			this->hkoglPanelControl1->TabIndex = 0;
			this->hkoglPanelControl1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &Form1::hkoglPanelControl1_Paint);
			this->hkoglPanelControl1->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::hkoglPanelControl1_MouseDown);
			this->hkoglPanelControl1->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::hkoglPanelControl1_MouseMove);
			this->hkoglPanelControl1->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::hkoglPanelControl1_MouseWheel);
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			this->openFileDialog1->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &Form1::openFileDialog1_FileOk);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(865, 720);
			this->Controls->Add(this->groupBox2);
			this->Controls->Add(this->groupBox1);
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->groupBox1->ResumeLayout(false);
			this->groupBox2->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
private: System::Void hkoglPanelControl1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
			 glEnable(GL_COLOR_MATERIAL);
			 glClearColor( 1.0, 1.0, 1.0, 1.0);
			 glClear( GL_COLOR_BUFFER_BIT  );

			 point center;
			 center[0] = 0.0 ;
			 center[1] = 0.0 ;
			 center[2] = 0.0 ;
			 camera.setupGL( xf * center, 1.0 ) ;


			 glPushMatrix();
			 glMatrixMode(GL_MODELVIEW);
			 glMultMatrixd((double *)xf);

			 if ( mesh != NULL )
			 {
				 mesh->Render_Solid();
				 mesh->RenderSpecifiedPoint();
			 }

			 glPopMatrix();
		 }
private: System::Void hkoglPanelControl1_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 if (e->Button == System::Windows::Forms::MouseButtons::Left ||
				 e->Button == System::Windows::Forms::MouseButtons::Middle)
			 {
				 point center;
				 Mouse_State = Mouse::NONE;
				 center[0] = 0.0 ;
				 center[1] = 0.0 ;
				 center[2] = 0.0 ;				    
				 camera.mouse(e->X,e->Y, Mouse_State,
					 xf * center,
					 1.0, xf);
			 }

			 if(e->Button == System::Windows::Forms::MouseButtons::Right && peeling_state == false)
			 {
				 int y = hkoglPanelControl1->Height - e->Y;
				 std::cout<<"Depth:"<<dp_com->m_pZBuffer[ (y)* hkoglPanelControl1->Width +  e->X]<<std::endl;
				 VECTOR_LAYER vLayer;
				 dp_com->Check_Layers(e->X, y);
				 size_t count = dp_com->Get_Layer_Count() / 2;
				 if(count <= 0)
					 return;

				 this->listSelectLayer->Location = Point(e->X, e->Y);
				 this->listSelectLayer->Size = System::Drawing::Size(60, 12 * (count + 1));
				 //this->listSelectLayer->Location.X = e->X;
				 //this->listSelectLayer->Location.Y = e->Y;
				 this->listSelectLayer->Visible = true;
				 this->listSelectLayer->Items->Clear();
				 for(int i = 0; i < count; i++)
				 {
					 System::String^ str;
					 str = "Layer" + i;
					 this->listSelectLayer->Items->Add(str);
				 }
			 }
			 else if(e->Button == System::Windows::Forms::MouseButtons::Right)
			 {
				 GLint viewport[4];
				 GLdouble modelview[16];
				 GLdouble projection[16];
				 GLfloat winX, winY, winZ;
				 glPushMatrix();

				 glMatrixMode(GL_MODELVIEW);     glMultMatrixd((double *)xf);
				 glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
				 //printf("a11:%f,a12:%f, a13:%f, a14:%f\n", modelview[0], modelview[1], modelview[2], modelview[3]);
				 //printf("a21:%f,a22:%f, a23:%f, a24:%f\n", modelview[4], modelview[5], modelview[6], modelview[7]);
				 //printf("a31:%f,a32:%f, a33:%f, a34:%f\n", modelview[8], modelview[9], modelview[10], modelview[11]);
				 //printf("a41:%f,a42:%f, a43:%f, a44:%f\n", modelview[12], modelview[13], modelview[14], modelview[15]);
				 //printf(" \n");

				 glMatrixMode(GL_PROJECTION_MATRIX);     glMultMatrixd((double *)xf);
				 glGetDoublev( GL_PROJECTION_MATRIX, projection );
				 //printf("a11:%f,a12:%f, a13:%f, a14:%f\n", projection[0], projection[1], projection[2], projection[3]);
				 //printf("a21:%f,a22:%f, a23:%f, a24:%f\n", projection[4], projection[5], projection[6], projection[7]);
				 //printf("a31:%f,a32:%f, a33:%f, a34:%f\n", projection[8], projection[9], projection[10], projection[11]);
				 //printf("a41:%f,a42:%f, a43:%f, a44:%f\n", projection[12], projection[13], projection[14], projection[15]);
				 //printf(" \n");

				 glMatrixMode(GL_VIEWPORT); glMultMatrixd((double *)xf);
				 glGetIntegerv( GL_VIEWPORT, viewport );
				 //printf("x:%d, y:%d, z:%d, w:%d\n", viewport[0], viewport[1], viewport[2], viewport[3]);

				 winX = (float)e->X;
				 winY = (float)viewport[3] - (float)e->Y;
				 GLdouble objX, objY, objZ;
				 glEnable(GL_DEPTH_TEST);
				 glReadPixels( int(winX), int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
				 glDisable(GL_DEPTH_TEST);
				 printf("x:%f, y:%f, depth:%f\n", winX, winY, winZ);
				 gluUnProject( winX, winY, winZ, modelview, projection, viewport, &objX, &objY, &objZ);

				 mesh->clear_sp_p();
				 mesh->add_sp_p( OMT::MyMesh::Point(objX,objY,objZ), 1.0f, 0.0f, 1.0f);
				 printf("x:%f, y:%f, z:%f\n", objX, objY, objZ);
				 this->Refresh();
			 }
		 }
private: System::Void hkoglPanelControl1_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 if (e->Button == System::Windows::Forms::MouseButtons::Left)
			 { 
				 point center;
				 Mouse_State = Mouse::ROTATE;
				 center[0] = 0.0 ;
				 center[1] = 0.0 ;
				 center[2] = 0.0 ;				    
				 camera.mouse(e->X,e->Y, Mouse_State,
					 xf * center,
					 1.0, xf);
				 hkoglPanelControl1->Invalidate();
			 }

			 if (e->Button == System::Windows::Forms::MouseButtons::Middle)
			 { 
				 point center;
				 Mouse_State = Mouse::MOVEXY;
				 center[0] = 0.0 ;
				 center[1] = 0.0 ;
				 center[2] = 0.0 ;				    
				 camera.mouse(e->X,e->Y, Mouse_State,
					 xf * center,
					 1.0, xf);
				 hkoglPanelControl1->Invalidate();
			 }
		 }
private: System::Void OpenMesh_button_Click(System::Object^  sender, System::EventArgs^  e) {
			    openFileDialog1->ShowDialog();
		 }
private: System::Void openFileDialog1_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e) {
			 std::string filename;
			 MarshalString( openFileDialog1->FileName, filename );

			 if( mesh != NULL )
				 delete mesh ;

			 mesh = new Tri_Mesh;


			 if(ReadFile(  filename, mesh  ) )
				 std::cout<<filename<<std::endl;

			 hkoglPanelControl1->Invalidate();
		 }
private: System::Void hkoglPanelControl1_MouseWheel(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
		 {
			 if (e->Delta<0)
			 {
				 point center;
				 Mouse_State = Mouse::WHEELDOWN;
				 center[0] = 0.0 ;
				 center[1] = 0.0 ;
				 center[2] = 0.0 ;				    
				 camera.mouse(e->X,e->Y, Mouse_State,
					 xf * center,
					 1.0, xf);
				 hkoglPanelControl1->Invalidate();
			 }
			 else
			 {
				 point center;
				 Mouse_State = Mouse::WHEELUP;
				 center[0] = 0.0 ;
				 center[1] = 0.0 ;
				 center[2] = 0.0 ;				    
				 camera.mouse(e->X,e->Y, Mouse_State,
					 xf * center,
					 1.0, xf);
				 hkoglPanelControl1->Invalidate();
			 }
		 }
private: System::Void DepthPeel_button_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if(mesh == NULL)
				 return;

			 if ( peeling_state )
			 {
				 mesh->clear_sp_p();
				 glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				 glEnable(GL_BLEND);
				 glDisable( GL_DEPTH_TEST ) ;
				 peeling_state = false ;
				 if(  dp_com->Scene_Init() )
				 {
					 dp_com->Set_BufferObj( hkoglPanelControl1->Width, hkoglPanelControl1->Height );
					 //用for迴圈將每一層Depth Layer 抓出來


					 for(int l = 0; l < MAX_LAYERS ; l++ ) 
					 {		      
						 dp_com->Peeling_layer( hkoglPanelControl1->Width, hkoglPanelControl1->Height, l, mesh, camera, xf );
					 }

					 std::cout<<"Peeling Success....."<<std::endl;
				 }
				 else
				 {
					 std::cout<<"Peeling Fail.............WTX.............."<<std::endl;
				 }
			 } 
			 else
			 {
				 glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				 glDisable(GL_BLEND);
				 glEnable( GL_DEPTH_TEST ) ;
				 peeling_state = true ;
				 mesh->clear_sp_p();
			 }

			 glPolygonMode(  GL_FRONT, GL_FILL ) ;
			 glPolygonMode(  GL_BACK, GL_POINT ) ;
			 hkoglPanelControl1->Invalidate( );
		 }
private: System::Void listSelectLayer_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 this->listSelectLayer->Visible = false;
			 int index = this->listSelectLayer->SelectedIndex;
			 float in, out;
			 dp_com->Get_Depth(index, in, out);
			 float depth =  (in + out)/2;
			 printf("In:%f, Out:%f, Depth:%f\n", in, out, depth);
			 if(depth < 1.0)
			 {
				 GLint viewport[4];
				 GLdouble modelview[16];
				 GLdouble projection[16];
				 GLfloat winX, winY, winZ = depth;
				 glPushMatrix();

				 glMatrixMode(GL_MODELVIEW);     glMultMatrixd((double *)xf);
				 glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
				 //printf("a11:%f,a12:%f, a13:%f, a14:%f\n", modelview[0], modelview[1], modelview[2], modelview[3]);
				 //printf("a21:%f,a22:%f, a23:%f, a24:%f\n", modelview[4], modelview[5], modelview[6], modelview[7]);
				 //printf("a31:%f,a32:%f, a33:%f, a34:%f\n", modelview[8], modelview[9], modelview[10], modelview[11]);
				 //printf("a41:%f,a42:%f, a43:%f, a44:%f\n", modelview[12], modelview[13], modelview[14], modelview[15]);
				 //printf(" \n");


				 glMatrixMode(GL_PROJECTION_MATRIX);     glMultMatrixd((double *)xf);
				 glGetDoublev( GL_PROJECTION_MATRIX, projection );
				 //printf("a11:%f,a12:%f, a13:%f, a14:%f\n", projection[0], projection[1], projection[2], projection[3]);
				 //printf("a21:%f,a22:%f, a23:%f, a24:%f\n", projection[4], projection[5], projection[6], projection[7]);
				 //printf("a31:%f,a32:%f, a33:%f, a34:%f\n", projection[8], projection[9], projection[10], projection[11]);
				 //printf("a41:%f,a42:%f, a43:%f, a44:%f\n", projection[12], projection[13], projection[14], projection[15]);
				 //printf(" \n");


				 glMatrixMode(GL_VIEWPORT); glMultMatrixd((double *)xf);
				 glGetIntegerv( GL_VIEWPORT, viewport );
				 //printf("x:%d, y:%d, z:%d, w:%d\n", viewport[0], viewport[1], viewport[2], viewport[3]);

				 dp_com->Get_Pixel_Position(winX, winY);
				 //winY = (float)viewport[3] - winY;

				 GLdouble drawX, drawY, drawZ;
				 printf("x:%f,y:%f,depth:%f\n", winX, winY, winZ);
				 gluUnProject( winX, winY, winZ, modelview, projection, viewport, &drawX, &drawY, &drawZ);

				 if(drawZ == -1.f)
				 {
					 return;
				 }
				 mesh->add_sp_p( OMT::MyMesh::Point(drawX,drawY,drawZ), 0.0f, 0.0f, 1.0f);
				 printf("Point.x:%f,Point.y:%f,Point.z:%f\n", drawX, drawY, drawZ);

				 //printf("in : x:%f,y:%f,depth:%f\n", winX, winY, in);
				 //gluUnProject( winX, winY, in, modelview, projection, viewport, &drawX, &drawY, &drawZ);

				 //if(drawZ == -1.f)
				 //{
					// return;
				 //}
				 //mesh->add_sp_p( OMT::MyMesh::Point(drawX,drawY,drawZ), 1.0f, 0.0f, 0.0f);
				 //printf("In : Point.x:%f,Point.y:%f,Point.z:%f\n", drawX, drawY, drawZ);

				 //printf("Out : x:%f,y:%f,depth:%f\n", winX, winY, out);
				 //gluUnProject( winX, winY, out, modelview, projection, viewport, &drawX, &drawY, &drawZ);

				 //if(drawZ == -1.f)
				 //{
					// return;
				 //}
				 //mesh->add_sp_p( OMT::MyMesh::Point(drawX,drawY,drawZ), 1.0f, 0.0f, 0.0f);
				 //printf("Out : Point.x:%f,Point.y:%f,Point.z:%f\n", drawX, drawY, drawZ);
			 }
		 }
};
}


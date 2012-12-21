#pragma once


#include "DotNetUtilities.h"
#include "GUA_OM.h"
//#include <armadillo>

int g_mouse_x, g_mouse_y;
bool g_isSelected;

Tri_Mesh *mesh;
//static float fov = 0.7f;
//arma::mat xf;
namespace OpenMesh_Course {

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
			//xf.set_size(4, 4);
			//xf.eye();//identity
			//xf(0, 0) = 0.0;
			//xf(0, 1) = 0.0;
			//xf(0, 2) = 0.0;
			mesh = NULL ;
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
	private: HKOGLPanel::HKOGLPanelControl^  hkoglPanelControl1;
	private: System::Windows::Forms::GroupBox^  gpCommand;
	private: System::Windows::Forms::Button^  btnLoadMesh;
	private: System::Windows::Forms::OpenFileDialog^  openMeshFileDialog;











	private: System::Windows::Forms::GroupBox^  gbShowType;

	private: System::Windows::Forms::CheckBox^  cbWireframe;
	private: System::Windows::Forms::CheckBox^  cbLightingModel;















	private: HKOGLPanel::HKOGLPanelControl^  hkoglPanelControl2;
	private: System::Windows::Forms::Label^  lOutput;
	private: System::Windows::Forms::Button^  btnLoadTexture;
	private: System::Windows::Forms::OpenFileDialog^  openTextureFileDialog;
	private: System::Windows::Forms::TextBox^  textBoxRing;
	private: System::Windows::Forms::CheckBox^  cbRTM;
	private: System::Windows::Forms::TrackBar^  trackRadin;
	private: System::Windows::Forms::CheckBox^  cbSelected;




















	protected: 

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
			HKOGLPanel::HKCCameraProp^  hkcCameraProp3 = (gcnew HKOGLPanel::HKCCameraProp());
			HKOGLPanel::HKCDisplayProp^  hkcDisplayProp3 = (gcnew HKOGLPanel::HKCDisplayProp());
			HKOGLPanel::HKCPixelFormat^  hkcPixelFormat3 = (gcnew HKOGLPanel::HKCPixelFormat());
			HKOGLPanel::HKCTrackballProp^  hkcTrackballProp3 = (gcnew HKOGLPanel::HKCTrackballProp());
			HKOGLPanel::HKCTrackballTrig^  hkcTrackballTrig7 = (gcnew HKOGLPanel::HKCTrackballTrig());
			HKOGLPanel::HKCTrackballTrig^  hkcTrackballTrig8 = (gcnew HKOGLPanel::HKCTrackballTrig());
			HKOGLPanel::HKCTrackballTrig^  hkcTrackballTrig9 = (gcnew HKOGLPanel::HKCTrackballTrig());
			HKOGLPanel::HKCCameraProp^  hkcCameraProp4 = (gcnew HKOGLPanel::HKCCameraProp());
			HKOGLPanel::HKCDisplayProp^  hkcDisplayProp4 = (gcnew HKOGLPanel::HKCDisplayProp());
			HKOGLPanel::HKCPixelFormat^  hkcPixelFormat4 = (gcnew HKOGLPanel::HKCPixelFormat());
			HKOGLPanel::HKCTrackballProp^  hkcTrackballProp4 = (gcnew HKOGLPanel::HKCTrackballProp());
			HKOGLPanel::HKCTrackballTrig^  hkcTrackballTrig10 = (gcnew HKOGLPanel::HKCTrackballTrig());
			HKOGLPanel::HKCTrackballTrig^  hkcTrackballTrig11 = (gcnew HKOGLPanel::HKCTrackballTrig());
			HKOGLPanel::HKCTrackballTrig^  hkcTrackballTrig12 = (gcnew HKOGLPanel::HKCTrackballTrig());
			this->hkoglPanelControl1 = (gcnew HKOGLPanel::HKOGLPanelControl());
			this->gpCommand = (gcnew System::Windows::Forms::GroupBox());
			this->gbShowType = (gcnew System::Windows::Forms::GroupBox());
			this->cbSelected = (gcnew System::Windows::Forms::CheckBox());
			this->trackRadin = (gcnew System::Windows::Forms::TrackBar());
			this->cbRTM = (gcnew System::Windows::Forms::CheckBox());
			this->textBoxRing = (gcnew System::Windows::Forms::TextBox());
			this->lOutput = (gcnew System::Windows::Forms::Label());
			this->cbWireframe = (gcnew System::Windows::Forms::CheckBox());
			this->cbLightingModel = (gcnew System::Windows::Forms::CheckBox());
			this->btnLoadTexture = (gcnew System::Windows::Forms::Button());
			this->btnLoadMesh = (gcnew System::Windows::Forms::Button());
			this->openMeshFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());
			this->hkoglPanelControl2 = (gcnew HKOGLPanel::HKOGLPanelControl());
			this->openTextureFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());
			this->gpCommand->SuspendLayout();
			this->gbShowType->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackRadin))->BeginInit();
			this->SuspendLayout();
			// 
			// hkoglPanelControl1
			// 
			this->hkoglPanelControl1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left));
			hkcCameraProp3->Enabled = true;
			hkcCameraProp3->Far = 1000;
			hkcCameraProp3->Fov = 45;
			hkcCameraProp3->Near = 0.001;
			hkcCameraProp3->Type = HKOGLPanel::HKCCameraProp::CAMERATYPE::PERSPECTIVE;
			hkcCameraProp3->Zoom = 1;
			this->hkoglPanelControl1->Camera_Property = hkcCameraProp3;
			hkcDisplayProp3->Back_Color = System::Drawing::Color::White;
			hkcDisplayProp3->Grid_Axis = System::Drawing::Color::Black;
			hkcDisplayProp3->Grid_Draw = true;
			hkcDisplayProp3->Grid_Line = System::Drawing::Color::DimGray;
			this->hkoglPanelControl1->Display_Property = hkcDisplayProp3;
			this->hkoglPanelControl1->Empty_Panel = false;
			this->hkoglPanelControl1->Location = System::Drawing::Point(12, 12);
			this->hkoglPanelControl1->Name = L"hkoglPanelControl1";
			hkcPixelFormat3->Accumu_Buffer_Bits = HKOGLPanel::HKCPixelFormat::PIXELBITS::BITS_0;
			hkcPixelFormat3->Alpha_Buffer_Bits = HKOGLPanel::HKCPixelFormat::PIXELBITS::BITS_0;
			hkcPixelFormat3->Stencil_Buffer_Bits = HKOGLPanel::HKCPixelFormat::PIXELBITS::BITS_0;
			this->hkoglPanelControl1->Pixel_Format = hkcPixelFormat3;
			this->hkoglPanelControl1->Size = System::Drawing::Size(460, 467);
			this->hkoglPanelControl1->TabIndex = 0;
			hkcTrackballProp3->Const_Res = false;
			hkcTrackballProp3->Delta_Res = 500;
			hkcTrackballProp3->Enabled = true;
			hkcTrackballProp3->Move_Res = 1000;
			hkcTrackballTrig7->Key_Down = false;
			hkcTrackballTrig7->Keyboard = System::Windows::Forms::Keys::None;
			hkcTrackballTrig7->Mouse = System::Windows::Forms::MouseButtons::Right;
			hkcTrackballTrig7->Mouse_Down = false;
			hkcTrackballProp3->Move_Trigger = hkcTrackballTrig7;
			hkcTrackballTrig8->Key_Down = false;
			hkcTrackballTrig8->Keyboard = System::Windows::Forms::Keys::None;
			hkcTrackballTrig8->Mouse = System::Windows::Forms::MouseButtons::Left;
			hkcTrackballTrig8->Mouse_Down = false;
			hkcTrackballProp3->Rotate_Trigger = hkcTrackballTrig8;
			hkcTrackballProp3->Zoom_Res = 100;
			hkcTrackballTrig9->Key_Down = false;
			hkcTrackballTrig9->Keyboard = System::Windows::Forms::Keys::None;
			hkcTrackballTrig9->Mouse = System::Windows::Forms::MouseButtons::Middle;
			hkcTrackballTrig9->Mouse_Down = false;
			hkcTrackballProp3->Zoom_Trigger = hkcTrackballTrig9;
			this->hkoglPanelControl1->Trackball_Property = hkcTrackballProp3;
			this->hkoglPanelControl1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &Form1::hkoglPanelControl1_Paint);
			this->hkoglPanelControl1->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::hkoglPanelControl1_MouseDown);
			this->hkoglPanelControl1->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::hkoglPanelControl1_MouseMove);
			this->hkoglPanelControl1->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::hkoglPanelControl1_MouseUp);
			// 
			// gpCommand
			// 
			this->gpCommand->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->gpCommand->Controls->Add(this->gbShowType);
			this->gpCommand->Controls->Add(this->btnLoadTexture);
			this->gpCommand->Controls->Add(this->btnLoadMesh);
			this->gpCommand->Location = System::Drawing::Point(906, 0);
			this->gpCommand->Name = L"gpCommand";
			this->gpCommand->Size = System::Drawing::Size(174, 479);
			this->gpCommand->TabIndex = 2;
			this->gpCommand->TabStop = false;
			this->gpCommand->Text = L"Command";
			// 
			// gbShowType
			// 
			this->gbShowType->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->gbShowType->Controls->Add(this->cbSelected);
			this->gbShowType->Controls->Add(this->trackRadin);
			this->gbShowType->Controls->Add(this->cbRTM);
			this->gbShowType->Controls->Add(this->textBoxRing);
			this->gbShowType->Controls->Add(this->lOutput);
			this->gbShowType->Controls->Add(this->cbWireframe);
			this->gbShowType->Controls->Add(this->cbLightingModel);
			this->gbShowType->Location = System::Drawing::Point(6, 152);
			this->gbShowType->Name = L"gbShowType";
			this->gbShowType->Size = System::Drawing::Size(162, 321);
			this->gbShowType->TabIndex = 3;
			this->gbShowType->TabStop = false;
			this->gbShowType->Text = L"Show Type";
			// 
			// cbSelected
			// 
			this->cbSelected->AutoSize = true;
			this->cbSelected->Location = System::Drawing::Point(6, 88);
			this->cbSelected->Name = L"cbSelected";
			this->cbSelected->Size = System::Drawing::Size(75, 16);
			this->cbSelected->TabIndex = 8;
			this->cbSelected->Text = L"Select Face";
			this->cbSelected->UseVisualStyleBackColor = true;
			// 
			// trackRadin
			// 
			this->trackRadin->Location = System::Drawing::Point(36, 110);
			this->trackRadin->Name = L"trackRadin";
			this->trackRadin->Size = System::Drawing::Size(120, 45);
			this->trackRadin->TabIndex = 7;
			// 
			// cbRTM
			// 
			this->cbRTM->AutoSize = true;
			this->cbRTM->Location = System::Drawing::Point(6, 65);
			this->cbRTM->Name = L"cbRTM";
			this->cbRTM->Size = System::Drawing::Size(142, 16);
			this->cbRTM->TabIndex = 6;
			this->cbRTM->Text = L"Render Texture to Model";
			this->cbRTM->UseVisualStyleBackColor = true;
			// 
			// textBoxRing
			// 
			this->textBoxRing->Location = System::Drawing::Point(6, 110);
			this->textBoxRing->Name = L"textBoxRing";
			this->textBoxRing->Size = System::Drawing::Size(24, 22);
			this->textBoxRing->TabIndex = 5;
			this->textBoxRing->Text = L"10";
			// 
			// lOutput
			// 
			this->lOutput->AutoSize = true;
			this->lOutput->Location = System::Drawing::Point(6, 143);
			this->lOutput->Name = L"lOutput";
			this->lOutput->Size = System::Drawing::Size(24, 12);
			this->lOutput->TabIndex = 4;
			this->lOutput->Text = L"Log";
			// 
			// cbWireframe
			// 
			this->cbWireframe->AutoSize = true;
			this->cbWireframe->Checked = true;
			this->cbWireframe->CheckState = System::Windows::Forms::CheckState::Checked;
			this->cbWireframe->Location = System::Drawing::Point(6, 43);
			this->cbWireframe->Name = L"cbWireframe";
			this->cbWireframe->Size = System::Drawing::Size(103, 16);
			this->cbWireframe->TabIndex = 1;
			this->cbWireframe->Text = L"Show Wireframe";
			this->cbWireframe->UseVisualStyleBackColor = true;
			this->cbWireframe->CheckedChanged += gcnew System::EventHandler(this, &Form1::cbWireframe_CheckedChanged);
			// 
			// cbLightingModel
			// 
			this->cbLightingModel->AutoSize = true;
			this->cbLightingModel->Location = System::Drawing::Point(6, 21);
			this->cbLightingModel->Name = L"cbLightingModel";
			this->cbLightingModel->Size = System::Drawing::Size(126, 16);
			this->cbLightingModel->TabIndex = 0;
			this->cbLightingModel->Text = L"Show Lighting Model";
			this->cbLightingModel->UseVisualStyleBackColor = true;
			this->cbLightingModel->CheckedChanged += gcnew System::EventHandler(this, &Form1::cbModel_CheckedChanged);
			// 
			// btnLoadTexture
			// 
			this->btnLoadTexture->Location = System::Drawing::Point(6, 85);
			this->btnLoadTexture->Name = L"btnLoadTexture";
			this->btnLoadTexture->Size = System::Drawing::Size(162, 61);
			this->btnLoadTexture->TabIndex = 5;
			this->btnLoadTexture->Text = L"Load Texture";
			this->btnLoadTexture->UseVisualStyleBackColor = true;
			this->btnLoadTexture->Click += gcnew System::EventHandler(this, &Form1::btnLoadTexture_Click);
			// 
			// btnLoadMesh
			// 
			this->btnLoadMesh->Location = System::Drawing::Point(6, 21);
			this->btnLoadMesh->Name = L"btnLoadMesh";
			this->btnLoadMesh->Size = System::Drawing::Size(162, 58);
			this->btnLoadMesh->TabIndex = 0;
			this->btnLoadMesh->Text = L"Load Mesh";
			this->btnLoadMesh->UseVisualStyleBackColor = true;
			this->btnLoadMesh->Click += gcnew System::EventHandler(this, &Form1::btnLoadMesh_Click);
			// 
			// openMeshFileDialog
			// 
			this->openMeshFileDialog->DefaultExt = L"obj";
			this->openMeshFileDialog->Filter = L"Mesh File|*.obj|All file|*.*";
			this->openMeshFileDialog->Title = L"Open Mesh File";
			this->openMeshFileDialog->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &Form1::openMeshFileDialog_FileOk);
			// 
			// hkoglPanelControl2
			// 
			this->hkoglPanelControl2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			hkcCameraProp4->Enabled = true;
			hkcCameraProp4->Far = 1000;
			hkcCameraProp4->Fov = 45;
			hkcCameraProp4->Near = 0.001;
			hkcCameraProp4->Type = HKOGLPanel::HKCCameraProp::CAMERATYPE::PERSPECTIVE;
			hkcCameraProp4->Zoom = 1;
			this->hkoglPanelControl2->Camera_Property = hkcCameraProp4;
			hkcDisplayProp4->Back_Color = System::Drawing::Color::White;
			hkcDisplayProp4->Grid_Axis = System::Drawing::Color::Black;
			hkcDisplayProp4->Grid_Draw = true;
			hkcDisplayProp4->Grid_Line = System::Drawing::Color::DimGray;
			this->hkoglPanelControl2->Display_Property = hkcDisplayProp4;
			this->hkoglPanelControl2->Empty_Panel = false;
			this->hkoglPanelControl2->Location = System::Drawing::Point(478, 12);
			this->hkoglPanelControl2->Name = L"hkoglPanelControl2";
			hkcPixelFormat4->Accumu_Buffer_Bits = HKOGLPanel::HKCPixelFormat::PIXELBITS::BITS_0;
			hkcPixelFormat4->Alpha_Buffer_Bits = HKOGLPanel::HKCPixelFormat::PIXELBITS::BITS_0;
			hkcPixelFormat4->Stencil_Buffer_Bits = HKOGLPanel::HKCPixelFormat::PIXELBITS::BITS_0;
			this->hkoglPanelControl2->Pixel_Format = hkcPixelFormat4;
			this->hkoglPanelControl2->Size = System::Drawing::Size(422, 467);
			this->hkoglPanelControl2->TabIndex = 10;
			hkcTrackballProp4->Const_Res = false;
			hkcTrackballProp4->Delta_Res = 500;
			hkcTrackballProp4->Enabled = true;
			hkcTrackballProp4->Move_Res = 1000;
			hkcTrackballTrig10->Key_Down = false;
			hkcTrackballTrig10->Keyboard = System::Windows::Forms::Keys::None;
			hkcTrackballTrig10->Mouse = System::Windows::Forms::MouseButtons::Right;
			hkcTrackballTrig10->Mouse_Down = false;
			hkcTrackballProp4->Move_Trigger = hkcTrackballTrig10;
			hkcTrackballTrig11->Key_Down = false;
			hkcTrackballTrig11->Keyboard = System::Windows::Forms::Keys::None;
			hkcTrackballTrig11->Mouse = System::Windows::Forms::MouseButtons::Left;
			hkcTrackballTrig11->Mouse_Down = false;
			hkcTrackballProp4->Rotate_Trigger = hkcTrackballTrig11;
			hkcTrackballProp4->Zoom_Res = 100;
			hkcTrackballTrig12->Key_Down = false;
			hkcTrackballTrig12->Keyboard = System::Windows::Forms::Keys::None;
			hkcTrackballTrig12->Mouse = System::Windows::Forms::MouseButtons::Middle;
			hkcTrackballTrig12->Mouse_Down = false;
			hkcTrackballProp4->Zoom_Trigger = hkcTrackballTrig12;
			this->hkoglPanelControl2->Trackball_Property = hkcTrackballProp4;
			this->hkoglPanelControl2->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &Form1::hkoglPanelControl2_Paint);
			this->hkoglPanelControl2->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::hkoglPanelControl2_MouseDown);
			// 
			// openTextureFileDialog
			// 
			this->openTextureFileDialog->DefaultExt = L"JPG";
			this->openTextureFileDialog->Filter = L"JPG File|*.JPG|BMP File|*.BMP|All file|*.*";
			this->openTextureFileDialog->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &Form1::openTextureFileDialog_FileOk);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1092, 491);
			this->Controls->Add(this->hkoglPanelControl2);
			this->Controls->Add(this->hkoglPanelControl1);
			this->Controls->Add(this->gpCommand);
			this->Name = L"Form1";
			this->Text = L"Digital Mesh";
			this->gpCommand->ResumeLayout(false);
			this->gbShowType->ResumeLayout(false);
			this->gbShowType->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackRadin))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
private: System::Void hkoglPanelControl1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) 
		 {
			 if ( mesh != NULL/* && cbLightingModel->Checked*/)
				 mesh->Render_Solid();
			 //else
				// mesh->Render_No_Lighting_Solid();
			 if ( mesh != NULL && cbWireframe->Checked)
				 mesh->Render_Wireframe();
			 if(mesh)
			 {
				 //if(g_isSelected)
				 //{
					// mesh->clear_sp_mapping();
					// mesh->clear_mapping_face();
					// mesh->clear_sp_f();
					// mesh->drawRect(g_winX, g_winY, 40, 40, g_viewport, g_modelview, g_projection);
					// mesh->selectFace(g_winX, g_winY, 20, 20, 20, 20, g_viewport, g_modelview, g_projection);
				 //}
				 mesh->RenderSpecifiedPoint();
				 mesh->RenderSpecifiedVertex();
				 mesh->RenderSpecifiedFace();
				 mesh->RenderSpecifiedEdge();
				 if (cbRTM->Checked)
					 mesh->RenderTextureToModel();
				 if(cbSelected->Checked)
				 {
					 float value = (float)trackRadin->Value / trackRadin->Maximum * 0.5;
					 mesh->drawCircle(g_mouse_x, g_mouse_y, value * 0.5, 1.0, 0.0, 0.0, 32);
					 mesh->SelectNring(100, g_mouse_x, g_mouse_y, value);
					 mesh->RenderSelectFace();
				 }
			 }
		 }
private: System::Void btnLoadMesh_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 openMeshFileDialog->ShowDialog();
		 }
private: System::Void openMeshFileDialog_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e) 
		 {
			 std::string filename;
			 MarshalString( openMeshFileDialog->FileName, filename );

			 if( mesh != NULL )
				 delete mesh ;

			 mesh = new Tri_Mesh;


			 if(ReadFile(  filename, mesh  ) )
				 std::cout<<filename<<std::endl;

			 hkoglPanelControl1->Invalidate();
		 }
private: System::Void cbModel_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			 hkoglPanelControl1->Refresh();
		 }
private: System::Void cbWireframe_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			 hkoglPanelControl1->Refresh();
		 }
private: System::Void cbAxis_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			 hkoglPanelControl1->Refresh();
		 }
private: System::Void hkoglPanelControl1_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
		 {
			 if(e->Button == System::Windows::Forms::MouseButtons::Right)
			 {
				 if(mesh)
				 {
					 GLint viewport[4];
					 GLdouble modelview[16];
					 GLdouble projection[16];
					 GLfloat winX, winY, winZ;
					 GLdouble objX, objY, objZ;
					 objX = objY = objZ = 0.0;
					 glPushMatrix();

					 glMatrixMode(GL_MODELVIEW);	//glMultMatrixd(xf.memptr());
					 glGetDoublev( GL_MODELVIEW_MATRIX, modelview );

					 glMatrixMode(GL_PROJECTION_MATRIX);	//glMultMatrixd(xf.memptr());
					 glGetDoublev( GL_PROJECTION_MATRIX, projection );

					 glMatrixMode(GL_VIEWPORT); //glMultMatrixd(xf.memptr());
					 glGetIntegerv( GL_VIEWPORT, viewport );

					 winX = (float)e->X;
					 winY = (float)viewport[3] - (float)e->Y;

					 glReadPixels( int(winX), int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

					 if(winZ>=0.99999f)
					 {
						 std::cerr << "Click on background (z= " << winZ << ")" << std::endl;
						 glPopMatrix();
						 return;
					 }
					 gluUnProject( winX, winY, winZ, modelview, projection, viewport, &objX, &objY, &objZ);

					 GLdouble orgX, orgY, orgZ;
					 gluUnProject( winX, winY, 0, modelview, projection, viewport, &orgX, &orgY, &orgZ);

					 lOutput->Text = "ObjectX: "+objX+"\nObjectY: "+objY+"\nObjectZ: "+(objZ);
					 glPopMatrix();


					 mesh->clear_sp_p();
					 mesh->clear_sp_v();
					 mesh->clear_sp_f();
					 mesh->clear_sp_e();
					 mesh->clear_sp_mapping();
					 //加入目前的滑鼠點
					 OMT::Point mouse(objX,objY,objZ);
					 mesh->add_sp_p(mouse, 1.0f, 0.0f, 1.0f);
					 //OMT::FHandle handle;
					 //if(mesh->findFace(mouse, handle) < INIT_DIST)
						//mesh->VertexMapping(handle);
					 //g_isSelected = true;


					 OMP::MyMesh::Point orgPoint(orgX, orgY, orgZ);
					 OMP::Vector3d rayDir = mouse - orgPoint;
					 rayDir = rayDir.normalize();
					 //FaceOneRing(orgPoint, rayDir);
					 mesh->SelectNring(Convert::ToInt32(textBoxRing->Text), orgPoint, rayDir);

					 this->Refresh();
				 }
			 }
			 else if(e->Button == System::Windows::Forms::MouseButtons::Left)
			 {
				 if(cbSelected->Checked)
				 {
					 cbSelected->Checked = false;
					 if(mesh)
						 mesh->Selected();
					 Refresh();
				 }
			 }
		 }
private: System::Void hkoglPanelControl2_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) 
		 {
			 if(mesh)
			 {
				 mesh->RenderTexture();
				 //mesh->RenderUVMapping();
				 mesh->RenderBound2D(1.0, 0.0, 0.0);
			 }

		 }
private: System::Void hkoglPanelControl2_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
		 {
			 switch(e->Button)
			 {
			 case System::Windows::Forms::MouseButtons::Right:
				 break;
			 case System::Windows::Forms::MouseButtons::Left:
				 break;
			 case System::Windows::Forms::MouseButtons::Middle:
				 break;
			 default:
				 break;
			 }
		 }
private: System::Void openTextureFileDialog_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e) 
		 {
			 if(mesh)
			 {
				 std::string filename;
				 MarshalString(openTextureFileDialog->FileName, filename );
				 //hkoglPanelControl1->Make_Current();
				 //mesh->LoadGLTextures(filename);
				 //hkoglPanelControl2->Make_Current();
				 //mesh->LoadGLTextures(filename);

				 mesh->LoadImage(filename, 0);

				 hkoglPanelControl1->Make_Current();
				 mesh->GenTextures(0, 0);
				 hkoglPanelControl2->Make_Current();
				 mesh->GenTextures(0, 0);
				 Refresh();
			 }
		 }
private: System::Void btnLoadTexture_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 openTextureFileDialog->ShowDialog();
		 }
private: System::Void hkoglPanelControl1_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
		 {
			 g_mouse_x = e->X;
			 g_mouse_y = e->Y;
			 lOutput->Text = "X: "+g_mouse_x+" Y: "+g_mouse_y;
			 Refresh();
		 }
private: System::Void hkoglPanelControl1_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
		 {
			 if(e->Button == System::Windows::Forms::MouseButtons::Right)
			 {
				 g_isSelected = false;
			 }
		 }
};
}


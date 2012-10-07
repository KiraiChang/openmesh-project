#pragma once


#include "DotNetUtilities.h"
#include "GUA_OM.h"
//#include <armadillo>

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




	private: System::Windows::Forms::GroupBox^  gpSelectType;

	private: System::Windows::Forms::RadioButton^  rbSelectFace;

	private: System::Windows::Forms::RadioButton^  rbSelectEdge;

	private: System::Windows::Forms::RadioButton^  rbSelectVertex;
	private: System::Windows::Forms::GroupBox^  gbShowType;
	private: System::Windows::Forms::CheckBox^  cbAxis;
	private: System::Windows::Forms::CheckBox^  cbWireframe;
	private: System::Windows::Forms::CheckBox^  cbModel;
	private: System::Windows::Forms::Label^  lOutput;





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
			HKOGLPanel::HKCCameraProp^  hkcCameraProp1 = (gcnew HKOGLPanel::HKCCameraProp());
			HKOGLPanel::HKCDisplayProp^  hkcDisplayProp1 = (gcnew HKOGLPanel::HKCDisplayProp());
			HKOGLPanel::HKCPixelFormat^  hkcPixelFormat1 = (gcnew HKOGLPanel::HKCPixelFormat());
			HKOGLPanel::HKCTrackballProp^  hkcTrackballProp1 = (gcnew HKOGLPanel::HKCTrackballProp());
			HKOGLPanel::HKCTrackballTrig^  hkcTrackballTrig1 = (gcnew HKOGLPanel::HKCTrackballTrig());
			HKOGLPanel::HKCTrackballTrig^  hkcTrackballTrig2 = (gcnew HKOGLPanel::HKCTrackballTrig());
			HKOGLPanel::HKCTrackballTrig^  hkcTrackballTrig3 = (gcnew HKOGLPanel::HKCTrackballTrig());
			this->hkoglPanelControl1 = (gcnew HKOGLPanel::HKOGLPanelControl());
			this->gpCommand = (gcnew System::Windows::Forms::GroupBox());
			this->gbShowType = (gcnew System::Windows::Forms::GroupBox());
			this->cbAxis = (gcnew System::Windows::Forms::CheckBox());
			this->cbWireframe = (gcnew System::Windows::Forms::CheckBox());
			this->cbModel = (gcnew System::Windows::Forms::CheckBox());
			this->gpSelectType = (gcnew System::Windows::Forms::GroupBox());
			this->rbSelectFace = (gcnew System::Windows::Forms::RadioButton());
			this->rbSelectEdge = (gcnew System::Windows::Forms::RadioButton());
			this->rbSelectVertex = (gcnew System::Windows::Forms::RadioButton());
			this->btnLoadMesh = (gcnew System::Windows::Forms::Button());
			this->openMeshFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());
			this->lOutput = (gcnew System::Windows::Forms::Label());
			this->gpCommand->SuspendLayout();
			this->gbShowType->SuspendLayout();
			this->gpSelectType->SuspendLayout();
			this->SuspendLayout();
			// 
			// hkoglPanelControl1
			// 
			hkcCameraProp1->Enabled = true;
			hkcCameraProp1->Far = 1000;
			hkcCameraProp1->Fov = 45;
			hkcCameraProp1->Near = 0.001;
			hkcCameraProp1->Type = HKOGLPanel::HKCCameraProp::CAMERATYPE::PERSPECTIVE;
			hkcCameraProp1->Zoom = 1;
			this->hkoglPanelControl1->Camera_Property = hkcCameraProp1;
			hkcDisplayProp1->Back_Color = System::Drawing::Color::White;
			hkcDisplayProp1->Grid_Axis = System::Drawing::Color::Black;
			hkcDisplayProp1->Grid_Draw = true;
			hkcDisplayProp1->Grid_Line = System::Drawing::Color::DimGray;
			this->hkoglPanelControl1->Display_Property = hkcDisplayProp1;
			this->hkoglPanelControl1->Dock = System::Windows::Forms::DockStyle::Left;
			this->hkoglPanelControl1->Empty_Panel = false;
			this->hkoglPanelControl1->Location = System::Drawing::Point(0, 0);
			this->hkoglPanelControl1->Name = L"hkoglPanelControl1";
			hkcPixelFormat1->Accumu_Buffer_Bits = HKOGLPanel::HKCPixelFormat::PIXELBITS::BITS_0;
			hkcPixelFormat1->Alpha_Buffer_Bits = HKOGLPanel::HKCPixelFormat::PIXELBITS::BITS_0;
			hkcPixelFormat1->Stencil_Buffer_Bits = HKOGLPanel::HKCPixelFormat::PIXELBITS::BITS_0;
			this->hkoglPanelControl1->Pixel_Format = hkcPixelFormat1;
			this->hkoglPanelControl1->Size = System::Drawing::Size(800, 835);
			this->hkoglPanelControl1->TabIndex = 0;
			hkcTrackballProp1->Const_Res = false;
			hkcTrackballProp1->Delta_Res = 500;
			hkcTrackballProp1->Enabled = true;
			hkcTrackballProp1->Move_Res = 1000;
			hkcTrackballTrig1->Key_Down = false;
			hkcTrackballTrig1->Keyboard = System::Windows::Forms::Keys::None;
			hkcTrackballTrig1->Mouse = System::Windows::Forms::MouseButtons::Right;
			hkcTrackballTrig1->Mouse_Down = false;
			hkcTrackballProp1->Move_Trigger = hkcTrackballTrig1;
			hkcTrackballTrig2->Key_Down = false;
			hkcTrackballTrig2->Keyboard = System::Windows::Forms::Keys::None;
			hkcTrackballTrig2->Mouse = System::Windows::Forms::MouseButtons::Left;
			hkcTrackballTrig2->Mouse_Down = false;
			hkcTrackballProp1->Rotate_Trigger = hkcTrackballTrig2;
			hkcTrackballProp1->Zoom_Res = 100;
			hkcTrackballTrig3->Key_Down = false;
			hkcTrackballTrig3->Keyboard = System::Windows::Forms::Keys::None;
			hkcTrackballTrig3->Mouse = System::Windows::Forms::MouseButtons::Middle;
			hkcTrackballTrig3->Mouse_Down = false;
			hkcTrackballProp1->Zoom_Trigger = hkcTrackballTrig3;
			this->hkoglPanelControl1->Trackball_Property = hkcTrackballProp1;
			this->hkoglPanelControl1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &Form1::hkoglPanelControl1_Paint);
			this->hkoglPanelControl1->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::hkoglPanelControl1_MouseDown);
			// 
			// gpCommand
			// 
			this->gpCommand->Controls->Add(this->lOutput);
			this->gpCommand->Controls->Add(this->gbShowType);
			this->gpCommand->Controls->Add(this->gpSelectType);
			this->gpCommand->Controls->Add(this->btnLoadMesh);
			this->gpCommand->Dock = System::Windows::Forms::DockStyle::Top;
			this->gpCommand->Location = System::Drawing::Point(800, 0);
			this->gpCommand->Name = L"gpCommand";
			this->gpCommand->Size = System::Drawing::Size(184, 835);
			this->gpCommand->TabIndex = 2;
			this->gpCommand->TabStop = false;
			this->gpCommand->Text = L"Command";
			// 
			// gbShowType
			// 
			this->gbShowType->Controls->Add(this->cbAxis);
			this->gbShowType->Controls->Add(this->cbWireframe);
			this->gbShowType->Controls->Add(this->cbModel);
			this->gbShowType->Location = System::Drawing::Point(19, 102);
			this->gbShowType->Name = L"gbShowType";
			this->gbShowType->Size = System::Drawing::Size(153, 93);
			this->gbShowType->TabIndex = 3;
			this->gbShowType->TabStop = false;
			this->gbShowType->Text = L"Show Type";
			// 
			// cbAxis
			// 
			this->cbAxis->AutoSize = true;
			this->cbAxis->Location = System::Drawing::Point(6, 65);
			this->cbAxis->Name = L"cbAxis";
			this->cbAxis->Size = System::Drawing::Size(74, 16);
			this->cbAxis->TabIndex = 2;
			this->cbAxis->Text = L"Show Axis";
			this->cbAxis->UseVisualStyleBackColor = true;
			this->cbAxis->CheckedChanged += gcnew System::EventHandler(this, &Form1::cbAxis_CheckedChanged);
			// 
			// cbWireframe
			// 
			this->cbWireframe->AutoSize = true;
			this->cbWireframe->Location = System::Drawing::Point(6, 43);
			this->cbWireframe->Name = L"cbWireframe";
			this->cbWireframe->Size = System::Drawing::Size(103, 16);
			this->cbWireframe->TabIndex = 1;
			this->cbWireframe->Text = L"Show Wireframe";
			this->cbWireframe->UseVisualStyleBackColor = true;
			this->cbWireframe->CheckedChanged += gcnew System::EventHandler(this, &Form1::cbWireframe_CheckedChanged);
			// 
			// cbModel
			// 
			this->cbModel->AutoSize = true;
			this->cbModel->Location = System::Drawing::Point(6, 21);
			this->cbModel->Name = L"cbModel";
			this->cbModel->Size = System::Drawing::Size(83, 16);
			this->cbModel->TabIndex = 0;
			this->cbModel->Text = L"Show Model";
			this->cbModel->UseVisualStyleBackColor = true;
			this->cbModel->CheckedChanged += gcnew System::EventHandler(this, &Form1::cbModel_CheckedChanged);
			// 
			// gpSelectType
			// 
			this->gpSelectType->Controls->Add(this->rbSelectFace);
			this->gpSelectType->Controls->Add(this->rbSelectEdge);
			this->gpSelectType->Controls->Add(this->rbSelectVertex);
			this->gpSelectType->Location = System::Drawing::Point(19, 210);
			this->gpSelectType->Name = L"gpSelectType";
			this->gpSelectType->Size = System::Drawing::Size(153, 93);
			this->gpSelectType->TabIndex = 1;
			this->gpSelectType->TabStop = false;
			this->gpSelectType->Text = L"Select Type";
			// 
			// rbSelectFace
			// 
			this->rbSelectFace->AutoSize = true;
			this->rbSelectFace->Location = System::Drawing::Point(7, 66);
			this->rbSelectFace->Name = L"rbSelectFace";
			this->rbSelectFace->Size = System::Drawing::Size(74, 16);
			this->rbSelectFace->TabIndex = 2;
			this->rbSelectFace->TabStop = true;
			this->rbSelectFace->Text = L"Select Face";
			this->rbSelectFace->UseVisualStyleBackColor = true;
			// 
			// rbSelectEdge
			// 
			this->rbSelectEdge->AutoSize = true;
			this->rbSelectEdge->Location = System::Drawing::Point(6, 44);
			this->rbSelectEdge->Name = L"rbSelectEdge";
			this->rbSelectEdge->Size = System::Drawing::Size(77, 16);
			this->rbSelectEdge->TabIndex = 1;
			this->rbSelectEdge->TabStop = true;
			this->rbSelectEdge->Text = L"Select Edge";
			this->rbSelectEdge->UseVisualStyleBackColor = true;
			// 
			// rbSelectVertex
			// 
			this->rbSelectVertex->AutoSize = true;
			this->rbSelectVertex->Location = System::Drawing::Point(7, 22);
			this->rbSelectVertex->Name = L"rbSelectVertex";
			this->rbSelectVertex->Size = System::Drawing::Size(84, 16);
			this->rbSelectVertex->TabIndex = 0;
			this->rbSelectVertex->TabStop = true;
			this->rbSelectVertex->Text = L"Select Vertex";
			this->rbSelectVertex->UseVisualStyleBackColor = true;
			// 
			// btnLoadMesh
			// 
			this->btnLoadMesh->Dock = System::Windows::Forms::DockStyle::Top;
			this->btnLoadMesh->Location = System::Drawing::Point(3, 18);
			this->btnLoadMesh->Name = L"btnLoadMesh";
			this->btnLoadMesh->Size = System::Drawing::Size(178, 78);
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
			// lOutput
			// 
			this->lOutput->AutoSize = true;
			this->lOutput->Location = System::Drawing::Point(19, 310);
			this->lOutput->Name = L"lOutput";
			this->lOutput->Size = System::Drawing::Size(0, 12);
			this->lOutput->TabIndex = 4;
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(984, 835);
			this->Controls->Add(this->gpCommand);
			this->Controls->Add(this->hkoglPanelControl1);
			this->Name = L"Form1";
			this->Text = L"Digital Mesh";
			this->gpCommand->ResumeLayout(false);
			this->gpCommand->PerformLayout();
			this->gbShowType->ResumeLayout(false);
			this->gbShowType->PerformLayout();
			this->gpSelectType->ResumeLayout(false);
			this->gpSelectType->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion
private: System::Void hkoglPanelControl1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) 
		 {
			 if ( mesh != NULL && cbModel->Checked)
				 mesh->Render_Solid();
			 if ( mesh != NULL && cbWireframe->Checked)
				 mesh->Render_Wireframe();
			 if(mesh)
			 {
				 mesh->RenderSpecifiedPoint();
				 mesh->RenderSpecifiedVertex();
				 mesh->RenderSpecifiedFace();
				 mesh->RenderSpecifiedEdge();
			 }
		 }
private: System::Void btnLoadMesh_Click(System::Object^  sender, System::EventArgs^  e) {
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

				 lOutput->Text = "ObjectX: "+objX+"\nObjectY: "+objY+"\nObjectZ: "+(objZ);
				 glPopMatrix();


				 mesh->clear_sp_p();
				 mesh->clear_sp_v();
				 mesh->clear_sp_f();
				 mesh->clear_sp_e();
				 //加入目前的滑鼠點
				 mesh->add_sp_p( OMT::MyMesh::Point(objX,objY,objZ), 1.0f, 0.0f, 1.0f);

				 if(rbSelectVertex->Checked)
				 {

				 }
				 else if(rbSelectEdge->Checked)
				 {

				 }
				 else if(rbSelectFace->Checked)
				 {

				 }
				 this->Refresh();
			 }
		 }
};
}


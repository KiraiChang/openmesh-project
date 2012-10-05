#pragma once


#include "DotNetUtilities.h"
#include "GUA_OM.h"

Tri_Mesh *mesh;
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
			HKOGLPanel::HKCCameraProp^  hkcCameraProp6 = (gcnew HKOGLPanel::HKCCameraProp());
			HKOGLPanel::HKCDisplayProp^  hkcDisplayProp6 = (gcnew HKOGLPanel::HKCDisplayProp());
			HKOGLPanel::HKCPixelFormat^  hkcPixelFormat6 = (gcnew HKOGLPanel::HKCPixelFormat());
			HKOGLPanel::HKCTrackballProp^  hkcTrackballProp6 = (gcnew HKOGLPanel::HKCTrackballProp());
			HKOGLPanel::HKCTrackballTrig^  hkcTrackballTrig16 = (gcnew HKOGLPanel::HKCTrackballTrig());
			HKOGLPanel::HKCTrackballTrig^  hkcTrackballTrig17 = (gcnew HKOGLPanel::HKCTrackballTrig());
			HKOGLPanel::HKCTrackballTrig^  hkcTrackballTrig18 = (gcnew HKOGLPanel::HKCTrackballTrig());
			this->hkoglPanelControl1 = (gcnew HKOGLPanel::HKOGLPanelControl());
			this->gpCommand = (gcnew System::Windows::Forms::GroupBox());
			this->gpSelectType = (gcnew System::Windows::Forms::GroupBox());
			this->rbSelectFace = (gcnew System::Windows::Forms::RadioButton());
			this->rbSelectEdge = (gcnew System::Windows::Forms::RadioButton());
			this->rbSelectVertex = (gcnew System::Windows::Forms::RadioButton());
			this->btnLoadMesh = (gcnew System::Windows::Forms::Button());
			this->openMeshFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());
			this->gpCommand->SuspendLayout();
			this->gpSelectType->SuspendLayout();
			this->SuspendLayout();
			// 
			// hkoglPanelControl1
			// 
			hkcCameraProp6->Enabled = true;
			hkcCameraProp6->Far = 1000;
			hkcCameraProp6->Fov = 45;
			hkcCameraProp6->Near = 0.001;
			hkcCameraProp6->Type = HKOGLPanel::HKCCameraProp::CAMERATYPE::PERSPECTIVE;
			hkcCameraProp6->Zoom = 1;
			this->hkoglPanelControl1->Camera_Property = hkcCameraProp6;
			hkcDisplayProp6->Back_Color = System::Drawing::Color::White;
			hkcDisplayProp6->Grid_Axis = System::Drawing::Color::Black;
			hkcDisplayProp6->Grid_Draw = true;
			hkcDisplayProp6->Grid_Line = System::Drawing::Color::DimGray;
			this->hkoglPanelControl1->Display_Property = hkcDisplayProp6;
			this->hkoglPanelControl1->Dock = System::Windows::Forms::DockStyle::Left;
			this->hkoglPanelControl1->Empty_Panel = false;
			this->hkoglPanelControl1->Location = System::Drawing::Point(0, 0);
			this->hkoglPanelControl1->Name = L"hkoglPanelControl1";
			hkcPixelFormat6->Accumu_Buffer_Bits = HKOGLPanel::HKCPixelFormat::PIXELBITS::BITS_0;
			hkcPixelFormat6->Alpha_Buffer_Bits = HKOGLPanel::HKCPixelFormat::PIXELBITS::BITS_0;
			hkcPixelFormat6->Stencil_Buffer_Bits = HKOGLPanel::HKCPixelFormat::PIXELBITS::BITS_0;
			this->hkoglPanelControl1->Pixel_Format = hkcPixelFormat6;
			this->hkoglPanelControl1->Size = System::Drawing::Size(800, 835);
			this->hkoglPanelControl1->TabIndex = 0;
			hkcTrackballProp6->Const_Res = false;
			hkcTrackballProp6->Delta_Res = 500;
			hkcTrackballProp6->Enabled = true;
			hkcTrackballProp6->Move_Res = 1000;
			hkcTrackballTrig16->Key_Down = false;
			hkcTrackballTrig16->Keyboard = System::Windows::Forms::Keys::None;
			hkcTrackballTrig16->Mouse = System::Windows::Forms::MouseButtons::Right;
			hkcTrackballTrig16->Mouse_Down = false;
			hkcTrackballProp6->Move_Trigger = hkcTrackballTrig16;
			hkcTrackballTrig17->Key_Down = false;
			hkcTrackballTrig17->Keyboard = System::Windows::Forms::Keys::None;
			hkcTrackballTrig17->Mouse = System::Windows::Forms::MouseButtons::Left;
			hkcTrackballTrig17->Mouse_Down = false;
			hkcTrackballProp6->Rotate_Trigger = hkcTrackballTrig17;
			hkcTrackballProp6->Zoom_Res = 100;
			hkcTrackballTrig18->Key_Down = false;
			hkcTrackballTrig18->Keyboard = System::Windows::Forms::Keys::None;
			hkcTrackballTrig18->Mouse = System::Windows::Forms::MouseButtons::Middle;
			hkcTrackballTrig18->Mouse_Down = false;
			hkcTrackballProp6->Zoom_Trigger = hkcTrackballTrig18;
			this->hkoglPanelControl1->Trackball_Property = hkcTrackballProp6;
			this->hkoglPanelControl1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &Form1::hkoglPanelControl1_Paint);
			// 
			// gpCommand
			// 
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
			// gpSelectType
			// 
			this->gpSelectType->Controls->Add(this->rbSelectFace);
			this->gpSelectType->Controls->Add(this->rbSelectEdge);
			this->gpSelectType->Controls->Add(this->rbSelectVertex);
			this->gpSelectType->Location = System::Drawing::Point(19, 103);
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
			this->gpSelectType->ResumeLayout(false);
			this->gpSelectType->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion
private: System::Void hkoglPanelControl1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) 
		 {
			 if ( mesh != NULL )
				 mesh->Render_Solid();
		 }
private: System::Void hkoglPanelControl2_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
		 }
private: System::Void btnLoadMesh_Click(System::Object^  sender, System::EventArgs^  e) {
			 openMeshFileDialog->ShowDialog();
		 }
private: System::Void openMeshFileDialog_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e) {
			 std::string filename;
			 MarshalString( openMeshFileDialog->FileName, filename );

			 if( mesh != NULL )
				 delete mesh ;

			 mesh = new Tri_Mesh;


			 if(ReadFile(  filename, mesh  ) )
				 std::cout<<filename<<std::endl;

			 hkoglPanelControl1->Invalidate();
		 }
};
}


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

	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
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
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->groupBox1->SuspendLayout();
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
			this->hkoglPanelControl1->Size = System::Drawing::Size(1300, 835);
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
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->button1);
			this->groupBox1->Dock = System::Windows::Forms::DockStyle::Top;
			this->groupBox1->Location = System::Drawing::Point(1300, 0);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(228, 835);
			this->groupBox1->TabIndex = 2;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"groupBox1";
			// 
			// button1
			// 
			this->button1->Dock = System::Windows::Forms::DockStyle::Top;
			this->button1->Location = System::Drawing::Point(3, 18);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(222, 78);
			this->button1->TabIndex = 0;
			this->button1->Text = L"button1";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
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
			this->ClientSize = System::Drawing::Size(1528, 835);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->hkoglPanelControl1);
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->groupBox1->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
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
private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
		     openFileDialog1->ShowDialog();
		 }
private: System::Void hkoglPanelControl1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) 
		 {
			 if ( mesh != NULL )
				 mesh->Render_Solid();
		 }
private: System::Void hkoglPanelControl2_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
		 }
};
}


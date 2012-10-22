#pragma once
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

namespace CLRApplication {

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
	private: OpenGLPanel::OpenGLPanelControl^  openGLPanelControl1;
	protected: 

	protected: 

	protected: 

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
			this->openGLPanelControl1 = (gcnew OpenGLPanel::OpenGLPanelControl());
			this->SuspendLayout();
			// 
			// openGLPanelControl1
			// 
			this->openGLPanelControl1->Far = 0;
			this->openGLPanelControl1->Fov = 0;
			this->openGLPanelControl1->Location = System::Drawing::Point(12, 12);
			this->openGLPanelControl1->Name = L"openGLPanelControl1";
			this->openGLPanelControl1->Near = 0;
			this->openGLPanelControl1->Size = System::Drawing::Size(855, 596);
			this->openGLPanelControl1->TabIndex = 0;
			this->openGLPanelControl1->Type = OpenGLPanel::OpenGLPanelControl::CAMERATYPE::PERSPECTIVE;
			this->openGLPanelControl1->Zoom = 0;
			// 
			// Form1
			// 
			this->ClientSize = System::Drawing::Size(888, 620);
			this->Controls->Add(this->openGLPanelControl1);
			this->Name = L"Form1";
			this->ResumeLayout(false);

		}
#pragma endregion
	};
}


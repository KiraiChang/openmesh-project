#pragma once
#include "open_gl\m_open_gl.h"
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

MOpenGL m_openGL;

namespace OpenGLPanel {
	/// <summary>
	/// Summary for OpenGLPanelControl
	/// </summary>
	public ref class OpenGLPanelControl : public System::Windows::Forms::UserControl
	{
	public:
		enum class CAMERATYPE
		{
			PERSPECTIVE = 0,
			ORTHOGRAPHIC,
		};
	public:
		property double Far;
		property double Fov;
		property double Near;
		property CAMERATYPE Type;
		property double Zoom;

		OpenGLPanelControl(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			OpenGLInit();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~OpenGLPanelControl()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::ComponentModel::IContainer^  components;
	private: System::Windows::Forms::Timer^  m_timer;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		HDC  m_hDC; // Device Context Handler

		void OpenGLInit(void)
		{
			// Get Device Context
			//m_hDC = GetDC((HWND)(this->Handle.ToInt32()));
			m_hDC = GetDC((HWND)(this->Handle.ToInt32()));

			if(!m_openGL.init(m_hDC))
			{
				MessageBox::Show("OpenGL Init Error");
			}

			m_openGL.initOpenGL();
			m_openGL.initScene(0, 0, this->ClientSize.Width, this->ClientSize.Height);
		}

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->m_timer = (gcnew System::Windows::Forms::Timer(this->components));
			this->SuspendLayout();
			// 
			// m_timer
			// 
			this->m_timer->Enabled = true;
			this->m_timer->Interval = 20;
			this->m_timer->Tick += gcnew System::EventHandler(this, &OpenGLPanelControl::m_timer_Tick);
			// 
			// OpenGLPanelControl
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Name = L"OpenGLPanelControl";
			this->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &OpenGLPanelControl::OpenGLPanelControl_Paint);
			this->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &OpenGLPanelControl::OpenGLPanelControl_MouseDown);
			this->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &OpenGLPanelControl::OpenGLPanelControl_MouseMove);
			this->Resize += gcnew System::EventHandler(this, &OpenGLPanelControl::OpenGLPanelControl_Resize);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void OpenGLPanelControl_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) 
			 {
				 m_openGL.renderScene();
				 m_openGL.swapBuffers();
			 }
	private: System::Void OpenGLPanelControl_Resize(System::Object^  sender, System::EventArgs^  e) 
			 {
				 m_openGL.initScene(this->Location.X, this->Location.Y, this->ClientSize.Width, this->ClientSize.Height);
				 Refresh();
			 }
	private: System::Void m_timer_Tick(System::Object^  sender, System::EventArgs^  e) 
			 {
				 Refresh();
			 }
	private: System::Void OpenGLPanelControl_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
			 {

			 }
	private: System::Void OpenGLPanelControl_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
			 {
				 // `如果按下滑鼠右鍵, 就旋轉鏡頭.`
				 if ( e->Button == System::Windows::Forms::MouseButtons::Right ) 
				 {
					 //m_openGL.calcCamera(e->Location.X, e->Location.Y);
				 }
			 }
};
}

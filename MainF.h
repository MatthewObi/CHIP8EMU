#ifndef MAINF_H
#define MAINF_H

#ifndef WX_PRECOMP
	//(*HeadersPCH(MainF)
	#include <wx/menu.h>
	#include <wx/filedlg.h>
	#include <wx/frame.h>
	#include <wx/gauge.h>
	//*)
#endif
//(*Headers(MainF)
#include "include/wxSFMLCanvas.h"
//*)

#include "System.h"

class MainF: public wxFrame
{
	public:

		MainF(wxWindow* parent,wxWindowID id=wxID_ANY);
		virtual ~MainF();

		//(*Declarations(MainF)
		wxMenuItem* MenuItemClose;
		wxMenuItem* MenuItem2;
		wxMenuItem* MenuItem1;
		wxGauge* Gauge1;
		wxFileDialog* FileDialog1;
		wxMenu* Menu1;
		wxMenuItem* MenuItem3;
		wxMenuBar* MenuBar1;
		Canvas* SFMLCanvas;
		wxMenu* Menu2;
		//*)

	protected:

		//(*Identifiers(MainF)
		static const long ID_SFMLCANVAS;
		static const long ID_GAUGE1;
		static const long ID_MENUITEM1;
		static const long ID_MENUITEMCLOSE;
		static const long ID_MENUITEM2;
		static const long ID_MENUITEM3;
		//*)

	private:

		//(*Handlers(MainF)
		void OnMenuQuitSelected(wxCommandEvent& event);
		void OnMenuFileOpenSelected(wxCommandEvent& event);
		void OnMenuCloseSelected(wxCommandEvent& event);
		void OnResize(wxSizeEvent& event);
		void OnAbout(wxCommandEvent& event);
		//*)
		sf::RenderTexture m_buffer;

		DECLARE_EVENT_TABLE()
};

#endif

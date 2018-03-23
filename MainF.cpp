#include "wx_pch.h"
#include "MainF.h"

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(MainF)
	#include <wx/intl.h>
	#include <wx/string.h>
	//*)
#endif
//(*InternalHeaders(MainF)
#include <wx/bitmap.h>
#include <wx/icon.h>
#include <wx/image.h>
//*)

//(*IdInit(MainF)
const long MainF::ID_SFMLCANVAS = wxNewId();
const long MainF::ID_GAUGE1 = wxNewId();
const long MainF::ID_MENUITEM1 = wxNewId();
const long MainF::ID_MENUITEMCLOSE = wxNewId();
const long MainF::ID_MENUITEM2 = wxNewId();
const long MainF::ID_MENUITEM3 = wxNewId();
//*)

BEGIN_EVENT_TABLE(MainF,wxFrame)
	//(*EventTable(MainF)
	//*)
END_EVENT_TABLE()

MainF::MainF(wxWindow* parent,wxWindowID id)
{
    m_buffer.create(64, 32);
    m_buffer.display();
    SetTitle(wxString("CHIP8EMU"));
	//(*Initialize(MainF)
	Create(parent, wxID_ANY, _("CHIP8EMU"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("wxID_ANY"));
	SetClientSize(wxSize(640,400));
	SetMinSize(wxSize(640,400));
	SetMaxSize(wxSize(640,400));
	SetBackgroundColour(wxColour(0,0,0));
	{
		wxIcon FrameIcon;
		FrameIcon.CopyFromBitmap(wxBitmap(wxImage(_T("D:\\Projects\\wxWidgets\\Test\\MyTest\\C8.ico"))));
		SetIcon(FrameIcon);
	}
	SFMLCanvas = new Canvas((wxWindow*)this,ID_SFMLCANVAS,wxDefaultPosition,wxSize(640,400),&m_buffer/*,,wxDefaultValidator,_T("ID_SFMLCANVAS")*/);
	Gauge1 = new wxGauge(this, ID_GAUGE1, 100, wxPoint(0,464), wxSize(640,16), 0, wxDefaultValidator, _T("ID_GAUGE1"));
	Gauge1->Hide();
	MenuBar1 = new wxMenuBar();
	Menu1 = new wxMenu();
	MenuItem1 = new wxMenuItem(Menu1, ID_MENUITEM1, _("Open..."), wxEmptyString, wxITEM_NORMAL);
	Menu1->Append(MenuItem1);
	MenuItemClose = new wxMenuItem(Menu1, ID_MENUITEMCLOSE, _("Close"), wxEmptyString, wxITEM_NORMAL);
	Menu1->Append(MenuItemClose);
	MenuItemClose->Enable(false);
	Menu1->AppendSeparator();
	MenuItem2 = new wxMenuItem(Menu1, ID_MENUITEM2, _("Quit"), wxEmptyString, wxITEM_NORMAL);
	Menu1->Append(MenuItem2);
	MenuBar1->Append(Menu1, _("File"));
	Menu2 = new wxMenu();
	MenuItem3 = new wxMenuItem(Menu2, ID_MENUITEM3, _("About"), wxEmptyString, wxITEM_NORMAL);
	Menu2->Append(MenuItem3);
	MenuBar1->Append(Menu2, _("Help"));
	SetMenuBar(MenuBar1);
	FileDialog1 = new wxFileDialog(this, _("Select file"), _("."), wxEmptyString, wxFileSelectorDefaultWildcardStr, wxFD_DEFAULT_STYLE|wxFD_OPEN|wxFD_FILE_MUST_EXIST, wxDefaultPosition, wxDefaultSize, _T("wxFileDialog"));
	Center();

	Connect(ID_MENUITEM1,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&MainF::OnMenuFileOpenSelected);
	Connect(ID_MENUITEMCLOSE,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&MainF::OnMenuCloseSelected);
	Connect(ID_MENUITEM2,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&MainF::OnMenuQuitSelected);
	Connect(ID_MENUITEM3,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&MainF::OnAbout);
	Connect(wxEVT_SIZE,(wxObjectEventFunction)&MainF::OnResize);
	//*)
}

MainF::~MainF()
{
	//(*Destroy(MainF)
	//*)
}

void MainF::OnAbout(wxCommandEvent& event)
{
    if(wxMessageBox(wxString("CHIP8EMU CHIP8 emulator written in C++.\nAuthor: Matthew Obi\nCopyright: 2018")) == wxID_OK) {

    }
}


void MainF::OnMenuQuitSelected(wxCommandEvent& event)
{
    Close(true);
}

void MainF::OnMenuFileOpenSelected(wxCommandEvent& event)
{
    if (FileDialog1->ShowModal() == wxID_OK) // if the user click "Open" instead of "cancel"
	{
		wxString CurrentDocPath = FileDialog1->GetPath();
		// Set the Title to reflect the  file open
		SetTitle(wxString("CHIP8EMU - ") << FileDialog1->GetFilename());
		SFMLCanvas->sys.load(std::string(FileDialog1->GetPath()));
		MenuItemClose->Enable(true);
	}
}

void MainF::OnMenuCloseSelected(wxCommandEvent& event)
{
    SetTitle("CHIP8EMU");
    SFMLCanvas->sys.close();
    MenuItemClose->Enable(false);
}

void MainF::OnResize(wxSizeEvent& event)
{
    SFMLCanvas->onResize(event);
}

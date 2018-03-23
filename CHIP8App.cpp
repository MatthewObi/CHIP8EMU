/***************************************************************
 * Name:      CHIP8App.cpp
 * Purpose:   Code for Application Class
 * Author:    Matt ()
 * Created:   2017-11-22
 * Copyright: Matt ()
 * License:
 **************************************************************/

#include "wx_pch.h"
#include "CHIP8App.h"

//(*AppHeaders
#include "MainF.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(CHIP8App);

bool CHIP8App::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	MainF* Frame = new MainF(0);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    //*)
    return wxsOK;

}

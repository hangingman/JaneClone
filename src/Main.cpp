#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/image.h>
#include "JaneClone.h"


// OnInitが呼ばれる前にロケールのクラスを継承しておく
class wxMain: public wxApp {
	wxLocale m_Locale;
public:
	wxMain() : m_Locale(wxLANGUAGE_DEFAULT){}
    bool OnInit();
};

IMPLEMENT_APP(wxMain)

bool wxMain::OnInit()
{
    wxInitAllImageHandlers();
    JaneClone* wxJaneClone = new JaneClone(NULL, wxID_ANY, wxEmptyString);
    SetTopWindow(wxJaneClone);
    wxJaneClone->Show();
    return true;
}

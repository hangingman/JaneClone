/* JaneClone - a text board site viewer for 2ch
 * Copyright (C) 2012 Hiroyuki Nagata
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * Contributor:
 *	Hiroyuki Nagata <newserver002@gmail.com>
 */

//#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/image.h>
#include <wx/snglinst.h>
#include "janeclone.hpp"


/*
 * wxAppを継承したwxMainを宣言
 */
class wxMain: public wxApp {

     wxLocale m_Locale;

public:
     wxMain() : m_Locale(wxLANGUAGE_DEFAULT){}
     bool OnInit();
     int OnExit();

private:
     wxSingleInstanceChecker* m_checker;
     JaneClone* wxJaneClone;
};

IMPLEMENT_APP(wxMain)

/**
 * wxMainの実装
 */
bool wxMain::OnInit() {

     // JaneClone起動前に複数起動をチェックする
     const wxString name = wxString::Format("JaneClone-%s", wxGetUserId().c_str());
     m_checker = new wxSingleInstanceChecker(name);
     if ( m_checker->IsAnotherRunning() ) {
	  wxMessageBox(wxT("誤作動防止のためJaneCloneは複数起動できません。終了します。"), wxT("JaneClone起動"), wxOK | wxICON_ERROR);
	  return false;
     }

     wxInitAllImageHandlers();
     wxJaneClone = new JaneClone(NULL, wxID_ANY, wxEmptyString);

     SetTopWindow(wxJaneClone);
     wxJaneClone->Show();
     return true;
}
/**
 * 終了後の後始末
 */
int wxMain::OnExit() {
    delete m_checker;
    delete wxJaneClone;
    return 0;
}

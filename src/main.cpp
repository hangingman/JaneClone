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
#include "janeclone.hpp"


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

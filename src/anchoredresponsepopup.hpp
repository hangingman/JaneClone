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

#ifndef ANCHOREDRESPONSEPOPUP_H_
#define ANCHOREDRESPONSEPOPUP_H_

#include <wx/html/htmlwin.h>
#include <wx/scrolwin.h>
#include <wx/popupwin.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/stattext.h>
#include "datatype.hpp"

class AnchoredResponsePopup: public wxPopupTransientWindow {

	public:
	AnchoredResponsePopup( wxWindow *parent, wxPoint& point, wxSize size, wxString& htmlSource );
    virtual ~AnchoredResponsePopup(){};

    void Popup(wxWindow *focus = NULL);
    void OnDismiss();
    bool ProcessLeftDown(wxMouseEvent& event);
    bool Show( bool show = true );
    wxSize GetPopupWindowSize();

private:
    void OnMouse( wxMouseEvent &event );
    void OnSize( wxSizeEvent &event );
    void OnSetFocus( wxFocusEvent &event );
    void OnKillFocus( wxFocusEvent &event );

private:
    wxHtmlWindow *htmlWin;

    DECLARE_CLASS(AnchoredResponsePopup)
    DECLARE_EVENT_TABLE()
};

#endif /* ANCHOREDRESPONSEPOPUP_H_ */

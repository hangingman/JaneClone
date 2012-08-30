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

#include "AnchoredResponsePopup.h"

IMPLEMENT_CLASS(AnchoredResponsePopup,wxPopupTransientWindow)

BEGIN_EVENT_TABLE(AnchoredResponsePopup,wxPopupTransientWindow)
EVT_MOUSE_EVENTS( AnchoredResponsePopup::OnMouse )
EVT_SIZE( AnchoredResponsePopup::OnSize )
EVT_SET_FOCUS( AnchoredResponsePopup::OnSetFocus )
EVT_KILL_FOCUS( AnchoredResponsePopup::OnKillFocus )
END_EVENT_TABLE()

AnchoredResponsePopup::AnchoredResponsePopup(wxWindow* parent, wxString& htmlSource)
:wxPopupTransientWindow( parent )
{
	m_panel = new wxScrolledWindow( this, wxID_ANY );
	m_panel->SetBackgroundColour( *wxLIGHT_GREY );

	m_panel->Connect(wxEVT_MOTION,
			wxMouseEventHandler(AnchoredResponsePopup::OnMouse),
			NULL, this);

	m_htmlWin = new wxHtmlWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHW_SCROLLBAR_AUTO);
	wxString html = HTML_HEADER;
	html+=htmlSource;
	html+=HTML_FOOTER;
	m_htmlWin->SetPage(html);

	wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );
	topSizer->Add( m_htmlWin, 0, wxEXPAND, 5 );

	m_panel->SetSizer( topSizer );
	topSizer->Fit(m_panel);
	topSizer->Fit(this);
}

void AnchoredResponsePopup::Popup(wxWindow* focus) {
    wxPopupTransientWindow::Popup();
}

void AnchoredResponsePopup::OnDismiss() {
    wxPopupTransientWindow::OnDismiss();
}

bool AnchoredResponsePopup::ProcessLeftDown(wxMouseEvent& event) {
    return wxPopupTransientWindow::ProcessLeftDown(event);
}

bool AnchoredResponsePopup::Show(bool show) {
    return wxPopupTransientWindow::Show(show);
}

void AnchoredResponsePopup::OnMouse(wxMouseEvent& event) {
}

void AnchoredResponsePopup::OnSize(wxSizeEvent& event) {
}

void AnchoredResponsePopup::OnSetFocus(wxFocusEvent& event) {
}

void AnchoredResponsePopup::OnKillFocus(wxFocusEvent& event) {
}


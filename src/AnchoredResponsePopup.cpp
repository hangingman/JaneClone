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

enum {
	Minimal_StartSimplePopup, Minimal_StartScrolledPopup, Minimal_LogWindow,
};

IMPLEMENT_CLASS(AnchoredResponsePopup,wxPopupTransientWindow)

BEGIN_EVENT_TABLE(AnchoredResponsePopup,wxPopupTransientWindow)
EVT_MOUSE_EVENTS( AnchoredResponsePopup::OnMouse )
EVT_SIZE( AnchoredResponsePopup::OnSize )
EVT_SET_FOCUS( AnchoredResponsePopup::OnSetFocus )
EVT_KILL_FOCUS( AnchoredResponsePopup::OnKillFocus )
END_EVENT_TABLE()

AnchoredResponsePopup::AnchoredResponsePopup( wxWindow *parent, wxPoint& point, wxSize size, wxString& htmlSource )
:wxPopupTransientWindow( parent )
{
	this->Position(point, size);
	wxBoxSizer *topsizer;
	wxHtmlWindow *htmlWin;
	wxString html = HTML_HEADER;
	html+=htmlSource;
	html+=HTML_FOOTER;

	topsizer = new wxBoxSizer(wxVERTICAL);
	// ここでサイズ指定してやらないと異常終了します
	htmlWin = new wxHtmlWindow(this, wxID_ANY, point, size, wxHW_SCROLLBAR_NEVER);
	htmlWin->SetBorders(0);
	htmlWin->SetPage(html);
	htmlWin->SetSize(htmlWin->GetInternalRepresentation()->GetWidth(),
			htmlWin->GetInternalRepresentation()->GetHeight());

	topsizer->Add(htmlWin, 1, wxALL, 10);
	htmlWin->SetSizer(topsizer);
	topsizer->Fit(this);
}

void AnchoredResponsePopup::Popup(wxWindow* WXUNUSED(focus)) {
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

void AnchoredResponsePopup::OnSize(wxSizeEvent &event) {
	event.Skip();
}

void AnchoredResponsePopup::OnSetFocus(wxFocusEvent &event) {
	event.Skip();
}

void AnchoredResponsePopup::OnKillFocus(wxFocusEvent &event) {
	event.Skip();
}

void AnchoredResponsePopup::OnMouse(wxMouseEvent &event) {
	event.Skip();
}

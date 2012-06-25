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

#include "ThreadContentWindow.h"

IMPLEMENT_DYNAMIC_CLASS(ThreadContentWindow, wxHtmlWindow)

ThreadContentWindow::ThreadContentWindow() {
}

bool ThreadContentWindow::LoadFile(const wxFileName& filename) {

}
bool ThreadContentWindow::LoadPage(const wxString& location) {

}
bool ThreadContentWindow::OnCellClicked(wxHtmlCell *cell, wxCoord x, wxCoord y,
		const wxMouseEvent& event) {

}
void ThreadContentWindow::OnCellMouseHover(wxHtmlCell *cell, wxCoord x,
		wxCoord y) {

}
void ThreadContentWindow::OnLinkClicked(const wxHtmlLinkInfo& link) {

}
wxHtmlOpeningStatus ThreadContentWindow::OnOpeningURL(wxHtmlURLType type,
		const wxString& url, wxString *redirect) {

}
void ThreadContentWindow::OnSetTitle(const wxString& title) {

}
void ThreadContentWindow::ReadCustomization(wxConfigBase *cfg, wxString path) {

}
void ThreadContentWindow::WriteCustomization(wxConfigBase *cfg, wxString path) {

}

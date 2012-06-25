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

#ifndef THREADCONTENTWINDOW_H_
#define THREADCONTENTWINDOW_H_

#include <wx/html/htmlwin.h>

class WXDLLEXPORT ThreadContentWindow : public wxHtmlWindow {
public:
	ThreadContentWindow();
	bool LoadFile(const wxFileName& filename);
	bool LoadPage(const wxString& location);
	bool OnCellClicked(wxHtmlCell *cell, wxCoord x, wxCoord y, const wxMouseEvent& event);
	void OnCellMouseHover(wxHtmlCell *cell, wxCoord x, wxCoord y);
	void OnLinkClicked(const wxHtmlLinkInfo& link);
	wxHtmlOpeningStatus OnOpeningURL(wxHtmlURLType type,const wxString& url, wxString *redirect);
	void OnSetTitle(const wxString& title);
	void ReadCustomization(wxConfigBase *cfg, wxString);
	void WriteCustomization(wxConfigBase *cfg, wxString);

private:
	DECLARE_DYNAMIC_CLASS(ThreadContentWindow)
};

#endif /* THREADCONTENTWINDOW_H_ */

/**
 * imagepanel.hpp - simple panel for wxImage
 * I refer http://wiki.wxwidgets.org/An_image_panel
 *
 * Copyright (c) 2013 Hiroyuki Nagata <newserver002@gmail.com>
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
#ifndef IMAGEPANEL_HPP_
#define IMAGEPANEL_HPP_

#include <wx/wx.h>
#include <wx/sizer.h>

class wxImagePanel : public wxPanel {

     wxBitmap image;

public:
     /// Default constructor
     wxImagePanel(wxWindow* parent, const wxString file, const wxBitmapType type);
     /// Copy constructor
     wxImagePanel(const wxImagePanel& rhs);
     /// Assignment operator
     wxImagePanel& operator=(const wxImagePanel& rhs);

     void PaintEvent(wxPaintEvent & evt);
     void PaintNow();
     void Render(wxDC& dc);

     DECLARE_EVENT_TABLE()
};


#endif /* IMAGEPANEL_HPP_ */

/**
 * imagepanel.cpp - simple panel for wxImage
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
#include "imagepanel.hpp"
#include "janecloneimageviewer.hpp"

BEGIN_EVENT_TABLE(wxImagePanel, wxPanel)

EVT_RIGHT_DOWN(wxImagePanel::RightClick)
// some useful events
/*
  EVT_MOTION(wxImagePanel::mouseMoved)
  EVT_LEFT_DOWN(wxImagePanel::mouseDown)
  EVT_LEFT_UP(wxImagePanel::mouseReleased)
  EVT_RIGHT_DOWN(wxImagePanel::rightClick)
  EVT_LEAVE_WINDOW(wxImagePanel::mouseLeftWindow)
  EVT_KEY_DOWN(wxImagePanel::keyPressed)
  EVT_KEY_UP(wxImagePanel::keyReleased)
  EVT_MOUSEWHEEL(wxImagePanel::mouseWheelMoved)
*/
// catch paint events
EVT_PAINT(wxImagePanel::PaintEvent)
 
END_EVENT_TABLE()

/**
 * Default constructor
 */
wxImagePanel::wxImagePanel(wxWindow* parent, const wxString file, const wxBitmapType type) : wxPanel(parent) {
     // load the file... ideally add a check to see if loading was successful
     image.LoadFile(file, type);
}
/**
 * Copy constructor
 */
wxImagePanel::wxImagePanel(const wxImagePanel& rhs) {
}
/**
 * Assignment operator
 */
wxImagePanel& wxImagePanel::operator=(const wxImagePanel& rhs) {
     if (this != &rhs) {
	  // TODO: implement copy
     }
     return *this;
}
/*
 * Called by the system of by wxWidgets when the panel needs
 * to be redrawn. You can also trigger this call by
 * calling Refresh()/Update().
 */
void wxImagePanel::PaintEvent(wxPaintEvent & evt) {
     // depending on your system you may need to look at double-buffered dcs
     wxPaintDC dc(this);
     Render(dc);
}
/*
 * Alternatively, you can use a clientDC to paint on the panel
 * at any time. Using this generally does not free you from
 * catching paint events, since it is possible that e.g. the window
 * manager throws away your drawing when the window comes to the
 * background, and expects you will redraw it when the window comes
 * back (by sending a paint event).
 */
void wxImagePanel::PaintNow() {
     // depending on your system you may need to look at double-buffered dcs
     wxClientDC dc(this);
     Render(dc);
}
/*
 * Here we do the actual rendering. I put it in a separate
 * method so that it can work no matter what type of DC
 * (e.g. wxPaintDC or wxClientDC) is used.
 */
void wxImagePanel::Render(wxDC&  dc) {
     dc.DrawBitmap( image, 0, 0, false );
}
/**
 * パネル上で右クリックされた場合の処理
 */
void wxImagePanel::RightClick(wxMouseEvent& event) {

     // 親クラスのイベントを呼ぶ・戻ってこない
     return ((JaneCloneImageViewer*)this->GetParent())->OnRightClickImageViewer(event);
}


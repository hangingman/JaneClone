/**
 * imagepanel.hpp - simple panel for wxImage
 * I refer http://wiki.wxwidgets.org/An_image_panel
 *
 * Copyright (c) 2013 Hiroyuki Nagata <newserver002@gmail.com>
 * 
 *
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

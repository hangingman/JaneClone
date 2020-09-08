/**
 * imagepanel.hpp - simple panel for wxImage
 * I refer http://wiki.wxwidgets.org/An_image_panel
 *
 * Copyright (C) 2013-2014 Hiroyuki Nagata <idiotpanzer@gmail.com>
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
 *	Hiroyuki Nagata <idiotpanzer@gmail.com>
 */
#ifndef IMAGEPANEL_HPP_
#define IMAGEPANEL_HPP_

#include <wx/wx.h>
#include <wx/sizer.h>
#include "datatype.hpp"
#include "enums.hpp"

class wxImagePanel : public wxPanel {

     wxBitmap image;

public:
     /// Default constructor
     wxImagePanel(wxWindow* parent, std::unique_ptr<DownloadImageResult>& result, const wxBitmapType type);
     /// Copy constructor
     wxImagePanel(const wxImagePanel& rhs);
     /// Assignment operator
     wxImagePanel& operator=(const wxImagePanel& rhs);     

     void PaintEvent(wxPaintEvent & evt);
     void PaintNow();
     void Render(wxDC& dc);
     wxString GetFilePath();
     wxString GetImageURL();
     const DownloadImageResult& GetDownloadImageResult()
	  {
	       return imageInfo;
	  };
     
     void Rotate90(bool clockwise);
     void Resize(bool toBig);
     void Reset();
     double GetMagnification();
     void SetMagnification(double magnification);

private:
     // 画像ファイル情報
     DownloadImageResult imageInfo;
     // ビットマップの種別
     wxBitmapType m_type;
     // 画像の倍率(0.1 ~ x ~ 2.0)
     double magnification;
     // 画像の元のサイズ
     int originalHeight;
     int originalWidth;

     DECLARE_EVENT_TABLE()
};


#endif /* IMAGEPANEL_HPP_ */

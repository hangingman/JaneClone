/**
 * janecloneimageviewer.hpp -  janeclone's plain imageviewer
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
#ifndef JANECLONEIMAGEVIEWER_HPP_
#define JANECLONEIMAGEVIEWER_HPP_

#include <wx/wx.h>
#include <wx/aui/auibook.h>
#include <wx/dc.h>
#include <wx/dcclient.h>
#include "enums.hpp"
#include "datatype.hpp"
#include "imagepanel.hpp"

/**
 * 画像のサムネイルと本体の表示を行うウィンドウ
 */
//class JaneCloneImageViewer : public wxDialog {
class JaneCloneImageViewer : public wxFrame {

public:

     /// constructor
     JaneCloneImageViewer(wxWindow* parent, int id, const wxString& title,
			  const wxPoint& pos = wxDefaultPosition, const wxSize& size =
			  wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE);
     
     // 画像ファイルの情報を設定する
     void SetImageFile(DownloadImageResult* result);

private:
     /// Default constructor
     JaneCloneImageViewer();
     ~JaneCloneImageViewer();
   
     void set_properties(const wxString& title);
     void do_layout();

     // Auiの管理オブジェクト
     wxAuiManager m_mgr;
     // 画像を載せるノートブック
     wxAuiNotebook* thumbnailNoteBook;

     /// Copy constructor
     JaneCloneImageViewer(const JaneCloneImageViewer& rhs);
     /// Assignment operator
     JaneCloneImageViewer& operator=(const JaneCloneImageViewer& rhs);
};


#endif /* JANECLONEIMAGEVIEWER_HPP_ */

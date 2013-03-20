/**
 * janecloneimageviewer.cpp - janeclone's plain imageviewer
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

#include "janecloneimageviewer.hpp"

#ifndef __WXMSW__
#include"../rc/janeclone.xpm"
#endif

/**
 * Default constructor
 */
JaneCloneImageViewer::JaneCloneImageViewer() {
}
/**
 * Default constructor
 */
JaneCloneImageViewer::JaneCloneImageViewer(wxWindow* parent, wxString& title):
     wxDialog(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE) {

     // アイコンの設定を行う
#ifdef __WXMSW__
     SetIcon(wxICON(wxicon));
#else
     SetIcon(wxICON(janeclone));
#endif

     set_properties(title);
     do_layout();
}
/**
 * タイトルとサイズを設定
 */
void JaneCloneImageViewer::set_properties(const wxString& title) {
    SetTitle(title);
    SetSize(wxSize(640, 480));
}
/**
 * レイアウトを設定する
 */
void JaneCloneImageViewer::do_layout() {
    Layout();
}
/**
 * Default destructor
 */
JaneCloneImageViewer::~JaneCloneImageViewer() {
}

/**
 * Copy constructor
 */
JaneCloneImageViewer::JaneCloneImageViewer(const JaneCloneImageViewer& rhs) {
}
/**
 * Assignment operator
 */
JaneCloneImageViewer& JaneCloneImageViewer::operator=(const JaneCloneImageViewer& rhs) {
    if (this != &rhs) {
        // TODO: implement copy
    }
    return *this;
}
/**
 *  画像ファイルの情報を設定する
 */
void JaneCloneImageViewer::SetImageFile(DownloadImageResult* result) {

}

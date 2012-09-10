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

#ifndef RESPONSEWINDOW_HPP_
#define RESPONSEWINDOW_HPP_

#include <wx/wx.h>
#include <wx/notebook.h>

/**
 * レス書き込み用の子ウィンドウ
 */
class ResponseWindow : public wxDialog {

public:
     ResponseWindow(wxWindow* parent, wxString& title);
     ~ResponseWindow(){};

     void Popup(wxWindow *focus = NULL);

private:
     // ノートブックにタブを設定する
     void SetTabToNotebook();

     // レス用のウィンドウの基盤にあたるノートブック
     wxNotebook* responseNoteBook;
     
};

#endif /* RESPONSEWINDOW_HPP_ */

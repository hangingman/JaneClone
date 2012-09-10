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

#include "responsewindow.hpp"

#ifndef __WXMSW__
#include"../rc/janeclone.xpm"
#endif

ResponseWindow::ResponseWindow(wxWindow* parent, wxString& title)
     : wxDialog( NULL, -1, wxEmptyString, wxDefaultPosition, wxSize(640, 480)) {
     
     // アイコンの設定を行う
#ifdef __WXMSW__
     SetIcon(wxICON(wxicon));
#else
     SetIcon(wxICON(janeclone));
#endif

     // 対象のスレッドタイトルを設定する
     SetTitle(wxT("『") + title + wxT("』にレス"));

     // 一つのパネルには一つのサイザーが伴う
     wxPanel* panel = new wxPanel(this, wxID_ANY);
     wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

     // hbox1 ノートブック部分
     wxBoxSizer* hbox1 = new wxBoxSizer(wxHORIZONTAL);
     responseNoteBook = new wxNotebook(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_TOP);
     hbox1->Add(responseNoteBook, 0, wxTOP, 8);
     vbox->Add(this, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);

     // hbox2 チェックボックス等
     //wxBoxSizer* hbox2 = new wxBoxSizer(wxHORIZONTAL);

     Centre();
     ShowModal();
     Destroy();
}
/**
 * ノートブックにタブを設定する
 */
void ResponseWindow::SetTabToNotebook() {

}



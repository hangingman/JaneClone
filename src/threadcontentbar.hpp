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

// -*- C++ -*- generated by wxGlade 0.6.3 on Sat Dec 29 22:07:54 2012

#include <wx/wx.h>
#include <wx/image.h>
#include <wx/aui/aui.h>
#include "enums.hpp"
#include "threadcontentwindow.hpp"

#ifndef THREADCONTENTBAR_H
#define THREADCONTENTBAR_H

// begin wxGlade: ::dependencies
// end wxGlade

// 各ボタンに貼り付けられる画像
static const wxString autoReloadImg = wxT("rc/appointment-new.png");
static const wxString redResExtractImg = wxT("rc/system-search.png");
static const wxString refreshImg = wxT("rc/view-refresh.png");
static const wxString scrollToNewResImg = wxT("rc/go-bottom.png");
static const wxString stopImg = wxT("rc/dialog-error.png");
static const wxString resExtractImg = wxT("rc/edit-find-replace.png");
static const wxString newThreadImg = wxT("rc/go-last.png");
static const wxString responseImg = wxT("rc/format-justify-left.png");
static const wxString bookMarkImg = wxT("rc/bookmark-new.png");
static const wxString deleteLogImg = wxT("rc/edit-delete.png");
static const wxString closeImg = wxT("rc/emblem-unreadable.png");
static const wxString normalSearchImg = wxT("rc/system-search.png");
static const wxString hideSearchBarImg = wxT("rc/emblem-unreadable.png");
static const wxString forwardImg = wxT("rc/go-down.png");
static const wxString backwardImg = wxT("rc/go-up.png");

// 各ウィジェットのサイズなど
static const wxSize   threadContentBarImgSize = wxSize(32, 32);
static const wxSize   searchWordComboSize = wxSize(320, 32);

class ThreadContentBar: public wxPanel {

public:
     // begin wxGlade: ThreadContentBar::ids
     // end wxGlade

     ThreadContentBar(wxWindow* parent, int id, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, 
		      long style=wxDEFAULT_FRAME_STYLE);

     // スレッドタイトルをクラスに設定する
     void SetTitle(const wxString& title);
     // スレッドの内容を設定する
     void SetThreadContentWindow(const wxString& threadContentPath);

private:
     // begin wxGlade: ThreadContentBar::methods
     void set_properties();
     void do_layout();
     // end wxGlade

     // ボタン上をマウスがホバーした場合の処理
     //void OnHoverTCBAutoReload(wxMouseEvent& event);

     // ボタンをマウスがクリックした場合の処理
     // void OnClickTCBAutoReload(wxCommandEvent& event);
     // void OnClickTCBRedResExtract(wxCommandEvent event);
     // void OnClickTCBRefresh(wxCommandEvent event);
     // void OnClickTCBScrollToNewRes(wxCommandEvent event);
     // void OnClickTCBStop(wxCommandEvent event);
     // void OnClickTCBResExtract(wxCommandEvent event);
     // void OnClickTCBNewThread(wxCommandEvent event);
     // void OnClickTCBResponse(wxCommandEvent event);
     // void OnClickTCBBookMark(wxCommandEvent event);
     // void OnClickTCBDeleteLog(wxCommandEvent event);
     // void OnClickTCBClose(wxCommandEvent event);
     // void OnClickTCBNormalSearch(wxCommandEvent event);
     // void OnClickTCBHideSearchBar(wxCommandEvent event);
     // void OnClickTCBForward(wxCommandEvent event);
     // void OnClickTCBBackward(wxCommandEvent event);

     // スレッドの内容を描画するクラス
     wxPanel* threadContentPanel;
     // 検索ワードの履歴管理用文字列
     wxString* searchWordCombo_choices;

protected:
     // begin wxGlade: ThreadContentBar::attributes
     wxStaticText* threadName;
     wxPanel* spacePanel1;

     wxAuiToolBar* threadToolbar1;
     wxAuiToolBar* threadToolbar2;

     wxPanel* threadContentsBarUpperSizer;
     wxBitmapButton* nomalSearchButton;
     wxComboBox* searchWordCombo;
     wxButton* backwardButton;
     wxButton* forwardButton;
     wxPanel* panel_2;
     wxBitmapButton* hideSearchBarButton;

     DECLARE_EVENT_TABLE()
     // end wxGlade
}; // wxGlade: end class


#endif // THREADCONTENTBAR_H

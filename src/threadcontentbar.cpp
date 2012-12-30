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

#include "threadcontentbar.hpp"

// event table
BEGIN_EVENT_TABLE(ThreadContentBar, wxPanel)
// マウスホバー時のイベントテーブル
// EVT_MOUSE_EVENTS(ThreadContentBar::OnHoverTCBAutoReload)
// EVT_ENTER_WINDOW(ID_TCBRedResExtract, ThreadContentBar::OnHoverTCBRedResExtract)
// EVT_ENTER_WINDOW(ID_TCBRefresh, ThreadContentBar::OnHoverTCBRefresh)
// EVT_ENTER_WINDOW(ID_TCBScrollToNewRes,  ThreadContentBar::OnHoverTCBScrollToNewRes)
// EVT_ENTER_WINDOW(ID_TCBStop,            ThreadContentBar::OnHoverTCBStop)
// EVT_ENTER_WINDOW(ID_TCBResExtract,      ThreadContentBar::OnHoverTCBResExtract)
// EVT_ENTER_WINDOW(ID_TCBNewThread,       ThreadContentBar::OnHoverTCBNewThread)
// EVT_ENTER_WINDOW(ID_TCBResponse,        ThreadContentBar::OnHoverTCBResponse)
// EVT_ENTER_WINDOW(ID_TCBBookMark,        ThreadContentBar::OnHoverTCBBookMark)
// EVT_ENTER_WINDOW(ID_TCBDeleteLog,       ThreadContentBar::OnHoverTCBDeleteLog)
// EVT_ENTER_WINDOW(ID_TCBClose,           ThreadContentBar::OnHoverTCBClose)
// EVT_ENTER_WINDOW(ID_TCBNormalSearch,    ThreadContentBar::OnHoverTCBNormalSearch)
// EVT_ENTER_WINDOW(ID_TCBHideSearchBar,   ThreadContentBar::OnHoverTCBHideSearchBar)
// EVT_ENTER_WINDOW(ID_TCBForward,         ThreadContentBar::OnHoverTCBForward)
// EVT_ENTER_WINDOW(ID_TCBBackward,        ThreadContentBar::OnHoverTCBBackward)
END_EVENT_TABLE()


ThreadContentBar::ThreadContentBar(wxWindow* parent, int wxWindowID, const wxPoint& pos, const wxSize& size, long style):
     wxPanel(parent, wxWindowID, pos, size, wxDEFAULT_FRAME_STYLE)
{
     // begin wxGlade: ThreadContentBar::ThreadContentBar
     threadContentsBarUpperSizer = new wxPanel(this, wxID_ANY);
     threadContentsBarUpperSizer->SetBackgroundColour(*wxLIGHT_GREY);

     // スレッドタイトルを保持する
     threadName = new wxStaticText(threadContentsBarUpperSizer, wxID_ANY, wxEmptyString);
     spacePanel1 = new wxPanel(threadContentsBarUpperSizer, wxID_ANY);
     // オートリロード
     autoReloadButton = new wxBitmapButton(threadContentsBarUpperSizer, ID_TCBAutoReload, wxBitmap(autoReloadImg, wxBITMAP_TYPE_ANY),
	                                   wxDefaultPosition, threadContentBarImgSize);
     // 赤レス抽出
     redResExtractButton = new wxBitmapButton(threadContentsBarUpperSizer, wxID_ANY, wxBitmap(redResExtractImg, wxBITMAP_TYPE_ANY),
					      wxDefaultPosition, threadContentBarImgSize);
     // 画面更新
     refreshButton = new wxBitmapButton(threadContentsBarUpperSizer, wxID_ANY, wxBitmap(refreshImg, wxBITMAP_TYPE_ANY),
					wxDefaultPosition, threadContentBarImgSize);
     // 新着レスまで移動
     scrollToNewResponseButton = new wxBitmapButton(threadContentsBarUpperSizer, wxID_ANY, wxBitmap(scrollToNewResImg, wxBITMAP_TYPE_ANY),
						    wxDefaultPosition, threadContentBarImgSize);
     // 中止
     stopButton = new wxBitmapButton(threadContentsBarUpperSizer, wxID_ANY, wxBitmap(stopImg, wxBITMAP_TYPE_ANY),
				     wxDefaultPosition, threadContentBarImgSize);
     // レス抽出
     resExtractButton = new wxBitmapButton(threadContentsBarUpperSizer, wxID_ANY, wxBitmap(resExtractImg, wxBITMAP_TYPE_ANY),
					   wxDefaultPosition, threadContentBarImgSize);
     // 新しいスレに移動
     newThreadButton = new wxBitmapButton(threadContentsBarUpperSizer, wxID_ANY, wxBitmap(newThreadImg, wxBITMAP_TYPE_ANY),
					  wxDefaultPosition, threadContentBarImgSize);
     // レス
     responseButton = new wxBitmapButton(threadContentsBarUpperSizer, wxID_ANY, wxBitmap(responseImg, wxBITMAP_TYPE_ANY),
					 wxDefaultPosition, threadContentBarImgSize);
     // お気に入り
     bookMarkButton = new wxBitmapButton(threadContentsBarUpperSizer, wxID_ANY, wxBitmap(bookMarkImg, wxBITMAP_TYPE_ANY),
					 wxDefaultPosition, threadContentBarImgSize);
     // ログ削除
     deleteLogButton = new wxBitmapButton(threadContentsBarUpperSizer, wxID_ANY, wxBitmap(deleteLogImg, wxBITMAP_TYPE_ANY),
					  wxDefaultPosition, threadContentBarImgSize);
     // 閉じる
     closeButton = new wxBitmapButton(threadContentsBarUpperSizer, wxID_ANY, wxBitmap(closeImg, wxBITMAP_TYPE_ANY),
				      wxDefaultPosition, threadContentBarImgSize);
     // 通常検索
     nomalSearchButton = new wxBitmapButton(this, wxID_ANY, wxBitmap(normalSearchImg, wxBITMAP_TYPE_ANY),
					    wxDefaultPosition, threadContentBarImgSize);

     // スレッド内検索用コンボボックス
     searchWordCombo_choices = NULL;
     searchWordCombo = new wxComboBox(this, wxID_ANY, wxT(""), wxDefaultPosition, searchWordComboSize, 0, searchWordCombo_choices, wxCB_DROPDOWN);
     searchWordCombo->SetFont(wxFont(12, wxMODERN, wxNORMAL, wxNORMAL, 0, wxT("")));

     // 引っかかった検索ワードを前後させる
     backwardButton = new wxButton(this, wxID_ANY, wxT("前へ"));
     backwardButton->SetBitmap(wxBitmap(backwardImg, wxBITMAP_TYPE_ANY));
     forwardButton = new wxButton(this, wxID_ANY, wxT("次へ"));
     forwardButton->SetBitmap(wxBitmap(forwardImg, wxBITMAP_TYPE_ANY));
     panel_2 = new wxPanel(this, wxID_ANY);

     // 検索バーを隠す
     hideSearchBarButton = new wxBitmapButton(this, wxID_ANY, wxBitmap(hideSearchBarImg, wxBITMAP_TYPE_ANY),
					      wxDefaultPosition, threadContentBarImgSize);
     // スレッドの内容が乗るパネル
     threadContentPanel = new wxPanel(this, wxID_ANY);

     set_properties();
     do_layout();
     // end wxGlade
}


void ThreadContentBar::set_properties()
{
    // begin wxGlade: ThreadContentBar::set_properties
    threadName->SetFont(wxFont(12, wxMODERN, wxNORMAL, wxNORMAL, 0, wxT("")));
    autoReloadButton->SetSize(autoReloadButton->GetBestSize());
    redResExtractButton->SetSize(redResExtractButton->GetBestSize());
    refreshButton->SetSize(refreshButton->GetBestSize());
    scrollToNewResponseButton->SetSize(scrollToNewResponseButton->GetBestSize());
    stopButton->SetSize(stopButton->GetBestSize());
    resExtractButton->SetSize(resExtractButton->GetBestSize());
    newThreadButton->SetSize(newThreadButton->GetBestSize());
    responseButton->SetSize(responseButton->GetBestSize());
    bookMarkButton->SetSize(bookMarkButton->GetBestSize());
    deleteLogButton->SetSize(deleteLogButton->GetBestSize());
    closeButton->SetSize(closeButton->GetBestSize());
    nomalSearchButton->SetSize(nomalSearchButton->GetBestSize());
    hideSearchBarButton->SetSize(hideSearchBarButton->GetBestSize());
    // end wxGlade
}


void ThreadContentBar::do_layout()
{
    // begin wxGlade: ThreadContentBar::do_layout
    wxBoxSizer* threadContentsBarSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* horizonalSizer2 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* horizonalSizer1 = new wxBoxSizer(wxHORIZONTAL);
    horizonalSizer1->Add(threadName, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    horizonalSizer1->Add(spacePanel1, 1, wxEXPAND, 0);
    horizonalSizer1->Add(autoReloadButton, 0, wxALIGN_RIGHT, 0);
    horizonalSizer1->Add(redResExtractButton, 0, wxALIGN_RIGHT, 0);
    horizonalSizer1->Add(refreshButton, 0, wxALIGN_RIGHT, 0);
    horizonalSizer1->Add(scrollToNewResponseButton, 0, wxALIGN_RIGHT, 0);
    horizonalSizer1->Add(stopButton, 0, 0, 0);
    horizonalSizer1->Add(resExtractButton, 0, 0, 0);
    horizonalSizer1->Add(newThreadButton, 0, 0, 0);
    horizonalSizer1->Add(responseButton, 0, 0, 0);
    horizonalSizer1->Add(bookMarkButton, 0, 0, 0);
    horizonalSizer1->Add(deleteLogButton, 0, 0, 0);
    horizonalSizer1->Add(closeButton, 0, 0, 0);
    threadContentsBarUpperSizer->SetSizer(horizonalSizer1);
    threadContentsBarSizer->Add(threadContentsBarUpperSizer, 0, wxEXPAND, 0);
    horizonalSizer2->Add(nomalSearchButton, 0, 0, 0);
    horizonalSizer2->Add(searchWordCombo, 0, 0, 0);
    horizonalSizer2->Add(backwardButton, 0, 0, 0);
    horizonalSizer2->Add(forwardButton, 0, 0, 0);
    horizonalSizer2->Add(panel_2, 1, wxEXPAND, 0);
    horizonalSizer2->Add(hideSearchBarButton, 0, wxALIGN_RIGHT, 0);
    threadContentsBarSizer->Add(horizonalSizer2, 0, wxEXPAND, 0);
    threadContentsBarSizer->Add(threadContentPanel, 1, wxEXPAND | wxBOTTOM, 5);
    SetSizer(threadContentsBarSizer);
    Layout();
    // end wxGlade
}

void ThreadContentBar::SetTitle(const wxString& title) {
     // スレッドタイトルを設定する
     threadName->SetLabel(wxT("　") + title);
}

void ThreadContentBar::SetThreadContentWindow(const wxString& threadContentPath) {
     // スレッドの内容を表すウィンドウをthreadContentPanelを親として宣言する
     ThreadContentWindow* tcw = new ThreadContentWindow(threadContentPanel, threadContentPath);
     wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
     vbox->Add(tcw, 1, wxEXPAND | wxALL, 5);
     threadContentPanel->SetSizer(vbox);
}

void ThreadContentBar::OnHoverTCBAutoReload(wxMouseEvent& event) {
     wxMessageBox(wxT("test"));
}
void ThreadContentBar::OnHoverTCBRedResExtract(wxMouseEvent event) {
}
void ThreadContentBar::OnHoverTCBRefresh(wxMouseEvent event) {
}
void ThreadContentBar::OnHoverTCBScrollToNewRes(wxMouseEvent event) {
}
void ThreadContentBar::OnHoverTCBStop(wxMouseEvent event) {
}
void ThreadContentBar::OnHoverTCBResExtract(wxMouseEvent event) {
}
void ThreadContentBar::OnHoverTCBNewThread(wxMouseEvent event) {
}
void ThreadContentBar::OnHoverTCBResponse(wxMouseEvent event) {
}
void ThreadContentBar::OnHoverTCBBookMark(wxMouseEvent event) {
}
void ThreadContentBar::OnHoverTCBDeleteLog(wxMouseEvent event) {
}
void ThreadContentBar::OnHoverTCBClose(wxMouseEvent event) {
}
void ThreadContentBar::OnHoverTCBNormalSearch(wxMouseEvent event) {
}
void ThreadContentBar::OnHoverTCBHideSearchBar(wxMouseEvent event) {
}
void ThreadContentBar::OnHoverTCBForward(wxMouseEvent event) {
}
void ThreadContentBar::OnHoverTCBBackward(wxMouseEvent event) {
}

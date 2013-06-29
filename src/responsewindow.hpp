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
#include <wx/html/htmlwin.h>
#include <wx/html/htmlwin.h>
#include <wx/html/winpars.h>
#include <wx/valgen.h>
#include "enums.hpp"
#include "socketcommunication.hpp"
#include "datatype.hpp"
#include "janecloneutil.hpp"
#include "wx/wxnkf.h"

/**
 * レス書き込み用の子ウィンドウ
 */
// begin wxGlade: ::extracode
// end wxGlade

/**

 書き込みの流れ

 1. COOKIEファイルがあるかどうか確認する(start)

 ・確認する要素
  ・COOKIE (日付等)
  ・HIDDEN (隠し要素１)
  ・PERN   (隠し要素２)

 ・システム上あり得る状態(これ以外の状態はイレギュラー)
 (a). 全てない                       (2)へ
 (b). COOKIEとHIDDENのみある         (3)へ
 (c). COOKIE, HIDDEN, PERNすべてある (4)へ

 2. 初回書き込み用電文を投げてCOOKIEとHIDDENをもらう (1)へ

 3. 書き込み同意画面にて「同意する」をクリックして書き込み成功, PERNをもらう(end)

 4. COOKIE, HIDDEN, PERNを添えて書き込み成功(end)

 */

class ResponseWindow: public wxDialog {

public:
     // begin wxGlade: ResponseWindow::ids
     // end wxGlade
     ResponseWindow(wxWindow* parent, wxString& title, URLvsBoardName& boardInfoHash,
		    ThreadInfo& threadInfoHash, wxPoint& point, wxTextCtrl* logCtrl);
     /**
      * ログ出力用のウィンドウを設定する
      */
     void SetLogWindow(wxTextCtrl* logCtrl) {
	  this->m_logCtrl = logCtrl;
     };

private:
     // begin wxGlade: ResponseWindow::methods
     void set_properties(const wxString& title);
     void do_layout();
     // end wxGlade
     
     // レスの投稿
     void OnPostResponse(wxCommandEvent &event);
     void QuitResponseWindow(wxCommandEvent &event);
     // 投稿内容の確認ボタン
     void PostConfirmForm(wxCommandEvent &event);
     // クッキーの状態チェック
     int CheckCookie();

     void PostFirstResponse(SocketCommunication* sock);
     
     void PostConfirm(SocketCommunication* sock);
     
     void PostResponse(SocketCommunication* sock);
     // 2chのサーバから受け取ったHTMLファイルを読み取りwxStringに格納する
     void ReadResponseHtml(const wxString& resultHtmlPath, wxString& htmlSource);

     // レス投稿ウィンドウのモード設定
     bool f_threadPost;

     // 内部で保持するスレッドや板情報
     URLvsBoardName  m_boardInfo;
     ThreadInfo      m_threadInfo;
     /**
      * ログとして出力するためのテキストコントロールのポインタ
      */
     wxTextCtrl* m_logCtrl;
     /**
      * 投稿内容を保持するオブジェクトのインスタンス
      */
     PostContent* m_postContent;


     /** 定数化されたページ数 */
#define KAKIKO_PAGE     1
#define PREVIEW_PAGE    2
#define LOCAL_RULE_PAGE 3
#define SETTING_PAGE    4

protected:
     // begin wxGlade: ResponseWindow::attributes
     wxStaticText* boardNameTitle;
     wxStaticText* name;
     wxComboBox* nameCombo;
     wxStaticText* mail;
     wxComboBox* mailCombo;
     wxCheckBox* sageCheck;
     wxTextCtrl* text_ctrl_1;
     wxPanel* resPane;
     wxHtmlWindow* previewWindow;
     wxPanel* previewPane;
     wxHtmlWindow* localRuleWindow;
     wxPanel* localRulePane;
     wxTextCtrl* settingText;
     wxPanel* settingPane;
     wxNotebook* resNoteBook;
     wxCheckBox* handleCheck;
     wxCheckBox* frontCheck;
     wxCheckBox* formatCheck;
     wxCheckBox* beCheck;
     wxComboBox* beMailCombo;
     wxCheckBox* proxyCheck;
     wxComboBox* proxyCombo;
     wxPanel* space;
     wxButton* postButton;
     wxButton* quitButton;
     // end wxGlade
     DECLARE_EVENT_TABLE()
}; // wxGlade: end class

#endif /* RESPONSEWINDOW_HPP_ */

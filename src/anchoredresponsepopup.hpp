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

#ifndef ANCHOREDRESPONSEPOPUP_HPP_
#define ANCHOREDRESPONSEPOPUP_HPP_

#include <wx/msgdlg.h>
#include <wx/html/htmlwin.h>
#include <wx/scrolwin.h>
#include <wx/popupwin.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/stattext.h>
#include <wx/dir.h>
#include <wx/fileconf.h>
#include "datatype.hpp"
#include "janecloneutil.hpp"

class AnchoredResponsePopup: public wxPopupTransientWindow {

public:
     /**
      * ポップアップ用ウィンドウのコンストラクタ
      *
      * @param wxWindow* parent     親ウィンドウのインスタンス
      * @param wxPoint&  point      ウィンドウの出現位置
      * @param wxSize    size       ウィンドウのサイズ
      * @raram wxString& htmlSource 表示させるHTMLソース
      */
     AnchoredResponsePopup( wxWindow *parent, wxPoint& point, wxSize size, wxString& htmlSource );
     virtual ~AnchoredResponsePopup(){};

     void Popup(wxWindow *focus = NULL);
     void OnDismiss();
     bool ProcessLeftDown(wxMouseEvent& event);
     bool Show( bool show = true );
     wxSize GetPopupWindowSize();

private:
     void EnterWindow(wxMouseEvent &event);
     void LeaveWindow(wxMouseEvent &event);
     void OnSize( wxSizeEvent &event );
     void OnSetFocus( wxFocusEvent &event );
     void OnKillFocus( wxFocusEvent &event );
     // スキン用のファイルが有るかどうか確認する
     bool CheckSkinFiles(SkinInfo* skin);
     // 指定されたファイル中のテキストをメモリに展開する
     wxString ReadPlainTextFile(const wxString& filePath);

     // ユーザーがリンクをクリックした場合ブラウザでジャンプ
     void OnLinkClocked(wxHtmlLinkEvent& event)
	  {
	       const wxHtmlLinkInfo linkInfo = event.GetLinkInfo();
	       const wxString href = linkInfo.GetHref();
	       wxLaunchDefaultBrowser(href);
	  };


private:
     wxHtmlWindow *htmlWin;

     DECLARE_CLASS(AnchoredResponsePopup)
     DECLARE_EVENT_TABLE()
};

#endif /* ANCHOREDRESPONSEPOPUP_HPP_ */

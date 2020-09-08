/* JaneClone - a text board site viewer for 2ch
 * Copyright (C) 2012-2014 Hiroyuki Nagata
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

#ifndef THREADCONTENTWEBVIEW_HPP_
#define THREADCONTENTWEBVIEW_HPP_

#include <wx/html/htmlwin.h>
#include <wx/html/htmldefs.h>
#include <wx/event.h>
#include <wx/clipbrd.h>
#include <wx/dataobj.h>
#include <wx/image.h>
#include <wx/webview.h>
#include "enums.hpp"
#include "datatype.hpp"
#include "janecloneutil.hpp"
#include "janecloneuiutil.hpp"
#include "janecloneimageviewer.hpp"
#include "socketcommunication.hpp"

class ThreadContentWebView : public wxHtmlWindow {

public:
     /**
      * 通常のコンストラクタ
      */
     ThreadContentWebView(wxWindow* parent, const wxString& threadContentPath);
     /**
      * オペレーターに対する参照返し
      */
     ThreadContentWebView& operator=(const ThreadContentWebView&) {return *this;}
     /**
      * ウィンドウ上で右クリックした場合の処理
      */
     void OnRightClickHtmlWindow(wxMouseEvent& event);
     /**
      * 現在開いているHTMLのスクロール位置を取得する
      */
     void GetHtmlWindowScrollPos(wxPoint* p);
     /**
      * 外のクラスから強制的にスクロールさせる
      */
     void ForceScroll(const wxPoint* p);
     /**
      * このクラスが保持するスレッドのレス数を返す
      */
     int GetCurrentThreadResponseNum() {
	  return m_response_number;
     };

private:
     /**
      * 指定されたパスからHTMLファイルを読み出し、2ch形式に加工する
      */
     const wxString GetConvertedDatFile(const wxString& threadContentPath);
     /**
      * メインのスレッドにログとイベントを送る
      */
     void SendLogging(wxString& message) {
	  JaneCloneUiUtil::SendLoggingHelper(message);
     };

     // linkを左クリックした時に起こるイベント
     void OnLeftClickHtmlWindow(wxHtmlLinkEvent& event);
     // リンクが2chのものかどうか判定
     void OnClickOrdinaryLink(const wxString& link);
     // 画像ビューアの状態を確認し、設定する
     void SetJaneCloneImageViewer(const wxString& href, const wxString& ext);
     // リサイズ時のイベント
     void OnSize(wxSizeEvent& event);
     // スキン用のファイルが有るかどうか確認する
     bool CheckSkinFiles(SkinInfo* skin);
     // 指定されたファイル中のテキストをメモリに展開する
     wxString ReadPlainTextFile(const wxString& filePath);

private:
     // 内部でもつリンク情報
     wxHtmlLinkInfo* m_linkInfo;
     // wxWebViewのインスタンス
     wxWebView* m_browser;
     // 内部でもつテキスト情報
     wxString m_selectedText;
     // 内部でもつHTML情報
     wxString m_htmlSource;
     // リサイズ前のスクロール座標
     int m_x, m_y;
     // このクラスが保持するスレッドのレス数
     int m_response_number;
     // 書き込みウィンドウに渡すレス番号
     long m_response;
     // スクロールさせる必要があるかどうか
     bool fNeedScroll;
};

#endif /* THREADCONTENTWEBVIEW_HPP_ */

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

#ifndef THREADCONTENTWINDOW_HPP_
#define THREADCONTENTWINDOW_HPP_

#include <wx/html/htmlwin.h>
#include <wx/html/htmldefs.h>
#include <wx/event.h>
#include <wx/clipbrd.h>
#include <wx/dataobj.h>
#include <wx/image.h>
#include "enums.hpp"
#include "datatype.hpp"
#include "janecloneutil.hpp"
#include "janecloneuiutil.hpp"
#include "janecloneimageviewer.hpp"
#include "socketcommunication.hpp"



class HtmlSourceDialog : public wxDialog
{
public:
    HtmlSourceDialog(const wxString& html);

};

class ThreadContentWindow : public wxHtmlWindow
{

public:
    /**
     * 通常のコンストラクタ
     */
    ThreadContentWindow(wxWindow* parent, const wxString& threadContentPath);
    /**
     * Hash用のコンストラクタ
     */
    ThreadContentWindow() {}
    ThreadContentWindow(const ThreadContentWindow&) {}
    /**
     * オペレーターに対する参照返し
     */
    ThreadContentWindow& operator=(const ThreadContentWindow&) {return *this;}
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
     * HTMLソースを取得する
     */
    const wxString GetThreadRawHTML() { return m_htmlSource;};
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

    // HtmlWindowで選択しているテキストをクリップボードにコピーする
    void CopyFromHtmlWindow(wxCommandEvent& event);
    // HtmlWindowで選択しているURLをクリップボードにコピーする
    void CopyURLFromHtmlWindow(wxCommandEvent& event);
    // HTMLソースを表示する
    void ShowRawHtmlFromHtmlWindow(wxCommandEvent& event);
    // HtmlWindowでテキストを全て選択する
    void SelectAllTextHtmlWindow(wxCommandEvent& event);
    // 選択したテキストでヤフー検索
    void SearchSelectWordByYahoo(wxCommandEvent& event);
    // 選択したテキストでGoogle検索
    void SearchSelectWordByGoogle(wxCommandEvent& event);
    // 選択したテキストでAmazon検索
    void SearchSelectWordByAmazon(wxCommandEvent& event);
    // 選択したテキストでスレタイ検索
    void SearchThreadBySelectWord(wxCommandEvent& event);
    // HTMLのデバッグ用イベント
    void HtmlSourceDebug(wxCommandEvent& event);
    // linkを左クリックした時に起こるイベント
    void OnLeftClickHtmlWindow(wxHtmlLinkEvent& event);
    // レス番号を左クリックした時に起こるイベント
    void OnLeftClickResponseNumber(wxHtmlLinkEvent& event, const wxString& href, const long res);
    // リンクが2chのものかどうか判定
    void OnClickOrdinaryLink(const wxString& link);
    // レス番号を指定して書き込みウィンドウを開く
    void CallResponseWindowWithAnchor(wxCommandEvent& event);
    // レス番号を指定して書き込みウィンドウを開く
    void CallResponseWindowWithQuote(wxCommandEvent& event);
    // 画像ビューアの状態を確認し、設定する
    void SetJaneCloneImageViewer(const wxString& href, const wxString& ext);
    // レスの内容をクリップボードにコピーする
    void CopyTContentsToClipBoard(wxCommandEvent& event);
    // 指定されたレスの内容をすべてクリップボードにコピーする
    void CopyTAllToClipBoard(wxCommandEvent& event);
    // リサイズ時のイベント
    void OnSize(wxSizeEvent& event);
    // スキン用のファイルが有るかどうか確認する
    bool CheckSkinFiles(SkinInfo* skin);
    // 指定されたファイル中のテキストをメモリに展開する
    wxString ReadPlainTextFile(const wxString& filePath);

    DECLARE_EVENT_TABLE()
    DECLARE_DYNAMIC_CLASS(ThreadContentWindow)

    private:
    // 内部でもつリンク情報
    wxHtmlLinkInfo* m_linkInfo;
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

protected:

};

#endif /* THREADCONTENTWINDOW_HPP_ */

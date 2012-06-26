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

#ifndef THREADCONTENTWINDOW_H_
#define THREADCONTENTWINDOW_H_

#include <wx/html/htmlwin.h>
#include <wx/regex.h>
#include "JaneCloneUtil.h"

// 読み込みに失敗した場合に表示するページ
static const wxString FAIL_TO_READ_PAGE =
		wxT("<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; \
			charset=UTF-8\"><title></title></head><body><span>ファイルの読み込みに失敗しました・リロードしてみてください</span></body></html>");

// ヘッダ部分にあたるHTML
static const wxString HTML_HEADER =
		wxT("<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; \
		charset=UTF-8\"></head><body bgcolor=#efefef text=black link=blue alink=red vlink=#660099>");

// フッター部分にあたるHTML
static const wxString HTML_FOOTER = wxT("</body></html>");

// スレッド読み込み用正規表現
static const wxRegEx regexThread(_T("^(.+)<>(.*)<>(.+)<>(.*)<>$"), wxRE_ADVANCED + wxRE_ICASE);

class WXDLLEXPORT ThreadContentWindow : public wxHtmlWindow {

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
	 * HTMLのセルがクリックされた時に起こるイベント
	 */
	bool OnCellClicked(wxHtmlCell *cell, wxCoord x, wxCoord y, const wxMouseEvent& event);
	/**
	 * HTMLのセル上でカーソルが動いた時に起こるイベント
	 */
	void OnCellMouseHover(wxHtmlCell *cell, wxCoord x, wxCoord y);
	/**
	 * URLが開かれた時に呼ばれるメソッド
	 */
	wxHtmlOpeningStatus OnOpeningURL(wxHtmlURLType type,const wxString& url, wxString *redirect);
	/**
	 * <TITLE>タグを読み込んだ時に呼ばれる
	 */
	void OnSetTitle(const wxString& title);
	/**
	 * カスタム設定を読み込むメソッド
	 */
	void ReadCustomization(wxConfigBase *cfg, wxString);
	/**
	 * カスタム設定を書き込むメソッド
	 */
	void WriteCustomization(wxConfigBase *cfg, wxString);

private:
	/**
	 * 指定されたパスからHTMLファイルを読み出し、2ch形式に加工する
	 */
	const wxString GetConvertedDatFile(const wxString& threadContentPath);

	DECLARE_DYNAMIC_CLASS(ThreadContentWindow)
};

#endif /* THREADCONTENTWINDOW_H_ */

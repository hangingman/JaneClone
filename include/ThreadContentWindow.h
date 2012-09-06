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
#include <wx/event.h>
#include <wx/clipbrd.h>
#include <wx/dataobj.h>
#include "DataType.h"
#include "JaneCloneUtil.h"

// 読み込みに失敗した場合に表示するページ
static const wxString FAIL_TO_READ_PAGE =
		wxT("<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; \
			charset=UTF-8\"><title></title></head><body><span>ファイルの読み込みに失敗しました・リロードしてみてください</span></body></html>");

// enum
enum {
	ID_CopyFromHtmlWindow, 		// HtmlWindowで選択しているテキストをクリップボードにコピーする
	ID_CopyURLFromHtmlWindow, 	// HtmlWindowで選択しているURLをクリップボードにコピーする
	ID_SelectAllTextHtmlWindow 	// HtmlWindowでテキストを全て選択する
};

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
	 * 内部で持っているHTMLソースを返す
	 */
	wxString GetInternalHtmlSource();
	/**
	 * ウィンドウ上で右クリックした場合の処理
	 */
	void OnRightClickHtmlWindow(wxMouseEvent& event);

private:
	/**
	 * 指定されたパスからHTMLファイルを読み出し、2ch形式に加工する
	 */
	const wxString GetConvertedDatFile(const wxString& threadContentPath);
	/**
	 * スレッドの最初の行を処理するメソッド
	 */
	wxString ProcessFirstResponse(wxString& threadRecord);
	/**
	 * スレッドの１以降を処理するメソッド
	 */
	wxString ProcessRestResponse(wxString& threadRecord, int number);

	// HtmlWindowで選択しているテキストをクリップボードにコピーする
	void CopyFromHtmlWindow(wxCommandEvent& event);
	// HtmlWindowで選択しているURLをクリップボードにコピーする
	void CopyURLFromHtmlWindow(wxCommandEvent& event);
	// HtmlWindowでテキストを全て選択する
	void SelectAllTextHtmlWindow(wxCommandEvent& event);

	DECLARE_EVENT_TABLE()
	DECLARE_DYNAMIC_CLASS(ThreadContentWindow)
};

#endif /* THREADCONTENTWINDOW_H_ */

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

class WXDLLEXPORT ThreadContentWindow : public wxHtmlWindow {
public:
	/**
	 * 通常のコンストラクタ
	 */
	ThreadContentWindow(wxWindow* parent);
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

	DECLARE_DYNAMIC_CLASS(ThreadContentWindow)
};

#endif /* THREADCONTENTWINDOW_H_ */

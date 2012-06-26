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

#include "ThreadContentWindow.h"

IMPLEMENT_DYNAMIC_CLASS(ThreadContentWindow, wxHtmlWindow)

/**
 * 通常のコンストラクタ
 */
ThreadContentWindow::ThreadContentWindow(wxWindow* parent):
wxHtmlWindow(parent, wxID_ANY-1, wxDefaultPosition, wxDefaultSize, wxHW_DEFAULT_STYLE) {

}
/**
 * HTMLのセルがクリックされた時に起こるイベント
 */
bool ThreadContentWindow::OnCellClicked(wxHtmlCell *cell, wxCoord x, wxCoord y,
		const wxMouseEvent& event) {
}
/**
 * HTMLのセル上でカーソルが動いた時に起こるイベント
 */
void ThreadContentWindow::OnCellMouseHover(wxHtmlCell *cell, wxCoord x,
		wxCoord y) {

}
/**
 * URLが開かれた時に呼ばれるメソッド
 */
wxHtmlOpeningStatus ThreadContentWindow::OnOpeningURL(wxHtmlURLType type,
		const wxString& url, wxString *redirect) {
}
/**
 * <TITLE>タグを読み込んだ時に呼ばれる
 */
void ThreadContentWindow::OnSetTitle(const wxString& title) {

}
/**
 * カスタム設定を読み込むメソッド
 */
void ThreadContentWindow::ReadCustomization(wxConfigBase *cfg, wxString path) {

}
/**
 * カスタム設定を書き込むメソッド
 */
void ThreadContentWindow::WriteCustomization(wxConfigBase *cfg, wxString path) {

}

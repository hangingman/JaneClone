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
ThreadContentWindow::ThreadContentWindow(wxWindow* parent, const wxString& threadContentPath):
wxHtmlWindow(parent, wxID_ANY-1, wxDefaultPosition, wxDefaultSize, wxHW_SCROLLBAR_AUTO) {

	// 指定されたパスからHTMLファイルを読み出す
	const wxString htmlSource = GetConvertedDatFile(threadContentPath);
	// メモリに読み込んだHTMLを表示する
	this->SetPage(htmlSource);
}

/**
 * 指定されたパスからHTMLファイルを読み出し、2ch形式に加工する
 */
const wxString ThreadContentWindow::GetConvertedDatFile(
		const wxString& threadContentPath) {

	// wxStringにバッファするサイズを計測する
	size_t fileSize = JaneCloneUtil::GetFileSize(threadContentPath);

	if (fileSize == 0)
		// 読み込みに失敗した場合
		return FAIL_TO_READ_PAGE;

	// 取得サイズ分だけwxStringを確保する
	wxString htmlSource;
	htmlSource.Alloc(fileSize);
	htmlSource += HTML_HEADER;

	// テキストファイルの読み込み
	wxTextFile datfile;
	datfile.Open(threadContentPath, wxConvUTF8);
	wxString str;
	int number = 0;

	// スレッドのそれぞれの要素
	wxString default_nanashi;
	wxString mail;
	wxString day_and_ID;
	wxString res;

	// ファイルがオープンされているならば
	if (datfile.IsOpened()) {
		for (str = datfile.GetFirstLine(); !datfile.Eof();
				str = datfile.GetNextLine()) {

			// 正規表現のコンパイルにエラーがなければ
			if (regexThread.IsValid()) {
				// マッチさせる
				if (regexThread.Matches(str)) {
					// マッチさせたそれぞれの要素を取得する
					default_nanashi.Clear();
					default_nanashi = regexThread.GetMatch(str, 1);
					mail.Clear();
					mail = regexThread.GetMatch(str, 2);
					day_and_ID.Clear();
					day_and_ID = regexThread.GetMatch(str, 3);
					res.Clear();
					res = regexThread.GetMatch(str, 4);
				}
			}

			// ひとかたまりのHTMLソースにまとめる
			wxString lumpOfHTML = wxT("<dt>");
			lumpOfHTML += wxString::Format("%d", number);
			if ( mail != wxEmptyString ) {
				// もしメ欄になにか入っているならば
				lumpOfHTML += wxT(" ：<a href=\"mailto:");
				lumpOfHTML += mail;
				lumpOfHTML += wxT("\"><b>");
				lumpOfHTML += default_nanashi;
				lumpOfHTML += wxT("</b></a>");
				lumpOfHTML += day_and_ID;
				lumpOfHTML += wxT("<dd>");
				lumpOfHTML += res;
			} else {
				// 空の場合
				lumpOfHTML += wxT(" ：<font color=green><b>");
				lumpOfHTML += default_nanashi;
				lumpOfHTML += wxT("</b></font>");
				lumpOfHTML += day_and_ID;
				lumpOfHTML += wxT("<dd>");
				lumpOfHTML += res;
			}

			// HTMLソースを加える
			htmlSource += lumpOfHTML;

			number++;
		}
	}

	htmlSource += HTML_FOOTER;
	datfile.Close();

	return htmlSource;
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

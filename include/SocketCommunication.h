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

#ifndef SOCKETCOMMUNICATION_H_
#define SOCKETCOMMUNICATION_H_

#include "JaneCloneUtil.h"
#include <wx/wx.h>
#include <wx/filefn.h>
#include <wx/textfile.h>
#include <wx/protocol/http.h>
#include <wx/wfstream.h>
#include <wx/datstrm.h>

class SocketCommunication {

// テキストの終端文字が何で終わるのかを定義
#ifdef __WXMSW__
	#define TEXT_ENDLINE_TYPE wxTextFileType_Dos
#endif
#ifdef __WXGTK__
	#define TEXT_ENDLINE_TYPE wxTextFileType_Unix
#endif
#ifdef __WXMAC__)
	#define TEXT_ENDLINE_TYPE wxTextFileType_Mac
#endif

public:
	/**
	 * 板一覧ファイルをダウンロードしてくるメソッド 引数はサーバーのフルURL、サーバ名、板名、保存先
	 * もし前回通信した際のログが残っていれば更新の確認のみ行う
	 */
	int DownloadBoardList(const wxString outputPath, const wxString headerPath);
	/**
	 * スレッド一覧をダウンロードしてくるメソッド
	 * @param 板名,URL,サーバー名
	 * @return datファイル保存先
	 */
	wxString DownloadThreadList(const wxString& boardName,
			const wxString& boardURL, const wxString& boardNameAscii);
private:
	/**
	 * 前回の通信ログが存在すれば、最後に取得した日時を変数に格納する
	 */
	wxString CheckLastModifiedTime(const wxString headerPath);
	/**
	 * 新規に板一覧情報を取得しに行く
	 */
	int DownloadBoardListNew(const wxString outputPath,
			const wxString headerPath);
	/**
	 * 前回との差分を取得しに行く
	 */
	int DownloadBoardListMod(const wxString outputPath,
			const wxString headerPath);
	/**
	 * 新規にスレッド一覧をダウンロードしてくるメソッド
	 * @param 板名,URL,サーバー名
	 * @return 実行コード
	 */
	int DownloadThreadListNew(const wxString& gzipPath,
			const wxString& headerPath);
	/**
	 * 前回との差分のスレッド一覧をダウンロードしてくるメソッド
	 * @param 板名,URL,サーバー名
	 * @return 実行コード
	 */
	int DownloadThreadListMod(const wxString& gzipPath,
			const wxString& headerPath);
	/**
	 * 一時ファイルを消す
	 */
	void RemoveTmpFile(const wxString removeFile);
	/**
	 * HTTPヘッダを書きだす
	 */
	void WriteHeaderFile(const wxHTTP& http, const wxString& headerPath);
};

#endif /* SOCKETCOMMUNICATION_H_ */

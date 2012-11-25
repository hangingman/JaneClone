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

#include <wx/wx.h>
#include <wx/string.h>
#include <wx/filefn.h>
#include <wx/filename.h>
#include <wx/textfile.h>
#include <wx/protocol/http.h>
#include <wx/protocol/protocol.h>
#include <wx/wfstream.h>
#include <wx/datstrm.h>
#include <wx/regex.h>
#include <wx/dir.h>
#include <wx/utils.h>
#include <wx/config.h>
#include <wx/fileconf.h>
#include <wx/sstream.h>
#include <wx/sckstrm.h>
#include "janecloneutil.hpp"

class SocketCommunication {

     /**
      * テキストの終端文字が何で終わるのかを定義
      */
#ifdef __WXMSW__
#define TEXT_ENDLINE_TYPE wxTextFileType_Dos
#endif
#ifdef __WXGTK__
#define TEXT_ENDLINE_TYPE wxTextFileType_Unix
#endif
#ifdef __WXMAC__
#define TEXT_ENDLINE_TYPE wxTextFileType_Mac
#endif

     // クッキーの設定ファイル
#define COOKIE_CONFIG_FILE wxT("cookie.env")

public:


     /**
      * 板一覧ファイルをダウンロードしてくるメソッド
      * もし前回通信した際のログが残っていれば更新の確認のみ行う
      * @param  板一覧datファイル保存先
      * @param  板一覧headerファイル保存先
      * @return 実行コード
      */
     int DownloadBoardList(const wxString outputPath, const wxString headerPath);
     /**
      * スレッド一覧をダウンロードしてくるメソッド
      * @param 板名
      * @param URL
      * @param サーバー名
      * @return ダウンロードしたdatファイル保存先
      */
     wxString DownloadThreadList(const wxString boardName,
				 const wxString boardURL, const wxString boardNameAscii);
     /**
      * スレッドのデータをダウンロードしてくるメソッド
      * @param 板名
      * @param URL
      * @param サーバー名
      * @param 固有番号
      * @return ダウンロードしたdatファイル保存先
      */
     wxString DownloadThread(const wxString boardName, const wxString boardURL,
			     const wxString boardNameAscii, const wxString origNumber);

     /**
      * スレッドに書き込みを行うメソッド
      * @param 板名,URL,サーバー名
      * @return 書き込み結果
      */
     wxString PostToThread(URLvsBoardName& boardInfoHash, ThreadInfo& threadInfoHash);
     /**
      * 投稿内容をソケット通信クラスに設定する
      * @param PostContent構造体
      */
     void SetPostContent(PostContent* postContent);
     /**
      * ログ出力用のウィンドウを設定する
      */
     void SetLogWindow(wxTextCtrl* logCtrl) {
	  this->m_logCtrl = logCtrl;
     };

private:
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
      * @param gzipのダウンロード先パス
      * @param HTTPヘッダのダウンロード先パス
      * @param 板名（ascii）
      * @return 実行コード
      */
     int DownloadThreadListNew(const wxString gzipPath,
			       const wxString headerPath, const wxString boardNameAscii,
			       const wxString hostName, const wxString boardURL);
     /**
      * 前回との差分のスレッド一覧をダウンロードしてくるメソッド
      * @param gzipのダウンロード先パス
      * @param HTTPヘッダのダウンロード先パス
      * @param 板名（ascii）
      * @return 実行コード
      */
     int DownloadThreadListMod(const wxString gzipPath,
			       const wxString headerPath, const wxString boardNameAscii,
			       const wxString hostName, const wxString boardURL);

     /**
      * 新規にスレッドのデータをダウンロードしてくるメソッド
      * @param gzipのダウンロード先パス
      * @param HTTPヘッダのダウンロード先パス
      * @param 板名（ascii）
      * @param 固有番号
      * @param サーバーのホスト名
      * @return 実行コード
      */
     int DownloadThreadNew(const wxString gzipPath, const wxString headerPath,
			   const wxString boardNameAscii, const wxString origNumber,
			   const wxString hostName);

     /**
      * 前回との差分のスレッドのデータをダウンロードしてくるメソッド
      * @param gzipのダウンロード先パス
      * @param HTTPヘッダのダウンロード先パス
      * @param 板名（ascii）
      * @param 固有番号
      * @param サーバーのホスト名
      * @return 実行コード
      */
     int DownloadThreadMod(const wxString gzipPath, const wxString headerPath,
			   const wxString boardNameAscii, const wxString origNumber,
			   const wxString hostName);

     /**
      * 初回のクッキー受け取りと確認用ポスト
      */
     wxString PostToThreadFirst(const wxString hostName, URLvsBoardName& boardInfoHash, ThreadInfo& threadInfoHas);
     /**
      * ２回目以降の書き込みメソッド
      */
     wxString PostToThreadRest(const wxString hostName, URLvsBoardName& boardInfoHash, ThreadInfo& threadInfoHas);
     
     /**
      * 通信ログに残っているHTTPレスポンスコードを取得する
      */
     wxString GetHTTPResponseCode(const wxString headerPath,
				  const wxString reqCode);
     /**
      * 一時ファイルを消す
      */
     void RemoveTmpFile(const wxString removeFile);
     /**
      * HTTPヘッダを書きだす
      */
     void WriteHeaderFile(wxHTTP& http, const wxString headerPath);
     /**
      * COOKIE関連の初期化処理を行う
      */
     bool InitializeCookie();
     /**
      * ログとして出力するためのテキストコントロールのポインタ
      */
     wxTextCtrl* m_logCtrl;

     // wxFileConfigクラスのインスタンス(書き込みの際のみ確保される)
     wxFileConfig* config;
     // 投稿内容を保存するクラス(書き込みの際のみ確保される)
     PostContent* postContent;
};

#endif /* SOCKETCOMMUNICATION_H_ */

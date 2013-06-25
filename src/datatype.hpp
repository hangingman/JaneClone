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

#ifndef DATATYPE_HPP_
#define DATATYPE_HPP_

#ifdef HAVE_CONFIG_H
   #include "config.h"
#endif

#include <wx/wx.h>
#include <vector>
#include <map>
#include <wx/regex.h>


// マクロ置換用マクロ
#define XSTR(x) #x
#define STR(x)  XSTR(x)

/*
 * 定数値の宣言
 */
/** JaneClone公式サイトのURL */
#define JANECLONE_DOWNLOADSITE wxT("http://hiroyuki-nagata.github.io/")
/** JaneCloneの作業用隠しフォルダ名 */
#define JANECLONE_DIR wxT(".jc")
/** コンフィグファイル名 */
#define APP_CONFIG_FILE wxT("janeclone.env")
/** クッキーの設定ファイル */
#define COOKIE_CONFIG_FILE wxT("janeclone.env")

// ファイルの区切り文字
#ifdef __WXMSW__
  #define wxFileSeparator wxT("\\") 
#else
  #define wxFileSeparator wxT("/") 
#endif

// 実行ファイルの拡張子
#ifdef __WXMSW__
  #define wxExt wxT(".exe") 
#else
  #define wxExt wxT("") 
#endif

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

     /** 定数化された書き込み前のCOOKIEの状態 */
#define NO_COOKIE         10
#define HAS_COOKIE_HIDDEN 11
#define HAS_PERN          12

// 板一覧情報ファイルのパス
#define BOARD_LIST_PATH ::wxGetHomeDir() + wxFileSeparator + JANECLONE_DIR + wxFileSeparator + wxT("dat") + wxFileSeparator + wxT("boardlist.html")
// 板一覧情報ファイルのヘッダ情報のパス
#define BOARD_LIST_HEADER_PATH ::wxGetHomeDir() + wxFileSeparator + JANECLONE_DIR + wxFileSeparator + wxT("dat") + wxFileSeparator + wxT("boardlistheader.html")

/** 各ウィジェットの名前を表す定数値 */
#define JANECLONE_WINDOW       wxT("janeclone_window")
#define SEARCH_BAR             wxT("m_search_ctrl")
#define SEARCH_BOX             wxT("search_box")
#define URL_BAR                wxT("m_url_input_panel")
#define BOARD_TREE             wxT("m_tree_ctrl")
#define BOARD_TREE_PANEL       wxT("m_tree_panel")
#define SHINGETU_NODE_TREE     wxT("m_shingetsu_tree_ctrl")
#define SHINGETU_NODE_PANEL    wxT("m_shingetsu_tree_panel")
#define BOARD_TREE_SEARCH      wxT("board_tree_search")
#define THREADLIST_SEARCH      wxT("thread_list_search")
#define SHINGETU_NODE_SEARCH   wxT("m_shingetsu_tree_search")
#define LOG_WINDOW             wxT("m_logCtrl")
#define BOARD_NOTEBOOK         wxT("boardNoteBook")
#define THREAD_NOTEBOOK        wxT("threadNoteBook")

// ライセンスのデフォルトのパス
static const wxString licencePath        = wxT("rc/gpl-2.0.txt");

// ユーザーエージェント
static const std::string userAgent       = std::string("Monazilla/1.00 JaneClone(") + std::string(STR(PACKAGE_VERSION)) + std::string(")");

// バージョン
static const wxString janecloneVersion   = wxT(STR(PACKAGE_VERSION));

// 各ボタンに貼り付けられる画像
static const wxString autoReloadImg      = wxT("rc/appointment-new.png");
static const wxString redResExtractImg   = wxT("rc/system-search.png");
static const wxString refreshImg         = wxT("rc/view-refresh.png");
static const wxString scrollToNewResImg  = wxT("rc/go-bottom.png");
static const wxString stopImg            = wxT("rc/dialog-error.png");
static const wxString resExtractImg      = wxT("rc/edit-find-replace.png");
static const wxString newThreadImg       = wxT("rc/go-last.png");
static const wxString responseImg        = wxT("rc/page-edit.png");
static const wxString bookMarkImg        = wxT("rc/bookmark-new.png");
static const wxString deleteLogImg       = wxT("rc/edit-delete.png");
static const wxString closeImg           = wxT("rc/emblem-unreadable.png");
static const wxString normalSearchImg    = wxT("rc/system-search.png");
static const wxString hideSearchBarImg   = wxT("rc/emblem-unreadable.png");
static const wxString forwardImg         = wxT("rc/go-down.png");
static const wxString backwardImg        = wxT("rc/go-up.png");
static const wxString regexImg           = wxT("rc/emblem-regex.png");
// ツールバー用アイコン
static const wxString sideTreeImg        = wxT("rc/application-side-tree.png");
static const wxString twoPaneWinImg      = wxT("rc/application-tile-horizontal.png");
static const wxString thrPaneWinImg      = wxT("rc/view-choose.png");
static const wxString thrColumnWinImg    = wxT("rc/view-file-columns.png");
static const wxString configImg          = wxT("rc/configure-2.png");
static const wxString logSearchImg       = wxT("rc/system-search-5.png");
static const wxString helpImg            = wxT("rc/help-browser.png");
// スレッド一覧リスト用画像
static const wxString threadCheckImg     = wxT("rc/dialog-accept.png");
static const wxString threadAddImg       = wxT("rc/edit-add-3.png");
static const wxString threadDropImg      = wxT("rc/go-down-4.png");
static const wxString threadNewImg       = wxT("rc/download-3.png");

/**
 * クラス
 */

// 2chの板名とURLを対応させるクラス
class URLvsBoardName {
public:
     wxString boardName;
     wxString boardURL;
     wxString boardNameAscii;
};

// スレッド一覧用のクラス
class ThreadList {
public:
     // キー値(ホスト名にこの番号をつけることでスレッドのURLになる)
     wxString oid;
     // 番号(単に取得したdatファイルの順序から)
     int number;
     // タイトル
     wxString title;
     // レス
     int response;
     // 取得
     int cachedResponseNumber;
     // 新着
     int newResponseNumber;
     // 増レス
     int increaseResponseNumber;
     // 勢い
     wxString momentum;
     // 最終取得
     wxString lastUpdate;
     // since
     wxString since;
     // 板
     wxString boardName;
};

// 2chの板名(ascii)と固有番号を対応させて保存するクラス
class ThreadInfo {
public:
     // スレッドタイトル
     wxString title;
     // スレッド固有番号
     wxString origNumber;
     // 板名(英数)
     wxString boardNameAscii;
};

/**
 * 構造体
 */

// 書き込みを行う際の投稿内容
typedef struct {
     wxString name;
     wxString mail;
     wxString kakikomi;
} PostContent;

// 画像をダウンロードした後の結果
typedef struct {
     wxString imagePath; // 画像ファイルのパス
     wxString imageURL;  // 画像ファイルのURL
     wxString ext;       // 画像ファイルの拡張子
     bool     result;    // 取得の成否
} DownloadImageResult;

// URIを分解した時の各要素
typedef struct {
     wxString protocol;
     wxString hostname;
     wxString port;
     wxString path;
} PartOfURI;

/**
 * 変数
 */

// ヘッダ部分にあたるHTML
static const wxString HTML_HEADER =
		wxT("<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"></head><body bgcolor=#efefef text=black link=blue alink=red vlink=#660099>");

// ポップアップウィンドウのヘッダ部分にあたるHTML
static const wxString HTML_HEADER_POPUP =
		wxT("<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"></head><body bgcolor=#eedcb3 text=black link=blue alink=red vlink=#660099>");

// 投稿失敗時のHTML
static const wxString FAIL_TO_POST = wxT("<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; \
			charset=UTF-8\"><title></title></head><body><span>レスの投稿に失敗しました</span></body></html>");

// フッター部分にあたるHTML
static const wxString HTML_FOOTER = wxT("</body></html>");

// スレッドの１番目を読み込む正規表現
static const wxRegEx regexThreadFst(_T("^(.+)<>(.*)<>(.+)<>(.*)<>(.+)"), wxRE_ADVANCED + wxRE_ICASE);

// スレッド読み込み用正規表現
static const wxRegEx regexThread(_T("^(.+)<>(.*)<>(.+)<>(.*)<>$"), wxRE_ADVANCED + wxRE_ICASE);

// URL検出用正規表現
static const wxRegEx regexURL(_T("(http|https|ttp|ftp)://([[:alnum:]]|[[:punct:]]|[=]|[~]|[+])*"), wxRE_ADVANCED + wxRE_ICASE);

// 画像リンク検出用正規表現
static const wxRegEx regexImage(_T("(http|https|ttp|ftp)://([[:alnum:]]|[[:punct:]]|[=]|[~]|[+])*.(jpg|jpeg|png|gif|bmp)"), wxRE_ADVANCED + wxRE_ICASE);

// URIマッチング用正規表現
static const wxRegEx regexURI(_T("(http|https|ttp|ftp)://([-0-9a-zA-Z\\._]*)(:[0-9]+)?([-/\\.a-zA-Z0-9_#~:.?+=&%!@]*)"), wxRE_ADVANCED + wxRE_ICASE);

#endif /* DATATYPE_HPP_ */

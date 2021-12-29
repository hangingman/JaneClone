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

#ifndef DATATYPE_HPP_
#define DATATYPE_HPP_

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include <wx/wx.h>
#include <vector>
#include <map>
#include <memory>
#include <wx/regex.h>
#include <wx/hashmap.h>
#include <wx/filefn.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>

// マクロ置換用マクロ
#define XSTR(x) #x
#define STR(x)  XSTR(x)

/*
 * 定数値の宣言
 */
/** JaneClone公式サイトのURL */
#define JANECLONE_DOWNLOADSITE wxT("http://hangingman.github.io/JaneClone/")
/** ●公式サイトのURL */
#define IICH_VIEWER_OFFICIAL   wxT("http://2ch.tora3.net/")
/** JaneCloneの作業用隠しフォルダ名 */
#define JANECLONE_DIR ".jc"
/** コンフィグファイル名 */
#define APP_CONFIG_FILE wxT("janeclone.env")
/** クッキーの設定ファイル */
#define COOKIE_CONFIG_FILE wxT("janeclone.env")
/** Mac OSX のアプリケーションバンドル構造 */
#define MAC_OSX_CURDIR_PREFIX wxGetCwd() + wxT("/JaneClone.app/Contents/MacOS/")
/** HTML中のタグ */
#define BR "<br/>"

// 実行ファイルの拡張子
#ifdef __WXMSW__
#  define wxExt wxT(".exe")
#else
#  define wxExt wxT("")
#endif

/**
 * テキストの終端文字が何で終わるのかを定義
 */
#ifdef __WXMSW__
#  define TEXT_ENDLINE_TYPE wxTextFileType_Dos
#endif
#ifdef __WXGTK__
#  define TEXT_ENDLINE_TYPE wxTextFileType_Unix
#endif
#ifdef __WXMAC__
#  define TEXT_ENDLINE_TYPE wxTextFileType_Mac
#endif

/** 定数化された書き込み前のCOOKIEの状態 */
#define NO_COOKIE         10
#define HAS_COOKIE_HIDDEN 11
#define HAS_PREN          12

/** 定数化されたdatファイルの種別 */
#define KIND_THREAD_DAT   10
#define KIND_BOARD__DAT   11
#define KIND_DAT_PATH     12

// 板一覧情報ファイルのパス
#define BOARD_LIST_PATH ::wxGetHomeDir() + wxFILE_SEP_PATH + JANECLONE_DIR + wxFILE_SEP_PATH + wxT("dat") + wxFILE_SEP_PATH + wxT("boardlist.html")
// 板一覧情報ファイルのヘッダ情報のパス
#define BOARD_LIST_HEADER_PATH ::wxGetHomeDir() + wxFILE_SEP_PATH + JANECLONE_DIR + wxFILE_SEP_PATH + wxT("dat") + wxFILE_SEP_PATH + wxT("boardlistheader.html")

/** 各ウィジェットの名前を表す定数値 */
#define JANECLONE_WINDOW       wxT("janeclone_window")
#define SEARCH_BAR             wxT("m_search_ctrl")
#define SEARCH_BOX             wxT("search_box")
#define URL_BAR                wxT("m_url_input_panel")
#define BOARD_TREE             wxT("m_tree_ctrl")
#define SHINGETU_NODE_TREE     wxT("m_shingetsu_tree_ctrl")
#define SHINGETU_NODE_PANEL    wxT("m_shingetsu_tree_panel")
#define BOARD_TREE_SEARCH      wxT("board_tree_search")
#define THREADLIST_SEARCH      wxT("thread_list_search")
#define FAVS_TREE              wxT("favs_tree")
#define NOW_READ_TREE          wxT("now_read_tree")
#define FAVS_TREE_SEARCH       wxT("favs_tree_search")
#define NOW_READ_TREE_SEARCH   wxT("now_read_tree_seach")
#define SHINGETU_NODE_SEARCH   wxT("m_shingetsu_tree_search")
#define LOG_WINDOW             wxT("m_logCtrl")
#define BOARD_NOTEBOOK         wxT("boardNoteBook")
#define THREAD_NOTEBOOK        wxT("threadNoteBook")
#define BOARD_TREE_NOTEBOOK    wxT("boardTreeNoteBook")

// ユーザーエージェント
#ifdef PACKAGE_VERSION
static const std::string userAgent = std::string("Monazilla/1.00 JaneClone(" PACKAGE_VERSION ")" );
#else
static const std::string userAgent = std::string("Monazilla/1.00 JaneClone(Unknown)");
#endif

// バージョン
static const wxString janecloneVersion = wxT( PACKAGE_VERSION );

// 実行されているバイナリのパス
inline wxString GetJcExecutablePath() {
    wxFileName f(wxStandardPaths::Get().GetExecutablePath());
    wxString appPath(f.GetPath());
    return appPath;
}

// パッケージでインストールされるパス
static const wxRegEx packageBinPath(_T("(/usr/bin|/usr/local/bin)"), wxRE_ADVANCED + wxRE_ICASE);

// リソースファイルのパス
inline wxString GetResourcePath(wxString fileName) {

#if defined(__WXGTK__)
    wxFileName exePath = wxFileName::DirName(GetJcExecutablePath());
    wxFileName filePath;

    if (packageBinPath.Matches(exePath.GetFullPath(wxPATH_UNIX))) {
        // 実行ファイルが /usr 以下 の場合パッケージビルドとみなす
        filePath = wxFileName::DirName("/usr/share/pixmaps/janeclone");
    } else {
        filePath = wxFileName::DirName(GetJcExecutablePath());
        filePath.AppendDir("rc");
    }

#elif defined (__WXMSW__)
    wxFileName filePath = wxFileName::DirName(GetJcExecutablePath());
    filePath.AppendDir("rc");
#elif defined(__WXMAC__)
    wxFileName exePath = wxFileName::DirName(GetJcExecutablePath());
    wxFileName filePath;

    if (exePath.GetFullPath().StartsWith("/Applications")) {
        // 実行ファイルが /Applications 以下 の場合パッケージビルドとみなす
        filePath = wxFileName::DirName("/Applications/JaneClone.app/Contents/Resources");
    } else {
        filePath = wxFileName::DirName(GetJcExecutablePath());
        filePath.AppendDir("rc");
    }

#endif

    if (fileName != wxEmptyString) {
        wxFileName f(fileName);
        filePath.SetName(f.GetName());
        filePath.SetExt(f.GetExt());
    }
    return filePath.GetFullPath();
}

// Javascriptのデフォルトのパス
static const wxString jsDir              = wxT("script/");
static const wxString jsPath             = wxT("script/embedded.js");

// ライセンスのデフォルトのパス
static const wxString licencePath        = GetResourcePath("gpl-2.0.txt");

// 各ボタンに貼り付けられる画像
static const wxString autoReloadImg      = GetResourcePath("appointment-new.png");
static const wxString redResExtractImg	 = GetResourcePath("system-search.png");
static const wxString refreshImg	 = GetResourcePath("view-refresh.png");
static const wxString scrollToNewResImg	 = GetResourcePath("go-bottom.png");
static const wxString stopImg		 = GetResourcePath("dialog-error.png");
static const wxString resExtractImg	 = GetResourcePath("edit-find-replace.png");
static const wxString newThreadImg	 = GetResourcePath("go-last.png");
static const wxString goNextImg		 = GetResourcePath("go-next.png");
static const wxString responseImg	 = GetResourcePath("page-edit.png");
static const wxString bookMarkImg	 = GetResourcePath("bookmark-new.png");
static const wxString deleteLogImg	 = GetResourcePath("edit-delete.png");
static const wxString closeImg		 = GetResourcePath("emblem-unreadable.png");
static const wxString normalSearchImg	 = GetResourcePath("system-search.png");
static const wxString hideSearchBarImg	 = GetResourcePath("emblem-unreadable.png");
static const wxString forwardImg	 = GetResourcePath("go-down.png");
static const wxString backwardImg	 = GetResourcePath("go-up.png");
static const wxString regexImg		 = GetResourcePath("emblem-regex.png");
// ツールバー用アイコン
static const wxString sideTreeImg        = GetResourcePath("application-side-tree.png");
static const wxString twoPaneWinImg	 = GetResourcePath("application-tile-horizontal.png");
static const wxString thrPaneWinImg	 = GetResourcePath("view-choose.png");
static const wxString thrColumnWinImg	 = GetResourcePath("view-file-columns.png");
static const wxString configImg		 = GetResourcePath("configure-2.png");
static const wxString logSearchImg	 = GetResourcePath("system-search-5.png");
static const wxString helpImg		 = GetResourcePath("help-browser.png");
// スレッド一覧リスト用画像
static const wxString threadCheckImg	 = GetResourcePath("dialog-accept.png");
static const wxString threadAddImg	 = GetResourcePath("edit-add-3.png");
static const wxString threadDropImg	 = GetResourcePath("go-down-4.png");
static const wxString threadNewImg	 = GetResourcePath("download-3.png");
// スレタブ用画像
static const wxString threadTabNewImg	 = GetResourcePath("document-new-7.png");
static const wxString threadTabAddImg	 = GetResourcePath("document-new-8.png");
static const wxString threadTabDrpImg	 = GetResourcePath("document-close-4.png");
// ツールバー用画像
static const wxString folderImg          = GetResourcePath("folder.png");
static const wxString textHtmlImg	 = GetResourcePath("text-html.png");
// スレッド上のデフォルトの画像アイコン
static const wxString defaultIconImg     = GetResourcePath("image-x-generic.png");
static const wxString yahooIconImg	 = GetResourcePath("yahoo.png");
static const wxString googleIconImg	 = GetResourcePath("google.png");
static const wxString amazonIconImg	 = GetResourcePath("amazon.png");


/**
 * 構造体
 */

// 2chの板名とURLを対応させるクラス
typedef struct
{
    wxString boardName;
    wxString boardURL;
    wxString boardNameAscii;
} URLvsBoardName;

// スレッド一覧用のクラス
typedef struct
{
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
} ThreadList;

// 2chの板名(ascii)と固有番号を対応させて保存するクラス
typedef struct
{
    // スレッドタイトル
    wxString title;
    // スレッド固有番号
    wxString origNumber;
    // 板名(英数)
    wxString boardNameAscii;
} ThreadInfo;

// 書き込みを行う際の投稿内容
typedef struct
{
    wxString name;
    wxString mail;
    wxString kakikomi;
} PostContent;

// 画像をダウンロードした後の結果
typedef struct
{
    wxString imagePath; // 画像ファイルのパス
    wxString imageURL;  // 画像ファイルのURL
    wxString ext;       // 画像ファイルの拡張子
    wxString fileName;  // 画像ファイル名      ex) xxx.jpg
    bool     result;    // 取得の成否
    ThreadInfo* threadInfo;// 取得元スレッド情報
} DownloadImageResult;

// URIを分解した時の各要素
typedef struct
{
    wxString protocol;
    wxString hostname;
    wxString port;
    wxString path;
} PartOfURI;

// スキン使用時の各種情報
typedef struct
{
    wxString footer; // Footer.html
    wxString header; // Header.html
    wxString newres; // NewRes.html
    wxString popup;  // PopupRes.html
    wxString res;    // Res.html
    wxArrayString jsPath; // Javascriptファイルのパス
} SkinInfo;

// ダウンロードした画像情報
typedef struct
{
    wxString fileName;     // URLを含んだファイル名    ex) http://hogehoge/donwload0x0x.jpg
    wxString uuidFileName; // UUIDを配られたファイル名 ex) XXXXXXXXXXXXXXXX.jpg
} ImageFileInfo;

// 板名とそのURLを保持するwxHashMap　JaneCloneが起動している間は保持される
// URLvsBoardNameのHashMap（板名をkeyとしてBoardURLとascii文字の固有名を持つ）
WX_DECLARE_HASH_MAP( wxString, // type of the keys
                     URLvsBoardName,      // type of the values
                     wxStringHash ,       // hasher
                     wxStringEqual,       // key equality predicate
                     NameURLHash);	       // name of the class

// ユーザーがタブに保持しているスレッドの情報を保存するHashSetの宣言
WX_DECLARE_HASH_MAP( wxString,        // type of the keys
                     ThreadInfo,      // 実体を詰める
                     wxStringHash ,   // hasher
                     wxStringEqual,   // key equality predicate
                     ThreadInfoHash); // name of the class


/**
 * 変数
 */

// ヘッダ部分にあたるHTML
static const wxString HTML_HEADER =
    wxT("<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"></head><body bgcolor=#efefef text=black link=blue alink=red vlink=#660099>");

// ヘッダ部分にあたるHTML
static const wxString CUSTOM_HTML_HEADER =
    wxT("<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">");

// ポップアップウィンドウのヘッダ部分にあたるHTML
static const wxString HTML_HEADER_POPUP =
    wxT("<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"></head><body bgcolor=#eedcb3 text=black link=blue alink=red vlink=#660099>");

// 投稿失敗時のHTML
static const wxString FAIL_TO_POST = wxT("<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; \
			charset=UTF-8\"><title></title></head><body><span>レスの投稿に失敗しました</span></body></html>");

// 読み込みに失敗した場合に表示するページ
static const wxString FAIL_TO_READ_PAGE =
    wxT("<html><head><meta http-equiv=\"Content-Type\" content=\"text/html;charset=UTF-8\"><title></title></head><body><span>ファイルの読み込みに失敗しました・リロードしてみてください</span></body></html>");

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

// HTMLタグマッチング用正規表現
static const wxRegEx regexHtmlTag(_T("<(\"[^\"]*\"|'[^']*'|[^'\">])*>"), wxRE_ADVANCED + wxRE_ICASE);

// レスアンカーマッチング用正規表現
static const wxRegEx regexResAnchor(_T(">>([[:digit:]]{1,4})"), wxRE_ADVANCED + wxRE_ICASE);

// 2chのIDマッチング用正規表現
static const wxRegEx regexID(_T("ID:(([-/\\.a-zA-Z0-9_#~:.?+=&%!@]){8,9})"), wxRE_ADVANCED + wxRE_ICASE);

// 2chのレス番号マッチング用正規表現
static const wxRegEx regexIndex(_T("<a href=\"#([[:digit:]]{1,4})\">([[:digit:]]{1,4})</a>"), wxRE_ADVANCED + wxRE_ICASE);

// emoji等タグマッチング用正規表現
static const wxRegEx regexUnicodeRef(_T("&#([[:digit:]]{1,7});"), wxRE_ADVANCED + wxRE_ICASE);


#endif /* DATATYPE_HPP_ */

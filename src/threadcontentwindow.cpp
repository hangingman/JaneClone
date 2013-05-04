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

#include "threadcontentwindow.hpp"
#include "janeclone.hpp"

IMPLEMENT_DYNAMIC_CLASS(ThreadContentWindow, wxHtmlWindow)
// event table
BEGIN_EVENT_TABLE(ThreadContentWindow, wxHtmlWindow)
// 右クリック時のイベント
EVT_RIGHT_DOWN(ThreadContentWindow::OnRightClickHtmlWindow)
// リンク押下時のイベント
EVT_HTML_LINK_CLICKED(wxID_ANY, ThreadContentWindow::OnLeftClickHtmlWindow)
// 右クリックメニューイベント
EVT_MENU(ID_CopyFromHtmlWindow, ThreadContentWindow::CopyFromHtmlWindow)
EVT_MENU(ID_CopyURLFromHtmlWindow, ThreadContentWindow::CopyURLFromHtmlWindow)
EVT_MENU(ID_SelectAllTextHtmlWindow, ThreadContentWindow::SelectAllTextHtmlWindow)
EVT_MENU(ID_SearchSelectWordByYahoo,ThreadContentWindow::SearchSelectWordByYahoo)
EVT_MENU(ID_SearchSelectWordByGoogle,ThreadContentWindow::SearchSelectWordByGoogle)
EVT_MENU(ID_SearchSelectWordByAmazon,ThreadContentWindow::SearchSelectWordByAmazon)
EVT_MENU(ID_SearchThreadBySelectWord,ThreadContentWindow::SearchThreadBySelectWord)
// リサイズがかかった際のイベント
EVT_SIZE(ThreadContentWindow::OnSize)

#ifdef DEBUG
// HTMLのデバッグ用イベント
EVT_MENU(ID_HtmlSourceDebug, ThreadContentWindow::HtmlSourceDebug)
#endif
END_EVENT_TABLE()

/**
 * 通常のコンストラクタ
 */
ThreadContentWindow::ThreadContentWindow(wxWindow* parent, const wxString& threadContentPath):
wxHtmlWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHW_SCROLLBAR_AUTO) {

     // 指定されたパスからHTMLファイルを読み出す
     wxString htmlSource = GetConvertedDatFile(threadContentPath);
     // 設定ファイルの準備をする
     wxString configFile = wxGetCwd() + wxFileSeparator + wxT("prop") + wxFileSeparator + APP_CONFIG_FILE;
     wxFileConfig* config = new wxFileConfig(wxT("JaneClone"), wxEmptyString, configFile, wxEmptyString, wxCONFIG_USE_LOCAL_FILE);

     // フォント設定を読み出し
     wxString fontName;
     config->Read(wxT("HTML.Font"), &fontName, wxEmptyString);
     int p, realSize;
     config->Read(wxT("HTML.PointSize"), &p, 0);
     delete config;
     // HTMLソース中のテキストの種類とサイズを決定する
     static int f_size[] = {p - 2, p - 1, p, p + 1, p + 2, p + 3, p + 4 };

     // 設定ファイルにフォントが設定されていなければSetFontsは実行しない
     if (fontName != wxEmptyString && p != 0) {
	  this->SetFonts(fontName, wxEmptyString, f_size);
     }
     // メモリに読み込んだHTMLを表示する
     this->SetPage(htmlSource);
     // 外部から参照可能なHTML
     this->m_htmlSource = htmlSource;
     // スクロールのフラグ
     fNeedScroll = false;
}
/**
 * 指定されたパスからHTMLファイルを読み出し、2ch形式に加工する
 */
const wxString ThreadContentWindow::GetConvertedDatFile(const wxString& threadContentPath) {

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
     int number = 1;

     // ファイルがオープンされているならば
     if (datfile.IsOpened()) {
	  for (str = datfile.GetFirstLine(); !datfile.Eof();
	       str = datfile.GetNextLine()) {

	       if (str.IsNull())
		    continue;

	       if (number == 1) {
		    // 最初の１行目の処理
		    htmlSource += ProcessFirstResponse(str);
	       } else {
		    // それ以降の処理
		    htmlSource += ProcessRestResponse(str, number);
	       }

	       number++;
	  }
     }

     htmlSource += HTML_FOOTER;
     datfile.Close();

     return htmlSource;
}
/**
 * スレッドの最初の行を処理するメソッド・スレの１には最後の「<>」の後にスレッドタイトルがつく
 */
wxString ThreadContentWindow::ProcessFirstResponse(wxString& threadRecord) {

     // スレッドのそれぞれの要素
     wxString default_nanashi, mail, day_and_ID, res;

     // ひとかたまりのHTMLソースにまとめる
     wxString lumpOfHTML = wxT("<dt>");
     lumpOfHTML += wxT("<a href=\"#1\">1</a>");

     // 正規表現でレスの内容を取り出してメモリに展開する
     if (regexThreadFst.IsValid()) {
	  if (regexThreadFst.Matches(threadRecord)) {
	       // マッチさせたそれぞれの要素を取得する
	       default_nanashi = regexThreadFst.GetMatch(threadRecord, 1);
	       mail = regexThreadFst.GetMatch(threadRecord, 2);
	       day_and_ID = regexThreadFst.GetMatch(threadRecord, 3);

	       // レスの最初に<table>タグを入れる
	       res.Append(wxT("<table border=0 id=\"1\">"));
	       res.Append(regexThreadFst.GetMatch(threadRecord, 4));
	       // 画像があれば<img>タグをつける
	       JaneCloneUtil::AddImgTag(res);
	       res.Append(wxT("</table>"));
	       // レス内部のURLに<a>タグをつける
	       res = JaneCloneUtil::ReplaceURLText(res);
	       // レスの最後に改行
	       res.Append(wxT("<br>"));
	  }
     }

     if (mail != wxEmptyString) {
	  // もしメ欄になにか入っているならば
	  lumpOfHTML += wxT(" 名前：<a href=\"mailto:");
	  lumpOfHTML += mail;
	  lumpOfHTML += wxT("\"><b>");
	  lumpOfHTML += default_nanashi;
	  lumpOfHTML += wxT("</b></a>");
	  lumpOfHTML += day_and_ID;
	  lumpOfHTML += wxT("<dd>");
	  lumpOfHTML += res;
     } else {
	  // 空の場合
	  lumpOfHTML += wxT(" 名前：<font color=green><b>");
	  lumpOfHTML += default_nanashi;
	  lumpOfHTML += wxT("</b></font>");
	  lumpOfHTML += day_and_ID;
	  lumpOfHTML += wxT("<dd>");
	  lumpOfHTML += res;
     }

     return lumpOfHTML;
}
/**
 * スレッドの１以降を処理するメソッド
 */
wxString ThreadContentWindow::ProcessRestResponse(wxString& threadRecord, int number) {

     // スレッドのそれぞれの要素
     wxString default_nanashi, mail, day_and_ID, res;

     // ひとかたまりのHTMLソースにまとめる
     wxString lumpOfHTML = wxT("<dt>");
     wxString num, link;
     num << number;
     link = wxT("<a href=\"#") + num + wxT("\">") + num + wxT("</a>");
     lumpOfHTML += link;

     // 正規表現でレスの内容を取り出してメモリに展開する
     if (regexThread.IsValid()) {
	  if (regexThread.Matches(threadRecord)) {
	       // マッチさせたそれぞれの要素を取得する
	       default_nanashi = regexThread.GetMatch(threadRecord, 1);
	       mail = regexThread.GetMatch(threadRecord, 2);
	       day_and_ID = regexThread.GetMatch(threadRecord, 3);	       
	       // レスの最初に<table>タグを入れる
	       res.Append(wxT("<table border=0 id=\"") + num + wxT("\">"));
	       res.Append(regexThread.GetMatch(threadRecord, 4));
	       // 画像があれば<img>タグをつける
	       JaneCloneUtil::AddImgTag(res);
	       res.Append(wxT("</table>"));
	       // レス内部のURLに<a>タグをつける
	       res = JaneCloneUtil::ReplaceURLText(res);
	       // レスの最後に改行
	       res.Append(wxT("<br>"));
	  }
     }

     if (mail != wxEmptyString) {
	  // もしメ欄になにか入っているならば
	  lumpOfHTML += wxT(" 名前：<a href=\"mailto:");
	  lumpOfHTML += mail;
	  lumpOfHTML += wxT("\"><b>");
	  lumpOfHTML += default_nanashi;
	  lumpOfHTML += wxT("</b></a>");
	  lumpOfHTML += day_and_ID;
	  lumpOfHTML += wxT("<dd>");
	  lumpOfHTML += res;
     } else {
	  // 空の場合
	  lumpOfHTML += wxT(" 名前：<font color=green><b>");
	  lumpOfHTML += default_nanashi;
	  lumpOfHTML += wxT("</b></font>");
	  lumpOfHTML += day_and_ID;
	  lumpOfHTML += wxT("<dd>");
	  lumpOfHTML += res;
     }

     return lumpOfHTML;
}
/**
 * URLを開いた時の状態
 */
// wxHtmlOpeningStatus ThreadContentWindow::OnOpeningURL(wxHtmlURLType WXUNUSED(type),
// 						      const wxString& url,
// 						      wxString *WXUNUSED(redirect)) const {

//      //GetRelatedFrame()->SetStatusText(url + _T(" lately opened"),1);
//      return wxHTML_OPEN;
// }
/**
 * スレッドのHtmlWindowで右クリックした場合の処理
 */
void ThreadContentWindow::OnRightClickHtmlWindow(wxMouseEvent& event) {

     wxMenu* copy = new wxMenu();
     copy->Append(ID_CopyFromHtmlWindow, wxT("コピー"));
     copy->Append(ID_CopyURLFromHtmlWindow, wxT("リンクをコピー"));
     copy->Enable(ID_CopyURLFromHtmlWindow, false); // デフォルトでは使用不能
     copy->Append(ID_SelectAllTextHtmlWindow, wxT("全て選択"));
#ifdef DEBUG
     // デバッグ用メニュー
     copy->Append(ID_HtmlSourceDebug, wxT("HTMLソース表示"));
#endif

     // イベント発生後にあったデータをクリアする
     m_selectedText.Clear();
     m_selectedText = this->SelectionToText();

     if (m_selectedText != wxEmptyString) {
	  // テキストを選択している場合は追加で選択肢を表示する
	  copy->AppendSeparator();

	  // 検索用のアイテムを設定する
	  wxMenuItem* itemYahoo = new wxMenuItem(copy, ID_SearchSelectWordByYahoo, wxT("選択範囲をYahooで検索"));
	  wxMenuItem* itemGoogle = new wxMenuItem(copy, ID_SearchSelectWordByGoogle, wxT("選択範囲をGoogleで検索"));
	  wxMenuItem* itemAmazon = new wxMenuItem(copy, ID_SearchSelectWordByAmazon, wxT("選択範囲をamazonで検索")); 


#ifndef __WXMAC__
	  wxImageHandler* pngLoader = new wxPNGHandler();
	  wxImage::AddHandler(pngLoader);
	  
	  wxBitmap bitmap;
#ifdef __WXMSW__
	  bitmap.LoadFile(wxT("rc\\yahoo.png"), wxBITMAP_TYPE_PNG);
	  itemYahoo->SetBitmap(bitmap);
	  bitmap.LoadFile(wxT("rc\\google.png"), wxBITMAP_TYPE_PNG);
	  itemGoogle->SetBitmap(bitmap);
	  bitmap.LoadFile(wxT("rc\\amazon.png"), wxBITMAP_TYPE_PNG);
	  itemAmazon->SetBitmap(bitmap);
#else
	  bitmap.LoadFile(wxT("rc/yahoo.png"), wxBITMAP_TYPE_PNG);
	  itemYahoo->SetBitmap(bitmap);
	  bitmap.LoadFile(wxT("rc/google.png"), wxBITMAP_TYPE_PNG);
	  itemGoogle->SetBitmap(bitmap);
	  bitmap.LoadFile(wxT("rc/amazon.png"), wxBITMAP_TYPE_PNG);
	  itemAmazon->SetBitmap(bitmap);
#endif
#endif

	  copy->Append(itemYahoo);
	  copy->Append(itemGoogle);
	  copy->Append(itemAmazon);
	  copy->Append(ID_SearchThreadBySelectWord, wxT("選択範囲でスレッドタイトル検索"));
	  copy->AppendSeparator();
	  copy->Append(wxID_ANY, wxT("抽出ポップアップ"));
	  copy->Append(wxID_ANY, wxT("この文にレス"));
	  copy->Append(wxID_ANY, wxT("メモ欄に追加"));
	  copy->Append(wxID_ANY, wxT("この文にメモ欄でレス"));
	  copy->Append(wxID_ANY, wxT("選択単語でレス抽出"));
	  wxMenu *ng = new wxMenu;
	  ng->Append(wxID_ANY, wxT("NGWordに追加"));
	  ng->Append(wxID_ANY, wxT("NGNameに追加"));
	  ng->AppendSeparator();
	  ng->Append(wxID_ANY, wxT("選択単語であぼ～ん"));
	  ng->Append(wxID_ANY, wxT("選択単語で透明あぼ～ん"));
	  copy->AppendSubMenu(ng, wxT("NG処理"));
	  copy->AppendSeparator();
     }

     int x, y;
     CalcUnscrolledPosition(event.m_x, event.m_y, &x, &y);
     wxHtmlCell *cell = GetInternalRepresentation()->FindCellByPos(x, y);
     m_linkInfo = NULL;

     if(cell)
	  m_linkInfo = cell->GetLink();

     if (m_linkInfo) {
	  if (m_linkInfo->GetHref() != wxEmptyString) {
	       // リンク上で右クリックしたのでなければ使用不能に
	       copy->Enable(ID_CopyURLFromHtmlWindow, true);
	  }
     }

     // ポップアップメニューを表示させる
     PopupMenu(copy);
}
/**
 * HtmlWindowで選択しているテキストをクリップボードにコピーする
 */
void ThreadContentWindow::CopyFromHtmlWindow(wxCommandEvent& event) {

     wxString selectText = this->SelectionToText();

     if (wxTheClipboard->Open()) {
	  wxTheClipboard->Clear();
	  wxTheClipboard->SetData(new wxTextDataObject(selectText));
	  wxTheClipboard->Close();
     }
}
/*
 * HtmlWindowで選択しているURLをクリップボードにコピーする
 */
void ThreadContentWindow::CopyURLFromHtmlWindow(wxCommandEvent& event) {

     wxString url;

     if (m_linkInfo) {
	  url = m_linkInfo->GetHref();
     }

     if (wxTheClipboard->Open()) {
	  wxTheClipboard->Clear();
	  wxTheClipboard->SetData(new wxTextDataObject(url));
	  wxTheClipboard->Close();
     }
}
/*
 * HtmlWindowでテキストを全て選択する
 */
void ThreadContentWindow::SelectAllTextHtmlWindow(wxCommandEvent& event) {
     this->SelectAll();
}
/**
 * 選択したテキストでヤフー検索
 */
void ThreadContentWindow::SearchSelectWordByYahoo(wxCommandEvent& event) {

     // 検索方法の元ネタは右：http://developer.yahoo.co.jp/other/query_parameters/search/websearch.html
     wxString url = wxT("http://search.yahoo.co.jp/search?p=");
     url += m_selectedText;
     url += wxT("&ei=UTF-8");
     wxLaunchDefaultBrowser(url);
}
/**
 * 選択したテキストでGoogle検索
 */
void ThreadContentWindow::SearchSelectWordByGoogle(wxCommandEvent& event) {

     // ここはHTTPSじゃなくてHTTP通信でいいのかなあ？
     wxString url = wxT("http://www.google.co.jp/search?q=");
     url += m_selectedText;
     url += wxT("&ie=utf-8");
     wxLaunchDefaultBrowser(url);
}
/**
 * 選択したテキストでAmazon検索
 */
void ThreadContentWindow::SearchSelectWordByAmazon(wxCommandEvent& event) {

     // AmazonはShift_JISによるURLエンコードしか受け付けないようだ
     wxString url = wxT("http://www.amazon.co.jp/gp/search/?__mk_ja_JP=%83J%83%5E%83J%83i&field-keywords=");
     wxNKF* nkf = new wxNKF();
     const std::string buffer = nkf->WxToMultiByte(m_selectedText, wxT("--ic=UTF-8 --oc=CP932"));
     const wxString urlEncode(JaneCloneUtil::UrlEncode(buffer).c_str(), wxConvUTF8);
     url += urlEncode;
     delete nkf;
	
     // 文字列をutf-8からShift_JISに変換しておく必要があるようだ
     wxLaunchDefaultBrowser(url);
}
/**
 * 選択したテキストでスレタイ検索
 */
void ThreadContentWindow::SearchThreadBySelectWord(wxCommandEvent& event) {

     // ! Fix Me !
     // 機能がいまいちよくわからない。もうちょっと考える。
}
/**
 * リサイズ時のイベント
 */
void ThreadContentWindow::OnSize(wxSizeEvent& event) {

     // 現在位置の取得
     int x, y; 
     GetViewStart(&x, &y);
     wxHtmlWindow::OnSize(event);
     Scroll(x, y);
     event.Skip(false);
}
/**
 * 左クリック時に起こるイベント
 */
void ThreadContentWindow::OnLeftClickHtmlWindow(wxHtmlLinkEvent& event) {
     
     const wxHtmlLinkInfo linkInfo = event.GetLinkInfo();
     const wxString href = linkInfo.GetHref();
     const wxString target = linkInfo.GetTarget();
     
     // bmp,jpg,jpeg,png,gifなどの拡張子が末尾に付いている場合ダウンロードを行う
     if (regexImage.IsValid()) {
	  // 正規表現のコンパイルにエラーがなければマッチさせる
	  if (regexImage.Matches(href)) {
	       // 画像ファイルをクリックしたのでダウンロードする
	       const wxString ext = regexImage.GetMatch(href, 3);
	       this->SetJaneCloneImageViewer(href, ext);
	  } else {
	       // マッチしなければそのままデフォルトのブラウザで開く
	       wxLaunchDefaultBrowser(href);
	  }
     }
}
/*
 * 画像ビューアの状態を確認し、設定する
 */
void ThreadContentWindow::SetJaneCloneImageViewer(const wxString& href, const wxString& ext) {

     // 画像をダウンロードする
     SocketCommunication* sock = new SocketCommunication();
     DownloadImageResult* result = new DownloadImageResult;
     result->ext = ext;
     result->imageURL = href;
     sock->DownloadImageFile(href, result);
     delete sock;

     // 画像ビューアに表示させる
     JaneClone::GetJaneCloneImageViewer()->Show(true);
     JaneClone::GetJaneCloneImageViewer()->SetImageFile(result);

     // wxMemoryFSHandlerに登録されているファイルを削除し、新しいファイルを登録する
     wxString filename = wxFileSystem::URLToFileName(result->imageURL).GetFullName();
     wxMemoryFSHandler::RemoveFile(filename);

     // wxBitmapTypeの判別
     wxBitmapType type;

     if (!ext.CmpNoCase(wxT("png"))) {
     	  type = wxBITMAP_TYPE_PNG;
     } else if (!ext.CmpNoCase(wxT("jpg"))) {
     	  type = wxBITMAP_TYPE_JPEG;
     } else if (!ext.CmpNoCase(wxT("jpeg"))) {
     	  type = wxBITMAP_TYPE_JPEG;
     } else if (!ext.CmpNoCase(wxT("gif"))) {
     	  type = wxBITMAP_TYPE_GIF;
     } else if (!ext.CmpNoCase(wxT("bmp"))) {
     	  type = wxBITMAP_TYPE_BMP;
     } else if (!ext.CmpNoCase(wxT("ico"))) {
     	  type = wxBITMAP_TYPE_ICO;
     } else if (!ext.CmpNoCase(wxT("xpm"))) {
     	  type = wxBITMAP_TYPE_XPM;
     } else if (!ext.CmpNoCase(wxT("tiff"))) {
     	  type = wxBITMAP_TYPE_TIF;
     } else {
     	  type = wxBITMAP_TYPE_ANY;
     }

     // resultの結果を元に画像のサムネイルと画像を配置する
     wxImage image;
     wxBitmap bitmap;
     
     // load wxImage
     if (!image.LoadFile(result->imagePath)) {
	  wxMessageBox(wxT("画像ファイルの読み出しに失敗しました"),
		       wxT("画像ビューア"),
		       wxICON_ERROR);
	  result->result = false;
	  delete result;
	  return;
     }
     // wxImage to wxBitmap
     bitmap = wxBitmap(image);

     if (!bitmap.Ok()) {
	  wxMessageBox(wxT("画像データの内部変換に失敗しました"),
		       wxT("画像ビューア"),
		       wxICON_ERROR);
	  result->result = false;
	  delete result;
	  return;
     }
     // 画像を登録する
     wxMemoryFSHandler::AddFile(filename, bitmap, type);
     this->AppendToPage(wxEmptyString);

     delete result;
}
/*
 * HTMLのデバッグ用イベント
 */
void ThreadContentWindow::HtmlSourceDebug(wxCommandEvent& event) {
     wxMessageBox(this->m_htmlSource);
}

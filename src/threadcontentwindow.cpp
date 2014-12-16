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
 *	Hiroyuki Nagata <newserver002@gmail.com>
 */

#include "threadcontentwindow.hpp"
#include "janeclone.hpp"

IMPLEMENT_DYNAMIC_CLASS(ThreadContentWindow, wxHtmlWindow)

BEGIN_EVENT_TABLE(ThreadContentWindow, wxHtmlWindow)
   EVT_RIGHT_DOWN(ThreadContentWindow::OnRightClickHtmlWindow)
   EVT_HTML_LINK_CLICKED(wxID_ANY, ThreadContentWindow::OnLeftClickHtmlWindow)
   EVT_MENU(ID_CopyFromHtmlWindow, ThreadContentWindow::CopyFromHtmlWindow)
   EVT_MENU(ID_CopyURLFromHtmlWindow, ThreadContentWindow::CopyURLFromHtmlWindow)
   EVT_MENU(ID_ShowRawHtmlFromHtmlWindow, ThreadContentWindow::ShowRawHtmlFromHtmlWindow)
   EVT_MENU(ID_SelectAllTextHtmlWindow, ThreadContentWindow::SelectAllTextHtmlWindow)
   EVT_MENU(ID_SearchSelectWordByYahoo,ThreadContentWindow::SearchSelectWordByYahoo)
   EVT_MENU(ID_SearchSelectWordByGoogle,ThreadContentWindow::SearchSelectWordByGoogle)
   EVT_MENU(ID_SearchSelectWordByAmazon,ThreadContentWindow::SearchSelectWordByAmazon)
   EVT_MENU(ID_SearchThreadBySelectWord,ThreadContentWindow::SearchThreadBySelectWord)
   EVT_MENU(ID_CallResponseWindowAnchor, ThreadContentWindow::CallResponseWindowWithAnchor)
   EVT_MENU(ID_CallResponseWindowQuote, ThreadContentWindow::CallResponseWindowWithQuote)
   EVT_MENU(ID_CopyTContentsToClipBoard, ThreadContentWindow::CopyTContentsToClipBoard) 
   EVT_MENU(ID_CopyTAllToClipBoard, ThreadContentWindow::CopyTAllToClipBoard)   
   EVT_SIZE(ThreadContentWindow::OnSize) 
   #ifdef DEBUG
   EVT_MENU(ID_HtmlSourceDebug, ThreadContentWindow::HtmlSourceDebug)
   #endif
END_EVENT_TABLE()

/**
 * 通常のコンストラクタ
 */
ThreadContentWindow::ThreadContentWindow(wxWindow* parent, const wxString& threadContentPath):
wxHtmlWindow(parent, ID_ThreadContentWindow, wxDefaultPosition, wxDefaultSize, wxHW_SCROLLBAR_AUTO) {

#ifdef __WXMSW__
     // ウィンドウ内に入った際のイベント通知
     this->Connect(ID_ThreadContentWindow,
		   wxEVT_ENTER_WINDOW,
		   wxMouseEventHandler(JaneClone::OnEnterWindow),
		   NULL, this);
#endif

     // 指定されたパスからHTMLファイルを読み出す
     wxString htmlSource = GetConvertedDatFile(threadContentPath);
     // 設定ファイルの準備をする
     // ユーザーのホームディレクトリを取得
     wxDir workDir(::wxGetHomeDir());
     wxDir jcDir(::wxGetHomeDir() + wxFileSeparator + JANECLONE_DIR);

     // ユーザーのホームディレクトリに隠しフォルダがあるかどうか確認
     if (!workDir.HasSubDirs(JANECLONE_DIR)) {
	  ::wxMkdir(jcDir.GetName());
     }
     if (!jcDir.HasSubDirs(wxT("prop"))) {
	  ::wxMkdir(jcDir.GetName() + wxFileSeparator + wxT("prop"));
     }

     // スキンを使わないならデフォルトフォントを設定する
     SkinInfo* skinInfo = new SkinInfo;
     bool       useSkin = CheckSkinFiles(skinInfo);
     delete skinInfo;
     
     if (!useSkin) {

	  // フォントのポイント数
	  const int p = 12;
	  static int f_size[] = {p - 2, p - 1, p, p + 1, p + 2, p + 3, p + 4 };

#ifdef __WXMSW__
	  wxString fontName = wxT("ＭＳ Ｐゴシック");
	  this->SetFonts(fontName, wxEmptyString, f_size);
#elif  __WXGTK__
	  wxFont f;
	  if (f.SetFaceName(wxT("Mona"))) {
	       this->SetFonts(wxT("Mona"), wxEmptyString, f_size);
	  } else if (f.SetFaceName(wxT("Mona-VLGothic"))) {
	       this->SetFonts(wxT("Mona-VLGothic"), wxEmptyString, f_size);
	  } else if (f.SetFaceName(wxT("Mona-sazanami"))) {
	       this->SetFonts(wxT("Mona-sazanami"), wxEmptyString, f_size);
	  }

#elif  __WXMAC__
	  wxString fontName = wxT("Mona");
	  this->SetFonts(fontName, wxEmptyString, f_size);
#endif
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

     // スキンがあれば使う
     SkinInfo* skinInfo = new SkinInfo;
     bool       useSkin = CheckSkinFiles(skinInfo);

     // 取得サイズ分だけwxStringを確保する
     wxString htmlSource;
     htmlSource.Alloc(fileSize);

     if (useSkin && skinInfo->header != wxEmptyString) {
	  htmlSource += skinInfo->header;
     } else {
	  htmlSource += HTML_HEADER;
     }     

     // テキストファイルの読み込み
     wxTextFile datfile;
     datfile.Open(threadContentPath, wxConvUTF8);
     wxString str;
     int number = 1;

     // ファイルがオープンされているならば
     if (datfile.IsOpened()) {
	  for (str = datfile.GetFirstLine(); 
	       !datfile.Eof();
	       str = datfile.GetNextLine()) {

	       if (str.IsNull())
		    continue;

	       if (number == 1) {
		    // 最初の１行目の処理
		    htmlSource += JaneCloneUtil::ProcessFirstResponse(str);
	       } else {
		    // それ以降の処理
		    htmlSource += JaneCloneUtil::ProcessRestResponse(str, number);
	       }

	       number++;
	       this->m_response_number = number;
	  }
     }

     if (useSkin && skinInfo->footer != wxEmptyString) {
	  htmlSource += skinInfo->footer;
     } else {
	  htmlSource += HTML_FOOTER;
     }

     // スキン用構造体を片付ける
     delete skinInfo;
     datfile.Close();

     // ID:xxxxxxxxxx を置換する
     htmlSource = JaneCloneUtil::AddID(htmlSource);
     // >>xx のようなアンカーを受けているレスを赤くする
     htmlSource = JaneCloneUtil::AddColorAnchoredID(htmlSource);

     return htmlSource;
}
/**
 * 現在開いているHTMLのスクロール位置を取得する
 */
void ThreadContentWindow::GetHtmlWindowScrollPos(wxPoint* p) {
     // 現在位置の取得
     int x, y;
     GetViewStart(&x, &y);
     p->x = x;
     p->y = y;
}
/**
 * 外のクラスから強制的にスクロールさせる
 */
void ThreadContentWindow::ForceScroll(const wxPoint* p) {
     Scroll(p->x, p->y);
}
/**
 * スレッドのHtmlWindowで右クリックした場合の処理
 */
void ThreadContentWindow::OnRightClickHtmlWindow(wxMouseEvent& event) {

     wxMenu* copy = new wxMenu();
     copy->Append(ID_CopyFromHtmlWindow, wxT("コピー"));
     copy->Append(ID_CopyURLFromHtmlWindow, wxT("リンクをコピー"));
     copy->Enable(ID_CopyURLFromHtmlWindow, false); // デフォルトでは使用不能
     copy->Append(ID_SelectAllTextHtmlWindow, wxT("全て選択"));
     copy->Append(ID_ShowRawHtmlFromHtmlWindow, wxT("ソースを表示する"));

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
#elif __WXGTK__
	  bitmap.LoadFile(wxT("rc/yahoo.png"), wxBITMAP_TYPE_PNG);
	  itemYahoo->SetBitmap(bitmap);
	  bitmap.LoadFile(wxT("rc/google.png"), wxBITMAP_TYPE_PNG);
	  itemGoogle->SetBitmap(bitmap);
	  bitmap.LoadFile(wxT("rc/amazon.png"), wxBITMAP_TYPE_PNG);
	  itemAmazon->SetBitmap(bitmap);
#elif __WXMAC__
	  bitmap.LoadFile(wxT("JaneClone.app/Contents/MacOS/rc/yahoo.png"), wxBITMAP_TYPE_PNG);
	  itemYahoo->SetBitmap(bitmap);
	  bitmap.LoadFile(wxT("JaneClone.app/Contents/MacOS/rc/google.png"), wxBITMAP_TYPE_PNG);
	  itemGoogle->SetBitmap(bitmap);
	  bitmap.LoadFile(wxT("JaneClone.app/Contents/MacOS/rc/amazon.png"), wxBITMAP_TYPE_PNG);
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
void ThreadContentWindow::CopyURLFromHtmlWindow(wxCommandEvent& event) 
{
     wxString url;

     if (m_linkInfo) 
     {
	  url = m_linkInfo->GetHref();
     }

     if (wxTheClipboard->Open()) 
     {
	  wxTheClipboard->Clear();
	  wxTheClipboard->SetData(new wxTextDataObject(url));
	  wxTheClipboard->Close();
     }
}

/**
 * HTMLソースを表示する
 */
void ThreadContentWindow::ShowRawHtmlFromHtmlWindow(wxCommandEvent& event)
{
     const wxString html = this->m_htmlSource;
     HtmlSourceDialog* htmldlg = new HtmlSourceDialog(ExtractBoardList::HtmlFormat(html));
     htmldlg->Show(true);
}

/**
 * HTMLソース表示ダイアログのコンストラクタ
 */
HtmlSourceDialog::HtmlSourceDialog(const wxString& html)
     : wxDialog(NULL, -1, wxT("HTMLソース"), wxDefaultPosition, wxSize(640, 480), wxRESIZE_BORDER|wxDEFAULT_DIALOG_STYLE)
{
     wxPanel *panel = new wxPanel(this, -1);
     wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);

     // ラベル部分
     wxBoxSizer *hbox2 = new wxBoxSizer(wxHORIZONTAL);
     wxStaticText *st2 = new wxStaticText(panel, wxID_ANY, wxT("HTMLソース"));
     hbox2->Add(st2, 0);
     vbox->Add(hbox2, 0, wxLEFT | wxTOP, 10);
     vbox->Add(-1, 10);

     // HTMLソース表示部分
     wxBoxSizer* hbox3 = new wxBoxSizer(wxHORIZONTAL);
     wxTextCtrl* m_tc2 = new wxTextCtrl(panel, wxID_ANY, wxT(""), wxPoint(-1, -1), wxSize(-1, -1), wxTE_MULTILINE|wxTE_READONLY);
     m_tc2->SetValue(html);
     hbox3->Add(m_tc2, 1, wxEXPAND);
     vbox->Add(hbox3, 1, wxLEFT | wxRIGHT | wxEXPAND, 10);
     vbox->Add(-1, 25);

     panel->SetSizer(vbox);

     Centre();
     ShowModal();
     Destroy();
}

/*
 * HtmlWindowでテキストを全て選択する
 */
void ThreadContentWindow::SelectAllTextHtmlWindow(wxCommandEvent& event) 
{
     this->SelectAll();
}
/**
 * 選択したテキストでヤフー検索
 */
void ThreadContentWindow::SearchSelectWordByYahoo(wxCommandEvent& event) 
{
     // 検索方法の元ネタは右：http://developer.yahoo.co.jp/other/query_parameters/search/websearch.html
     wxString url = wxT("http://search.yahoo.co.jp/search?p=");
     url += m_selectedText;
     url += wxT("&ei=UTF-8");
     wxLaunchDefaultBrowser(url);
}
/**
 * 選択したテキストでGoogle検索
 */
void ThreadContentWindow::SearchSelectWordByGoogle(wxCommandEvent& event) 
{
     // ここはHTTPSじゃなくてHTTP通信でいいのかなあ？
     wxString url = wxT("http://www.google.co.jp/search?q=");
     url += m_selectedText;
     url += wxT("&ie=utf-8");
     wxLaunchDefaultBrowser(url);
}
/**
 * 選択したテキストでAmazon検索
 */
void ThreadContentWindow::SearchSelectWordByAmazon(wxCommandEvent& event) 
{
     // AmazonはShift_JISによるURLエンコードしか受け付けないようだ
     wxString url = wxT("http://www.amazon.co.jp/gp/search/?__mk_ja_JP=%83J%83%5E%83J%83i&field-keywords=");
     const std::string buffer = babel::utf8_to_sjis(std::string(m_selectedText.mb_str()));
     const wxString urlEncode(JaneCloneUtil::UrlEncode(buffer).c_str(), wxConvUTF8);
     url += urlEncode;
	
     // 文字列をutf-8からShift_JISに変換しておく必要があるようだ
     wxLaunchDefaultBrowser(url);
}
/**
 * 選択したテキストでスレタイ検索
 */
void ThreadContentWindow::SearchThreadBySelectWord(wxCommandEvent& event) 
{
     // ! Fix Me !
     // 機能がいまいちよくわからない。もうちょっと考える。
}
/**
 * リサイズ時のイベント
 */
void ThreadContentWindow::OnSize(wxSizeEvent& event) 
{
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
void ThreadContentWindow::OnLeftClickHtmlWindow(wxHtmlLinkEvent& event) 
{     
     const wxHtmlLinkInfo linkInfo = event.GetLinkInfo();
     const wxString href = linkInfo.GetHref();
     const wxString target = linkInfo.GetTarget();
     wxString rest = wxEmptyString;
     long res = 0;
     
     // bmp,jpg,jpeg,png,gifなどの拡張子が末尾に付いている場合ダウンロードを行う
     if (regexImage.IsValid()) 
     {
	  // 正規表現のコンパイルにエラーがなければマッチさせる
	  if (regexImage.Matches(href)) 
	  {
	       // 画像ファイルをクリックしたのでダウンロードする
	       const wxString ext = regexImage.GetMatch(href, 3);
	       this->SetJaneCloneImageViewer(href, ext);

	  } 
	  else if (href.StartsWith(wxT("#"), &rest) && rest.ToLong(&res, 10)) 
	  {
	       if ( 0 < res && res <= 1000) 
	       {
		    OnLeftClickResponseNumber(event, href, res);
	       }
	  } 
	  else 
	  {
	       // 通常のリンクかどうか判定して処理
	       OnClickOrdinaryLink(href);
	  }
     }
}
/*
 * レス番号を左クリックした時に起こるイベント
 */
void ThreadContentWindow::OnLeftClickResponseNumber(wxHtmlLinkEvent& event, const wxString& href, const long res) {

     // 引数として渡された情報はクラス変数に格納する
     m_response = -1;
     m_response = res;

     wxMenu* response = new wxMenu();
     response->Append(ID_CallResponseWindowAnchor, wxT("これにレス"));
     response->Append(ID_CallResponseWindowQuote, wxT("引用付きレス"));
     response->AppendSeparator();

     wxMenu *abone = new wxMenu;
     abone->Append(wxID_ANY, wxT("NGNameに追加"));
     abone->Append(wxID_ANY, wxT("NGAddrに追加"));
     abone->Append(wxID_ANY, wxT("datからNGwordに追加"));
     abone->Append(wxID_ANY, wxT("NGIDに追加"));
     abone->Append(wxID_ANY, wxT("NGBEに追加"));
     abone->Append(wxID_ANY, wxT("あぼ～ん・チェックを解除"));

     wxMenu* copy = new wxMenu();
     copy->Append(ID_CopyTContentsToClipBoard, wxT("内容をコピー"));
     copy->Append(ID_CopyTURLToClipBoard, wxT("URLをコピー"));
     copy->Append(ID_CopyTTitleToClipBoard, wxT("タイトルをコピー"));
     copy->Append(ID_CopyTBothDataToClipBoard, wxT("タイトルとURLをコピー"));
     copy->Append(ID_CopyTAllToClipBoard, wxT("タイトルとURLと内容をコピー"));
     response->AppendSubMenu(copy, wxT("このレスをコピー"));
     response->AppendSeparator();
     response->Append(wxID_ANY, wxT("ここまで読んだ"));
     response->AppendSeparator();
     response->Append(wxID_ANY, wxT("あぼ〜ん"));
     response->Append(wxID_ANY, wxT("透明あぼ〜ん"));
     response->AppendSeparator();
     response->Append(wxID_ANY, wxT("ここからあぼ〜ん"));
     response->AppendSubMenu(abone, wxT("NGItemに追加"));
     response->AppendSeparator();
     response->Append(wxID_ANY, wxT("レスの内容をAAListに登録"));

     // ポップアップメニューを表示させる
     PopupMenu(response);
}
/**
 * リンクが2chのものかどうか判定
 */
void ThreadContentWindow::OnClickOrdinaryLink(const wxString& link) 
{
     // デフォルトのブラウザを使用するかどうか
     bool useDefaultBrowser = true;
     // URLからホスト名を取得する
     PartOfURI uri;
     bool ret = JaneCloneUtil::SubstringURI(link, &uri);
     
     if (ret) 
     {
	  if (uri.hostname.Contains(wxT("2ch.net"))) useDefaultBrowser = false;
     } 
     else 
     {
	  // エラー、とりあえずブラウザで開く
	  useDefaultBrowser = true;
     }

     if (useDefaultBrowser) 
     {
	  wxLaunchDefaultBrowser(link);
     } 
     else 
     {
	  // 板の場合：      http://anago.2ch.net/software
	  // スレッドの場合：http://anago.2ch.net/test/read.cgi/software/1365347301
	  // レス番まで指定：http://anago.2ch.net/test/read.cgi/software/1365347301/369

	  /** どちらにせよスレッドの名前を取得しなくてはいけないため、板でもスレッドでも
	   *  まず、板一覧を取得してその後にスレッドを取得する。*/
	  wxString path    = uri.path;
	  wxString host    = uri.hostname;
	  wxString rest    = wxEmptyString;
	  bool urlIsThread = false;
	  if (path.StartsWith(wxT("/test/read.cgi/"), &rest)) urlIsThread = true;

	  // 文字列を分割する(StringTokenizerはなぜか使用できない...バグってる？)
	  std::vector<std::string> container;
	  JaneCloneUtil::SplitStdString(container, std::string(path.mb_str()), "/");

	  wxString boardName;      // 板名
	  wxString boardURL; 	   // URLを保持する文字列
	  wxString boardNameAscii; // サーバー名を保持する文字列
	  wxString origNumber;     // スレッドの固有番号
	  wxString title;          // スレッドの名前

	  if (urlIsThread) 
	  {
	       if (container.size() < 4) 
	       {
		    // ERROR
		    wxLaunchDefaultBrowser(link);
		    return;
	       }
	       
	       boardNameAscii = wxString(container.at(3).c_str(), wxConvUTF8);
	       boardURL       = wxT("http://") + host + wxT("/") + boardNameAscii;
	       origNumber     = wxString(container.at(4).c_str(), wxConvUTF8);

	  } 
	  else 
	  {
	       if (container.size() < 1) 
	       {
		    // ERROR
		    wxLaunchDefaultBrowser(link);
		    return;
	       }
	       boardNameAscii = wxString(container.at(1).c_str(), wxConvUTF8);
	       boardURL       = wxT("http://") + host + wxT("/") + boardNameAscii;
	  }

	  if (wxWindow* grand = this->GetGrandParent()) 
	  {
	       wxAuiNotebook* threadNoteBook = dynamic_cast<wxAuiNotebook*>(grand->FindWindowByLabel(THREAD_NOTEBOOK));
	       wxAuiNotebook* boardNoteBook  = dynamic_cast<wxAuiNotebook*>(grand->FindWindowByLabel(BOARD_NOTEBOOK));
	       wxTextCtrl*    m_logCtrl      = dynamic_cast<wxTextCtrl*>(grand->FindWindowByLabel(LOG_WINDOW));

	       if (threadNoteBook && boardNoteBook) {
		    // 必要な構造体を宣言する
		    ThreadInfo threadInfoHash;
		    URLvsBoardName boardInfoHash;
	       
		    if (JaneClone* wxJaneClone = dynamic_cast<JaneClone*>(boardNoteBook->GetParent())) {

			 // ハッシュから板名を探す
			 NameURLHash::iterator it;
			 for (it = wxJaneClone->retainHash.begin(); it != wxJaneClone->retainHash.end(); ++it) 
			 {
			      wxString key  = it->first;
			      boardInfoHash = it->second;

			      if (boardInfoHash.boardNameAscii == boardNameAscii) 
			      {
				   boardName = boardInfoHash.boardName;
				   break;
			      }
			 }

			 // 板一覧のツリーをクリックして、それをノートブックに反映するメソッド
			 wxJaneClone->SetBoardNameToNoteBook(boardName, boardURL, boardNameAscii);
			 if (!urlIsThread) return; // スレッドでないならここで終了

			 // スレッドタイトルを取得するため、リストコントロールを引き出してくる
			 VirtualBoardListCtrl* vbListCtrl = 
			      dynamic_cast<VirtualBoardListCtrl*>(wxWindow::FindWindowByName(boardInfoHash.boardName));
			 if (vbListCtrl == NULL) 
			 {
			      // ERROR
			      wxLaunchDefaultBrowser(link);
			      return;
			 }

			 // STLで見つけてやる.ラムダ式など使ってみる.
			 std::vector<VirtualBoardListItem>::iterator it2 = 
			      std::find_if(vbListCtrl->m_vBoardList.begin(), 
					   vbListCtrl->m_vBoardList.end(),
					   [&origNumber] (const VirtualBoardListItem& item) -> bool {
						return item.getOid() == origNumber;
					   });

			 if (it2 != vbListCtrl->m_vBoardList.end()) 
			 {
			      // タイトル取得
			      const wxString title = (*it2).getTitle();
 
			      // ソケット通信を行う
			      SocketCommunication* sock = new SocketCommunication();
			      const wxString threadContentPath = sock->DownloadThread(boardName, 
										      boardURL, 
										      boardNameAscii, 
										      origNumber);
			      delete sock;
			      // 無事に通信が終了したならばステータスバーに表示
			      wxJaneClone->SetStatusText(wxT(" スレッドのダウンロード終了"));

			      // スレッドの内容をノートブックに反映する
			      wxJaneClone->SetThreadContentToNoteBook(threadContentPath, origNumber, title);
			      // ノートブックに登録されたスレッド情報をハッシュに登録する
			      ThreadInfoHash tiHash;
			      wxJaneClone->GetThreadInfoHash(tiHash);

			      ThreadInfo info;
			      info.origNumber = origNumber;
			      info.boardNameAscii = boardNameAscii;
			      tiHash[title] = info;

			      wxJaneClone->SetThreadInfoHash(tiHash);

			 } 
			 else 
			 {
			      // ERROR
			      wxLaunchDefaultBrowser(link);
			      return;
			 }
		    }
	       }
	  }  
     }
}
/*
 * レス番号を指定して書き込みウィンドウを開く
 */
void ThreadContentWindow::CallResponseWindowWithAnchor(wxCommandEvent& event) 
{
     /**
      * ここのコードはthis->GetGrandParent()と書けば一度にJaneClone本体のインスタンスが
      * 取得できるはずなのだができない.一度JaneCloneの子インスタンスを取得してから
      * 改めてJaneClone本体のインスタンスを取得している. Is this bug ?
      */
     if (wxWindow* grand = this->GetGrandParent()) 
     {
	  wxAuiNotebook* threadNoteBook = dynamic_cast<wxAuiNotebook*>(grand->FindWindowByLabel(THREAD_NOTEBOOK));
	  wxAuiNotebook* boardNoteBook  = dynamic_cast<wxAuiNotebook*>(grand->FindWindowByLabel(BOARD_NOTEBOOK));
	  wxTextCtrl*    m_logCtrl      = dynamic_cast<wxTextCtrl*>(grand->FindWindowByLabel(LOG_WINDOW));

	  if (threadNoteBook && boardNoteBook) 
	  {
	       // 必要な構造体を宣言する
	       ThreadInfo threadInfoHash;
	       URLvsBoardName boardInfoHash;
	       
	       if (JaneClone* wxJaneClone = dynamic_cast<JaneClone*>(boardNoteBook->GetParent())) 
	       {
		    // スレッド情報をコピーしてくる
		    ThreadInfoHash tiHash;
		    wxJaneClone->GetThreadInfoHash(tiHash);
		    // 選択されたスレタブの情報を集める
		    wxString title = threadNoteBook->GetPageText(threadNoteBook->GetSelection());
		    threadInfoHash = tiHash[title];
		    threadInfoHash.title = title; // タイトル情報を設定する

		    // ハッシュからURLを探す
		    NameURLHash::iterator it;
		    for (it = wxJaneClone->retainHash.begin(); it != wxJaneClone->retainHash.end(); ++it) 
		    {
			 wxString key = it->first;
			 boardInfoHash = it->second;

			 if (boardInfoHash.boardNameAscii == threadInfoHash.boardNameAscii) break;
		    }

		    // ウィンドウの大きさを取得する
		    int wScreenPx, hScreenPx;
		    ::wxDisplaySize(&wScreenPx, &hScreenPx);
		    // レス用のウィンドウは 640:480なので、ちょうど中央にくるように調整する
		    wxPoint point(wScreenPx/2 - 320, hScreenPx/2 - 240);
		    ResponseWindow* response = new ResponseWindow(wxJaneClone, title, boardInfoHash, threadInfoHash, point, m_logCtrl);
		    // ウィンドウにテキストを設定する
		    response->AddKakikomiText(wxString::Format(wxT(">>%ld"), m_response));
		    response->Show(true);
	       }
	  }
     }
}
/*
 * レス内容を引用して書き込みウィンドウを開く
 */
void ThreadContentWindow::CallResponseWindowWithQuote(wxCommandEvent& event) 
{
     if (wxWindow* grand = this->GetGrandParent()) 
     {
	  wxAuiNotebook* threadNoteBook = dynamic_cast<wxAuiNotebook*>(grand->FindWindowByLabel(THREAD_NOTEBOOK));
	  wxAuiNotebook* boardNoteBook  = dynamic_cast<wxAuiNotebook*>(grand->FindWindowByLabel(BOARD_NOTEBOOK));
	  wxTextCtrl*    m_logCtrl      = dynamic_cast<wxTextCtrl*>(grand->FindWindowByLabel(LOG_WINDOW));

	  if (threadNoteBook && boardNoteBook) 
	  {
	       // 必要な構造体を宣言する
	       ThreadInfo threadInfoHash;
	       URLvsBoardName boardInfoHash;
	       
	       if (JaneClone* wxJaneClone = dynamic_cast<JaneClone*>(boardNoteBook->GetParent())) 
	       {
		    // スレッド情報をコピーしてくる
		    ThreadInfoHash tiHash;
		    wxJaneClone->GetThreadInfoHash(tiHash);
		    // 選択されたスレタブの情報を集める
		    wxString title = threadNoteBook->GetPageText(threadNoteBook->GetSelection());
		    threadInfoHash = tiHash[title];
		    threadInfoHash.title = title; // タイトル情報を設定する

		    // ハッシュからURLを探す
		    NameURLHash::iterator it;
		    for (it = wxJaneClone->retainHash.begin(); it != wxJaneClone->retainHash.end(); ++it) 
		    {
			 wxString key = it->first;
			 boardInfoHash = it->second;

			 if (boardInfoHash.boardNameAscii == threadInfoHash.boardNameAscii) break;
		    }

		    // ウィンドウの大きさを取得する
		    int wScreenPx, hScreenPx;
		    ::wxDisplaySize(&wScreenPx, &hScreenPx);
		    // レス用のウィンドウは 640:480なので、ちょうど中央にくるように調整する
		    wxPoint point(wScreenPx/2 - 320, hScreenPx/2 - 240);
		    ResponseWindow* response = new ResponseWindow(wxJaneClone, title, boardInfoHash, threadInfoHash, point, m_logCtrl);
		
		    // ウィンドウにテキストを設定する
		    wxString quote  = wxString::Format(wxT(">>%ld\n"), m_response);
		    quote += JaneCloneUtil::FindAnchoredResponseText(threadInfoHash.boardNameAscii, threadInfoHash.origNumber, m_response);

		    response->AddKakikomiText(quote);
		    response->Show(true);
	       }
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
     JaneCloneImageViewer* imageViewer = JaneClone::GetJaneCloneImageViewer();
     imageViewer->Show(true);
     imageViewer->SetImageFile(result);
     imageViewer->Raise();

     // wxMemoryFSHandlerに登録されているファイルを削除し、新しいファイルを登録する
     wxString filename = wxFileSystem::URLToFileName(result->imageURL).GetFullName();

     // FIXME: JaneClone-1.1.5
     //wxMemoryFSHandler::RemoveFile(filename);

     // wxBitmapTypeの判別
     wxBitmapType type;

     // FIXME: たぶん正規表現使った方がいい
     if (!ext.CmpNoCase(wxT("png"))) 
     {
	  type = wxBITMAP_TYPE_PNG;
     } 
     else if (!ext.CmpNoCase(wxT("jpg"))) 
     {
	  type = wxBITMAP_TYPE_JPEG;
     } 
     else if (!ext.CmpNoCase(wxT("jpeg"))) 
     {
	  type = wxBITMAP_TYPE_JPEG;
     } 
     else if (!ext.CmpNoCase(wxT("gif"))) 
     {
	  type = wxBITMAP_TYPE_GIF;
     } 
     else if (!ext.CmpNoCase(wxT("bmp"))) 
     {
	  type = wxBITMAP_TYPE_BMP;
     } 
     else if (!ext.CmpNoCase(wxT("ico"))) 
     {
	  type = wxBITMAP_TYPE_ICO;
     } 
     else if (!ext.CmpNoCase(wxT("xpm"))) 
     {
	  type = wxBITMAP_TYPE_XPM;
     } 
     else if (!ext.CmpNoCase(wxT("tiff"))) 
     {
	  type = wxBITMAP_TYPE_TIF;
     } 
     else 
     {
	  type = wxBITMAP_TYPE_ANY;
     }

     // resultの結果を元に画像のサムネイルと画像を配置する
     wxImage image;
     wxBitmap bitmap;
     
     // load wxImage
     if (!image.LoadFile(result->imagePath)) 
     {
	  wxMessageBox(wxT("画像ファイルの読み出しに失敗しました"),
		       wxT("画像ビューア"),
		       wxICON_ERROR);
	  result->result = false;
	  delete result;
	  return;
     }
     // wxImage to wxBitmap
     bitmap = wxBitmap(image);

     if (!bitmap.Ok()) 
     {
	  wxMessageBox(wxT("画像データの内部変換に失敗しました"),
		       wxT("画像ビューア"),
		       wxICON_ERROR);
	  result->result = false;
	  delete result;
	  return;
     }
     // 画像を登録する

     // FIXME: JaneClone-1.1.5
     // wxMemoryFSHandler::AddFile(filename, bitmap, type);
     // 現在位置の取得とスクロール
     int x, y; 
     GetViewStart(&x, &y);
     this->AppendToPage(wxEmptyString);
     Scroll(x, y);

     delete result;
}
/*
 * HTMLのデバッグ用イベント
 */
void ThreadContentWindow::HtmlSourceDebug(wxCommandEvent& event) 
{
     wxMessageBox(this->m_htmlSource);
}
/*
 * レスの内容をクリップボードにコピーする
 */
void ThreadContentWindow::CopyTContentsToClipBoard(wxCommandEvent& event) 
{
     if (wxWindow* grand = this->GetGrandParent()) 
     {
	  wxAuiNotebook* threadNoteBook = dynamic_cast<wxAuiNotebook*>(grand->FindWindowByLabel(THREAD_NOTEBOOK));

	  if (threadNoteBook) 
	  {
	       // 必要な構造体を宣言する
	       ThreadInfo threadInfoHash;
	       URLvsBoardName boardInfoHash;
	       
	       if (JaneClone* wxJaneClone = dynamic_cast<JaneClone*>(threadNoteBook->GetParent())) 
	       {
		    // スレッド情報をコピーしてくる
		    ThreadInfoHash tiHash;
		    wxJaneClone->GetThreadInfoHash(tiHash);
		    // 選択されたスレタブの情報を集める
		    const wxString title = threadNoteBook->GetPageText(threadNoteBook->GetSelection());
		    threadInfoHash = tiHash[title];
		    threadInfoHash.title = title; // タイトル情報を設定する

		    // ハッシュからURLを探す
		    NameURLHash::iterator it;
		    for (it = wxJaneClone->retainHash.begin(); it != wxJaneClone->retainHash.end(); ++it) {
			 const wxString key = it->first;
			 boardInfoHash = it->second;
			 if (boardInfoHash.boardNameAscii == threadInfoHash.boardNameAscii) break;
		    }
	       }

	       // レスの内容を取得する
	       const wxString response = JaneCloneUtil::FindAnchoredResponseText(threadInfoHash.boardNameAscii, 
										 threadInfoHash.origNumber,
										 m_response, false);

	       if (wxTheClipboard->Open()) 
	       {
		    // 情報をクリップボードに渡す
		    wxTheClipboard->Clear();
		    wxTheClipboard->SetData(new wxTextDataObject(response));
		    wxTheClipboard->Close();
	       }
	  }
     }
}
/*
 * 指定されたレスの内容をすべてクリップボードにコピーする
 */
void ThreadContentWindow::CopyTAllToClipBoard(wxCommandEvent& event) 
{
     // 必要な構造体を宣言する
     ThreadInfo     threadInfoHash;
     URLvsBoardName boardInfoHash;
     wxString       response;
     wxString       boardURL;

     if (wxWindow* grand = this->GetGrandParent()) 
     {
	  wxAuiNotebook* threadNoteBook = dynamic_cast<wxAuiNotebook*>(grand->FindWindowByLabel(THREAD_NOTEBOOK));
	  if (threadNoteBook) 
	  {	       
	       if (JaneClone* wxJaneClone = dynamic_cast<JaneClone*>(threadNoteBook->GetParent())) 
	       {
		    // スレッド情報をコピーしてくる
		    ThreadInfoHash tiHash;
		    wxJaneClone->GetThreadInfoHash(tiHash);
		    // 選択されたスレタブの情報を集める
		    const wxString title = threadNoteBook->GetPageText(threadNoteBook->GetSelection());
		    threadInfoHash = tiHash[title];
		    threadInfoHash.title = title; // タイトル情報を設定する

		    // ハッシュからURLを探す
		    NameURLHash::iterator it;
		    for (it = wxJaneClone->retainHash.begin(); it != wxJaneClone->retainHash.end(); ++it) 
		    {
			 const wxString key = it->first;
			 boardInfoHash = it->second;
			 if (boardInfoHash.boardNameAscii == threadInfoHash.boardNameAscii) break;
		    }
	       }

	       // レスの内容を取得する
	       response = JaneCloneUtil::FindAnchoredResponseText(threadInfoHash.boardNameAscii, threadInfoHash.origNumber,
								  m_response, false);
	       // URLを取得する
	       boardURL = boardInfoHash.boardURL;
	  }
     }
     wxString threadURL = boardURL;

     // ホスト名の後の板名を除く
     int begin = threadURL.Find(threadInfoHash.boardNameAscii);
     if (begin == wxNOT_FOUND) 
     {
	  return;
     }
     threadURL = threadURL.Mid(0, begin);
     threadURL += wxT("test/read.cgi/");
     threadURL += threadInfoHash.boardNameAscii;
     threadURL += wxT("/");
     threadURL += threadInfoHash.origNumber;
     threadURL += wxT("/");

     if (wxTheClipboard->Open()) 
     {
	  wxTheClipboard->SetData(new wxTextDataObject(threadInfoHash.title + wxT("\n") + threadURL + wxT("\n") + response));
	  wxTheClipboard->Close();
     }
}
/**
 * スキン用のファイルが有るかどうか確認する
 */
bool ThreadContentWindow::CheckSkinFiles(SkinInfo* skin) 
{
     // スキン用のパスが設定されていなければ即リターン
     const wxString key = wxT("DEFAULT_SKINFILE_PATH");
     wxString skinPath = wxEmptyString;
     JaneCloneUtil::GetJaneCloneProperties(key, &skinPath);
     bool ret = false;

     if (skinPath == wxEmptyString) 
     {
	  return false;
     }

     if (!wxDir::Exists(skinPath)) 
     {
	  wxMessageBox(wxT("スキン用のディレクトリが存在しません、設定画面を開いてスキンのパス設定を確認してください。"), 
		       wxT("スキン設定"), wxICON_ERROR);
	  return false;
     }

     // Footer.html
     if (wxFile::Exists(skinPath + wxFileSeparator + wxT("Footer.html"))) 
     {
	  const wxString filePath = skinPath + wxFileSeparator + wxT("Footer.html");
	  skin->footer = ReadPlainTextFile(filePath);
	  ret = true;
     }	  
     // Header.html
     if (wxFile::Exists(skinPath + wxFileSeparator + wxT("Header.html"))) 
     {
	  const wxString filePath = skinPath + wxFileSeparator + wxT("Header.html");
	  skin->header = ReadPlainTextFile(filePath);
	  ret = true;
     }
     // NewRes.html
     if (wxFile::Exists(skinPath + wxFileSeparator + wxT("NewRes.html"))) 
     {
	  const wxString filePath = skinPath + wxFileSeparator + wxT("NewRes.html");
	  skin->newres = ReadPlainTextFile(filePath);
	  ret = true;
     }
     // PopupRes.html
     if (wxFile::Exists(skinPath + wxFileSeparator + wxT("PopupRes.html"))) 
     {
	  const wxString filePath = skinPath + wxFileSeparator + wxT("PopupRes.html");
	  skin->popup = ReadPlainTextFile(filePath);
	  ret = true;
     }
     // Res.html
     if (wxFile::Exists(skinPath + wxFileSeparator + wxT("Res.html"))) 
     {
	  const wxString filePath = skinPath + wxFileSeparator + wxT("Res.html");
	  skin->res = ReadPlainTextFile(filePath);
	  ret = true;
     }
     // ***.js
     // TODO: SpiderMonkeyの適用

     if (ret) 
     {  
	  const wxString message = wxT("スキンを適用します\n");
	  JaneCloneUiUtil::SendLoggingHelper(message);
     }
     
     return ret;
}
/**
 * 指定されたファイル中のテキストをメモリに展開する
 */
wxString ThreadContentWindow::ReadPlainTextFile(const wxString& filePath) 
{
     wxTextFile textFile;
     wxString   htmlDOM;
     textFile.Open(filePath, wxConvUTF8);

     for (int i = 0; i < textFile.GetLineCount(); i++) {
	  htmlDOM += textFile[i];
     }

     return htmlDOM;
}

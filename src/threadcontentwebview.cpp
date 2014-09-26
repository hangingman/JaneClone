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

#include "threadcontentwebview.hpp"
#include "janeclone.hpp"
#include "script/onload_js.hpp"

IMPLEMENT_DYNAMIC_CLASS(ThreadContentWindow, wxWebView)

BEGIN_EVENT_TABLE(ThreadContentWindow, wxWebView)
END_EVENT_TABLE()

/**
 * 通常のコンストラクタ
 */
ThreadContentWindow::ThreadContentWindow(wxWindow* parent, const wxString& threadContentPath):
wxHtmlWindow(parent, ID_ThreadContentWindow, wxDefaultPosition, wxDefaultSize, wxHW_SCROLLBAR_AUTO) {

     // ブラウザの準備
     wxBoxSizer* topsizer = new wxBoxSizer(wxVERTICAL);
     m_browser = wxWebView::New(this, wxID_ANY);
     topsizer->Add(m_browser, wxSizerFlags().Expand().Proportion(1));
     SetSizer(topsizer);

#ifdef __WXMSW__
     // ウィンドウ内に入った際のイベント通知
     this->Connect(ID_ThreadContentWindow,
		   wxEVT_ENTER_WINDOW,
		   wxMouseEventHandler(JaneClone::OnEnterWindow),
		   NULL, this);
#endif

     // 指定されたパスからHTMLファイルを読み出す
     wxString htmlSource = GetConvertedDatFile(threadContentPath);

     // メモリに読み込んだHTMLを表示する
     m_browser->SetPage(htmlSource, wxEmptyString);
     wxMessageBox(onload_js);
     m_browser->RunScript(onload_js);
     
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
     wxString fontName  = wxEmptyString;

     if (!useSkin) {

#ifdef __WXMSW__
	  fontName = wxT("ＭＳ Ｐゴシック");
#elif  __WXGTK__
	  wxFont f;
	  if (f.SetFaceName(wxT("Mona"))) {
	       fontName = wxT("Mona");
	  } else if (f.SetFaceName(wxT("Mona-VLGothic"))) {
	       fontName = wxT("Mona-VLGothic")
	  } else if (f.SetFaceName(wxT("Mona-sazanami"))) {
	       fontName = wxT("Mona-sazanami");
	  }

#elif  __WXMAC__
	  fontName = wxT("Mona");
#endif
     }

     // 取得サイズ分だけwxStringを確保する
     wxString htmlSource;
     htmlSource.Alloc(fileSize);

     if (useSkin && skinInfo->header != wxEmptyString) {
	  htmlSource += skinInfo->header;
     } else {
	  htmlSource += CUSTOM_HTML_HEADER;

	  // embeded javascript
	  htmlSource += onload_js;
	  
	  htmlSource += wxT("<body bgcolor=#efefef text=black link=blue alink=red vlink=#660099");
	  htmlSource += wxT(" style=\" font-family: ");
	  htmlSource += fontName;
          htmlSource += wxT("\">");
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
     wxString rest = wxEmptyString;
     long res = 0;
     
     // bmp,jpg,jpeg,png,gifなどの拡張子が末尾に付いている場合ダウンロードを行う
     if (regexImage.IsValid()) {
	  // 正規表現のコンパイルにエラーがなければマッチさせる
	  if (regexImage.Matches(href)) {
	       // 画像ファイルをクリックしたのでダウンロードする
	       const wxString ext = regexImage.GetMatch(href, 3);
	       this->SetJaneCloneImageViewer(href, ext);

	  } else if (href.StartsWith(wxT("#"), &rest) && rest.ToLong(&res, 10)) {
	       if ( 0 < res && res <= 1000) {
		    // OnLeftClickResponseNumber(event, href, res);
	       }
	  } else {
	       // 通常のリンクかどうか判定して処理
	       OnClickOrdinaryLink(href);
	  }
     }
}
/**
 * リンクが2chのものかどうか判定
 */
void ThreadContentWindow::OnClickOrdinaryLink(const wxString& link) {

     // デフォルトのブラウザを使用するかどうか
     bool useDefaultBrowser = true;
     // URLからホスト名を取得する
     PartOfURI uri;
     bool ret = JaneCloneUtil::SubstringURI(link, &uri);
     
     if (ret) {
	  if (uri.hostname.Contains(wxT("2ch.net"))) useDefaultBrowser = false;
     } else {
	  // エラー、とりあえずブラウザで開く
	  useDefaultBrowser = true;
     }

     if (useDefaultBrowser) {
	  wxLaunchDefaultBrowser(link);
     } else {
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

	  if (urlIsThread) {
	       if (container.size() < 4) {
		    // ERROR
		    wxLaunchDefaultBrowser(link);
		    return;
	       }
	       
	       boardNameAscii = wxString(container.at(3).c_str(), wxConvUTF8);
	       boardURL       = wxT("http://") + host + wxT("/") + boardNameAscii;
	       origNumber     = wxString(container.at(4).c_str(), wxConvUTF8);

	  } else {
	       if (container.size() < 1) {
		    // ERROR
		    wxLaunchDefaultBrowser(link);
		    return;
	       }
	       boardNameAscii = wxString(container.at(1).c_str(), wxConvUTF8);
	       boardURL       = wxT("http://") + host + wxT("/") + boardNameAscii;
	  }

	  if (wxWindow* grand = this->GetGrandParent()) {

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
			 for (it = wxJaneClone->retainHash.begin(); it != wxJaneClone->retainHash.end(); ++it) {
			      wxString key  = it->first;
			      boardInfoHash = it->second;

			      if (boardInfoHash.boardNameAscii == boardNameAscii) {
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
			 if (vbListCtrl == NULL) {
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

			 if (it2 != vbListCtrl->m_vBoardList.end()) {

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

			 } else {
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

     // FIXME: JaneClone-1.1.3
     //wxMemoryFSHandler::RemoveFile(filename);

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

     // FIXME: JaneClone-1.1.3
     //wxMemoryFSHandler::AddFile(filename, bitmap, type);
     // 現在位置の取得とスクロール
     int x, y; 
     GetViewStart(&x, &y);
     this->AppendToPage(wxEmptyString);
     Scroll(x, y);

     delete result;
}
/**
 * スキン用のファイルが有るかどうか確認する
 */
bool ThreadContentWindow::CheckSkinFiles(SkinInfo* skin) {

     // スキン用のパスが設定されていなければ即リターン
     const wxString key = wxT("DEFAULT_SKINFILE_PATH");
     wxString skinPath = wxEmptyString;
     JaneCloneUtil::GetJaneCloneProperties(key, &skinPath);
     bool ret = false;

     if (skinPath == wxEmptyString) {
	  return false;
     }

     if (!wxDir::Exists(skinPath)) {
	  wxMessageBox(wxT("スキン用のディレクトリが存在しません、設定画面を開いてスキンのパス設定を確認してください。"), 
		       wxT("スキン設定"), wxICON_ERROR);
	  return false;
     }

     // Footer.html
     if (wxFile::Exists(skinPath + wxFileSeparator + wxT("Footer.html"))) {
	  const wxString filePath = skinPath + wxFileSeparator + wxT("Footer.html");
	  skin->footer = ReadPlainTextFile(filePath);
	  ret = true;
     }	  
     // Header.html
     if (wxFile::Exists(skinPath + wxFileSeparator + wxT("Header.html"))) {
	  const wxString filePath = skinPath + wxFileSeparator + wxT("Header.html");
	  skin->header = ReadPlainTextFile(filePath);
	  ret = true;
     }
     // NewRes.html
     if (wxFile::Exists(skinPath + wxFileSeparator + wxT("NewRes.html"))) {
	  const wxString filePath = skinPath + wxFileSeparator + wxT("NewRes.html");
	  skin->newres = ReadPlainTextFile(filePath);
	  ret = true;
     }
     // PopupRes.html
     if (wxFile::Exists(skinPath + wxFileSeparator + wxT("PopupRes.html"))) {
	  const wxString filePath = skinPath + wxFileSeparator + wxT("PopupRes.html");
	  skin->popup = ReadPlainTextFile(filePath);
	  ret = true;
     }
     // Res.html
     if (wxFile::Exists(skinPath + wxFileSeparator + wxT("Res.html"))) {
	  const wxString filePath = skinPath + wxFileSeparator + wxT("Res.html");
	  skin->res = ReadPlainTextFile(filePath);
	  ret = true;
     }
     // ***.js
     // TODO: SpiderMonkeyの適用

     if (ret) {  
	  wxString message = wxT("スキンを適用します\n");
	  SendLogging(message);
     }
     
     return ret;
}
/**
 * 指定されたファイル中のテキストをメモリに展開する
 */
wxString ThreadContentWindow::ReadPlainTextFile(const wxString& filePath) {

     wxTextFile textFile;
     wxString   htmlDOM;
     textFile.Open(filePath, wxConvUTF8);

     for (int i = 0; i < textFile.GetLineCount(); i++) {
	  htmlDOM += textFile[i];
     }

     return htmlDOM;
}

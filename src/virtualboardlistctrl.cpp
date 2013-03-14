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

#include "virtualboardlistctrl.hpp"

IMPLEMENT_DYNAMIC_CLASS(VirtualBoardListCtrl, wxListCtrl)

/**
 * コンストラクタ：配置するwindowと板名を指定
 * @param wxWindow* parent     親ウィンドウ
 * @param wxString boardName   板名
 * @param wxString outputPath  datファイルのパス
 */
VirtualBoardListCtrl::VirtualBoardListCtrl(wxWindow* parent,
					   const wxString& boardName, const wxString& outputPath) :
wxListCtrl(parent, ID_BOARDLISTCTRL, wxDefaultPosition, wxDefaultSize,
	   wxLC_REPORT | wxLC_VIRTUAL) {

     this->Hide();
     // クラス自体の目印を設置する
     SetLabel(boardName);
     // テキストファイルの読み込み
     wxTextFile datfile(outputPath);
     datfile.Open();

     // スレッド一覧読み込み用正規表現を準備する
     wxRegEx reThreadLine(_T("([[:digit:]]+).dat<>(.+)\\(([[:digit:]]{1,4})\\)"),
			  wxRE_ADVANCED + wxRE_ICASE);
     // スレッドに番号をつける
     int loopNumber = 1;

     // テキストファイルの終端まで読み込む
     for (wxString line = datfile.GetFirstLine(); !datfile.Eof(); line = datfile.GetNextLine()) {

	  if (line.Contains(_("&"))) { 
	       line = convCharacterReference(line);
	  }
	  // アイテム用の文字列を先に宣言する
	  wxString itemNumber, itemBoardName, itemOid, itemSince, itemTitle, itemResponse, itemCachedResponseNumber,
	       itemNewResponseNumber, itemIncreaseResponseNumber, itemMomentum, itemLastUpdate;

	  // 番号
	  itemNumber = wxString::Format(wxT("%i"), loopNumber);
	  // 板名
	  itemBoardName = boardName;

	  // 正規表現で情報を取得する
	  if (reThreadLine.Matches(line)) {
	       // キー値を取得する
	       itemOid = reThreadLine.GetMatch(line, 1);
	       // since
	       itemSince = JaneCloneUtil::CalcThreadCreatedTime(itemOid);
	       // スレタイを取得する
	       itemTitle = reThreadLine.GetMatch(line, 2);
	       // レス数を取得する
	       itemResponse = reThreadLine.GetMatch(line, 3);
	  }

	  /**
	   * 新規ダウンロードの場合多くは空白となる
	   */
	  // 取得
	  itemCachedResponseNumber = wxEmptyString;
	  // 新着
	  itemNewResponseNumber = wxEmptyString;
	  // 増レス
	  itemIncreaseResponseNumber = wxEmptyString;
	  // 勢い
	  itemMomentum = wxEmptyString;
	  // 最終取得
	  itemLastUpdate = wxEmptyString;

	  // リストにアイテムを挿入する
	  m_vBoardList.push_back(VirtualBoardListItem(itemNumber, itemTitle, itemResponse, itemCachedResponseNumber,
						      itemNewResponseNumber, itemIncreaseResponseNumber, itemMomentum,
						      itemLastUpdate, itemSince, itemOid, itemBoardName));

	  // ループ変数をインクリメント
	  loopNumber++;
     }

     /**
      * データ挿入
      */

     // データを挿入
     SetItemCount(m_vBoardList.size());

     InsertColumn(COL_NUM, wxT("番号"));
     InsertColumn(COL_TITLE, wxT("タイトル"));
     InsertColumn(COL_RESP, wxT("レス"));
     InsertColumn(COL_CACHEDRES, wxT("取得"));
     InsertColumn(COL_NEWRESP, wxT("新着"));
     InsertColumn(COL_INCRESP, wxT("増レス"));
     InsertColumn(COL_MOMENTUM, wxT("勢い"));
     InsertColumn(COL_LASTUP, wxT("最終取得"));
     InsertColumn(COL_SINCE, wxT("SINCE"));
     InsertColumn(COL_OID, wxT("固有番号"));
     InsertColumn(COL_BOARDNAME, wxT("板"));

     // フラグを全てtrueに設定する
     f_number = true;
     f_title = true;
     f_response = true;
     f_cachedResponseNumber = true;
     f_newResponseNumber = true;
     f_increaseResponseNumber = true;
     f_momentum = true;
     f_lastUpdate = true;
     f_since = true;
     f_oid = true;
     f_boardName = true;

     this->Show();
}
/**
 * コンストラクタ：ログ一覧リスト作成用
 * @param wxWindow* parent     親ウィンドウ
 * @param wxString boardName   板名(ログ一覧で固定)
 * @param wxString outputPath  datファイルのパス
 */
VirtualBoardListCtrl::VirtualBoardListCtrl(wxWindow* parent,
					   const wxString& boardName, const wxArrayString& datFileList) :
     wxListCtrl(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
		wxLC_REPORT | wxLC_VIRTUAL) {

     this->Hide();

     /**
      * datファイルの数だけ処理を繰り返す
      */
     for (unsigned int i = 0; i < datFileList.GetCount(); i++) {
	  // テキストファイルの読み込み
	  wxTextFile datfile(datFileList[i]);
	  datfile.Open();

	  // アイテム用の文字列を先に宣言する
	  wxString itemNumber, itemBoardName, itemOid, itemSince, itemTitle, itemResponse, itemCachedResponseNumber,
	       itemNewResponseNumber, itemIncreaseResponseNumber, itemMomentum, itemLastUpdate;

	  // スレッドに番号をつける
	  int loopNumber = i + 1;

	  // datファイルの１行目だけ読み込む
	  wxString line = datfile.GetFirstLine();

	  if (line.Contains(_("&"))) { 
	       line = convCharacterReference(line);
	  }
	  /**
	   * リストに値を設定する
	   */

	  // 番号
	  itemNumber = wxString::Format(wxT("%i"), loopNumber);
	  // 板名
	  //item.boardName = boardName;
	  // キー値を取得する
	  wxFileName* filename = new wxFileName(datFileList[i], wxPATH_NATIVE);
	  itemOid = filename->GetName();
	  delete filename;
	  // since
	  itemSince = JaneCloneUtil::CalcThreadCreatedTime(itemOid);
	  // スレタイを取得する
	  if (regexThreadFst.Matches(line)) {
	       itemTitle = regexThreadFst.GetMatch(line, 5);
	  }
	  // レス数を取得する
	  itemResponse = wxString::Format(wxT("%i"), datfile.GetLineCount());

	  /**
	   *　ログ一覧なのでこの辺は空白でいいんじゃないだろうか
	   */
	  // 取得
	  itemCachedResponseNumber = wxEmptyString;
	  // 新着
	  itemNewResponseNumber = wxEmptyString;
	  // 増レス
	  itemIncreaseResponseNumber = wxEmptyString;
	  // 勢い
	  itemMomentum = wxEmptyString;
	  // 最終取得
	  itemLastUpdate = wxEmptyString;

	  // リストにアイテムを挿入する
	  m_vBoardList.push_back(VirtualBoardListItem(itemNumber, itemTitle, itemResponse, itemCachedResponseNumber,
						      itemNewResponseNumber, itemIncreaseResponseNumber, itemMomentum,
						      itemLastUpdate, itemSince, itemOid, itemBoardName));

	  // ループ変数をインクリメント
	  loopNumber++;
     }

     /**
      * データ挿入
      */

     // データを挿入
     SetItemCount(m_vBoardList.size());

     InsertColumn(COL_NUM, wxT("番号"));
     InsertColumn(COL_TITLE, wxT("タイトル"));
     InsertColumn(COL_RESP, wxT("レス"));
     InsertColumn(COL_CACHEDRES, wxT("取得"));
     InsertColumn(COL_NEWRESP, wxT("新着"));
     InsertColumn(COL_INCRESP, wxT("増レス"));
     InsertColumn(COL_MOMENTUM, wxT("勢い"));
     InsertColumn(COL_LASTUP, wxT("最終取得"));
     InsertColumn(COL_SINCE, wxT("SINCE"));
     InsertColumn(COL_OID, wxT("固有番号"));
     InsertColumn(COL_BOARDNAME, wxT("板"));

     this->Show();
}
/**
 * 内部リストの更新処理
 * @param wxString boardName   板名
 * @pram  wxString outputPath  datファイルのパス
 * @param VirtualBoardList     更新したリストのコンテナ
 */
VirtualBoardList VirtualBoardListCtrl::Refresh(const wxString& boardName, const wxString& outputPath) {

     this->Hide();

     // 内部にデータがあればリストをクリアする
     if ( ! m_vBoardList.empty()) {
	  m_vBoardList.clear();
     }

     // テキストファイルの読み込み
     wxTextFile datfile(outputPath);
     datfile.Open();

     // スレッド一覧読み込み用正規表現を準備する
     wxRegEx reThreadLine(_T("([[:digit:]]+).dat<>(.+)\\(([[:digit:]]{1,4})\\)"),
			  wxRE_ADVANCED + wxRE_ICASE);
     // スレッドに番号をつける
     int loopNumber = 1;

     // テキストファイルの終端まで読み込む
     for (wxString line = datfile.GetFirstLine(); !datfile.Eof(); line = datfile.GetNextLine()) {

	  if (line.Contains(_("&"))) { 
	       line = convCharacterReference(line);
	  }

	  // アイテム用の文字列を先に宣言する
	  wxString itemNumber, itemBoardName, itemOid, itemSince, itemTitle, itemResponse, itemCachedResponseNumber,
	       itemNewResponseNumber, itemIncreaseResponseNumber, itemMomentum, itemLastUpdate;

	  // 番号
	  itemNumber = wxString::Format(wxT("%i"), loopNumber);
	  wxMessageBox(itemNumber);
	  // 板名
	  itemBoardName = boardName;

	  // 正規表現で情報を取得する
	  if (reThreadLine.Matches(line)) {
	       // キー値を取得する
	       itemOid = reThreadLine.GetMatch(line, 1);
	       // since
	       itemSince = JaneCloneUtil::CalcThreadCreatedTime(itemOid);
	       // スレタイを取得する
	       itemTitle = reThreadLine.GetMatch(line, 2);
	       // レス数を取得する
	       itemResponse = reThreadLine.GetMatch(line, 3);
	  }

	  /**
	   * 新規ダウンロードの場合多くは空白となる
	   */
	  // 取得
	  itemCachedResponseNumber = wxEmptyString;
	  // 新着
	  itemNewResponseNumber = wxEmptyString;
	  // 増レス
	  itemIncreaseResponseNumber = wxEmptyString;
	  // 勢い
	  itemMomentum = wxEmptyString;
	  // 最終取得
	  itemLastUpdate = wxEmptyString;

	  // リストにアイテムを挿入する
	  m_vBoardList.push_back(VirtualBoardListItem(itemNumber, itemTitle, itemResponse, itemCachedResponseNumber,
						      itemNewResponseNumber, itemIncreaseResponseNumber, itemMomentum,
						      itemLastUpdate, itemSince, itemOid, itemBoardName));

	  // ループ変数をインクリメント
	  loopNumber++;
     }

     /**
      * データ挿入
      */

     // データを挿入
     SetItemCount(m_vBoardList.size());

     InsertColumn(COL_NUM, wxT("番号"));
     InsertColumn(COL_TITLE, wxT("タイトル"));
     InsertColumn(COL_RESP, wxT("レス"));
     InsertColumn(COL_CACHEDRES, wxT("取得"));
     InsertColumn(COL_NEWRESP, wxT("新着"));
     InsertColumn(COL_INCRESP, wxT("増レス"));
     InsertColumn(COL_MOMENTUM, wxT("勢い"));
     InsertColumn(COL_LASTUP, wxT("最終取得"));
     InsertColumn(COL_SINCE, wxT("SINCE"));
     InsertColumn(COL_OID, wxT("固有番号"));
     InsertColumn(COL_BOARDNAME, wxT("板"));

     this->Show();

     return m_vBoardList;
}

/**
 * wxListCtrl備え付きの仮想関数 ： wxListCtrlを継承したクラスで仮想関数を実装することで使えるようになる
 * 指定されたアイテムとカラムに存在するテキストを返すことができる
 */
wxString VirtualBoardListCtrl::OnGetItemText(long item, long column) const {

     wxString result = wxEmptyString;

     switch (column) {
     
     case COL_NUM:
	  result = m_vBoardList[item].getNumber();
	  break;
     case COL_TITLE:
	  result = m_vBoardList[item].getTitle();
	  break;
     case COL_RESP:
	  result = m_vBoardList[item].getResponse();
	  break;
     case COL_CACHEDRES:
	  result = m_vBoardList[item].getCachedResponseNumber();
	  break;
     case COL_NEWRESP:
	  result = m_vBoardList[item].getNewResponseNumber();
	  break;
     case COL_INCRESP:
	  result = m_vBoardList[item].getIncreaseResponseNumber();
	  break;
     case COL_MOMENTUM:
	  result = m_vBoardList[item].getMomentum();
	  break;
     case COL_LASTUP:
	  result = m_vBoardList[item].getLastUpdate();
	  break;
     case COL_SINCE:
	  result = m_vBoardList[item].getSince();
	  break;
     case COL_OID:
	  result = m_vBoardList[item].getOid();
	  break;
     case COL_BOARDNAME:
	  result = m_vBoardList[item].getBoardName();
	  break;
     default:
	  wxFAIL_MSG("板一覧リストからデータを取り出す処理に失敗しました");
	  break;
     }

     return result;
}
/**
 * 内部のリストをソートする
 */
void VirtualBoardListCtrl::SortVectorItems(int col) {
     
     
     this->Hide();

     // 要素を一度全て削除する
     DeleteAllItems();

     // カラムの位置によって分岐させる
     switch (col) {
     
     case COL_NUM:
	  f_number ? std::sort(m_vBoardList.begin(), m_vBoardList.end(), VirtualBoardListItem::PredicateReverseNumber)
	       : std::sort(m_vBoardList.begin(), m_vBoardList.end(), VirtualBoardListItem::PredicateForwardNumber);
	  f_number ? f_number = false
	       : f_number = true;
	  break;

     case COL_TITLE:
	  f_title ? std::sort(m_vBoardList.begin(), m_vBoardList.end(), VirtualBoardListItem::PredicateReverseTitle)
	       : std::sort(m_vBoardList.begin(), m_vBoardList.end(), VirtualBoardListItem::PredicateForwardTitle);
	  f_title ? f_title = false
	       : f_title = true;
	  break;

     case COL_RESP:

          f_response ? std::sort(m_vBoardList.begin(), m_vBoardList.end(), VirtualBoardListItem::PredicateReverseResponse)
	       : std::sort(m_vBoardList.begin(), m_vBoardList.end(), VirtualBoardListItem::PredicateForwardResponse);
	  f_response ? f_response = false
	       : f_response = true; 

	  break;
     case COL_CACHEDRES:
	  
	  f_cachedResponseNumber ? std::sort(m_vBoardList.begin(), m_vBoardList.end(), VirtualBoardListItem::PredicateReverseCachedResponseNumber)
	       : std::sort(m_vBoardList.begin(), m_vBoardList.end(), VirtualBoardListItem::PredicateForwardCachedResponseNumber);
	  f_cachedResponseNumber ? f_cachedResponseNumber = false
	       : f_cachedResponseNumber = true;

	  break;

     case COL_NEWRESP:
	  
	  f_newResponseNumber ? std::sort(m_vBoardList.begin(), m_vBoardList.end(), VirtualBoardListItem::PredicateReverseNewResponseNumber)
	       : std::sort(m_vBoardList.begin(), m_vBoardList.end(), VirtualBoardListItem::PredicateForwardNewResponseNumber);
	  f_newResponseNumber ? f_newResponseNumber = false
	       : f_newResponseNumber = true;

	  break;
     case COL_INCRESP:
	  
	  f_increaseResponseNumber ? std::sort(m_vBoardList.begin(), m_vBoardList.end(), VirtualBoardListItem::PredicateReverseIncreaseResNum)
	       : std::sort(m_vBoardList.begin(), m_vBoardList.end(), VirtualBoardListItem::PredicateForwardIncreaseResNum);
	  f_increaseResponseNumber ? f_increaseResponseNumber = false
	       : f_increaseResponseNumber = true;

	  break;
     case COL_MOMENTUM:

	  f_momentum ? std::sort(m_vBoardList.begin(), m_vBoardList.end(), VirtualBoardListItem::PredicateReverseMomentum)
	       : std::sort(m_vBoardList.begin(), m_vBoardList.end(), VirtualBoardListItem::PredicateForwardMomentum);
	  f_momentum ? f_momentum = false
	       : f_momentum = true;

	  break;
     case COL_LASTUP:
	  
	  f_lastUpdate ? std::sort(m_vBoardList.begin(), m_vBoardList.end(), VirtualBoardListItem::PredicateReverseLastUpdate)
	       : std::sort(m_vBoardList.begin(), m_vBoardList.end(), VirtualBoardListItem::PredicateForwardLastUpdate);
	  f_lastUpdate ? f_lastUpdate = false
	       : f_lastUpdate = true;

	  break;
     case COL_SINCE:
	  
	  f_since ? std::sort(m_vBoardList.begin(), m_vBoardList.end(), VirtualBoardListItem::PredicateReverseSince)
	       : std::sort(m_vBoardList.begin(), m_vBoardList.end(), VirtualBoardListItem::PredicateForwardSince);
	  f_since ? f_since = false
	       : f_since = true;

	  break;
     case COL_OID:
	  
	  f_since ? std::sort(m_vBoardList.begin(), m_vBoardList.end(), VirtualBoardListItem::PredicateReverseOid)
	       : std::sort(m_vBoardList.begin(), m_vBoardList.end(), VirtualBoardListItem::PredicateForwardOid);
	  f_since ? f_since = false
	       : f_since = true;

	  break;
     case COL_BOARDNAME:
	  f_boardName ? std::sort(m_vBoardList.begin(), m_vBoardList.end(), VirtualBoardListItem::PredicateReverseSince)
	       : std::sort(m_vBoardList.begin(), m_vBoardList.end(), VirtualBoardListItem::PredicateForwardSince);
	  f_since ? f_since = false
	       : f_since = true;

	  break;
     default:
	  wxFAIL_MSG("板一覧リストのソートに失敗しました");
	  break;
     }

     SetItemCount(m_vBoardList.size());
     this->Show();
}
/**
 * 文字列中の実体参照文字を変換する
 */
wxString VirtualBoardListCtrl::convCharacterReference(wxString& inputString) {

     wxString buffer = inputString;
     buffer.Replace(_T("&nbsp;"), _T(" "), true);
     buffer.Replace(_T("&lt;"), _T("<"), true);
     buffer.Replace(_T("&gt;"), _T(">"), true);
     buffer.Replace(_T("&amp;"), _T("&"), true);
     buffer.Replace(_T("&quot;"), _T("\""), true);
     buffer.Replace(_T("&apos;"), _T("'"), true);
     return buffer;
}

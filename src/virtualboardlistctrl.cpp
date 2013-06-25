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
#include "janeclone.hpp"

IMPLEMENT_DYNAMIC_CLASS(VirtualBoardListCtrl, wxListCtrl)

BEGIN_EVENT_TABLE(VirtualBoardListCtrl, wxListCtrl)
   EVT_ENTER_WINDOW(VirtualBoardListCtrl::MotionEnterWindow)
   EVT_LEAVE_WINDOW(VirtualBoardListCtrl::MotionLeaveWindow)
   EVT_SET_FOCUS(VirtualBoardListCtrl::SetFocus)
END_EVENT_TABLE()

/**
 * コンストラクタ：配置するwindowと板名を指定
 * @param wxWindow*  parent			 親ウィンドウ
 * @param wxWindowID id				 ウィンドウID
 * @param wxString   boardName			 板名
 * @param wxString   outputPath			 datファイルのパス
 * @param map<wxString,ThreadList>& oldThreadMap 古いスレッドの情報を保持するコンテナ
 * @rapam bool	   targetIsShingetsu		 読み込み対象は新月のCSVか
 */
VirtualBoardListCtrl::VirtualBoardListCtrl(wxWindow* parent, 
					   const wxWindowID id,
					   const wxString& boardName,
					   const wxString& outputPath, 
					   const std::map<wxString,ThreadList>& oldThreadMap,
					   bool targetIsShingetsu):

wxListCtrl(parent, id, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_VIRTUAL) {

     this->Hide();
     // クラス自体の目印を設置する
     SetLabel(boardName);
     
     // リストに使用する画像を設定する
     wxImageList* threadImage = new wxImageList(16, 16);
     wxBitmap idx1(threadCheckImg, wxBITMAP_TYPE_PNG);
     threadImage->Add(idx1);
     wxBitmap idx2(threadAddImg, wxBITMAP_TYPE_PNG);
     threadImage->Add(idx2);
     wxBitmap idx3(threadDropImg, wxBITMAP_TYPE_PNG);
     threadImage->Add(idx3);
     wxBitmap idx4(threadNewImg, wxBITMAP_TYPE_PNG);
     threadImage->Add(idx4);
     this->AssignImageList(threadImage, wxIMAGE_LIST_SMALL);
     // 背景色の設定
     m_attr.SetBackgroundColour(*wxLIGHT_GREY);

     // ファイル読み出しメソッドの変更
     targetIsShingetsu ? FileLoadMethodShingetsu(boardName, outputPath, oldThreadMap)
	  : FileLoadMethod2ch(boardName, outputPath, oldThreadMap);

     InsertColumn(COL_CHK, wxT("!"), wxLIST_FORMAT_CENTRE);
     InsertColumn(COL_NUM, wxT("番号"), wxLIST_FORMAT_RIGHT);
     InsertColumn(COL_TITLE, wxT("タイトル"));
     InsertColumn(COL_RESP, wxT("レス"), wxLIST_FORMAT_RIGHT);
     InsertColumn(COL_CACHEDRES, wxT("取得"), wxLIST_FORMAT_RIGHT);
     InsertColumn(COL_NEWRESP, wxT("新着"), wxLIST_FORMAT_RIGHT);
     InsertColumn(COL_INCRESP, wxT("増レス"), wxLIST_FORMAT_RIGHT);
     InsertColumn(COL_MOMENTUM, wxT("勢い"), wxLIST_FORMAT_RIGHT);
     InsertColumn(COL_LASTUP, wxT("最終取得"));
     InsertColumn(COL_SINCE, wxT("SINCE"));
     InsertColumn(COL_OID, wxT("固有番号"));
     InsertColumn(COL_BOARDNAME, wxT("板"));

     // フラグを全てtrueに設定する
     f_check = true;
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
 * 2chのdatファイルを読み出す処理
 */
void VirtualBoardListCtrl::FileLoadMethod2ch(const wxString& boardName, const wxString& outputPath, 
					     const std::map<wxString,ThreadList>& oldThreadMap) {

     // 過去のデータがあるかどうかのフラグを立てる
     bool hasOldData, noNeedToChkThreadState = false;
     wxString stub = wxT("NO_NEED_TO_CHK_THREAD_STATE");
     if (!oldThreadMap.empty()) hasOldData = true;
     if (!(oldThreadMap.find(stub) == oldThreadMap.end())) noNeedToChkThreadState = true;

     // テキストファイルの読み込み
     wxTextFile datfile(outputPath);
     datfile.Open();

     // スレッド一覧読み込み用正規表現を準備する
     wxRegEx reThreadLine(_T("([[:digit:]]+).dat<>(.+)\\(([[:digit:]]{1,4})\\)"), wxRE_ADVANCED + wxRE_ICASE);
     // スレッドに番号をつける
     int loopNumber = 1;

     // テキストファイルの終端まで読み込む
     for (wxString line = datfile.GetFirstLine(); !datfile.Eof(); line = datfile.GetNextLine()) {

	  if (line.Contains(_("&"))) {
	       // 実態参照文字の変換
	       line = JaneCloneUtil::ConvCharacterReference(line);
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

	  if (hasOldData) {
	       // 過去のデータがある
	  } else {
	       // 過去のデータがない

	       // 取得
	       itemCachedResponseNumber   = itemResponse;
	       // 新着
	       itemNewResponseNumber      = itemResponse;
	       // 増レス
	       itemIncreaseResponseNumber = itemResponse;
	  }

	  // 最終取得
	  itemLastUpdate = wxEmptyString;

	  // 勢い
	  itemMomentum = JaneCloneUtil::CalcThreadMomentum(itemResponse, itemOid);

	  // リストにアイテムを挿入する
	  VirtualBoardListItem listItem = VirtualBoardListItem(itemNumber, itemTitle, itemResponse, itemCachedResponseNumber,
							       itemNewResponseNumber, itemIncreaseResponseNumber, itemMomentum,
							       itemLastUpdate, itemSince, itemOid, itemBoardName);
	  // この項目の状態を設定する
	  if (hasOldData) {

	       if (oldThreadMap.find(itemOid) == oldThreadMap.end()) {
		    // 要素がない場合の処理
		    listItem.setCheck(THREAD_STATE_NEW);

	       } else {
		    // 要素がある場合の処理
		    std::map<wxString, ThreadList>::const_iterator it;
		    it = oldThreadMap.find(itemOid);
		    const ThreadList oldThread = it->second;
		    int newResInt = wxAtoi(itemResponse) - oldThread.response;
		    // 取得した情報をリストの項目に設定する
		    wxString oldRes = wxString::Format(wxT("%d"), oldThread.response);
		    wxString newRes = wxString::Format(wxT("%d"), newResInt);

		    listItem.setCachedResponseNumber(oldRes);
		    listItem.setNewResponseNumber(newRes);
		    listItem.setIncreaseResponseNumber(newRes);

		    if (newResInt == 0) {
			 // 新着なし
			 listItem.setCheck(THREAD_STATE_NORMAL);
		    } else {
			 // 新着あり
			 listItem.setCheck(THREAD_STATE_ADD);
		    }
	       }
	  }

	  // 特殊処理・レス数が1000ならばDAT落ち扱い
	  if (itemResponse == wxT("1000")) listItem.setCheck(THREAD_STATE_DROP);
	  // 新着チェックする必要のないJaneClone起動時であればすべて普通に
	  if (noNeedToChkThreadState) listItem.setCheck(THREAD_STATE_NORMAL);

	  // Listctrlに項目を追加する
	  m_vBoardList.push_back(listItem);

	  // ループ変数をインクリメント
	  ++loopNumber;
     }

     datfile.Close();

     /**
      * データ挿入
      */

     // データを挿入
     SetItemCount(m_vBoardList.size());
}
/**
 * 新月ののcsvファイルを読み出す処理
 */
void VirtualBoardListCtrl::FileLoadMethodShingetsu(const wxString& boardName, const wxString& outputPath, 
						   const std::map<wxString,ThreadList>& oldThreadMap) {

     // テキストファイルの読み込み
     wxTextFile csvfile(outputPath);
     csvfile.Open();

     // スレッドに番号をつける
     int loopNumber = 1;

     // テキストファイルの終端まで読み込む
     for (wxString line = csvfile.GetFirstLine(); !csvfile.Eof(); line = csvfile.GetNextLine()) {

	  // アイテム用の文字列を先に宣言する
	  wxString itemNumber, itemBoardName, itemOid, itemSince, itemTitle, itemResponse, itemCachedResponseNumber,
	       itemNewResponseNumber, itemIncreaseResponseNumber, itemMomentum, itemLastUpdate, itemFilename, itemPath, itemUri, itemSize;

	  // lineをコンマで分割する
	  wxStringTokenizer tkz(line, wxT(","));
	  
	  while (tkz.HasMoreTokens()) {

	       itemFilename   = tkz.GetNextToken();
	       itemOid        = tkz.GetNextToken();
	       itemLastUpdate = tkz.GetNextToken();
	       itemPath       = tkz.GetNextToken();
	       itemUri        = tkz.GetNextToken();
	       itemBoardName  = tkz.GetNextToken(); // type
	       itemTitle      = tkz.GetNextToken();
	       itemResponse   = tkz.GetNextToken();
	       itemSize       = tkz.GetNextToken();

	       break;
	  }

	  // 番号
	  itemNumber = wxString::Format(wxT("%i"), loopNumber);
	  // since
	  itemSince = JaneCloneUtil::CalcThreadCreatedTime(itemOid);
	  // 勢い
	  itemMomentum = JaneCloneUtil::CalcThreadMomentum(itemResponse, itemOid);

          // リストにアイテムを挿入する
	  VirtualBoardListItem listItem = VirtualBoardListItem(itemNumber, itemTitle, itemResponse, itemCachedResponseNumber,
							       itemNewResponseNumber, itemIncreaseResponseNumber, itemMomentum,
							       itemLastUpdate, itemSince, itemOid, itemBoardName);
	  listItem.setFilename(itemFilename);

          // Listctrlに項目を追加する
	  m_vBoardList.push_back(listItem);
	  // ループ変数をインクリメント
	  ++loopNumber;

     }

     csvfile.Close();

     /**
      * データ挿入
      */

     // データを挿入
     SetItemCount(m_vBoardList.size());
}
/**
 * 内部リストの更新処理
 * @param wxString boardName   板名
 * @pram  wxString outputPath  datファイルのパス
 * @param VirtualBoardList     更新したリストのコンテナ
 */
VirtualBoardList VirtualBoardListCtrl::ThreadListUpdate(const wxString& boardName, const wxString& outputPath) {

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
	       line = JaneCloneUtil::ConvCharacterReference(line);
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
	   * 更新処理
	   */
	  // 取得
	  itemCachedResponseNumber = wxEmptyString;
	  // 新着
	  itemNewResponseNumber = wxEmptyString;
	  // 増レス
	  itemIncreaseResponseNumber = wxEmptyString;
	  // 勢い
	  itemMomentum = JaneCloneUtil::CalcThreadMomentum(itemResponse, itemOid);
	  // 最終取得
	  itemLastUpdate = wxEmptyString;

	  // リストにアイテムを挿入する
	  VirtualBoardListItem listItem = VirtualBoardListItem(itemNumber, itemTitle, itemResponse, itemCachedResponseNumber,
							       itemNewResponseNumber, itemIncreaseResponseNumber, itemMomentum,
							       itemLastUpdate, itemSince, itemOid, itemBoardName);
	  // この項目の状態を設定する
	  listItem.setCheck(THREAD_STATE_ADD);

	  // Listctrlに項目を追加する
	  m_vBoardList.push_back(listItem);

	  // ループ変数をインクリメント
	  ++loopNumber;
     }

     datfile.Close();

     /**
      * データ挿入
      */

     // データを挿入
     SetItemCount(m_vBoardList.size());

     InsertColumn(COL_CHK, wxT("!"), wxLIST_FORMAT_CENTRE);
     InsertColumn(COL_NUM, wxT("番号"), wxLIST_FORMAT_RIGHT);
     InsertColumn(COL_TITLE, wxT("タイトル"));
     InsertColumn(COL_RESP, wxT("レス"), wxLIST_FORMAT_RIGHT);
     InsertColumn(COL_CACHEDRES, wxT("取得"), wxLIST_FORMAT_RIGHT);
     InsertColumn(COL_NEWRESP, wxT("新着"), wxLIST_FORMAT_RIGHT);
     InsertColumn(COL_INCRESP, wxT("増レス"), wxLIST_FORMAT_RIGHT);
     InsertColumn(COL_MOMENTUM, wxT("勢い"), wxLIST_FORMAT_RIGHT);
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

     case COL_CHK:
	  // アイコン部分には文字列を入れない
	  result = wxEmptyString;
	  break;
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
 * 仮想リスト内のアイコンを表示させる
 */
int VirtualBoardListCtrl::OnGetItemColumnImage(long item, long column) const {

     if (column != COL_CHK) {
	  return -1;
     }
     return m_vBoardList[item].getCheck();
}
/**
 * 仮想リスト内の色情報等の設定
 */
wxListItemAttr *VirtualBoardListCtrl::OnGetItemAttr(long item) const {
    return item % 2 ? NULL : (wxListItemAttr *)&m_attr;
}
/**
 * コンストラクタ：ログ一覧リスト作成用
 * @param wxWindow* parent     親ウィンドウ
 * @param wxString boardName   板名(ログ一覧で固定)
 * @param wxString outputPath  datファイルのパス
 */
VirtualBoardListCtrl::VirtualBoardListCtrl(wxWindow* parent,const wxString& boardName, const wxArrayString& datFileList) :
     wxListCtrl(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,wxLC_REPORT | wxLC_VIRTUAL) {

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
	       line = JaneCloneUtil::ConvCharacterReference(line);
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
	  itemMomentum = JaneCloneUtil::CalcThreadMomentum(itemResponse, itemOid);
	  // 最終取得
	  itemLastUpdate = wxEmptyString;

	  // リストにアイテムを挿入する
	  VirtualBoardListItem listItem = VirtualBoardListItem(itemNumber, itemTitle, itemResponse, itemCachedResponseNumber,
							       itemNewResponseNumber, itemIncreaseResponseNumber, itemMomentum,
							       itemLastUpdate, itemSince, itemOid, itemBoardName);
	  // この項目の状態を設定する
	  listItem.setCheck(THREAD_STATE_ADD);
	  // Listctrlに項目を追加する
	  m_vBoardList.push_back(listItem);

	  // ループ変数をインクリメント
	  ++loopNumber;
	  // ファイルをクローズ
	  datfile.Close();
     }

     /**
      * データ挿入
      */

     // データを挿入
     SetItemCount(m_vBoardList.size());

     InsertColumn(COL_CHK, wxT("!"), wxLIST_FORMAT_CENTRE);
     InsertColumn(COL_NUM, wxT("番号"), wxLIST_FORMAT_RIGHT);
     InsertColumn(COL_TITLE, wxT("タイトル"));
     InsertColumn(COL_RESP, wxT("レス"), wxLIST_FORMAT_RIGHT);
     InsertColumn(COL_CACHEDRES, wxT("取得"), wxLIST_FORMAT_RIGHT);
     InsertColumn(COL_NEWRESP, wxT("新着"), wxLIST_FORMAT_RIGHT);
     InsertColumn(COL_INCRESP, wxT("増レス"), wxLIST_FORMAT_RIGHT);
     InsertColumn(COL_MOMENTUM, wxT("勢い"), wxLIST_FORMAT_RIGHT);
     InsertColumn(COL_LASTUP, wxT("最終取得"));
     InsertColumn(COL_SINCE, wxT("SINCE"));
     InsertColumn(COL_OID, wxT("固有番号"));
     InsertColumn(COL_BOARDNAME, wxT("板"));

     this->Show();
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

     case COL_CHK:
	  f_check ? std::sort(m_vBoardList.begin(), m_vBoardList.end(), VirtualBoardListItem::PredicateReverseCheck)
	       : std::sort(m_vBoardList.begin(), m_vBoardList.end(), VirtualBoardListItem::PredicateForwardCheck);
	  f_check ? f_check = false
	       : f_check = true;
	  break;
     
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
 * スレッドタイトル検索を実施する
 */
void VirtualBoardListCtrl::SearchAndSortItems(const wxString& keyword) {

     this->Hide();

     // 要素を一度全て削除する
     DeleteAllItems();

     VirtualBoardList work;
     
     // 文字列に一致するものをコピーして再構築
     std::copy_if(m_vBoardList.begin(), 
		  m_vBoardList.end(), 
		  back_inserter(work), 
		  [&keyword] (const VirtualBoardListItem& item) -> bool { return item.getTitle().Contains(keyword);});
     // 文字列に一致しないものをコピーして再構築
     std::copy_if(m_vBoardList.begin(), 
		  m_vBoardList.end(), 
		  back_inserter(work), 
		  [&keyword] (const VirtualBoardListItem& item) -> bool { return ! item.getTitle().Contains(keyword);});
     
     // リストを削除して構築し直す
     m_vBoardList.clear();
     m_vBoardList = std::move(work);

     SetItemCount(m_vBoardList.size());
     this->Show();
}




void VirtualBoardListCtrl::MotionEnterWindow(wxMouseEvent& event) {
     event.Skip();
}
void VirtualBoardListCtrl::MotionLeaveWindow(wxMouseEvent& event) {
     event.Skip();
}
void VirtualBoardListCtrl::SetFocus(wxFocusEvent& event) {
     event.Skip();
}

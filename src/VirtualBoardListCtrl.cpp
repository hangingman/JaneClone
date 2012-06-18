/*
 * VirtualBoardListCtrl.cpp
 *
 *  Created on: 2012/06/18
 *      Author: learning
 */

#include "VirtualBoardListCtrl.h"

IMPLEMENT_DYNAMIC_CLASS(VirtualBoardListCtrl, wxListCtrl)

/**
 * コンストラクタ：配置するwindowと板名を指定
 * @param wxWindow* parent     親ウィンドウ
 * @param wxString boardName   板名
 * @param wxString outputPath  datファイルのパス
 */
VirtualBoardListCtrl::VirtualBoardListCtrl(wxWindow* parent,
		const wxString& boardName, const wxString& outputPath) :
		wxListCtrl(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
				wxLC_REPORT | wxLC_VIRTUAL) {

	this->Hide();

	// テキストファイルの読み込み
	wxTextFile datfile(outputPath);
	datfile.Open();

	// スレッド一覧読み込み用正規表現を準備する
	wxRegEx reThreadLine(_T("([[:digit:]]+).dat<>(.+)\\(([[:digit:]]{1,4})\\)"),
			wxRE_ADVANCED + wxRE_ICASE);
	// スレッドに番号をつける
	int loopNumber = 1;

	// テキストファイルの終端まで読み込む
	for (wxString line = datfile.GetFirstLine(); !datfile.Eof();
			line = datfile.GetNextLine()) {

		// アイテム1つ分用意
		VirtualBoardListItem item;

		// 番号
		item.number = wxString::Format(wxT("%i"), loopNumber);
		// 板名
		item.boardName = boardName;

		// 正規表現で情報を取得する
		if (reThreadLine.Matches(line)) {
			// キー値を取得する
			item.oid = reThreadLine.GetMatch(line, 1);
			// since
			item.since = JaneCloneUtil::CalcThreadCreatedTime(item.oid);
			// スレタイを取得する
			item.title = reThreadLine.GetMatch(line, 2);
			// レス数を取得する
			item.response = reThreadLine.GetMatch(line, 3);
		}

		/**
		 * 新規ダウンロードの場合多くは空白となる
		 */
		// 取得
		item.cachedResponseNumber = wxEmptyString;
		// 新着
		item.newResponseNumber = wxEmptyString;
		// 増レス
		item.increaseResponseNumber = wxEmptyString;
		// 勢い
		item.increaseResponseNumber = wxEmptyString;
		// 最終取得
		item.lastUpdate = wxEmptyString;

		// リストにアイテムを挿入する
		m_vBoardList.push_back(item);

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
 * wxListCtrl備え付きの仮想関数 ： wxListCtrlを継承したクラスで仮想関数を実装することで使えるようになる
 * 指定されたアイテムとカラムに存在するテキストを返すことができる
 */
wxString VirtualBoardListCtrl::OnGetItemText(long item, long column) const {

	switch (column) {
	case COL_NUM:
		return m_vBoardList[item].number;
	case COL_TITLE:
		return m_vBoardList[item].title;
	case COL_RESP:
		return m_vBoardList[item].response;
	case COL_CACHEDRES:
		return m_vBoardList[item].cachedResponseNumber;
	case COL_NEWRESP:
		return m_vBoardList[item].newResponseNumber;
	case COL_INCRESP:
		return m_vBoardList[item].increaseResponseNumber;
	case COL_MOMENTUM:
		return m_vBoardList[item].momentum;
	case COL_LASTUP:
		return m_vBoardList[item].lastUpdate;
	case COL_SINCE:
		return m_vBoardList[item].since;
	case COL_OID:
		return m_vBoardList[item].oid;
	case COL_BOARDNAME:
		return m_vBoardList[item].boardName;
	default:
		wxFAIL_MSG("板一覧リストからデータを取り出す処理に失敗しました");
	}
	return wxEmptyString;
}


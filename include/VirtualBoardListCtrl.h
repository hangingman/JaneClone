/*
 * VirtualBoardListCtrl.h
 *
 *  Created on: 2012/06/17
 *      Author: learning
 */

#ifndef VIRTUALBOARDLISTCTRL_H_
#define VIRTUALBOARDLISTCTRL_H_

#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/regex.h>
#include <wx/txtstrm.h>
#include <vector>
#include "JaneCloneUtil.h"

struct VirtualBoardListItem {
	// 番号(単に取得したdatファイルの順序から)
	wxString number;
	// タイトル
	wxString title;
	// レス
	wxString response;
	// 取得
	wxString cachedResponseNumber;
	// 新着
	wxString newResponseNumber;
	// 増レス
	wxString increaseResponseNumber;
	// 勢い
	wxString momentum;
	// 最終取得
	wxString lastUpdate;
	// since
	wxString since;
	// 固有番号(ホスト名にこの番号をつけることでスレッドのURLになる)
	wxString oid;
	// 板
	wxString boardName;
};

typedef std::vector<VirtualBoardListItem> VirtualBoardList;

class WXDLLEXPORT VirtualBoardListCtrl: public wxListCtrl {

	enum Columns {
		COL_NUM = 0, // 番号
		COL_TITLE, // タイトル
		COL_RESP, // レス
		COL_CACHEDRES, // 取得
		COL_NEWRESP, // 新着
		COL_INCRESP, // 増レス
		COL_MOMENTUM, // 勢い
		COL_LASTUP, // 最終取得
		COL_SINCE, // SINCE
		COL_OID, // 固有番号
		COL_BOARDNAME // 板
	};

public:
	/**
	 * コンストラクタ：配置するwindowと板名を指定
	 * @param wxWindow* parent     親ウィンドウ
	 * @param wxString boardName   板名
	 * @param wxString outputPath  datファイルのパス
	 */
	VirtualBoardListCtrl(wxWindow* parent, const wxString& boardName,
			const wxString& outputPath);
	/**
	 * Hash用のコンストラクタ
	 */
	VirtualBoardListCtrl(){}
	VirtualBoardListCtrl(const VirtualBoardListCtrl&){}
	/**
	 * オペレーターに対する参照返し
	 */
	VirtualBoardListCtrl& operator=(const VirtualBoardListCtrl&){return *this;}
	/**
	 * 指定されたアイテムとカラムに存在するテキストを返す
	 * @param long item            インデックス
	 * @param long column          カラム
	 * @return wxString text       セル内のテキスト
	 */
	wxString OnGetItemText(long item, long column) const;

	/**
	 * リスト内部のアイテムの数を返す
	 */
	long GetItemCount() {
		return m_vBoardList.size();
	}
	/**
	 * リスト内部のカラムの数を返す
	 */
	long GetColumnCount() {
		return 11;
	}

private:
	// 内部にあるリスト
	VirtualBoardList m_vBoardList;

	DECLARE_DYNAMIC_CLASS(VirtualBoardListCtrl)
};

#endif /* VIRTUALBOARDLISTCTRL_H_ */

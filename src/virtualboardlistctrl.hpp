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

#ifndef VIRTUALBOARDLISTCTRL_H_
#define VIRTUALBOARDLISTCTRL_H_

#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/regex.h>
#include <wx/txtstrm.h>
#include <vector>
#include "janecloneutil.hpp"

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
		COL_NUM = 0, 	// 番号
		COL_TITLE,		// タイトル
		COL_RESP,		// レス
		COL_CACHEDRES,	// 取得
		COL_NEWRESP,	// 新着
		COL_INCRESP,	// 増レス
		COL_MOMENTUM,	// 勢い
		COL_LASTUP,		// 最終取得
		COL_SINCE,		// SINCE
		COL_OID,		// 固有番号
		COL_BOARDNAME	// 板
	};

public:
	/**
	 * コンストラクタ：配置するwindowと板名を指定
	 * @param wxWindow* parent     親ウィンドウ
	 * @param wxString boardName   板名
	 * @param wxString outputPath  datファイルのパス
	 */
	VirtualBoardListCtrl(wxWindow* parent, const wxString& boardName, const wxString& outputPath);
	/**
	 * コンストラクタ：ログ一覧リスト作成用
	 * @param wxWindow* parent     親ウィンドウ
	 * @param wxString boardName   板名(ログ一覧で固定)
	 * @param wxString outputPath  datファイルのパス
	 */
	VirtualBoardListCtrl(wxWindow* parent, const wxString& boardName, const wxArrayString& datFileList);
	/**
	 * 内部リストの更新処理
	 * @param wxString boardName   板名
	 * @pram  wxString outputPath  datファイルのパス
	 * @param VirtualBoardList     更新したリストのコンテナ
	 */
	VirtualBoardList Refresh(const wxString& boardName, const wxString& outputPath);
	/**
	 * Hash用のコンストラクタ
	 */
	VirtualBoardListCtrl() {}
	VirtualBoardListCtrl(const VirtualBoardListCtrl&) {}
	/**
	 * オペレーターに対する参照返し
	 */
	VirtualBoardListCtrl& operator=(const VirtualBoardListCtrl&) {return *this;}
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

	// 内部にあるリスト
	VirtualBoardList m_vBoardList;

private:

	DECLARE_DYNAMIC_CLASS(VirtualBoardListCtrl)
};

#endif /* VIRTUALBOARDLISTCTRL_H_ */

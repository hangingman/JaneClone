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

#ifndef DATATYPE_H_
#define DATATYPE_H_

// wxHashMap用のクラス -- 2chの板名とURLを対応させる
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
	int momentum;
	// 最終取得
	wxString lastUpdate;
	// since
	wxString since;
	// 板
	wxString boardName;
};

#endif /* DATATYPE_H_ */

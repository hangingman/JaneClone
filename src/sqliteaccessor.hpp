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

#ifndef SQLITEACCESSOR_HPP_
#define SQLITEACCESSOR_HPP_

#include <iostream>
#include <wx/wx.h>
#include <wx/dir.h>
#include <wx/file.h>
#include <wx/datetime.h>
#include <wx/wxsqlite3.h>
#include "datatype.hpp"

// SQLiteのデータベースファイルのパス
#ifdef __WXMSW__
static const wxString SQLITE_FILE_PATH = wxT("\\janeclone.db");
#else
static const wxString SQLITE_FILE_PATH = wxT("/janeclone.db");
#endif

class SQLiteAccessor {

public:
     /**
      * Metakitデータベースの初期化・トランザクションあり
      */
     SQLiteAccessor();
     /**
      * 板一覧情報のコミットを行う
      */
     void SetBoardInfoCommit(wxArrayString* boardInfoArray);
     /**
      * 板一覧情報をMetakit内のテーブルから取得しArrayStringの形で返す
      */
     static wxArrayString GetBoardInfo();
     /**
      * ユーザーがJaneClone終了時にタブで開いていた板の名前を登録する
      */
     static void SetUserLookingBoardList(wxArrayString& userLookingBoardListArray);
     /**
      * JaneClone開始時に以前ユーザーがタブで開いていた板の名前を取得する
      */
     static wxArrayString GetUserLookedBoardList();
     /**
      * ユーザーがJaneClone終了時にタブで開いていたスレッドの情報を登録する
      */
     static void SetUserLookingThreadList(wxArrayString& userLookingThreadListArray);
     /**
      * JaneClone開始時に以前ユーザーがタブで開いていたスレッドの情報を取得する
      */
     static wxArrayString GetUserLookedThreadList();
     /**
      * スレタブを閉じた際に情報をSQLiteに格納する
      */
     static void SetClosedThreadInfo(ThreadInfo* t);
     /**
      * 最近閉じたスレッドタブ名を取得する
      */
     static wxArrayString GetClosedThreadInfo();
     /**
      * 板タブを閉じた際に情報をSQLiteに格納する
      */
     static void SetClosedBoardInfo(URLvsBoardName* hash);
     /**
      * 最近閉じた板タブ名リストを取得する
      */
     static wxArrayString GetClosedBoardInfo();
     /**
      * 指定されたテーブルに情報が存在するかどうかを調べるメソッド
      */
     static bool TableHasData(const wxString);
     /**
      * 指定されたテーブルにレコードが何件が存在するかどうかを調べるメソッド
      */
     static int HowManyRecord(const wxString);
     /**
      * 指定されたテーブルを削除する
      */
     static void DropTable(const wxString);
     /**
      * 指定されたテーブルのデータを削除する
      */
     static void DeleteTableData(const wxString);

private:
     wxArrayString* boardInfoArray;
};

#endif /* SQLITEACCESSOR_HPP_ */

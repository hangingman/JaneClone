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
#include <tuple>
#include <wx/wx.h>
#include <wx/dir.h>
#include <wx/file.h>
#include <wx/datetime.h>
#include <wx/wxsqlite3.h>
#include "enums.hpp"
#include "datatype.hpp"

// SQLiteのデータベースファイルのパス
#ifdef __WXMSW__
static const wxString SQLITE_FILE_PATH = wxT("\\janeclone.db");
#else
static const wxString SQLITE_FILE_PATH = wxT("/janeclone.db");
#endif

// SQL処理
#define INITIALIZE_JC_WXSQLITE3(db, now)    \
     try {                                  \
     wxString dbFile = GetDBFilePath();     \
     wxSQLite3Database::InitializeSQLite(); \
     db.Open(dbFile);                       \
     db.Begin();			    \

#define CLOSE_CONN_JC_WXSQLITE3(db) \
     db.Commit(); \
     db.Close();  \
     } catch (wxSQLite3Exception& e) {  \
	  wxMessageBox(e.GetMessage()); \
     }                                  \

#define CLOSE_CONNONLY_JC_WXSQLITE3(db) \
     db.Close();  \
     } catch (wxSQLite3Exception& e) {  \
	  wxMessageBox(e.GetMessage()); \
     }                                  \


class SQLiteAccessor {

public:
     /**
      * SQLiteデータベースの初期化・トランザクションあり
      */
     SQLiteAccessor();
     /**
      * 板一覧情報のコミットを行う
      */
     void SetBoardInfoCommit(wxArrayString* boardInfoArray);
     /**
      * 板一覧情報をSQLite内のテーブルから取得しArrayStringの形で返す
      */
     static wxArrayString GetBoardInfo();
     /**
      * 板一覧情報(カテゴリ一覧)をSQLite内のテーブルから取得しArrayStringの形で返す
      */
     static wxArrayString GetCategoryList();
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
      * ユーザーがスレッドの情報をお気に入りに登録する
      */
     static void SetUserFavoriteThreadList(wxArrayString& userFavoriteThreadListArray);
     /**
      * ユーザーがお気に入りに登録しているスレッドの情報を取得する
      */
     static void GetUserFavoriteThreadList(std::vector<std::tuple<wxString, wxString, wxString>>& favoriteList);
     /**
      * スレタブの情報をSQLiteに格納する
      */
     static void SetThreadInfo(ThreadInfo* t, const wxWindowID id = wxID_ANY);
     /**
      * 最近閉じたスレッドタブ名を取得する
      */
     static wxArrayString GetThreadInfo(const wxWindowID id = wxID_ANY);
     /**
      * スレッドタブ情報を取得する
      * @param number     メニューリストの何番目にあるか
      * @param threadInfo スレッド情報格納用クラス
      * @param id         イベントID
      */
     static void GetThreadFullInfo(const int number, std::unique_ptr<ThreadInfo>& threadInfo, const wxWindowID id);
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
     /**
      * ユーザーが検索ボックスで検索したキーワードをSQLiteに格納する
      */
     static void SetUserSearchedKeyword(const wxString& keyword, const wxWindowID id);
     /**
      * ユーザーが検索ボックスで検索したキーワードを取得する
      */
     static wxArrayString GetUserSearchedKeyword(const wxWindowID id);
     /**
      * 登録済みの新月の公開ノード一覧を取得する
      */
     static wxArrayString GetShingetsuNodeList();
     /**
      * 新月の公開ノードを登録する
      */
     static void SetShingetsuNode(const wxString& nodeURL);
     /**
      * ダウンロードした画像のファイル名とUUIDをデータベースに格納する
      * @param const std::vector<ImageFileInfo>* ファイル情報の配列
      */
     static void SetImageFileName(std::vector<ImageFileInfo>& imageFileInfoArray);
     /**
      * ダウンロードした画像のファイル名とUUIDをデータベースに格納する
      * @param ImageFileInfo& imageFileInfo ファイル情報
      */
     static void SetImageFileName(ImageFileInfo& imageFileInfo);
     /**
      * 画像のファイル名とUUIDのリストをデータベースから取得する
      * @param  const wxArrayString&        画像ファイル名の配列
      * @param  std::vector<ImageFileInfo>& ファイル情報の配列
      * @return bool                        取得可否 true:取得成功 false:取得失敗
      */
     static bool GetImageFileName(const wxArrayString& fileNameArray, std::vector<ImageFileInfo>& imageFileInfoArray);
     /**
      * 画像のファイル名とUUIDのをデータベースから取得する
      * @param  const wxString&        画像ファイル名
      * @param  ImageFileInfo>&        ファイル情報
      * @return bool                   取得可否 true:取得成功 false:取得失敗
      */
     static bool GetImageFileName(const wxString& fileName, ImageFileInfo& imageFileInfo);

private:

     /**
      * JaneCloneが使用するSQLiteのDBファイルの場所を返す
      */
     static wxString GetDBFilePath();

     wxArrayString* boardInfoArray;
};

#endif /* SQLITEACCESSOR_HPP_ */

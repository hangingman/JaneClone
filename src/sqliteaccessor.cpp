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

#include "sqliteaccessor.hpp"

// BOARD_INFOのView内構造
static const char* BOARD_INFO_STRUCTURE =
     "BOARD_INFO[BOARDNAME_KANJI:S,BOARD_URL:S,CATEGORY:S]";
// USER_LOOKING_BOARDLISTのView内構造
static const char* USER_LOOKING_BOARDLIST_STRUCTURE =
     "USER_LOOKING_BOARDLIST[BOARDNAME_KANJI:S]";
// USER_LOOKING_THREADLISTのView内構造
static const char* USER_LOOKING_THREADLIST_STRUCTURE =
     "USER_LOOKING_THREADLIST[THREAD_TITLE:S,THREAD_ORIG_NUM:S,BOARDNAME_ASCII:S]";

/**
 * データベースの初期化
 */
SQLiteAccessor::SQLiteAccessor() {
     // カレントディレクトリを設定
     wxDir dir(wxGetCwd());
     // datフォルダが存在するか確認。無ければ確認＆フォルダを作成
     if (!dir.Exists(wxT("./dat/"))) {
	  ::wxMkdir(wxT("./dat/"));
     }
     // 板一覧情報を入れる配列のインスタンスを作成
     boardInfoArray = new wxArrayString();
     // // dbファイルの初期化
     // wxString dbFile = SQLITE_FILE_PATH;
     // // 空のViewを作る
     // c4_Storage storage(dbFile.mb_str(), true);
     // c4_View vBoardInfo = storage.GetAs(BOARD_INFO_STRUCTURE);
     // storage.Commit();
}
/**
 * 板一覧情報をクラス変数の配列に追加する
 */
void SQLiteAccessor::SetBoardInfo(const wxString category, const wxString name,
				   const wxString url) {
     // それぞれの中身が空でなければ配列に板一覧情報を設定する
     if (name.Length() > 0 && url.Length() > 0 && category.Length() > 0) {
	  boardInfoArray->Add(name);
	  boardInfoArray->Add(url);
	  boardInfoArray->Add(category);
     }
}
/**
 * 板一覧情報のコミットを行う
 */
void SQLiteAccessor::SetBoardInfoCommit() {
     // dbファイルの初期化
     wxString dbFile = SQLITE_FILE_PATH;
     // もしすでにdatファイルが存在していれば削除
     if (wxFile::Exists(dbFile)) {
	  wxRemoveFile(dbFile);
     }
     // // プロパティを用意する（カラム名）
     // c4_StringProp pBoardName("BOARDNAME_KANJI");
     // c4_StringProp pBoardURL("BOARD_URL");
     // c4_StringProp pCategory("CATEGORY");

     // // Viewを得る
     // c4_Storage storage(dbFile.mb_str(), true);
     // // View内のカラムを指定する（コンマとカラム名の間にスペースがあると指定した名前を引けなくなるので注意）
     // c4_View vBoardInfo = storage.GetAs(BOARD_INFO_STRUCTURE);

     // // 配列内のレコードを追加する
     // for (unsigned int i = 0; i < boardInfoArray->GetCount(); i += 3) {
     // 	  // wxStringに一度変換
     // 	  // レコードを追加する
     // 	  c4_Row row;
     // 	  pBoardName(row) = boardInfoArray->Item(i).mb_str();
     // 	  pBoardURL(row) = boardInfoArray->Item(i + 1).mb_str();
     // 	  pCategory(row) = boardInfoArray->Item(i + 2).mb_str();
     // 	  vBoardInfo.Add(row);
     // }
     // // コミット実行
     // storage.Commit();
     // delete boardInfoArray;
}
/**
 * 板一覧情報をMetakit内のテーブルから取得しArrayStringの形で返す
 */
wxArrayString SQLiteAccessor::GetBoardInfo() {

     // dbファイルの初期化
     wxString dbFile = SQLITE_FILE_PATH;
     // // Viewを得る
     // c4_Storage storage(dbFile.mb_str(), true);
     // c4_View vBoardInfo = storage.GetAs(BOARD_INFO_STRUCTURE);
     // c4_String types;

     // for (int i = 0; i < vBoardInfo.NumProperties(); i++) {
     // 	  c4_Property prop = vBoardInfo.NthProperty(i);
     // 	  char t = prop.Type();
     // 	  types += t;// here is set "SSS"
     // }

     // リザルトセットをArrayStringに設定する
     wxArrayString array;

     // for (int j = 0; j < vBoardInfo.GetSize(); ++j) {
     // 	  wxMessageBox(wxString::Format(_("vBoardInfo.GetSize:%d"), vBoardInfo.GetSize()));
     // 	  c4_RowRef r = vBoardInfo[j];

     // 	  c4_StringProp boardName("BOARDNAME_KANJI");
     // 	  const char* test1 = boardName.Get(vBoardInfo[0]);
     // 	  wxMessageBox(wxString::Format(_("%s"), test1));
     // 	  wxMessageBox(wxString(test1, wxConvUTF8));

     // 	  const char* test2 = boardName.Get(vBoardInfo[1]);
     // 	  wxMessageBox(wxString::Format(_("%s"), test2));
     // 	  wxMessageBox(wxString(test2, wxConvUTF8));

     // 	  const char* test3 = boardName.Get(vBoardInfo[2]);
     // 	  wxMessageBox(wxString::Format(_("%s"), test3));
     // 	  wxMessageBox(wxString(test3, wxConvUTF8));
     //}

     return array;
}
/**
 * 指定されたテーブルに情報が存在するかどうか聞く(トランザクション処理なし単独)
 */
bool SQLiteAccessor::TableHasView(const wxString viewName) {
     // dbファイルの初期化
     wxString dbFile = SQLITE_FILE_PATH;
     // // Viewを得る
     // c4_Storage storage(dbFile.mb_str(), true);
     // // View一覧を取得する
     // wxString viewList = wxString(storage.Description(), wxConvUTF8);

     // if (viewList.Contains(viewName)) {
      	  return true;
     // } else {
     // 	  return false;
     // }
}
/**
 * 指定されたテーブルを削除する
 */
void SQLiteAccessor::DropView(const wxString viewName) {
     // dbファイルの初期化
     wxString dbFile = SQLITE_FILE_PATH;
     // // Viewを得る
     // c4_Storage storage(dbFile.mb_str(), true);

     // // 指定されたView名をViewの構造に変えて、中身を削除する
     // if (viewName == wxT("BOARD_INFO")) {
     // 	  // ２ちゃんねるの板一覧情報テーブルの場合
     // 	  c4_View v = storage.GetAs(BOARD_INFO_STRUCTURE);
     // 	  v.RemoveAll();
     // 	  storage.Commit();
     // } else if (viewName == wxT("USER_LOOKING_BOARDLIST")) {
     // 	  // ユーザーが最後に見ていた板の一覧情報の場合
     // 	  c4_View v = storage.GetAs(USER_LOOKING_BOARDLIST_STRUCTURE);
     // 	  v.RemoveAll();
     // 	  storage.Commit();
     // } else if (viewName == wxT("USER_LOOKING_THREADLIST")) {
     // 	  // ユーザーが最後に見ていたスレッドの一覧情報の場合
     // 	  c4_View v = storage.GetAs(USER_LOOKING_THREADLIST_STRUCTURE);
     // 	  v.RemoveAll();
     // 	  storage.Commit();
     // }
}
/**
 * ユーザーがJaneClone終了時にタブで開いていた板の名前を登録する
 */
void SQLiteAccessor::SetUserLookingBoardList(
     wxArrayString& userLookingBoardListArray) {

     // // Viewの中身を削除する
     // DropView(wxT("USER_LOOKING_BOARDLIST"));

     // // dbファイルの初期化
     // wxString dbFile = SQLITE_FILE_PATH;
     // // プロパティを用意する（カラム名）
     // c4_StringProp pBoardName("BOARDNAME_KANJI");
     // // Viewを得る
     // c4_Storage storage(dbFile.mb_str(), true);
     // // View内のカラムを指定する（コンマとカラム名の間にスペースがあると指定した名前を引けなくなるので注意）
     // c4_View vUserLookingBoardList = storage.GetAs(
     // 	  USER_LOOKING_BOARDLIST_STRUCTURE);

     // // 配列内のレコードを追加する
     // for (unsigned int i = 0; i < userLookingBoardListArray.GetCount(); i++) {
     // 	  c4_Row row;
     // 	  pBoardName(row) = userLookingBoardListArray[i].mb_str();
     // 	  vUserLookingBoardList.Add(row);
     // }

     // // コミット実行
     // storage.Commit();
}
/**
 * JaneClone開始時に以前ユーザーがタブで開いていた板の名前を取得する
 */
wxArrayString SQLiteAccessor::GetUserLookedBoardList() {

     // // dbファイルの初期化
     // wxString dbFile = SQLITE_FILE_PATH;
     // // Viewを得る
     // c4_Storage storage(dbFile.mb_str(), true);
     // c4_View vUserLookingBoardList = storage.GetAs(
     // 	  USER_LOOKING_BOARDLIST_STRUCTURE);
     // c4_String types;

     // for (int i = 0; i < vUserLookingBoardList.NumProperties(); i++) {
     // 	  c4_Property prop = vUserLookingBoardList.NthProperty(i);
     // 	  char t = prop.Type();
     // 	  types += t;
     // }

     // リザルトセットをArrayStringに設定する
     wxArrayString array;

     // for (int j = 0; j < vUserLookingBoardList.GetSize(); ++j) {
     // 	  c4_RowRef r = vUserLookingBoardList[j];

     // 	  for (int k = 0; k < types.GetLength(); k++) {
     // 	       c4_Property p = vUserLookingBoardList.NthProperty(k);
     // 	       wxString boardName = wxString((const char*) ((c4_StringProp&) p)(r), wxConvUTF8);
     // 	       array.Add(boardName);
     // 	  }
     // }

     return array;
}
/**
 * ユーザーがJaneClone終了時にタブで開いていたスレッドの情報を登録する
 */
void SQLiteAccessor::SetUserLookingThreadList(
     wxArrayString& userLookingThreadListArray) {

     // // Viewの中身を削除する
     // DropView(wxT("USER_LOOKING_THREADLIST"));

     // // dbファイルの初期化
     // wxString dbFile = SQLITE_FILE_PATH;
     // // プロパティを用意する（カラム名）
     // c4_StringProp pThreTitle("THREAD_TITLE");
     // c4_StringProp pOrigNum("THREAD_ORIG_NUM");
     // c4_StringProp pBoardNameAscii("BOARDNAME_ASCII");

     // // Viewを得る
     // c4_Storage storage(dbFile.mb_str(), true);
     // // View内のカラムを指定する（コンマとカラム名の間にスペースがあると指定した名前を引けなくなるので注意）
     // c4_View vUserLookingThreadList = storage.GetAs(USER_LOOKING_THREADLIST_STRUCTURE);

     // // 配列内のレコードを追加する
     // for (unsigned int i = 0; i < userLookingThreadListArray.GetCount(); i += 3) {
     // 	  c4_Row row;

     // 	  unsigned int confirm = i;
     // 	  if (confirm+2 > userLookingThreadListArray.GetCount()) {
     // 	       // インデックスの外まで見に行ってしまうのを防ぐ
     // 	       continue;
     // 	  }

     // 	  pThreTitle(row) = userLookingThreadListArray[i].mb_str();
     // 	  pOrigNum(row) = userLookingThreadListArray[i+1].mb_str();
     // 	  pBoardNameAscii(row) = userLookingThreadListArray[i+2].mb_str();

     // 	  vUserLookingThreadList.Add(row);
     // }

     // // コミット実行
     // storage.Commit();
}
/**
 * JaneClone開始時に以前ユーザーがタブで開いていたスレッドの情報を取得する
 */
wxArrayString SQLiteAccessor::GetUserLookedThreadList() {

     // dbファイルの初期化
     wxString dbFile = SQLITE_FILE_PATH;
     // // Viewを得る
     // c4_Storage storage(dbFile.mb_str(), true);
     // c4_View vUserLookingThreadList = storage.GetAs(USER_LOOKING_THREADLIST_STRUCTURE);
     // c4_String types;

     // for (int i = 0; i < vUserLookingThreadList.NumProperties(); i++) {
     // 	  c4_Property prop = vUserLookingThreadList.NthProperty(i);
     // 	  char t = prop.Type();
     // 	  types += t;
     // }

     // リザルトセットをArrayStringに設定する
     wxArrayString array;

     // for (int j = 0; j < vUserLookingThreadList.GetSize(); ++j) {
     // 	  c4_RowRef r = vUserLookingThreadList[j];

     // 	  for (int k = 0; k < types.GetLength(); k += 3) {
     // 	       c4_Property p = vUserLookingThreadList.NthProperty(k);
     // 	       wxString title = wxString((const char*) ((c4_StringProp&) p)(r), wxConvUTF8);
     // 	       array.Add(title);

     // 	       p = vUserLookingThreadList.NthProperty(k + 1);
     // 	       wxString origNumber = wxString((const char*) ((c4_StringProp&) p)(r), wxConvUTF8);
     // 	       array.Add(origNumber);

     // 	       p = vUserLookingThreadList.NthProperty(k + 2);
     // 	       wxString boardNameAscii = wxString((const char*) ((c4_StringProp&) p)(r), wxConvUTF8);
     // 	       array.Add(boardNameAscii);
     // 	  }
     // }

     return array;
}



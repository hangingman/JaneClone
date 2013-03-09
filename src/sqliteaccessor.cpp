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

/**
 * データベースの初期化、トランザクションあり
 */
SQLiteAccessor::SQLiteAccessor() {
     
     // dbファイルの初期化
     wxString dbFile = wxGetCwd() + SQLITE_FILE_PATH;

     try {
	  wxSQLite3Database::InitializeSQLite();
	  wxSQLite3Database db;
	  db.Open(dbFile);
	  db.Begin();
	  // 必要なテーブルが無ければ作成
	  db.ExecuteUpdate(wxT("CREATE TABLE IF NOT EXISTS BOARD_INFO(BOARDNAME_KANJI TEXT, BOARD_URL TEXT, CATEGORY TEXT)"));
	  db.ExecuteUpdate(wxT("CREATE TABLE IF NOT EXISTS USER_LOOKING_BOARDLIST(BOARDNAME_KANJI TEXT)"));
	  db.ExecuteUpdate(wxT("CREATE TABLE IF NOT EXISTS USER_LOOKING_THREADLIST(THREAD_TITLE TEXT, THREAD_ORIG_NUM TEXT, BOARDNAME_ASCII TEXT)"));
	  db.Commit();
	  db.Close();

     } catch (wxSQLite3Exception& e) {
	  wxMessageBox(e.GetMessage());
     }
}
/**
 * 板一覧情報をクラス変数の配列に追加する
 */
void SQLiteAccessor::SetBoardInfo(const wxString category, const wxString name,
				  const wxString url) {

     // それぞれの中身が空でなければ配列に板一覧情報を設定する
     this->boardInfoArray = new wxArrayString();

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
     wxString dbFile = wxGetCwd() + SQLITE_FILE_PATH;
     // もしすでにdatファイルが存在していれば削除
     if (wxFile::Exists(dbFile)) {
	  wxRemoveFile(dbFile);
     }
     // dbファイルの初期化
     wxSQLite3Database::InitializeSQLite();
     wxSQLite3Database db;
     db.Open(dbFile);
     db.Begin();

     // PreparedStatementを準備する
     const wxString sqlIn = wxT("INSERT INTO BOARD_INFO (BOARDNAME_KANJI, BOARD_URL, CATEGORY) VALUES (?, ?, ?)");
     wxSQLite3Statement stmt = db.PrepareStatement (sqlIn);

     // 配列内のレコードを追加する
     for (unsigned int i = 0; i < boardInfoArray->GetCount(); i += 3) {
     	  // レコードを追加する
	  stmt.ClearBindings();
	  stmt.Bind(0, boardInfoArray->Item(i));
	  stmt.Bind(1, boardInfoArray->Item(i+1));
	  stmt.Bind(2, boardInfoArray->Item(i+2));
     	  stmt.ExecuteUpdate();
     }
     // コミット実行
     db.Commit();
     db.Close();
     
}
/**
 * 板一覧情報をMetakit内のテーブルから取得しArrayStringの形で返す
 */
wxArrayString SQLiteAccessor::GetBoardInfo() {

     // dbファイルの初期化
     wxString dbFile = wxGetCwd() + SQLITE_FILE_PATH;
     // dbファイルの初期化
     wxSQLite3Database::InitializeSQLite();
     wxSQLite3Database db;
     db.Open(dbFile);

     // リザルトセットをArrayStringに設定する
     wxArrayString array;
     // リザルトセットを用意する
     wxSQLite3ResultSet rs;
     const wxString sqlSe = wxT("SELECT BOARDNAME_KANJI, BOARD_URL, CATEGORY FROM BOARD_INFO");

     // SQL文を実行する
     rs = db.ExecuteQuery(sqlSe);
     db.Close();
     

     while (!rs.Eof()) {
	  wxString boardName = rs.GetAsString(wxT("BOARDNAME_KANJI"));
	  wxString url = rs.GetAsString(wxT("BOARD_URL"));
	  wxString category = rs.GetAsString(wxT("CATEGORY"));

	  // 各項目がNULLで無ければArrayStringに詰める
	  if (boardName.Length() > 0 && url.Length() > 0 && category.Length() > 0) {
	       array.Add(boardName);
	       array.Add(url);
	       array.Add(category);
	  }
	  rs.NextRow();
     }
     return array;
}
/**
 * 指定されたテーブルに情報が存在するかどうか聞く(トランザクション処理なし単独)
 */
bool SQLiteAccessor::TableHasData(const wxString tableName) {

     // dbファイルの初期化
     wxString dbFile = wxGetCwd() + SQLITE_FILE_PATH;

     try {
	  
	  wxSQLite3Database::InitializeSQLite();
	  wxSQLite3Database db;
	  db.Open(dbFile);

	  // リザルトセットを用意する
	  wxSQLite3ResultSet rs;
	  // SQL文を用意する
	  wxString SQL_QUERY = wxT("SELECT COUNT(*) from ") + tableName;

	  // SQL文を実行する
	  rs = db.ExecuteQuery(SQL_QUERY);
	  db.Close();

	  // SQL文を実行し結果を受け取る
	  if (!rs.IsNull(0)) {
	       int recordNum = rs.GetInt(0);
	       if ( 0 == recordNum) {
		    return false;
	       } else {
		    return true;
	       }
	  }

     } catch (wxSQLite3Exception& e) {
	  wxMessageBox(e.GetMessage());
     }

     // ここまで来てしまうとエラーなのでfalse
     return false;
}
/**
 * 指定されたテーブルを削除する
 */
void SQLiteAccessor::DropTable(const wxString tableName) {
     // dbファイルの初期化
     wxString dbFile = wxGetCwd() + SQLITE_FILE_PATH;

     // dbファイルの初期化
     wxSQLite3Database::InitializeSQLite();
     wxSQLite3Database db;
     db.Open(dbFile);
     db.Begin();

     // PreparedStatementを準備する
     const wxString sql = wxT("DROP TABLE ?");
     wxSQLite3Statement stmt = db.PrepareStatement (sql);
     stmt.Bind(0, tableName);
     stmt.ExecuteUpdate();

     // コミット実行
     db.Commit();
     db.Close();
     
}
/**
 * ユーザーがJaneClone終了時にタブで開いていた板の名前を登録する
 */
void SQLiteAccessor::SetUserLookingBoardList(wxArrayString& userLookingBoardListArray) {

     // Tableの中身を削除する
     DropTable(wxT("USER_LOOKING_BOARDLIST"));

     // dbファイルの初期化
     wxString dbFile = wxGetCwd() + SQLITE_FILE_PATH;
     wxSQLite3Database::InitializeSQLite();
     wxSQLite3Database db;
     db.Open(dbFile);
     db.Begin();

     // PreparedStatementを準備する
     const wxString sqlIn = wxT("INSERT INTO USER_LOOKING_BOARDLIST (BOARDNAME_KANJI) VALUES (?)");
     wxSQLite3Statement stmt = db.PrepareStatement (sqlIn);

     // 配列内のレコードを追加する
     for (unsigned int i = 0; i < userLookingBoardListArray.GetCount();i++) {
	  // レコードを追加する
	  stmt.ClearBindings();
	  stmt.Bind(0, userLookingBoardListArray.Item(i));
	  stmt.ExecuteUpdate();
     }
     // コミット実行
     db.Commit();
     db.Close();
     
}
/**
 * JaneClone開始時に以前ユーザーがタブで開いていた板の名前を取得する
 */
wxArrayString SQLiteAccessor::GetUserLookedBoardList() {

     // dbファイルの初期化
     wxString dbFile = wxGetCwd() + SQLITE_FILE_PATH;
     wxSQLite3Database::InitializeSQLite();
     wxSQLite3Database db;
     db.Open(dbFile);

     // リザルトセットを用意する
     wxSQLite3ResultSet rs;
     // SQL文を用意する
     wxString sqlSe = wxT("SELECT BOARDNAME_KANJI from USER_LOOKING_BOARDLIST");

     // SQL文を実行する
     rs = db.ExecuteQuery(sqlSe);
     db.Close();
     
     // リザルトセットをArrayStringに設定する
     wxArrayString array;

     while (!rs.Eof()) {
	  wxString boardName = rs.GetAsString(wxT("BOARDNAME_KANJI"));

	  // 各項目がNULLで無ければArrayStringに詰める
	  if (boardName.Length() > 0) {
	       array.Add(boardName);
	  }
	  rs.NextRow();
     }
     return array;
}
/**
 * ユーザーがJaneClone終了時にタブで開いていたスレッドの情報を登録する
 */
void SQLiteAccessor::SetUserLookingThreadList(wxArrayString& userLookingThreadListArray) {

     // Tableの中身を削除する
     DropTable(wxT("USER_LOOKING_THREADLIST"));

     // dbファイルの初期化
     wxString dbFile = wxGetCwd() + SQLITE_FILE_PATH;
     wxSQLite3Database::InitializeSQLite();
     wxSQLite3Database db;
     db.Open(dbFile);
     db.Begin();

     // PreparedStatementを準備する
     const wxString sqlIn 
	  = wxT("INSERT INTO USER_LOOKING_THREADLIST (THREAD_TITLE, THREAD_ORIG_NUM, BOARDNAME_ASCII) VALUES (?, ?, ?)");
     wxSQLite3Statement stmt = db.PrepareStatement (sqlIn);

     // 配列内のレコードを追加する
     for (unsigned int i = 0; i < userLookingThreadListArray.GetCount();i+=3) {
	  // レコードを追加する
	  stmt.ClearBindings();
	  stmt.Bind(0, userLookingThreadListArray.Item(i));
	  stmt.Bind(1, userLookingThreadListArray.Item(i+1));
	  stmt.Bind(2, userLookingThreadListArray.Item(i+2));
	  stmt.ExecuteUpdate();
     }
     // コミット実行
     db.Commit();
     db.Close();
     
}
/**
 * JaneClone開始時に以前ユーザーがタブで開いていたスレッドの情報を取得する
 */
wxArrayString SQLiteAccessor::GetUserLookedThreadList() {

     // dbファイルの初期化
     wxString dbFile = wxGetCwd() + SQLITE_FILE_PATH;
     wxSQLite3Database::InitializeSQLite();
     wxSQLite3Database db;
     db.Open(dbFile);

     // リザルトセットを用意する
     wxSQLite3ResultSet rs;
     // SQL文を用意する
     wxString sqlSe = wxT("SELECT THREAD_TITLE, THREAD_ORIG_NUM, BOARDNAME_ASCII from USER_LOOKING_THREADLIST");

     // SQL文を実行する
     rs = db.ExecuteQuery(sqlSe);
     db.Close();
     
     // リザルトセットをArrayStringに設定する
     wxArrayString array;

     while (!rs.Eof()) {
	  wxString title = rs.GetAsString(wxT("THREAD_TITLE"));
	  wxString origNumber = rs.GetAsString(wxT("THREAD_ORIG_NUM"));
	  wxString boardNameAscii = rs.GetAsString(wxT("BOARDNAME_ASCII"));

	  // 各項目がNULLで無ければArrayStringに詰める
	  if (title.Length() > 0 && origNumber > 0 && boardNameAscii > 0) {
	       array.Add(title);
	       array.Add(origNumber);
	       array.Add(boardNameAscii);
	  }
	  rs.NextRow();
     }
     return array;
}



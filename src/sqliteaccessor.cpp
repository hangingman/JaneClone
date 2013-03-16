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
	  // 2chのすべての板一覧情報
	  db.ExecuteUpdate(wxT("CREATE TABLE IF NOT EXISTS BOARD_INFO(BOARDNAME_KANJI TEXT, BOARD_URL TEXT, CATEGORY TEXT)"));
	  // ユーザーが前回見ていたタブについての情報
	  db.ExecuteUpdate(wxT("CREATE TABLE IF NOT EXISTS USER_LOOKING_BOARDLIST(BOARDNAME_KANJI TEXT)"));
	  db.ExecuteUpdate(wxT("CREATE TABLE IF NOT EXISTS USER_LOOKING_THREADLIST(THREAD_TITLE TEXT, THREAD_ORIG_NUM TEXT, BOARDNAME_ASCII TEXT)"));
	  // ユーザーが最近閉じたタブについての情報
	  db.ExecuteUpdate(
	       wxT("CREATE TABLE IF NOT EXISTS USER_CLOSED_BOARDLIST(TIMEINFO TIMESTAMP, BOARDNAME_KANJI TEXT, BOARD_URL TEXT, BOARDNAME_ASCII TEXT)"));
	  db.ExecuteUpdate(
	       wxT("CREATE TABLE IF NOT EXISTS USER_CLOSED_THREADLIST(TIMEINFO TIMESTAMP, THREAD_TITLE TEXT, THREAD_ORIG_NUM TEXT, BOARDNAME_ASCII TEXT)"));
	  db.Commit();
	  db.Close();

     } catch (wxSQLite3Exception& e) {
	  wxMessageBox(e.GetMessage());
     }
}
/**
 * 板一覧情報のコミットを行う
 */
void SQLiteAccessor::SetBoardInfoCommit(wxArrayString* boardInfoArray) {

     // dbファイルの初期化
     wxString dbFile = wxGetCwd() + SQLITE_FILE_PATH;

     try {
	  
	  // dbファイルの初期化
	  wxSQLite3Database::InitializeSQLite();
	  wxSQLite3Database db;
	  db.Open(dbFile);
	  db.Begin();

	  // PreparedStatementを準備する
	  const wxString sqlIn = wxT("INSERT INTO BOARD_INFO (BOARDNAME_KANJI, BOARD_URL, CATEGORY) VALUES (?, ?, ?)");
	  wxSQLite3Statement stmt = db.PrepareStatement (sqlIn);

	  for (unsigned int i = 0; i < boardInfoArray->GetCount(); i += 3) {
	       // レコードを追加する
	       stmt.ClearBindings();
	       stmt.Bind(1, boardInfoArray->Item(i));
	       stmt.Bind(2, boardInfoArray->Item(i+1));
	       stmt.Bind(3, boardInfoArray->Item(i+2));
	       stmt.ExecuteUpdate();
	  }
	  // コミット実行
	  db.Commit();
	  db.Close();

     } catch (wxSQLite3Exception& e) {
	  wxMessageBox(e.GetMessage());
     }
}
/**
 * 板一覧情報をMetakit内のテーブルから取得しArrayStringの形で返す
 */
wxArrayString SQLiteAccessor::GetBoardInfo() {

     // dbファイルの初期化
     wxString dbFile = wxGetCwd() + SQLITE_FILE_PATH;

     try {	  
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

     } catch (wxSQLite3Exception& e) {
	  wxMessageBox(e.GetMessage());
     }
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

     try {
	  
	  // dbファイルの初期化
	  wxSQLite3Database::InitializeSQLite();
	  wxSQLite3Database db;
	  db.Open(dbFile);
	  db.Begin();

	  // PreparedStatementを準備する
	  const wxString sql = wxT("DROP TABLE ?");
	  wxSQLite3Statement stmt = db.PrepareStatement (sql);
	  stmt.Bind(1, tableName);
	  stmt.ExecuteUpdate();

	  // コミット実行
	  db.Commit();
	  db.Close();

     } catch (wxSQLite3Exception& e) {
	  wxMessageBox(e.GetMessage());
     }
}
/**
 * 指定されたテーブルのデータを削除する
 */
void SQLiteAccessor::DeleteTableData(const wxString tableName) {

     // dbファイルの初期化
     wxString dbFile = wxGetCwd() + SQLITE_FILE_PATH;

     try {
	  
	  // dbファイルの初期化
	  wxSQLite3Database::InitializeSQLite();
	  wxSQLite3Database db;
	  db.Open(dbFile);
	  db.Begin();
	  // PreparedStatementを準備する
	  const wxString sql = wxT("DELETE FROM ") + tableName;
	  wxSQLite3Statement stmt = db.PrepareStatement(sql);
	  stmt.ExecuteUpdate();
	  // コミット実行
	  db.Commit();
	  db.Close();

     } catch (wxSQLite3Exception& e) {
	  wxMessageBox(e.GetMessage());
     }
}
/**
 * ユーザーがJaneClone終了時にタブで開いていた板の名前を登録する
 */
void SQLiteAccessor::SetUserLookingBoardList(wxArrayString& userLookingBoardListArray) {

     // Tableの中身を削除する
     DeleteTableData(wxT("USER_LOOKING_BOARDLIST"));

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
	  stmt.Bind(1, userLookingBoardListArray.Item(i));
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

     while (rs.NextRow()) {
	  wxString boardName = rs.GetAsString(wxT("BOARDNAME_KANJI"));

	  // 各項目がNULLで無ければArrayStringに詰める
	  if (boardName.Length() > 0) {
	       array.Add(boardName);
	  }
     }
     return array;
}
/**
 * ユーザーがJaneClone終了時にタブで開いていたスレッドの情報を登録する
 */
void SQLiteAccessor::SetUserLookingThreadList(wxArrayString& userLookingThreadListArray) {

     // Tableの中身を削除する
     DeleteTableData(wxT("USER_LOOKING_THREADLIST"));

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
	  stmt.Bind(1, userLookingThreadListArray.Item(i));
	  stmt.Bind(2, userLookingThreadListArray.Item(i+1));
	  stmt.Bind(3, userLookingThreadListArray.Item(i+2));
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

     while (rs.NextRow()) {
	  wxString title = rs.GetAsString(wxT("THREAD_TITLE"));
	  wxString origNumber = rs.GetAsString(wxT("THREAD_ORIG_NUM"));
	  wxString boardNameAscii = rs.GetAsString(wxT("BOARDNAME_ASCII"));

	  // 各項目がNULLで無ければArrayStringに詰める
	  if (title.Length() > 0 && origNumber > 0 && boardNameAscii > 0) {
	       array.Add(title);
	       array.Add(origNumber);
	       array.Add(boardNameAscii);
	  }
     }
     return array;
}
/**
 * スレタブを閉じた際に情報をSQLiteに格納する
 */
void SQLiteAccessor::SetClosedThreadInfo(ThreadInfo* t) {

     // ユーザが閉じたスレッドのうち、データベースに保存されている数
     int userClosedThreadListNum = SQLiteAccessor::HowManyRecord(wxT("USER_CLOSED_THREADLIST"));
     // 現在時間timestamp
     wxDateTime now = wxDateTime::Now();

     // dbファイルの初期化
     wxString dbFile = wxGetCwd() + SQLITE_FILE_PATH;
     wxSQLite3Database::InitializeSQLite();

     try {
	  wxSQLite3Database db;
	  db.Open(dbFile);
	  db.Begin();
	  /** 閉じられたスレタブの情報をインサート */
	  const wxString sqlIn = 
	       wxT("INSERT INTO USER_CLOSED_THREADLIST(TIMEINFO, THREAD_TITLE, THREAD_ORIG_NUM, BOARDNAME_ASCII) VALUES (?,?,?,?)");
	  wxSQLite3Statement stmt = db.PrepareStatement (sqlIn);
	  stmt.ClearBindings();
	  stmt.BindTimestamp(1, now);
	  stmt.Bind(2,t->title);
	  stmt.Bind(3,t->origNumber);
	  stmt.Bind(4,t->boardNameAscii);
	  stmt.ExecuteUpdate();

	  // コミット実行
	  db.Commit();
	  db.Close();

     } catch (wxSQLite3Exception& e) {
	  wxMessageBox(e.GetMessage());
     }
}
/**
 * 最近閉じた板タブ名リストを取得する
 */
wxArrayString SQLiteAccessor::GetClosedBoardInfo() {

     // dbファイルの初期化
     wxString dbFile = wxGetCwd() + SQLITE_FILE_PATH;
     wxArrayString array;

     try {
	  
	  wxSQLite3Database::InitializeSQLite();
	  wxSQLite3Database db;
	  db.Open(dbFile);

	  // リザルトセットを用意する
	  wxSQLite3ResultSet rs;
	  // SQL文を用意する
	  wxString SQL_QUERY = wxT("SELECT BOARDNAME_KANJI from USER_CLOSED_BOARDLIST");

	  // SQL文を実行する
	  rs = db.ExecuteQuery(SQL_QUERY);
	  db.Close();

	  while (rs.NextRow()) {
	       wxString boardName = rs.GetAsString(wxT("BOARDNAME_KANJI"));

	       // 各項目がNULLで無ければArrayStringに詰める
	       if (boardName.Length() > 0) {
		    array.Add(boardName);
	       }
	  }

	  return array;

     } catch (wxSQLite3Exception& e) {
	  wxMessageBox(e.GetMessage());
     }

     return NULL;
}
/**
 * 板タブを閉じた際に情報をSQLiteに格納する
 */
void SQLiteAccessor::SetClosedBoardInfo(URLvsBoardName* hash) {

     // ユーザが閉じた板のうち、データベースに保存されている数
     int userClosedBoardListNum = SQLiteAccessor::HowManyRecord(wxT("USER_CLOSED_BOARDLIST"));
     // 現在時間timestamp
     wxDateTime now = wxDateTime::Now();

     // dbファイルの初期化
     wxString dbFile = wxGetCwd() + SQLITE_FILE_PATH;
     wxSQLite3Database::InitializeSQLite();

     try {
	  wxSQLite3Database db;
	  db.Open(dbFile);
	  db.Begin();
	  /** 閉じられたスレタブの情報をインサート */
	  const wxString sqlIn = 
	       wxT("INSERT INTO USER_CLOSED_BOARDLIST(TIMEINFO, BOARDNAME_KANJI, BOARD_URL, BOARDNAME_ASCII) VALUES (?,?,?,?)");
	  wxSQLite3Statement stmt = db.PrepareStatement (sqlIn);
	  stmt.ClearBindings();
	  stmt.BindTimestamp(1, now);
	  stmt.Bind(2,hash->boardName);
	  stmt.Bind(3,hash->boardURL);
	  stmt.Bind(4,hash->boardNameAscii);
	  stmt.ExecuteUpdate();

	  // コミット実行
	  db.Commit();
	  db.Close();

     } catch (wxSQLite3Exception& e) {
	  wxMessageBox(e.GetMessage());
     }
}
/**
 * 最近閉じたスレタブ名リストを取得する
 */
wxArrayString SQLiteAccessor::GetClosedThreadInfo() {

     // dbファイルの初期化
     wxString dbFile = wxGetCwd() + SQLITE_FILE_PATH;
     wxArrayString array;

     try {
	  
	  wxSQLite3Database::InitializeSQLite();
	  wxSQLite3Database db;
	  db.Open(dbFile);

	  // リザルトセットを用意する
	  wxSQLite3ResultSet rs;
	  // SQL文を用意する
	  wxString SQL_QUERY = wxT("SELECT THREAD_TITLE from USER_CLOSED_THREADLIST");

	  // SQL文を実行する
	  rs = db.ExecuteQuery(SQL_QUERY);
	  db.Close();

	  while (rs.NextRow()) {
	       wxString title = rs.GetAsString(wxT("THREAD_TITLE"));

	       // 各項目がNULLで無ければArrayStringに詰める
	       if (title.Length() > 0) {
		    array.Add(title);
	       }
	  }

	  return array;

     } catch (wxSQLite3Exception& e) {
	  wxMessageBox(e.GetMessage());
     }

     return NULL;
}
/**
 * 指定されたテーブルにレコードが何件存在するかどうかを調べるメソッド
 */
int SQLiteAccessor::HowManyRecord(const wxString tableName) {

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
	       return recordNum;
	  }

     } catch (wxSQLite3Exception& e) {
	  wxMessageBox(e.GetMessage());
     }

     return 0;
} 

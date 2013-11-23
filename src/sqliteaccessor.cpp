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
     wxString dbFile = GetDBFilePath();

     try {
	  wxSQLite3Database::InitializeSQLite();
	  wxSQLite3Database db;
	  db.Open(dbFile);
	  db.Begin();
	  // 2chのすべての板一覧情報
	  db.ExecuteUpdate(wxT("CREATE TABLE IF NOT EXISTS BOARD_INFO(BOARDNAME_KANJI TEXT, BOARD_URL TEXT, CATEGORY TEXT)"));
	  // ユーザーが登録している新月のノード情報
	  db.ExecuteUpdate(wxT("CREATE TABLE IF NOT EXISTS BOARD_INFO_SHINGETSU(TIMEINFO TIMESTAMP, BOARD_URL TEXT)"));
	  // ユーザーが前回見ていたタブについての情報
	  db.ExecuteUpdate(wxT("CREATE TABLE IF NOT EXISTS USER_LOOKING_BOARDLIST(BOARDNAME_KANJI TEXT)"));
	  db.ExecuteUpdate(wxT("CREATE TABLE IF NOT EXISTS USER_LOOKING_THREADLIST(THREAD_TITLE TEXT, THREAD_ORIG_NUM TEXT, BOARDNAME_ASCII TEXT)"));
	  // ユーザーが最近閉じたタブについての情報
	  db.ExecuteUpdate(wxT("CREATE TABLE IF NOT EXISTS USER_CLOSED_BOARDLIST(TIMEINFO TIMESTAMP, BOARDNAME_KANJI TEXT, BOARD_URL TEXT, BOARDNAME_ASCII TEXT)"));
	  db.ExecuteUpdate(wxT("CREATE TABLE IF NOT EXISTS USER_CLOSED_THREADLIST(TIMEINFO TIMESTAMP, THREAD_TITLE TEXT, THREAD_ORIG_NUM TEXT, BOARDNAME_ASCII TEXT)"));
	  db.ExecuteUpdate(wxT("CREATE TABLE IF NOT EXISTS USER_FAVORITE_THREADLIST(TIMEINFO TIMESTAMP, THREAD_TITLE TEXT, THREAD_ORIG_NUM TEXT, BOARDNAME_ASCII TEXT)"));
	  // ユーザーが検索したキーワードを保存する(板一覧ツリー、スレッド一覧リスト、スレッド表示ウィンドウでの検索)
	  db.ExecuteUpdate(wxT("CREATE TABLE IF NOT EXISTS USER_SEARCHED_BOARDNAME(TIMEINFO TIMESTAMP, KEYWORD TEXT)"));
	  db.ExecuteUpdate(wxT("CREATE TABLE IF NOT EXISTS USER_SEARCHED_THREADNAME(TIMEINFO TIMESTAMP, KEYWORD TEXT)"));
	  db.ExecuteUpdate(wxT("CREATE TABLE IF NOT EXISTS USER_SEARCHED_THREADCONTENTS(TIMEINFO TIMESTAMP, KEYWORD TEXT)"));

	  // 画像データの保存先
	  db.ExecuteUpdate(wxT("CREATE TABLE IF NOT EXISTS CACHED_IMAGE(TIMEINFO TIMESTAMP, FILENAME TEXT, UUID_FILENAME TEXT)"));

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
     wxString dbFile = GetDBFilePath();

     try {
	  
	  // dbファイルの初期化
	  wxSQLite3Database::InitializeSQLite();
	  wxSQLite3Database db;
	  db.Open(dbFile);
	  db.Begin();

	  // 既存のデータは消しておく
	  const wxString sqlDel = wxT("DELETE FROM BOARD_INFO");
	  wxSQLite3Statement stmt1 = db.PrepareStatement(sqlDel);
	  stmt1.ExecuteUpdate();	  

	  const wxString sqlIn = wxT("INSERT INTO BOARD_INFO (BOARDNAME_KANJI, BOARD_URL, CATEGORY) VALUES (?, ?, ?)");
	  wxSQLite3Statement stmt2 = db.PrepareStatement (sqlIn);

	  for (unsigned int i = 0; i < boardInfoArray->GetCount(); i += 3) {
	       // レコードを追加する
	       stmt2.ClearBindings();
	       stmt2.Bind(1, boardInfoArray->Item(i));
	       stmt2.Bind(2, boardInfoArray->Item(i+1));
	       stmt2.Bind(3, boardInfoArray->Item(i+2));
	       stmt2.ExecuteUpdate();
	  }
	  // コミット実行
	  db.Commit();
	  db.Close();

     } catch (wxSQLite3Exception& e) {
	  wxMessageBox(e.GetMessage());
     }
}
/**
 * 板一覧情報をSQLite内のテーブルから取得しArrayStringの形で返す
 */
wxArrayString SQLiteAccessor::GetBoardInfo() {

     // dbファイルの初期化
     wxString dbFile = GetDBFilePath();
     // リザルトセットをArrayStringに設定する
     wxArrayString array;

     try {	  
	  // dbファイルの初期化
	  wxSQLite3Database::InitializeSQLite();
	  wxSQLite3Database db;
	  db.Open(dbFile);

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

     return array;
}
/**
 * 指定されたテーブルに情報が存在するかどうか聞く(トランザクション処理なし単独)
 */
bool SQLiteAccessor::TableHasData(const wxString tableName) {

     // dbファイルの初期化
     wxString dbFile = GetDBFilePath();

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
     wxString dbFile = GetDBFilePath();

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
     wxString dbFile = GetDBFilePath();

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
     wxString dbFile = GetDBFilePath();
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
     wxString dbFile = GetDBFilePath();
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
     wxString dbFile = GetDBFilePath();
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
     wxString dbFile = GetDBFilePath();
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
	  if (title.Len() > 0 && origNumber.Len() > 0 && boardNameAscii.Len() > 0) {
	       array.Add(title);
	       array.Add(origNumber);
	       array.Add(boardNameAscii);
	  }
     }
     return array;
}
/**
 * ユーザーがスレッドの情報をお気に入りに登録する
 */
void SQLiteAccessor::SetUserFavoriteThreadList(wxArrayString& userFavoriteThreadListArray) {

     // Tableの中身を削除する
     DeleteTableData(wxT("USER_FAVORITE_THREADLIST"));

     // dbファイルの初期化
     wxString dbFile = GetDBFilePath();
     wxSQLite3Database::InitializeSQLite();
     wxSQLite3Database db;
     db.Open(dbFile);
     db.Begin();

     // PreparedStatementを準備する
     const wxString sqlIn 
	  = wxT("insert into USER_FAVORITE_THREADLIST (THREAD_TITLE, THREAD_ORIG_NUM, BOARDNAME_ASCII) VALUES (?, ?, ?)");
     wxSQLite3Statement stmt = db.PrepareStatement (sqlIn);

     // 配列内のレコードを追加する
     for (unsigned int i = 0; i < userFavoriteThreadListArray.GetCount();i+=3) {
	  // レコードを追加する
	  stmt.ClearBindings();
	  stmt.Bind(1, userFavoriteThreadListArray.Item(i));
	  stmt.Bind(2, userFavoriteThreadListArray.Item(i+1));
	  stmt.Bind(3, userFavoriteThreadListArray.Item(i+2));
	  stmt.ExecuteUpdate();
     }
     // コミット実行
     db.Commit();
     db.Close();
     
}
/**
 * ユーザーがお気に入りに登録しているスレッドの情報を取得する
 */
wxArrayString SQLiteAccessor::GetUserFavoriteThreadList() {

     // dbファイルの初期化
     wxString dbFile = GetDBFilePath();
     wxSQLite3Database::InitializeSQLite();
     wxSQLite3Database db;
     db.Open(dbFile);

     // リザルトセットを用意する
     wxSQLite3ResultSet rs;
     // SQL文を用意する
     wxString sqlSe = wxT("select THREAD_TITLE, THREAD_ORIG_NUM, BOARDNAME_ASCII from USER_FAVORITE_THREADLIST");

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
	  if (title.Len() > 0 && origNumber.Len() > 0 && boardNameAscii.Len() > 0) {
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
     wxString dbFile = GetDBFilePath();
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
     wxString dbFile = GetDBFilePath();
     wxArrayString array;

     try {
	  
	  wxSQLite3Database::InitializeSQLite();
	  wxSQLite3Database db;
	  db.Open(dbFile);

	  // リザルトセットを用意する
	  wxSQLite3ResultSet rs;
	  // SQL文を用意する
	  wxString SQL_QUERY = wxT("select BOARDNAME_KANJI from USER_CLOSED_BOARDLIST order by timeinfo desc limit 16");

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

     return array;
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
     wxString dbFile = GetDBFilePath();
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
     wxString dbFile = GetDBFilePath();
     wxArrayString array;

     try {
	  
	  wxSQLite3Database::InitializeSQLite();
	  wxSQLite3Database db;
	  db.Open(dbFile);

	  // リザルトセットを用意する
	  wxSQLite3ResultSet rs;
	  // SQL文を用意する
	  wxString SQL_QUERY = wxT("SELECT THREAD_TITLE from USER_CLOSED_THREADLIST order by timeinfo desc limit 16");

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

     return array;
}
/**
 * 最近閉じたスレッドタブ情報を取得する
 */
void SQLiteAccessor::GetClosedThreadFullInfo(const int number, ThreadInfo* threadInfo, const wxWindowID id) {

     // dbファイルの初期化
     wxString dbFile = GetDBFilePath();
     wxArrayString array;

     // テーブル名の決定
     wxString tableName;
     if (1000 <= id && id < 1100) {
	  tableName = wxT("USER_CLOSED_THREADLIST");
     } else if (1200 <= id && id < 1300) {
	  tableName = wxT("USER_FAVORITE_THREADLIST");
     } else {
	  // ERROR
	  return;
     }

     try {
	  
	  wxSQLite3Database::InitializeSQLite();
	  wxSQLite3Database db;
	  db.Open(dbFile);

	  // リザルトセットを用意する
	  wxSQLite3ResultSet rs;
	  // SQL文を用意する
	  const wxString SQL_QUERY = 
	       wxT("SELECT THREAD_TITLE, THREAD_ORIG_NUM, BOARDNAME_ASCII from ") 
	       + tableName
	       + wxT(" limit 1 offset ")
	       + wxString::Format(wxT("%d"), number);

	  // SQL文を実行する
	  rs = db.ExecuteQuery(SQL_QUERY);
	  db.Close();

	  while (rs.NextRow()) {
	       threadInfo->title          = rs.GetAsString(wxT("THREAD_TITLE"));
	       threadInfo->origNumber     = rs.GetAsString(wxT("THREAD_ORIG_NUM"));
	       threadInfo->boardNameAscii = rs.GetAsString(wxT("BOARDNAME_ASCII"));
	  }

     } catch (wxSQLite3Exception& e) {
	  wxMessageBox(e.GetMessage());
     }
}
/**
 * 指定されたテーブルにレコードが何件存在するかどうかを調べるメソッド
 */
int SQLiteAccessor::HowManyRecord(const wxString tableName) {

     // dbファイルの初期化
     wxString dbFile = GetDBFilePath();

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
/**
 * JaneCloneが使用するSQLiteのDBファイルの場所を返す
 */
wxString SQLiteAccessor::GetDBFilePath() {

     wxString dbFile = ::wxGetHomeDir() + wxFileSeparator + JANECLONE_DIR + SQLITE_FILE_PATH;
     return dbFile;
}
/**
 * ユーザーが検索ボックスで検索したキーワードをSQLiteに格納する
 */
void SQLiteAccessor::SetUserSearchedKeyword(const wxString& keyword, const wxWindowID id) {

     // 現在時間timestamp
     wxDateTime now = wxDateTime::Now();

     // dbファイルの初期化
     wxString dbFile = GetDBFilePath();
     wxSQLite3Database::InitializeSQLite();

     try {
	  wxSQLite3Database db;
	  db.Open(dbFile);
	  db.Begin();

	  wxString boardName;
	  
	  if (id == ID_BoardSearchBarCombo) {
	       boardName = wxT("USER_SEARCHED_BOARDNAME");
	  } else if (id == ID_ThreadSearchBarCombo) {
	       boardName = wxT("USER_SEARCHED_THREADNAME");
	  } else {
	       // エラールート
	       db.Commit();
	       db.Close();
	       return;
	  }

	  const wxString sqlIn = wxT("INSERT INTO ") + boardName + wxT("(TIMEINFO, KEYWORD) VALUES (?,?)");
	  wxSQLite3Statement stmt = db.PrepareStatement (sqlIn);
	  stmt.ClearBindings();

	  stmt.BindTimestamp(1, now);
	  stmt.Bind(2, keyword);
	  stmt.ExecuteUpdate();

	  // コミット実行
	  db.Commit();
	  db.Close();

     } catch (wxSQLite3Exception& e) {
	  wxMessageBox(e.GetMessage());
     }
}
/**
 * ユーザーが検索ボックスで検索したキーワードを取得する
 */
wxArrayString SQLiteAccessor::GetUserSearchedKeyword(const wxWindowID id) {

     // dbファイルの初期化
     wxString dbFile = GetDBFilePath();
     wxArrayString array;

     try {

	  wxSQLite3Database::InitializeSQLite();
	  wxSQLite3Database db;
	  db.Open(dbFile);

	  // リザルトセットを用意する
	  wxSQLite3ResultSet rs;
	  wxString boardName;
	  
	  if (id == ID_BoardSearchBarCombo) {
	       boardName = wxT("USER_SEARCHED_BOARDNAME");
	  } else if (id == ID_ThreadSearchBarCombo) {
	       boardName = wxT("USER_SEARCHED_THREADNAME");
	  } else {
	       // エラールート
	       db.Close();
	       return array;
	  }

	  // SQL文を用意する
	  const wxString SQL_QUERY = wxT("SELECT KEYWORD from ") + boardName;

	  // SQL文を実行する
	  rs = db.ExecuteQuery(SQL_QUERY);
	  db.Close();

	  while (rs.NextRow()) {
	       wxString keyword = rs.GetAsString(wxT("KEYWORD"));

	       // 各項目がNULLで無ければArrayStringに詰める
	       if (keyword.Length() > 0) {
		    array.Add(keyword);
	       }
	  }

	  return array;

     } catch (wxSQLite3Exception& e) {
	  wxMessageBox(e.GetMessage());
     }

     return array;
}
/**
 * 登録済みの新月の公開ノード一覧を取得する
 */
wxArrayString SQLiteAccessor::GetShingetsuNodeList() {

     // dbファイルの初期化
     wxString dbFile = GetDBFilePath();
     wxArrayString array;

     try {
	  
	  wxSQLite3Database::InitializeSQLite();
	  wxSQLite3Database db;
	  db.Open(dbFile);

	  // リザルトセットを用意する
	  wxSQLite3ResultSet rs;
	  // SQL文を用意する
	  const wxString SQL_QUERY = wxT("SELECT BOARD_URL from BOARD_INFO_SHINGETSU");

	  // SQL文を実行する
	  rs = db.ExecuteQuery(SQL_QUERY);
	  db.Close();

	  while (rs.NextRow()) {
	       array.Add(rs.GetAsString(wxT("BOARD_URL")));
	  }

     } catch (wxSQLite3Exception& e) {
	  wxMessageBox(e.GetMessage());
     }

     return array;
}
/**
 * 新月の公開ノードを登録する
 */
void SQLiteAccessor::SetShingetsuNode(const wxString& nodeURL) {


     // dbファイルの初期化
     wxString dbFile = GetDBFilePath();
     // 現在時間timestamp
     wxDateTime now = wxDateTime::Now();

     try {
	  
	  // dbファイルの初期化
	  wxString dbFile = GetDBFilePath();
	  wxSQLite3Database::InitializeSQLite();
	  wxSQLite3Database db;
	  db.Open(dbFile);
	  db.Begin();

	  /** 同じURLが登録されているか */

	  // リザルトセットを用意する
	  wxSQLite3ResultSet rs;
	  // SQL文を用意する
	  const wxString SQL_QUERY = wxT("select count(BOARD_URL) from BOARD_INFO_SHINGETSU where BOARD_URL = '") + nodeURL + wxT("'");
	  rs = db.ExecuteQuery(SQL_QUERY);

	  int record = 0;

	  if (!rs.IsNull(0)) record = rs.GetInt(0);
	  if (record == 0) {
	       wxString sqlIn = wxT("insert into BOARD_INFO_SHINGETSU values(?, ?)");
	       wxSQLite3Statement stmt = db.PrepareStatement (sqlIn);

	       // レコードを追加する
	       stmt.ClearBindings();
	       stmt.BindTimestamp(1, now);
	       stmt.Bind(2, nodeURL);
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
 * ダウンロードした画像のファイル名とUUIDをデータベースに格納する
 */
void SQLiteAccessor::SetImageFileName(std::vector<ImageFileInfo>& imageFileInfo)
{
     wxSQLite3Database db;
     wxDateTime now = wxDateTime::Now();

     INITIALIZE_JC_WXSQLITE3(db, now)

     for (std::vector<ImageFileInfo>::iterator it = imageFileInfo.begin(); it != imageFileInfo.end(); it++) {
          const wxString sqlIn = wxT("INSERT INTO CACHED_IMAGE (TIMEINFO, FILENAME, UUID_FILENAME) VALUES (?,?,?)");
	  wxSQLite3Statement stmt = db.PrepareStatement (sqlIn);
	  stmt.ClearBindings();

	  stmt.BindTimestamp(1, now);

#ifndef __clang__
	  stmt.Bind(2, *it->fileName);
	  stmt.Bind(3, *it->uuidFileName);
#else
	  stmt.Bind(2, it->fileName);
	  stmt.Bind(3, it->uuidFileName);
#endif
	  stmt.ExecuteUpdate();
      }

     CLOSE_CONN_JC_WXSQLITE3(db)
}
/**
 * ダウンロードした画像のファイル名とUUIDをデータベースに格納する
 * @param const ImageFileInfo* imageFileInfo ファイル情報
 */
void SQLiteAccessor::SetImageFileName(ImageFileInfo& imageFileInfo)
{
     wxSQLite3Database db;
     wxDateTime now = wxDateTime::Now();

     INITIALIZE_JC_WXSQLITE3(db, now)

     const wxString sqlIn = wxT("INSERT INTO CACHED_IMAGE (TIMEINFO, FILENAME, UUID_FILENAME) VALUES (?,?,?)");
     wxSQLite3Statement stmt = db.PrepareStatement (sqlIn);
     stmt.ClearBindings();

     stmt.BindTimestamp(1, now);
     stmt.Bind(2, imageFileInfo.fileName);
     stmt.Bind(3, imageFileInfo.uuidFileName);
     stmt.ExecuteUpdate();

     CLOSE_CONN_JC_WXSQLITE3(db)
}
/**
 * 画像のファイル名とUUIDのリストをデータベースから取得する
 * true : データあり
 * false: データなし
 */
bool SQLiteAccessor::GetImageFileName(const wxArrayString& fileNameArray, std::vector<ImageFileInfo>& imageFileInfoArray)
{
     wxSQLite3Database db;
     wxDateTime now = wxDateTime::Now();

     INITIALIZE_JC_WXSQLITE3(db, now)

     // リザルトセットを用意する
     wxSQLite3ResultSet rs;      

     for (int i = 0; i < fileNameArray.GetCount(); i++) {
          const wxString sqlSe = wxT("select UUID_FILENAME from CACHED_IMAGE where FILENAME = ? ");
	  wxSQLite3Statement stmt = db.PrepareStatement (sqlSe);
	  stmt.ClearBindings();

	  stmt.Bind(1, fileNameArray[i]);
	  rs = stmt.ExecuteQuery();

	  ImageFileInfo iFile;
	  iFile.fileName = fileNameArray[i];

	  while (rs.NextRow()) {
	       iFile.uuidFileName = rs.GetAsString(wxT("UUID_FILENAME"));
	  }

	  imageFileInfoArray.push_back(iFile);
     }

     CLOSE_CONN_JC_WXSQLITE3(db)
     
     if (imageFileInfoArray.size() != 0 ) {
          return true;
     } else {
          return false;
     }
}
/**
 * 画像のファイル名とUUIDをデータベースから取得する
 * @param  const wxString&	  画像ファイル名
 * @param  ImageFileInfo>&	  ファイル情報 
 * @return true: データあり false: データなし
 */
bool SQLiteAccessor::GetImageFileName(const wxString& fileName, ImageFileInfo& imageFileInfo)
{
     wxSQLite3Database db;
     wxDateTime now = wxDateTime::Now();

     INITIALIZE_JC_WXSQLITE3(db, now)

     // リザルトセットを用意する
     wxSQLite3ResultSet rs;      

     // SQL文を用意する
     const wxString SQL_QUERY = wxT("select UUID_FILENAME from CACHED_IMAGE where FILENAME = ? ");
      
     // SQL文を実行する
     wxSQLite3Statement stmt = db.PrepareStatement (SQL_QUERY);
     rs = stmt.ExecuteQuery();
      
     while (rs.NextRow()) {
          wxString uuidFileName = rs.GetAsString(wxT("UUID_FILENAME"));
      
          if (uuidFileName.Length() > 0) {
	       imageFileInfo.fileName = fileName;
	       imageFileInfo.uuidFileName = uuidFileName;
          }
     }

     CLOSE_CONNONLY_JC_WXSQLITE3(db)

     if ( imageFileInfo.uuidFileName != wxEmptyString ) {
          return true;
     } else {
          return false;
     }
}

/*
 * SQLiteAccessor.cpp
 *
 *  Created on: 2012/04/17
 *      Author: learning
 */

#include "SQLiteAccessor.h"

/**
 * SQLiteデータベースの初期化・トランザクションあり
 */
SQLiteAccessor::SQLiteAccessor() {
	// カレントディレクトリを設定
	wxDir dir(wxGetCwd());
	// sqliteフォルダが存在するか確認。無ければ確認＆フォルダを作成
	if (!dir.Exists(wxT("./sqlite/"))) {
		::wxMkdir(wxT("./sqlite/"));
	}
	// dbファイルが存在するか確認
	// SQLite3の初期化
	const wxString dbFile = wxGetCwd() + wxT("./sqlite/JaneClone.db");
	wxSQLite3Database::InitializeSQLite();
	// Dbを開く
	this->Open(dbFile);
	this->Begin(WXSQLITE_TRANSACTION_DEFAULT);

	// 板一覧情報用のテーブル
	this->ExecuteUpdate(wxT("CREATE TABLE IF NOT EXISTS BOARD_INFO(BOARDNAME_KANJI TEXT, BOARD_URL TEXT, CATEGORY TEXT)"));
}

/**
 * 板一覧情報をトランザクション処理で追加する
 */
void SQLiteAccessor::SetBoardInfo(const wxString category, const wxString name, const wxString url)
{
	// INSERTクエリを作成する
	wxString SQL_QUERY =
	wxT("INSERT INTO BOARD_INFO (BOARDNAME_KANJI, BOARD_URL, CATEGORY) VALUES ('");
	SQL_QUERY += name;
	SQL_QUERY += wxT("','");
	SQL_QUERY += url;
	SQL_QUERY += wxT("','");
	SQL_QUERY += category;
	SQL_QUERY += wxT("')");

	this->ExecuteUpdate(SQL_QUERY);
}

/**
 * コミットを行う
 */
void SQLiteAccessor::SetCommit()
{
	// SQLiteを閉じる
	this->Commit();
	Close();
}

/**
 * 板一覧情報をSQLite内のテーブルから取得しArrayStringの形で返す
 */
wxArrayString SQLiteAccessor::GetBoardInfo()
{
	// SQLite3の初期化
	const wxString dbFile = wxGetCwd() + wxT("./sqlite/JaneClone.db");
	wxSQLite3Database::InitializeSQLite();
	// Dbを開く
	wxSQLite3Database db;
	db.Open(dbFile);

	// リザルトセットを用意する
	wxSQLite3ResultSet rs;
	// SQL文を用意する
	wxString SQL_QUERY =
			wxT("SELECT BOARDNAME_KANJI, BOARD_URL, CATEGORY ");
	SQL_QUERY+=wxT("FROM BOARD_INFO");

	// SQL文を実行する
	rs = db.ExecuteQuery(SQL_QUERY);
	db.Close();

	// リザルトセットをArrayStringに設定する
	wxArrayString array;

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
bool SQLiteAccessor::TableHasInfo(const wxString tableName)
{
	// SQLite3の初期化
	const wxString dbFile = wxGetCwd() + wxT("./sqlite/JaneClone.db");
	wxSQLite3Database::InitializeSQLite();
	// Dbを開く
	wxSQLite3Database db;
	db.Open(dbFile);

	// リザルトセットを用意する
	wxSQLite3ResultSet rs;
	// SQL文を用意する
	wxString SQL_QUERY = wxT("SELECT COUNT(*) from ");
	SQL_QUERY += tableName;

	// SQL文を実行する
	rs = db.ExecuteQuery(SQL_QUERY);
	db.Close();

	// SQL文を実行し結果を受け取る
	if (!rs.IsNull(0)) {
		int recordNum = rs.GetInt(0);
		if ( 0==recordNum) {
			return false;
		} else {
			return true;
		}
	}

	// ここまで来てしまうとエラーなのでfalse
	return false;
}

/**
 * 指定されたテーブルを削除する
 */
void SQLiteAccessor::DropTable(const wxString tableName)
{
	// SQLite3の初期化
	const wxString dbFile = wxGetCwd() + wxT("./sqlite/JaneClone.db");
	wxSQLite3Database::InitializeSQLite();
	// Dbを開く
	wxSQLite3Database db;
	db.Open(dbFile);

	// SQL文を用意する
	wxString SQL_QUERY = wxT("DROP TABLE IF EXISTS ");
	SQL_QUERY += tableName;

	// SQL文を実行する
	db.ExecuteQuery(SQL_QUERY);
	db.Close();
}






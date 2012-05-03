/*
 * SQLiteAccessor.cpp
 *
 *  Created on: 2012/04/17
 *      Author: learning
 */

#include "SQLiteAccessor.h"

SQLiteAccessor::SQLiteAccessor() {
	// カレントディレクトリを設定
	wxDir dir(wxGetCwd());
	// sqliteフォルダが存在するか確認。無ければ確認＆フォルダを作成
	if (!dir.Exists(wxT("./sqlite/"))) {
		::wxMkdir(wxT("./sqlite/"));
	}
	// SQLite3の初期化
	const wxString dbFile = wxGetCwd() + wxT("./sqlite/JaneClone.db");
	wxSQLite3Database::InitializeSQLite();
	// Dbを開く
	this->Open(dbFile);
	this->Begin(WXSQLITE_TRANSACTION_DEFAULT);

	// テスト用のテーブル
	this->ExecuteUpdate(wxT("CREATE TABLE IF NOT EXISTS BOARD_INFO(BOARDNAME_KANJI TEXT, BOARD_URL TEXT, CATEGORY TEXT)"));
}

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

void SQLiteAccessor::SetCommit()
{
	// SQLiteを閉じる
	this->Commit();
	Close();
}








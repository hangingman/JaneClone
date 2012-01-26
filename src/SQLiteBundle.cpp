/*
 * SQLiteBundle.cpp
 *
 *  Created on: 2012/01/09
 *      Author: learning
 */

#include "SQLiteBundle.h"

SQLiteBundle::SQLiteBundle()
{
	// SQLite3の初期化
	const wxString dbFile = wxGetCwd() + wxT("./JaneClone.db");
	wxSQLite3Database::InitializeSQLite();

	// Dbを開く
	this->Open(dbFile);

	// テーブルの作成
	// ユーザーが見ていた板名を覚えるテーブル
	if (!this->TableExists(wxT("USER_PREV_BOARD_MT"))) {
	  this->ExecuteQuery(wxT("CREATE TABLE USER_PREV_BOARD_MT(PATH TEXT, BOARDNAME TEXT)"));
	}
	// ユーザーが見ていたスレッドを覚えるテーブル
	if (!this->TableExists(wxT("USER_PREV_THREAD_MT"))) {
	  this->ExecuteQuery(wxT("CREATE TABLE USER_PREV_THREAD_MT(PATH TEXT, THREADNAME TEXT)"));
	}
}

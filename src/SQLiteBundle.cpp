/*
 * SQLiteBundle.cpp
 *
 *  Created on: 2012/01/09
 *      Author: learning
 */

#include "SQLiteBundle.h"

SQLiteBundle::SQLiteBundle() {
	// カレントディレクトリを設定
	wxDir dir(wxGetCwd());
	// sqliteフォルダが存在するか確認。無ければ確認＆フォルダを作成
	if (!dir.Exists(wxT("./sqlite/"))) {
		wxMessageBox(wxT("sqlite用ディレクトリが見当たらないので作成します。"));
		::wxMkdir(wxT("./sqlite/"));
	}
	// SQLite3の初期化
	const wxString dbFile = wxGetCwd() + wxT("./sqlite/JaneClone.db");
	wxSQLite3Database::InitializeSQLite();

	// Dbを開く
	Open(dbFile);

	// テーブルの作成：テーブルが存在しなければ作っておく
	// ユーザーが見ていた板名を覚えるテーブル
	this->ExecuteQuery(
			wxT(
					"CREATE TABLE IF NOT EXISTS USER_PREV_BOARD_MT(OID INTEGER, PATH TEXT, BOARDNAME TEXT)"));

	// ユーザーが見ていたスレッドを覚えるテーブル
	this->ExecuteQuery(
			wxT(
					"CREATE TABLE IF NOT EXISTS USER_PREV_THREAD_MT(OID INTEGER, PATH TEXT, THREADNAME TEXT)"));

	// 板名、URL、asciiの板名を保存するテーブル
	this->ExecuteQuery(
			wxT(
					"CREATE TABLE IF NOT EXISTS URL_VS_BOARDNAME_MT(BOARDNAME_KANJI TEXT, BOARD_URL TEXT, BOARDNAME_ASCII TEXT)"));

	// SQLiteを閉じる
	Close();
}

// JaneClone終了時にユーザが開いていた板一覧をSQLiteに登録する処理
int SQLiteBundle::UserLookingBoardRegister(wxArrayString userLookBoard) {
	int rc = 0;

	// ユーザーが見ていた板名内にデータが存在すれば削除する
	this->ExecuteQuery(wxT("DELETE FROM USER_PREV_BOARD_MT"));

	// SQLクエリを作成する
	const wxString INSERT_USER_PREV_BOARD = wxT("");

	return rc;
}
// JaneClone開始時にユーザが以前開いていた板一覧をNotebookに設定する処理
int SQLiteBundle::UserLookingBoardSetter() {
	int rc = 0;

	return rc;
}

// JaneClone起動時・板一覧更新時に板名とURL、板名(ascii)のデータを保持したテーブルの更新を行う
int SQLiteBundle::UrlVsBoardNameTableSetter(wxArrayString urlVsBoardArray) {
	int rc = 0;

	// 板名、URL、asciiの板名を保存するテーブルにデータが存在すれば削除する
	//this->ExecuteQuery(wxT("DELETE FROM URL_VS_BOARDNAME_MT"));

	// SQLiteに格納する行数を確認する
	if (0 != (urlVsBoardArray.GetCount() % 3)) {
		return -1;
	}

	/**
	for (int i = 0; i < urlVsBoardArray.GetCount(); i = i + 3) {

		wxString SQL_QUERY_URL_VS_BOARDNAME = wxT("INSERT OR REPLACE INTO URL_VS_BOARDNAME_MT VALUES");
		SQL_QUERY_URL_VS_BOARDNAME += wxT("('");
		SQL_QUERY_URL_VS_BOARDNAME += urlVsBoardArray[i]; // 板名(漢字)
		SQL_QUERY_URL_VS_BOARDNAME += wxT("','");
		SQL_QUERY_URL_VS_BOARDNAME += urlVsBoardArray[i + 1]; // URL
		SQL_QUERY_URL_VS_BOARDNAME += wxT("','");
		SQL_QUERY_URL_VS_BOARDNAME += urlVsBoardArray[i + 2]; // 板名(ascii)
		SQL_QUERY_URL_VS_BOARDNAME += wxT("')");

		this->ExecuteQuery(SQL_QUERY_URL_VS_BOARDNAME);
	}
	*/

	return rc;
}

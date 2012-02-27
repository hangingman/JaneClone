/*
 * SQLiteBundle.cpp
 *
 *  Created on: 2012/01/09
 *      Author: learning
 */

#include "SQLiteBundle.h"

SQLiteBundle::SQLiteBundle()
{
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

  // テーブルの作成
  // ユーザーが見ていた板名を覚えるテーブル
  if (!this->TableExists(wxT("USER_PREV_BOARD_MT"))) {
    this->ExecuteQuery(wxT("CREATE TABLE USER_PREV_BOARD_MT(OID INTEGER, PATH TEXT, BOARDNAME TEXT)"));
  }
  // ユーザーが見ていたスレッドを覚えるテーブル
  if (!this->TableExists(wxT("USER_PREV_THREAD_MT"))) {
    this->ExecuteQuery(wxT("CREATE TABLE USER_PREV_THREAD_MT(OID INTEGER, PATH TEXT, THREADNAME TEXT)"));
  }

  // 板名、URL、asciiの板名を保存するテーブル
  if (!this->TableExists(wxT("URL_VS_BOARDNAME_MT"))) {
    this->ExecuteQuery(wxT("CREATE URL_VS_BOARDNAME_MT(BOARDNAME_KANJI TEXT, BOARD_URL TEXT, BOARDNAME_ASCII TEXT)"));
  }

  // SQLiteを閉じる
  Close();
}

// JaneClone終了時にユーザが開いていた板一覧をSQLiteに登録する処理
void SQLiteBundle::UserLookingBoardRegister(wxAuiNotebook* boardNoteBook)
{

}
// JaneClone開始時にユーザが以前開いていた板一覧をNotebookに設定する処理
void SQLiteBundle::UserLookingBoardSetter(wxAuiNotebook* boardNoteBook)
{

}

// 板一覧更新時に板名とURL、板名(ascii)のデータを保持したテーブルを作る
void SQLiteBundle::URLvsBoardNameTableSetter() 
{

}

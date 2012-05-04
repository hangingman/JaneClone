/*
 * SQLiteAccessor.h
 *
 *  Created on: 2012/04/17
 *      Author: learning
 */

#ifndef SQLITEACCESSOR_H_
#define SQLITEACCESSOR_H_

#include <iostream>
#include <wx/wx.h>
#include <wx/dir.h>
#include <wx/wxsqlite3.h>

class SQLiteAccessor: public wxSQLite3Database {
public:
	/**
	 * SQLiteデータベースの初期化・トランザクションあり
	 */
	SQLiteAccessor();
	/**
	 * 板一覧情報をトランザクション処理で追加する
	 */
	void SetBoardInfo(const wxString, const wxString, const wxString);
	/**
	 * コミットを行う
	 */
	void SetCommit();
	/**
	 * 板一覧情報をSQLite内のテーブルから取得しArrayStringの形で返す
	 */
	static wxArrayString GetBoardInfo();
	/**
	 * 指定されたテーブルに情報が存在するかどうか聞く
	 */
	static bool TableHasInfo(const wxString);
	/**
	 * 指定されたテーブルを削除する
	 */
	static void DropTable(const wxString);
};

#endif /* SQLITEACCESSOR_H_ */

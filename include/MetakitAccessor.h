/*
 * MetakitAccessor.h
 *
 *  Created on: 2012/05/13
 *      Author: learning
 */

#ifndef METAKITACCESSOR_H_
#define METAKITACCESSOR_H_

#include <iostream>
#include <wx/wx.h>
#include <wx/dir.h>
#include <wx/file.h>
#include <mk4.h>
#include <mk4str.h>

// Metakitのファイルパス
static const wxString METAKIT_FILE_PATH = wxT("./metakit/JaneClone.dat");

class MetakitAccessor {
public:
	/**
	 * Metakitデータベースの初期化・トランザクションあり
	 */
	MetakitAccessor();
	/**
	 * 板一覧情報をクラス変数の配列に追加する
	 */
	void SetBoardInfo(const wxString, const wxString, const wxString);
	/**
	 * 板一覧情報のコミットを行う
	 */
	void SetBoardInfoCommit();
	/**
	 * 板一覧情報をMetakit内のテーブルから取得しArrayStringの形で返す
	 */
	static wxArrayString GetBoardInfo();
	/**
	 * 指定されたテーブルに情報が存在するかどうかを調べるメソッド
	 */
	static bool TableHasView(const wxString);
	/**
	 * 指定されたテーブルを削除する
	 */
	static void DropView(const wxString);

private:
	wxArrayString* boardInfoArray;
};

#endif /* METAKITACCESSOR_H_ */

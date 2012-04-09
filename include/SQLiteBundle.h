/*
 * SQLiteBundle.h
 *
 *  Created on: 2012/01/09
 *      Author: learning
 */

#ifndef SQLITEBUNDLE_H_
#define SQLITEBUNDLE_H_

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/dir.h>
#include <wx/wxsqlite3.h>
#include "DataType.h"

/*
 * SQLiteBundleクラス
 * wxSQLite3Databaseを継承しユーザーが以前に見た板名スレなどを逐次記録する。
 */

// URLvsBoardNameクラスについてはDataType.h参照
WX_DECLARE_HASH_MAP( int, URLvsBoardName*, wxIntegerHash, wxIntegerEqual, NameURLHash );

class SQLiteBundle: public wxSQLite3Database {

public:
	// コンストラクタでいろいろ初期化を行う
	SQLiteBundle();

	// JaneClone終了時にユーザが開いていた板一覧をSQLiteに登録する処理
	int UserLookingBoardRegister(wxArrayString);
	// JaneClone開始時にユーザが以前開いていた板一覧をNotebookに設定する処理
	int UserLookingBoardSetter();
	// 板一覧更新時に板名とURL、板名(ascii)のデータを保持したテーブルを作る
	int UrlVsBoardNameTableSetter(wxArrayString);

};

#endif /* SQLITEBUNDLE_H_ */

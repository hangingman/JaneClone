/*
 * MetakitAccessor.cpp
 *
 *  Created on: 2012/05/13
 *      Author: learning
 */

#include "MetakitAccessor.h"

/**
 * データベースの初期化
 */
MetakitAccessor::MetakitAccessor() {
	// カレントディレクトリを設定
	wxDir dir(wxGetCwd());
	// metakitフォルダが存在するか確認。無ければ確認＆フォルダを作成
	if (!dir.Exists(wxT("./metakit/"))) {
		::wxMkdir(wxT("./metakit/"));
	}
	// 板一覧情報を入れる配列のインスタンスを作成
	boardInfoArray = new wxArrayString();
	// dbファイルの初期化
	wxString dbFile = METAKIT_FILE_PATH;
	// 空のViewを作る
	c4_Storage storage((const char*)dbFile.mb_str(), true);
	c4_View vBoardInfo = storage.GetAs(BOARD_INFO_STRUCTURE);
	storage.Commit();
}

/**
 * 板一覧情報をクラス変数の配列に追加する
 */
void MetakitAccessor::SetBoardInfo(const wxString category, const wxString name,
		const wxString url) {
	// それぞれの中身が空でなければ配列に板一覧情報を設定する
	if (name.Length() > 0 && url.Length() > 0 && category.Length() > 0) {
		boardInfoArray->Add(name);
		boardInfoArray->Add(url);
		boardInfoArray->Add(category);
	}
}

/**
 * 板一覧情報のコミットを行う
 */
void MetakitAccessor::SetBoardInfoCommit() {
	// dbファイルの初期化
	wxString dbFile = METAKIT_FILE_PATH;
	// もしすでにdatファイルが存在していれば削除
	if (wxFile::Exists(dbFile)) {
		wxRemoveFile(dbFile);
	}
	// プロパティを用意する（カラム名）
	c4_StringProp pBoardName("BOARDNAME_KANJI");
	c4_StringProp pBoardURL("BOARD_URL");
	c4_StringProp pCategory("CATEGORY");

	// Viewを得る
	c4_Storage storage((const char*)dbFile.mb_str(), true);
	// View内のカラムを指定する（コンマとカラム名の間にスペースがあると指定した名前を引けなくなるので注意）
	c4_View vBoardInfo = storage.GetAs(BOARD_INFO_STRUCTURE);

	// 配列内のレコードを追加する
	for (unsigned int i = 0; i < boardInfoArray->GetCount(); i += 3) {
		// wxStringに一度変換
		// レコードを追加する
		c4_Row row;
		pBoardName(row) = (const char*)boardInfoArray->Item(i).mb_str();
		pBoardURL(row) = (const char*)boardInfoArray->Item(i + 1).mb_str();
		pCategory(row) = (const char*)boardInfoArray->Item(i + 2).mb_str();
		vBoardInfo.Add(row);
	}
	// コミット実行
	storage.Commit();
	delete boardInfoArray;
}

/**
 * 板一覧情報をSQLite内のテーブルから取得しArrayStringの形で返す
 */
wxArrayString MetakitAccessor::GetBoardInfo() {
	// dbファイルの初期化
	wxString dbFile = METAKIT_FILE_PATH;
	// Viewを得る
	c4_Storage storage((const char*)dbFile.mb_str(), true);
	c4_View vBoardInfo = storage.GetAs(BOARD_INFO_STRUCTURE);
	c4_String types;

	// ??? 解析中 ↓　なんでこここれで値がとれているのかよくわからん
	for (int i = 0; i < vBoardInfo.NumProperties(); i++) {
		c4_Property prop = vBoardInfo.NthProperty(i);
		char t = prop.Type();
		types += t;
	}

	// リザルトセットをArrayStringに設定する
	wxArrayString array;

	for (int j = 0; j < vBoardInfo.GetSize(); ++j){
		c4_RowRef r = vBoardInfo[j];

		for (int k = 0; k < types.GetLength(); k+=3) {
			c4_Property p = vBoardInfo.NthProperty(k);
			wxString boardName = wxString((const char*) ((c4_StringProp&) p) (r), wxConvUTF8);
			array.Add(boardName);

			p = vBoardInfo.NthProperty(k+1);
			wxString url = wxString ((const char*) ((c4_StringProp&) p) (r), wxConvUTF8);
			array.Add(url);

			p = vBoardInfo.NthProperty(k+2);
			wxString category = wxString ((const char*) ((c4_StringProp&) p) (r), wxConvUTF8);
			array.Add(category);
		}
	}

	return array;
}
/**
 * 指定されたテーブルに情報が存在するかどうか聞く(トランザクション処理なし単独)
 */
bool MetakitAccessor::TableHasView(const wxString viewName) {
	// dbファイルの初期化
	wxString dbFile = METAKIT_FILE_PATH;
	// Viewを得る
	c4_Storage storage((const char*)dbFile.mb_str(), true);
	// View一覧を取得する
	wxString viewList = wxString(storage.Description(), wxConvUTF8);

	if (viewList.Contains(viewName)) {
		return true;
	} else {
		return false;
	}
}

/**
 * 指定されたテーブルを削除する
 */
void MetakitAccessor::DropView(const wxString viewName) {
	// dbファイルの初期化
	wxString dbFile = METAKIT_FILE_PATH;
	// Viewを得る
	c4_Storage storage((const char*)dbFile.mb_str(), true);

	// 指定されたView名をViewの構造に変えて、中身を削除する
	if (viewName == wxT("BOARD_INFO")) {
		c4_View v = storage.GetAs(BOARD_INFO_STRUCTURE);
		v.RemoveAll();
		storage.Commit();
	}
}


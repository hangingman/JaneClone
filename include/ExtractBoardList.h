#ifndef EXTRACTBOARDLIST_H
#define EXTRACTBOARDLIST_H

#include <wx/wx.h>
#include <wx/string.h>
#include <libxml/HTMLparser.h>

using namespace std;

class ExtractBoardList
{
	public:
		// コンストラクタ
		ExtractBoardList();
		// 外部から接続して板一覧情報を得る
		wxArrayString GetBoardList();
		// 可変長配列
		wxArrayString m_list;

	private:
		// 内部の処理関数
		void FindBoardInfo(xmlNode*& element, wxArrayString& array);
		// 構造体へのポインタ
		htmlDocPtr m_doc;

};

#endif // EXTRACTBOARDLIST_H

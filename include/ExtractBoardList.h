#ifndef EXTRACTBOARDLIST_H
#define EXTRACTBOARDLIST_H

#include <libxml/HTMLparser.h>
#include <vector>
#include <string>

using namespace std;

class ExtractBoardList
{
	public:
		// コンストラクタ
		ExtractBoardList();
		// 外部から接続して板一覧情報を得る
		vector<char*> GetBoardList();
	private:
		// 内部の処理関数
		void CollectBoardInfo(htmlNodePtr element);
		// 構造体へのポインタ
		htmlDocPtr m_doc;
		// 板一覧の配列
		vector<char*> *m_boardList;
};

#endif // EXTRACTBOARDLIST_H

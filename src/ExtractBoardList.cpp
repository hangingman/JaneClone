#include "ExtractBoardList.h"

ExtractBoardList::ExtractBoardList()
{
	// コンストラクタ内の処理
	htmlDocPtr m_doc;
}

// 板一覧の情報が入ったリストを作って返す
vector<char*> ExtractBoardList::GetBoardList()
{
	// ファイル名とエンコードの設定
	char* file = "./dat/BoardListUTF8.html";
	char* enc = "utf-8";
	std::vector<char*> m_boardList;

	// xmlの読み込み
	if (m_doc = htmlReadFile(file, enc, 1)) {
  	htmlNodePtr root = xmlDocGetRootElement(m_doc);
        if(root != NULL){
            CollectBoardInfo(root);
        }
        xmlFreeDoc(m_doc);
        m_doc = NULL;
	}

	xmlCleanupParser();
	xmlCleanupCharEncodingHandlers();

	return m_boardList;
}

void ExtractBoardList::CollectBoardInfo(htmlNodePtr element)
{
	// 板一覧の配列
	for(htmlNodePtr node = element; node != NULL; node = node->next){
		if(node->type == XML_ELEMENT_NODE){
			// もしノードの中身が「A」タグだったら
			if(xmlStrcasecmp(node->name, (const xmlChar*)"A") == 0){
				for(xmlAttrPtr attr = node->properties; attr != NULL; attr = attr->next){
					if(xmlStrcasecmp(attr->name, (const xmlChar*)"HREF") == 0){
						// 配列に要素を詰め込む
						m_boardList->push_back((char*)node->children->content);
					}
				}
			}
            if(node->children != NULL)
            {
                ExtractBoardList::CollectBoardInfo(node->children);
            }
		}
	}
}

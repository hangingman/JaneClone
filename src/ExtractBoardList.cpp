#include "ExtractBoardList.h"


/**
 * ExtractBoardList
 * コンストラクタ
 */
ExtractBoardList::ExtractBoardList(const char* file)
{
	//　コンストラクタ
	htmlDocPtr m_doc;
	// インスタンスを用意する
	accessor = new SQLiteAccessor();

	// ファイル名とエンコードの設定
	char* enc = "utf-8";

	// xmlの読み込み
	if (m_doc = htmlReadFile(file, enc, 1)) {
  	htmlNodePtr root = xmlDocGetRootElement(m_doc);
        if(root != NULL){
            ExtractBoardList::FindBoardInfo(root);
        }
        xmlFreeDoc(m_doc);
        m_doc = NULL;
	}

	xmlCleanupParser();
	xmlCleanupCharEncodingHandlers();

	accessor->SetCommit();
	delete accessor;
}

/**
 *  FindBoardInfo
 *  板一覧情報を収集しSQLiteに格納する
 */
void ExtractBoardList::FindBoardInfo(xmlNode*& element){

	wxString lsCategory;
	wxString lsName;
	wxString lsUrl;

	// 板一覧の配列
	for(htmlNodePtr node = element; node != NULL; node = node->next){
		if(node->type == XML_ELEMENT_NODE){
			/** もしノードの中身が「B」タグだったら:カテゴリ名 */
			if(xmlStrcasecmp(node->name, (const xmlChar*)"B") == 0){
				// 配列に要素を詰め込む
				if (sizeof(node->children->content) > 0){
					wxString category(node->children->content, wxConvUTF8);
					lsCategory = category;
				}
			}
			/** もしノードの中身が「A」タグだったら：板名 */
			if(xmlStrcasecmp(node->name, (const xmlChar*)"A") == 0){
				for(xmlAttrPtr attr = node->properties; attr != NULL; attr = attr->next){
					if(xmlStrcasecmp(attr->name, (const xmlChar*)"HREF") == 0){
						// 配列に要素を詰め込む
						if (sizeof(node->children->content) > 0 && sizeof(node->properties[0].children->content) > 0){
							wxString name(node->children->content, wxConvUTF8);
							lsName = name;
							wxString url(node->properties[0].children->content, wxConvUTF8);
							lsUrl = url;

							// 格納した情報をSQLiteに配置する
							accessor->SetBoardInfo(lsCategory, lsName, lsUrl);
						}
					}
				}
			}
            if(node->children != NULL)
            {
                ExtractBoardList::FindBoardInfo(node->children);
            }
		}
	}
}

/* JaneClone - a text board site viewer for 2ch
 * Copyright (C) 2012 Hiroyuki Nagata
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * Contributor:
 *	Hiroyuki Nagata <newserver002@gmail.com>
 */

#include "ExtractBoardList.h"

/**
 * ExtractBoardList
 * コンストラクタ
 */
ExtractBoardList::ExtractBoardList(const char* file) {
	// HTML読み込み用構造体
	htmlDocPtr m_doc;
	// インスタンスを用意する
	accessor = new MetakitAccessor();

	// ファイル名とエンコードの設定
	const char* enc = "utf-8";

	// HTMLの読み込み
	m_doc = htmlReadFile(file, enc, HTML_PARSE_RECOVER );

	if (NULL == m_doc) {
		// NULLが返された場合その時点で終了する
		xmlCleanupParser();
		xmlCleanupCharEncodingHandlers();
		delete accessor;
	}

	// htmlNodePtrに変換する
	htmlNodePtr root = xmlDocGetRootElement(m_doc);

	if (NULL == root) {
		// NULLが返された場合その時点で終了する
		xmlFreeDoc(m_doc);
		m_doc = NULL;
		xmlCleanupParser();
		xmlCleanupCharEncodingHandlers();
		delete accessor;
	} else {
		// 正常処理
		FindBoardInfo(root);
		xmlFreeDoc(m_doc);
		m_doc = NULL;
	}

	// 終了後の後片付け
	xmlCleanupParser();
	xmlCleanupCharEncodingHandlers();
	accessor->SetBoardInfoCommit();
	delete accessor;
}

/**
 *  FindBoardInfo
 *  板一覧情報を収集しSQLiteに格納する
 */
void ExtractBoardList::FindBoardInfo(xmlNode*& element) {

	wxString lsCategory;
	wxString lsName;
	wxString lsUrl;

	// 板一覧の配列
	for (htmlNodePtr node = element; node != NULL; node = node->next) {
		if (node->type == XML_ELEMENT_NODE) {
			/** もしノードの中身が「B」タグだったら:カテゴリ名 */
			if (xmlStrcasecmp(node->name, (const xmlChar*) "B") == 0) {
				// 配列に要素を詰め込む
				if (sizeof(node->children->content) > 0) {
#if defined(__WXMSW__)
					wxString category(node->children->content, wxConvUTF8);
#else
					wxString category((const char*) node->children->content,
							wxConvUTF8);
#endif
					lsCategory = category;
				}
			}
			/** もしノードの中身が「A」タグだったら：板名 */
			if (xmlStrcasecmp(node->name, (const xmlChar*) "A") == 0) {
				for (xmlAttrPtr attr = node->properties; attr != NULL; attr =
						attr->next) {
					if (xmlStrcasecmp(attr->name, (const xmlChar*) "HREF")
							== 0) {
						// 配列に要素を詰め込む
						if (sizeof(node->children->content) > 0
								&& sizeof(node->properties[0].children->content)
										> 0) {
#if defined(__WXMSW__)
							wxString name(node->children->content, wxConvUTF8);
							wxString url(node->properties[0].children->content, wxConvUTF8);
#else
							wxString name((const char*) node->children->content,
									wxConvUTF8);
							wxString url(
									(const char*) node->properties[0].children->content,
									wxConvUTF8);
#endif

							lsName = name;
							lsUrl = url;

							// 格納した情報をMetakitに配置する
							accessor->SetBoardInfo(lsCategory, lsName, lsUrl);
						}
					}
				}
			}
			// 再帰的に処理する
			if (node->children != NULL) {
				ExtractBoardList::FindBoardInfo(node->children);
			}
		}
	}
}

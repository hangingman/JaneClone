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

#include "extractboardlist.hpp"

/**
 * ExtractBoardList
 * コンストラクタ
 */
ExtractBoardList::ExtractBoardList(const char* file) {

     // HTML読み込み用構造体
     htmlDocPtr m_doc;
     // SQLiteAccessorのインスタンスを準備する
     SQLiteAccessor* accessor = new SQLiteAccessor();
     boardInfoArray = new wxArrayString();

     // ファイル名とエンコードの設定
     const char* enc = "utf-8";

     // HTMLの読み込み
     m_doc = htmlReadFile(file, enc, HTML_PARSE_RECOVER );

     if (NULL == m_doc) {
	  // NULLが返された場合その時点で終了する
	  xmlCleanupParser();
	  xmlCleanupCharEncodingHandlers();
	  delete accessor;
	  delete boardInfoArray;
	  return;
     }

     // htmlNodePtrに変換する
     htmlNodePtr root = xmlDocGetRootElement(m_doc);

     if (NULL == root) {
	  // NULLが返された場合その時点で終了する
	  xmlCleanupParser();
	  xmlCleanupCharEncodingHandlers();
	  delete accessor;
	  delete boardInfoArray;
	  return;
     } else {
	  // 正常処理
	  FindBoardInfo(root);
	  xmlCleanupParser();
	  xmlCleanupCharEncodingHandlers();
     }

     accessor->SetBoardInfoCommit(boardInfoArray);
     delete accessor;
     delete boardInfoArray;
}
/**
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
			 // wx-2.8ではキャストの方法がこれしかない
			 wxString category((const char*) node->children->content,wxConvUTF8);
			 lsCategory = category;
		    }
	       }
	       /** もしノードの中身が「A」タグだったら：板名 */
	       if (xmlStrcasecmp(node->name, (const xmlChar*) "A") == 0) {
		    for (xmlAttrPtr attr = node->properties; attr != NULL; attr = attr->next) {
			 if (xmlStrcasecmp(attr->name, (const xmlChar*) "HREF") == 0) {
			      // 配列に要素を詰め込む
			      if (sizeof(node->children->content) > 0 && 
				  sizeof(node->properties[0].children->content) > 0) {
				   // wx-2.8ではキャストの方法がこれしかない
				   wxString name((const char*) node->children->content, wxConvUTF8);
				   wxString url((const char*) node->properties[0].children->content, wxConvUTF8);
				   lsName = name;
				   lsUrl = url;

				   // 格納した情報をMetakitに配置する
				   this->SetBoardInfo(lsCategory, lsName, lsUrl);
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
/**
 * 板一覧情報をクラス変数の配列に追加する
 */
void ExtractBoardList::SetBoardInfo(const wxString category, const wxString name, const wxString url) {

     // それぞれの中身が空でなければ配列に板一覧情報を設定する
     if (name.Length() > 0 && url.Length() > 0 && category.Length() > 0) {
	  boardInfoArray->Add(name);
	  boardInfoArray->Add(url);
	  boardInfoArray->Add(category);
     }
}

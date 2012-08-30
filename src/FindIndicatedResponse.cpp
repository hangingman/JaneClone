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

#include "FindIndicatedResponse.h"

/**
 * コンストラクタ
 */
FindIndicatedResponse::FindIndicatedResponse(wxString& allSource,
		wxString& resNumber) {

	// HTML読み込み用構造体
	htmlDocPtr m_doc;
	// 取得したい情報が入っているHTMLソース
	wxCharBuffer buffer = allSource.ToUTF8();
	// URL指定(NULLだとダメ)
	const char* URL = _("./tmp");
	// エンコードの設定
	const char* enc = "utf-8";

	// メモリからHTMLを読み出す
	m_doc = htmlReadMemory(allSource.data(), sizeof(allSource), URL, enc, HTML_PARSE_RECOVER);

	if (NULL == m_doc) {
		// NULLが返された場合その時点で終了する
		xmlCleanupParser();
		xmlCleanupCharEncodingHandlers();
		return;
	}

	// htmlNodePtrに変換する
	htmlNodePtr root = xmlDocGetRootElement(m_doc);

	if (NULL == root) {
		// NULLが返された場合その時点で終了する
		xmlFreeDoc(m_doc);
		m_doc = NULL;
		xmlCleanupParser();
		xmlCleanupCharEncodingHandlers();
		return;

	} else {
		// 正常処理
		FindIndicatedResponseNode(root);
		xmlFreeDoc(m_doc);
		m_doc = NULL;
	}

	// 終了後の後片付け
	xmlCleanupParser();
	xmlCleanupCharEncodingHandlers();
}
/**
 * HTMLをパースした結果を渡す処理
 */
wxString FindIndicatedResponse::GetIndicatedResponse() {
	return indicatedRes;
}
/**
 *  FindBoardInfo
 *  板一覧情報を収集しSQLiteに格納する
 */
void FindIndicatedResponse::FindIndicatedResponseNode(xmlNode*& element) {
}


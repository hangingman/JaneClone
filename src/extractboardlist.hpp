/**JaneClone - a text board site viewer for 2ch
 * Copyright (C) 2012-2014 Hiroyuki Nagata
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
 *	Hiroyuki Nagata <idiotpanzer@gmail.com>
 */

#ifndef EXTRACTBOARDLIST_HPP_
#define EXTRACTBOARDLIST_HPP_

#include <libxml/HTMLparser.h>
#include <libxml/HTMLtree.h>
#include <wx/wx.h>
#include "sqliteaccessor.hpp"

class ExtractBoardList {

public:
     // コンストラクタ
     ExtractBoardList(const char* file);
     // HTML整形
     static const wxString HtmlFormat(const wxString& html);

private:
     // 内部の処理関数
     void FindBoardInfo(xmlNode*& element);
     void SetBoardInfo(const wxString category, const wxString name, const wxString url);

     // 構造体
     htmlDocPtr m_doc;
     // 板情報を含む配列
     wxArrayString* boardInfoArray;
     // 除外するカテゴリ
     const wxString excludeCategory1 = wxT("特別企画");
     const wxString excludeCategory2 = wxT("他のサイト");
};

#endif // EXTRACTBOARDLIST_HPP

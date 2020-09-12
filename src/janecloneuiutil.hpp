/* JaneClone - a text board site viewer for 2ch
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

#ifndef JANECLONEUIUTIL_HPP_
#define JANECLONEUIUTIL_HPP_

#include <wx/treectrl.h>
#include <wx/imaglist.h>
#include "datatype.hpp"
#include "enums.hpp"

/**
 * JaneCloneのUI操作用クラス
 */
class JaneCloneUiUtil {

public:
     /**
      * wxTreeCtrlのインスタンスを受け取って共通の設定を行う
      *
      * @param wxTreeCtrl* treeCtrl 設定対象のツリー
      * @param const wxWindowID id  設定対象のGUIの部位を表すID
      */
     void static SetTreeCtrlCommonSetting(wxTreeCtrl* treeCtrl, const wxWindowID id);
     /**
      * wxWidgetsのイベント通知関数のラッパー
      *
      * @param const wxWindowID type
      * @param const wxWindowID id
      * @param const wxString&   m          Message for event destination
      * @param wxObject* o          wxObject for event destination
      */
     void static QueueEventHelper(const wxWindowID type, const wxWindowID id, const wxString& m = wxEmptyString, wxObject* o = NULL);
     /**
      * JaneCloneのログ処理のラッパー
      *
      * @param const wxString& message
      */
     void static SendLoggingHelper(const wxString& message);
};

#endif /* JANECLONEUIUTIL_HPP_ */

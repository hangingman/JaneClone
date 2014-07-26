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

#include "janecloneuiutil.hpp"


void JaneCloneUiUtil::SetTreeCtrlCommonSetting(wxTreeCtrl* treeCtrl, const wxWindowID id) 
{
     // プロパティファイルにフォント設定/背景色があれば使用する
     wxString widgetsName = wxT("ID_TreeFontButton");
     wxString widgetsInfo = wxEmptyString;
     JaneCloneUtil::GetJaneCloneProperties(widgetsName, &widgetsInfo);
     if (widgetsInfo != wxEmptyString) 
     {
	  wxFont font;
	  bool ret = font.SetNativeFontInfoUserDesc(widgetsInfo);
	  if(ret) treeCtrl->SetFont(font);
     }
     widgetsName = wxT("ID_BoardListBGColorButton");
     widgetsInfo.Clear();
     JaneCloneUtil::GetJaneCloneProperties(widgetsName, &widgetsInfo);
     if (widgetsInfo != wxEmptyString) 
     {
	  wxColour bgColor;
	  bool ret = bgColor.Set(widgetsInfo);
	  if(ret) treeCtrl->SetBackgroundColour(bgColor);
     }

     wxTreeItemData treeData;
     wxTreeItemId m_rootId;

     // イメージリストにアイコンを登録する
#ifndef __WXMAC__
     wxImageList* treeImage = new wxImageList(16, 16);
     wxBitmap idx1(wxT("rc/folder.png"), wxBITMAP_TYPE_PNG);
     treeImage->Add(idx1);
     wxBitmap idx2(wxT("rc/text-html.png"), wxBITMAP_TYPE_PNG);
     treeImage->Add(idx2);
#else // Macの場合画像ファイル読み込みの場所が異なる
     wxImageList* treeImage = new wxImageList(16, 16);
     wxBitmap idx1(wxT("JaneClone.app/Contents/MacOS/rc/folder.png"), wxBITMAP_TYPE_PNG);
     treeImage->Add(idx1);
     wxBitmap idx2(wxT("JaneClone.app/Contents/MacOS/rc/text-html.png") , wxBITMAP_TYPE_PNG);
     treeImage->Add(idx2);
#endif
     treeCtrl->AssignImageList(treeImage);

     switch (id) 
     {

     case ID_BoardTreectrl:
     {	  
	  treeCtrl->SetLabel(BOARD_TREE);
	  wxTreeItemId rootTemp = treeCtrl->AddRoot(wxT("2ch板一覧"));
	  treeCtrl->SetItemImage(rootTemp, 0, wxTreeItemIcon_Normal);
     }
     break;

     case ID_FavsTreectrl:
     {
	  treeCtrl->SetLabel(FAVS_TREE);
	  wxTreeItemId rootTemp = treeCtrl->AddRoot(wxT("お気に入り一覧"));
	  treeCtrl->SetItemImage(rootTemp, 0, wxTreeItemIcon_Normal);
     }
     break;
	  
     case ID_NowReadingTreectrl:
     {
	  treeCtrl->SetLabel(NOW_READ_TREE);
	  wxTreeItemId rootTemp = treeCtrl->AddRoot(wxT("閲覧中一覧"));
	  treeCtrl->SetItemImage(rootTemp, 0, wxTreeItemIcon_Normal);
     }
     
     break;
     }
}

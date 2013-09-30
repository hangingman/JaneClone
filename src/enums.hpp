/**
 * enums.hpp - janeclones' enums
 *
 * Copyright (c) 2013 Hiroyuki Nagata <newserver002@gmail.com>
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

#ifndef ENUMS_HPP_
#define ENUMS_HPP_

#include "enumstring.hpp"

// テーブル要素
#define X_TABLE_ITEM(x) X(x, STR(x))

// 使いたい列挙型を定義する
// コメントがかけないぞ、こまった
#define X_TABLE \
X_TABLE_ITEM(ID_Restart) \
X_TABLE_ITEM(ID_Test) \
X_TABLE_ITEM(ID_Logging)\
X_TABLE_ITEM(ID_WindowMinimize)\
X_TABLE_ITEM(ID_GetBoardList) \
X_TABLE_ITEM(ID_CheckLogDirectory) \
X_TABLE_ITEM(ID_GetVersionInfo) \
X_TABLE_ITEM(ID_ThreadNoteBook) \
X_TABLE_ITEM(ID_BoardNoteBook) \
X_TABLE_ITEM(ID_ThumbnailNoteBook) \
X_TABLE_ITEM(ID_BoardTreectrl) \
X_TABLE_ITEM(ID_ShingetsuBoardTreectrl) \
X_TABLE_ITEM(ID_BoardTreeNoteBook) \
X_TABLE_ITEM(ID_OneBoardTabClose) \
X_TABLE_ITEM(ID_ExcepSelTabClose) \
X_TABLE_ITEM(ID_AllBoardTabClose) \
X_TABLE_ITEM(ID_AllLeftBoardTabClose) \
X_TABLE_ITEM(ID_AllRightBoardTabClose) \
X_TABLE_ITEM(ID_OnOpenBoardByBrowser) \
X_TABLE_ITEM(ID_ReloadOneBoard) \
X_TABLE_ITEM(ID_CopyBURLToClipBoard) \
X_TABLE_ITEM(ID_CopyBTitleToClipBoard) \
X_TABLE_ITEM(ID_CopyBBothDataToClipBoard) \
X_TABLE_ITEM(ID_CopyTURLToClipBoard) \
X_TABLE_ITEM(ID_CopyTTitleToClipBoard) \
X_TABLE_ITEM(ID_CopyTBothDataToClipBoard) \
X_TABLE_ITEM(ID_CopyTContentsToClipBoard) \
X_TABLE_ITEM(ID_CopyTAllToClipBoard) \
X_TABLE_ITEM(ID_OneThreadTabClose) \
X_TABLE_ITEM(ID_ExcepSelThreadTabClose) \
X_TABLE_ITEM(ID_AllThreadTabClose) \
X_TABLE_ITEM(ID_AllLeftThreadTabClose) \
X_TABLE_ITEM(ID_AllRightThreadTabClose) \
X_TABLE_ITEM(ID_OnOpenThreadByBrowser) \
X_TABLE_ITEM(ID_SaveDatFile) \
X_TABLE_ITEM(ID_SaveDatFileToClipBoard) \
X_TABLE_ITEM(ID_DeleteDatFile) \
X_TABLE_ITEM(ID_ReloadThisThread) \
X_TABLE_ITEM(ID_CallResponseWindow) \
X_TABLE_ITEM(ID_BoardListCtrl) \
X_TABLE_ITEM(ID_ShingetsuBoardListCtrl) \
X_TABLE_ITEM(ID_FontDialogBoardTree) \
X_TABLE_ITEM(ID_FontDialogLogWindow) \
X_TABLE_ITEM(ID_FontDialogBoardNotebook) \
X_TABLE_ITEM(ID_FontDialogThreadNotebook) \
X_TABLE_ITEM(ID_FontDialogThreadContents) \
X_TABLE_ITEM(ID_URLWindowButton) \
X_TABLE_ITEM(ID_UserLastClosedThreadMenuUp) \
X_TABLE_ITEM(ID_UserLastClosedBoardMenuUp) \
X_TABLE_ITEM(ID_UserLookingTabsMenuUp) \
X_TABLE_ITEM(ID_UserLookingTabsControl) \
X_TABLE_ITEM(ID_UserLookingTabsMenuClick) \
X_TABLE_ITEM(ID_OnOpenJaneCloneOfficial) \
X_TABLE_ITEM(ID_OnOpen2chViewerOfficial) \
X_TABLE_ITEM(ID_ThreadSearchBar) \
X_TABLE_ITEM(ID_BoardSearchBar) \
X_TABLE_ITEM(ID_ShingetsuBoardSearchBar) \
X_TABLE_ITEM(ID_ThreadSearchBarCombo) \
X_TABLE_ITEM(ID_BoardSearchBarCombo) \
X_TABLE_ITEM(ID_SearchBoxDoSearch) \
X_TABLE_ITEM(ID_SearchBoxUp) \
X_TABLE_ITEM(ID_SearchBoxDown) \
X_TABLE_ITEM(ID_SearchBoxCopy) \
X_TABLE_ITEM(ID_SearchBoxCut) \
X_TABLE_ITEM(ID_SearchBoxSelectAll) \
X_TABLE_ITEM(ID_SearchBoxClear) \
X_TABLE_ITEM(ID_SearchBoxNormalSearch) \
X_TABLE_ITEM(ID_SearchBoxRegexSearch) \
X_TABLE_ITEM(ID_SearchBarHide) \
X_TABLE_ITEM(ID_FloatToolBar) \
X_TABLE_ITEM(ID_SwitchSeparateXY) \
X_TABLE_ITEM(ID_SwitchTwoThreePane) \
X_TABLE_ITEM(ID_ShowBoardListTree) \
X_TABLE_ITEM(ID_SwitchRightPane) \
X_TABLE_ITEM(ID_CallSettingWindow) \
X_TABLE_ITEM(ID_ResponseWindow) \
X_TABLE_ITEM(ID_QuitResponseWindow) \
X_TABLE_ITEM(ID_PostResponse)   \
X_TABLE_ITEM(ID_PostConfirmForm)   \
X_TABLE_ITEM(ID_TCBAutoReload)   \
X_TABLE_ITEM(ID_TCBRedResExtract)  \
X_TABLE_ITEM(ID_TCBRefresh)   \
X_TABLE_ITEM(ID_TCBScrollToNewRes) \
X_TABLE_ITEM(ID_TCBStop)   \
X_TABLE_ITEM(ID_TCBResExtract) \
X_TABLE_ITEM(ID_TCBNewThread) \
X_TABLE_ITEM(ID_TCBBookMark) \
X_TABLE_ITEM(ID_TCBDeleteLog) \
X_TABLE_ITEM(ID_TCBClose) \
X_TABLE_ITEM(ID_TCBNormalSearch) \
X_TABLE_ITEM(ID_TCBHideSearchBar) \
X_TABLE_ITEM(ID_TCBForward) \
X_TABLE_ITEM(ID_TCBBackward) \
X_TABLE_ITEM(ID_ThreadContentBarUpdate) \
X_TABLE_ITEM(ID_CopyFromHtmlWindow) \
X_TABLE_ITEM(ID_CopyURLFromHtmlWindow) \
X_TABLE_ITEM(ID_SelectAllTextHtmlWindow) \
X_TABLE_ITEM(ID_SearchSelectWordByYahoo) \
X_TABLE_ITEM(ID_SearchSelectWordByGoogle) \
X_TABLE_ITEM(ID_SearchSelectWordByAmazon) \
X_TABLE_ITEM(ID_SearchThreadBySelectWord) \
X_TABLE_ITEM(ID_CallResponseWindowAnchor) \
X_TABLE_ITEM(ID_CallResponseWindowQuote) \
X_TABLE_ITEM(ID_HtmlSourceDebug) \
X_TABLE_ITEM(ID_OneThumbnailTabClose) \
X_TABLE_ITEM(ID_AllThumbnailTabClose) \
X_TABLE_ITEM(ID_AllLeftThumbnailTabClose) \
X_TABLE_ITEM(ID_AllRightThumbnailTabClose) \
X_TABLE_ITEM(ID_SelectLeftThumbnailTab) \
X_TABLE_ITEM(ID_SelectRightThumbnailTab) \
X_TABLE_ITEM(ID_OnOpenImageByBrowser) \
X_TABLE_ITEM(ID_HideThumbnailTab) \
X_TABLE_ITEM(ID_OnClickMenuCOL_CHK) \
X_TABLE_ITEM(ID_OnClickMenuCOL_NUM) \
X_TABLE_ITEM(ID_OnClickMenuCOL_TITLE) \
X_TABLE_ITEM(ID_OnClickMenuCOL_RESP) \
X_TABLE_ITEM(ID_OnClickMenuCOL_CACHEDRES) \
X_TABLE_ITEM(ID_OnClickMenuCOL_NEWRESP) \
X_TABLE_ITEM(ID_OnClickMenuCOL_INCRESP) \
X_TABLE_ITEM(ID_OnClickMenuCOL_MOMENTUM) \
X_TABLE_ITEM(ID_OnClickMenuCOL_LASTUP) \
X_TABLE_ITEM(ID_OnClickMenuCOL_SINCE) \
X_TABLE_ITEM(ID_OnClickMenuCOL_OID) \
X_TABLE_ITEM(ID_OnClickMenuCOL_BOARDNAME) \
X_TABLE_ITEM(ID_OnCancelSetting) \
X_TABLE_ITEM(ID_OnOkSetting) \
X_TABLE_ITEM(ID_SettingPanelTree) \
X_TABLE_ITEM(ID_SettingPanelUpdate) \
X_TABLE_ITEM(ID_NetworkPanelUpdate) \
X_TABLE_ITEM(ID_NetworkPanel) \
X_TABLE_ITEM(ID_NetworkPanelUseProxy) \
X_TABLE_ITEM(ID_NetworkPanelUseProxyCache) \
X_TABLE_ITEM(ID_NetworkPanelBasicAuthUserName) \
X_TABLE_ITEM(ID_NetworkPanelBasicAuthPassword) \
X_TABLE_ITEM(ID_NetworkPanelProxyReceiveAddr) \
X_TABLE_ITEM(ID_NetworkPanelProxyReceivePort) \
X_TABLE_ITEM(ID_NetworkPanelProxySendAddr) \
X_TABLE_ITEM(ID_NetworkPanelProxySendPort) \
X_TABLE_ITEM(ID_NetworkPanelProxySSLAuthAddr) \
X_TABLE_ITEM(ID_NetworkPanelProxySSLAuthPort) \
X_TABLE_ITEM(ID_PathSettingPanel) \
X_TABLE_ITEM(ID_BrowserCheck) \
X_TABLE_ITEM(ID_BrowserPathTC) \
X_TABLE_ITEM(ID_DirPickerBrowser) \
X_TABLE_ITEM(ID_BoardListTC) \
X_TABLE_ITEM(ID_DirPickerBoardList) \
X_TABLE_ITEM(ID_SkinPathTC) \
X_TABLE_ITEM(ID_DirPickerSkin) \
X_TABLE_ITEM(ID_ClearBrowserPath) \
X_TABLE_ITEM(ID_ClearBoardListPath) \
X_TABLE_ITEM(ID_ClearSkinPath) \
X_TABLE_ITEM(ID_BehaviorPanel) \
X_TABLE_ITEM(ID_OpenBNewTabCheck) \
X_TABLE_ITEM(ID_OnlyOneCategoryCheck) \
X_TABLE_ITEM(ID_OpenTNewTabCheck) \
X_TABLE_ITEM(ID_ThreadNewTabCombo) \
X_TABLE_ITEM(ID_LastReadThreadPosCheck) \
X_TABLE_ITEM(ID_ThreadJumpSetting) \
X_TABLE_ITEM(ID_RedrawNewThreadCheck) \
X_TABLE_ITEM(ID_ShowResponseCombo) \
X_TABLE_ITEM(ID_FavoriteNewTabCheck) \
X_TABLE_ITEM(ID_FavoriteOnlyOneFolder) \
X_TABLE_ITEM(ID_ColorFontSettingPanel) \
X_TABLE_ITEM(ID_TreeSampleLabel) \
X_TABLE_ITEM(ID_ThreadListSampleLabel) \
X_TABLE_ITEM(ID_ExtractSampleLabel) \
X_TABLE_ITEM(ID_LogWindowSampleLabel) \
X_TABLE_ITEM(ID_ThreadTitleSampleLabel) \
X_TABLE_ITEM(ID_ThreadViewSampleLabel) \
X_TABLE_ITEM(ID_KakikoSampleLabel) \
X_TABLE_ITEM(ID_MemoSampleLabel) \
X_TABLE_ITEM(ID_HintSampleLabel) \
X_TABLE_ITEM(ID_LinkSampleLabel) \
X_TABLE_ITEM(ID_OthersSampleLabel) \
X_TABLE_ITEM(ID_AllSampleLabel) \
X_TABLE_ITEM(ID_TreeFontButton) \
X_TABLE_ITEM(ID_ThreadListFontButton) \
X_TABLE_ITEM(ID_ExtractFontButton) \
X_TABLE_ITEM(ID_LogWindowFontButton) \
X_TABLE_ITEM(ID_ThreadTitleFontButton) \
X_TABLE_ITEM(ID_KakikoFontButton) \
X_TABLE_ITEM(ID_MemoFontButton) \
X_TABLE_ITEM(ID_HintFontButton) \
X_TABLE_ITEM(ID_LinkFontButton) \
X_TABLE_ITEM(ID_OthersFontButton) \
X_TABLE_ITEM(ID_AllFontButton) \
X_TABLE_ITEM(ID_BoardListBGColorButton) \
X_TABLE_ITEM(ID_ThreadListBGColorButton) \
X_TABLE_ITEM(ID_LogWindowBGColorButton) \
X_TABLE_ITEM(ID_ThreadTitleBGColorButton) \
X_TABLE_ITEM(ID_ThreadViewBGColorButton) \
X_TABLE_ITEM(ID_MemoBGColorButton) \
X_TABLE_ITEM(ID_HintBGColorButton) \
X_TABLE_ITEM(ID_PopupBGColorButton) \
X_TABLE_ITEM(ID_AllBGColorButton) \
X_TABLE_ITEM(ID_UserSettingPanel) \
X_TABLE_ITEM(ID_MaruUserID) \
X_TABLE_ITEM(ID_MaruUserPassword) \
X_TABLE_ITEM(ID_MaruAutoLoginCheck) \
X_TABLE_ITEM(ID_MaruLogoutButton) \
X_TABLE_ITEM(ID_BEMailAddress) \
X_TABLE_ITEM(ID_BEPassword) \
X_TABLE_ITEM(ID_TabColorSettingPanel) \
X_TABLE_ITEM(ID_ThreadTabColorCheck) \
X_TABLE_ITEM(ID_ActiveTabBGColorButton) \
X_TABLE_ITEM(ID_DefaultActiveTabSampleLabel) \
X_TABLE_ITEM(ID_ReadingActiveTabSampleLabel) \
X_TABLE_ITEM(ID_BrokenActiveTabSampleLabel) \
X_TABLE_ITEM(ID_CannotPostActiveTabSampleLabel) \
X_TABLE_ITEM(ID_UpdateActiveTabSampleLabel) \
X_TABLE_ITEM(ID_PartialContentActiveTabSampleLabel) \
X_TABLE_ITEM(ID_DeactiveTabBGColorButton) \
X_TABLE_ITEM(ID_DefaultDeactiveTabSampleLabel) \
X_TABLE_ITEM(ID_ReadingDeactiveTabSampleLabel) \
X_TABLE_ITEM(ID_BrokenDeactiveTabSampleLabel) \
X_TABLE_ITEM(ID_CannotPostDeactiveTabSampleLabel) \
X_TABLE_ITEM(ID_UpdateDeactiveTabSampleLabel) \
X_TABLE_ITEM(ID_PartialContentDeactiveTabSampleLabel) \
X_TABLE_ITEM(ID_ThreadTabDefaultFontColorButton) \
X_TABLE_ITEM(ID_ThreadTabReadingFontColorButton) \
X_TABLE_ITEM(ID_ThreadTabBrokenFontColorButton) \
X_TABLE_ITEM(ID_ThreadTabCannotPostFontColorButton) \
X_TABLE_ITEM(ID_ThreadTabUpdateFontColorButton) \
X_TABLE_ITEM(ID_ThreadTabPartialContentFontColorButton) \
X_TABLE_ITEM(ID_AutoReloadSampleLabel) \
X_TABLE_ITEM(ID_AutoReloadFontColorButton) \
X_TABLE_ITEM(ID_TabControlSettingPanel) \
X_TABLE_ITEM(ID_TabControlSettingPanelOpenTabChoice) \
X_TABLE_ITEM(ID_TabControlSettingPanelOpenTabFromOtherTabChoice) \
X_TABLE_ITEM(ID_TabControlSettingPanelCloseThreadViewTabChoice) \
X_TABLE_ITEM(ID_TabControlSettingPanelCloseThreadTabChoice) \
X_TABLE_ITEM(ID_JaneCloneEnumsEnd)

// 列挙型の実体をマクロで生成する
enum JANECLONE_ENUMS {
     ID_Quit = 1,
     #define X(Enum, String) Enum,
     X_TABLE
     #undef X
     ID_UserLastClosedThreadClick = 1000,  // ユーザーが最後に閉じたスレッドの情報をクリックした(動的なIDのため1000~)
     ID_UserLastClosedBoardClick  = 1100,  // ユーザーが最後に閉じた板の情報をクリックした(動的なIDのため1100~)     
     ID_ThreadContentBar  = 1500           // ThreadContentBar自体のID(動的なIDのため1500~)
};

// 列挙型を文字列化
Begin_Enum_String( JANECLONE_ENUMS )
{

#define X(Enum, String) \
     Enum_String( Enum )                                \

     X_TABLE
#undef X

}
End_Enum_String;

#endif /* ENUMS_HPP_ */

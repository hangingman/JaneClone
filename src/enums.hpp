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

// 使いたい列挙型を定義する.バックスラッシュの後ろに文字があるとコンパイルエラーになるので注意
#define X_TABLE \
<<<<<<< HEAD
X_TABLE_ITEM(ID_Restart)			/* 再起動						　　　　*/ \
X_TABLE_ITEM(ID_Test)				/* テスト用						　　　　*/ \
X_TABLE_ITEM(ID_Logging)			/* ログ出力制御用						*/ \
X_TABLE_ITEM(ID_WindowMinimize)			/* ウィンドウを最小化する					*/ \
X_TABLE_ITEM(ID_GetBoardList)			/* 板一覧情報取得						*/ \
X_TABLE_ITEM(ID_CheckLogDirectory)		/* 保存されているログをスレッド一覧に表示する			*/ \
X_TABLE_ITEM(ID_GetVersionInfo)			/* バージョン情報						*/ \
X_TABLE_ITEM(ID_ThreadNoteBook)			/* スレッド一覧ノートブックに使うID				*/ \
X_TABLE_ITEM(ID_BoardNoteBook)			/* 板一覧用ノートブックに使うID					*/ \
X_TABLE_ITEM(ID_ThumbnailNoteBook)		/* 画像ビューア用ID						*/ \
X_TABLE_ITEM(ID_BoardTreectrl)			/* 板一覧ツリー用ID						*/ \
X_TABLE_ITEM(ID_ShingetsuBoardTreectrl)		/* 新月公開ノード一覧ツリー用ID					*/ \
X_TABLE_ITEM(ID_BoardTreeNoteBook)		/* 板一覧ツリーを載せるノートブックに使うID			*/ \
X_TABLE_ITEM(ID_OneBoardTabClose)		/* スレッド一覧タブをひとつ閉じる				*/ \
X_TABLE_ITEM(ID_ExcepSelTabClose)		/* 現在選択されていないスレッド一覧タブを閉じる			*/ \
X_TABLE_ITEM(ID_AllBoardTabClose)		/* すべてのスレッド一覧タブを閉じる				*/ \
X_TABLE_ITEM(ID_AllLeftBoardTabClose)		/* これより左のスレッド一覧タブをを閉じる			*/ \
X_TABLE_ITEM(ID_AllRightBoardTabClose)		/* これより右のスレッド一覧タブを閉じる				*/ \
X_TABLE_ITEM(ID_OnOpenBoardByBrowser)		/* スレッド一覧をブラウザで開く					*/ \
X_TABLE_ITEM(ID_ReloadOneBoard)			/* アクティブなスレッド一覧をひとつ更新する			*/ \
X_TABLE_ITEM(ID_CopyBURLToClipBoard)		/* 板のURLをクリップボードにコピーする				*/ \
X_TABLE_ITEM(ID_CopyBTitleToClipBoard)		/* 板のタイトルをクリップボードにコピーする			*/ \
X_TABLE_ITEM(ID_CopyBBothDataToClipBoard)	/* 板のURLとタイトルをクリップボードにコピーする		*/ \
X_TABLE_ITEM(ID_CopyTURLToClipBoard)		/* スレッドのURLをクリップボードにコピーする			*/ \
X_TABLE_ITEM(ID_CopyTTitleToClipBoard)		/* スレッドのタイトルをクリップボードにコピーする		*/ \
X_TABLE_ITEM(ID_CopyTBothDataToClipBoard)	/* スレッドのURLとタイトルをクリップボードにコピーする		*/ \
X_TABLE_ITEM(ID_CopyTContentsToClipBoard)	/* スレッドの内容をクリップボードにコピーする			*/ \
X_TABLE_ITEM(ID_CopyTAllToClipBoard)		/* スレッドの内容, URL, タイトルをクリップボードにコピーする	*/ \
X_TABLE_ITEM(ID_OneThreadTabClose)		/* スレタブをひとつ閉じる					*/ \
X_TABLE_ITEM(ID_ExcepSelThreadTabClose)		/* 現在選択されていないスレタブを閉じる				*/ \
X_TABLE_ITEM(ID_AllThreadTabClose)		/* すべてのスレタブを閉じる					*/ \
X_TABLE_ITEM(ID_AllLeftThreadTabClose)		/* これより左のスレタブをを閉じる				*/ \
X_TABLE_ITEM(ID_AllRightThreadTabClose)		/* これより右のスレタブを閉じる					*/ \
X_TABLE_ITEM(ID_OnOpenThreadByBrowser)		/* スレッドをブラウザで開く					*/ \
X_TABLE_ITEM(ID_SaveDatFile)			/* datファイルに名前を付けて保存				*/ \
X_TABLE_ITEM(ID_SaveDatFileToClipBoard)		/* datをクリップボードにコピー					*/ \
X_TABLE_ITEM(ID_DeleteDatFile)			/* このログを削除						*/ \
X_TABLE_ITEM(ID_ReloadThisThread)		/* スレッドの再読み込み						*/ \
X_TABLE_ITEM(ID_CallResponseWindow)		/* 書き込み用のウィンドウを呼び出す				*/ \
X_TABLE_ITEM(ID_BoardListCtrl)			/* 板一覧リスト自体を表すID					*/ \
X_TABLE_ITEM(ID_ShingetsuBoardListCtrl)		/* 新月の板一覧リスト自体を表すID				*/ \
X_TABLE_ITEM(ID_FontDialogBoardTree)		/* 板一覧ツリー部分のフォントの指定を行う			*/ \
X_TABLE_ITEM(ID_FontDialogLogWindow)		/* ログ出力画面部分のフォントの指定を行う			*/ \
X_TABLE_ITEM(ID_FontDialogBoardNotebook)	/* スレッド一覧部分のフォントの指定を行う			*/ \
X_TABLE_ITEM(ID_FontDialogThreadNotebook)	/* スレッド画面部分のフォント設定を呼び出			*/ \
X_TABLE_ITEM(ID_FontDialogThreadContents)	/* スレッド内で使用するフォント設定を呼び			*/ \
X_TABLE_ITEM(ID_URLWindowButton)		/* URL入力ウィンドウのボタンを表すID				*/ \
X_TABLE_ITEM(ID_UserLastClosedThreadMenuUp)	/* ユーザーが最後に閉じたスレッドの情報を			*/ \
X_TABLE_ITEM(ID_UserLastClosedBoardMenuUp)	/* ユーザーが最後に閉じた板の情報を項目に			*/ \
X_TABLE_ITEM(ID_UserLookingTabsMenuUp)		/* ユーザーが現在見ているスレタブ・板タブ			*/ \
X_TABLE_ITEM(ID_UserLookingTabsControl)		/* ユーザーが現在フォーカスしているウィン			*/ \
X_TABLE_ITEM(ID_UserLookingTabsMenuClick)	/* ユーザーが現在見ているスレタブ・板タブ			*/ \
X_TABLE_ITEM(ID_OnOpenJaneCloneOfficial)	/* JaneClone公式サイトをブラウザで開く				*/ \
X_TABLE_ITEM(ID_OnOpen2chViewerOfficial)	/* ●公式サイトをブラウザで開く					*/ \
X_TABLE_ITEM(ID_ThreadSearchBar)		/* スレッド検索ボックスのID					*/ \
X_TABLE_ITEM(ID_BoardSearchBar)			/* 板名検索ボックスのID						*/ \
X_TABLE_ITEM(ID_ShingetsuBoardSearchBar)	/* 新月の板名検索ボックスのID					*/ \
X_TABLE_ITEM(ID_ThreadSearchBarCombo)		/* スレッド検索ボックスのコンボボックス				*/ \
X_TABLE_ITEM(ID_BoardSearchBarCombo)		/* 板名検索ボックスのコンボボックス				*/ \
X_TABLE_ITEM(ID_SearchBoxDoSearch)		/* 検索ボックスで検索ボタンを押す				*/ \
X_TABLE_ITEM(ID_SearchBoxUp)			/* 検索ボックスで上に移動					*/ \
X_TABLE_ITEM(ID_SearchBoxDown)			/* 検索ボックスで下に移動					*/ \
X_TABLE_ITEM(ID_SearchBoxCopy)			/* 検索ボックスでコピー実行					*/ \
X_TABLE_ITEM(ID_SearchBoxCut)			/* 検索ボックスで切り取り実行					*/ \
X_TABLE_ITEM(ID_SearchBoxSelectAll)		/* 検索ボックスで全て選択					*/ \
X_TABLE_ITEM(ID_SearchBoxClear)			/* 検索ボックスをクリア						*/ \
X_TABLE_ITEM(ID_SearchBoxNormalSearch)		/* 検索ボックスで通常検索					*/ \
X_TABLE_ITEM(ID_SearchBoxRegexSearch)		/* 検索ボックスで正規表現検索					*/ \
X_TABLE_ITEM(ID_SearchBarHide)			/* 検索ボックスを隠す						*/ \
X_TABLE_ITEM(ID_FloatToolBar)			/* アイコン付きのツールバー自体のID				*/ \
X_TABLE_ITEM(ID_SwitchSeparateXY)		/* 縦⇔横分割切り替え						*/ \
X_TABLE_ITEM(ID_SwitchTwoThreePane)		/* ２⇔３ペイン切り替え						*/ \
X_TABLE_ITEM(ID_ShowBoardListTree)		/* 板一覧ツリー表示切り替え					*/ \
X_TABLE_ITEM(ID_SwitchRightPane)		/* 右側ペインの内容切り替え					*/ \
X_TABLE_ITEM(ID_CallSettingWindow)		/* ビューア設定画面を呼び出す					*/ \
X_TABLE_ITEM(ID_ResponseWindow)			/* */ \
X_TABLE_ITEM(ID_QuitResponseWindow)		/* */ \
X_TABLE_ITEM(ID_PostResponse)			/* */ \
X_TABLE_ITEM(ID_PostConfirmForm)		/* */ \
X_TABLE_ITEM(ID_TCBAutoReload)			/* */ \
X_TABLE_ITEM(ID_TCBRedResExtract)		/* */ \
X_TABLE_ITEM(ID_TCBRefresh)			/* */ \
X_TABLE_ITEM(ID_TCBScrollToNewRes)		/* */ \
X_TABLE_ITEM(ID_TCBStop)			/* */ \
X_TABLE_ITEM(ID_TCBResExtract)			/* */ \
X_TABLE_ITEM(ID_TCBNewThread)			/* */ \
X_TABLE_ITEM(ID_TCBBookMark)			/* */ \
X_TABLE_ITEM(ID_TCBDeleteLog)			/* */ \
X_TABLE_ITEM(ID_TCBClose)			/* */ \
X_TABLE_ITEM(ID_TCBNormalSearch)		/* */ \
X_TABLE_ITEM(ID_TCBHideSearchBar)		/* */ \
X_TABLE_ITEM(ID_TCBForward)			/* */ \
X_TABLE_ITEM(ID_TCBBackward)			/* */ \
X_TABLE_ITEM(ID_ThreadContentBarUpdate)		/* */ \
X_TABLE_ITEM(ID_CopyFromHtmlWindow)		/* HtmlWindowで選択しているテキストをクリップボードにコピーする  */ \
X_TABLE_ITEM(ID_CopyURLFromHtmlWindow)		/* HtmlWindowで選択しているURLをクリップボードにコピーする	 */ \
X_TABLE_ITEM(ID_SelectAllTextHtmlWindow)	/* HtmlWindowでテキストを全て選択する				 */ \
X_TABLE_ITEM(ID_SearchSelectWordByYahoo)	/* 選択したテキストでヤフー検索					 */ \
X_TABLE_ITEM(ID_SearchSelectWordByGoogle)	/* 選択したテキストでGoogle検索					 */ \
X_TABLE_ITEM(ID_SearchSelectWordByAmazon)	/* 選択したテキストでAmazon検索					 */ \
X_TABLE_ITEM(ID_SearchThreadBySelectWord)	/* 選択したテキストでスレタイ検索				 */ \
X_TABLE_ITEM(ID_CallResponseWindowAnchor)	/* 書き込み用のウィンドウをアンカー付きで呼び出す		 */ \
X_TABLE_ITEM(ID_CallResponseWindowQuote)	/* 書き込み用のウィンドウをレス引用付きで呼び出す		 */ \
X_TABLE_ITEM(ID_HtmlSourceDebug)		/* */ \
X_TABLE_ITEM(ID_OneThumbnailTabClose)		/* 画像タブをひとつ閉じる	                                 */ \
X_TABLE_ITEM(ID_AllThumbnailTabClose)		/* すべての画像タブを閉じる					 */ \
X_TABLE_ITEM(ID_AllLeftThumbnailTabClose)	/* これより左の画像タブをを閉じる				 */ \
X_TABLE_ITEM(ID_AllRightThumbnailTabClose)	/* これより右の画像タブを閉じる					 */ \
X_TABLE_ITEM(ID_SelectLeftThumbnailTab)		/* 左の画像タブに移動						 */ \
X_TABLE_ITEM(ID_SelectRightThumbnailTab)	/* 右の画像タブに移動						 */ \
X_TABLE_ITEM(ID_OnOpenImageByBrowser)		/* 画像をブラウザで開く						 */ \
X_TABLE_ITEM(ID_HideThumbnailTab)		/* 画像ビューアを隠す						 */ \
X_TABLE_ITEM(ID_OnClickMenuCOL_CHK) 		/* 新着チェック */ \
X_TABLE_ITEM(ID_OnClickMenuCOL_NUM)		/* 番号	        */ \
X_TABLE_ITEM(ID_OnClickMenuCOL_TITLE)		/* タイトル     */ \
X_TABLE_ITEM(ID_OnClickMenuCOL_RESP)		/* レス	        */ \
X_TABLE_ITEM(ID_OnClickMenuCOL_CACHEDRES)	/* 取得	        */ \
X_TABLE_ITEM(ID_OnClickMenuCOL_NEWRESP)		/* 新着	        */ \
X_TABLE_ITEM(ID_OnClickMenuCOL_INCRESP)		/* 増レス       */ \
X_TABLE_ITEM(ID_OnClickMenuCOL_MOMENTUM)	/* 勢い	        */ \
X_TABLE_ITEM(ID_OnClickMenuCOL_LASTUP)		/* 最終取得     */ \
X_TABLE_ITEM(ID_OnClickMenuCOL_SINCE)		/* SINCE        */ \
X_TABLE_ITEM(ID_OnClickMenuCOL_OID)		/* 固有番号     */ \
X_TABLE_ITEM(ID_OnClickMenuCOL_BOARDNAME)	/* 板	        */ \
X_TABLE_ITEM(ID_OnCancelSetting)		/* 設定画面をキャンセル	      */ \
X_TABLE_ITEM(ID_OnOkSetting)			/* 設定画面をOkで終了	      */ \
X_TABLE_ITEM(ID_SettingPanelTree)		/* 設定画面の描画を切り替える */ \
X_TABLE_ITEM(ID_SettingPanelUpdate)		/* 設定画面の更新をメインスレッドに伝える	     */ \
X_TABLE_ITEM(ID_NetworkPanelUpdate)		/* ネットワーク設定画面の更新をメインスレッドに伝える*/ \
X_TABLE_ITEM(ID_NetworkPanel)			/* ネットワーク設定用画面		  */ \
X_TABLE_ITEM(ID_NetworkPanelUseProxy)		/* プロキシを使用するかどうか		  */ \
X_TABLE_ITEM(ID_NetworkPanelUseProxyCache)	/* プロキシでキャッシュを使用するかどうか */ \
X_TABLE_ITEM(ID_NetworkPanelBasicAuthUserName)	/* ベーシック認証のユーザー名		  */ \
X_TABLE_ITEM(ID_NetworkPanelBasicAuthPassword)	/* ベーシック認証のパスワード		  */ \
X_TABLE_ITEM(ID_NetworkPanelProxyReceiveAddr)	/* Proxy受信用アドレス			  */ \
X_TABLE_ITEM(ID_NetworkPanelProxyReceivePort)	/* Proxy受信用ポート			  */ \
X_TABLE_ITEM(ID_NetworkPanelProxySendAddr)	/* Proxy送信用アドレス			  */ \
X_TABLE_ITEM(ID_NetworkPanelProxySendPort)	/* Proxy送信用ポート			  */ \
X_TABLE_ITEM(ID_NetworkPanelProxySSLAuthAddr)	/* Proxy SSL認証用アドレス		  */ \
X_TABLE_ITEM(ID_NetworkPanelProxySSLAuthPort)	/* Proxy SSL認証用ポート		  */ \
X_TABLE_ITEM(ID_PathSettingPanel) 		/* 各種パス設定用画面					*/ \
X_TABLE_ITEM(ID_BrowserCheck)			/* 使用するブラウザを指定するかどうか			*/ \
X_TABLE_ITEM(ID_BrowserPathTC)			/* ブラウザのパスをもつテキストコントロール		*/ \
X_TABLE_ITEM(ID_DirPickerBrowser)		/* ブラウザのパスを参照する				*/ \
X_TABLE_ITEM(ID_BoardListTC)			/* 板一覧リストを保持する場所をもつテキストコントロール */ \
X_TABLE_ITEM(ID_DirPickerBoardList)		/* 板一覧リストのパスを参照する				*/ \
X_TABLE_ITEM(ID_SkinPathTC)			/* スキン用のファイルのパスをもつテキストコントロール	*/ \
X_TABLE_ITEM(ID_DirPickerSkin)			/* スキン用ファイルのパスを参照する			*/ \
X_TABLE_ITEM(ID_ClearBrowserPath)		/* ブラウザパス設定値を削除				*/ \
X_TABLE_ITEM(ID_ClearBoardListPath)		/* 板一覧リストのパス設定値を削除			*/ \
X_TABLE_ITEM(ID_ClearSkinPath)			/* スキンのパス設定値を削除				*/ \
X_TABLE_ITEM(ID_BehaviorPanel)			/* 動作設定用パネル					*/ \
X_TABLE_ITEM(ID_OpenBNewTabCheck)		/* 新しいタブで板を開く					*/ \
X_TABLE_ITEM(ID_OnlyOneCategoryCheck)		/* カテゴリを一つしか開かない				*/ \
X_TABLE_ITEM(ID_OpenTNewTabCheck)		/* 新しいタブでスレを開く				*/ \
X_TABLE_ITEM(ID_ThreadNewTabCombo)		/* 新しいタブでスレを開く				*/ \
X_TABLE_ITEM(ID_LastReadThreadPosCheck)		/* 最後に見たスレの位置にジャンプ			*/ \
X_TABLE_ITEM(ID_ThreadJumpSetting)		/* 最後に見たスレの位置にジャンプ			*/ \
X_TABLE_ITEM(ID_RedrawNewThreadCheck)		/* 新着チェック時に既得レスを描き直す			*/ \
X_TABLE_ITEM(ID_ShowResponseCombo)		/* 既得スレの表示レス数					*/ \
X_TABLE_ITEM(ID_FavoriteNewTabCheck)		/* お気に入りを新しいタブで開く				*/ \
X_TABLE_ITEM(ID_FavoriteOnlyOneFolder)		/* お気に入りフォルダを一つしか開かない			*/ \
X_TABLE_ITEM(ID_ColorFontSettingPanel) 		/* 色・フォント設定用画面	      */ \
X_TABLE_ITEM(ID_TreeSampleLabel)		/* ツリーサンプル		      */ \
X_TABLE_ITEM(ID_ThreadListSampleLabel)		/* スレ欄サンプル		      */ \
X_TABLE_ITEM(ID_ExtractSampleLabel)		/* 抽出サンプル			      */ \
X_TABLE_ITEM(ID_LogWindowSampleLabel)		/* ログ出力画面サンプル		      */ \
X_TABLE_ITEM(ID_ThreadTitleSampleLabel)		/* スレッドタイトルサンプル	      */ \
X_TABLE_ITEM(ID_ThreadViewSampleLabel)		/* スレビューサンプル		      */ \
X_TABLE_ITEM(ID_KakikoSampleLabel)		/* 書き込みサンプル		      */ \
X_TABLE_ITEM(ID_MemoSampleLabel)		/* メモ欄サンプル		      */ \
X_TABLE_ITEM(ID_HintSampleLabel)		/* ヒントサンプル		      */ \
X_TABLE_ITEM(ID_LinkSampleLabel)		/* リンクサンプル		      */ \
X_TABLE_ITEM(ID_OthersSampleLabel)		/* その他サンプル		      */ \
X_TABLE_ITEM(ID_AllSampleLabel)			/* ツリーフォント設定ボタン	      */ \
X_TABLE_ITEM(ID_TreeFontButton)			/* スレ欄フォント設定ボタン	      */ \
X_TABLE_ITEM(ID_ThreadListFontButton)		/* 抽出フォント設定ボタン	      */ \
X_TABLE_ITEM(ID_ExtractFontButton)		/* ログ出力画面フォント設定ボタン     */ \
X_TABLE_ITEM(ID_LogWindowFontButton)		/* スレッドタイトルフォント設定ボタン */ \
X_TABLE_ITEM(ID_ThreadTitleFontButton)		/* 書き込みフォント設定ボタン	      */ \
X_TABLE_ITEM(ID_KakikoFontButton)		/* メモ欄フォント設定ボタン	      */ \
X_TABLE_ITEM(ID_MemoFontButton)			/* ヒントフォント設定ボタン	      */ \
X_TABLE_ITEM(ID_HintFontButton)			/* リンクフォント設定ボタン	      */ \
X_TABLE_ITEM(ID_LinkFontButton)			/* その他フォント設定ボタン	      */ \
X_TABLE_ITEM(ID_OthersFontButton)		/* すべてフォント設定ボタン	      */ \
X_TABLE_ITEM(ID_AllFontButton)			/* スレ欄背景色設定ボタン	      */ \
X_TABLE_ITEM(ID_BoardListBGColorButton)		/* 抽出背景色設定ボタン		      */ \
X_TABLE_ITEM(ID_ThreadListBGColorButton)	/* ログ出力画面背景色設定ボタン	      */ \
X_TABLE_ITEM(ID_LogWindowBGColorButton)		/* スレッドタイトル背景色設定ボタン   */ \
X_TABLE_ITEM(ID_ThreadTitleBGColorButton)	/* スレビュー背景色設定ボタン	      */ \
X_TABLE_ITEM(ID_ThreadViewBGColorButton)	/* メモ欄背景色設定ボタン	      */ \
X_TABLE_ITEM(ID_MemoBGColorButton)		/* ヒント背景色設定ボタン	      */ \
X_TABLE_ITEM(ID_HintBGColorButton)		/* ポップアップ背景色設定ボタン	      */ \
X_TABLE_ITEM(ID_PopupBGColorButton)		/* すべての背景色設定ボタン	      */ \
X_TABLE_ITEM(ID_AllBGColorButton) 		/* */ \
X_TABLE_ITEM(ID_UserSettingPanel) 		/* */ \
X_TABLE_ITEM(ID_MaruUserID) 			\
X_TABLE_ITEM(ID_MaruUserPassword)			\
X_TABLE_ITEM(ID_MaruAutoLoginCheck)			\
X_TABLE_ITEM(ID_MaruLogoutButton)			\
X_TABLE_ITEM(ID_BEMailAddress)			\
X_TABLE_ITEM(ID_BEPassword) 		\
X_TABLE_ITEM(ID_TabColorSettingPanel) 		\
X_TABLE_ITEM(ID_ThreadTabColorCheck) 		\
X_TABLE_ITEM(ID_ActiveTabBGColorButton) 		\
X_TABLE_ITEM(ID_DefaultActiveTabSampleLabel) 		\
X_TABLE_ITEM(ID_ReadingActiveTabSampleLabel) 		\
X_TABLE_ITEM(ID_BrokenActiveTabSampleLabel) 		\
X_TABLE_ITEM(ID_CannotPostActiveTabSampleLabel) 		\
X_TABLE_ITEM(ID_UpdateActiveTabSampleLabel) 		\
X_TABLE_ITEM(ID_PartialContentActiveTabSampleLabel) 		\
X_TABLE_ITEM(ID_DeactiveTabBGColorButton) 		\
X_TABLE_ITEM(ID_DefaultDeactiveTabSampleLabel) 		\
X_TABLE_ITEM(ID_ReadingDeactiveTabSampleLabel) 		\
X_TABLE_ITEM(ID_BrokenDeactiveTabSampleLabel) 		\
X_TABLE_ITEM(ID_CannotPostDeactiveTabSampleLabel) 		\
X_TABLE_ITEM(ID_UpdateDeactiveTabSampleLabel) 		\
X_TABLE_ITEM(ID_PartialContentDeactiveTabSampleLabel) 		\
X_TABLE_ITEM(ID_ThreadTabDefaultFontColorButton) 		\
X_TABLE_ITEM(ID_ThreadTabReadingFontColorButton) 		\
X_TABLE_ITEM(ID_ThreadTabBrokenFontColorButton) 		\
X_TABLE_ITEM(ID_ThreadTabCannotPostFontColorButton) 		\
X_TABLE_ITEM(ID_ThreadTabUpdateFontColorButton) 		\
X_TABLE_ITEM(ID_ThreadTabPartialContentFontColorButton) 		\
X_TABLE_ITEM(ID_AutoReloadSampleLabel) 		\
X_TABLE_ITEM(ID_AutoReloadFontColorButton) 		\
=======
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
>>>>>>> 0e6a76b248a2c3cc1e7d208f80056b0b2c17ff65
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

#define X(Enum, String)  \
     Enum_String( Enum ) \

     X_TABLE
#undef X

}
End_Enum_String;

#endif /* ENUMS_HPP_ */

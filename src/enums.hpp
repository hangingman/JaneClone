/**
 * enums.hpp - janeclones' enums
 *
 * Copyright (C) 2013-2014 Hiroyuki Nagata <idiotpanzer@gmail.com>
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

#ifndef ENUMS_HPP_
#define ENUMS_HPP_

#include "enumstring.hpp"

// テーブル要素
#define X_TABLE_ITEM(x) X(x, STR(x))

// 使いたい列挙型を定義する.バックスラッシュの後ろに文字があるとコンパイルエラーになるので注意
#define X_TABLE \
X_TABLE_ITEM(ID_Restart)			/* 再起動						　　　　*/ \
X_TABLE_ITEM(ID_Test)				/* テスト用						　　　　*/ \
X_TABLE_ITEM(ID_Logging)			/* ログ出力制御用						*/ \
X_TABLE_ITEM(ID_WindowMinimize)			/* ウィンドウを最小化する					*/ \
X_TABLE_ITEM(ID_GetBoardList)			/* 板一覧情報取得						*/ \
X_TABLE_ITEM(ID_CheckLogDirectory)		/* 保存されているログをスレッド一覧に表示する			*/ \
X_TABLE_ITEM(ID_GetVersionInfo)			/* バージョン情報						*/ \
X_TABLE_ITEM(ID_WxJaneClone)                    /* メインのウィンドウID                                         */ \
X_TABLE_ITEM(ID_ThreadNoteBook)			/* スレッド一覧ノートブックに使うID				*/ \
X_TABLE_ITEM(ID_BoardNoteBook)			/* 板一覧用ノートブックに使うID					*/ \
X_TABLE_ITEM(ID_ThumbnailNoteBook)		/* 画像ビューア用ID						*/ \
X_TABLE_ITEM(ID_BoardTreectrl)			/* 板一覧ツリー用ID						*/ \
X_TABLE_ITEM(ID_ShingetsuBoardTreectrl)		/* 新月公開ノード一覧ツリー用ID					*/ \
X_TABLE_ITEM(ID_FavsTreectrl)                   /* お気に入りツリー用ID                                         */ \
X_TABLE_ITEM(ID_NowReadingTreectrl)             /* 閲覧中ツリー用ID                                             */ \
X_TABLE_ITEM(ID_BoardTreeNoteBook)		/* 板一覧ツリーを載せるノートブックに使うID			*/ \
X_TABLE_ITEM(ID_ThreadContentWindow)            /* スレビュー画面用ID                                           */ \
X_TABLE_ITEM(ID_OneBoardTabClose)		/* スレッド一覧タブをひとつ閉じる				*/ \
X_TABLE_ITEM(ID_ExcepSelTabClose)		/* 現在選択されていないスレッド一覧タブを閉じる			*/ \
X_TABLE_ITEM(ID_AllBoardTabClose)		/* すべてのスレッド一覧タブを閉じる				*/ \
X_TABLE_ITEM(ID_AllLeftBoardTabClose)		/* これより左のスレッド一覧タブをを閉じる			*/ \
X_TABLE_ITEM(ID_AllRightBoardTabClose)		/* これより右のスレッド一覧タブを閉じる				*/ \
X_TABLE_ITEM(ID_OnOpenBoardByBrowser)		/* スレッド一覧をブラウザで開く					*/ \
X_TABLE_ITEM(ID_ReloadOneBoard)			/* アクティブなスレッド一覧をひとつ更新する			*/ \
X_TABLE_ITEM(ID_ReloadAllBoard)			/* すべてのタブのスレッド一覧を更新する　　　			*/ \
X_TABLE_ITEM(ID_CopyBURLToClipBoard)		/* 板のURLをクリップボードにコピーする				*/ \
X_TABLE_ITEM(ID_CopyBTitleToClipBoard)		/* 板のタイトルをクリップボードにコピーする			*/ \
X_TABLE_ITEM(ID_CopyBBothDataToClipBoard)	/* 板のURLとタイトルをクリップボードにコピーする		*/ \
X_TABLE_ITEM(ID_DeleteBSelectedDatFile)         /* 選択中のログを削除                                           */ \
X_TABLE_ITEM(ID_DeleteBAllDatFile) 		/* すべてのログを削除                                           */ \
X_TABLE_ITEM(ID_DeleteBAllDatFileWithoutFav)	/* お気に入り以外のログを削除                                   */ \
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
X_TABLE_ITEM(ID_JaneCloneMgrUpdate)		/* Auiマネージャーの更新を行う	         		        */ \
X_TABLE_ITEM(ID_CommonAuiToolBarUpdate)         /* Auiツールバーの更新を行う 				        */ \
X_TABLE_ITEM(ID_FavsTreectrlUpdate)             /* お気に入りツリーの更新を行う					*/ \
X_TABLE_ITEM(ID_NowReadingTreectrlUpdate)	/* 閲覧中ツリーの更新を行う					*/ \
X_TABLE_ITEM(ID_UserLookingTabsControl)		/* ユーザーが現在フォーカスしているウィン			*/ \
X_TABLE_ITEM(ID_MoveNextTab)                    /* 表示しているタブを移動する(次)				*/ \
X_TABLE_ITEM(ID_MovePrevTab)			/* 表示しているタブを移動する(前)				*/ \
X_TABLE_ITEM(ID_UserLookingTabsMenuClick)	/* ユーザーが現在見ているスレタブ・板タブ			*/ \
X_TABLE_ITEM(ID_OnOpenJaneCloneOfficial)	/* JaneClone公式サイトをブラウザで開く				*/ \
X_TABLE_ITEM(ID_OnOpen2chViewerOfficial)	/* ●公式サイトをブラウザで開く					*/ \
X_TABLE_ITEM(ID_ThreadContentSearchBar)		/* スレッド内容バーの検索ボックスのID				*/ \
X_TABLE_ITEM(ID_ThreadSearchBar)		/* スレッド検索ボックスのID					*/ \
X_TABLE_ITEM(ID_BoardSearchBar)			/* 板名検索ボックスのID						*/ \
X_TABLE_ITEM(ID_ShingetsuBoardSearchBar)	/* 新月の板名検索ボックスのID					*/ \
X_TABLE_ITEM(ID_FavsSearchBar)                  /* お気に入りのデータ検索ボックスのID                           */ \
X_TABLE_ITEM(ID_NowReadingSearchBar)		/* 閲覧中のデータ検索ボックスのID                               */ \
X_TABLE_ITEM(ID_ThreadSearchBarCombo)		/* スレッド検索ボックスのコンボボックス				*/ \
X_TABLE_ITEM(ID_BoardSearchBarCombo)		/* 板名検索ボックスのコンボボックス				*/ \
X_TABLE_ITEM(ID_FavsSearchBarCombo)             /* お気に入りのデータ検索ボックスのコンボボックス		*/ \
X_TABLE_ITEM(ID_NowReadingSearchBarCombo)	/* 閲覧中のデータ検索ボックスのコンボボックス			*/ \
X_TABLE_ITEM(ID_ThreadSearchBoxDoSearch)        /* 検索ボックス(スレッド一覧)で検索ボタンを押す			*/ \
X_TABLE_ITEM(ID_BoardSearchBoxDoSearch)		/* 検索ボックス(板一覧)で検索ボタンを押す			*/ \
X_TABLE_ITEM(ID_FavsSearchBoxDoSeach)           /* 検索ボックス(お気に入り一覧)で検索ボタンを押す		*/ \
X_TABLE_ITEM(ID_NowReadingBoxDoSeach)		/* 検索ボックス(閲覧中一覧)で検索ボタンを押す			*/ \
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
X_TABLE_ITEM(ID_CallSettingWindow)		/* 設定画面を呼び出す					        */ \
X_TABLE_ITEM(ID_CallViewerSettingWindow)        /* ビューア設定画面を呼び出す 					*/ \
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
X_TABLE_ITEM(ID_TCBHideSearchBar)		/* スレッド内容バーの内容を隠す                                  */ \
X_TABLE_ITEM(ID_TCBForward)			/* */ \
X_TABLE_ITEM(ID_TCBBackward)			/* */ \
X_TABLE_ITEM(ID_ThreadContentBarUpdate)		/* */ \
X_TABLE_ITEM(ID_CopyFromHtmlWindow)		/* HtmlWindowで選択しているテキストをクリップボードにコピーする  */ \
X_TABLE_ITEM(ID_CopyURLFromHtmlWindow)		/* HtmlWindowで選択しているURLをクリップボードにコピーする	 */ \
X_TABLE_ITEM(ID_ShowRawHtmlFromHtmlWindow)      /* HtmlWindowでスレッドのHTMLソースを表示する                        */ \
X_TABLE_ITEM(ID_SelectAllTextHtmlWindow)	/* HtmlWindowでテキストを全て選択する				 */ \
X_TABLE_ITEM(ID_SearchSelectWordByYahoo)	/* 選択したテキストでヤフー検索					 */ \
X_TABLE_ITEM(ID_SearchSelectWordByGoogle)	/* 選択したテキストでGoogle検索					 */ \
X_TABLE_ITEM(ID_SearchSelectWordByAmazon)	/* 選択したテキストでAmazon検索					 */ \
X_TABLE_ITEM(ID_SearchThreadBySelectWord)	/* 選択したテキストでスレタイ検索				 */ \
X_TABLE_ITEM(ID_CallResponseWindowAnchor)	/* 書き込み用のウィンドウをアンカー付きで呼び出す		 */ \
X_TABLE_ITEM(ID_CallResponseWindowQuote)	/* 書き込み用のウィンドウをレス引用付きで呼び出す		 */ \
X_TABLE_ITEM(ID_HtmlSourceDebug)		/* */ \
X_TABLE_ITEM(ID_ImagePanel)		        /* 画像パネル                                                    */ \
X_TABLE_ITEM(ID_OneThumbnailTabClose)		/* 画像タブをひとつ閉じる	                                 */ \
X_TABLE_ITEM(ID_AllThumbnailTabClose)		/* すべての画像タブを閉じる					 */ \
X_TABLE_ITEM(ID_SaveAsImages)		        /* 名前をつけて保存					         */ \
X_TABLE_ITEM(ID_SaveAsImagesAll)		/* すべて保存					         	 */ \
X_TABLE_ITEM(ID_AllLeftThumbnailTabClose)	/* これより左の画像タブをを閉じる				 	 */ \
X_TABLE_ITEM(ID_AllRightThumbnailTabClose)	/* これより右の画像タブを閉じる					 */ \
X_TABLE_ITEM(ID_SelectLeftThumbnailTab)		/* 左の画像タブに移動						 */ \
X_TABLE_ITEM(ID_SelectRightThumbnailTab)	/* 右の画像タブに移動						 */ \
X_TABLE_ITEM(ID_OnOpenImageByBrowser)		/* 画像をブラウザで開く						 */ \
X_TABLE_ITEM(ID_OpenThreadGotImage)		/* 画像取得元のスレッドを開く					 */ \
X_TABLE_ITEM(ID_CopyImageURLToClipBoard)	/* 画像URLをクリップボードにコピーする				 */ \
X_TABLE_ITEM(ID_Rotate90AntiClockwise)          /* 画像を90度左回転させる                                        */ \
X_TABLE_ITEM(ID_Rotate90Clockwise)              /* 画像を90度右回転させる                                        */ \
X_TABLE_ITEM(ID_ZoomIn)                         /* 画像を拡大する                                                */ \
X_TABLE_ITEM(ID_ZoomOut)                        /* 画像を縮小する                                                */ \
X_TABLE_ITEM(ID_ResetImageOriginalSize)         /* 画像のサイズを元に戻す                                        */ \
X_TABLE_ITEM(ID_HideThumbnailTab)		/* 画像ビューアを隠す						 */ \
X_TABLE_ITEM(ID_OnClickMenuCOL_CHK) 		/* 新着チェック                                         */ \
X_TABLE_ITEM(ID_OnClickMenuCOL_NUM)		/* 番号	                                                */ \
X_TABLE_ITEM(ID_OnClickMenuCOL_TITLE)		/* タイトル                                             */ \
X_TABLE_ITEM(ID_OnClickMenuCOL_RESP)		/* レス	                                                */ \
X_TABLE_ITEM(ID_OnClickMenuCOL_CACHEDRES)	/* 取得	                                                */ \
X_TABLE_ITEM(ID_OnClickMenuCOL_NEWRESP)		/* 新着	                                                */ \
X_TABLE_ITEM(ID_OnClickMenuCOL_INCRESP)		/* 増レス                                               */ \
X_TABLE_ITEM(ID_OnClickMenuCOL_MOMENTUM)	/* 勢い	                                                */ \
X_TABLE_ITEM(ID_OnClickMenuCOL_LASTUP)		/* 最終取得                                             */ \
X_TABLE_ITEM(ID_OnClickMenuCOL_SINCE)		/* SINCE                                                */ \
X_TABLE_ITEM(ID_OnClickMenuCOL_OID)		/* 固有番号                                             */ \
X_TABLE_ITEM(ID_OnClickMenuCOL_BOARDNAME)	/* 板	                                                */ \
X_TABLE_ITEM(ID_OnCancelSetting)		/* 設定画面をキャンセル	                                */ \
X_TABLE_ITEM(ID_OnOkSetting)			/* 設定画面をOkで終了	                                */ \
X_TABLE_ITEM(ID_SettingPanelTree)		/* 設定画面の描画を切り替える                           */ \
X_TABLE_ITEM(ID_SettingPanelUpdate)		/* 設定画面の更新をメインスレッドに伝える	        */ \
X_TABLE_ITEM(ID_ViewerSettingNote)              /* ビューア設定画面                                     */ \
X_TABLE_ITEM(ID_NetworkPanelUpdate)		/* ネットワーク設定画面の更新をメインスレッドに伝える   */ \
X_TABLE_ITEM(ID_NetworkPanel)			/* ネットワーク設定用画面		                */ \
X_TABLE_ITEM(ID_NetworkPanelUseProxy)		/* プロキシを使用するかどうか		                */ \
X_TABLE_ITEM(ID_NetworkPanelUseProxyCache)	/* プロキシでキャッシュを使用するかどうか               */ \
X_TABLE_ITEM(ID_NetworkPanelBasicAuthUserName)	/* ベーシック認証のユーザー名		                */ \
X_TABLE_ITEM(ID_NetworkPanelBasicAuthPassword)	/* ベーシック認証のパスワード		                */ \
X_TABLE_ITEM(ID_NetworkPanelProxyReceiveAddr)	/* Proxy受信用アドレス			                */ \
X_TABLE_ITEM(ID_NetworkPanelProxyReceivePort)	/* Proxy受信用ポート			                */ \
X_TABLE_ITEM(ID_NetworkPanelProxySendAddr)	/* Proxy送信用アドレス			                */ \
X_TABLE_ITEM(ID_NetworkPanelProxySendPort)	/* Proxy送信用ポート			                */ \
X_TABLE_ITEM(ID_NetworkPanelProxySSLAuthAddr)	/* Proxy SSL認証用アドレス		                */ \
X_TABLE_ITEM(ID_NetworkPanelProxySSLAuthPort)	/* Proxy SSL認証用ポート		                        */ \
X_TABLE_ITEM(ID_NetworkPanelBoardListURL)       /* ボード一覧取得URL                                      */ \
X_TABLE_ITEM(ID_NetworkPanelReceiveBufferSize)  /* 受信バッファサイズ                                      */ \
X_TABLE_ITEM(ID_NetworkPanelMaxConn)            /* 最大接続数                                             */ \
X_TABLE_ITEM(ID_NetworkPanelUserAgent)          /* 2chにアクセスする時のユーザーエージェント                  */ \
X_TABLE_ITEM(ID_PathSettingPanel) 		/* 各種パス設定用画面					*/ \
X_TABLE_ITEM(ID_BrowserCheck)			/* 使用するブラウザを指定するかどうか			*/ \
X_TABLE_ITEM(ID_BrowserPathTC)			/* ブラウザのパスをもつテキストコントロール		        */ \
X_TABLE_ITEM(ID_DirPickerBrowser)		/* ブラウザのパスを参照する				*/ \
X_TABLE_ITEM(ID_BoardListTC)			/* 板一覧リストを保持する場所をもつテキストコントロール        */ \
X_TABLE_ITEM(ID_DirPickerBoardList)		/* 板一覧リストのパスを参照する				*/ \
X_TABLE_ITEM(ID_SkinPathTC)			/* スキン用のファイルのパスをもつテキストコントロール	        */ \
X_TABLE_ITEM(ID_DirPickerSkin)			/* スキン用ファイルのパスを参照する			        */ \
X_TABLE_ITEM(ID_ClearBrowserPath)		/* ブラウザパス設定値を削除				*/ \
X_TABLE_ITEM(ID_ClearBoardListPath)		/* 板一覧リストのパス設定値を削除			        */ \
X_TABLE_ITEM(ID_ClearSkinPath)			/* スキンのパス設定値を削除				*/ \
X_TABLE_ITEM(ID_BehaviorPanel)			/* 動作設定用パネル					*/ \
X_TABLE_ITEM(ID_OpenBNewTabCheck)		/* 新しいタブで板を開く					*/ \
X_TABLE_ITEM(ID_OnlyOneCategoryCheck)		/* カテゴリを一つしか開かない				*/ \
X_TABLE_ITEM(ID_OpenTNewTabCheck)		/* 新しいタブでスレを開く				        */ \
X_TABLE_ITEM(ID_ThreadNewTabCombo)		/* 新しいタブでスレを開く				        */ \
X_TABLE_ITEM(ID_LastReadThreadPosCheck)		/* 最後に見たスレの位置にジャンプ			        */ \
X_TABLE_ITEM(ID_ThreadJumpSetting)		/* 最後に見たスレの位置にジャンプ			        */ \
X_TABLE_ITEM(ID_RedrawNewThreadCheck)		/* 新着チェック時に既得レスを描き直す			*/ \
X_TABLE_ITEM(ID_ShowResponseCombo)		/* 既得スレの表示レス数					*/ \
X_TABLE_ITEM(ID_FavoriteNewTabCheck)		/* お気に入りを新しいタブで開く				*/ \
X_TABLE_ITEM(ID_FavoriteOnlyOneFolder)		/* お気に入りフォルダを一つしか開かない			*/ \
X_TABLE_ITEM(ID_ColorFontSettingPanel) 		/* 色・フォント設定用画面	              */ \
X_TABLE_ITEM(ID_TreeSampleLabel)		/* ツリーサンプル		              */ \
X_TABLE_ITEM(ID_ThreadListSampleLabel)		/* スレ欄サンプル		              */ \
X_TABLE_ITEM(ID_ExtractSampleLabel)		/* 抽出サンプル			      */ \
X_TABLE_ITEM(ID_LogWindowSampleLabel)		/* ログ出力画面サンプル		      */ \
X_TABLE_ITEM(ID_ThreadTitleSampleLabel)		/* スレッドタイトルサンプル	      */ \
X_TABLE_ITEM(ID_ThreadViewSampleLabel)		/* スレビューサンプル		      */ \
X_TABLE_ITEM(ID_KakikoSampleLabel)		/* 書き込みサンプル		      */ \
X_TABLE_ITEM(ID_MemoSampleLabel)		/* メモ欄サンプル		              */ \
X_TABLE_ITEM(ID_HintSampleLabel)		/* ヒントサンプル		              */ \
X_TABLE_ITEM(ID_LinkSampleLabel)		/* リンクサンプル		              */ \
X_TABLE_ITEM(ID_OthersSampleLabel)		/* その他サンプル		              */ \
X_TABLE_ITEM(ID_AllSampleLabel)			/* ツリーフォント設定ボタン	      */ \
X_TABLE_ITEM(ID_TreeFontButton)			/* スレ欄フォント設定ボタン	      */ \
X_TABLE_ITEM(ID_ThreadListFontButton)		/* 抽出フォント設定ボタン	              */ \
X_TABLE_ITEM(ID_ExtractFontButton)		/* ログ出力画面フォント設定ボタン         */ \
X_TABLE_ITEM(ID_LogWindowFontButton)		/* スレッドタイトルフォント設定ボタン      */ \
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
X_TABLE_ITEM(ID_TabControlSettingPanel) \
X_TABLE_ITEM(ID_TabControlSettingPanelOpenTabChoice) \
X_TABLE_ITEM(ID_TabControlSettingPanelOpenTabFromOtherTabChoice) \
X_TABLE_ITEM(ID_TabControlSettingPanelCloseThreadViewTabChoice) \
X_TABLE_ITEM(ID_TabControlSettingPanelCloseThreadTabChoice) \
X_TABLE_ITEM(ID_AddThreadFavorite)                /* お気に入りに追加(スレッド) 	   	*/ \
X_TABLE_ITEM(ID_DelThreadFavorite)		  /* お気に入りを削除(スレッド) 	   	*/ \
X_TABLE_ITEM(ID_AddBoardFavorite)		  /* お気に入りに追加(板)	 		*/ \
X_TABLE_ITEM(ID_DelBoardFavorite)		  /* お気に入りを削除(板)	 		*/ \
X_TABLE_ITEM(ID_AddAllThreadFavorite)             /* すべてのタブをお気に入りに追加	   	*/ \
X_TABLE_ITEM(ID_ResponseWindowSageChk)            /* レス用ウィンドウのsageチェック	        */ \
X_TABLE_ITEM(ID_ResponseWindowBeChk)              /* レス用ウィンドウのBEチェック	   	*/ \
X_TABLE_ITEM(ID_ChangeUserLastAttached)           /* ユーザーが最後に触ったウィジェットを記録   */ \
X_TABLE_ITEM(ID_ReloadThreadByName)               /* スレッド名を元にスレッドを更新する         */ \
X_TABLE_ITEM(ID_NewBoardAddDialog)                /* 外部板登録ダイアログ                       */ \
X_TABLE_ITEM(ID_CallNewBoardAddDialog)            /* 外部板登録用ダイアログを呼び出す           */ \
X_TABLE_ITEM(ID_OperationPanel)                   /* 各種操作設定用画面                         */ \
X_TABLE_ITEM(ID_KakikomiPanel)                    /* 書き込み設定用画面                         */ \
X_TABLE_ITEM(ID_DoePanel)                         /* スレ表示欄の設定画面                       */ \
X_TABLE_ITEM(ID_OtherSettingPanelOne)             /* その他の設定画面１                         */ \
X_TABLE_ITEM(ID_ResetBoardListTree)               /* 板一覧ツリーのウィジェットの初期化イベント */ \
X_TABLE_ITEM(ID_Receive_Timeout_Sec)              /* 受信タイムアウト秒			        */ \
X_TABLE_ITEM(ID_Connection_Timeout_Sec)		  /* 接続タイムアウト秒	                        */ \
X_TABLE_ITEM(ID_JaneCloneEnumsEnd)

// 列挙型の実体をマクロで生成する
enum JANECLONE_ENUMS {
     ID_Quit = 1,
     #define X(Enum, String) Enum,
     X_TABLE
     #undef X
     ID_UserLastClosedThreadClick = 1000,  // ユーザーが最後に閉じたスレッドの情報をクリックした(動的なIDのため1000~)
     ID_UserLastClosedBoardClick  = 1100,  // ユーザーが最後に閉じた板の情報をクリックした(動的なIDのため1100~)
     ID_UserFavoriteThreadClick   = 1200,  // ユーザーがお気に入りに登録したスレッドの情報をクリックした(動的なIDのため1200~)
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

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

// 列挙型の宣言
enum JANECLONE_ENUMS
{
        // janeclone.hpp
        ID_Quit = 1,                    // 終了
	ID_Restart,      		// 再起動
	ID_Test,                        // テスト用ID
	ID_Logging,                     // ログ出力制御用
	ID_WindowMinimize,              // ウィンドウを最小化する
	ID_GetBoardList,        	// 板一覧情報取得
	ID_CheckLogDirectory,           // 保存されているログをスレッド一覧に表示する
	ID_GetVersionInfo,              // バージョン情報
	ID_ThreadNoteBook,		// スレッド一覧ノートブックに使うID
	ID_BoardNoteBook,       	// 板一覧用ノートブックに使うID
	ID_ThumbnailNoteBook,           // 画像ビューア用ID
	ID_BoardTreectrl,               // 板一覧ツリー用ID
	ID_ShingetsuBoardTreectrl,      // 新月公開ノード一覧ツリー用ID
	ID_BoardTreeNoteBook,           // 板一覧ツリーを載せるノートブックに使うID
	ID_OneBoardTabClose,		// スレッド一覧タブをひとつ閉じる
	ID_ExcepSelTabClose,		// 現在選択されていないスレッド一覧タブを閉じる
	ID_AllBoardTabClose,		// すべてのスレッド一覧タブを閉じる
	ID_AllLeftBoardTabClose,	// これより左のスレッド一覧タブをを閉じる
	ID_AllRightBoardTabClose,	// これより右のスレッド一覧タブを閉じる
	ID_OnOpenBoardByBrowser,	// スレッド一覧をブラウザで開く
	ID_ReloadOneBoard,      	// アクティブなスレッド一覧をひとつ更新する
	ID_CopyBURLToClipBoard,		// 板のURLをクリップボードにコピーする
	ID_CopyBTitleToClipBoard,	// 板のタイトルをクリップボードにコピーする
	ID_CopyBBothDataToClipBoard,    // 板のURLとタイトルをクリップボードにコピーする
	ID_CopyTURLToClipBoard,		// スレッドのURLをクリップボードにコピーする
	ID_CopyTTitleToClipBoard,	// スレッドのタイトルをクリップボードにコピーする
	ID_CopyTBothDataToClipBoard,    // スレッドのURLとタイトルをクリップボードにコピーする
	ID_CopyTContentsToClipBoard,    // スレッドの内容をクリップボードにコピーする
	ID_CopyTAllToClipBoard,         // スレッドの内容, URL, タイトルをクリップボードにコピーする
	ID_OneThreadTabClose,		// スレタブをひとつ閉じる
	ID_ExcepSelThreadTabClose,	// 現在選択されていないスレタブを閉じる
	ID_AllThreadTabClose,		// すべてのスレタブを閉じる
	ID_AllLeftThreadTabClose,	// これより左のスレタブをを閉じる
	ID_AllRightThreadTabClose,	// これより右のスレタブを閉じる
	ID_OnOpenThreadByBrowser,	// スレッドをブラウザで開く
	ID_SaveDatFile, 		// datファイルに名前を付けて保存
	ID_SaveDatFileToClipBoard,	// datをクリップボードにコピー
	ID_DeleteDatFile,		// このログを削除
	ID_ReloadThisThread,    	// スレッドの再読み込み
	ID_CallResponseWindow,          // 書き込み用のウィンドウを呼び出す
	ID_BoardListCtrl,               // 板一覧リスト自体を表すID
	ID_ShingetsuBoardListCtrl,      // 新月の板一覧リスト自体を表すID
	ID_FontDialogBoardTree,         // 板一覧ツリー部分のフォントの指定を行う
	ID_FontDialogLogWindow,         // ログ出力画面部分のフォントの指定を行う
	ID_FontDialogBoardNotebook,     // スレッド一覧部分のフォントの指定を行う
	ID_FontDialogThreadNotebook,    // スレッド画面部分のフォント設定を呼び出す
	ID_FontDialogThreadContents,    // スレッド内で使用するフォント設定を呼び出す
	ID_URLWindowButton,             // URL入力ウィンドウのボタンを表すID
	ID_UserLastClosedThreadMenuUp,  // ユーザーが最後に閉じたスレッドの情報を項目に補充する
	ID_UserLastClosedBoardMenuUp,   // ユーザーが最後に閉じた板の情報を項目に補充する
	ID_UserLookingTabsMenuUp,       // ユーザーが現在見ているスレタブ・板タブの情報を項目に補充する
	ID_UserLookingTabsControl,      // ユーザーが現在フォーカスしているウィンドウの操作を行う
	ID_UserLookingTabsMenuClick,    // ユーザーが現在見ているスレタブ・板タブの情報をメニューからクリックした
	ID_OnOpenJaneCloneOfficial,     // JaneClone公式サイトをブラウザで開く
	ID_OnOpen2chViewerOfficial,     // ●公式サイトをブラウザで開く
	ID_ThreadSearchBar,             // スレッド検索ボックスのID
	ID_BoardSearchBar,              // 板名検索ボックスのID
	ID_ShingetsuBoardSearchBar,     // 新月の板名検索ボックスのID
	ID_ThreadSearchBarCombo,        // スレッド検索ボックスのコンボボックス
	ID_BoardSearchBarCombo,         // 板名検索ボックスのコンボボックス
	ID_SearchBoxDoSearch,           // 検索ボックスで検索ボタンを押す
	ID_SearchBoxUp,                 // 検索ボックスで上に移動
	ID_SearchBoxDown,               // 検索ボックスで下に移動
	ID_SearchBoxCopy,               // 検索ボックスでコピー実行
	ID_SearchBoxCut,                // 検索ボックスで切り取り実行
	ID_SearchBoxSelectAll,          // 検索ボックスで全て選択
	ID_SearchBoxClear,              // 検索ボックスをクリア
	ID_SearchBoxNormalSearch,       // 検索ボックスで通常検索
	ID_SearchBoxRegexSearch,        // 検索ボックスで正規表現検索
	ID_SearchBarHide,               // 検索ボックスを隠す
	ID_FloatToolBar,                // アイコン付きのツールバー自体のID
	ID_SwitchSeparateXY,            // 縦⇔横分割切り替え
	ID_SwitchTwoThreePane,          // ２⇔３ペイン切り替え
	ID_ShowBoardListTree,           // 板一覧ツリー表示切り替え
	ID_SwitchRightPane,             // 右側ペインの内容切り替え
	ID_CallSettingWindow,           // ビューア設定画面を呼び出す
	// responsewindow.hpp
	ID_ResponseWindow,
	ID_QuitResponseWindow,
	ID_PostResponse,
	ID_PostConfirmForm,
	// threadcontentbar.hpp
	ID_TCBAutoReload,
	ID_TCBRedResExtract,
	ID_TCBRefresh,
	ID_TCBScrollToNewRes,
	ID_TCBStop,
	ID_TCBResExtract,
	ID_TCBNewThread,
	ID_TCBBookMark,
	ID_TCBDeleteLog,
	ID_TCBClose,
	ID_TCBNormalSearch,
	ID_TCBHideSearchBar,
	ID_TCBForward,
	ID_TCBBackward,
#ifdef __WXMAC__
	ID_ThreadContentBarUpdate,      // スレッドバーの更新が起きたときに通知されるイベント
#endif
	// threadcontentwindow.hpp
	ID_CopyFromHtmlWindow,          // HtmlWindowで選択しているテキストをクリップボードにコピーする
	ID_CopyURLFromHtmlWindow,       // HtmlWindowで選択しているURLをクリップボードにコピーする
	ID_SelectAllTextHtmlWindow,     // HtmlWindowでテキストを全て選択する
	ID_SearchSelectWordByYahoo,     // 選択したテキストでヤフー検索
	ID_SearchSelectWordByGoogle,    // 選択したテキストでGoogle検索
	ID_SearchSelectWordByAmazon,    // 選択したテキストでAmazon検索
	ID_SearchThreadBySelectWord,    // 選択したテキストでスレタイ検索
	ID_CallResponseWindowAnchor,    // 書き込み用のウィンドウをアンカー付きで呼び出す
	ID_CallResponseWindowQuote,     // 書き込み用のウィンドウをレス引用付きで呼び出す
#ifdef DEBUG
	ID_HtmlSourceDebug,             // HTMLのデバッグ用命令
#endif
	// janecloneimageviewer.hpp
	ID_OneThumbnailTabClose,        // 画像タブをひとつ閉じる
	ID_AllThumbnailTabClose,        // すべての画像タブを閉じる
	ID_AllLeftThumbnailTabClose,	// これより左の画像タブをを閉じる
	ID_AllRightThumbnailTabClose,	// これより右の画像タブを閉じる
	ID_SelectLeftThumbnailTab,	// 左の画像タブに移動
	ID_SelectRightThumbnailTab,	// 右の画像タブに移動
	ID_OnOpenImageByBrowser,	// 画像をブラウザで開く
	ID_HideThumbnailTab,	        // 画像ビューアを隠す
	// スレッド一覧リストの新着状態を表す定数
	//ID_ThreadCheck,                 // 取得しているスレッドを表す 
	//ID_ThreadAdd,                   // スレッドにレスの増加が見られた状態
	//ID_ThreadDrop,                  // DAT落ちしたスレッドを表す
	//ID_ThreadNew,                   // 新規に取得したスレッドを表す
	// メニューからスレッド一覧リストをソートする
	ID_OnClickMenuCOL_CHK,          // 新着チェック
	ID_OnClickMenuCOL_NUM,          // 番号	       
	ID_OnClickMenuCOL_TITLE,	// タイトル    
	ID_OnClickMenuCOL_RESP,	        // レス	       
	ID_OnClickMenuCOL_CACHEDRES,    // 取得	   
	ID_OnClickMenuCOL_NEWRESP,	// 新着	       
	ID_OnClickMenuCOL_INCRESP,	// 増レス      
	ID_OnClickMenuCOL_MOMENTUM,	// 勢い	       
	ID_OnClickMenuCOL_LASTUP,	// 最終取得    
	ID_OnClickMenuCOL_SINCE,	// SINCE       
	ID_OnClickMenuCOL_OID,	        // 固有番号	   
	ID_OnClickMenuCOL_BOARDNAME,	// 板
	// settingwindow.hpp
	ID_OnCancelSetting,             // 設定画面をキャンセル
	ID_OnOkSetting,                 // 設定画面をOkで終了
	ID_SettingPanelTree,            // 設定画面の描画を切り替える

#ifdef __WXMAC__
	ID_SettingPanelUpdate,          // 設定画面の更新をメインスレッドに伝える
	ID_NetworkPanelUpdate,          // ネットワーク設定画面の更新をメインスレッドに伝える
#endif
	// otherpanels.hpp
        ID_NetworkPanel,                 // ネットワーク設定用画面
        ID_NetworkPanelUseProxy,         // プロキシを使用するかどうか
        ID_NetworkPanelUseProxyCache,    // プロキシでキャッシュを使用するかどうか
        ID_NetworkPanelBasicAuthUserName,// ベーシック認証のユーザー名
        ID_NetworkPanelBasicAuthPassword,// ベーシック認証のパスワード
        ID_NetworkPanelProxyReceiveAddr, // Proxy受信用アドレス
        ID_NetworkPanelProxyReceivePort, // Proxy受信用ポート
        ID_NetworkPanelProxySendAddr,    // Proxy送信用アドレス
        ID_NetworkPanelProxySendPort,    // Proxy送信用ポート
        ID_NetworkPanelProxySSLAuthAddr, // Proxy SSL認証用アドレス
        ID_NetworkPanelProxySSLAuthPort, // Proxy SSL認証用ポート
	ID_PathSettingPanel,             // 各種パス設定用画面
	ID_BrowserCheck,                 // 使用するブラウザを指定するかどうか 
	ID_BrowserPathTC,                // ブラウザのパスをもつテキストコントロール
	ID_DirPickerBrowser,             // ブラウザのパスを参照する
	ID_BoardListTC,                  // 板一覧リストを保持する場所をもつテキストコントロール
	ID_DirPickerBoardList,           // 板一覧リストのパスを参照する
	ID_SkinPathTC,                   // スキン用のファイルのパスをもつテキストコントロール
	ID_DirPickerSkin,                // スキン用ファイルのパスを参照する
        ID_ClearBrowserPath,             // ブラウザパス設定値を削除 
        ID_ClearBoardListPath,		 // 板一覧リストのパス設定値を削除
        ID_ClearSkinPath,		 // スキンのパス設定値を削除
	ID_BehaviorPanel,                // 動作設定用パネル
	ID_OpenBNewTabCheck,             // 新しいタブで板を開く
	ID_OnlyOneCategoryCheck,         // カテゴリを一つしか開かない
	ID_OpenTNewTabCheck,             // 新しいタブでスレを開く
	ID_ThreadNewTabCombo,            // 新しいタブでスレを開く
	ID_LastReadThreadPosCheck,       // 最後に見たスレの位置にジャンプ
	ID_ThreadJumpSetting,            // 最後に見たスレの位置にジャンプ
	ID_RedrawNewThreadCheck,         // 新着チェック時に既得レスを描き直す
	ID_ShowResponseCombo,            // 既得スレの表示レス数
	ID_FavoriteNewTabCheck,          // お気に入りを新しいタブで開く
	ID_FavoriteOnlyOneFolder,        // お気に入りフォルダを一つしか開かない
	ID_ColorFontSettingPanel,        // 色・フォント設定用画面
        ID_TreeSampleLabel,              // ツリーサンプル
        ID_ThreadListSampleLabel,        // スレ欄サンプル
        ID_ExtractSampleLabel,           // 抽出サンプル
        ID_LogWindowSampleLabel,	 // ログ出力画面サンプル
        ID_ThreadTitleSampleLabel,	 // スレッドタイトルサンプル
        ID_ThreadViewSampleLabel,	 // スレビューサンプル
	ID_KakikoSampleLabel,		 // 書き込みサンプル
	ID_MemoSampleLabel,		 // メモ欄サンプル
	ID_HintSampleLabel,		 // ヒントサンプル
	ID_LinkSampleLabel,		 // リンクサンプル
	ID_OthersSampleLabel,		 // その他サンプル
	ID_TreeFontButton,		 // ツリーフォント設定ボタン
	ID_ThreadListFontButton,	 // スレ欄フォント設定ボタン
	ID_ExtractFontButton,		 // 抽出フォント設定ボタン
	ID_LogWindowFontButton,		 // ログ出力画面フォント設定ボタン
	ID_ThreadTitleFontButton,	 // スレッドタイトルフォント設定ボタン
	ID_KakikoFontButton,		 // 書き込みフォント設定ボタン
	ID_MemoFontButton,		 // メモ欄フォント設定ボタン
	ID_HintFontButton,		 // ヒントフォント設定ボタン
	ID_LinkFontButton,		 // リンクフォント設定ボタン
	ID_OthersFontButton,		 // その他フォント設定ボタン
	ID_AllFontButton,		 // すべてフォント設定ボタン
	ID_ThreadBGColorButton,		 // スレ欄背景色設定ボタン
	ID_ThreadListBGColorButton,	 // 抽出背景色設定ボタン   
	ID_LogWindowBGColorButton,	 // ログ出力画面背景色設定ボタン     
	ID_ThreadTitleBGColorButton,	 // スレッドタイトル背景色設定ボタン
	ID_ThreadViewBGColorButton,	 // スレビュー背景色設定ボタン
	ID_MemoBGColorButton,		 // メモ欄背景色設定ボタン	     
	ID_HintBGColorButton,		 // ヒント背景色設定ボタン   
	ID_PopupBGColorButton,		 // ポップアップ背景色設定ボタン   
	ID_AllBGColorButton,		 // すべての背景色設定ボタン	    

	ID_UserLastClosedThreadClick = 1000,  // ユーザーが最後に閉じたスレッドの情報をクリックした(動的なIDのため1000~)
	ID_UserLastClosedBoardClick  = 1100,  // ユーザーが最後に閉じた板の情報をクリックした(動的なIDのため1100~)
	ID_ThreadContentBar          = 1500   // ThreadContentBar自体のID(動的なIDのため1500~)
};

// 列挙型を文字列化
// Begin_Enum_String( JANECLONE_ENUMS )
// {
//     Enum_String( Sunday );
//     Enum_String( Saturday );
// }
// End_Enum_String;

#endif /* ENUMS_HPP_ */

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

// enum
enum {
        // janeclone.hpp
        ID_Quit = 1,                    // 終了
	ID_Restart,      		// 再起動
	ID_WindowMinimize,              // ウィンドウを最小化する
	ID_GetBoardList,        	// 板一覧情報取得
	ID_CheckLogDirectory,           // 保存されているログをスレッド一覧に表示する
	ID_GetVersionInfo,              // バージョン情報
	ID_ThreadNoteBook,		// スレッド一覧ノートブックに使うID
	ID_BoardNoteBook,       	// 板一覧用ノートブックに使うID
	ID_ThumbnailNoteBook,           // 画像ビューア用ID
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
	ID_UserLastClosedThreadClick,   // ユーザーが最後に閉じたスレッドの情報をクリックした
	ID_UserLastClosedBoardClick,    // ユーザーが最後に閉じた板の情報をクリックした
	ID_UserLookingTabsMenuClick,    // ユーザーが現在見ているスレタブ・板タブの情報をメニューからクリックした
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
	// threadcontentwindow.hpp
	ID_CopyFromHtmlWindow,          // HtmlWindowで選択しているテキストをクリップボードにコピーする
	ID_CopyURLFromHtmlWindow,       // HtmlWindowで選択しているURLをクリップボードにコピーする
	ID_SelectAllTextHtmlWindow,     // HtmlWindowでテキストを全て選択する
	ID_SearchSelectWordByYahoo,     // 選択したテキストでヤフー検索
	ID_SearchSelectWordByGoogle,    // 選択したテキストでGoogle検索
	ID_SearchSelectWordByAmazon,    // 選択したテキストでAmazon検索
	ID_SearchThreadBySelectWord     // 選択したテキストでスレタイ検索
};

#endif /* ENUMS_HPP_ */

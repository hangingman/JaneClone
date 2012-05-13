/*
 * SocketCommunication.h
 *
 *  Created on: 2012/01/04
 *      Author: learning
 */

#ifndef SOCKETCOMMUNICATION_H_
#define SOCKETCOMMUNICATION_H_

#include "JaneCloneUtil.h"
#include <wx/wx.h>
#include <wx/filefn.h>
#include <wx/textfile.h>
#include <iostream>
#include <fstream>
#include "gimite/socket.h"

class SocketCommunication {

public:
	/**
	 * 板一覧ファイルをダウンロードしてくるメソッド 引数はサーバーのフルURL、サーバ名、板名、保存先
	 * もし前回通信した際のログが残っていれば更新の確認のみ行う
	 */
	int DownloadBoardList(const wxString outputPath, const wxString headerPath);
	/**
	 * スレッド一覧をダウンロードしてくるメソッド
	 * @param 板名,URL,サーバー名
	 * @return datファイル保存先
	 */
	wxString DownloadThreadList(const wxString& boardName,
			const wxString& boardURL, const wxString& boardNameAscii);
private:
	/**
	 * 前回の通信ログが存在すれば、最後に取得した日時を変数に格納する
	 */
	wxString CheckLastModifiedTime(const wxString headerPath);
	/**
	 * 新規に板一覧情報を取得しに行く
	 */
	int DownloadBoardListNew(const wxString outputPath,
			const wxString headerPath);
	/**
	 * 前回との差分を取得しに行く
	 */
	int DownloadBoardListMod(const wxString outputPath,
			const wxString headerPath);
	/**
	 * 新規にスレッド一覧をダウンロードしてくるメソッド
	 * @param 板名,URL,サーバー名
	 * @return 実行コード
	 */
	int DownloadThreadListNew(const wxString& gzipPath, const wxString& headerPath);
	/**
	 * 前回との差分のスレッド一覧をダウンロードしてくるメソッド
	 * @param 板名,URL,サーバー名
	 * @return 実行コード
	 */
	int DownloadThreadListMod(const wxString& gzipPath, const wxString& headerPath);
	/**
	 * 一時ファイルを消す
	 */
	void RemoveTmpFile(const wxString removeFile);
};

#endif /* SOCKETCOMMUNICATION_H_ */

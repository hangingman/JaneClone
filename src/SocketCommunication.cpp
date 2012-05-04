/*
 * SocketCommunication.cpp
 *
 *  Created on: 2012/01/04
 *      Author: learning
 */

#include "SocketCommunication.h"
/**
 * 板一覧ファイルをダウンロードしてくるメソッド 引数はサーバーのフルURL、サーバ名、板名、保存先
 */
int SocketCommunication::DownloadBoardList(const wxString outputPath,
		const wxString headerPath) {

	// 実行コード
	int rc = 0;
	// 拡張子をgzipに変える
	wxString gzipPath = outputPath;
	gzipPath.Replace(".html", ".gzip");
	// 一時保存用ファイルのパスを設定する
	wxString tmpPath = wxT("./dat/tmp.html");

	// 解凍された板一覧情報が存在しないor前回の通信ログが残っていないならば通常通りソケット通信を行う
	if ((!wxFileExists(outputPath)) || (!wxFileExists(headerPath))) {
		rc = DownloadBoardListNew(gzipPath, headerPath);
		// そうでなければ前回の通信の差分を取得しに行く
	} else {
		rc = DownloadBoardListMod(gzipPath, headerPath);
	}

	// gzip拡張子のファイルがあれば、ファイルの解凍・UTF化を行う
	if (wxFile::Exists(gzipPath)) {
		JaneCloneUtil::DecommpressFile(gzipPath, tmpPath);
		JaneCloneUtil::ConvertSJISToUTF8(tmpPath, (wxString&) outputPath);
	}
	// 更新が終わったらgzipファイルとSJISファイルを消しておく
	RemoveTmpFile(gzipPath);
	RemoveTmpFile(tmpPath);

	return rc;
}

/**
 * 新規に板一覧情報を取得しに行く
 */
int SocketCommunication::DownloadBoardListNew(const wxString outputPath,
		const wxString headerPath) {
	// 実行コード
	int rc = 0;
	// gimiteによる通信開始
	gimite::startup_socket();
	{
		// menu.2ch.netのポート80(HTTP)に接続。
		gimite::socket_stream sst("menu.2ch.net", 80);
		//エラーチェック。
		if (!sst) {
			std::cerr << "Failed to connect." << std::endl;
		} else {
			//文字列を送信。
			sst << "GET /bbsmenu.html HTTP/1.1 \r\n";
			sst << "Accept-Encoding: gzip \r\n";
			sst << "Host: menu.2ch.net \r\n";
			sst << "Accept: */* \r\n";
			sst << "Referer: http://menu.2ch.net/ \r\n";
			sst << "Accept-Language: ja \r\n";
			sst << "User-Agent: Monazilla/1.00 (monaweb/1.00) \r\n";
			sst << "Connection: close \r\n";
			sst << "\r\n";

			// 受信用文字列
			std::string s;
			// ヘッダを取り除くためのフラグ
			bool flag = false;
			// gzipのヘッダ作成
			char HEX[] = { 0x1f, 0x8b, 0x08, 0x00 };

			// ファイルに書き出す(バイナリ部分)
			wxCharBuffer binaryFile = outputPath.ToUTF8();
			std::ofstream ofs(binaryFile.data(),
					std::ios::binary | std::ios::trunc);
			// ファイルに書き出す(ヘッダ部分)
			wxCharBuffer headerFile = headerPath.ToUTF8();
			std::ofstream ofsh(headerFile.data(), std::ios::trunc);

			// 1行ずつ受信してバイナリ形式で書きこむ
			while (std::getline(sst, s)) {
				if (flag) {
					// 真の場合
					ofs << s << std::endl;
				} else {
					// 偽の場合
					unsigned int loc = s.find(*HEX, 0);
					if (loc != std::string::npos) {
						ofs << s << std::endl;
						flag = true;
					} else {
						// ヘッダーを書きだす（新規）
						ofsh << s;
					}
				}
			}
		}
		// gimiteによる通信終了
		gimite::cleanup_socket();

		return rc;
	}
}
/**
 * 前回との差分を取得しに行く
 */
int SocketCommunication::DownloadBoardListMod(const wxString outputPath,
		const wxString headerPath) {
	// 実行コード
	int rc = 0;
	// 最終更新日時をヘッダ情報から取得する
	wxString lastModifiedTime = CheckLastModifiedTime(headerPath);
	// バイナリとヘッダは前回取得した名前と同じでは困るのでtmpファイルとして作成しておく
	wxString tmpOutputPath = outputPath;
	tmpOutputPath += wxT(".tmp");
	wxString tmpHeaderPath = headerPath;
	tmpHeaderPath += wxT(".tmp");

	// 304かどうか判断するためのフラグ
	bool mod_flag = false;
	// ヘッダを取り除くためのフラグ
	bool flag = false;
	// gzipのヘッダ作成
	char HEX[] = { 0x1f, 0x8b, 0x08, 0x00 };

	// gimiteによる通信開始
	gimite::startup_socket();
	{
		// menu.2ch.netのポート80(HTTP)に接続。
		gimite::socket_stream sst("menu.2ch.net", 80);
		//エラーチェック。
		if (!sst) {
			std::cerr << "Failed to connect." << std::endl;
		} else {
			//文字列を送信。
			sst << "GET /bbsmenu.html HTTP/1.1 \r\n";
			sst << "Accept-Encoding: gzip \r\n";
			sst << "Host: menu.2ch.net \r\n";
			sst << "If-Modified-Since:" << lastModifiedTime.mb_str() << "\r\n";
			sst << "Accept: */* \r\n";
			sst << "Referer: http://menu.2ch.net/ \r\n";
			sst << "Accept-Language: ja \r\n";
			sst << "User-Agent: Monazilla/1.00 (monaweb/1.00) \r\n";
			sst << "Connection: close \r\n";
			sst << "\r\n";

			// 受信用文字列
			std::string s;

			// ファイルに書き出す(バイナリ部分)
			wxCharBuffer binaryFile = tmpOutputPath.ToUTF8();
			std::ofstream ofs(binaryFile.data(),
					std::ios::binary | std::ios::trunc);
			// ファイルに書き出す(ヘッダ部分)
			wxCharBuffer headerFile = tmpHeaderPath.ToUTF8();
			std::ofstream ofsh(headerFile.data(), std::ios::trunc);

			/** 1行目に「HTTP/1.1 304 Not Modified」が含まれない場合のみ更新を行う */
			while (std::getline(sst, s)) {

				/** 更新フラグ立たず・ヘッダ部分 */
				if (!mod_flag && !flag) {
					unsigned int loc = s.find("304 Not Modified");
					// １行目の読み出しで304があればbreak
					if (loc != std::string::npos) {
						ofs.close();
						ofsh.close();
						// tmpファイルは消しておく
						RemoveTmpFile(tmpOutputPath);
						RemoveTmpFile(tmpHeaderPath);
						break;
						// そうでなければファイルストリームの準備をする
					} else {
						// 更新フラグを立てる
						mod_flag = true;
						// ヘッダーを書きだす（更新）
						ofsh << s;
					}

					/** 更新フラグ立つ・ヘッダ部分 */
				} else if (mod_flag && !flag) {
					unsigned int loc = s.find(*HEX, 0);
					if (loc != std::string::npos) {
						ofs << s << std::endl;
						flag = true;
					} else {
						// ヘッダーを書きだす（更新）
						ofsh << s;
					}
					/** 更新フラグ立つ・ボディ部分 */
				} else if (mod_flag && flag) {
					ofs << s << std::endl;
				}
			}
		}
		// gimiteによる通信終了
		gimite::cleanup_socket();

		// 更新が最後までうまく行った場合以前のファイルを削除する
		if (mod_flag && flag) {
			RemoveTmpFile(outputPath);
			RemoveTmpFile(headerPath);
			// tmpファイルを本物のファイルとする
			wxRenameFile(tmpOutputPath, outputPath);
			wxRenameFile(tmpHeaderPath, headerPath);
		}
	}

	return rc;
}

/**
 * スレッド一覧をダウンロードしてくるメソッド 　引数はサーバーのフルURL、サーバ名、板名、保存先
 */
int SocketCommunication::DownloadThreadList(const wxString& boardURL,
		const wxString& server, const wxString& boardName,
		const wxString outputPath, const wxString headerPath) {

	int rc = 0;
	// wxからstdへの変換
	std::string boardURLSTL = std::string(boardURL.mb_str());
	std::string serverSTL = std::string(server.mb_str());
	std::string boardNameSTL = std::string(boardName.mb_str());

	gimite::startup_socket();
	{
		//ikura.2ch.netのポート80(HTTP)に接続。
		gimite::socket_stream sst(serverSTL, 80);
		//エラーチェック。
		if (!sst) {
			std::cerr << "Failed to connect." << std::endl;
		} else {
			//文字列を送信。
			sst << "GET /" + boardNameSTL + "/subject.txt HTTP/1.1 \r\n";
			sst << "Accept-Encoding: gzip \r\n";
			sst << "Host: " + serverSTL + " \r\n";
			sst << "Accept: */*\r\n";
			sst << "Referer: " + boardURLSTL + " \r\n";
			sst << "Accept-Language: ja \r\n";
			sst << "User-Agent: Mozilla/5.0 \r\n";
			sst << "Connection: close \r\n";
			sst << "\r\n";

			// 受信用文字列
			std::string s;
			// ヘッダを取り除くためのフラグ
			bool flag = false;
			// gzipのヘッダ作成
			char HEX[] = { 0x1f, 0x8b, 0x08, 0x00 };

			// ファイルに書き出す(バイナリ部分)
			wxCharBuffer binaryFile = outputPath.ToUTF8();
			std::ofstream ofs(binaryFile.data(),
					std::ios::binary | std::ios::trunc);
			// ファイルに書き出す(ヘッダ部分)
			wxCharBuffer headerFile = headerPath.ToUTF8();
			std::ofstream ofsh(headerFile.data(), std::ios::trunc);
			// 1行ずつ受信してバイナリ形式で書きこむ
			// このへんの処理はWin, Linux共通ではないだろうから同じソースコードで違う挙動になるかもしれない
			while (std::getline(sst, s)) {
				if (flag) {
					// 真の場合
					ofs << s << std::endl;
				} else {
					// 偽の場合
					unsigned int loc = s.find(*HEX, 0);
					if (loc != std::string::npos) {
						ofs << s << std::endl;
						flag = true;
					} else {
						// ヘッダーを書きだす
						ofsh << s << std::endl;
					}
				}
			}
		}
		//最後に1回だけ呼び出してください。
		gimite::cleanup_socket();
		return rc;
	}
}

/**
 * 前回の通信ログが存在すれば、最後に取得した日時を変数に格納する
 */
wxString SocketCommunication::CheckLastModifiedTime(const wxString headerPath) {
	wxString lastGetTime = "";

	// ログを読み込む
	wxTextFile file(headerPath);
	file.Open(wxConvAuto(wxFONTENCODING_UTF8));
	wxString line;

	if (file.IsOpened()) {
		// ログの中身を1行ずつ走査
		for (line = file.GetFirstLine(); !file.Eof();
				line = file.GetNextLine()) {
			if (line.Contains(wxT("Last-Modified:"))) {
				lastGetTime = line.Mid(14);
				break;
			}
		}
		// ファイルのクローズを行う
		file.Close();
		return lastGetTime;

	} else {
		std::cout << "ファイルのオープンに失敗しました" << std::endl;
	}
}

/**
 * 一時ファイルを消す
 */
void SocketCommunication::RemoveTmpFile(const wxString removeFile) {
	if (wxFile::Exists(removeFile)) {
		wxRemoveFile(removeFile);
	}
}


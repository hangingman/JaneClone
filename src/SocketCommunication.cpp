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

#include "SocketCommunication.h"
/**
 * 板一覧ファイルをダウンロードしてくるメソッド 引数は板一覧ファイル保存先、板一覧ファイルヘッダ保存先
 */
int SocketCommunication::DownloadBoardList(const wxString outputPath,
		const wxString headerPath) {

	// 実行コード
	int rc = 0;
	// 拡張子をgzipに変える
	wxString gzipPath = outputPath;
	gzipPath.Replace(wxT(".html"), wxT(".gzip"));
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
 * 前回の通信ログが存在すれば、最後に取得した日時を変数に格納する
 */
wxString SocketCommunication::CheckLastModifiedTime(const wxString headerPath) {
	wxString lastGetTime = wxT("");

	// ログを読み込む
	wxTextFile file(headerPath);
	//file.Open(wxConvAuto(wxFONTENCODING_UTF8)); <-- fix me ! wx2.9からの新機能なので他の実装方法を考える
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
 * スレッド一覧をダウンロードしてくるメソッド
 * @param 板名,URL,サーバー名
 * @return datファイル保存先
 */
wxString SocketCommunication::DownloadThreadList(const wxString & boardName,
		const wxString & boardURL, const wxString & boardNameAscii) {

	// 実行コード
	int rc = 0;
	// 出力するファイルの名前
	wxString outputFileName = boardNameAscii;
	outputFileName+=wxT(".dat");
	// 出力先のファイルパスを設定する
	wxString outputFilePath = wxT("./dat/");
	outputFilePath+=boardNameAscii;
	outputFilePath+=wxT("/");

	// 保存用フォルダ存在するか確認。無ければフォルダを作成
	if (!wxFile::Exists(outputFilePath)) {
		::wxMkdir(outputFilePath);
	}
	outputFilePath+=outputFileName;
	// gzip用のパスを設定する
	wxString gzipPath = outputFilePath;
	gzipPath.Replace(wxT(".dat"), wxT(".gzip"));
	// ヘッダーのパスを設定する
	wxString headerPath = outputFilePath;
	headerPath.Replace(wxT(".dat"), wxT(".header"));

	// 解凍された板一覧情報が存在しないor前回の通信ログが残っていないならば通常通りソケット通信を行う
	if ((!wxFileExists(outputFilePath)) || (!wxFileExists(headerPath))) {
		rc = DownloadThreadListNew((const wxString)gzipPath, (const wxString)headerPath);
		// そうでなければ前回の通信の差分を取得しに行く
	} else {
		rc = DownloadThreadListNew((const wxString)gzipPath, (const wxString)headerPath);
	}

	// gzip拡張子のファイルがあれば、ファイルの解凍を行う
	if (wxFile::Exists(gzipPath)) {
		JaneCloneUtil::DecommpressFile(gzipPath, outputFilePath);
	}
	// 更新が終わったらgzipファイルを消しておく
	RemoveTmpFile(gzipPath);

	return outputFileName;
}

/**
 * 新規にスレッド一覧をダウンロードしてくるメソッド
 * @param 板名,URL,サーバー名
 * @return 実行コード
 */
int SocketCommunication::DownloadThreadListNew(const wxString& gzipPath, const wxString& headerPath) {
	int rc = 0;

	return rc;
}

/**
 * 前回との差分のスレッド一覧をダウンロードしてくるメソッド
 * @param 板名,URL,サーバー名
 * @return 実行コード
 */
int SocketCommunication::DownloadThreadListMod(const wxString& gzipPath, const wxString& headerPath) {
	int rc = 0;

	return rc;
}

/**
 * 一時ファイルを消す
 */
void SocketCommunication::RemoveTmpFile(const wxString removeFile) {
	if (wxFile::Exists(removeFile)) {
		wxRemoveFile(removeFile);
	}
}

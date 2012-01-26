/*
 * SocketCommunication.cpp
 *
 *  Created on: 2012/01/04
 *      Author: learning
 */

#include "SocketCommunication.h"

// スレッド一覧をダウンロードしてくるメソッド 　引数はサーバーのフルURL、サーバ名、板名、保存先
// うまくいけばtrueを返す
bool SocketCommunication::DownloadThreadList(wxString& boardURL,
		wxString& server, wxString& boardName, wxString outputPath, wxString headerPath) {
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
			char HEX[] = {0x1f,0x8b,0x08,0x00};

			// ファイルに書き出す(バイナリ部分)
			wxCharBuffer binaryFile = outputPath.ToUTF8();
			std::ofstream ofs(binaryFile.data(), std::ios::binary | std::ios::trunc);
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
	return true;
	}
}

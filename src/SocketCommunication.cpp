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

	wxHTTP http;
	http.SetHeader(_T("Accept-Encoding"), _T("gzip"));
	http.SetHeader(_T("Host"), _T("menu.2ch.net"));
	http.SetHeader(_T("Accept"), _T(""));
	http.SetHeader(_T("Referer"), _T("http://menu.2ch.net/"));
	http.SetHeader(_T("Accept-Language"), _T("ja"));
	http.SetHeader(_T("User-Agent"), _T("Mozilla/5.0"));
	http.SetTimeout(5);

	wxString server = wxT("menu.2ch.net");
	wxString path = wxT("/bbsmenu.html");
	wxString msg = wxT("");

	// 保存先を決める
	wxFileOutputStream output(outputPath);
	wxDataOutputStream store(output);

	if (http.Connect(server, 80)) {
		wxInputStream *stream;
		stream = http.GetInputStream(path);

		if (stream == NULL) {
			output.Close();
			return -1;
		} else {
			unsigned char buffer[1024];
			int byteRead;

			// ヘッダを書きだす
			WriteHeaderFile(http, headerPath);

			// ストリームを受け取るループ部分
			while (!stream->Eof()) {
				stream->Read(buffer, sizeof(buffer));
				store.Write8(buffer, sizeof(buffer));
				byteRead = stream->LastRead();
				if (byteRead <= 0) {
					break;
				}
			}

			output.Close();
		}
	} else {
		output.Close();
		return -1;
	}

	return rc;
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

	wxHTTP http;
	http.SetHeader(_T("Accept-Encoding"), _T("gzip"));
	http.SetHeader(_T("Host"), _T("menu.2ch.net"));
	http.SetHeader(_T("If-Modified-Since"), lastModifiedTime);
	http.SetHeader(_T("Accept"), _T(""));
	http.SetHeader(_T("Referer"), _T("http://menu.2ch.net/"));
	http.SetHeader(_T("Accept-Language"), _T("ja"));
	http.SetHeader(_T("User-Agent"), _T("Mozilla/5.0"));
	http.SetTimeout(5);

	wxString server = wxT("menu.2ch.net");
	wxString path = wxT("/bbsmenu.html");
	wxString msg = wxT("");

	// 保存先を決める
	wxFileOutputStream output(tmpOutputPath);
	wxDataOutputStream store(output);

	if (http.Connect(server, 80)) {
		wxInputStream *stream;
		stream = http.GetInputStream(path);

		if (stream == NULL) {
			output.Close();
			return -1;
		} else if (304 == http.GetResponse()) {
			output.Close();
			// レスポンスコードが304ならば変更なしなので正常終了
			RemoveTmpFile(tmpOutputPath);
			return 0;
		} else {
			unsigned char buffer[1024];
			int byteRead;
			int totalRead;
			totalRead = 0;

			// ヘッダを書きだす
			WriteHeaderFile(http, (const wxString) tmpHeaderPath);

			// ストリームを受け取るループ部分
			while (!stream->Eof()) {
				stream->Read(buffer, sizeof(buffer));
				store.Write8(buffer, sizeof(buffer));
				byteRead = stream->LastRead();
				if (byteRead <= 0) {
					break;
				}
				totalRead += byteRead;
			}

			output.Close();

			// 最後までうまく行った場合
			RemoveTmpFile(outputPath);
			RemoveTmpFile(headerPath);
			// tmpファイルを本物のファイルとする
			wxRenameFile(tmpOutputPath, outputPath);
			wxRenameFile(tmpHeaderPath, headerPath);
		}
	} else {
		output.Close();
		return -1;
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
	file.Open(wxConvUTF8);
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
	}

	return lastGetTime = wxT("could't read lastgettime");
}

/**
 * スレッド一覧をダウンロードしてくるメソッド
 * @param 板名,URL,サーバー名
 * @return datファイル保存先
 */
wxString SocketCommunication::DownloadThreadList(const wxString boardName,
		const wxString boardURL, const wxString boardNameAscii) {

	// 出力するファイルの名前
	wxString outputFileName = boardNameAscii;
	outputFileName += wxT(".dat");
	// 出力先のファイルパスを設定する
	wxString outputFilePath = wxT("./dat/");
	outputFilePath += boardNameAscii;
	outputFilePath += wxT("/");

	// 保存用フォルダ存在するか確認。無ければフォルダを作成
	if (!wxFile::Exists(outputFilePath)) {
		::wxMkdir(outputFilePath);
	}
	outputFilePath += outputFileName;
	// gzip用のパスを設定する
	wxString gzipPath = outputFilePath;
	gzipPath.Replace(wxT(".dat"), wxT(".gzip"));
	// 一時保存用のパスを設定する
	wxString tmpPath = outputFilePath;
	tmpPath.Replace(wxT(".dat"), wxT(".tmp"));
	// ヘッダーのパスを設定する
	wxString headerPath = outputFilePath;
	headerPath.Replace(wxT(".dat"), wxT(".header"));

	// URLからホスト名を取得する
	wxRegEx reThreadList(_T("(http://)([^/]+)/([^/]+)"),
			wxRE_ADVANCED + wxRE_ICASE);
	// ホスト名
	wxString hostName;
	if (reThreadList.IsValid()) {
		if (reThreadList.Matches(boardURL)) {
			hostName = reThreadList.GetMatch(boardURL, 2);
		}
	}

	// 解凍された板一覧情報が存在しないor前回の通信ログが残っていないならば通常通りソケット通信を行う
	if ((!wxFileExists(outputFilePath)) || (!wxFileExists(headerPath))) {
		DownloadThreadListNew((const wxString) gzipPath,
				(const wxString) headerPath, (const wxString) boardNameAscii,
				(const wxString) hostName, (const wxString) boardURL);
	} else {
		// そうでなければ前回の通信の差分を取得しに行く
		DownloadThreadListMod((const wxString) gzipPath,
				(const wxString) headerPath, (const wxString) boardNameAscii,
				(const wxString) hostName, (const wxString) boardURL);
	}

	// gzip拡張子のファイルがあれば、ファイルの解凍・UTF化を行う
	if (wxFile::Exists(gzipPath)) {
		JaneCloneUtil::DecommpressFile(gzipPath, tmpPath);
		JaneCloneUtil::ConvertSJISToUTF8(tmpPath, outputFilePath);
	}
	// 更新が終わったらgzipファイルとSJISファイルを消しておく
	RemoveTmpFile(gzipPath);
	RemoveTmpFile(tmpPath);

	return outputFilePath;
}

/**
 * 新規にスレッド一覧をダウンロードしてくるメソッド
 * @param gzipのダウンロード先パス
 * @param HTTPヘッダのダウンロード先パス
 * @param 板名（ascii）
 * @return 実行コード
 */
int SocketCommunication::DownloadThreadListNew(const wxString gzipPath,
		const wxString headerPath, const wxString boardNameAscii,
		const wxString hostName, const wxString boardURL) {

	/**
	 * スレッド一覧取得の凡例
	 *
	 * GET /[板名]/subject.txt HTTP/1.1
	 * Accept-Encoding: gzip
	 * Host: [サーバー]
	 * Accept: ＊/＊
	 * Referer: http://[サーバー]/[板名]/
	 * Accept-Language: ja
	 * User-Agent: Monazilla/1.00 (ブラウザ名/バージョン)
	 * Connection: close
	 */

	int rc = 0;

	// 取得先のパスを引数から作成する
	wxString getPath = wxT("GET /");
	getPath += boardNameAscii;
	getPath += wxT("/subject.txt");

	wxHTTP http;
	http.SetHeader(getPath, _T("HTTP/1.1"));
	http.SetHeader(_T("Accept-Encoding"), _T("gzip"));
	http.SetHeader(_T("Host"), hostName);
	http.SetHeader(_T("Accept"), _T("*/*"));
	http.SetHeader(_T("Referer"), boardURL);
	http.SetHeader(_T("Accept-Language"), _T("ja"));
	http.SetHeader(_T("User-Agent"), _T("Monazilla/1.00"));
	http.SetHeader(_T("Connection"), _T("close"));
	http.SetTimeout(5);

	wxString server = hostName;
	wxString path = wxT("/");
	path += boardNameAscii;
	path += wxT("/subject.txt");
	wxString msg = "";

	// 保存先を決める
	wxFileOutputStream output(gzipPath);
	wxDataOutputStream store(output);

	if (http.Connect(server, 80)) {
		wxInputStream *stream;
		stream = http.GetInputStream(path);

		if (stream == NULL) {
			return -1;
		} else {
			unsigned char buffer[1024];
			int byteRead;

			// ヘッダを書きだす
			WriteHeaderFile(http, (const wxString) headerPath);

			// ストリームを受け取るループ部分
			while (!stream->Eof()) {
				stream->Read(buffer, sizeof(buffer));
				store.Write8(buffer, sizeof(buffer));
				byteRead = stream->LastRead();
				if (byteRead <= 0) {
					break;
				}
			}
		}
	} else {
		return -1;
	}

	return rc;
}

/**
 * 前回との差分のスレッド一覧をダウンロードしてくるメソッド
 * @param gzipのダウンロード先パス
 * @param HTTPヘッダのダウンロード先パス
 * @param 板名（ascii）
 * @return 実行コード
 */
int SocketCommunication::DownloadThreadListMod(const wxString gzipPath,
		const wxString headerPath, const wxString boardNameAscii,
		const wxString hostName, const wxString boardURL) {
	/**
	 * スレッド一覧取得の凡例
	 *
	 * GET /[板名]/subject.txt HTTP/1.1
	 * Accept-Encoding: gzip
	 * Host: [サーバー]
	 * If-Modified-Since:[前回取得日時]
	 * Accept: ＊/＊
	 * Referer: http://[サーバー]/[板名]/
	 * Accept-Language: ja
	 * User-Agent: Monazilla/1.00 (ブラウザ名/バージョン)
	 * Connection: close
	 */
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

/**
 * HTTPヘッダを書きだす
 */
void SocketCommunication::WriteHeaderFile(wxHTTP& http,
		const wxString headerPath) {

	// ヘッダファイルの書き出し先を作る
	wxTextFile headerFile(headerPath);

	// 既にファイルが存在するならば中身を消す
	if (headerFile.Exists()) {
		headerFile.Open(wxConvUTF8);
		headerFile.Clear();
		// 存在しないなら作成する
	} else {
		headerFile.Create();
	}

	// ヘッダの内容を書きだしていく
	wxString status = wxT("HTTP1.1/ ");
	status += wxString::Format(_T("%d"), http.GetResponse());
	headerFile.AddLine(status, TEXT_ENDLINE_TYPE);
	headerFile.AddLine(http.GetHeader(wxT("Date")), TEXT_ENDLINE_TYPE);
	headerFile.AddLine(http.GetHeader(wxT("Server")), TEXT_ENDLINE_TYPE);
	headerFile.AddLine(http.GetHeader(wxT("Last-Modified")), TEXT_ENDLINE_TYPE);
	headerFile.AddLine(http.GetHeader(wxT("ETag")), TEXT_ENDLINE_TYPE);
	headerFile.AddLine(http.GetHeader(wxT("Accept-Ranges")), TEXT_ENDLINE_TYPE);
	headerFile.AddLine(http.GetHeader(wxT("Content-Length")),
			TEXT_ENDLINE_TYPE);
	headerFile.AddLine(http.GetHeader(wxT("Connection")), TEXT_ENDLINE_TYPE);
	headerFile.AddLine(http.GetContentType(), TEXT_ENDLINE_TYPE);

	// 書きだした内容を保存する
	headerFile.Write(wxTextFileType_None, wxConvUTF8);
	headerFile.Close();
}


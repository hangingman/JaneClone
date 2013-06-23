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

#include "socketcommunication.hpp"

/**
 * デストラクタ
 */
SocketCommunication::~SocketCommunication() {}
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
     wxString tmpPath = ::wxGetHomeDir() 
	  + wxFileSeparator 
	  + JANECLONE_DIR
	  + wxFileSeparator
	  + wxT("dat")
	  + wxFileSeparator
	  + wxT("tmp.html");

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
	  // 更新が終わったらgzipファイルとSJISファイルを消しておく
	  RemoveTmpFile(gzipPath);
	  RemoveTmpFile(tmpPath);
     }

     return rc;
}

/**
 * 新規に板一覧情報を取得しに行く
 */
int SocketCommunication::DownloadBoardListNew(const wxString outputPath,
					      const wxString headerPath) {

     // ヘッダの作成
     std::list<std::string> headers;
     headers.push_back("Accept-Encoding: gzip");
     headers.push_back("Host: menu.2ch.net");
     headers.push_back("Accept: ");
     headers.push_back("Referer: http://menu.2ch.net/");
     headers.push_back("Accept-Language: ja");
     headers.push_back("User-Agent: Monazilla/1.00");

     wxString server = wxT("menu.2ch.net");
     wxString path = wxT("/bbsmenu.html");

     const std::string url = std::string(server.mb_str()) + std::string(path.mb_str());

     try {

	  // 保存先を決める
	  curlpp::Cleanup myCleanup;
	  curlpp::Easy myRequest;
	  myRequest.setOpt(new curlpp::options::Url(url));
	  myRequest.setOpt(new curlpp::options::HttpHeader(headers));
	  myRequest.setOpt(new curlpp::options::Timeout(5));
	  myRequest.setOpt(new curlpp::options::Verbose(true));
          // ヘッダー用ファンクタを設定する
	  myRequest.setOpt(new curlpp::options::HeaderFunction(
				curlpp::types::WriteFunctionFunctor(this, &SocketCommunication::WriteHeaderData)));

	  // メインのデータ出力
	  std::ofstream ofs(outputPath.mb_str() , std::ios::out | std::ios::trunc | std::ios::binary );
	  curlpp::options::WriteStream ws(&ofs);
	  myRequest.setOpt(ws);

	  *m_logCtrl << wxT("2chの板一覧情報を取得 (ん`　 )") << wxT("\n");
	  *m_logCtrl << server + path << wxT("\n");
	  myRequest.perform();

	  // レスポンスヘッダーの書き出し
	  if (!respBuf.empty()) {
	       std::ofstream ofsHeader(headerPath.mb_str() , std::ios::out | std::ios::trunc );
	       ofsHeader << respBuf << std::endl;
	  }

	  return 0;

     } catch (curlpp::RuntimeError &e) {
	  *m_logCtrl << wxString(e.what(), wxConvUTF8) << wxString(outputPath.c_str(), wxConvUTF8) << wxT("\n");
     } catch (curlpp::LogicError &e) {
	  *m_logCtrl << wxString(e.what(), wxConvUTF8) << wxString(outputPath.c_str(), wxConvUTF8) << wxT("\n");
     }

     return -1;
}
/**
 * 前回との差分を取得しに行く
 */
int SocketCommunication::DownloadBoardListMod(const wxString outputPath,
					      const wxString headerPath) {

     // 最終更新日時をヘッダ情報から取得する
     wxString lastModifiedTime = GetHTTPResponseCode(headerPath,wxT("Last-Modified:"));
     // バイナリとヘッダは前回取得した名前と同じでは困るのでtmpファイルとして作成しておく
     wxString tmpOutputPath = outputPath;
     tmpOutputPath += wxT(".tmp");
     wxString tmpHeaderPath = headerPath;
     tmpHeaderPath += wxT(".tmp");

     // ヘッダの作成
     std::list<std::string> headers;
     headers.push_back("Accept-Encoding: gzip");
     headers.push_back("Host: menu.2ch.net");
     headers.push_back("If-Modified-Since:" + std::string(lastModifiedTime.mb_str()));
     headers.push_back("Accept: ");
     headers.push_back("Referer: http://menu.2ch.net/");
     headers.push_back("Accept-Language: ja");
     headers.push_back("User-Agent: Monazilla/1.00");

     wxString server = wxT("menu.2ch.net");
     wxString path = wxT("/bbsmenu.html");

     const std::string url = std::string(server.mb_str()) + std::string(path.mb_str());

     try {

	  // 保存先を決める
	  curlpp::Cleanup myCleanup;
	  curlpp::Easy myRequest;
	  myRequest.setOpt(new curlpp::options::Url(url));
	  myRequest.setOpt(new curlpp::options::HttpHeader(headers));
	  myRequest.setOpt(new curlpp::options::Timeout(5));
	  myRequest.setOpt(new curlpp::options::Verbose(true));
          // ヘッダー用ファンクタを設定する
	  myRequest.setOpt(new curlpp::options::HeaderFunction(
				curlpp::types::WriteFunctionFunctor(this, &SocketCommunication::WriteHeaderData)));

	  std::ofstream ofs(tmpOutputPath.mb_str() , std::ios::out | std::ios::trunc | std::ios::binary );
	  curlpp::options::WriteStream ws(&ofs);
	  myRequest.setOpt(ws);

	  *m_logCtrl << wxT("2chの板一覧情報を取得 (ん`　 )") << wxT("\n");
	  *m_logCtrl << server + path << wxT("\n");
	  
	  myRequest.perform();

	  long rc = curlpp::infos::ResponseCode::get(myRequest);
	  *m_logCtrl << wxT("HTTP") << wxString::Format(wxT("%lu"), rc) << wxT("\n");

	  
	  if (rc = 304) {
	       // レスポンスコードが304ならば変更なしなので正常終了
	       RemoveTmpFile(tmpOutputPath);
	       return 0;
	  }

	  // 最後までうまく行った場合
	  RemoveTmpFile(outputPath);
	  // tmpファイルを本物のファイルとする
	  wxRenameFile(tmpOutputPath, outputPath);
	  // レスポンスヘッダーの書き出し
	  if (!respBuf.empty()) {
	       std::ofstream ofsHeader(headerPath.mb_str() , std::ios::out | std::ios::trunc );
	       ofsHeader << respBuf << std::endl;
	  }
	  
	  return 0;

     } catch (curlpp::RuntimeError &e) {
	  *m_logCtrl << wxString(e.what(), wxConvUTF8) << wxString(outputPath.c_str(), wxConvUTF8) << wxT("\n");
     } catch (curlpp::LogicError &e) {
	  *m_logCtrl << wxString(e.what(), wxConvUTF8) << wxString(outputPath.c_str(), wxConvUTF8) << wxT("\n");
     }

     return -1;
}
/**
 * 通信ログに残っているHTTPレスポンスコードを取得する
 */
wxString SocketCommunication::GetHTTPResponseCode(const wxString headerPath,
						  const wxString reqCode) {

     wxString resCode = wxT("");

     // ログを読み込む
     wxTextFile file(headerPath);
     file.Open(wxConvUTF8);
     wxString line;

     if (file.IsOpened()) {
	  // ログの中身を1行ずつ走査
	  for (line = file.GetFirstLine(); !file.Eof(); line =
		    file.GetNextLine()) {
	       if (line.Contains(reqCode)) {
		    // 「Last-Modified:」、「ETag:」のような引数が入る
		    resCode = line.Mid(reqCode.Len());
		    break;
	       }
	  }
	  // ファイルのクローズを行う
	  file.Close();
	  return resCode;
     }

     return resCode = wxT("could't read response code");
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
     wxString outputFilePath = 
	  ::wxGetHomeDir()
	  + wxFileSeparator 
	  + JANECLONE_DIR
	  + wxFileSeparator
	  + wxT("dat")
	  + wxFileSeparator
	  + boardNameAscii;

     // 保存用フォルダ存在するか確認。無ければフォルダを作成
     if (!wxDir::Exists(outputFilePath)) {
	  ::wxMkdir(outputFilePath);
     }

     outputFilePath += wxFileSeparator;
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

     // ヘッダの作成
     std::list<std::string> headers;
     headers.push_back(std::string(getPath.mb_str()) + ": HTTP/1.1");
     headers.push_back("Accept-Encoding: gzip");
     headers.push_back("Host: " + std::string(hostName.mb_str()));
     headers.push_back("Accept: */*");
     headers.push_back("Referer: " + std::string(boardURL.mb_str()));
     headers.push_back("Accept-Language: ja");
     headers.push_back("User-Agent: Monazilla/1.00");
     headers.push_back("Connection: close");

     wxString server = hostName;
     wxString path = wxT("/");
     path += boardNameAscii;
     path += wxT("/subject.txt");

     const std::string url = std::string(server.mb_str()) + std::string(path.mb_str());

     try {

	  // 保存先を決める
	  curlpp::Cleanup myCleanup;
	  curlpp::Easy myRequest;
	  myRequest.setOpt(new curlpp::options::Url(url));
	  myRequest.setOpt(new curlpp::options::HttpHeader(headers));
	  myRequest.setOpt(new curlpp::options::Timeout(5));
	  myRequest.setOpt(new curlpp::options::Verbose(true));
          // ヘッダー用ファンクタを設定する
	  myRequest.setOpt(new curlpp::options::HeaderFunction(
				curlpp::types::WriteFunctionFunctor(this, &SocketCommunication::WriteHeaderData)));

	  std::ofstream ofs(gzipPath.mb_str() , std::ios::out | std::ios::trunc | std::ios::binary );
	  curlpp::options::WriteStream ws(&ofs);
	  myRequest.setOpt(ws);

	  *m_logCtrl << wxT("スレッド一覧を取得 (ん`　 )") << wxT("\n");
	  *m_logCtrl << server + path << wxT("\n");

	  myRequest.perform();

	  // レスポンスヘッダーの書き出し
	  if (!respBuf.empty()) {
	       std::ofstream ofsHeader(headerPath.mb_str() , std::ios::out | std::ios::trunc );
	       ofsHeader << respBuf << std::endl;
	  }
	  
	  return 0;

     } catch (curlpp::RuntimeError &e) {
	  *m_logCtrl << wxString(e.what(), wxConvUTF8) << wxString(gzipPath.c_str(), wxConvUTF8) << wxT("\n");
     } catch (curlpp::LogicError &e) {
	  *m_logCtrl << wxString(e.what(), wxConvUTF8) << wxString(gzipPath.c_str(), wxConvUTF8) << wxT("\n");
     }

     return -1;
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

     // 最終更新日時をヘッダ情報から取得する
     wxString lastModifiedTime = GetHTTPResponseCode(headerPath, wxT("Last-Modified:"));
     // バイナリとヘッダは前回取得した名前と同じでは困るのでtmpファイルとして作成しておく
     wxString tmpOutputPath(gzipPath);
     tmpOutputPath.Replace(wxT(".gzip"), wxT(".tmp"));
     wxString tmpHeaderPath = headerPath;
     tmpHeaderPath += wxT(".tmp");

     // 取得先のパスを引数から作成する
     wxString getPath = wxT("GET /");
     getPath += boardNameAscii;
     getPath += wxT("/subject.txt");

     // ヘッダの作成
     std::list<std::string> headers;
     headers.push_back(std::string(getPath.mb_str()) + ": HTTP/1.1");
     headers.push_back("Accept-Encoding: gzip");
     headers.push_back("Host: " + std::string(hostName.mb_str()));
     headers.push_back("If-Modified-Since: " + std::string(lastModifiedTime.mb_str()));
     headers.push_back("Accept: */*");
     headers.push_back("Referer: " + std::string(boardURL.mb_str()));
     headers.push_back("Accept-Language: ja");
     headers.push_back("User-Agent: Monazilla/1.00");
     headers.push_back("Connection: close");

     wxString server = hostName;
     wxString path = wxT("/");
     path += boardNameAscii;
     path += wxT("/subject.txt");

     const std::string url = std::string(server.mb_str()) + std::string(path.mb_str());

     try {

	  // 保存先を決める
	  curlpp::Cleanup myCleanup;
	  curlpp::Easy myRequest;
	  myRequest.setOpt(new curlpp::options::Url(url));
	  myRequest.setOpt(new curlpp::options::HttpHeader(headers));
	  myRequest.setOpt(new curlpp::options::Timeout(5));
	  myRequest.setOpt(new curlpp::options::Verbose(true));
          // ヘッダー用ファンクタを設定する
	  myRequest.setOpt(new curlpp::options::HeaderFunction(
				curlpp::types::WriteFunctionFunctor(this, &SocketCommunication::WriteHeaderData)));

	  std::ofstream ofs(tmpOutputPath.mb_str() , std::ios::out | std::ios::trunc | std::ios::binary );
	  curlpp::options::WriteStream ws(&ofs);
	  myRequest.setOpt(ws);

	  *m_logCtrl << wxT("スレッド一覧を取得 (ん`　 )") << wxT("\n");
	  *m_logCtrl << server + path << wxT("\n");
	  myRequest.perform();

	  long rc = curlpp::infos::ResponseCode::get(myRequest);
	  
	  if (rc = 304) {
	       // レスポンスコードが304ならば変更なしなので正常終了
	       RemoveTmpFile(tmpOutputPath);
	       return 0;
	  }

	  // tmpファイルを本物のファイルとする
	  wxRenameFile(tmpOutputPath, gzipPath);

	  // レスポンスヘッダーの書き出し
	  if (!respBuf.empty()) {
	       std::ofstream ofsHeader(headerPath.mb_str() , std::ios::out | std::ios::trunc );
	       ofsHeader << respBuf << std::endl;
	  }
	  
	  return 0;

     } catch (curlpp::RuntimeError &e) {
	  *m_logCtrl << wxString(e.what(), wxConvUTF8) << wxString(gzipPath.c_str(), wxConvUTF8) << wxT("\n");
     } catch (curlpp::LogicError &e) {
	  *m_logCtrl << wxString(e.what(), wxConvUTF8) << wxString(gzipPath.c_str(), wxConvUTF8) << wxT("\n");
     }

     return -1;
}

/**
 * スレッドのデータをダウンロードしてくるメソッド
 * @param 板名
 * @param URL
 * @param サーバー名
 * @param 固有番号
 * @return ダウンロードしたdatファイル保存先
 */
wxString SocketCommunication::DownloadThread(const wxString boardName,
					     const wxString boardURL, const wxString boardNameAscii,
					     const wxString origNumber) {

     // 出力するファイルの名前
     wxString outputFileName = origNumber + wxT(".dat");
     // 出力先のファイルパスを設定する
     wxString outputFilePath = 
	  ::wxGetHomeDir()
	  + wxFileSeparator 
	  + JANECLONE_DIR
	  + wxFileSeparator
	  + wxT("dat")
	  + wxFileSeparator
	  + boardNameAscii;

     // 保存用フォルダ存在するか確認。無ければフォルダを作成
     if (!wxDir::Exists(outputFilePath)) {
	  ::wxMkdir(outputFilePath);
     }

     outputFilePath += wxFileSeparator;
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

     if ((!wxFileExists(outputFilePath)) || (!wxFileExists(headerPath))) {
	  // 解凍された板一覧情報が存在しないor前回の通信ログが残っていないならば通常通りソケット通信を行う
	  DownloadThreadNew((const wxString) gzipPath,
			    (const wxString) headerPath, (const wxString) boardNameAscii,
			    (const wxString) origNumber, (const wxString) hostName);
     } else {
	  // そうでなければ前回の通信の差分を取得しに行く
	  DownloadThreadMod((const wxString) gzipPath,
			    (const wxString) headerPath, (const wxString) boardNameAscii,
			    (const wxString) origNumber, (const wxString) hostName);
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
 * 新規にスレッドのデータをダウンロードしてくるメソッド
 * @param gzipのダウンロード先パス
 * @param HTTPヘッダのダウンロード先パス
 * @param 板名（ascii）
 * @param 固有番号
 * @return 実行コード
 */
void SocketCommunication::DownloadThreadNew(const wxString gzipPath,
					    const wxString headerPath, const wxString boardNameAscii,
					    const wxString origNumber, const wxString hostName) {

     /**
      * スレッド取得の凡例(１回目)
      *
      * GET /[板名]/dat/[スレッド番号].dat HTTP/1.1
      * Accept-Encoding: gzip
      * Host: [サーバー]
      * Accept: ＊/＊
      * Referer: http://[サーバー]/test/read.cgi/[板名]/[スレッド番号]/
      * Accept-Language: ja
      * User-Agent: Monazilla/1.00 (ブラウザ名/バージョン)
      * Connection: close
      */

     // 取得先のパスを引数から作成する
     const wxString getPath = wxT("GET /") + boardNameAscii + wxT("/dat/")
	  + origNumber + wxT(".dat");
     // リファラを引数から作成する
     const wxString referer = wxT("http://") + hostName + wxT("/test/read.cgi/")
	  + boardNameAscii + wxT("/") + origNumber;

     // ヘッダの作成
     std::list<std::string> headers;
     headers.push_back(std::string(getPath.mb_str()) + ": HTTP/1.1");
     headers.push_back("Accept-Encoding: gzip");
     headers.push_back("Host: " + std::string(hostName.mb_str()));
     headers.push_back("Accept: */*");
     headers.push_back("Referer: "+ std::string(referer.mb_str()));
     headers.push_back("Accept-Language: ja");
     headers.push_back("User-Agent: Monazilla/1.00");
     headers.push_back("Connection: close");

     wxString server = hostName;
     wxString path = wxT("/") + boardNameAscii + wxT("/dat/") + origNumber + wxT(".dat");
     const std::string url = std::string(server.mb_str()) + std::string(path.mb_str());

     try {

	  // 保存先を決める
	  curlpp::Cleanup myCleanup;
	  curlpp::Easy myRequest;
	  myRequest.setOpt(new curlpp::options::Url(url));
	  myRequest.setOpt(new curlpp::options::HttpHeader(headers));
	  myRequest.setOpt(new curlpp::options::Timeout(5));
	  myRequest.setOpt(new curlpp::options::Verbose(true));
          // ヘッダー用ファンクタを設定する
	  myRequest.setOpt(new curlpp::options::HeaderFunction(
				curlpp::types::WriteFunctionFunctor(this, &SocketCommunication::WriteHeaderData)));

	  std::ofstream ofs(gzipPath.mb_str() , std::ios::out | std::ios::trunc | std::ios::binary );
	  curlpp::options::WriteStream ws(&ofs);
	  myRequest.setOpt(ws);

	  *m_logCtrl << wxT("2chのスレッドを取得 (ん`　 )") << wxT("\n");
	  *m_logCtrl << server + path << wxT("\n");
	  myRequest.perform();

	  // レスポンスヘッダーの書き出し
	  if (!respBuf.empty()) {
	       std::ofstream ofsHeader(headerPath.mb_str() , std::ios::out | std::ios::trunc );
	       ofsHeader << respBuf << std::endl;
	  }

     } catch (curlpp::RuntimeError &e) {
	  *m_logCtrl << wxString(e.what(), wxConvUTF8) << wxString(gzipPath.c_str(), wxConvUTF8) << wxT("\n");
     } catch (curlpp::LogicError &e) {
	  *m_logCtrl << wxString(e.what(), wxConvUTF8) << wxString(gzipPath.c_str(), wxConvUTF8) << wxT("\n");
     }
}

/**
 * 前回との差分のスレッドのデータをダウンロードしてくるメソッド
 * @param URL
 * @param サーバー名
 * @param 固有番号
 * @return ダウンロードしたdatファイル保存先
 */
int SocketCommunication::DownloadThreadMod(const wxString gzipPath,
					   const wxString headerPath, const wxString boardNameAscii,
					   const wxString origNumber, const wxString hostName) {

     /**
      * スレッド取得の凡例(２回目)
      *
      * GET /[板名]/dat/[スレッド番号].dat HTTP/1.1
      * Host: [サーバー]
      * Accept: ＊/＊
      * Referer: http://[サーバー]/test/read.cgi/[板名]/[スレッド番号]/
      * Accept-Language: ja
      * If-Modified-Since:***
      * If-None-Match:***
      * Range: bytes=***
      * User-Agent: Monazilla/1.00 (ブラウザ名/バージョン)
      * Connection: close
      */

     int rc = 0;

     // 取得先のパスを引数から作成する
     const wxString getPath = wxT("GET /") + boardNameAscii + wxT("/dat/") + origNumber + wxT(".dat");
     // リファラを引数から作成する
     const wxString referer = wxT("http://") + hostName + wxT("/test/read.cgi/") + boardNameAscii + wxT("/") + origNumber;
     // 最終更新時間を作成する
     const wxString lastModifiedTime = GetHTTPResponseCode(headerPath, wxT("Last-Modified:"));
     // etag
     const wxString etag = GetHTTPResponseCode(headerPath, wxT("ETag:"));
     // ファイルサイズ
     wxString datFilePath = headerPath;
     datFilePath.Replace(wxT(".header"), wxT(".dat"));
     wxULongLong fileSize = wxFileName::DirName(datFilePath).GetSize();

     // ヘッダの作成
     std::list<std::string> headers;
     headers.push_back(std::string(getPath.mb_str()) + ": HTTP/1.1");
     headers.push_back("Host: " + std::string(hostName.mb_str()));
     headers.push_back("Accept: */*");
     headers.push_back("Referer: "+ std::string(referer.mb_str()));
     headers.push_back("If-Modified-Since: " + std::string(lastModifiedTime.mb_str()));
     headers.push_back("If-None-Match: " + std::string(etag.mb_str()));
     headers.push_back("Range: " + std::string(fileSize.ToString().mb_str()));
     headers.push_back("User-Agent: Monazilla/1.00");
     headers.push_back("Connection: close");

     wxString server = hostName;
     wxString path = wxT("/") + boardNameAscii + wxT("/dat/") + origNumber + wxT(".dat");
     const std::string url = std::string(server.mb_str()) + std::string(path.mb_str());

     try {

	  // 保存先を決める
	  curlpp::Cleanup myCleanup;
	  curlpp::Easy myRequest;
	  myRequest.setOpt(new curlpp::options::Url(url));
	  myRequest.setOpt(new curlpp::options::HttpHeader(headers));
	  myRequest.setOpt(new curlpp::options::Timeout(5));
	  myRequest.setOpt(new curlpp::options::Verbose(true));
          // ヘッダー用ファンクタを設定する
	  myRequest.setOpt(new curlpp::options::HeaderFunction(
				curlpp::types::WriteFunctionFunctor(this, &SocketCommunication::WriteHeaderData)));
          // BODY用ファンクタを設定する
	  myRequest.setOpt(new curlpp::options::WriteFunction(
				curlpp::types::WriteFunctionFunctor(this, &SocketCommunication::WriteDataInternal)));	  

	  *m_logCtrl << wxT("2chのスレッドを取得 (ん`　 )") << wxT("\n");
	  *m_logCtrl << server + path << wxT("\n");
	  myRequest.perform();

	  long rc = curlpp::infos::ResponseCode::get(myRequest);
	  
	  if (rc == 304) {
	       // レスポンスコードが304ならば変更なし、何もしない
	  } else if (rc == 206) {
	       // スレッドに更新ありの場合の処理、更新部分を追加する
	       if (!bodyBuf.empty()) {
		    std::ofstream ofsBody(gzipPath.mb_str() , std::ios::out | std::ios::app );
		    ofsBody << bodyBuf << std::endl;
	       }
	  }

	  // レスポンスヘッダーの書き出し
	  if (!respBuf.empty()) {
	       std::ofstream ofsHeader(headerPath.mb_str() , std::ios::out | std::ios::trunc );
	       ofsHeader << respBuf << std::endl;
	  }

     } catch (curlpp::RuntimeError &e) {
	  *m_logCtrl << wxString(e.what(), wxConvUTF8) << wxString(gzipPath.c_str(), wxConvUTF8) << wxT("\n");
     } catch (curlpp::LogicError &e) {
	  *m_logCtrl << wxString(e.what(), wxConvUTF8) << wxString(gzipPath.c_str(), wxConvUTF8) << wxT("\n");
     }

     return -1;
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
size_t SocketCommunication::WriteHeaderData(char *ptr, size_t size, size_t nmemb) {

     // 文字列をメンバ変数に格納
     size_t realsize = size * nmemb;
     std::string line(static_cast<const char *>(ptr), realsize);

     // ログに出力する
     if (std::string::npos != line.find("HTTP")) *m_logCtrl << wxString(line.c_str(), wxConvUTF8) << wxT("\n");

     respBuf.append(line);

     return realsize;
}
/**
 * HTTPボディを書きだす
 */
size_t SocketCommunication::WriteDataInternal(char *ptr, size_t size, size_t nmemb) {
     // 文字列をメンバ変数に格納
     size_t realsize = size * nmemb;
     bodyBuf.append( static_cast<const char *>(ptr), realsize );
     return realsize;
}
/**
   ・bbs.cgiを呼び出すとスレッドにレスの書き込みができます。

   ・bbs.cgiは荒らし対策・規約承諾チェックなどが施されており、複数回呼び出す必要があります。

   ＜初回の書き込み＞
   bbs.cgiを呼び出す
   →書き込み確認画面・・・応答メッセージよりクッキー（１）・隠し項目（hana=mogera）を受け取る
   →再度bbs.cgiを呼び出す・・・クッキー（１）と隠し項目を付加する
   →書き込み完了・・・応答メッセージよりクッキー（２）を受け取る

   ＜２回目以降の書き込み＞
   クッキー（１）（２）を付加してbbs.cgiを呼び出す→書き込み完了

   参考：http://www.monazilla.org/index.php?e=199
*/

/**
 * スレッドへの初回書き込みを行うメソッド
 * @param 板名,URL,サーバー名
 * @return 書き込み結果
 */
wxString SocketCommunication::PostFirstToThread(URLvsBoardName& boardInfoHash, ThreadInfo& threadInfoHash, const int status) {

/**
   要求メッセージの一例（初回投稿時・２回目）
   POST /test/bbs.cgi HTTP/1.1
   Host: [サーバー]
   Accept: ＊/＊
   Referer: http://[サーバー]/test/read.cgi/[板名]/[スレッド番号]/
   Accept-Language: ja
   User-Agent: Monazilla/1.00 (ブラウザ名/バージョン)
   Content-Length: ポストするデータの合計サイズ(バイト)
   Cookie:応答ヘッダのSet-Cookieに記述された内容 Connection: close

   bbs=[板名]&key=[スレッド番号]&time=[投稿時間]&FROM=[名前]&mail=[メール]&MESSAGE=[本文]&submit=[ボタンの文字]&[不可視項目名]=[不可視項目値]
*/

     // URLからホスト名を取得する
     wxRegEx reThreadList(_T("(http://)([^/]+)/([^/]+)"),
			  wxRE_ADVANCED + wxRE_ICASE);
     // ホスト名
     wxString hostName;
     if (reThreadList.IsValid()) {
	  if (reThreadList.Matches(boardInfoHash.boardURL)) {
	       hostName = reThreadList.GetMatch(boardInfoHash.boardURL, 2);
	  }
     }

     // 投稿時間を算出する(UNIX Time)
     wxString timeNow = JaneCloneUtil::GetTimeNow();

     wxDir dir(::wxGetHomeDir() + wxFileSeparator + JANECLONE_DIR);
     wxString headerPath = dir.GetName();
     wxDir jcDir(::wxGetHomeDir() + wxFileSeparator + JANECLONE_DIR);
     wxDir datDir(jcDir.GetName() + wxFileSeparator + wxT("dat"));

     // ユーザーのホームディレクトリに隠しフォルダがあるかどうか確認
     if (!dir.HasSubDirs(JANECLONE_DIR)) {
	  ::wxMkdir(jcDir.GetName());
     }

     if (!jcDir.HasSubDirs(wxT("dat"))) {
	  ::wxMkdir(jcDir.GetName() + wxFileSeparator + wxT("dat"));
     }

     if (!datDir.HasSubDirs(wxT("kakikomi"))) {
	  ::wxMkdir(datDir.GetName() + wxFileSeparator + wxT("kakikomi"));
     }

#ifdef __WXMSW__
     // Windowsではパスの区切りは"\"
     headerPath += wxT("\\dat\\");
     headerPath += wxT("kakikomi");
     headerPath += wxT("\\");
     headerPath += timeNow;
     headerPath += wxT(".header");
#else
     // それ以外ではパスの区切りは"/"
     headerPath += wxT("/dat/");
     headerPath += wxT("kakikomi");
     headerPath += wxT("/");
     headerPath += timeNow;
     headerPath += wxT(".header");
#endif

     wxString buttonText = wxT("%8F%91%82%AB%8D%9E%82%DE");

     // Postする内容のデータサイズを取得する
     wxString kakikomiInfo = wxT("bbs=") + boardInfoHash.boardNameAscii + wxT("&key=")
	  + threadInfoHash.origNumber + wxT("&time=") + timeNow + wxT("&FROM=")
	  + postContent->name + wxT("&mail") + postContent->mail + wxT("&MESSAGE")
	  + postContent->kakikomi + wxT("&submit=") + buttonText;

     // URL
     const wxString boardURL = boardInfoHash.boardURL;
     // リファラを引数から作成する
     const wxString referer = wxT("http://") + hostName + wxT("/") + boardInfoHash.boardNameAscii + wxT("/");

     // wxHTTPはまだ発展途上のクラスのようで、2012年現在POST用メソッドが安定版に
     // 登録されていないので、代わりにwxSocketClientを使う
     // ソースの統一性を持たせるため、後で全てwxSocketClientに変えるかもしれない
     wxSocketClient* socket = new wxSocketClient();
     socket->SetTimeout(5);

     /**
      * ヘッダを設定する
      */
     wxString header = wxT("");
     // POST
     header += wxT("POST /test/bbs.cgi HTTP/1.1\n");
     // Connection close
     header += wxT("Connection: close\n");
     // Content-Type
     header += wxT("Content-Type: application/x-www-form-urlencoded\n");
     // Content-Length
     header += wxT("Content-Length: ");
     header += wxString::Format(_("%zd"), kakikomiInfo.Len());
     header += wxT("\n");
     // hostname
     header += wxT("Host: ");
     header += hostName;
     header += wxT("\n");
     // Accept
     header += wxT("Accept: text/html, */*\n");
     // Referer
     header += wxT("Referer: ");
     header += referer;
     header += wxT("\n");
     // Accept-Language
     header += wxT("Accept-Language: ja\n");
     // User-Agent
     header += wxT("User-Agent: Monazilla/1.00 (JaneClone/0.80)\n");
     // POST
     header += wxT("\n");
     header += kakikomiInfo;

     // ホストに接続する
     wxIPV4address* address = new wxIPV4address();
     address->Hostname(hostName);
     address->Service(80);

     if (!socket->Connect(*address)) {
	  // 接続失敗
	  *m_logCtrl << wxT("　(ﾟ)(ﾟ) ") << wxT("\n");
	  *m_logCtrl << wxT("彡　　と ＜　書込失敗、ち～ん") << wxT("\n");
	  delete socket;
	  delete address;
	  return FAIL_TO_POST;
     }

     // ヘッダ情報を書き込む
     socket->Write(header.c_str(),header.Len());
     wxString wHeaderLog = wxString::Format(_("Wrote %u out of %zd bytes"),socket->LastCount(), header.Len());
     *m_logCtrl << wHeaderLog << wxT("\n");

     // レスポンスを受け取る
     wxString resPath = headerPath;
     wxFileOutputStream output(headerPath);
     wxDataOutputStream out(output);

     wxInputStream *stream = new wxSocketInputStream(*socket);
     if (!stream) {
	  // ERROR
	  *m_logCtrl << wxT("内部エラー：ストリームの作成に失敗") << wxT("\n");
	  delete stream;
	  return FAIL_TO_POST;
     }

     unsigned char ch[1];
     int byteRead;

     // ストリームを受け取るループ部分
     while (!stream->Eof()) {
	  stream->Read(ch, 1);
	  out.Write8(ch, 1);
	  byteRead = stream->LastRead();
	  if (byteRead < 0) {
	       break;
	  }
     }

     delete stream;
     output.Close();

     // Shift_JIS から UTF-8への変換処理
     wxNKF* nkf = new wxNKF();
     wxString tmpPath = headerPath;
     tmpPath.Replace(wxT(".header"), wxT(".tmp"));
     nkf->Convert(headerPath, tmpPath, wxT("--ic=CP932 --oc=UTF-8"));
     delete nkf;

     // ファイルのリネーム
     wxRenameFile(tmpPath, headerPath);
     // COOKIEのデータをコンフィグファイルに書き出す
     WriteCookieData(headerPath);

     return headerPath;
}
/**
 * 投稿確認ボタンイベントの後にスレッドに書き込むメソッド
 * @param 板名,URL,サーバー名
 * @return 書き込み結果
 */
wxString SocketCommunication::PostConfirmToThread(URLvsBoardName& boardInfoHash, ThreadInfo& threadInfoHash, const int status) {

/**
   要求メッセージの一例（初回投稿時・２回目）
   POST /test/bbs.cgi HTTP/1.1
   Host: [サーバー]
   Accept: ＊/＊
   Referer: http://[サーバー]/test/read.cgi/[板名]/[スレッド番号]/
   Accept-Language: ja
   User-Agent: Monazilla/1.00 (ブラウザ名/バージョン)
   Content-Length: ポストするデータの合計サイズ(バイト)
   Cookie:応答ヘッダのSet-Cookieに記述された内容 Connection: close

   bbs=[板名]&key=[スレッド番号]&time=[投稿時間]&FROM=[名前]&mail=[メール]&MESSAGE=[本文]&submit=[ボタンの文字]&[不可視項目名]=[不可視項目値]
*/

     // 不可視項目値をコンフィグファイルから取得する
     InitializeCookie();
     wxString hiddenName = wxT("ERROR"), hiddenVal = wxT("ERROR"), cookie = wxT("ERROR");
     JaneCloneUtil::GetJaneCloneProperties(wxT("HiddenName"), &hiddenName);
     JaneCloneUtil::GetJaneCloneProperties(wxT("HiddenValue"), &hiddenVal);
     JaneCloneUtil::GetJaneCloneProperties(wxT("Cookie"), &cookie);

     // 読み取り失敗ならば書込失敗
     if (hiddenName == wxT("ERROR") || hiddenVal == wxT("ERROR") || cookie == wxT("ERROR"))
	  return FAIL_TO_POST;

     // URLからホスト名を取得する
     wxRegEx reThreadList(_T("(http://)([^/]+)/([^/]+)"),
			  wxRE_ADVANCED + wxRE_ICASE);
     // ホスト名
     wxString hostName;
     if (reThreadList.IsValid()) {
	  if (reThreadList.Matches(boardInfoHash.boardURL)) {
	       hostName = reThreadList.GetMatch(boardInfoHash.boardURL, 2);
	  }
     }

     // 投稿時間を算出する(UNIX Time)
     wxString timeNow = JaneCloneUtil::GetTimeNow();

     wxDir dir(::wxGetHomeDir() + wxFileSeparator + JANECLONE_DIR);
     wxString headerPath = dir.GetName();
     wxDir jcDir(::wxGetHomeDir() + wxFileSeparator + JANECLONE_DIR);
     wxDir datDir(jcDir.GetName() + wxFileSeparator + wxT("dat"));

     // ユーザーのホームディレクトリに隠しフォルダがあるかどうか確認
     if (!dir.HasSubDirs(JANECLONE_DIR)) {
	  ::wxMkdir(jcDir.GetName());
     }

     if (!jcDir.HasSubDirs(wxT("dat"))) {
	  ::wxMkdir(jcDir.GetName() + wxFileSeparator + wxT("dat"));
     }

     if (!datDir.HasSubDirs(wxT("kakikomi"))) {
	  ::wxMkdir(datDir.GetName() + wxFileSeparator + wxT("kakikomi"));
     }

#ifdef __WXMSW__
     // Windowsではパスの区切りは"\"
     headerPath += wxT("\\dat\\");
     headerPath += wxT("kakikomi");
     headerPath += wxT("\\");
     headerPath += timeNow;
     headerPath += wxT(".header");
#else
     // それ以外ではパスの区切りは"/"
     headerPath += wxT("/dat/");
     headerPath += wxT("kakikomi");
     headerPath += wxT("/");
     headerPath += timeNow;
     headerPath += wxT(".header");
#endif

     wxString buttonText = wxT("%8F%91%82%AB%8D%9E%82%DE");

     // Postする内容のデータサイズを取得する
     wxString kakikomiInfo = wxT("bbs=") + boardInfoHash.boardNameAscii + wxT("&key=")
	  + threadInfoHash.origNumber + wxT("&time=") + timeNow + wxT("&FROM=")
	  + postContent->name + wxT("&mail=") + postContent->mail + wxT("&MESSAGE=")
	  + postContent->kakikomi + wxT("&submit=") + buttonText + wxT("&")
	  + hiddenName + wxT("=") + hiddenVal;

     // URL
     const wxString boardURL = boardInfoHash.boardURL;
     // リファラを引数から作成する
     const wxString referer = wxT("http://") + hostName + wxT("/") + boardInfoHash.boardNameAscii + wxT("/");

     // wxHTTPはまだ発展途上のクラスのようで、2012年現在POST用メソッドが安定版に
     // 登録されていないので、代わりにwxSocketClientを使う
     // ソースの統一性を持たせるため、後で全てwxSocketClientに変えるかもしれない
     wxSocketClient* socket = new wxSocketClient();
     socket->SetTimeout(5);

     /**
      * ヘッダを設定する
      */
     wxString header = wxT("");
     // POST
     header += wxT("POST /test/bbs.cgi HTTP/1.1\n");
     // hostname
     header += wxT("Host: ");
     header += hostName;
     header += wxT("\n");
     // Accept
     header += wxT("Accept: */*\n");
     // Referer
     header += wxT("Referer: ");
     header += referer;
     header += wxT("\n");
     // Accept-Language
     header += wxT("Accept-Language: ja\n");
     // User-Agent
     header += wxT("User-Agent: Monazilla/1.00 (JaneClone/0.80)\n");
     // Content-Type
     header += wxT("Content-Type: application/x-www-form-urlencoded\n");
     // Content-Length
     header += wxT("Content-Length: ");
     header += wxString::Format(_("%zd"), kakikomiInfo.Len());
     header += wxT("\n");
     // Cookie
     header += wxT("Cookie: ");
     header += cookie;
     header += wxT(" Connection: close\n");
     // POST
     header += wxT("\n");
     header += kakikomiInfo;

     // ホストに接続する
     wxIPV4address* address = new wxIPV4address();
     address->Hostname(hostName);
     address->Service(80);

     // サーバ時間よりローカル端末の時間が進んでいる場合が多いので一度スリープ
     wxSleep(2); // とりあえず２秒

     if (!socket->Connect(*address)) {
	  // 接続失敗
	  *m_logCtrl << wxT("　(ﾟ)(ﾟ) ") << wxT("\n");
	  *m_logCtrl << wxT("彡　　と ＜　書込失敗、ち～ん") << wxT("\n");
	  delete socket;
	  delete address;
	  return FAIL_TO_POST;
     }

     // ヘッダ情報を書き込む
     socket->Write(header.c_str(),header.Len());
     wxString wHeaderLog = wxString::Format(_("Wrote %u out of %zd bytes"),socket->LastCount(), header.Len());
     *m_logCtrl << wHeaderLog << wxT("\n");

     // レスポンスを受け取る
     wxString resPath = headerPath;
     wxFileOutputStream output(headerPath);
     wxDataOutputStream out(output);

     wxInputStream *stream = new wxSocketInputStream(*socket);
     if (!stream) {
	  // ERROR
	  *m_logCtrl << wxT("内部エラー：ストリームの作成に失敗") << wxT("\n");
	  delete stream;
	  return FAIL_TO_POST;
     }

     unsigned char ch[1];
     int byteRead;

     // ストリームを受け取るループ部分
     while (!stream->Eof()) {
	  stream->Read(ch, 1);
	  out.Write8(ch, 1);
	  byteRead = stream->LastRead();
	  if (byteRead < 0) {
	       break;
	  }
     }

     delete stream;
     output.Close();

     // Shift_JIS から UTF-8への変換処理
     wxNKF* nkf = new wxNKF();
     wxString tmpPath = headerPath;
     tmpPath.Replace(wxT(".header"), wxT(".tmp"));
     nkf->Convert(headerPath, tmpPath, wxT("--ic=CP932 --oc=UTF-8"));
     delete nkf;

     // ファイルのリネーム
     wxRenameFile(tmpPath, headerPath);
     // PERNのデータをコンフィグファイルに書き出す
     WritePernData(headerPath);

     return headerPath;
}
/**
 * 通常の書き込みメソッド
 * @param 板名,URL,サーバー名
 * @return 書き込み結果
 */
wxString SocketCommunication::PostResponseToThread(URLvsBoardName& boardInfoHash, ThreadInfo& threadInfoHash, const int status) {

/**
   要求メッセージの一例（２回目以降）
   POST /test/bbs.cgi HTTP/1.1
   Host: [サーバー]
   Accept: ＊/＊
   Referer: http://[サーバー]/test/read.cgi/[板名]/[スレッド番号]/
   Accept-Language: ja
   User-Agent: Monazilla/1.00 (ブラウザ名/バージョン)
   Content-Length: ポストするデータの合計サイズ(バイト)
   Cookie:[***] Connection: close

   bbs=[板名]&key=[スレッド番号]&time=[投稿時間]&FROM=[名前]&mail=[メール]&MESSAGE=[本文]&submit=[ボタンの文字]&[不可視項目名]=[不可視項目値]

   =====(以上)=====

   [***] = Cookie: PON=xAjpuk10.tky.hoge.co.jp; HAP=0000000; hana=mogera; PREN=%96%bc%96%b3%82%b5%82%b3%82%f1
*/

     // 不可視項目値をコンフィグファイルから取得する
     InitializeCookie();
     wxString hiddenName = wxT("ERROR"), hiddenVal = wxT("ERROR"), cookie = wxT("ERROR"), pern = wxT("ERROR");
     JaneCloneUtil::GetJaneCloneProperties(wxT("HiddenName"), &hiddenName);
     JaneCloneUtil::GetJaneCloneProperties(wxT("HiddenValue"), &hiddenVal);
     JaneCloneUtil::GetJaneCloneProperties(wxT("Cookie"), &cookie);
     JaneCloneUtil::GetJaneCloneProperties(wxT("PERN"), &pern);

     // 読み取り失敗ならば書込失敗
     if (hiddenName == wxT("ERROR") || hiddenVal == wxT("ERROR") || cookie == wxT("ERROR") || pern == wxT("ERROR"))
	  return FAIL_TO_POST;

     // cookie用の文字列を作成する
     // ";"が最初に出てくる部分を求める
     wxString cookieString = cookie + wxT("; ") + hiddenName + wxT("=") + hiddenVal + wxT("; PREN=") + pern;

     // URLからホスト名を取得する
     wxRegEx reThreadList(_T("(http://)([^/]+)/([^/]+)"), wxRE_ADVANCED + wxRE_ICASE);
     // ホスト名
     wxString hostName;
     if (reThreadList.IsValid()) {
	  if (reThreadList.Matches(boardInfoHash.boardURL)) {
	       hostName = reThreadList.GetMatch(boardInfoHash.boardURL, 2);
	  }
     }

     // 投稿時間を算出する(UNIX Time)
     wxString timeNow = JaneCloneUtil::GetTimeNow();

     wxDir dir(::wxGetHomeDir() + wxFileSeparator + JANECLONE_DIR);
     wxString headerPath = dir.GetName();
     wxDir jcDir(::wxGetHomeDir() + wxFileSeparator + JANECLONE_DIR);
     wxDir datDir(jcDir.GetName() + wxFileSeparator + wxT("dat"));

     // ユーザーのホームディレクトリに隠しフォルダがあるかどうか確認
     if (!dir.HasSubDirs(JANECLONE_DIR)) {
	  ::wxMkdir(jcDir.GetName());
     }

     if (!jcDir.HasSubDirs(wxT("dat"))) {
	  ::wxMkdir(jcDir.GetName() + wxFileSeparator + wxT("dat"));
     }

     if (!datDir.HasSubDirs(wxT("kakikomi"))) {
	  ::wxMkdir(datDir.GetName() + wxFileSeparator + wxT("kakikomi"));
     }

#ifdef __WXMSW__
     // Windowsではパスの区切りは"\"
     headerPath += wxT("\\dat\\");
     headerPath += wxT("kakikomi");
     headerPath += wxT("\\");
     headerPath += timeNow;
     headerPath += wxT(".header");
#else
     // それ以外ではパスの区切りは"/"
     headerPath += wxT("/dat/");
     headerPath += wxT("kakikomi");
     headerPath += wxT("/");
     headerPath += timeNow;
     headerPath += wxT(".header");
#endif

     wxString buttonText = wxT("%8F%91%82%AB%8D%9E%82%DE");

     // Postする内容のデータサイズを取得する
     wxString kakikomiInfo = wxT("bbs=") + boardInfoHash.boardNameAscii + wxT("&key=")
	  + threadInfoHash.origNumber + wxT("&time=") + timeNow + wxT("&FROM=")
	  + postContent->name + wxT("&mail=") + postContent->mail + wxT("&MESSAGE=")
	  + postContent->kakikomi + wxT("&submit=") + buttonText;

     // URL
     const wxString boardURL = boardInfoHash.boardURL;
     // リファラを引数から作成する
     const wxString referer = wxT("http://") + hostName + wxT("/") + boardInfoHash.boardNameAscii + wxT("/");

     // wxHTTPはまだ発展途上のクラスのようで、2012年現在POST用メソッドが安定版に
     // 登録されていないので、代わりにwxSocketClientを使う
     // ソースの統一性を持たせるため、後で全てwxSocketClientに変えるかもしれない
     wxSocketClient* socket = new wxSocketClient();
     socket->SetTimeout(5);

     /**
      * ヘッダを設定する
      */
     wxString header = wxT("");
     // POST
     header += wxT("POST /test/bbs.cgi HTTP/1.1\n");
     // hostname
     header += wxT("Host: ");
     header += hostName;
     header += wxT("\n");
     // Accept
     header += wxT("Accept: */*\n");
     // Referer
     header += wxT("Referer: ");
     header += referer;
     header += wxT("\n");
     // Accept-Language
     header += wxT("Accept-Language: ja\n");
     // User-Agent
     header += wxT("User-Agent: Monazilla/1.00 (JaneClone/0.80)\n");
     // Content-Type
     header += wxT("Content-Type: application/x-www-form-urlencoded\n");
     // Content-Length
     header += wxT("Content-Length: ");
     header += wxString::Format(_("%zd"), kakikomiInfo.Len());
     header += wxT("\n");
     // Cookie
     header += wxT("Cookie: ");
     header += cookieString;
     header += wxT("\n");
     // Connection
     header += wxT("Connection: close\n");
     // POST
     header += wxT("\n");
     header += kakikomiInfo;

     // ホストに接続する
     wxIPV4address* address = new wxIPV4address();
     address->Hostname(hostName);
     address->Service(80);

     // サーバ時間よりローカル端末の時間が進んでいる場合が多いので一度スリープ
     wxSleep(2); // とりあえず２秒

     if (!socket->Connect(*address)) {
	  // 接続失敗
	  *m_logCtrl << wxT("　(ﾟ)(ﾟ) ") << wxT("\n");
	  *m_logCtrl << wxT("彡　　と ＜　書込失敗、ち～ん") << wxT("\n");
	  delete socket;
	  delete address;
	  return FAIL_TO_POST;
     }

     // ヘッダ情報を書き込む
     socket->Write(header.c_str(),header.Len());
     wxString wHeaderLog = wxString::Format(_("Wrote %u out of %zd bytes"),socket->LastCount(), header.Len());
     *m_logCtrl << wHeaderLog << wxT("\n");

     // レスポンスを受け取る
     wxString resPath = headerPath;
     wxFileOutputStream output(headerPath);
     wxDataOutputStream out(output);

     wxInputStream *stream = new wxSocketInputStream(*socket);
     if (!stream) {
	  // ERROR
	  *m_logCtrl << wxT("内部エラー：ストリームの作成に失敗") << wxT("\n");
	  delete stream;
	  return FAIL_TO_POST;
     }

     unsigned char ch[1];
     int byteRead;

     // ストリームを受け取るループ部分
     while (!stream->Eof()) {
	  stream->Read(ch, 1);
	  out.Write8(ch, 1);
	  byteRead = stream->LastRead();
	  if (byteRead < 0) {
	       break;
	  }
     }

     delete stream;
     output.Close();

     // Shift_JIS から UTF-8への変換処理
     wxNKF* nkf = new wxNKF();
     wxString tmpPath = headerPath;
     tmpPath.Replace(wxT(".header"), wxT(".tmp"));
     nkf->Convert(headerPath, tmpPath, wxT("--ic=CP932 --oc=UTF-8"));
     delete nkf;

     // ファイルのリネーム
     wxRenameFile(tmpPath, headerPath);

     return headerPath;
}


/**
 * COOKIE関連の初期化処理を行う
 */
void SocketCommunication::InitializeCookie() {

     // カレントディレクトリを設定
     wxDir dir(::wxGetHomeDir());
     wxDir jcDir(::wxGetHomeDir() + wxFileSeparator + JANECLONE_DIR);
     wxDir propDir(jcDir.GetName() + wxFileSeparator + wxT("prop"));

     // ユーザーのホームディレクトリに隠しフォルダがあるかどうか確認
     if (!dir.HasSubDirs(JANECLONE_DIR)) {
	  ::wxMkdir(jcDir.GetName());
     }

     if (!jcDir.HasSubDirs(wxT("prop"))) {
	  ::wxMkdir(jcDir.GetName() + wxFileSeparator + wxT("prop"));
     }
}
/**
 * 投稿内容をソケット通信クラスに設定する
 * @param PostContent構造体
 */
void SocketCommunication::SetPostContent(PostContent* postContent) {
     this->postContent = postContent;
}
/**
 * COOKIEのデータ書き出しを行う
 */
void SocketCommunication::WriteCookieData(wxString dataFilePath) {

     // HTTPヘッダファイルを読み込む
     InitializeCookie();
     wxTextFile cookieFile;
     cookieFile.Open(dataFilePath, wxConvUTF8);
     wxString str, cookie, hiddenName, hiddenVal;

     // ファイルがオープンされているならば
     if (cookieFile.IsOpened()) {
	  for (str = cookieFile.GetFirstLine(); !cookieFile.Eof(); str = cookieFile.GetNextLine()) {
	       // Set-Cookieに当たる部分を読み取る
	       wxString tmp;
	       if (str.StartsWith(wxT("Set-Cookie: "), &tmp)) {
		    cookie = tmp;
		    tmp.Clear();
	       }

	       if (str.IsEmpty())
		    break;
	  }
     }

     if (cookieFile.IsOpened()) {
	  for (str = cookieFile.GetFirstLine(); !cookieFile.Eof(); str = cookieFile.GetNextLine()) {
	       // hidden要素を抜き出す
	       if (str.Contains(wxT("<html>"))) {
		    // hidden要素正規表現
		    //                                     <input type=hidden name="yuki" value="akari">
		    static const wxRegEx hiddenElement(_T("<input type=hidden name=\"(.+?)\" value=\"(.+?)\">"), wxRE_ADVANCED + wxRE_ICASE);

		    if (hiddenElement.IsValid()) {
			 if (hiddenElement.Matches(str)) {
			      // マッチさせたそれぞれの要素を取得する
			      hiddenName = hiddenElement.GetMatch(str, 1);
			      hiddenVal = hiddenElement.GetMatch(str, 2);
			 }
		    }
	       }	       
	  }
     }

     // クッキー情報をコンフィグファイルに書き出す
     JaneCloneUtil::SetJaneCloneProperties(wxT("Cookie"), cookie);
     JaneCloneUtil::SetJaneCloneProperties(wxT("HiddenName"), hiddenName);
     JaneCloneUtil::SetJaneCloneProperties(wxT("HiddenValue"), hiddenVal);

     cookieFile.Close();
}
/*
 * PERNのデータ書き出しを行う
 */
void SocketCommunication::WritePernData(wxString dataFilePath) {

     // HTTPヘッダファイルを読み込む
     InitializeCookie();
     wxTextFile cookieFile;
     cookieFile.Open(dataFilePath, wxConvUTF8);
     wxString str, pern;

     // ファイルがオープンされているならば
     if (cookieFile.IsOpened()) {
	  for (str = cookieFile.GetFirstLine(); !cookieFile.Eof(); str = cookieFile.GetNextLine()) {
	       // Set-Cookieに当たる部分を読み取る
	       wxString tmp;
	       if (str.StartsWith(wxT("Set-Cookie: PREN="), &tmp)) {
		    pern = tmp;
	       }
	  }
     }

     // ";"が最初に出てくる部分を求める
     int index = pern.Find(wxT(";"));
     wxString tmp = pern.Mid(0, index - 1);
     pern = tmp;

     // クッキー情報をコンフィグファイルに書き出す
     JaneCloneUtil::SetJaneCloneProperties(wxT("PERN"), pern);

     cookieFile.Close();
}
/**
 * 指定されたURLからデータをダウンロードする
 */
void SocketCommunication::DownloadImageFile(const wxString& href, DownloadImageResult* result) {

     // http or ftp
     if (href.StartsWith(wxT("http")) || href.StartsWith(wxT("ttp"))) {
	  // http もしくは ttpの場合
	  DownloadImageFileByHttp(href, result);

     } else if (href.StartsWith(wxT("ftp"))) {
	  // ftp の場合(これってあんまり無くね？)
	  DownloadImageFileByFtp(href, result);

     } else {
	  wxMessageBox(wxT("ダウンロード対象のURLがhttp, ftpいずれでもありません."), wxT("画像ダウンロード"), wxICON_ERROR);
     }
}
/**
 * HTTPでのダウンロード
 */
void SocketCommunication::DownloadImageFileByHttp(const wxString& href, DownloadImageResult* result) {

     // 画像の保存先をコンフィグファイルから取得する
     // デフォルトは $HOME/.jc/cache
     InitializeCookie();
     wxString imageFilePath = ::wxGetHomeDir() + wxFileSeparator + JANECLONE_DIR;

     wxString tmp = wxFileSeparator;
     tmp += wxT("cache");
     JaneCloneUtil::GetJaneCloneProperties(wxT("CachePath"), &tmp);
     imageFilePath += tmp;

     // 画像ファイルパスを決定する
     wxString uuid = JaneCloneUtil::GenerateUUIDString();
     uuid.Replace(wxT("{"), wxT(""), true);
     uuid.Replace(wxT("}"), wxT(""), true);

     wxString ext  = wxT(".") + result->ext;
     imageFilePath += wxFileSeparator;
     imageFilePath += uuid;
     imageFilePath += ext;
     result->imagePath = imageFilePath;

     /** Content-typeの判別 */
     wxString contentType = JaneCloneUtil::DetermineContentType(href);

     /** hostname, pathの検出 */
     PartOfURI* uri = new PartOfURI;
     wxString url = href;
     bool ret = JaneCloneUtil::SubstringURI(url, uri);
     wxString server = uri->hostname;
     wxString path = uri->path;
     wxString msg = wxT("");
     delete uri;

     /** ダウンロード処理の開始 */
     wxHTTP http; 
     http.SetHeader(_T("Content-type"), contentType); 
     http.SetTimeout(10);

     // 保存先を決める
     wxFileOutputStream output(imageFilePath);
     wxDataOutputStream store(output);

     if (http.Connect(server, 80)) {
	  wxInputStream *stream;
	  stream = http.GetInputStream(path);

	  if (stream == NULL) {
	       output.Close();
	       //return -1;
	  } else {
	       unsigned char buffer[1024];
	       int byteRead;

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
     }
}
/**
 * FTPでのダウンロード
 */
void SocketCommunication::DownloadImageFileByFtp(const wxString& href, DownloadImageResult* result) {

     // 画像の保存先をコンフィグファイルから取得する
     // デフォルトは $HOME/.jc/cache
     InitializeCookie();
     wxString imageFilePath = ::wxGetHomeDir();

     wxString tmp = wxFileSeparator;
     tmp += wxT("cache");
     JaneCloneUtil::GetJaneCloneProperties(wxT("CachePath"), &tmp);
     imageFilePath += tmp;

}
/**
 * 指定された新月公開ノードホストにぶら下がるスレッド一覧リストをダウンロードしてくるメソッド
 * もし前回通信した際のログが残っていれば更新の確認のみ行う
 * @param  新月公開ノードのホスト名
 * @return 実行コード
 * 
 * 新月のAPIについては：http://shingetsu.info/saku/api
 */
bool SocketCommunication::DownloadShingetsuThreadList(const wxString& nodeHostname, wxString& outputFilePath) {

     // URIから各パラメーターを抜き取る
     PartOfURI* uri = new PartOfURI;
     bool urlIsSane = JaneCloneUtil::SubstringURI(nodeHostname, uri);
     const wxString protocol = uri->protocol;
     const wxString hostname = uri->hostname;
     wxString port = uri->port;
     //const wxString path = uri->path;
     delete uri;

     // 保存対象のディレクトリが存在するか確かめる
     wxString shingetsu = ::wxGetHomeDir()
	  + wxFileSeparator 
	  + JANECLONE_DIR 
	  + wxFileSeparator 
	  + wxT("shingetsu");

     wxDir chkDir(shingetsu);
     JaneCloneUtil::CreateSpecifyDirectory(chkDir, hostname);
     const std::string outputFilename = 
	  std::string(chkDir.GetName().mb_str()) 
	  + this->separator() 
	  + std::string(hostname.mb_str())
	  + this->separator()
	  + std::string(hostname.mb_str())
	  + ".csv";

     // 元のクラスにファイルパスを伝える
     outputFilePath = wxString(outputFilename.c_str(), wxConvUTF8);

     try {

	  // GETする対象URLを構築
	  std::string requestQuery = std::string(protocol.mb_str())
	       + "://"
	       + std::string(hostname.mb_str())
	       + "/gateway.cgi/csv/index/file,stamp,date,path,uri,type,title,records,size";
	  // portの取得
	  port.Replace(wxT(":"), wxEmptyString, true);
	  int portInteger = 80;
	  if (port != wxEmptyString) {
	       portInteger = wxAtoi(port);
	  }

	  curlpp::Cleanup myCleanup;
	  curlpp::Easy myRequest;
	  myRequest.setOpt(new cURLpp::Options::Url(requestQuery));
	  myRequest.setOpt(new cURLpp::Options::Port(portInteger));

	  std::ofstream ofs(outputFilename , std::ios::out | std::ios::trunc );

	  curlpp::options::WriteStream ws(&ofs);
	  myRequest.setOpt(ws);
	  
	  // ログ出力
	  *m_logCtrl << wxT("新月にアクセス (ん`　 )") << wxString(requestQuery.c_str(), wxConvUTF8) << wxT("\n");
	  myRequest.perform();

	  return true;

     } catch (curlpp::RuntimeError &e) {
	  *m_logCtrl << wxString(e.what(), wxConvUTF8) << wxString(outputFilename.c_str(), wxConvUTF8) << wxT("\n");
	  return false;
     } catch (curlpp::LogicError &e) {
	  *m_logCtrl << wxString(e.what(), wxConvUTF8) << wxString(outputFilename.c_str(), wxConvUTF8) << wxT("\n");
	  return false;
     }

     return false;
}
/**
 * 新月のスレッドをダウンロードしてくるメソッド
 * @param  公開ノードのURL
 * @param  スレッドのタイトル
 * @param  ファイル名
 * @return ダウンロードしたcsvファイルの保存先
 */
wxString SocketCommunication::DownloadShingetsuThread(const wxString& nodeHostname, const wxString& title, const wxString& filename) {

     // URIから各パラメーターを抜き取る
     PartOfURI* uri = new PartOfURI;
     bool urlIsSane = JaneCloneUtil::SubstringURI(nodeHostname, uri);
     const wxString protocol = uri->protocol;
     const wxString hostname = uri->hostname;
     wxString port = uri->port;
     delete uri;

     // 保存対象のディレクトリが存在するか確かめる
     wxString shingetsu = ::wxGetHomeDir()
	  + wxFileSeparator 
	  + JANECLONE_DIR 
	  + wxFileSeparator 
	  + wxT("shingetsu");

     wxDir chkDir(shingetsu);
     JaneCloneUtil::CreateSpecifyDirectory(chkDir, hostname);
     const std::string outputFilename = 
	  std::string(chkDir.GetName().mb_str()) 
	  + this->separator() 
	  + std::string(hostname.mb_str())
	  + this->separator()
	  + std::string(filename.mb_str())
	  + ".dat";

     try {

	  // GETする対象URLを構築
	  std::string requestQuery = std::string(protocol.mb_str())
	       + "://"
	       + std::string(hostname.mb_str())
	       + "/thread.cgi/"
	       + JaneCloneUtil::UrlEncode(title);

	  // portの取得
	  port.Replace(wxT(":"), wxEmptyString, true);
	  int portInteger = 80;
	  if (port != wxEmptyString) {
	       portInteger = wxAtoi(port);
	  }

	  curlpp::Cleanup myCleanup;
	  curlpp::Easy myRequest;
	  myRequest.setOpt(new cURLpp::Options::Url(requestQuery));
	  myRequest.setOpt(new cURLpp::Options::Port(portInteger));

	  std::ofstream ofs(outputFilename , std::ios::out | std::ios::trunc );

	  curlpp::options::WriteStream ws(&ofs);
	  myRequest.setOpt(ws);
	  
	  // ログ出力
	  *m_logCtrl << wxT("新月のスレッドを取得 (ん`　 )") << wxString(requestQuery.c_str(), wxConvUTF8) << wxT("\n");
	  myRequest.perform();

	  return wxString(outputFilename.c_str(), wxConvUTF8);

     } catch (curlpp::RuntimeError &e) {
	  *m_logCtrl << wxString(e.what(), wxConvUTF8) << wxString(outputFilename.c_str(), wxConvUTF8) << wxT("\n");
     } catch (curlpp::LogicError &e) {
	  *m_logCtrl << wxString(e.what(), wxConvUTF8) << wxString(outputFilename.c_str(), wxConvUTF8) << wxT("\n");
     }

     return wxEmptyString;
}

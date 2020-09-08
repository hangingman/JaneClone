/* JaneClone - a text board site viewer for 2ch
 * Copyright (C) 2012-2014 Hiroyuki Nagata
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

#include "socketcommunication.hpp"

const wxString SocketCommunication::properties[] = {
     wxT("ID_NetworkPanelUseProxy")		,/* プロキシを使用するかどうか				*/ 
     wxT("ID_NetworkPanelUseProxyCache")	,/* プロキシでキャッシュを使用するかどうか			*/	  
     wxT("ID_NetworkPanelBasicAuthUserName")	,/* ベーシック認証のユーザー名				*/ 
     wxT("ID_NetworkPanelBasicAuthPassword")	,/* ベーシック認証のパスワード				*/ 
     wxT("ID_NetworkPanelProxyReceiveAddr")	,/* Proxy受信用アドレス					*/ 
     wxT("ID_NetworkPanelProxyReceivePort")	,/* Proxy受信用ポート					*/	   
     wxT("ID_NetworkPanelProxySendAddr")	,/* Proxy送信用アドレス					*/	  
     wxT("ID_NetworkPanelProxySendPort")	,/* Proxy送信用ポート					*/		  
     wxT("ID_NetworkPanelProxySSLAuthAddr")	,/* Proxy SSL認証用アドレス				*/	   
     wxT("ID_NetworkPanelProxySSLAuthPort")	,/* Proxy SSL認証用ポート				*/	   
     wxT("ID_NetworkPanelBoardListURL")		,/* ボード一覧取得URL					*/		  
     wxT("ID_NetworkPanelReceiveBufferSize")	,/* 受信バッファサイズ					*/ 
     wxT("ID_NetworkPanelMaxConn")		,/* 最大接続数						*/ 
     wxT("ID_Receive_Timeout_Sec")	        ,/* 受信タイムアウト秒			                */
     wxT("ID_Connection_Timeout_Sec")		 /* 接続タイムアウト秒			                */
};

/**
 * コンストラクタ
 * プロパティファイルの設定値を取得する
 */
SocketCommunication::SocketCommunication()
{
     this->respBuf.clear();
     this->bodyBuf.clear();
     propMap.clear();

     for ( auto key : properties )
     {
	  wxString val;
	  JaneCloneUtil::GetJaneCloneProperties(key, &val);
	  if ( val != wxEmptyString ) 
	  {
	       propMap[key] = val;
	  }
     }
}
/**
 * 板一覧ファイルをダウンロードしてくるメソッド 引数は板一覧ファイル保存先、板一覧ファイルヘッダ保存先
 */
int SocketCommunication::DownloadBoardList(const wxString& outputPath,
					   const wxString& headerPath) 
{

     // 実行コード
     int rc = 0;
     // 拡張子をgzipに変える
     wxString gzipPath = outputPath;
     gzipPath.Replace(wxT(".html"), wxT(".gzip"));
     // 一時保存用ファイルのパスを設定する
     wxString tmpPath = ::wxGetHomeDir() 
	  + wxFILE_SEP_PATH 
	  + JANECLONE_DIR
	  + wxFILE_SEP_PATH
	  + wxT("dat")
	  + wxFILE_SEP_PATH
	  + wxT("tmp.html");

     if ((!wxFileExists(outputPath)) || (!wxFileExists(headerPath))) 
     {
	  // 解凍された板一覧情報が存在しないor前回の通信ログが残っていないならば通常通りソケット通信を行う
	  rc = DownloadBoardListNew(gzipPath, headerPath);
     } 
     else 
     {
	  // そうでなければ前回の通信の差分を取得しに行く
	  rc = DownloadBoardListMod(gzipPath, headerPath);
     }

     if (wxFile::Exists(gzipPath)) 
     {
	  // gzip拡張子のファイルがあれば、ファイルの解凍・UTF化を行う
	  JaneCloneUtil::DecommpressFile(gzipPath, tmpPath);
	  JaneCloneUtil::ConvertSJISToUTF8(tmpPath, (wxString&) outputPath);
	  // 更新が終わったらgzipファイルを消しておく
	  RemoveTmpFile(gzipPath);
	  RemoveTmpFile(tmpPath);
     }

     return rc;
}

/**
 * 新規に板一覧情報を取得しに行く
 */
int SocketCommunication::DownloadBoardListNew(const wxString& outputPath,
					      const wxString& headerPath) 
{

     // ヘッダの作成
     std::list<std::string> headers;
     headers.push_back("Accept-Encoding: gzip");
     headers.push_back("Host: menu.2ch.net");
     headers.push_back("Accept: ");
     headers.push_back("Referer: http://menu.2ch.net/");
     headers.push_back("Accept-Language: ja");
     headers.push_back("User-Agent: " + userAgent);

     wxString server = wxT("menu.2ch.net");
     wxString path = wxT("/bbsmenu.html");

     const std::string url = std::string(server.mb_str()) + std::string(path.mb_str());

     try {

	  // 保存先を決める
	  curlpp::Cleanup myCleanup;
	  curlpp::Easy myRequest;
	  LoadConfiguration(myRequest, RECV);
	  myRequest.setOpt(new curlpp::options::Url(url));
	  myRequest.setOpt(new curlpp::options::HttpHeader(headers));
	  myRequest.setOpt(new curlpp::options::Verbose(true));
          // ヘッダー用ファンクタを設定する
	  myRequest.setOpt(new curlpp::options::HeaderFunction(
				curlpp::types::WriteFunctionFunctor(this, &SocketCommunication::WriteHeaderData)));

	  // メインのデータ出力
	  std::ofstream ofs(outputPath.mb_str() , std::ios::out | std::ios::trunc | std::ios::binary );
	  curlpp::options::WriteStream ws(&ofs);
	  myRequest.setOpt(ws);

	  wxString message = wxT("2chの板一覧情報を取得 (ん`　 )\n");
	  message += server;
	  message += path;
	  message += wxT("\n");
	  JaneCloneUiUtil::SendLoggingHelper(message);

	  myRequest.perform();

	  // レスポンスヘッダーの書き出し
	  if (!respBuf.empty()) 
	  {
	       std::ofstream ofsHeader(headerPath.mb_str() , std::ios::out | std::ios::trunc );
	       ofsHeader << respBuf << std::endl;
	  }

	  return 0;

     } catch (curlpp::RuntimeError &e) {
	  wxString message = wxString((const char*)e.what(), wxConvUTF8) + wxString((const char*)outputPath.c_str(), wxConvUTF8) + wxT("\n");
	  JaneCloneUiUtil::SendLoggingHelper(message);
     } catch (curlpp::LogicError &e) {
	  wxString message = wxString((const char*)e.what(), wxConvUTF8) + wxString((const char*)outputPath.c_str(), wxConvUTF8) + wxT("\n");
	  JaneCloneUiUtil::SendLoggingHelper(message);
     }

     return -1;
}
/**
 * 前回との差分を取得しに行く
 */
int SocketCommunication::DownloadBoardListMod(const wxString& outputPath,
					      const wxString& headerPath) 
{
     // 最終更新日時をヘッダ情報から取得する
     wxString lastModifiedTime = GetHTTPResponseCode(headerPath,wxT("Last-Modified:"));
     // バイナリとヘッダは前回取得した名前と同じでは困るのでtmpファイルとして作成しておく
     wxString tmpOutputPath = outputPath;
     tmpOutputPath += wxT(".sjis");
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
     headers.push_back("User-Agent: " + userAgent);

     wxString server = wxT("menu.2ch.net");
     wxString path = wxT("/bbsmenu.html");

     const std::string url = std::string(server.mb_str()) + std::string(path.mb_str());

     try {

	  // 保存先を決める
	  curlpp::Cleanup myCleanup;
	  curlpp::Easy myRequest;
	  LoadConfiguration(myRequest, RECV);
	  myRequest.setOpt(new curlpp::options::Url(url));
	  myRequest.setOpt(new curlpp::options::HttpHeader(headers));
	  myRequest.setOpt(new curlpp::options::Verbose(true));
          // ヘッダー用ファンクタを設定する
	  myRequest.setOpt(new curlpp::options::HeaderFunction(
				curlpp::types::WriteFunctionFunctor(this, &SocketCommunication::WriteHeaderData)));

	  std::ofstream ofs(tmpOutputPath.mb_str() , std::ios::out | std::ios::trunc | std::ios::binary );
	  curlpp::options::WriteStream ws(&ofs);
	  myRequest.setOpt(ws);

	  wxString message = wxT("2chの板一覧情報を取得 (ん`　 )\n");
	  message += server;
	  message += path;
	  message += wxT("\n");
	  JaneCloneUiUtil::SendLoggingHelper(message);
	  
	  myRequest.perform();

	  long rc = curlpp::infos::ResponseCode::get(myRequest);
	  message = wxT("HTTP") + wxString::Format(wxT("%lu"), rc) + wxT("\n");
	  JaneCloneUiUtil::SendLoggingHelper(message);
	  
	  if (rc == 304) 
	  {
	       // レスポンスコードが304ならば変更なしなので正常終了
	       RemoveTmpFile(tmpOutputPath);
	       return 0;
	  }

	  // 最後までうまく行った場合
	  RemoveTmpFile(outputPath);
	  // tmpファイルを本物のファイルとする
	  wxRenameFile(tmpOutputPath, outputPath);
	  // レスポンスヘッダーの書き出し
	  if (!respBuf.empty()) 
	  {
	       std::ofstream ofsHeader(headerPath.mb_str() , std::ios::out | std::ios::trunc );
	       ofsHeader << respBuf << std::endl;
	  }
	  
	  return 0;

     } catch (curlpp::RuntimeError &e) {
	  wxString message = wxString((const char*)e.what(), wxConvUTF8) + wxString((const char*)outputPath.c_str(), wxConvUTF8) + wxT("\n");
	  JaneCloneUiUtil::SendLoggingHelper(message);
     } catch (curlpp::LogicError &e) {
	  wxString message = wxString((const char*)e.what(), wxConvUTF8) + wxString((const char*)outputPath.c_str(), wxConvUTF8) + wxT("\n");
	  JaneCloneUiUtil::SendLoggingHelper(message);
     }

     return -1;
}
/**
 * 通信ログに残っているHTTPレスポンスコードを取得する
 */
wxString SocketCommunication::GetHTTPResponseCode(const wxString& headerPath,
						  const wxString& reqCode) {

     wxString resCode = wxT("");

     // ログを読み込む
     wxTextFile file(headerPath);
     file.Open(wxConvUTF8);
     wxString line;

     if (file.IsOpened()) 
     {
	  // ログの中身を1行ずつ走査
	  for (line = file.GetFirstLine(); !file.Eof(); line = file.GetNextLine()) 
	  {
	       if (line.Contains(reqCode)) 
	       {
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
 * @param  板名
 * @param  URL
 * @param  サーバー名
 * @return datファイル保存先
 */
wxString SocketCommunication::DownloadThreadList(wxString& boardName, wxString& boardURL, wxString& boardNameAscii) 
{
     // URLからホスト名を取得する
     wxRegEx reThreadList(_T("(http://)([^/]+)/([^/]+)"), wxRE_ADVANCED + wxRE_ICASE);
     // ホスト名
     wxString hostName;
     if (reThreadList.IsValid()) 
     {
	  if (reThreadList.Matches(boardURL)) 
	  {
	       hostName = reThreadList.GetMatch(boardURL, 2);
	  }
     }

     // 入力された情報が他の掲示板のものであれば変換する
     const bool isShitaraba = SetOtherBoardInfomation(boardNameAscii, hostName, boardURL);
     // 出力するファイルの名前
     const wxString outputFileName = GetOutputFileName(isShitaraba, boardNameAscii);
     // 出力先のファイルパスを設定する
     wxString outputFilePath = GetOutputFilePath(isShitaraba, boardNameAscii);
     outputFilePath += wxFILE_SEP_PATH;
     outputFilePath += outputFileName;

     // gzip用のパスを設定する
     wxString gzipPath = outputFilePath;
     gzipPath.Replace(wxT(".dat"), wxT(".gzip"));
     // 一時保存用のパスを設定する
     wxString tmpPath = outputFilePath;
     if (isShitaraba)
     {
	  tmpPath.Replace(wxT(".dat"), wxT(".eucjp"));
     }
     else
     {
	  tmpPath.Replace(wxT(".dat"), wxT(".sjis"));
     }
     
     // ヘッダーのパスを設定する
     wxString headerPath = outputFilePath;
     headerPath.Replace(wxT(".dat"), wxT(".header"));

     if ((!wxFileExists(outputFilePath)) || (!wxFileExists(headerPath))) 
     {
	  // 解凍された板一覧情報が存在しないor前回の通信ログが残っていないならば通常通りソケット通信を行う
	  DownloadThreadListNew((const wxString) gzipPath,
				(const wxString) headerPath, (const wxString) boardNameAscii,
				(const wxString) hostName, (const wxString) boardURL);
     } 
     else 
     {
	  // そうでなければ前回の通信の差分を取得しに行く
	  DownloadThreadListMod((const wxString) gzipPath,
				(const wxString) headerPath, (const wxString) boardNameAscii,
				(const wxString) hostName, (const wxString) boardURL);
     }

     // gzip拡張子のファイルがあれば、ファイルの解凍・UTF化を行う
     if (wxFile::Exists(gzipPath)) 
     {
	  JaneCloneUtil::DecommpressFile(gzipPath, tmpPath);
     }

     if (wxFile::Exists(tmpPath))
     {
	  if (isShitaraba)
	  {
	       JaneCloneUtil::ConvertEUCJPToUTF8(tmpPath, outputFilePath);
	  }
	  else
	  {
	       JaneCloneUtil::ConvertSJISToUTF8(tmpPath, outputFilePath);
	  }
     }

     // 更新が終わったらgzipファイルを消しておく
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
int SocketCommunication::DownloadThreadListNew(const wxString& gzipPath,
					       const wxString& headerPath, const wxString& boardNameAscii,
					       const wxString& hostName, const wxString& boardURL) 
{

     /**
      * スレッド一覧取得の凡例</br>
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
     headers.push_back("User-Agent: " + userAgent);
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
	  LoadConfiguration(myRequest, RECV);
	  myRequest.setOpt(new curlpp::options::Url(url));
	  myRequest.setOpt(new curlpp::options::HttpHeader(headers));
	  myRequest.setOpt(new curlpp::options::Verbose(true));
          // ヘッダー用ファンクタを設定する
	  myRequest.setOpt(new curlpp::options::HeaderFunction(
				curlpp::types::WriteFunctionFunctor(this, &SocketCommunication::WriteHeaderData)));

	  std::ofstream ofs(gzipPath.mb_str() , std::ios::out | std::ios::trunc | std::ios::binary );
	  curlpp::options::WriteStream ws(&ofs);
	  myRequest.setOpt(ws);

	  wxString message = wxT("スレッド一覧を取得 (ん`　 )\n");
	  message += server;
	  message += path;
	  message += wxT("\n");
	  JaneCloneUiUtil::SendLoggingHelper(message);

	  myRequest.perform();

	  // レスポンスヘッダーの書き出し
	  if (!respBuf.empty()) 
	  {
	       std::ofstream ofsHeader(headerPath.mb_str() , std::ios::out | std::ios::trunc );
	       ofsHeader << respBuf << std::endl;
	  }
	  
	  return 0;

     } catch (curlpp::RuntimeError &e) {
	  wxString message = wxString((const char*)e.what(), wxConvUTF8) + wxString((const char*)gzipPath.c_str(), wxConvUTF8) + wxT("\n");
	  JaneCloneUiUtil::SendLoggingHelper(message);
     } catch (curlpp::LogicError &e) {
	  wxString message = wxString((const char*)e.what(), wxConvUTF8) + wxString((const char*)gzipPath.c_str(), wxConvUTF8) + wxT("\n");
	  JaneCloneUiUtil::SendLoggingHelper(message);
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
int SocketCommunication::DownloadThreadListMod(const wxString& gzipPath,
					       const wxString& headerPath, const wxString& boardNameAscii,
					       const wxString& hostName, const wxString& boardURL) 
{
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
     headers.push_back("User-Agent: " + userAgent);
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
	  LoadConfiguration(myRequest, RECV);
	  myRequest.setOpt(new curlpp::options::Url(url));
	  myRequest.setOpt(new curlpp::options::HttpHeader(headers));
	  myRequest.setOpt(new curlpp::options::Verbose(true));
          // ヘッダー用ファンクタを設定する
	  myRequest.setOpt(new curlpp::options::HeaderFunction(
				curlpp::types::WriteFunctionFunctor(this, &SocketCommunication::WriteHeaderData)));

	  std::ofstream ofs(gzipPath.mb_str() , std::ios::out | std::ios::trunc | std::ios::binary );
	  curlpp::options::WriteStream ws(&ofs);
	  myRequest.setOpt(ws);

	  wxString message = wxT("スレッド一覧を取得 (ん`　 )\n");
	  message += server;
	  message += path;
	  message += wxT("\n");
	  JaneCloneUiUtil::SendLoggingHelper(message);

	  myRequest.perform();

	  // レスポンスヘッダーの書き出し
	  if (!respBuf.empty()) 
	  {
	       std::ofstream ofsHeader(headerPath.mb_str() , std::ios::out | std::ios::trunc );
	       ofsHeader << respBuf << std::endl;
	  }
	  
	  return 0;

     } catch (curlpp::RuntimeError &e) {
	  wxString message = wxString((const char*)e.what(), wxConvUTF8) + wxString((const char*)gzipPath.c_str(), wxConvUTF8) + wxT("\n");
	  JaneCloneUiUtil::SendLoggingHelper(message);
     } catch (curlpp::LogicError &e) {
	  wxString message = wxString((const char*)e.what(), wxConvUTF8) + wxString((const char*)gzipPath.c_str(), wxConvUTF8) + wxT("\n");
	  JaneCloneUiUtil::SendLoggingHelper(message);
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
wxString SocketCommunication::DownloadThread(const wxString& boardName,
					     const wxString& boardURL, const wxString& boardNameAscii,
					     const wxString& origNumber) 
{

     // 出力するファイルの名前
     wxString outputFileName = origNumber + wxT(".dat");
     // 出力先のファイルパスを設定する
     wxString outputFilePath = 
	  ::wxGetHomeDir()
	  + wxFILE_SEP_PATH 
	  + JANECLONE_DIR
	  + wxFILE_SEP_PATH
	  + wxT("dat")
	  + wxFILE_SEP_PATH
	  + boardNameAscii;

     // 保存用フォルダ存在するか確認。無ければフォルダを作成
     if (!wxDir::Exists(outputFilePath)) 
     {
	  ::wxMkdir(outputFilePath);
     }

     outputFilePath += wxFILE_SEP_PATH;
     outputFilePath += outputFileName;

     // gzip用のパスを設定する
     wxString gzipPath = outputFilePath;
     gzipPath.Replace(wxT(".dat"), wxT(".gzip"));
     // 一時保存用のパスを設定する
     wxString tmpPath = outputFilePath;
     tmpPath.Replace(wxT(".dat"), wxT(".sjis"));
     // ヘッダーのパスを設定する
     wxString headerPath = outputFilePath;
     headerPath.Replace(wxT(".dat"), wxT(".header"));

     // URLからホスト名を取得する
     wxRegEx reThreadList(_T("(http://)([^/]+)/([^/]+)"),
			  wxRE_ADVANCED + wxRE_ICASE);
     // ホスト名
     wxString hostName;
     if (reThreadList.IsValid()) 
     {
	  if (reThreadList.Matches(boardURL)) 
	  {
	       hostName = reThreadList.GetMatch(boardURL, 2);
	  }
     }

     if ((!wxFileExists(outputFilePath)) || (!wxFileExists(headerPath))) 
     {
	  // 解凍された板一覧情報が存在しないor前回の通信ログが残っていないならば通常通りソケット通信を行う
	  DownloadThreadNew( gzipPath, headerPath, boardNameAscii, origNumber, hostName);
     } 
     else 
     {
	  // そうでなければ前回の通信の差分を取得しに行く
	  DownloadThreadMod( gzipPath, headerPath, boardNameAscii, origNumber, hostName);
     }

     if (wxFile::Exists(gzipPath)) 
     {
	  // gzip拡張子のファイルがあれば、ファイルの解凍を行う
	  JaneCloneUtil::DecommpressFile(gzipPath, tmpPath);
     }

     if (wxFile::Exists(tmpPath)) 
     {
	  // 対象のファイルがあれば、UTF-8化を行う
	  JaneCloneUtil::ConvertSJISToUTF8(tmpPath, outputFilePath);
     }

     // 更新が終わったらgzipファイルを消しておく
     // SJISファイルは残す（ファイルサイズを知っておくため）
     RemoveTmpFile(gzipPath);

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
void SocketCommunication::DownloadThreadNew(const wxString& gzipPath,
					    const wxString& headerPath, const wxString& boardNameAscii,
					    const wxString& origNumber, const wxString& hostName) 
{

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
	  + boardNameAscii + wxT("/") + origNumber + wxT("/");

     // ヘッダの作成
     std::list<std::string> headers;
     headers.push_back(std::string(getPath.mb_str()) + " HTTP/1.1");
     headers.push_back("Accept-Encoding: gzip");
     headers.push_back("Host: " + std::string(hostName.mb_str()));
     headers.push_back("Accept: */*");
     headers.push_back("Referer: "+ std::string(referer.mb_str()));
     headers.push_back("Accept-Language: ja");
     headers.push_back("User-Agent: " + userAgent);
     headers.push_back("Connection: close");

     wxString server = hostName;
     wxString path = wxT("/") + boardNameAscii + wxT("/dat/") + origNumber + wxT(".dat");
     const std::string url = std::string(server.mb_str()) + std::string(path.mb_str());

     try {

	  // 保存先を決める
	  curlpp::Cleanup myCleanup;
	  curlpp::Easy myRequest;
	  LoadConfiguration(myRequest, RECV);
	  myRequest.setOpt(new curlpp::options::Url(url));
	  myRequest.setOpt(new curlpp::options::HttpHeader(headers));
	  myRequest.setOpt(new curlpp::options::Verbose(true));
          // ヘッダー用ファンクタを設定する
	  myRequest.setOpt(new curlpp::options::HeaderFunction(
				curlpp::types::WriteFunctionFunctor(this, &SocketCommunication::WriteHeaderData)));

	  std::ofstream ofs(gzipPath.mb_str() , std::ios::out | std::ios::trunc | std::ios::binary );
	  curlpp::options::WriteStream ws(&ofs);
	  myRequest.setOpt(ws);

	  wxString message = wxT("2chのスレッドを取得 (ん`　 )\n");
	  message += server;
	  message += path;
	  message += wxT("\n");
	  JaneCloneUiUtil::SendLoggingHelper(message);

	  myRequest.perform();

	  long rc = curlpp::infos::ResponseCode::get(myRequest);
	  
	  if (rc == 200) 
	  {
	       // 通常スレッド取得
	       wxString message = wxT("新規取得 (ヽ´ん`)\n");
	       JaneCloneUiUtil::SendLoggingHelper(message);
	  } 
	  else if (rc == 203) 
	  {
	       // dat落ち確定
	       wxString message = wxT("dat落ちや 彡(゜)(゜) ち〜ん\n");
	       JaneCloneUiUtil::SendLoggingHelper(message);
	       DownloadThreadPast(gzipPath, headerPath, boardNameAscii, origNumber, hostName);
	  } 
	  else if (rc == 302) 
	  {
	       // dat落ちか削除
	       wxString message = wxT("dat落ちか削除済みやな 彡(゜)(゜) ち〜ん\n");
	       JaneCloneUiUtil::SendLoggingHelper(message);
	       DownloadThreadPast(gzipPath, headerPath, boardNameAscii, origNumber, hostName);
	  } 
	  else if (rc == 404) 
	  {
	       // dat落ちか削除
	       wxString message = wxT("サーバが見つからん 彡(゜)(゜) ち〜ん\n");
	       JaneCloneUiUtil::SendLoggingHelper(message);
	  }

	  // レスポンスヘッダーの書き出し
	  if (!respBuf.empty()) 
	  {
	       std::ofstream ofsHeader(headerPath.mb_str() , std::ios::out | std::ios::trunc );
	       ofsHeader << respBuf << std::endl;
	  }

     } catch (curlpp::RuntimeError &e) {
	  wxString message = wxString((const char*)e.what(), wxConvUTF8) + wxString((const char*)gzipPath.c_str(), wxConvUTF8) + wxT("\n");
	  JaneCloneUiUtil::SendLoggingHelper(message);
     } catch (curlpp::LogicError &e) {
	  wxString message = wxString((const char*)e.what(), wxConvUTF8) + wxString((const char*)gzipPath.c_str(), wxConvUTF8) + wxT("\n");
	  JaneCloneUiUtil::SendLoggingHelper(message);
     }
}

/**
 * 前回との差分のスレッドのデータをダウンロードしてくるメソッド
 * @param URL
 * @param サーバー名
 * @param 固有番号
 * @return ダウンロードしたdatファイル保存先
 */
int SocketCommunication::DownloadThreadMod(const wxString& gzipPath,
					   const wxString& headerPath, const wxString& boardNameAscii,
					   const wxString& origNumber, const wxString& hostName) 
{

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
     const wxString referer = wxT("http://") + hostName + wxT("/test/read.cgi/") + boardNameAscii + wxT("/") + origNumber + wxT("/");
     // 最終更新時間を作成する
     const wxString lastModifiedTime = GetHTTPResponseCode(headerPath, wxT("Last-Modified:"));
     // etag
     const wxString etag = GetHTTPResponseCode(headerPath, wxT("ETag:"));
     // 実際のdatファイル（UTF-8）
     wxString datFilePath = headerPath;
     datFilePath.Replace(wxT(".header"), wxT(".dat"));
     // 生のdatファイル（CP932）
     wxString sjisDatPath = headerPath;
     sjisDatPath.Replace(wxT(".header"), wxT(".sjis"));

     wxULongLong fileSize = wxFileName::GetSize(sjisDatPath);

     if (fileSize == wxInvalidSize) 
     {
	  wxMessageBox(wxT("ダウンロード済のdatファイルのサイズが異常です."), wxT("スレッド取得"), wxICON_ERROR);
	  return -1;
     }

     // ヘッダの作成
     std::list<std::string> headers;
     headers.push_back(std::string(getPath.mb_str()) + " HTTP/1.1");
     headers.push_back("Host: " + std::string(hostName.mb_str()));
     headers.push_back("Accept: */*");
     headers.push_back("Referer: "+ std::string(referer.mb_str()));
     headers.push_back("Accept-Language: ja");
     headers.push_back("If-Modified-Since: " + std::string(lastModifiedTime.mb_str()));
     headers.push_back("If-None-Match: " + std::string(etag.mb_str()));
     headers.push_back("Range: bytes= " + std::string(fileSize.ToString().mb_str()) + std::string("-"));
     headers.push_back("User-Agent: " + userAgent);
     headers.push_back("Connection: close");

     wxString server = hostName;
     wxString path = wxT("/") + boardNameAscii + wxT("/dat/") + origNumber + wxT(".dat");
     const std::string url = std::string(server.mb_str()) + std::string(path.mb_str());

     try {

	  // 保存先を決める
	  curlpp::Cleanup myCleanup;
	  curlpp::Easy myRequest;
	  LoadConfiguration(myRequest, RECV);
	  myRequest.setOpt(new curlpp::options::Url(url));
	  myRequest.setOpt(new curlpp::options::HttpHeader(headers));
	  myRequest.setOpt(new curlpp::options::Verbose(true));
          // ヘッダー用ファンクタを設定する
	  myRequest.setOpt(new curlpp::options::HeaderFunction(
				curlpp::types::WriteFunctionFunctor(this, &SocketCommunication::WriteHeaderData)));
          // BODY用ファンクタを設定する
	  myRequest.setOpt(new curlpp::options::WriteFunction(
				curlpp::types::WriteFunctionFunctor(this, &SocketCommunication::WriteDataInternal)));	  

	  wxString message = wxT("2chのスレッドを取得 (ん`　 )\n");
	  message += server;
	  message += path;
	  message += wxT("\n");
	  JaneCloneUiUtil::SendLoggingHelper(message);

	  myRequest.perform();

	  long rc = curlpp::infos::ResponseCode::get(myRequest);

	  if (rc == 200) 
	  {
	       if (!bodyBuf.empty()) 
	       {
		    std::ofstream ofsBody(datFilePath.mb_str() , std::ios::out | std::ios::app );
		    ofsBody << bodyBuf << std::endl;
	       }	       
	  } 
	  else if (rc == 304) 
	  {
	       // レスポンスコードが304ならば変更なし、何もしない
	       wxString message = wxT("更新なし (ヽ´ん`)\n");
	       JaneCloneUiUtil::SendLoggingHelper(message);

	  } 
	  else if (rc == 206) 
	  {
	       // スレッドに更新ありの場合の処理、更新部分を追加する
	       wxString message = wxT("更新あり (ヽ´ん`)\n");
	       JaneCloneUiUtil::SendLoggingHelper(message);

	       if (!bodyBuf.empty()) 
	       {
		    // sjisファイルの更新を実施
		    std::ofstream ofsSjis(sjisDatPath.mb_str(), std::ios::out | std::ios::app );
		    ofsSjis << bodyBuf;
		    // UTF-8に変換
		    wxRemoveFile(datFilePath);
		    JaneCloneUtil::ConvertSJISToUTF8(sjisDatPath, datFilePath);
	       }

	  } 
	  else if (rc == 203) 
	  {
	       // dat落ち確定
	       wxString message = wxT("dat落ちや 彡(゜)(゜) ち〜ん\n");
	       JaneCloneUiUtil::SendLoggingHelper(message);

	       DownloadThreadPast(gzipPath, headerPath, boardNameAscii, origNumber, hostName);

	       // 
	  } 
	  else if (rc == 302) 
	  {
	       // dat落ちか削除
	       wxString message = wxT("dat落ちか削除済みやな 彡(゜)(゜) ち〜ん\n");
	       JaneCloneUiUtil::SendLoggingHelper(message);
	       DownloadThreadPast(gzipPath, headerPath, boardNameAscii, origNumber, hostName);

	       // 
	  } 
	  else if (rc == 404) 
	  {
	       // dat落ちか削除
	       wxString message = wxT("サーバが見つからん 彡(゜)(゜) ち〜ん\n");
	       JaneCloneUiUtil::SendLoggingHelper(message);
	  }

	  // レスポンスヘッダーの書き出し
	  if (!respBuf.empty()) 
	  {
	       std::ofstream ofsHeader(headerPath.mb_str() , std::ios::out | std::ios::trunc );
	       ofsHeader << respBuf << std::endl;
	  }

	  return 0;

     } catch (curlpp::RuntimeError &e) {
	  wxString message = wxString((const char*)e.what(), wxConvUTF8) + wxString((const char*)datFilePath.mb_str(), wxConvUTF8) + wxT("\n");
	  JaneCloneUiUtil::SendLoggingHelper(message);
     } catch (curlpp::LogicError &e) {
	  wxString message = wxString((const char*)e.what(), wxConvUTF8) + wxString((const char*)datFilePath.mb_str(), wxConvUTF8) + wxT("\n");
	  JaneCloneUiUtil::SendLoggingHelper(message);
     }

     return -1;
}
/**
 * 過去のスレッドのデータをダウンロードしてくるメソッド
 * @param gzipのダウンロード先パス
 * @param HTTPヘッダのダウンロード先パス
 * @param 板名（ascii）
 * @param 固有番号
 * @param サーバーのホスト名
 * @param 要求するファイルの拡張子
 * @return 実行コード
 */
int SocketCommunication::DownloadThreadPast(const wxString& gzipPath, const wxString& headerPath,
					    const wxString& boardNameAscii, const wxString& origNumber,
					    const wxString& hostName, const wxString& ext) {

/**
 * dat落ちしたスレッドは、一定期間を経て過去ログ倉庫に収められます。（過去ログ化）
 * 過去ログ倉庫の格納場所は次の通りです。
 * ・スレッド番号が10桁の場合
 * http://[サーバー]/[板名]/kako/[スレッド番号（上4桁）]/[スレッド番号（上5桁）]/[スレッド番号].dat.gz
 * ・スレッド番号が9桁の場合
 * http://[サーバー]/[板名]/kako/[スレッド番号（上3桁）]/[スレッド番号].dat.gz
 * ・[スレッド番号].dat.gzが無い場合、[スレッド番号].datで要求すると取得できる場合があります。
 */
     wxString server = hostName;
     wxString path;
     
     if (origNumber.Len() == 10) 
     {
	  path += wxT("/");
	  path += boardNameAscii; 
	  path += wxT("/kako/");
	  path += origNumber.Mid(0, 4);
	  path += wxT("/");
	  path += origNumber.Mid(0, 5);
	  path += wxT("/");
	  path += origNumber;
	  path += ext;
     } 
     else if (origNumber.Len() == 9) 
     {
	  path += wxT("/");
	  path += boardNameAscii; 
	  path += wxT("/kako/");
	  path += origNumber.Mid(0, 3);
	  path += wxT("/");
	  path += origNumber;
	  path += ext;
     } 
     else 
     {
	  // エラー
	  return -1;
     }
     
     // 取得先のパスを引数から作成する
     const wxString getPath = wxT("GET ") + path;
     // リファラを引数から作成する
     const wxString referer = wxT("http://") + hostName + wxT("/test/read.cgi/") + boardNameAscii + wxT("/") + origNumber;
     // 取得するURLの構築
     const std::string url = std::string(server.mb_str()) + std::string(path.mb_str());
     // ヘッダの作成
     std::list<std::string> headers;
     headers.push_back(std::string(getPath.mb_str()) + ": HTTP/1.1");
     headers.push_back("Accept-Encoding: gzip");
     headers.push_back("Host: " + std::string(hostName.mb_str()));
     headers.push_back("Accept: */*");
     headers.push_back("Referer: "+ std::string(referer.mb_str()));
     headers.push_back("Accept-Language: ja");
     headers.push_back("User-Agent: " + userAgent);
     headers.push_back("Connection: close");

     try {

	  // 保存先を決める
	  curlpp::Cleanup myCleanup;
	  curlpp::Easy myRequest;
	  LoadConfiguration(myRequest, RECV);
	  myRequest.setOpt(new curlpp::options::Url(url));
	  myRequest.setOpt(new curlpp::options::HttpHeader(headers));
	  myRequest.setOpt(new curlpp::options::Verbose(true));
          // ヘッダー用ファンクタを設定する
	  myRequest.setOpt(new curlpp::options::HeaderFunction(
				curlpp::types::WriteFunctionFunctor(this, &SocketCommunication::WriteHeaderData)));

	  // 何も見つからなかった時に備えてgzipPathは仮の値に
	  const wxString pastTempPath = gzipPath + wxT("temp");
	  
	  std::ofstream ofs(pastTempPath.mb_str() , std::ios::out | std::ios::trunc | std::ios::binary );
	  curlpp::options::WriteStream ws(&ofs);
	  myRequest.setOpt(ws);

	  wxString message = wxT("2chのスレッド(過去スレ)を取得 (ん`　 )\n");
	  message += server;
	  message += path;
	  message += wxT("\n");
	  JaneCloneUiUtil::SendLoggingHelper(message);

	  // 過去スレ取得は２回目のクラスメソッド起動になるはずなので
	  // 文字列をクリアーしておく
	  this->respBuf.clear();
	  this->bodyBuf.clear();
	  myRequest.perform();

	  long rc = curlpp::infos::ResponseCode::get(myRequest);
	  
	  if (rc == 200) 
	  {
	       // 通常スレッド取得
	       wxString message = wxT("新規取得 (ヽ´ん`)\n");
	       JaneCloneUiUtil::SendLoggingHelper(message);

	       // ファイルのリネーム
	       wxRenameFile(pastTempPath, gzipPath);
	  } 
	  else if (rc == 203) 
	  {
	       // dat落ち確定
	       wxString message = wxT("dat落ちや 彡(゜)(゜) ち〜ん\n");
	       JaneCloneUiUtil::SendLoggingHelper(message);
	  } 
	  else if (rc == 302) 
	  {
	       // dat落ちか削除
	       wxString message = wxT("dat落ちか削除済みやな 彡(゜)(゜) ち〜ん\n");
	       JaneCloneUiUtil::SendLoggingHelper(message);
	  } 
	  else if (rc == 404) 
	  {
	       // dat落ちか削除
	       wxString message = wxT("サーバが見つからん 彡(゜)(゜) ち〜ん\n");
	       JaneCloneUiUtil::SendLoggingHelper(message);
	  }

	  // レスポンスヘッダーの書き出し
	  if (!respBuf.empty()) 
	  {
	       std::ofstream ofsHeader(headerPath.mb_str() , std::ios::out | std::ios::trunc );
	       ofsHeader << respBuf << std::endl;
	  }

	  // 一時ファイル削除
	  RemoveTmpFile(pastTempPath);

	  return 0;

     } catch (curlpp::RuntimeError &e) {
	  wxString message = wxString((const char*)e.what(), wxConvUTF8) + wxString((const char*)gzipPath.c_str(), wxConvUTF8) + wxT("\n");
	  JaneCloneUiUtil::SendLoggingHelper(message);
     } catch (curlpp::LogicError &e) {
	  wxString message = wxString((const char*)e.what(), wxConvUTF8) + wxString((const char*)gzipPath.c_str(), wxConvUTF8) + wxT("\n");
	  JaneCloneUiUtil::SendLoggingHelper(message);
     }
     
     return -1;
}
/**
 * 一時ファイルを消す
 */
void SocketCommunication::RemoveTmpFile(const wxString& removeFile) 
{
     if (wxFile::Exists(removeFile)) 
     {
	  wxRemoveFile(removeFile);
     }
}
/**
 * HTTPヘッダを書きだす
 */
size_t SocketCommunication::WriteHeaderData(char *ptr, size_t size, size_t nmemb) 
{

     // 文字列をメンバ変数に格納
     size_t realsize = size * nmemb;
     std::string line(static_cast<const char *>(ptr), realsize);

     // ログに出力する
     if (std::string::npos != line.find("HTTP")) 
     {
	  wxString message = wxString(line.c_str(), wxConvUTF8) + wxT("\n");
	  JaneCloneUiUtil::SendLoggingHelper(message);
     }
     
     // クッキーを書き出す
     if (std::string::npos != line.find("Set-Cookie:")) 
     {
	  wxString cookie;
	  wxString(line.c_str(), wxConvUTF8).StartsWith(wxT("Set-Cookie: "), &cookie);

	  // 2chからもらえるCookieは複数ある
	  if (cookie.Contains(wxT("PON"))) 
	  {
	       JaneCloneUtil::SetJaneCloneProperties(wxT("Cookie-PON"), cookie);
	  } 
	  else if (cookie.Contains(wxT("HAP"))) 
	  {
	       JaneCloneUtil::SetJaneCloneProperties(wxT("Cookie-HAP"), cookie);
	  } 
	  else if (cookie.Contains(wxT("DMDM"))) 
	  {
	       JaneCloneUtil::SetJaneCloneProperties(wxT("DMDM"), cookie);
	  } 
	  else if (cookie.Contains(wxT("MDMD"))) 
	  {
	       JaneCloneUtil::SetJaneCloneProperties(wxT("MDMD"), cookie);
	  } 
	  else 
	  {
	       JaneCloneUtil::SetJaneCloneProperties(wxT("PREN"), cookie);
	  }
     }

     respBuf.append(line);
     return realsize;
}
/**
 * HTTPボディを書きだす
 */
size_t SocketCommunication::WriteDataInternal(char *ptr, size_t size, size_t nmemb) 
{
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
wxString SocketCommunication::PostFirstToThread(URLvsBoardName& boardInfoHash, ThreadInfo& threadInfoHash, const int status) 
{

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
     if (reThreadList.IsValid()) 
     {
	  if (reThreadList.Matches(boardInfoHash.boardURL)) 
	  {
	       hostName = reThreadList.GetMatch(boardInfoHash.boardURL, 2);
	  }
     }

     // 投稿時間を算出する(UNIX Time)
     wxString timeNow = JaneCloneUtil::GetTimeNow();
     wxString message = wxT("UNIX Time:") + timeNow;
     JaneCloneUiUtil::SendLoggingHelper(message);

     wxDir dir(::wxGetHomeDir() + wxFILE_SEP_PATH + JANECLONE_DIR);
     wxString headerPath = dir.GetName();
     wxDir jcDir(::wxGetHomeDir() + wxFILE_SEP_PATH + JANECLONE_DIR);
     wxDir datDir(jcDir.GetName() + wxFILE_SEP_PATH + wxT("dat"));

     // ユーザーのホームディレクトリに隠しフォルダがあるかどうか確認
     if (!jcDir.HasSubDirs(wxT("dat"))) 
     {
	  ::wxMkdir(jcDir.GetName() + wxFILE_SEP_PATH + wxT("dat"));
     }

     if (!datDir.HasSubDirs(wxT("kakikomi"))) 
     {
	  ::wxMkdir(datDir.GetName() + wxFILE_SEP_PATH + wxT("kakikomi"));
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

     // 接続先
     std::string url(hostName.mb_str());
     url += "/test/bbs.cgi";
     // URL
     const wxString boardURL = boardInfoHash.boardURL;
     // リファラを引数から作成する
     const wxString referer = wxT("http://") + hostName + wxT("/") + boardInfoHash.boardNameAscii + wxT("/");
     // ヘッダのサイズ
     std::ostringstream stream;
     stream << kakikomiInfo.Len();
     const std::string kakikomiSize = stream.str();
     
     /**
      * ヘッダを設定する
      */
     std::list<std::string> headers;
     headers.push_back("POST /test/bbs.cgi HTTP/1.1");
     headers.push_back("Host: " + std::string(hostName.mb_str()));
     headers.push_back("Accept: */*");
     headers.push_back("Referer: " + std::string(referer.mb_str()));
     headers.push_back("Accept-Language: ja");
     headers.push_back("User-Agent: " + userAgent);
     headers.push_back("Content-Length: " + kakikomiSize);
     headers.push_back("Content-Type: application/x-www-form-urlencoded");
     headers.push_back("Connection: close");

     try {

	  // 保存先を決める
	  curlpp::Cleanup myCleanup;
	  curlpp::Easy myRequest;
	  LoadConfiguration(myRequest, SEND);
	  myRequest.setOpt(new curlpp::options::Url(url));
	  myRequest.setOpt(new curlpp::options::HttpHeader(headers));
	  const std::string postField = std::string(kakikomiInfo.mb_str()); 
	  myRequest.setOpt(new curlpp::options::PostFields(postField)); 
	  myRequest.setOpt(new curlpp::options::PostFieldSize(postField.length()));
	  myRequest.setOpt(new curlpp::options::Verbose(true));
	  // ヘッダー用ファンクタを設定する
	  myRequest.setOpt(new curlpp::options::HeaderFunction(
				curlpp::types::WriteFunctionFunctor(this, &SocketCommunication::WriteHeaderData)));

	  std::ofstream ofs(headerPath.mb_str() , std::ios::out | std::ios::trunc );
	  curlpp::options::WriteStream ws(&ofs);
	  myRequest.setOpt(ws);

	  wxString message = wxT("書き込み実行 (ヽ´ん`)\n");
	  JaneCloneUiUtil::SendLoggingHelper(message);

	  // 文字列をクリアーしておく
	  this->respBuf.clear();
	  this->bodyBuf.clear();

	  wxSleep(2);
	  myRequest.perform();

     } catch (curlpp::RuntimeError &e) {
	  wxString message = wxString((const char*)e.what(), wxConvUTF8) + wxString((const char*)headerPath.c_str(), wxConvUTF8) + wxT("\n");
	  JaneCloneUiUtil::SendLoggingHelper(message);
     } catch (curlpp::LogicError &e) {
	  wxString message = wxString((const char*)e.what(), wxConvUTF8) + wxString((const char*)headerPath.c_str(), wxConvUTF8) + wxT("\n");
	  JaneCloneUiUtil::SendLoggingHelper(message);
     }

     // Shift_JIS から UTF-8への変換処理
     wxString tmpPath = headerPath;
     tmpPath.Replace(wxT(".header"), wxT(".tmp"));
     JaneCloneUtil::ConvertSJISToUTF8(headerPath, tmpPath);
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
wxString SocketCommunication::PostConfirmToThread(URLvsBoardName& boardInfoHash, ThreadInfo& threadInfoHash, const int status)
{

/**
 *  要求メッセージの一例（初回投稿時・２回目）
 *  POST /test/bbs.cgi HTTP/1.1
 *  Host: [サーバー]
 *  Accept: ＊/＊
 *  Referer: http://[サーバー]/test/read.cgi/[板名]/[スレッド番号]/
 *  Accept-Language: ja
 *  User-Agent: Monazilla/1.00 (ブラウザ名/バージョン)
 *  Content-Length: ポストするデータの合計サイズ(バイト)
 *  Cookie:応答ヘッダのSet-Cookieに記述された内容 Connection: close
 * 
 *  bbs=[板名]&key=[スレッド番号]&time=[投稿時間]&FROM=[名前]&mail=[メール]&MESSAGE=[本文]&submit=[ボタンの文字]&[不可視項目名]=[不可視項目値]
 */

     // 不可視項目値をコンフィグファイルから取得する
     InitializeCookie();
     wxString hiddenName = wxT("ERROR"), hiddenVal = wxT("ERROR");
     JaneCloneUtil::GetJaneCloneProperties(wxT("HiddenName"), &hiddenName);
     JaneCloneUtil::GetJaneCloneProperties(wxT("HiddenValue"), &hiddenVal);

     // Cookie値を構築する
     wxString cookie;
     AssembleCookie(cookie, hiddenName, hiddenVal);

     // 読み取り失敗ならば書込失敗
     if (hiddenName == wxT("ERROR") || hiddenVal == wxT("ERROR"))
	  return FAIL_TO_POST;

     // URLからホスト名を取得する
     wxRegEx reThreadList(_T("(http://)([^/]+)/([^/]+)"), wxRE_ADVANCED + wxRE_ICASE);
     // ホスト名
     wxString hostName;
     if (reThreadList.IsValid()) 
     {
	  if (reThreadList.Matches(boardInfoHash.boardURL)) 
	  {
	       hostName = reThreadList.GetMatch(boardInfoHash.boardURL, 2);
	  }
     }

     // 投稿時間を算出する(UNIX Time)
     wxString timeNow = JaneCloneUtil::GetTimeNow();
     wxString message = wxT("UNIX Time:") + timeNow;
     JaneCloneUiUtil::SendLoggingHelper(message);

     wxDir dir(::wxGetHomeDir() + wxFILE_SEP_PATH + JANECLONE_DIR);
     wxString headerPath = dir.GetName();
     wxDir jcDir(::wxGetHomeDir() + wxFILE_SEP_PATH + JANECLONE_DIR);
     wxDir datDir(jcDir.GetName() + wxFILE_SEP_PATH + wxT("dat"));

     if (!jcDir.HasSubDirs(wxT("dat"))) 
     {
	  ::wxMkdir(jcDir.GetName() + wxFILE_SEP_PATH + wxT("dat"));
     }

     if (!datDir.HasSubDirs(wxT("kakikomi"))) 
     {
	  ::wxMkdir(datDir.GetName() + wxFILE_SEP_PATH + wxT("kakikomi"));
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

     // 接続先
     std::string url(hostName.mb_str());
     url += "/test/bbs.cgi";
     // URL
     const wxString boardURL = boardInfoHash.boardURL;
     // リファラを引数から作成する
     const wxString referer = wxT("http://") + hostName + wxT("/") + boardInfoHash.boardNameAscii + wxT("/");
     // ヘッダのサイズ
     std::ostringstream stream;
     stream << kakikomiInfo.Len();
     const std::string kakikomiSize = stream.str();
     
     /**
      * ヘッダを設定する
      */
     std::list<std::string> headers;
     headers.push_back("POST /test/bbs.cgi HTTP/1.1");
     headers.push_back("Host: " + std::string(hostName.mb_str()));
     headers.push_back("Accept: */*");
     headers.push_back("Referer: " + std::string(referer.mb_str()));
     headers.push_back("Accept-Language: ja");
     headers.push_back("User-Agent: " + userAgent);
     headers.push_back("Content-Length: " + kakikomiSize);
     headers.push_back("Content-Type: application/x-www-form-urlencoded");
     headers.push_back("Cookie: " + std::string(cookie.mb_str()));
     headers.push_back("Connection: close");

     try {

	  // 保存先を決める
	  curlpp::Cleanup myCleanup;
	  curlpp::Easy myRequest;
	  LoadConfiguration(myRequest, SEND);
	  myRequest.setOpt(new curlpp::options::Url(url));
	  myRequest.setOpt(new curlpp::options::HttpHeader(headers));
	  const std::string postField = std::string(kakikomiInfo.mb_str()); 
	  myRequest.setOpt(new curlpp::options::PostFields(postField)); 
	  myRequest.setOpt(new curlpp::options::PostFieldSize(postField.length()));
	  myRequest.setOpt(new curlpp::options::Verbose(true));
	  // ヘッダー用ファンクタを設定する
	  myRequest.setOpt(new curlpp::options::HeaderFunction(
				curlpp::types::WriteFunctionFunctor(this, &SocketCommunication::WriteHeaderData)));

	  std::ofstream ofs(headerPath.mb_str() , std::ios::out | std::ios::trunc );
	  curlpp::options::WriteStream ws(&ofs);
	  myRequest.setOpt(ws);

	  wxString message = wxT("書き込み実行 (ヽ´ん`)\n");
	  JaneCloneUiUtil::SendLoggingHelper(message);

	  // 文字列をクリアーしておく
	  this->respBuf.clear();
	  this->bodyBuf.clear();

	  wxSleep(2);
	  myRequest.perform();

     } catch (curlpp::RuntimeError &e) {
	  wxString message = wxString((const char*)e.what(), wxConvUTF8) + wxString((const char*)headerPath.c_str(), wxConvUTF8) + wxT("\n");
	  JaneCloneUiUtil::SendLoggingHelper(message);
     } catch (curlpp::LogicError &e) {
	  wxString message = wxString((const char*)e.what(), wxConvUTF8) + wxString((const char*)headerPath.c_str(), wxConvUTF8) + wxT("\n");
	  JaneCloneUiUtil::SendLoggingHelper(message);
     }

     // Shift_JIS から UTF-8への変換処理
     wxString tmpPath = headerPath;
     tmpPath.Replace(wxT(".header"), wxT(".tmp"));
     JaneCloneUtil::ConvertSJISToUTF8(headerPath, tmpPath);

     // ファイルのリネーム
     wxRenameFile(tmpPath, headerPath);

     return headerPath;
}
/**
 * 通常の書き込みメソッド
 * @param 板名,URL,サーバー名
 * @return 書き込み結果
 */
wxString SocketCommunication::PostResponseToThread(URLvsBoardName& boardInfoHash, ThreadInfo& threadInfoHash, const int status) 
{

/**
 *  要求メッセージの一例（２回目以降）
 *  POST /test/bbs.cgi HTTP/1.1
 *  Host: [サーバー]
 *  Accept: ＊/＊
 *  Referer: http://[サーバー]/test/read.cgi/[板名]/[スレッド番号]/
 *  Accept-Language: ja
 *  User-Agent: Monazilla/1.00 (ブラウザ名/バージョン)
 *  Content-Length: ポストするデータの合計サイズ(バイト)
 *  Cookie:[***] Connection: close
 * 
 *  bbs=[板名]&key=[スレッド番号]&time=[投稿時間]&FROM=[名前]&mail=[メール]&MESSAGE=[本文]&submit=[ボタンの文字]&[不可視項目名]=[不可視項目値]
 * 
 *  =====(以上)=====
 * 
 *  [***] = Cookie: PON=xAjpuk10.tky.hoge.co.jp; HAP=0000000; hana=mogera; PREN=%96%bc%96%b3%82%b5%82%b3%82%f1
 */

     // 不可視項目値をコンフィグファイルから取得する
     InitializeCookie();
     wxString hiddenName = wxT("ERROR"), hiddenVal = wxT("ERROR");
     JaneCloneUtil::GetJaneCloneProperties(wxT("HiddenName"), &hiddenName);
     JaneCloneUtil::GetJaneCloneProperties(wxT("HiddenValue"), &hiddenVal);

     // Cookie値を構築する
     wxString cookie;
     AssembleCookie(cookie, hiddenName, hiddenVal);

     // 読み取り失敗ならば書込失敗
     if (hiddenName == wxT("ERROR") || hiddenVal == wxT("ERROR"))
	  return FAIL_TO_POST;

     // URLからホスト名を取得する
     wxRegEx reThreadList(_T("(http://)([^/]+)/([^/]+)"), wxRE_ADVANCED + wxRE_ICASE);
     // ホスト名
     wxString hostName;
     if (reThreadList.IsValid()) 
     {
	  if (reThreadList.Matches(boardInfoHash.boardURL)) 
	  {
	       hostName = reThreadList.GetMatch(boardInfoHash.boardURL, 2);
	  }
     }

     // 投稿時間を算出する(UNIX Time)
     wxString timeNow = JaneCloneUtil::GetTimeNow();
     wxString message = wxT("UNIX Time:") + timeNow;
     JaneCloneUiUtil::SendLoggingHelper(message);

     wxDir dir(::wxGetHomeDir() + wxFILE_SEP_PATH + JANECLONE_DIR);
     wxString headerPath = dir.GetName();
     wxDir jcDir(::wxGetHomeDir() + wxFILE_SEP_PATH + JANECLONE_DIR);
     wxDir datDir(jcDir.GetName() + wxFILE_SEP_PATH + wxT("dat"));

     // ユーザーのホームディレクトリに隠しフォルダがあるかどうか確認
     if (!jcDir.HasSubDirs(wxT("dat"))) 
     {
	  ::wxMkdir(jcDir.GetName() + wxFILE_SEP_PATH + wxT("dat"));
     }

     if (!datDir.HasSubDirs(wxT("kakikomi"))) 
     {
	  ::wxMkdir(datDir.GetName() + wxFILE_SEP_PATH + wxT("kakikomi"));
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

     // 接続先
     std::string url(hostName.mb_str());
     url += "/test/bbs.cgi";
     // URL
     const wxString boardURL = boardInfoHash.boardURL;
     // リファラを引数から作成する
     const wxString referer = wxT("http://") + hostName + wxT("/") + boardInfoHash.boardNameAscii + wxT("/");
     // ヘッダのサイズ
     std::ostringstream stream;
     stream << kakikomiInfo.Len();
     const std::string kakikomiSize = stream.str();
     
     /**
      * ヘッダを設定する
      */
     std::list<std::string> headers;
     headers.push_back("POST /test/bbs.cgi HTTP/1.1");
     headers.push_back("Host: " + std::string(hostName.mb_str()));
     headers.push_back("Accept: */*");
     headers.push_back("Referer: " + std::string(referer.mb_str()));
     headers.push_back("Accept-Language: ja");
     headers.push_back("User-Agent: " + userAgent);
     headers.push_back("Content-Length: " + kakikomiSize);
     headers.push_back("Content-Type: application/x-www-form-urlencoded");
     headers.push_back("Cookie: " + std::string(cookie.mb_str()));
     headers.push_back("Connection: close");

     try {

	  // 保存先を決める
	  curlpp::Cleanup myCleanup;
	  curlpp::Easy myRequest;
	  LoadConfiguration(myRequest, SEND);
	  myRequest.setOpt(new curlpp::options::Url(url));
	  myRequest.setOpt(new curlpp::options::HttpHeader(headers));
	  const std::string postField = std::string(kakikomiInfo.mb_str()); 
	  myRequest.setOpt(new curlpp::options::PostFields(postField)); 
	  myRequest.setOpt(new curlpp::options::PostFieldSize(postField.length()));
	  myRequest.setOpt(new curlpp::options::Verbose(true));
	  // ヘッダー用ファンクタを設定する
	  myRequest.setOpt(new curlpp::options::HeaderFunction(
				curlpp::types::WriteFunctionFunctor(this, &SocketCommunication::WriteHeaderData)));

	  std::ofstream ofs(headerPath.mb_str() , std::ios::out | std::ios::trunc );
	  curlpp::options::WriteStream ws(&ofs);
	  myRequest.setOpt(ws);

	  wxString message = wxT("書き込み実行 (ヽ´ん`)\n");
	  JaneCloneUiUtil::SendLoggingHelper(message);

	  // 文字列をクリアーしておく
	  this->respBuf.clear();
	  this->bodyBuf.clear();

	  wxSleep(2);
	  myRequest.perform();

     } catch (curlpp::RuntimeError &e) {
	  wxString message = wxString((const char*)e.what(), wxConvUTF8) + wxString((const char*)headerPath.c_str(), wxConvUTF8) + wxT("\n");
	  JaneCloneUiUtil::SendLoggingHelper(message);
     } catch (curlpp::LogicError &e) {
	  wxString message = wxString((const char*)e.what(), wxConvUTF8) + wxString((const char*)headerPath.c_str(), wxConvUTF8) + wxT("\n");
	  JaneCloneUiUtil::SendLoggingHelper(message);
     }

     // Shift_JIS から UTF-8への変換処理
     wxString tmpPath = headerPath;
     tmpPath.Replace(wxT(".header"), wxT(".tmp"));
     JaneCloneUtil::ConvertSJISToUTF8(headerPath, tmpPath);

     // ファイルのリネーム
     wxRenameFile(tmpPath, headerPath);

     return headerPath;
}
/**
 * COOKIE関連の初期化処理を行う
 */
void SocketCommunication::InitializeCookie() 
{
     // カレントディレクトリを設定
     wxDir dir(::wxGetHomeDir());
     wxDir jcDir(::wxGetHomeDir() + wxFILE_SEP_PATH + JANECLONE_DIR);
     wxDir propDir(jcDir.GetName() + wxFILE_SEP_PATH + wxT("prop"));

     // ユーザーのホームディレクトリに隠しフォルダがあるかどうか確認
     if (!dir.HasSubDirs(JANECLONE_DIR)) 
     {
	  ::wxMkdir(jcDir.GetName());
     }

     if (!jcDir.HasSubDirs(wxT("prop"))) 
     {
	  ::wxMkdir(jcDir.GetName() + wxFILE_SEP_PATH + wxT("prop"));
     }
}
/**
 * COOKIE文字列の連結処理を行う
 */
void SocketCommunication::AssembleCookie(wxString& cookie, const wxString& hiddenName, const wxString& hiddenVal) 
{
     // まずクリア
     cookie.Clear();

     // BEログインチェック
     wxString widgetsName = wxEmptyString;
     bool     widgetsInfo = false;
     const std::string &str = EnumString<JANECLONE_ENUMS>::From( static_cast<JANECLONE_ENUMS>(ID_ResponseWindowBeChk) );
     widgetsName = wxString((const char*)str.c_str(), wxConvUTF8);
     JaneCloneUtil::GetJaneCloneProperties(widgetsName, &widgetsInfo);
     if (widgetsInfo) 
     {
	  // BEログインして書き込む場合
	  wxString dmdm = wxEmptyString;
	  wxString mdmd = wxEmptyString;
	  JaneCloneUtil::GetJaneCloneProperties(wxT("DMDM"), &dmdm);
	  JaneCloneUtil::GetJaneCloneProperties(wxT("MDMD"), &mdmd);
	  
	  if ( dmdm.Len() == 0 || mdmd.Len() == 0 ) 
	  {
	       // クッキーがないのでログインしてクッキー☆をもらう
	       LoginBe2ch();
	       JaneCloneUtil::GetJaneCloneProperties(wxT("DMDM"), &dmdm);
	       JaneCloneUtil::GetJaneCloneProperties(wxT("MDMD"), &mdmd);
	       SubstringCookie(dmdm);
	       SubstringCookie(mdmd);

	       if (dmdm.Len() != 0 && mdmd.Len() != 0) 
	       {
		    cookie.Append(dmdm);
		    cookie.Append(wxT("; "));
		    cookie.Append(mdmd);
		    cookie.Append(wxT("; "));
	       }
	       
	  } 
	  else 
	  {
	       // クッキーがあるのでそのまま処理する
	       SubstringCookie(dmdm);
	       SubstringCookie(mdmd);

	       if (dmdm.Len() != 0 && mdmd.Len() != 0) 
	       {
		    cookie.Append(dmdm);
		    cookie.Append(wxT("; "));
		    cookie.Append(mdmd);
		    cookie.Append(wxT("; "));
	       }
	  }
     } /** BEログインチェック処理終わり */

     // Cookie-PONの取得
     wxString cookiePon;
     JaneCloneUtil::GetJaneCloneProperties(wxT("Cookie-PON"), &cookiePon);
     SubstringCookie(cookiePon);
     
     // Cookie-HAPの取得
     wxString cookieHap;
     JaneCloneUtil::GetJaneCloneProperties(wxT("Cookie-HAP"), &cookieHap);
     SubstringCookie(cookieHap);

     // 隠し要素の取得
     wxString hidden = hiddenName + wxT("=") + hiddenVal + wxT("; ");

     // PRENの取得
     wxString pren;
     JaneCloneUtil::GetJaneCloneProperties(wxT("PREN"), &pren);
     SubstringCookie(pren);

     if (cookiePon.Len() != 0) 
     {
	  cookie.Append(cookiePon);
	  cookie.Append(wxT("; "));
     }

     if (cookieHap.Len() != 0) 
     {
	  cookie.Append(cookieHap);
	  cookie.Append(wxT("; "));
     }

     if (hidden.Len() != 0) 
     {
	  cookie.Append(hidden);
     }

     if (pren.Len() != 0) 
     {
	  cookie.Append(hidden);
     }

     if (cookie.EndsWith(wxT("; "))) 
     {
	  cookie = cookie.RemoveLast();
	  cookie = cookie.RemoveLast();
     }
}
/**
 * 投稿内容をソケット通信クラスに設定する
 * @param PostContent構造体
 */
void SocketCommunication::SetPostContent(std::unique_ptr<PostContent>& postContent) 
{
     this->postContent = std::move(postContent);
}
/**
 * COOKIEのデータ書き出しを行う
 */
void SocketCommunication::WriteCookieData(const wxString& dataFilePath) 
{
     // HTTPヘッダファイルを読み込む
     InitializeCookie();
     wxTextFile cookieFile;
     cookieFile.Open(dataFilePath, wxConvUTF8);
     wxString str, hiddenName, hiddenVal;

     if (cookieFile.IsOpened()) 
     {
	  for (str = cookieFile.GetFirstLine(); !cookieFile.Eof(); str = cookieFile.GetNextLine()) 
	  {
	       // hidden要素を抜き出す
	       if (str.Contains(wxT("<html>"))) 
	       {
		    // hidden要素正規表現
		    //                                     <input type=hidden name="yuki" value="akari">
		    static const wxRegEx hiddenElement(_T("<input type=hidden name=\"(.+?)\" value=\"(.+?)\">"), wxRE_ADVANCED + wxRE_ICASE);

		    if (hiddenElement.IsValid()) 
		    {
			 if (hiddenElement.Matches(str)) 
			 {
			      // マッチさせたそれぞれの要素を取得する
			      hiddenName = hiddenElement.GetMatch(str, 1);
			      hiddenVal = hiddenElement.GetMatch(str, 2);
			 }
		    }
	       }	       
	  }
     }

     // クッキー情報をコンフィグファイルに書き出す
     JaneCloneUtil::SetJaneCloneProperties(wxT("HiddenName"), hiddenName);
     JaneCloneUtil::SetJaneCloneProperties(wxT("HiddenValue"), hiddenVal);

     cookieFile.Close();
}
/**
 * 指定されたURLからデータをダウンロードする
 */
void SocketCommunication::DownloadImageFile(const wxString& href, std::unique_ptr<DownloadImageResult>& result) 
{
     // http or ftp
     if (href.StartsWith(wxT("http")) || href.StartsWith(wxT("ttp"))) 
     {
	  // http もしくは ttpの場合
	  DownloadImageFileByHttp(href, result);
	  SaveImageFileInfoDB(href, result);

     } 
     else if (href.StartsWith(wxT("ftp"))) 
     {
	  // ftp の場合(これってあんまり無くね？)
	  DownloadImageFileByFtp(href, result);
	  SaveImageFileInfoDB(href, result);

     } 
     else 
     {
	  wxMessageBox(wxT("ダウンロード対象のURLがhttp, ftpいずれでもありません."), wxT("画像ダウンロード"), wxICON_ERROR);
     }
}
/**
 * HTTPでのダウンロード
 */
void SocketCommunication::DownloadImageFileByHttp(const wxString& href, std::unique_ptr<DownloadImageResult>& result) 
{
     // 画像の保存先をコンフィグファイルから取得する
     // デフォルトは $HOME/.jc/cache
     InitializeCookie();
     wxString imageFilePath = ::wxGetHomeDir() + wxFILE_SEP_PATH + JANECLONE_DIR;

     wxString tmp = wxFILE_SEP_PATH;
     tmp += wxT("cache");
     JaneCloneUtil::GetJaneCloneProperties(wxT("CachePath"), &tmp);
     imageFilePath += tmp;

     // 画像ファイルパスを決定する
     wxString md5 = JaneCloneUtil::GenerateMD5String(href);

     wxString ext  = wxT(".") + result->ext;
     imageFilePath += wxFILE_SEP_PATH;
     imageFilePath += md5;
     imageFilePath += ext;
     result->imagePath = imageFilePath;
     result->fileName  = md5 + ext;

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

     if (http.Connect(server, 80)) 
     {
	  wxInputStream *stream;
	  stream = http.GetInputStream(path);

	  if (stream == NULL) 
	  {
	       output.Close();
	  } 
	  else 
	  {
	       unsigned char buffer[1024];
	       int byteRead;

	       // ストリームを受け取るループ部分
	       while (!stream->Eof()) 
	       {
		    stream->Read(buffer, sizeof(buffer));
		    store.Write8(buffer, sizeof(buffer));
		    byteRead = stream->LastRead();
		    if (byteRead <= 0) 
		    {
			 break;
		    }
	       }

	       output.Close();
	  }
     } 
     else 
     {
	  output.Close();
     }
}
/**
 * FTPでのダウンロード
 */
void SocketCommunication::DownloadImageFileByFtp(const wxString& href, std::unique_ptr<DownloadImageResult>& result) 
{
     // 画像の保存先をコンフィグファイルから取得する
     // デフォルトは $HOME/.jc/cache
     InitializeCookie();
     wxString imageFilePath = ::wxGetHomeDir();

     wxString tmp = wxFILE_SEP_PATH;
     tmp += wxT("cache");
     JaneCloneUtil::GetJaneCloneProperties(wxT("CachePath"), &tmp);
     imageFilePath += tmp;

}

#ifdef USE_SHINGETSU
/**
 * 指定された新月公開ノードホストにぶら下がるスレッド一覧リストをダウンロードしてくるメソッド
 * もし前回通信した際のログが残っていれば更新の確認のみ行う
 * @param  新月公開ノードのホスト名
 * @return 実行コード
 * 
 * 新月のAPIについては：http://shingetsu.info/saku/api
 */
bool SocketCommunication::DownloadShingetsuThreadList(const wxString& nodeHostname, wxString& outputFilePath) 
{

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
	  + wxFILE_SEP_PATH 
	  + JANECLONE_DIR 
	  + wxFILE_SEP_PATH 
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
	  if (port != wxEmptyString) 
	  {
	       portInteger = wxAtoi(port);
	  }

	  curlpp::Cleanup myCleanup;
	  curlpp::Easy myRequest;
	  LoadConfiguration(myRequest, RECV);
	  myRequest.setOpt(new cURLpp::Options::Url(requestQuery));
	  myRequest.setOpt(new cURLpp::Options::Port(portInteger));
	  std::ofstream ofs(outputFilename.c_str() , std::ios::out | std::ios::trunc );
	  curlpp::options::WriteStream ws(&ofs);
	  myRequest.setOpt(ws);
	  
	  // ログ出力
	  wxString message = wxT("新月にアクセス (ん`　 )") + wxString(requestQuery.c_str(), wxConvUTF8) + wxT("\n");
	  JaneCloneUiUtil::SendLoggingHelper(message);

	  myRequest.perform();

	  return true;

     } catch (curlpp::RuntimeError &e) {
	  wxString message = wxString((const char*)e.what(), wxConvUTF8) + wxString((const char*)outputFilename.c_str(), wxConvUTF8) + wxT("\n");
	  JaneCloneUiUtil::SendLoggingHelper(message);
     } catch (curlpp::LogicError &e) {
	  wxString message = wxString((const char*)e.what(), wxConvUTF8) + wxString((const char*)outputFilename.c_str(), wxConvUTF8) + wxT("\n");
	  JaneCloneUiUtil::SendLoggingHelper(message);
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
wxString SocketCommunication::DownloadShingetsuThread(const wxString& nodeHostname, const wxString& title, const wxString& filename) 
{
     // URIから各パラメーターを抜き取る
     PartOfURI* uri = new PartOfURI;
     bool urlIsSane = JaneCloneUtil::SubstringURI(nodeHostname, uri);
     const wxString protocol = uri->protocol;
     const wxString hostname = uri->hostname;
     wxString port = uri->port;
     delete uri;

     // 保存対象のディレクトリが存在するか確かめる
     wxString shingetsu = ::wxGetHomeDir()
	  + wxFILE_SEP_PATH 
	  + JANECLONE_DIR 
	  + wxFILE_SEP_PATH 
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
	  if (port != wxEmptyString) 
	  {
	       portInteger = wxAtoi(port);
	  }

	  curlpp::Cleanup myCleanup;
	  curlpp::Easy myRequest;
	  LoadConfiguration(myRequest, RECV);
	  myRequest.setOpt(new cURLpp::Options::Url(requestQuery));
	  myRequest.setOpt(new cURLpp::Options::Port(portInteger));
	  std::ofstream ofs(outputFilename.c_str() , std::ios::out | std::ios::trunc );
	  curlpp::options::WriteStream ws(&ofs);
	  myRequest.setOpt(ws);
	  
	  // ログ出力
	  wxString message = wxT("新月にアクセス (ん`　 )") + wxString(requestQuery.c_str(), wxConvUTF8) + wxT("\n");
	  JaneCloneUiUtil::SendLoggingHelper(message);

	  myRequest.perform();

	  return wxString(outputFilename.c_str(), wxConvUTF8);

     } catch (curlpp::RuntimeError &e) {
	  wxString message = wxString((const char*)e.what(), wxConvUTF8) + wxString((const char*)outputFilename.c_str(), wxConvUTF8) + wxT("\n");
	  JaneCloneUiUtil::SendLoggingHelper(message);
     } catch (curlpp::LogicError &e) {
	  wxString message = wxString((const char*)e.what(), wxConvUTF8) + wxString((const char*)outputFilename.c_str(), wxConvUTF8) + wxT("\n");
	  JaneCloneUiUtil::SendLoggingHelper(message);
     }

     return wxEmptyString;
}

#endif /** USE_SHINGETSU */
/**
 * ダウンロードした画像ファイル情報をDBに格納する
 */
void SocketCommunication::SaveImageFileInfoDB(const wxString& href, std::unique_ptr<DownloadImageResult>& result)
{
     if ( ! href || ! result) 
     {
	  wxString message = wxT("ダウンロードした画像ファイル情報が取得できませんでした.");
	  JaneCloneUiUtil::SendLoggingHelper(message);
	  return;
     }

     ImageFileInfo imageInfo;
     imageInfo.fileName = href;
     imageInfo.uuidFileName = result->fileName;
     
     // ファイル情報を格納
     SQLiteAccessor::SetImageFileName(imageInfo);
}
/**
 * BE２ちゃんねるにログインしてプロパティファイルに情報を書き出す
 */
void SocketCommunication::LoginBe2ch()
{
     wxString beMailAddress, bePassword;

     // プロパティファイルから設定されている項目を読みだして設定する
     std::pair <wxString, wxString> *pArray = new std::pair<wxString, wxString>[2];
     pArray[0]	= std::make_pair(wxT("ID_BEMailAddress"), beMailAddress);
     pArray[1]	= std::make_pair(wxT("ID_BEPassword"),    bePassword);	 

     for (int i = 0; i < 2; i++ )
     {
	  wxString widgetsName = pArray[i].first;
	  wxString widgetsInfo = wxEmptyString;
	  JaneCloneUtil::GetJaneCloneProperties(widgetsName, &widgetsInfo);
	  
	  if ( i == 0 ) 
	  {
	       beMailAddress = widgetsInfo;
	  } 
	  else if ( i == 1) 
	  {
	       bePassword = widgetsInfo;
	  }
     }

     delete[] pArray;

     if ( beMailAddress == wxEmptyString || bePassword == wxEmptyString ) 
     {
	  wxString log = wxT("BEのアドレスとパスワードが設定されてないよ（´・ω・｀）\n");
	  return;
     }

     /**
      * BEログインの処理実体
      */

     // Postする内容のデータサイズを取得する
     wxString kakikomiInfo = wxT("m=") 
	  + beMailAddress 
	  + wxT("&p=")
	  + bePassword
	  + wxT("&submit=") 
	  + wxT("%C5%D0%CF%BF");

     // ヘッダのサイズ
     std::ostringstream stream;
     stream << kakikomiInfo.Len();
     const std::string kakikomiSize = stream.str();
     
     /**
      * ヘッダを設定する
      */
     std::list<std::string> headers;
     headers.push_back("POST /test/login.php HTTP/1.1");
     headers.push_back("Accept-Encoding: gzip");
     headers.push_back("Accept: */*");
     headers.push_back("User-Agent: " + userAgent);
     headers.push_back("Content-Type: application/x-www-form-urlencoded");
     headers.push_back("Host: be.2ch.net");
     headers.push_back("Content-Length: " + kakikomiSize);
     headers.push_back("Connection: close");

     std::string url = "be.2ch.net/test/login.php";

     try {

	  curlpp::Cleanup myCleanup;
	  curlpp::Easy myRequest;
	  LoadConfiguration(myRequest, SEND);
	  myRequest.setOpt(new curlpp::options::Url(url));
	  myRequest.setOpt(new curlpp::options::HttpHeader(headers));
	  const std::string postField = std::string(kakikomiInfo.mb_str()); 
	  myRequest.setOpt(new curlpp::options::PostFields(postField)); 
	  myRequest.setOpt(new curlpp::options::PostFieldSize(postField.length()));
	  myRequest.setOpt(new curlpp::options::Verbose(true));
	  // ヘッダー用ファンクタを設定する
	  myRequest.setOpt(new curlpp::options::HeaderFunction(
				curlpp::types::WriteFunctionFunctor(this, &SocketCommunication::WriteHeaderData)));

	  wxString message = wxT("BEにログイン (ヽ´ん`)...\n");
	  JaneCloneUiUtil::SendLoggingHelper(message);

	  // 文字列をクリアーしておく
	  this->respBuf.clear();
	  this->bodyBuf.clear();

	  wxSleep(2);
	  myRequest.perform();

     } catch (curlpp::RuntimeError &e) {
	  wxString message = wxString((const char*)e.what(), wxConvUTF8) + wxT("\n");
	  JaneCloneUiUtil::SendLoggingHelper(message);
     } catch (curlpp::LogicError &e) {
	  wxString message = wxString((const char*)e.what(), wxConvUTF8) + wxT("\n");
	  JaneCloneUiUtil::SendLoggingHelper(message);
     }
}

/**
 * したらば掲示板の情報を取得する
 */
bool SocketCommunication::GetShitarabaBoardInfo(const wxString& path, wxString& boardName, wxString& category)
{
     std::vector<std::string> container;
     JaneCloneUtil::SplitStdString(container, std::string(path.mb_str()), "/");

     if (container.size() < 3)
     {
	  return false;
     }

     // http://jbbs.livedoor.jp/bbs/api/setting.cgi/[カテゴリ]/[番地]/
     std::string url = "jbbs.shitaraba.net/bbs/api/setting.cgi/" + container.at(1) + "/" + container.at(2) + "/";

     // 書き込み先
     wxDir dir(::wxGetHomeDir() + wxFILE_SEP_PATH + JANECLONE_DIR);
     wxString dataFilePath = dir.GetName();
     dataFilePath += wxFILE_SEP_PATH;
     dataFilePath += wxT("shitaraba_info.txt");

     try {

	  curlpp::Cleanup myCleanup;
	  curlpp::Easy myRequest;
	  LoadConfiguration(myRequest, RECV);
	  myRequest.setOpt(new curlpp::options::Url(url));
	  myRequest.setOpt(new curlpp::options::Verbose(true));

	  std::ofstream ofs(dataFilePath.mb_str() , std::ios::out | std::ios::trunc );
	  curlpp::options::WriteStream ws(&ofs);
	  myRequest.setOpt(ws);

	  wxString message = wxT("したらば掲示板にアクセス (ヽ´ん`)...\n");
	  JaneCloneUiUtil::SendLoggingHelper(message);

	  myRequest.perform();

     } catch (curlpp::RuntimeError &e) {
	  wxString message = wxString((const char*)e.what(), wxConvUTF8) + wxT("\n");
	  JaneCloneUiUtil::SendLoggingHelper(message);
	  return false;
     } catch (curlpp::LogicError &e) {
	  wxString message = wxString((const char*)e.what(), wxConvUTF8) + wxT("\n");
	  JaneCloneUiUtil::SendLoggingHelper(message);
	  return false;
     }

     /**
      * したらば掲示板の文字コードはクソEUC-JPである
      */
     wxString outputPath = dataFilePath;
     outputPath.Replace(wxT(".txt"), wxT(".out"));
     JaneCloneUtil::ConvertEUCJPToUTF8(dataFilePath, outputPath);
     
     wxTextFile shitarabaDataFile;
     shitarabaDataFile.Open(outputPath, wxConvUTF8);
     wxString str;

     // ファイルがオープンされているならば
     if (shitarabaDataFile.IsOpened()) 
     {
          for (str = shitarabaDataFile.GetFirstLine(); !shitarabaDataFile.Eof(); str = shitarabaDataFile.GetNextLine()) 
	  {
	       // データを取得する
	       wxString rest = wxEmptyString;

               if (str.StartsWith(wxT("BBS_TITLE="), &rest)) 
	       {
		    boardName = rest;
               }

               if (str.StartsWith(wxT("CATEGORY="), &rest)) 
	       {
		    category = rest;
               }
          }
     }

     shitarabaDataFile.Close();
     if ( boardName == wxEmptyString ) 
     {
	  return false;
     } 
     else 
     {
	  return true;
     }
}

/**
 * 入力された情報が他の掲示板のものであれば変換する
 *
 * @see    もし他の板を追加する機能を拡張するならば戻りはenum型にする
 *
 * @param  boardNameAscii アルファベットの板名
 * @param  ホスト名
 * @param  URL全体
 * @return URLが他掲示板のものである:true, 2chのURLである:false
 */
bool SocketCommunication::SetOtherBoardInfomation(wxString& boardNameAscii, wxString& hostName, wxString& boardURL)
{

     if ( hostName.Contains(wxT("jbbs.shitaraba.net")) ||
	  hostName.Contains(wxT("jbbs.livedoor.jp")) )
     {
	  // 入力されたURLはしたらば掲示板なのでboardNameAsciiの内容を変更
	  wxURI uri(boardURL);
	  boardNameAscii = uri.GetPath();

	  // したらば掲示板 => /news/xxxx/ の形式, 両端のスラッシュを削除
	  boardNameAscii = boardNameAscii.Mid(1, boardNameAscii.Len() - 2);
	  return true;
     }     

     return false;
}

/**
 * 出力ファイル名設定
 *
 * @param bool      isShitaraba    したらば掲示板:true, 2ch:false
 * @param wxString& boardNameAscii アルファベットの板名
 */
wxString SocketCommunication::GetOutputFileName(bool isShitaraba, wxString& boardNameAscii)
{
     wxString outputFileName = boardNameAscii + wxT(".dat");

     if (isShitaraba)
     {
	  // boardNameAscii = xxxx/0000
	  outputFileName = boardNameAscii.AfterFirst(wxT('/')) + wxT(".dat");
     }

     return outputFileName;
}

/**
 * 出力ファイルパス設定
 *
 * @param bool      isShitaraba    したらば掲示板:true, 2ch:false
 * @param wxString& boardNameAscii アルファベットの板名
 */
wxString SocketCommunication::GetOutputFilePath(bool isShitaraba, wxString& boardNameAscii)
{

#ifdef __WXMSW__
     boardNameAscii.Replace(wxT("/"), wxT("\\"));
     const wxString boardPath = boardNameAscii;
#else
     const wxString boardPath = boardNameAscii;
#endif
     // 出力先のファイルパスを設定する
     const wxString outputFilePath =
	  ::wxGetHomeDir()
	  + wxFILE_SEP_PATH 
	  + JANECLONE_DIR
	  + wxFILE_SEP_PATH
	  + wxT("dat")
	  + wxFILE_SEP_PATH
	  + boardNameAscii;

     // 保存用フォルダ存在するか確認。無ければフォルダを作成
     if (!wxDir::Exists(outputFilePath)) 
     {
	  ::wxMkdir(outputFilePath);
     }

     return outputFilePath;
}
/**
 * コンフィグ情報をCurl++のオブジェクトに設定する
 */
void SocketCommunication::LoadConfiguration(curlpp::Easy& request, const bool io)
{
     /**
      * Timeout Setting
      */
     if ( propMap.find(wxT("ID_Connection_Timeout_Sec")) != propMap.end() )
     {
	  const wxString connectTimeout = propMap[wxT("ID_Connection_Timeout_Sec")];
	  long sec = 0;
	  if ( connectTimeout.IsNumber() && connectTimeout.ToLong(&sec, 10) )
	  {
	       request.setOpt(new curlpp::options::Timeout(sec));
	  }
     }

     if ( propMap.find(wxT("ID_Receive_Timeout_Sec")) != propMap.end() )
     {
	  const wxString receiveTimeout = propMap[wxT("ID_Receive_Timeout_Sec")];
	  long sec = 0;
	  if ( receiveTimeout.IsNumber() && receiveTimeout.ToLong(&sec, 10) )
	  {
	       request.setOpt(new curlpp::options::ConnectTimeout(sec));
	  }
     }

     /**
      * BASIC Auth
      */
     if ( propMap.find(wxT("ID_NetworkPanelBasicAuthUserName")) != propMap.end() &&
	  propMap.find(wxT("ID_NetworkPanelBasicAuthPassword")) != propMap.end())
     {
	  const wxString username = propMap[wxT("ID_NetworkPanelBasicAuthUserName")];
	  const wxString password = propMap[wxT("ID_NetworkPanelBasicAuthPassword")]; // TODO: パスワードのHASH化
	  if ( username.IsWord() && password.IsWord() )
	  {
	       const wxString concat = username + wxT(":") + password;
	       request.setOpt(new curlpp::options::UserPwd(std::string(concat.mb_str())));
	  }
	  else
	  {
	       const wxString message = wxT("無効なBASIC認証設定が無視されました");
	       JaneCloneUiUtil::SendLoggingHelper(message);
	  }	  
     }

     /**
      * PROXY
      */
     if (propMap.find(wxT("ID_NetworkPanelUseProxy")) != propMap.end() )
     {
	  bool useProxy = JaneCloneUtil::ParseBool(propMap[wxT("ID_NetworkPanelUseProxy")]);

	  if (useProxy)
	  {
	       // ProxyCache
	       if ( propMap.find(wxT("ID_NetworkPanelUseProxyCache")) != propMap.end())
	       {
		    bool useProxyCache = JaneCloneUtil::ParseBool(propMap[wxT("ID_NetworkPanelUseProxyCache")]);
		    // TODO: (・_・)
		    //request.setOpt(new curlpp::options::UserPwd(std::string(concat.mb_str())));
	       }

	       // PROXYを使用する設定
	       if (io == SEND)
	       {
		    // 送信
		    // wxT("ID_NetworkPanelProxySendAddr")	,/* Proxy送信用アドレス					*/	  
		    // wxT("ID_NetworkPanelProxySendPort")	,/* Proxy送信用ポート					*/		  
		    if ( propMap.find(wxT("ID_NetworkPanelProxySendAddr")) != propMap.end() &&
			 propMap.find(wxT("ID_NetworkPanelProxySendPort")) != propMap.end())
		    {
			 const wxString proxy = propMap[wxT("ID_NetworkPanelProxySendAddr")];
			 const wxString port  = propMap[wxT("ID_NetworkPanelProxySendPort")];
			 long p = 0;

			 if ( proxy.IsWord() && port.IsNumber() && port.ToLong(&p, 10))
			 {
			      request.setOpt(new curlpp::options::Proxy(std::string(proxy.mb_str())));
			      request.setOpt(new curlpp::options::ProxyPort(p));		      
			 }
			 else
			 {
			      const wxString message = wxT("無効なProxy設定が無視されました");
			      JaneCloneUiUtil::SendLoggingHelper(message);
			 }     
		    }
	       }
	       else
	       {
		    // 受信
		    // wxT("ID_NetworkPanelProxyReceiveAddr")	,/* Proxy受信用アドレス					*/ 
		    // wxT("ID_NetworkPanelProxyReceivePort")	,/* Proxy受信用ポート					*/	   
		    if ( propMap.find(wxT("ID_NetworkPanelProxyReceiveAddr")) != propMap.end() &&
			 propMap.find(wxT("ID_NetworkPanelProxyReceivePort")) != propMap.end())
		    {
			 const wxString proxy = propMap[wxT("ID_NetworkPanelProxyReceiveAddr")];
			 const wxString port  = propMap[wxT("ID_NetworkPanelProxyReceivePort")];
			 long p = 0;

			 if ( proxy.IsWord() && port.IsNumber() && port.ToLong(&p, 10) )
			 {
			      request.setOpt(new curlpp::options::Proxy(std::string(proxy.mb_str())));
			      request.setOpt(new curlpp::options::ProxyPort(p));			      
			 }
			 else
			 {
			      const wxString message = wxT("無効なProxy設定が無視されました");
			      JaneCloneUiUtil::SendLoggingHelper(message);
			 }
		    }
	       }
	       // wxT("ID_NetworkPanelProxySSLAuthAddr")	,/* Proxy SSL認証用アドレス				*/	   
	       // wxT("ID_NetworkPanelProxySSLAuthPort")	,/* Proxy SSL認証用ポート				*/
	  }
     }
}

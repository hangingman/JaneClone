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

#include "janecloneutil.hpp"

/**
 * gzipファイルを解凍する処理
 * 引数１は読み込み元gzipファイルのPATH、引数２は解凍先のファイルのPATH
 * いずれもファイル名までを記述する
 */
void JaneCloneUtil::DecommpressFile(wxString & inputPath,
				    wxString & outputPath) {
     // gzファイルをZlibを使って解凍する
     const gzFile infile = gzopen(inputPath.mb_str(), "rb");
     FILE *outfile = fopen(outputPath.mb_str(), "wb");

     char buffer[S_SIZE];
     int num_read = 0;
     while ((num_read = gzread(infile, buffer, sizeof(buffer))) > 0) {
	  fwrite(buffer, 1, num_read, outfile);
     }

     // ファイルポインタを閉じる
     gzclose(infile);
     fclose(outfile);
}

/**
 * ダウンロードしたファイルの文字コードをShift-JISからUTF-8に変換する処理
 * 引数１は読み込み元のPATH、引数２は出力先ファイルのPATH いずれもファイル名までを記述する
 */
void JaneCloneUtil::ConvertSJISToUTF8(wxString & inputPath,
				      wxString & outputPath) {

     // LibNKFを呼び出してCP932からUTF-8への変換を行う
     wxNKF* nkf = new wxNKF();
     nkf->Convert(inputPath, outputPath, wxT("--ic=CP932 --oc=UTF-8"));
     delete nkf;
}

/**
 * 指定されたパスにあるHTTPヘッダファイルから取得日時を取得する処理
 */
wxString JaneCloneUtil::GetHTTPCommTimeFromHeader(wxString& headerPath) {
     // HTTPヘッダファイルを読み込む
     wxTextFile httpHeaderFile;
     httpHeaderFile.Open(headerPath, wxConvUTF8);
     wxString str;

     // ファイルがオープンされているならば
     if (httpHeaderFile.IsOpened()) {
	  for (str = httpHeaderFile.GetFirstLine(); !httpHeaderFile.Eof(); str =
		    httpHeaderFile.GetNextLine()) {
	       // 上から読み込んで一番最初に当たる日付が取得日時
	       if (str.Contains(wxT("Date:")) && str.Contains(wxT("GMT"))) {
		    httpHeaderFile.Close();
		    return str;
	       }
	  }
     }
     // ここまで来てしまった場合空文字を返す
     httpHeaderFile.Close();
     return wxEmptyString;
}

/**
 * 指定された数字からスレッドの作成された時間を計算する処理
 */
wxString JaneCloneUtil::CalcThreadCreatedTime(wxString& threadNum) {
     int threadNumInt = wxAtoi(threadNum);
     time_t timeGMT = (time_t) threadNumInt;
     struct tm *date = localtime(&timeGMT);
     char str[64];
     strftime(str, 63, "%x %H:%M", date);
     return wxString(str, wxConvUTF8);
}

/**
 * 指定されたパスにファイルがあればファイルサイズを返す処理
 */
size_t JaneCloneUtil::GetFileSize(const wxString& filePath) {

     /**
      * メモ：2chのスレッドの容量を測る場合、スレッドはデータ容量が約500KBで規制されるので
      * fileSizeをsize_tでキャストしても安全？
      * 符号なしint型の範囲は 0 ～ 65535
      */

     if (wxFileName::FileExists(filePath)) {

	  const wxULongLong fileSize = wxFileName::GetSize(filePath);

	  if (fileSize == wxInvalidSize) {
	       // ファイルが正常に開けなかった場合
	       return 0;
	  } else {
	       // unsigned long型で返却
	       return (size_t) fileSize.GetLo();
	  }
     } else {
	  // ファイルが存在しなかった場合
	  return 0;
     }
}
/**
 * アンカーで指定されたレスをファイルから読み取ってDOM形式にして送り返す
 */
wxString JaneCloneUtil::FindAnchoredResponse(wxString& boardNameAscii,
					     wxString& origNumber, wxString& resNumber) {

     // ファイルパスの組み立てとファイルの有無確認
     const wxDir dir(::wxGetHomeDir() + wxFileSeparator + JANECLONE_DIR);
     wxString filePath = dir.GetName();

#ifdef __WXMSW__
     // Windowsではパスの区切りは"\"
     filePath += wxT("\\dat\\");
     filePath += boardNameAscii;
     filePath += wxT("\\");
     filePath += origNumber;
     filePath += wxT(".dat");
#else
     // それ以外ではパスの区切りは"/"
     filePath += wxT("/dat/");
     filePath += boardNameAscii;
     filePath += wxT("/");
     filePath += origNumber;
     filePath += wxT(".dat");
#endif

     if (!wxFile::Exists(filePath)) {
	  // 無ければ空文字リターン
	  return wxEmptyString;
     }
     // 取得するレス番
     int resNumInt = wxAtoi(resNumber);

     // datファイルを読み込む
     wxTextFile datFile;
     datFile.Open(filePath, wxConvUTF8);
     wxString str;

     if (datFile.IsOpened() && 1 == resNumInt) {
	  str = datFile.GetFirstLine();
	  datFile.Close();

     } else if (datFile.IsOpened() && resNumInt > 1) {
	  datFile.GetFirstLine();

	  for (int i = 1; !datFile.Eof(); str = datFile.GetNextLine()) {
	       // 上から読み込んでレス番号にあたる行のレスを取得する
	       if (i == resNumInt) {
		    datFile.Close();
		    break;
	       }
	       ++i;
	  }
     }

     // HTMLのDOM形式にする
     wxString lumpOfHTML = HTML_HEADER_POPUP;
     lumpOfHTML += wxT("<dt>");
     lumpOfHTML += resNumber;

     wxString default_nanashi, mail, day_and_ID, res;

     // 正規表現でレスの内容を取り出してメモリに展開する

     /** ex) レスの形式
      *
      * [デフォルト名無し]<>[メ欄]<>[yyyy/mm/dd(D) HH:MM:SS:ss ID:aaaaaaaaa BE:xxxxxxxxx-2BE(n)]<>[書き込み]<>板名
      * [デフォルト名無し]<>[メ欄]<>[yyyy/mm/dd(D) HH:MM:SS:ss ID:aaaaaaaaa]<>[書き込み]<>
      */
     if (regexThread.IsValid() && resNumInt > 1) {
	  // >>1 以外の処理
	  if (regexThread.Matches(str)) {
	       // マッチさせたそれぞれの要素を取得する
	       default_nanashi = regexThread.GetMatch(str, 1);
	       mail = regexThread.GetMatch(str, 2);
	       day_and_ID = regexThread.GetMatch(str, 3);

	       // レスの最初に<table>タグを入れる
	       res.Append(wxT("<table border=0 id=\"") + resNumber + wxT("\">"));
	       res.Append(regexThread.GetMatch(str, 4));
	       res.Append(wxT("</table>"));

	       // レス内部のURLに<a>タグをつける
	       res = ReplaceURLText(res);
	       // レスの最後に改行
	       res.Append(wxT("<br>"));
	  }
     } else if (regexThreadFst.IsValid() && resNumInt == 1) {
	  // >>1 の場合の処理
	  if (regexThreadFst.Matches(str)) {
	       // マッチさせたそれぞれの要素を取得する
	       default_nanashi = regexThreadFst.GetMatch(str, 1);
	       mail = regexThreadFst.GetMatch(str, 2);
	       day_and_ID = regexThreadFst.GetMatch(str, 3);

	       // レスの最初に<table>タグを入れる
	       res.Append(wxT("<table border=0 id=\"") + resNumber + wxT("\">"));
	       res.Append(regexThreadFst.GetMatch(str, 4));
	       res.Append(wxT("</table>"));

	       // レス内部のURLに<a>タグをつける
	       res = ReplaceURLText(res);
	       // レスの最後に改行
	       res.Append(wxT("<br>"));
	  }
     }

     if (mail != wxEmptyString) {
	  // もしメ欄になにか入っているならば
	  lumpOfHTML += wxT(" 名前：<a href=\"mailto:");
	  lumpOfHTML += mail;
	  lumpOfHTML += wxT("\"><b>");
	  lumpOfHTML += default_nanashi;
	  lumpOfHTML += wxT("</b></a>");
	  lumpOfHTML += day_and_ID;
	  lumpOfHTML += wxT("<dd>");
	  lumpOfHTML += res;
     } else {
	  // 空の場合
	  lumpOfHTML += wxT(" 名前：<font color=green><b>");
	  lumpOfHTML += default_nanashi;
	  lumpOfHTML += wxT("</b></font>");
	  lumpOfHTML += day_and_ID;
	  lumpOfHTML += wxT("<dd>");
	  lumpOfHTML += res;
     }

     // HTMLソースを加える
     lumpOfHTML += HTML_FOOTER;

     return lumpOfHTML;
}
/**
 * レス内にURLがあれば<a>タグを付ける
 */
wxString JaneCloneUtil::ReplaceURLText(const wxString& responseText) {

     wxString text = responseText;
     wxString tmp, result;
     size_t start, len;

     if (regexURL.IsValid() && regexURL.Matches(responseText)) {
	  for (tmp = text; regexURL.Matches(tmp);
	       tmp = tmp.SubString(start + len, tmp.Len())) {
	       regexURL.GetMatch(&start, &len, 0);
	       result += tmp.SubString(0, start - 1);
	       result += wxT("<a href=\"");
	       result += tmp.SubString(start, start + len - 1);
	       result += wxT("\"/>");
	       result += tmp.SubString(start, start + len - 1);
	       result += wxT("</a>");
	  }
	  result += tmp;
     } else {
	  return responseText;
     }

     return result;
}
/**
 * レス内に画像があれば<img>タグを付ける
 */
void JaneCloneUtil::AddImgTag(wxString& responseText) {

     if (regexImage.IsValid() && regexImage.Matches(responseText)) {
	  // 画像URLを保存する配列
	  wxArrayString array;
	  wxString text = responseText;
	  wxString tmp;
	  size_t start, len;

	  // デフォルト画像の読み込み
	  wxString imagePath = wxGetCwd() + wxFileSeparator + wxT("rc") + wxFileSeparator + wxT("image-x-generic.png");
	  wxImage image;
	  wxBitmap bitmap;

          // load wxImage
	  if (!image.LoadFile(imagePath)) {
	       wxMessageBox(wxT("画像ファイルの読み出しに失敗しました"),
			    wxT("wxMemoryFSHandler"),
			    wxICON_ERROR);
	       return;
	  }
	  // wxImage to wxBitmap
	  bitmap = wxBitmap(image);

	  if (!bitmap.Ok()) {
	       wxMessageBox(wxT("画像ファイルの読み出しに失敗しました"),
			    wxT("wxMemoryFSHandler"),
			    wxICON_ERROR);
	       return;
	  }

	  // HTMLに改行を加える
	  responseText.Append(wxT("<br>"));

	  for (tmp = text; regexImage.Matches(tmp);
	       tmp = tmp.SubString(start + len, tmp.Len())) {
	       // ex) tmp = http://hogehoge.jpg
	       regexImage.GetMatch(&start, &len, 0);
	       array.Add(tmp.SubString(start, start + len - 1));
	  }

	  for (int i = 0; i < array.GetCount();i++) {
	       // ex) <img src=\"memory:logo.pcx\">
	       wxString filename = wxFileSystem::URLToFileName(array[i]).GetFullName();
	       wxMemoryFSHandler::AddFile(filename, bitmap, wxBITMAP_TYPE_PNG);
	       responseText.Append(wxT("<p><img src=\"memory:") + filename + wxT("\" width=16 height=16 /></p>"));
	  }
	  // HTMLに改行を加える
	  responseText.Append(wxT("<br>"));
     }
}
/**
 * 指定された文字列でdatファイルへのファイルパスを組み立てる
 */
wxString JaneCloneUtil::AssembleFilePath(wxString& boardNameAscii,
					 wxString& origNumber) {

     const wxDir dir(::wxGetHomeDir() + wxFileSeparator + JANECLONE_DIR);
     wxString filePath = dir.GetName();

#ifdef __WXMSW__
     // Windowsではパスの区切りは"\"
     filePath += wxT("\\dat\\");
     filePath += boardNameAscii;
     filePath += wxT("\\");
     filePath += origNumber;
     filePath += wxT(".dat");
#else
     // それ以外ではパスの区切りは"/"
     filePath += wxT("/dat/");
     filePath += boardNameAscii;
     filePath += wxT("/");
     filePath += origNumber;
     filePath += wxT(".dat");
#endif

     return filePath;
}
/**
 * 現在時刻をUNIX Timeで返す
 */
wxString JaneCloneUtil::GetTimeNow() {

     long unixTime = wxGetUTCTime();
     return wxString::Format(_("%ld"), unixTime);
}
/**
 * 文字列をURLエンコードする
 */
std::string JaneCloneUtil::UrlEncode(const std::string& str) {

     std::string retStr;  
  
     std::string::size_type length = str.size();   
     for ( std::string::size_type i = 0 ; i < length ; i++ )  
     {  
	  if (( '0' <= str[ i ] && str[ i ] <= '9' ) ||  
	      ( 'a' <= str[ i ] && str[ i ] <= 'z' ) ||  
	      ( 'A' <= str[ i ] && str[ i ] <= 'Z' ) ||  
	      str[ i ] == '-'   || str[ i ] == '.'   ||  
	      str[ i ] == '_'   || str[ i ] == '~' )  
	  {  
	       // 数字/アルファベット/[-][.][_][~]はそのまま  
	       retStr += str[ i ];  
	  }  
	  else  
	  {  
	       retStr += '%';  
	       char tmp[ 3 ];  
	       snprintf( tmp, 3, "%X", static_cast< unsigned char >( str[ i ] ));  
	       retStr += tmp;  
	  }  
     }  
  
     return retStr;  
}
/**
 * 文字列中の実体参照文字を変換する
 */
wxString JaneCloneUtil::ConvCharacterReference(wxString& inputString) {

     wxString buffer = inputString;
     buffer.Replace(_T("&nbsp;"), _T(" "), true);
     buffer.Replace(_T("&lt;"), _T("<"), true);
     buffer.Replace(_T("&gt;"), _T(">"), true);
     buffer.Replace(_T("&amp;"), _T("&"), true);
     buffer.Replace(_T("&quot;"), _T("\""), true);
     buffer.Replace(_T("&apos;"), _T("'"), true);
     return buffer;
}
/**
 * URLの末尾にある拡張子が何か判別し、Content-Typeを返す
 */
wxString JaneCloneUtil::DetermineContentType(const wxString& href) {

     // ex) image/png; charset=utf-8
     if (href.EndsWith(wxT(".jpg")) || href.EndsWith(wxT(".JPG"))) {
	  return wxT("image/jpg; charset=utf-8");
     } else if (href.EndsWith(wxT(".jpeg")) || href.EndsWith(wxT(".JPEG"))) {
	  return wxT("image/jpeg; charset=utf-8");
     } else if (href.EndsWith(wxT(".png")) || href.EndsWith(wxT(".PNG"))) {
	  return wxT("image/png; charset=utf-8");
     } else if (href.EndsWith(wxT(".gif")) || href.EndsWith(wxT(".GIF"))) {
	  return wxT("image/gif; charset=utf-8");
     } else if (href.EndsWith(wxT(".bmp")) || href.EndsWith(wxT(".BMP"))) {
	  return wxT("image/bmp; charset=utf-8");
     }
     
}
/**
 * URIから各パラメーターを抜き取る
 */
bool JaneCloneUtil::SubstringURI(wxString uri, PartOfURI* partOfUri) {

     // 正規表現のコンパイルにエラーがなければ
     if (regexURI.IsValid()) {
	  // マッチさせる
	  if (regexURI.Matches(uri)) {
	       partOfUri->protocol = regexURI.GetMatch(uri, 1);
	       partOfUri->hostname = regexURI.GetMatch(uri, 2);
	       partOfUri->port = regexURI.GetMatch(uri, 3);
	       partOfUri->path = regexURI.GetMatch(uri, 4);
	       return true;
	  }
     }
     return false;
}
/**
 * UUIDを生成する
 */     
wxString JaneCloneUtil::GenerateUUIDString() {
     return wxUUID::GetUUID();
}
/**
 * スレッドの勢い値を計算する
 * 勢い＝書き込み数÷（スレが立ってからの秒数÷86400）
 * @param itemResponse レス数
 * @param itemOid      スレがたった時間を表すUNIX Time
 * @return momentum    勢い
 */
wxString JaneCloneUtil::CalcThreadMomentum(wxString& itemResponse, wxString& itemOid) {

     unsigned long response;
     unsigned long oid;
     unsigned long now;
     itemResponse.ToULong(&response);
     itemOid.ToULong(&oid);
     now = ::wxGetUTCTime();

     // レス数が1の場合勢い値は1
     if (response == 1) return wxT("1.0");

     // 勢い値の計算
     try {
	  double elapsed = now - oid;
	  double momentum = response / elapsed * 86400;
	  return wxString::Format(wxT("%.1f"), momentum);

     } catch (std::range_error& e) {
	  return wxT("std::range_error");
     } catch (std::overflow_error& e) {
	  return wxT("std::overflow_error");
     } catch (std::underflow_error& e) {
	  return wxT("std::underflow_error");
     }
}
/**
 * スレッドの情報をOIDをキーとするmapに変換する
 * @param map<wxString,ThreadList>& oldThreadMap 古いスレッドの情報を保持するコンテナ
 */
void JaneCloneUtil::GenerateOldThreadMap(std::map<wxString,ThreadList>& oldThreadMap, URLvsBoardName& boardInfo) {

     // ファイルのパスを設定する
     wxString outputPath = ::wxGetHomeDir() 
	  + wxFileSeparator 
	  + JANECLONE_DIR
	  + wxFileSeparator
	  + wxT("dat")
	  + wxFileSeparator
	  + boardInfo.boardNameAscii
	  + wxFileSeparator
	  + boardInfo.boardNameAscii
	  + wxT(".dat");

     std::cout << "create old thread map" << std::endl;

     // ファイルが存在しなければそのままリターン
     // つまり完全に初回のスレッド一覧取得
     if(!wxFileExists(outputPath)) return;
     // テキストファイルの読み込み
     wxTextFile datfile(outputPath);
     datfile.Open();

     // スレッド一覧読み込み用正規表現を準備する
     wxRegEx reThreadLine(_T("([[:digit:]]+).dat<>(.+)\\(([[:digit:]]{1,4})\\)"), wxRE_ADVANCED + wxRE_ICASE);
     // スレッドに番号をつける
     int loopNumber = 1;

     // テキストファイルの終端まで読み込む
     for (wxString line = datfile.GetFirstLine(); !datfile.Eof(); line = datfile.GetNextLine()) {

	  if (line.Contains(_("&"))) { 
	       line = ConvCharacterReference(line);
	  }

	  ThreadList threadInfoList;
	  
	  // 番号
	  threadInfoList.number = loopNumber;
	  // 板名
	  threadInfoList.boardName = boardInfo.boardName;

	  // 正規表現で情報を取得する
	  if (reThreadLine.Matches(line)) {
	       // キー値を取得する
	       threadInfoList.oid = reThreadLine.GetMatch(line, 1);
	       // since
	       threadInfoList.since = JaneCloneUtil::CalcThreadCreatedTime(threadInfoList.oid);
	       // スレタイを取得する
	       threadInfoList.title = reThreadLine.GetMatch(line, 2);
	       // レス数を取得する
	       threadInfoList.response = wxAtoi(reThreadLine.GetMatch(line, 3));
	  }

	  /**
	   * 更新処理
	   */
	  // 取得
	  //itemCachedResponseNumber = wxEmptyString;
	  // 新着
	  //itemNewResponseNumber = wxEmptyString;
	  // 増レス
	  //itemIncreaseResponseNumber = wxEmptyString;
	  // 勢い
	  wxString response = wxString::Format(wxT("%d"), threadInfoList.response);
	  threadInfoList.momentum = JaneCloneUtil::CalcThreadMomentum(response, threadInfoList.oid);
	  // 最終取得
	  //itemLastUpdate = wxEmptyString;

	  // 項目を追加する
	  oldThreadMap.insert(std::map<wxString, ThreadList>::value_type(threadInfoList.oid, threadInfoList));
	  
	  // ループ変数をインクリメント
	  ++loopNumber;
	  std::cout << "old thread number:" << loopNumber << std::endl;
     }

     datfile.Close();
}
/**
 * プロパティファイルの指定されたKEYを指定されたVALUEで書き換える
 * @param const wxString& key     キー
 * @param const <class T>& value  値
 */
template <class T>
void JaneCloneUtil::SetJaneCloneProperties(const wxString& key, const T& value) {

     // 設定ファイルの準備をする
     const wxString jc = ::wxGetHomeDir() + wxFileSeparator + JANECLONE_DIR;
     const wxDir jcDir(jc);
     const wxString configFile = jcDir.GetName() + wxFileSeparator + wxT("prop") + wxFileSeparator + APP_CONFIG_FILE;
     wxFileConfig* config = new wxFileConfig(wxT("JaneClone"), wxEmptyString, configFile, wxEmptyString, wxCONFIG_USE_LOCAL_FILE);

     config->Write(key, value);
     config->Flush();
     delete config;
}
// テンプレート関数の実体化
template void JaneCloneUtil::SetJaneCloneProperties<wxString>(const wxString& key, const wxString& value);
template void JaneCloneUtil::SetJaneCloneProperties<long>(const wxString& key, const long& value);
template void JaneCloneUtil::SetJaneCloneProperties<double>(const wxString& key, const double& value);
template void JaneCloneUtil::SetJaneCloneProperties<bool>(const wxString& key, const bool& value);
/**
 * プロパティファイルの指定されたKEYを読みとってvalueに格納
 * @param  const wxString&  key    キー
 * @param        <class T>* value  値
 */
template <class T>
void JaneCloneUtil::GetJaneCloneProperties(const wxString& key, T* value) {

     // 設定ファイルの準備をする
     const wxString jc = ::wxGetHomeDir() + wxFileSeparator + JANECLONE_DIR;
     const wxDir jcDir(jc);
     const wxString configFile = jcDir.GetName() + wxFileSeparator + wxT("prop") + wxFileSeparator + APP_CONFIG_FILE;
     wxFileConfig* config = new wxFileConfig(wxT("JaneClone"), wxEmptyString, configFile, wxEmptyString, wxCONFIG_USE_LOCAL_FILE);

     config->Read(key, value);
     delete config;
}
// テンプレート関数の実体化
template void JaneCloneUtil::GetJaneCloneProperties<wxString>(const wxString& key, wxString* value);
template void JaneCloneUtil::GetJaneCloneProperties<long>(const wxString& key, long* value);
template void JaneCloneUtil::GetJaneCloneProperties<double>(const wxString& key, double* value);
template void JaneCloneUtil::GetJaneCloneProperties<bool>(const wxString& key, bool* value);

/**
 * 指定されたディレクトリの下に、指定された名前のディレクトリが存在するか確認して作成する
 */
void JaneCloneUtil::CreateSpecifyDirectory(wxDir& specifyDir, const wxString& dirName) {

     if (!specifyDir.HasSubDirs(dirName)) {
	  ::wxMkdir(specifyDir.GetName() + wxFileSeparator + dirName);
     }
}
/**
 * 新月のCSVファイル保存場所を作成する
 */
wxString JaneCloneUtil::CreateShingetsuThreadListFilePath(const wxString& nodeHostname) {

     // URIから各パラメーターを抜き取る
     PartOfURI* uri = new PartOfURI;
     bool urlIsSane = JaneCloneUtil::SubstringURI(nodeHostname, uri);
     const wxString protocol = uri->protocol;
     const wxString hostname = uri->hostname;
     const wxString port = uri->port;
     const wxString path = uri->path;
     delete uri;

     wxString shingetsuFilePath = ::wxGetHomeDir()
	  + wxFileSeparator 
	  + JANECLONE_DIR 
	  + wxFileSeparator 
	  + wxT("shingetsu")
	  + wxFileSeparator
	  + hostname
	  + wxFileSeparator
	  + hostname
	  + wxT(".csv");

     return shingetsuFilePath;
}

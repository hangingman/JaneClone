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
 *	Hiroyuki Nagata <newserver002@gmail.com>
 */

#ifndef JANECLONEUTIL_HPP_
#define JANECLONEUTIL_HPP_

#include <zlib.h>
#include <iostream>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <memory>
#include <wx/wx.h>
#include <wx/textfile.h>
#include <wx/file.h>
#include <wx/filename.h>
#include <wx/filefn.h>
#include <wx/dir.h>
#include <wx/regex.h>
#include <wx/fs_mem.h>
#include <wx/treectrl.h>
#include <wx/fileconf.h>
#include <guid.h>
#include <babel.h>
#include "datatype.hpp"
#include "enums.hpp"
#include "janecloneuiutil.hpp"

/**
 * JaneCloneのファイル操作用クラス
 */
class JaneCloneDirTraverser : public wxDirTraverser {

public:
     JaneCloneDirTraverser (){}

     wxDirTraverseResult OnFile(const wxString& fileName) {
	  m_files.Add(fileName);
	  wxString log = wxString(fileName);
	  SendLogging(log);
	  return wxDIR_CONTINUE;
     };

     wxDirTraverseResult OnDir(const wxString& dirName) {
	  m_files.Add(dirName);
	  wxString log = wxString(dirName);
	  SendLogging(log);
	  return wxDIR_CONTINUE;
     };

     void GetResultFiles(wxArrayString& result) {
	  result = m_files;
     };

private:
     // 取得したファイル
     wxArrayString m_files;

     // メインのスレッドにログとイベントを送る
     void SendLogging(wxString& message) {
	  JaneCloneUiUtil::SendLoggingHelper(message);
     };
};

/**
 * JaneCloneのファイル操作用クラス
 */
class JaneCloneUtil {

public:
     /**
      * gzipファイルを解凍する処理
      * 引数１は読み込み元gzipファイルのPATH、引数２は解凍先のファイルのPATH
      * いずれもファイル名までを記述する
      */
     static void DecommpressFile(wxString& inputPath, wxString& outputPath);
     /**
      * ダウンロードしたファイルの文字コードをShift-JISからUTF-8に変換する処理
      * @param wxString& inputPath  読み込み元のパス
      * @param wxString& outputPath 出力先ファイルのパス
      */
     static void ConvertSJISToUTF8(wxString& inputPath, wxString& outputPath);
     /**
      * ダウンロードしたファイルの文字コードをEUC-JPからUTF-8に変換する処理
      * @param wxString& inputPath  読み込み元のパス
      * @param wxString& outputPath 出力先ファイルのパス
      */
     static void ConvertEUCJPToUTF8(wxString& inputPath, wxString& outputPath);
	
     /**
      * 指定されたパスにあるHTTPヘッダファイルから取得日時を取得する処理
      */
     static wxString GetHTTPCommTimeFromHeader(wxString& headerPath);
     /**
      * 指定された数字からスレッドの作成された時間を計算する処理
      */
     static wxString CalcThreadCreatedTime(wxString& threadNum);
     /**
      * 指定されたパスにファイルがあればファイルサイズを返す処理
      */
     static size_t GetFileSize(const wxString& filePath);
     /**
      * アンカーで指定されたレスをファイルから読み取ってDOM形式にして送り返す
      * @param  const wxString& boardNameAscii         板名の英語名  
      * @param  const wxString& origNumber             UNIXタイムベースのオリジナルな番号
      * @param  const wxString& resNumberStart         取得するレスの番号(開始点)
      * @param  const wxString& resNumberEnd           取得するレスの番号(終了点)
      * @return wxString        取得したレスの内容
      */
     static wxString FindAnchoredResponse(const wxString& boardNameAscii, const wxString& origNumber, 
					  const wxString& resNumberStart, const wxString& resNumberEnd=wxEmptyString);
     /**
      * アンカーで指定されたレスをファイルから読み取ってレスだけを返す
      *
      * @param  const wxString& boardNameAscii         板名の英語名  
      * @param  const wxString& origNumber             UNIXタイムベースのオリジナルな番号
      * @param  const long      resNumber              取得するレスの番号
      * @param  const bool      useTriangularBrackets  true = 返り値に'>'がつく, false = '>'がつかない
      * @return wxString        取得したレスの内容
      */
     static wxString FindAnchoredResponseText(const wxString& boardNameAscii, const wxString& origNumber, 
					      const long resNumber, const bool useTriangularBrackets=true);
     /**
      * レスをIDで抽出してファイルから読み取ってDOM形式にして送り返す
      * @param  const wxString& boardNameAscii         板名の英語名  
      * @param  const wxString& origNumber             UNIXタイムベースのオリジナルな番号
      * @param  const wxString& extractId              抽出対象のID
      * @return wxString        取得したレスの内容
      */
     static wxString FindResponseById(const wxString& boardNameAscii, const wxString& origNumber, 
				      const wxString& extractId);
     /**
      * レス内にURLがあれば<a>タグを付ける
      */
     static wxString ReplaceURLText(const wxString& responseText);
     /**
      * レス内に画像があれば<a>タグを付ける
      */
     static void AddImgTag(wxString& responseText);
     /**
      * プレインテキスト内にアンカーがあれば<a>タグをつける
      */
     static wxString AddAnchorTag(wxString& responseText);
     /**
      * プレインテキスト内に2chのIDがあれば<a>タグをつける
      */
     static wxString AddID(wxString& responseText);
     /**
      * 指定された文字列でdatファイルへのファイルパスを組み立てる
      * @param 英字の板名
      * @param 固有番号
      * @param datファイルの種別 (KIND_THREAD_DAT, KIND_BOARD__DAT)
      */
     static wxString AssembleFilePath(wxString& boardNameAscii, wxString& origNumber, const int kind);
     /**
      * 現在時刻をUNIX Timeで返す
      */
     static wxString GetTimeNow();
     /**
      * 文字列をURLエンコードする
      */
     static std::string UrlEncode(const std::string& str);
     /**
      * 文字列をURLエンコードする
      */
     static std::string UrlEncode(const wxString& str) {
	  return JaneCloneUtil::UrlEncode(std::string(str.mb_str()));
     };
     /**
      * 文字列中の実体参照文字を変換する
      */
     static wxString ConvCharacterReference(wxString& inputString);
     /**
      * URLの末尾にある拡張子が何か判別し、Content-Typeを返す
      */
     static wxString DetermineContentType(const wxString& href);
     /**
      * URIから各パラメーターを抜き取る
      */
     static bool SubstringURI(const wxString& uri, PartOfURI* partOfUri);
     /**
      * UUIDを生成する
      */
     static wxString GenerateUUIDString();
     /**
      * スレッドの勢い値を計算する
      * 勢い = 書き込み数 / スレが立ってからの秒数 * 86400
      * @param  itemResponse レス数
      * @param  itemOid      スレがたった時間を表すUNIX Time
      * @return momentum     勢い
      */
     static wxString CalcThreadMomentum(wxString& itemResponse, wxString& itemOid);
     /**
      * スレッドの情報をOIDをキーとするmapに変換する
      * @param map<wxString,ThreadList>& oldThreadMap 古いスレッドの情報を保持するコンテナ
      */
     static void GenerateOldThreadMap(std::map<wxString,ThreadList>& oldThreadMap, URLvsBoardName& boardInfo);
     /**
      * プロパティファイルの指定されたKEYを指定されたVALUEで書き換える
      */
     template <class T> 
     static void SetJaneCloneProperties(const wxString& key, const T& value);
     /**
      * プロパティファイルの指定されたKEYを読み取る
      */
     template <class T> 
     static void GetJaneCloneProperties(const wxString& key, T* value);
     /**
      * プロパティファイルの指定されたKEYがあるかどうか確認する
      */
     static bool GetJaneCloneEntryExist(const wxString& key);
     /**
      * プロパティファイルの指定されたKEYを削除する
      */
     static void DeleteJaneClonePropertyEntry(const wxString& key);
     /**
      * 指定されたディレクトリの下に、指定された名前のディレクトリが存在するか確認して作成する
      */
     static void CreateSpecifyDirectory(wxDir& specifyDir, const wxString& dirName);
     /**
      * 新月のCSVファイル保存場所を作成する
      */
     static wxString CreateShingetsuThreadListFilePath(const wxString& nodeHostname);
     /**
      * スレッドの最初の行を処理するメソッド
      */
     static wxString ProcessFirstResponse(wxString& threadRecord);
     /**
      * スレッドの１以降を処理するメソッド
      */
     static wxString ProcessRestResponse(wxString& threadRecord, int number);
     /**
      * 文字列のスプリット関数(wxWidgets StringTokenizer is buggy :<)
      * I refered here, thanks ! http://stackoverflow.com/questions/53849/how-do-i-tokenize-a-string-in-c
      */
     static void SplitStdString(std::vector<std::string> & theStringVector,  /* Altered/returned value */
				const std::string& theString,
				const std::string& theDelimiter);
     /**
      * あるディレクトリ以下のすべてのファイルとディレクトリを再帰的に取得する
      * @param  targetDir 対象のディレクトリ
      * @param  result    結果を格納する配列
      * @result 取得したファイルとディレクトリの数
      */
     static size_t GetFileNamesRecursive(const wxDir& targetDir, wxArrayString& result);


     template<typename T, std::size_t N>
     static std::size_t NumOf(const T (&array)[N]){
	  return N;
     }

private:

     // ディスクからの読取サイズ
#define S_SIZE (2048)

};

#endif /* JANECLONEUTIL_HPP_ */

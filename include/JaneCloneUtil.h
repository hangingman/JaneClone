/*
 * JaneCloneUtil.h
 *
 *  Created on: 2012/05/04
 *      Author: learning
 */

#ifndef JANECLONEUTIL_H_
#define JANECLONEUTIL_H_

#include <wx/wx.h>
#include <wx/strconv.h>
#include <wx/textfile.h>
#include <wx/file.h>
#include <zlib.h>

class JaneCloneUtil {
public:
	/**
	 * gzipファイルを解凍する処理
	 * 引数１は読み込み元gzipファイルのPATH、引数２は解凍先のファイルのPATH
	 * いずれもファイル名までを記述する
	 */
	static void DecommpressFile(wxString& inputPath,
			wxString& outputPath);
	/**
	 * ダウンロードしたファイルの文字コードをShift-JISからUTF-8に変換する処理
	 * 引数１は読み込み元のPATH、引数２は出力先ファイルのPATH いずれもファイル名までを記述する
	 */
	static void ConvertSJISToUTF8(wxString& inputPath,
			wxString& outputPath);
private:
	// ディスクからの読取サイズ
	#define S_SIZE (2048)
};

#endif /* JANECLONEUTIL_H_ */

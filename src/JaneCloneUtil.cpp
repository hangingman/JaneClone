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

#include "JaneCloneUtil.h"

/**
 * gzipファイルを解凍する処理
 * 引数１は読み込み元gzipファイルのPATH、引数２は解凍先のファイルのPATH
 * いずれもファイル名までを記述する
 */
void JaneCloneUtil::DecommpressFile(wxString & inputPath,
		wxString & outputPath) {
	// gzファイルをZlibを使って解凍する
	gzFile infile = gzopen(inputPath.mb_str(), "rb");
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
	/* ! Fix Me !  全然うまく動かん */
	LibNKF* nkf = new LibNKF();
	nkf->Convert(std::string(inputPath.mb_str()),
			std::string(outputPath.mb_str()), "--ic=CP932 --oc=UTF-8");
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
				return str;
			}
		}
	}
	// ここまで来てしまった場合空文字を返す
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

		wxULongLong fileSize = wxFileName::GetSize(filePath);

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


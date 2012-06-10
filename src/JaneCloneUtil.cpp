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
void JaneCloneUtil::DecommpressFile( wxString & inputPath,
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
void JaneCloneUtil::ConvertSJISToUTF8( wxString & inputPath,
		 wxString & outputPath) {

	iconv_t icd;
	FILE *fp_src, *fp_dst;
	char s_src[S_SIZE], s_dst[S_SIZE];
	char *p_src;
	char *p_dst;
	size_t n_src, n_dst;
	//int* iconctl;

	// 文字コード変換はCP932からUTF-8
	icd = iconv_open("UTF-8", "CP932");
	//  iconvctl で変換不可能な文字があった時の設定
	//  変換不可能だったら捨てる
	//  不正な文字があり、捨てられたときは iconctlに1が入る
	//iconvctl(icd, ICONV_GET_DISCARD_ILSEQ, iconctl);  <-- fix me ! iconvctlは最新のlibiconvで共通で使えるのか検証する

	fp_src = fopen(inputPath.mb_str(), "r");
	fp_dst = fopen(outputPath.mb_str(), "w");

	while (true) {
		fgets(s_src, S_SIZE, fp_src);
		if (feof(fp_src))
			break;
		p_src = s_src;
		p_dst = s_dst;
		n_src = strlen(s_src);
		n_dst = S_SIZE - 1;
		while (0 < n_src) {
			size_t result;
			result = iconv(icd, &p_src, &n_src, &p_dst, &n_dst);
			// エラーがあれば止める
			if ((int)result == -1) {
				perror("iconv");
				break;
			}
		}
		*p_dst = '\0';
		fputs(s_dst, fp_dst);
	}
	fclose(fp_dst);
	fclose(fp_src);
	iconv_close(icd);
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
		for (str = httpHeaderFile.GetFirstLine(); !httpHeaderFile.Eof(); str = httpHeaderFile.GetNextLine()) {
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
	time_t timeGMT = (time_t)threadNumInt;
	struct tm *date = localtime(&timeGMT);
	char str[64];
	strftime(str, 63, "%x %H:%M", date);
	return wxString(str, wxConvUTF8);
}




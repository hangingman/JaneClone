/*
 * JaneCloneUtil.cpp
 *
 *  Created on: 2012/05/04
 *      Author: learning
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

	// 出力先ファイルを作成する
	wxFile outputFile;
	outputFile.Create(outputPath, true, wxS_DEFAULT);
	outputFile.Close();

	// Shift_JISファイルを読み込む
	wxTextFile shift_JIS_file;
	shift_JIS_file.Open(inputPath, wxCSConv(wxT("CP932")));
	wxString str;
	// 書き出し先のファイルを設定する
	wxTextFile utf8_file;
	utf8_file.Open(outputPath, wxConvUTF8);

	// きちんとどちらのファイルもオープンされているならば
	if (shift_JIS_file.IsOpened() && utf8_file.IsOpened()) {
		for (str = shift_JIS_file.GetFirstLine(); !shift_JIS_file.Eof(); str = shift_JIS_file.GetNextLine()) {
			// 書き出し先にデータを書き出す
			utf8_file.AddLine(str, wxTextFileType_Dos);
		}
	}

	// ファイルのクローズ
	shift_JIS_file.Close();
	utf8_file.Write(wxTextFileType_Dos, wxConvUTF8);
	utf8_file.Close();
}


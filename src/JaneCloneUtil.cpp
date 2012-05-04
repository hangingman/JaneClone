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
	iconv_t icd;
	FILE *fp_src, *fp_dst;
	char s_src[S_SIZE], s_dst[D_SIZE];
	char *p_src;
	char *p_dst;
	size_t n_src, n_dst;
	int* iconctl;

	// 文字コード変換はCP932からUTF-8
	icd = iconv_open("UTF-8", "CP932");
	//  iconvctl で変換不可能な文字があった時の設定
	//  変換不可能だったら捨てる
	//  不正な文字があり、捨てられたときは iconctlに1が入る
	iconvctl(icd, ICONV_GET_DISCARD_ILSEQ, iconctl);

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
			if (result == -1) {
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


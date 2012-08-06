/*
 * LibNKF.h
 *	libnkfの中心となるクラス
 *  Created on: 2012/07/10
 * Contributor: Hiroyuki Nagata
 */

#ifndef LIBNKF_H_
#define LIBNKF_H_

#include <iostream>
#include <string>
#include <sys/stat.h>
#include <cstdlib>
#include "NKFBaseConfig.h"
#include "FlagSet.h"
#include "UTF8Table.h"
#include "NKFNativeEncoding.h"
#include "Util.h"
#include "UTF16Util.h"
#include "InputCodeList.h"

/* HELP_OUTPUT */
#ifdef HELP_OUTPUT_STDERR
#define HELP_OUTPUT stderr
#else
#define HELP_OUTPUT stdout
#endif

/* all long option */
static const struct {
	const char *name;
	const char *alias;
} long_option[] = { { "ic=", "" }, { "oc=", "" }, { "base64", "jMB" }, { "euc",
		"e" }, { "euc-input", "E" }, { "fj", "jm" }, { "help", "" }, { "jis",
		"j" }, { "jis-input", "J" }, { "mac", "sLm" }, { "mime", "jM" }, {
		"mime-input", "m" }, { "msdos", "sLw" }, { "sjis", "s" }, {
		"sjis-input", "S" }, { "unix", "eLu" }, { "version", "v" }, { "windows",
		"sLw" }, { "hiragana", "h1" }, { "katakana", "h2" }, {
		"katakana-hiragana", "h3" }, { "guess=", "" }, { "guess", "g2" }, {
		"cp932", "" }, { "no-cp932", "" }, { "x0212", "" }, { "utf8", "w" }, {
		"utf16", "w16" }, { "ms-ucs-map", "" }, { "fb-skip", "" }, { "fb-html",
		"" }, { "fb-xml", "" }, { "fb-perl", "" }, { "fb-java", "" }, {
		"fb-subchar", "" }, { "fb-subchar=", "" }, { "utf8-input", "W" }, {
		"utf16-input", "W16" }, { "no-cp932ext", "" },
		{ "no-best-fit-chars", "" }, { "utf8mac-input", "" },
		{ "overwrite", "" }, { "overwrite=", "" }, { "in-place", "" }, {
				"in-place=", "" }, { "cap-input", "" }, { "url-input", "" }, {
				"numchar-input", "" }, { "no-output", "" }, { "debug", "" }, {
				"cp932inv", "" }, { "prefix=", "" }, };

class LibNKF {

public:
	/**
	 * コンストラクタ
	 */
	LibNKF();
	/**
	 * デストラクタ
	 */
	~LibNKF();
	/**
	 * 使い方の表示
	 */
	void ShowUsage();
	/**
	 * バージョン情報の表示
	 */
	void ShowVersion();
	/**
	 * 文字コードを変換する処理のラッパーで、外部に見せるメソッド
	 */
	int Convert(const std::string inputFile, const std::string outputFile, const std::string option);
	/**
	 * 文字コードを変換する処理のラッパーで、外部に見せるメソッド
	 */
	std::wstring Convert(const std::string inputFile, const std::string option);
	/**
	 * 変換後の文字列だけを取得する
	 */
	std::wstring GetConvertedString() {
		if (!oConvStr)
			return 0;

		return *oConvStr;
	};
	/**
	 * 変換後の文字列長だけを取得する
	 */
	unsigned int GetConvertedStringLength () {
		if (!oConvStr)
			return 0;

		return oConvStr->length();
	};

private:
	/**
	 * KanjiConvertメソッド中のswitch文用マクロ
	 */
	#define NEXT continue        /* no output, get next */
	#define SKIP c2=0;continue   /* no output, get next */
	#define MORE c2=c1;continue  /* need one more byte */
	#define SEND (void)0         /* output c1 and c2, get next */
	#define LAST break           /* end of loop, go closing  */
	/**
	 * 文字コード変換された文字列の出力先
	 */
	std::wstring* oConvStr;
	/**
	 * NKFのフラグのセット
	 */
	std::bitset<nkf_flag_num> nkfFlags;
	/**
	 * SetOption:オプションの判別と設定を行う
	 *
	 * return values:
	 *    0: success
	 *   -1: ArgumentError
	 */
	int SetOption(const std::string option);
	/**
	 * 入力文字コードと出力文字コードの設定と処理を行うクラスのインスタンス
	 */
	NKFNativeEncoding* inputEncoding;
	NKFNativeEncoding* outputEncoding;
	/**
	 * このクラスの主要メソッド：
	 * ファイルポインタを引数に文字コードを変換する
	 */
	int KanjiConvert(FILE *f);
	/**
	 * 設定されたフラグから文字コード変換に使うメソッドを決める
	 */
	int ModuleConnection();
	/**
	 * 入力された文字コードからフラグを設定する
	 */
	void SetInputEncoding(NKFNativeEncoding *enc);
	/**
	 * 出力する文字コード用にフラグを設定する
	 */
	void SetOutputEncoding(NKFNativeEncoding *enc);
	/**
	 * 入力文字コードを設定する
	 */
	void SetInputMode(int mode);
	/**
	 * BOMが存在するかチェックし、存在すれば無視する
	 */
	void CheckBom(FILE *f);
	/**
	 * ファイルポインタから1バイトnkf用のデータを読み取って返す
	 */
	static nkf_char StdGetC(FILE *f) {
		return getc(f);
	}
	;
	/**
	 * 一度読み込んだ文字をファイルストリームに戻す
	 */
	static void StdUnGetC(nkf_char c, FILE *f) {
		ungetc(c, f);
	}
	;
	/**
	 * 総当りで文字コードを調べる
	 */
	void CodeStatus(nkf_char c);
	/**
	 * LibNKFクラス内で持つしかない変数
	 */
	unsigned char prefix_table[256];

	char* backup_suffix;
	int fold_len;
	int fold_margin;
	int mimeout_mode; /* 0, -1, 'Q', 'B', 1, 2 */
	int shift_mode;/* 0 or 1 */
};

#endif /* LIBNKF_H_ */

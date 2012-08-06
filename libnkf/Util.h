/*
 * Util.h
 *	NKFの中で共通で使われているメソッドを集めたクラス
 *  Created on: 2012/07/12
 * Contributor: Hiroyuki Nagata
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <cstring>
#include "NKFNativeEncoding.h"

class Util {
public:
	/**
	 * 文字コード名からNKFEncodingクラスを判別し、インスタンスを返す
	 */
	static void NKFEncFind(const char *name, NKFNativeEncoding* enc);
	/**
	 * IDに対応するNKFEncodingクラスを返却する
	 */
	static void NKFEncFromIndex(int idx, NKFNativeEncoding* enc);
	/**
	 * 文字コード名から文字コードに対応するIDを取得する
	 */
	static int NKFEncFindIndex(const char *name);
	/**
	 * 文字コードを初期化する
	 */
	static NKFNativeEncoding* NKFDefaultEncoding();
	/**
	 * EUC-JPからUTF-8への変換
	 */
	static nkf_char E2wConv(nkf_char c2, nkf_char c1, std::bitset<nkf_flag_num> nkfFlags);
	/**
	 * Shift_JISからEUC-JPへの変換
	 */
	static nkf_char S2eConv(nkf_char c2, nkf_char c1, nkf_char *p2,
			nkf_char *p1, std::bitset<nkf_flag_num> nkfFlags);
	/**
	 * UTF-8からEUC-JPへの変換
	 */
	static nkf_char W2eConv(nkf_char c2, nkf_char c1, nkf_char c0, nkf_char *p2,
			nkf_char *p1, std::bitset<nkf_flag_num> nkfFlags);
	/**
	 * OSのロケールカらデフォルトのNKFEncodingを取得し返す
	 */
	static void NKFLocaleEncoding(NKFNativeEncoding* enc);
	/**
	 * UnicodeからUTF-8への変換
	 */
	static void NKFUnicodeToUTF8(nkf_char val, nkf_char *p1, nkf_char *p2,
			nkf_char *p3, nkf_char *p4);
	/**
	 * UTF-8からUnicodeへの変換
	 */
	static nkf_char NKFUTF8ToUnicode(nkf_char c1, nkf_char c2, nkf_char c3,
			nkf_char c4);
	/**
	 * UnicodeからSJISへの変換
	 */
	static int UnicodeToJISCommon(nkf_char c2, nkf_char c1, nkf_char c0,
			nkf_char *p2, nkf_char *p1, std::bitset<nkf_flag_num> nkfFlags);
	/**
	 * UnicodeからSJISへの変換
	 */
	static int UnicodeToJISCommon2(nkf_char c1, nkf_char c0,
			const unsigned short * const *pp, nkf_char psize, nkf_char *p2,
			nkf_char *p1, std::bitset<nkf_flag_num> nkfFlags);
	/**
	 *
	 */
	static nkf_char E2sConv(nkf_char c2, nkf_char c1, nkf_char *p2,
			nkf_char *p1, std::bitset<nkf_flag_num> nkfFlags);

	static nkf_char X0212Shift(nkf_char c);
	static nkf_char X0212Unshift(nkf_char c);
	static nkf_char W16eConv(nkf_char val, nkf_char *p2, nkf_char *p1, std::bitset<nkf_flag_num> nkfFlags);

private:
	/**
	 * ２つの引数を先頭から比較してboolを返す、比較する長さは１つ目の引数の長さ
	 */
	static bool StrncmpFromHead(const char* charCode, const char* name);
};

#endif /* UTIL_H_ */

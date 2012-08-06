/*
 * NKFNativeEncoding.h
 *
 *  Created on: 2012/07/11
 * Contributor: Hiroyuki Nagata
 */

#ifndef NKFNATIVEENCODING_H_
#define NKFNATIVEENCODING_H_

#include "NKFBaseConfig.h"
#include "FlagSet.h"
#include "UTF8Table.h"

/**
 * 構造体：nkf_encodingとnkf_native_encoding同じクラスとしたもの
 *
 * nkf_native_encoding内には
 * "ASCII" "ISO-2022-JP" "Shift_JIS" "EUC-JP" "UTF-8" "UTF-16" "UTF-32" が存在する
 * フラグによってIconvメソッドとOconvメソッドの処理を変える
 */
class NKFNativeEncoding {
public:
	/**
	 * 一意に決まる文字コード名のID・switch文等で使う
	 */
	int id;
	/**
	 * 一意に決まる文字コード名
	 */
	std::string name;
	/**
	 * 一意に決まる文字コード名のID・switch文等で使う(内部での変換用)
	 */
	int baseId;
	/**
	 * 一意に決まる文字コード名(内部での変換用)
	 */
	std::string baseName;
	/**
	 * エンディアン
	 */
	int endian;
	/**
	 * 入出力のモード
	 * inputEncodingクラス：inputMode, outputMode情報あり
	 * outputEncodingクラス：outputMode情報あり
	 */
	int inputMode;
	int outputMode;

	std::string iconvName;
	std::string oconvName;

	nkf_char Iconv(nkf_char c2, nkf_char c1, nkf_char c0, std::bitset<nkf_flag_num> nkfFlags, std::wstring* oConvStr);
	void Oconv(nkf_char c2, nkf_char c1, std::bitset<nkf_flag_num> nkfFlags, std::wstring* oConvStr);

private:
	/**
	 * s_iconv
	 */
	nkf_char SIconv(nkf_char c2, nkf_char c1, nkf_char c0, std::bitset<nkf_flag_num> nkfFlags, std::wstring* oConvStr);
	/**
	 * e_iconv
	 */
	nkf_char EIconv(nkf_char c2, nkf_char c1, nkf_char c0, std::bitset<nkf_flag_num> nkfFlags, std::wstring* oConvStr);
	/**
	 * w_iconv
	 */
	nkf_char WIconv(nkf_char c1, nkf_char c2, nkf_char c3, std::bitset<nkf_flag_num> nkfFlags, std::wstring* oConvStr);
	/**
	 * w_iconv16
	 */
	nkf_char WIconv16(nkf_char c2, nkf_char c1, nkf_char c0, std::bitset<nkf_flag_num> nkfFlags, std::wstring* oConvStr);
	/**
	 * w_iconv32
	 */
	nkf_char WIconv32(nkf_char c2, nkf_char c1, nkf_char c0, std::bitset<nkf_flag_num> nkfFlags, std::wstring* oConvStr);
	/**
	 * j_oconv
	 */
	void JOconv(nkf_char c2, nkf_char c1, std::bitset<nkf_flag_num> nkfFlags, std::wstring* oConvStr);
	/**
	 * s_oconv
	 */
	void SOconv(nkf_char c2, nkf_char c1, std::bitset<nkf_flag_num> nkfFlags, std::wstring* oConvStr);
	/**
	 * e_oconv
	 */
	void EOconv(nkf_char c2, nkf_char c1, std::bitset<nkf_flag_num> nkfFlags, std::wstring* oConvStr);
	/**
	 * w_oconv
	 */
	void WOconv(nkf_char c2, nkf_char c1, std::bitset<nkf_flag_num> nkfFlags, std::wstring* oConvStr);
	/**
	 * w_oconv16
	 */
	void WOconv16(nkf_char c2, nkf_char c1, std::bitset<nkf_flag_num> nkfFlags, std::wstring* oConvStr);
	/**
	 * w_oconv32
	 */
	void WOconv32(nkf_char c2, nkf_char c1, std::bitset<nkf_flag_num> nkfFlags, std::wstring* oConvStr);
	/**
	 * Asciiコードが混じった場合のエスケープシーケンス
	 */
	void OutputAsciiEscapeSequence(int mode, std::wstring* oConvStr);
};

#endif /* NKFNATIVEENCODING_H_ */

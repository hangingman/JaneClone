/*
 * InputCodeList.h
 * 使用する文字コードごとのメソッドと変数を設定するクラス
 *  Created on: 2012/07/17
 * Contributor: Hiroyuki Nagata
 */

#ifndef INPUTCODELIST_H_
#define INPUTCODELIST_H_

#include <string>
#include "FlagSet.h"
#include "Util.h"

#define PREFIX_EUCG3    NKF_INT32_C(0x8F00)
#define CLASS_MASK      NKF_INT32_C(0xFF000000)
#define CLASS_UNICODE   NKF_INT32_C(0x01000000)
#define VALUE_MASK      NKF_INT32_C(0x00FFFFFF)
#define UNICODE_BMP_MAX NKF_INT32_C(0x0000FFFF)
#define UNICODE_MAX     NKF_INT32_C(0x0010FFFF)
#define nkf_char_euc3_new(c) ((c) | PREFIX_EUCG3)
#define nkf_char_unicode_new(c) ((c) | CLASS_UNICODE)
#define nkf_char_unicode_p(c) ((c & CLASS_MASK) == CLASS_UNICODE)
#define nkf_char_unicode_bmp_p(c) ((c & VALUE_MASK) <= UNICODE_BMP_MAX)
#define nkf_char_unicode_value_p(c) ((c & VALUE_MASK) <= UNICODE_MAX)

#define SCORE_L2       (1)                   /* Kanji Level 2 */
#define SCORE_KANA     (SCORE_L2 << 1)       /* Halfwidth Katakana */
#define SCORE_DEPEND   (SCORE_KANA << 1)     /* MD Characters */
#define SCORE_CP932    (SCORE_DEPEND << 1)   /* IBM extended characters */
#define SCORE_X0212    (SCORE_CP932 << 1)    /* JIS X 0212 */
#define SCORE_NO_EXIST (SCORE_X0212 << 1)    /* Undefined Characters */
#define SCORE_iMIME    (SCORE_NO_EXIST << 1) /* MIME selected */
#define SCORE_ERROR    (SCORE_iMIME << 1)    /* Error */

#define SCORE_INIT (SCORE_iMIME)

static const nkf_char score_table_A0[] = {
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, SCORE_DEPEND, SCORE_DEPEND, SCORE_DEPEND,
    SCORE_DEPEND, SCORE_DEPEND, SCORE_DEPEND, SCORE_NO_EXIST,
};

static const nkf_char score_table_F0[] = {
    SCORE_L2, SCORE_L2, SCORE_L2, SCORE_L2,
    SCORE_L2, SCORE_DEPEND, SCORE_NO_EXIST, SCORE_NO_EXIST,
    SCORE_DEPEND, SCORE_DEPEND, SCORE_CP932, SCORE_CP932,
    SCORE_CP932, SCORE_NO_EXIST, SCORE_NO_EXIST, SCORE_ERROR,
};

class InputCode {
public:
	std::string name;
	nkf_char stat;
	nkf_char score;
	nkf_char index;
	nkf_char buf[3];
	void StatusFunc(InputCode* inputCode, nkf_char c, std::bitset<nkf_flag_num> nkfFlags);
	nkf_char IconvFunc(nkf_char c2, nkf_char c1, nkf_char c0, std::bitset<nkf_flag_num> nkfFlags);
	int _file_stat;

	/**
	 * 関数名からInputCodeを引く
	 */
	static InputCode* FindInputcodeByFunc(std::string funcName);
	/**
	 * Scoreの設定を行う
	 */
	static void ClrCodeScore(InputCode* ptr, nkf_char score);

private:
	/**
	 * StatusFunc内部のstatus関数
	 */
	void EStatus(InputCode* inputCode, nkf_char c, std::bitset<nkf_flag_num> nkfFlags);
	void SStatus(InputCode* inputCode, nkf_char c, std::bitset<nkf_flag_num> nkfFlags);
	void WStatus(InputCode* inputCode, nkf_char c, std::bitset<nkf_flag_num> nkfFlags);
	/**
	 * IConv関数
	 */
	nkf_char SIconv(nkf_char c2, nkf_char c1, nkf_char c0);
	nkf_char EIconv(nkf_char c2, nkf_char c1, nkf_char c0);
	nkf_char WIconv(nkf_char c2, nkf_char c1, nkf_char c0);
	nkf_char WIconv16(nkf_char c2, nkf_char c1, nkf_char c0);
	nkf_char WIconv32(nkf_char c2, nkf_char c1, nkf_char c0);
	/**
	 * その他のコールバック関数
	 */
	static void StatusCheck(InputCode* ptr, nkf_char c, std::bitset<nkf_flag_num> nkfFlags);
	static void StatusReset(InputCode* ptr);
	static void StatusClear(InputCode* ptr);
	static void StatusPushCh(InputCode* ptr, nkf_char c);
	static void StatusDisable(InputCode* ptr, std::bitset<nkf_flag_num> nkfFlags);
	static void CodeScore(InputCode* ptr, std::bitset<nkf_flag_num> nkfFlags);
	static void SetCodeScore(InputCode* ptr, nkf_char score);
};

#endif /* INPUTCODELIST_H_ */

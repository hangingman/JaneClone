/*
 * FlagSet.cpp
 * すべてのフラグを管理するクラス
 *  Created on: 2012/07/29
 * Contributor: Hiroyuki Nagata
 */

#include "NKFBaseConfig.h"
#include "FlagSet.h"

std::bitset<nkf_flag_num> FlagSet::GetFlagSet() {

	// 全機能FALSE
	std::bitset<nkf_flag_num> nkfFlags;

	// フラグを初期状態に設定する
	nkfFlags.set(binmode_f) = TRUE;
	nkfFlags.set(cp932inv_f) = TRUE;
	nkfFlags.set(kanji_intro) = DEFAULT_J;
	nkfFlags.set(ascii_intro) = DEFAULT_R;
	nkfFlags.set(mime_f) = MIME_DECODE_DEFAULT;

	return nkfFlags;
}

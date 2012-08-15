/*
 * utf16util.cpp
 *
 *  Created on: 2012/07/16
 * Contributor: Hiroyuki Nagata
 */

#include "utf16util.h"

nkf_char UTF16Util::NKFIconvUTF16(nkf_char c1, nkf_char c2, nkf_char c3,
		nkf_char c4) {

	nkf_char wc;

	if (c1 == EOF) {
		return EOF;
	}

	if (this->inputEndian == ENDIAN_BIG) {
		if (0xD8 <= c1 && c1 <= 0xDB) {
			if (0xDC <= c3 && c3 <= 0xDF) {
				wc = UTF16_TO_UTF32(c1 << 8 | c2, c3 << 8 | c4);
			} else
				return NKF_ICONV_NEED_TWO_MORE_BYTES;
		} else {
			wc = c1 << 8 | c2;
		}
	} else {
		if (0xD8 <= c2 && c2 <= 0xDB) {
			if (0xDC <= c4 && c4 <= 0xDF) {
				wc = UTF16_TO_UTF32(c2 << 8 | c1, c4 << 8 | c3);
			} else
				return NKF_ICONV_NEED_TWO_MORE_BYTES;
		} else {
			wc = c2 << 8 | c1;
		}
	}

	return wc;
}

void UTF16Util::SetInputEndian(int endian) {
	inputEndian = endian;
}

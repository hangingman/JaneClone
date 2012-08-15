/*
 * utf16util.h
 *
 *  Created on: 2012/07/16
 * Contributor: Hiroyuki Nagata
 */

#ifndef UTF16UTIL_H_
#define UTF16UTIL_H_

#include "flagset.h"
#include "wxnkfbaseconfig.h"

/**
 * macros for utf16
 */
#define NKF_ICONV_NEED_ONE_MORE_BYTE (size_t)-1
#define NKF_ICONV_NEED_TWO_MORE_BYTES (size_t)-2
#define UTF16_TO_UTF32(lead, trail) (((lead) << 10) + (trail) - NKF_INT32_C(0x35FDC00))

class UTF16Util {
public:
	nkf_char NKFIconvUTF16(nkf_char c1, nkf_char c2, nkf_char c3, nkf_char c4);
	void SetInputEndian(int endian);
private:
	int inputEndian;
};

#endif /* UTF16UTIL_H_ */

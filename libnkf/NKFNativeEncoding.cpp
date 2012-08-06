/*
 * NKFNativeEncoding.h
 *
 *  Created on: 2012/07/11
 * Contributor: Hiroyuki Nagata
 */

#include "NKFNativeEncoding.h"
#include "Util.h"
/**
 * まずいことにNKFNativeEncodingクラスとUtilクラスは相互参照である
 * コンパイルは通るが、あまりいい構成ではない
 */
nkf_char NKFNativeEncoding::Iconv(nkf_char c2, nkf_char c1, nkf_char c0,
		std::bitset<nkf_flag_num> nkfFlags, std::wstring* oConvStr) {

	switch (this->baseId) {

	case ASCII:
		return EIconv(c2, c1, c0, nkfFlags, oConvStr);
		break;
	case ISO_2022_JP:
		return EIconv(c2, c1, c0, nkfFlags, oConvStr);
		break;
	case SHIFT_JIS:
		return SIconv(c2, c1, c0, nkfFlags, oConvStr);
		break;
	case EUC_JP:
		return EIconv(c2, c1, c0, nkfFlags, oConvStr);
		break;
	case UTF_8:
		return WIconv(c2, c1, c0, nkfFlags, oConvStr);
		break;
	case UTF_16:
		return WIconv16(c2, c1, c0, nkfFlags, oConvStr);
		break;
	case UTF_32:
		return WIconv32(c2, c1, c0, nkfFlags, oConvStr);
		break;
	default:
		return -1;
	}
}

void NKFNativeEncoding::Oconv(nkf_char c2, nkf_char c1,
		std::bitset<nkf_flag_num> nkfFlags, std::wstring* oConvStr) {

	switch (this->outputMode) {

	case ASCII:
		EOconv(c2, c1, nkfFlags, oConvStr);
		break;
	case ISO_2022_JP:
		JOconv(c2, c1, nkfFlags, oConvStr);
		break;
	case SHIFT_JIS:
		SOconv(c2, c1, nkfFlags, oConvStr);
		break;
	case EUC_JP:
		EOconv(c2, c1, nkfFlags, oConvStr);
		break;
	case UTF_8:
		WOconv(c2, c1, nkfFlags, oConvStr);
		break;
	case UTF_16:
		break;
		WOconv16(c2, c1, nkfFlags, oConvStr);
		break;
	case UTF_32:
		WOconv32(c2, c1, nkfFlags, oConvStr);
		break;
	}
}

nkf_char NKFNativeEncoding::SIconv(nkf_char c2, nkf_char c1, nkf_char c0,
		std::bitset<nkf_flag_num> nkfFlags, std::wstring* oConvStr) {

	if (c2 == JIS_X_0201_1976_K || (0xA1 <= c2 && c2 <= 0xDF)) {
		if (nkfFlags[iso2022jp_f] && !nkfFlags[x0201_f]) {
			c2 = GETA1;
			c1 = GETA2;
		} else {
			c1 &= 0x7f;
		}
	} else if ((c2 == EOF) || (c2 == 0) || c2 < SP) {
		/* NOP */
	} else if (!nkfFlags[x0213_f] && 0xF0 <= c2 && c2 <= 0xF9 && 0x40 <= c1
			&& c1 <= 0xFC) {
		/* CP932 UDC */
		if (c1 == 0x7F)
			return 0;
		c1 =
				nkf_char_unicode_new((c2 - 0xF0) * 188 + (c1 - 0x40 - (0x7E < c1)) + 0xE000);
		c2 = 0;
	} else {
		nkf_char ret = Util::S2eConv(c2, c1, &c2, &c1, nkfFlags);
		if (ret)
			return ret;
	}
	this->Oconv(c2, c1, nkfFlags, oConvStr);
	return 0;
}

nkf_char NKFNativeEncoding::EIconv(nkf_char c2, nkf_char c1, nkf_char c0,
		std::bitset<nkf_flag_num> nkfFlags, std::wstring* oConvStr) {

	if (c2 == JIS_X_0201_1976_K || c2 == SS2) {
		if (nkfFlags[iso2022jp_f] && !nkfFlags[x0201_f]) {
			c2 = GETA1;
			c1 = GETA2;
		} else {
			c2 = JIS_X_0201_1976_K;
			c1 &= 0x7f;
		}
	} else if (c2 == 0x8f) {
		if (c0 == 0) {
			return -1;
		}
		if (!nkfFlags[cp51932_f] && !nkfFlags[x0213_f] && 0xF5 <= c1
				&& c1 <= 0xFE && 0xA1 <= c0 && c0 <= 0xFE) {
			/* encoding is eucJP-ms, so invert to Unicode Private User Area */
			c1 = nkf_char_unicode_new((c1 - 0xF5) * 94 + c0 - 0xA1 + 0xE3AC);
			c2 = 0;
		} else {
			c2 = (c2 << 8) | (c1 & 0x7f);
			c1 = c0 & 0x7f;
			if (nkfFlags[cp51932_f]) {
				nkf_char s2, s1;
				if (Util::E2sConv(c2, c1, &s2, &s1, nkfFlags) == 0) {
					Util::S2eConv(s2, s1, &c2, &c1, nkfFlags);
					if (c2 < 0x100) {
						c1 &= 0x7f;
						c2 &= 0x7f;
					}
				}
			}
		}
	} else if ((c2 == EOF) || (c2 == 0) || c2 < SP || c2 == ISO_8859_1) {
		/* NOP */
	} else {
		if (!nkfFlags[cp51932_f] && nkfFlags[ms_ucs_map_f] && 0xF5 <= c2
				&& c2 <= 0xFE && 0xA1 <= c1 && c1 <= 0xFE) {
			/* encoding is eucJP-ms, so invert to Unicode Private User Area */
			c1 = nkf_char_unicode_new((c2 - 0xF5) * 94 + c1 - 0xA1 + 0xE000);
			c2 = 0;
		} else {
			c1 &= 0x7f;
			c2 &= 0x7f;
			if (nkfFlags[cp51932_f] && 0x79 <= c2 && c2 <= 0x7c) {
				nkf_char s2, s1;
				if (Util::E2sConv(c2, c1, &s2, &s1, nkfFlags) == 0) {
					Util::S2eConv(s2, s1, &c2, &c1, nkfFlags);
					if (c2 < 0x100) {
						c1 &= 0x7f;
						c2 &= 0x7f;
					}
				}
			}
		}
	}
	this->Oconv(c2, c1, nkfFlags, oConvStr);
	return 0;
}

nkf_char NKFNativeEncoding::WIconv(nkf_char c1, nkf_char c2, nkf_char c3,
		std::bitset<nkf_flag_num> nkfFlags, std::wstring* oConvStr) {

	nkf_char ret = 0, c4 = 0;
	static const char w_iconv_utf8_1st_byte[] = { /* 0xC0 - 0xFF */
	20, 20, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21,
			21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 30, 31, 31, 31,
			31, 31, 31, 31, 31, 31, 31, 31, 31, 32, 33, 33, 40, 41, 41, 41, 42,
			43, 43, 43, 50, 50, 50, 50, 60, 60, 70, 70 };

	if (c3 > 0xFF) {
		c4 = c3 & 0xFF;
		c3 >>= 8;
	}

	if (c1 < 0 || 0xff < c1) {
	} else if (c1 == 0) { /* 0 : 1 byte*/
		c3 = 0;
	} else if ((c1 & 0xC0) == 0x80) { /* 0x80-0xbf : trail byte */
		return 0;
	} else {
		switch (w_iconv_utf8_1st_byte[c1 - 0xC0]) {
		case 21:
			if (c2 < 0x80 || 0xBF < c2)
				return 0;
			break;
		case 30:
			if (c3 == 0)
				return -1;
			if (c2 < 0xA0 || 0xBF < c2 || (c3 & 0xC0) != 0x80)
				return 0;
			break;
		case 31:
		case 33:
			if (c3 == 0)
				return -1;
			if ((c2 & 0xC0) != 0x80 || (c3 & 0xC0) != 0x80)
				return 0;
			break;
		case 32:
			if (c3 == 0)
				return -1;
			if (c2 < 0x80 || 0x9F < c2 || (c3 & 0xC0) != 0x80)
				return 0;
			break;
		case 40:
			if (c3 == 0)
				return -2;
			if (c2 < 0x90 || 0xBF < c2 || (c3 & 0xC0) != 0x80
					|| (c4 & 0xC0) != 0x80)
				return 0;
			break;
		case 41:
			if (c3 == 0)
				return -2;
			if (c2 < 0x80 || 0xBF < c2 || (c3 & 0xC0) != 0x80
					|| (c4 & 0xC0) != 0x80)
				return 0;
			break;
		case 42:
			if (c3 == 0)
				return -2;
			if (c2 < 0x80 || 0x8F < c2 || (c3 & 0xC0) != 0x80
					|| (c4 & 0xC0) != 0x80)
				return 0;
			break;
		default:
			return 0;
			break;
		}
	}
	if (c1 == 0 || c1 == EOF) {
	} else if ((c1 & 0xf8) == 0xf0) { /* 4 bytes */
		c2 = nkf_char_unicode_new(Util::NKFUTF8ToUnicode(c1, c2, c3, c4));
		c1 = 0;
	} else {
		ret = Util::W2eConv(c1, c2, c3, &c1, &c2, nkfFlags);
	}
	if (ret == 0) {
		this->Oconv(c1, c2, nkfFlags, oConvStr);
	}
	return ret;
}

nkf_char NKFNativeEncoding::WIconv16(nkf_char c2, nkf_char c1, nkf_char c0,
		std::bitset<nkf_flag_num> nkfFlags, std::wstring* oConvStr) {

	this->Oconv(c2, c1, nkfFlags, oConvStr);
	return 16; /* different from w_iconv32 */
}

nkf_char NKFNativeEncoding::WIconv32(nkf_char c2, nkf_char c1, nkf_char c0,
		std::bitset<nkf_flag_num> nkfFlags, std::wstring* oConvStr) {

	this->Oconv(c2, c1, nkfFlags, oConvStr);
	return 32; /* different from w_iconv16 */
}

void NKFNativeEncoding::JOconv(nkf_char c2, nkf_char c1,
		std::bitset<nkf_flag_num> nkfFlags, std::wstring* oConvStr) {

	if (c2 == 0 && nkf_char_unicode_p(c1)) {
		Util::W16eConv(c1, &c2, &c1, nkfFlags);
		if (c2 == 0 && nkf_char_unicode_p(c1)) {
			c2 = c1 & VALUE_MASK;
			if (nkfFlags[ms_ucs_map_f] && 0xE000 <= c2 && c2 <= 0xE757) {
				/* CP5022x UDC */
				c1 &= 0xFFF;
				c2 = 0x7F + c1 / 94;
				c1 = 0x21 + c1 % 94;
			} else {
				//if (encode_fallback)
				//	(*encode_fallback)(c1);
				//return;
			}
		}
	}
	if (c2 == 0) {
		OutputAsciiEscapeSequence(ASCII, oConvStr);
		oConvStr->push_back(c1);
	} else if (c2 == EOF) {
		OutputAsciiEscapeSequence(ASCII, oConvStr);
		oConvStr->push_back(EOF);
	} else if (c2 == ISO_8859_1) {
		OutputAsciiEscapeSequence(ISO_8859_1, oConvStr);
		oConvStr->push_back(c1 | 0x80);
	} else if (c2 == JIS_X_0201_1976_K) {
		OutputAsciiEscapeSequence(JIS_X_0201_1976_K, oConvStr);
		oConvStr->push_back(c1);
	} else if (is_eucg3(c2)) {
		OutputAsciiEscapeSequence(nkfFlags[x0213_f] ? JIS_X_0213_2 : JIS_X_0212,
				oConvStr);
		oConvStr->push_back(c2 & 0x7f);
		oConvStr->push_back(c1);
	} else {
		if (nkfFlags[ms_ucs_map_f] ?
				c2 < 0x20 || 0x92 < c2 || c1 < 0x20 || 0x7e < c1 :
				c2 < 0x20 || 0x7e < c2 || c1 < 0x20 || 0x7e < c1)
			return;
		OutputAsciiEscapeSequence(
				nkfFlags[x0213_f] ? JIS_X_0213_1 : JIS_X_0208, oConvStr);
		oConvStr->push_back(c2);
		oConvStr->push_back(c1);
	}
}

void NKFNativeEncoding::SOconv(nkf_char c2, nkf_char c1,
		std::bitset<nkf_flag_num> nkfFlags, std::wstring* oConvStr) {

	if (c2 == 0 && nkf_char_unicode_p(c1)) {
		Util::W16eConv(c1, &c2, &c1, nkfFlags);
		if (c2 == 0 && nkf_char_unicode_p(c1)) {
			c2 = c1 & VALUE_MASK;
			if (!nkfFlags[x0213_f] && 0xE000 <= c2 && c2 <= 0xE757) {
				/* CP932 UDC */
				c1 &= 0xFFF;
				c2 = c1 / 188 + (nkfFlags[cp932inv_f] ? 0xF0 : 0xEB);
				c1 = c1 % 188;
				c1 += 0x40 + (c1 > 0x3e);
				oConvStr->push_back(c2);
				oConvStr->push_back(c1);
				return;
			} else {
				//if (encode_fallback)
				//	(*encode_fallback)(c1);
				//return;
			}
		}
	}
	if (c2 == EOF) {
		oConvStr->push_back(EOF);
		return;
	} else if (c2 == 0) {
		this->outputMode = ASCII;
		oConvStr->push_back(c1);
	} else if (c2 == JIS_X_0201_1976_K) {
		this->outputMode = SHIFT_JIS;
		oConvStr->push_back(c1 | 0x80);
	} else if (c2 == ISO_8859_1) {
		this->outputMode = ISO_8859_1;
		oConvStr->push_back(c1 | 0x080);
	} else if (is_eucg3(c2)) {
		this->outputMode = SHIFT_JIS;
		if (Util::E2sConv(c2, c1, &c2, &c1, nkfFlags) == 0) {
			oConvStr->push_back(c2);
			oConvStr->push_back(c1);
		}
	} else {
		if (!nkf_isprint(c1) || !nkf_isprint(c2)) {
			// fix me !
			//LibNKF::SetIconv(FALSE, 0, flagPool);
			return; /* too late to rescue this char */
		}
		this->outputMode = SHIFT_JIS;
		Util::E2sConv(c2, c1, &c2, &c1, nkfFlags);

		if (nkfFlags[cp932inv_f]
				&& CP932INV_TABLE_BEGIN <= c2&& c2 <= CP932INV_TABLE_END) {
			nkf_char c = UTF8Table::cp932inv[c2 - CP932INV_TABLE_BEGIN][c1
					- 0x40];
			if (c) {
				c2 = c >> 8;
				c1 = c & 0xff;
			}
		}

		oConvStr->push_back(c2);
		//if (LibNKF::prefix_table[(unsigned char) c1]) {
		//	LibNKF::OPutC(LibNKF::prefix_table[(unsigned char) c1]);
		//}
		oConvStr->push_back(c1);
	}
}

void NKFNativeEncoding::EOconv(nkf_char c2, nkf_char c1,
		std::bitset<nkf_flag_num> nkfFlags, std::wstring* oConvStr) {

	if (c2 == 0 && nkf_char_unicode_p(c1)) {
		Util::W16eConv(c1, &c2, &c1, nkfFlags);
		if (c2 == 0 && nkf_char_unicode_p(c1)) {
			c2 = c1 & VALUE_MASK;
			if (nkfFlags[x0212_f] && 0xE000 <= c2 && c2 <= 0xE757) {
				/* eucJP-ms UDC */
				c1 &= 0xFFF;
				c2 = c1 / 94;
				c2 += c2 < 10 ? 0x75 : 0x8FEB;
				c1 = 0x21 + c1 % 94;
				if (is_eucg3(c2)) {
					oConvStr->push_back(0x8f);
					oConvStr->push_back((c2 & 0x7f) | 0x080);
					oConvStr->push_back(c1 | 0x080);
				} else {
					oConvStr->push_back((c2 & 0x7f) | 0x080);
					oConvStr->push_back(c1 | 0x080);
				}
				return;
			} else {
				//if (encode_fallback)
				//	(*encode_fallback)(c1);
				//return;
			}
		}
	}

	if (c2 == EOF) {
		oConvStr->push_back(EOF);
	} else if (c2 == 0) {
		this->outputMode = ASCII;
		oConvStr->push_back(c1);
	} else if (c2 == JIS_X_0201_1976_K) {
		this->outputMode = EUC_JP;
		oConvStr->push_back(SS2);
		oConvStr->push_back(c1 | 0x80);
	} else if (c2 == ISO_8859_1) {
		this->outputMode = ISO_8859_1;
		oConvStr->push_back(c1 | 0x080);
	} else if (is_eucg3(c2)) {
		this->outputMode = EUC_JP;
		if (!nkfFlags[cp932inv_f]) {
			nkf_char s2, s1;
			if (Util::E2sConv(c2, c1, &s2, &s1, nkfFlags) == 0) {
				Util::S2eConv(s2, s1, &c2, &c1, nkfFlags);
			}
		}
		if (c2 == 0) {
			this->outputMode = ASCII;
			oConvStr->push_back(c1);
		} else if (is_eucg3(c2)) {
			if (nkfFlags[x0212_f]) {
				oConvStr->push_back(0x8f);
				oConvStr->push_back((c2 & 0x7f) | 0x080);
				oConvStr->push_back(c1 | 0x080);
			}
		} else {
			oConvStr->push_back((c2 & 0x7f) | 0x080);
			oConvStr->push_back(c1 | 0x080);
		}
	} else {
		if (!nkf_isgraph(c1) || !nkf_isgraph(c2)) {
			// fix me !
			//LibNKF::SetIconv(FALSE, 0, flagPool);
			return; /* too late to rescue this char */
		}
		this->outputMode = EUC_JP;
		oConvStr->push_back(c2 | 0x080);
		oConvStr->push_back(c1 | 0x080);
	}
}

void NKFNativeEncoding::WOconv(nkf_char c2, nkf_char c1,
		std::bitset<nkf_flag_num> nkfFlags, std::wstring* oConvStr) {

	nkf_char c3, c4;
	nkf_char val;

	if (nkfFlags[output_bom_f]) {
		nkfFlags[output_bom_f] = FALSE;
		oConvStr->push_back('\357');
		oConvStr->push_back('\273');
		oConvStr->push_back('\277');
	}

	if (c2 == EOF) {
		oConvStr->push_back(EOF);
		return;
	}

	if (c2 == 0 && nkf_char_unicode_p(c1)) {
		val = c1 & VALUE_MASK;
		Util::NKFUnicodeToUTF8(val, &c1, &c2, &c3, &c4);
		oConvStr->push_back(c1);
		if (c2)
			oConvStr->push_back(c2);
		if (c3)
			oConvStr->push_back(c3);
		if (c4)
			oConvStr->push_back(c4);
		return;
	}

	if (c2 == 0) {
		oConvStr->push_back(c1);
	} else {
		val = Util::E2wConv(c2, c1, nkfFlags);
		if (val) {
			Util::NKFUnicodeToUTF8(val, &c1, &c2, &c3, &c4);
			oConvStr->push_back(c1);
			if (c2)
				oConvStr->push_back(c2);
			if (c3)
				oConvStr->push_back(c3);
			if (c4)
				oConvStr->push_back(c4);
		}
	}
}

void NKFNativeEncoding::WOconv16(nkf_char c2, nkf_char c1,
		std::bitset<nkf_flag_num> nkfFlags, std::wstring* oConvStr) {

	if (nkfFlags[output_bom_f]) {
		nkfFlags[output_bom_f] = FALSE;
		if (this->endian == ENDIAN_LITTLE) {
			oConvStr->push_back(0xFF);
			oConvStr->push_back(0xFE);
		} else {
			oConvStr->push_back(0xFE);
			oConvStr->push_back(0xFF);
		}
	}

	if (c2 == EOF) {
		oConvStr->push_back(EOF);
		return;
	}

	if (c2 == 0 && nkf_char_unicode_p(c1)) {
		if (nkf_char_unicode_bmp_p(c1)) {
			c2 = (c1 >> 8) & 0xff;
			c1 &= 0xff;
		} else {
			c1 &= VALUE_MASK;
			if (c1 <= UNICODE_MAX) {
				c2 = (c1 >> 10) + NKF_INT32_C(0xD7C0); /* high surrogate */
				c1 = (c1 & 0x3FF) + NKF_INT32_C(0xDC00); /* low surrogate */
				if (this->endian == ENDIAN_LITTLE) {
					oConvStr->push_back(c2 & 0xff);
					oConvStr->push_back((c2 >> 8) & 0xff);
					oConvStr->push_back(c1 & 0xff);
					oConvStr->push_back((c1 >> 8) & 0xff);
				} else {
					oConvStr->push_back((c2 >> 8) & 0xff);
					oConvStr->push_back(c2 & 0xff);
					oConvStr->push_back((c1 >> 8) & 0xff);
					oConvStr->push_back(c1 & 0xff);
				}
			}
			return;
		}
	} else if (c2) {
		nkf_char val = Util::E2wConv(c2, c1, nkfFlags);
		c2 = (val >> 8) & 0xff;
		c1 = val & 0xff;
		if (!val)
			return;
	}

	if (this->endian == ENDIAN_LITTLE) {
		oConvStr->push_back(c1);
		oConvStr->push_back(c2);
	} else {
		oConvStr->push_back(c2);
		oConvStr->push_back(c1);
	}
}

void NKFNativeEncoding::WOconv32(nkf_char c2, nkf_char c1,
		std::bitset<nkf_flag_num> nkfFlags, std::wstring* oConvStr) {

	if (nkfFlags[output_bom_f]) {
		nkfFlags[output_bom_f] = FALSE;
		if (this->endian == ENDIAN_LITTLE) {
			oConvStr->push_back(0xFF);
			oConvStr->push_back(0xFE);
			oConvStr->push_back(0);
			oConvStr->push_back(0);
		} else {
			oConvStr->push_back(0);
			oConvStr->push_back(0);
			oConvStr->push_back(0xFE);
			oConvStr->push_back(0xFF);
		}
	}

	if (c2 == EOF) {
		oConvStr->push_back(EOF);
		return;
	}

	if (c2 == ISO_8859_1) {
		c1 |= 0x80;
	} else if (c2 == 0 && nkf_char_unicode_p(c1)) {
		c1 &= VALUE_MASK;
	} else if (c2) {
		c1 = Util::E2wConv(c2, c1, nkfFlags);
		if (!c1)
			return;
	}
	if (this->endian == ENDIAN_LITTLE) {
		oConvStr->push_back(c1 & 0xFF);
		oConvStr->push_back((c1 >> 8) & 0xFF);
		oConvStr->push_back((c1 >> 16) & 0xFF);
		oConvStr->push_back(0);
	} else {
		oConvStr->push_back(0);
		oConvStr->push_back((c1 >> 16) & 0xFF);
		oConvStr->push_back((c1 >> 8) & 0xFF);
		oConvStr->push_back(c1 & 0xFF);
	}
}
/**
 * Asciiコードが混じった場合のエスケープシーケンス
 */
void NKFNativeEncoding::OutputAsciiEscapeSequence(int mode,
		std::wstring* oConvStr) {
	if (this->outputMode != ASCII && this->outputMode != ISO_8859_1) {
		oConvStr->push_back(ESC);
		oConvStr->push_back('(');
		oConvStr->push_back(ascii_intro);
		this->outputMode = mode;
	}
}


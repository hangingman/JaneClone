/*
 * util.cpp
 *
 * nkf common method
 *  Created on: 2012/07/12
 * Contributor: Hiroyuki Nagata
 */

#include "util.h"

/**
 * judge baseID and return wxNKFEncoding instance
 * 0: set input
 * 1: set output
 */
void Util::NKFEncFind(const char* name, wxNKFEncoding* enc, int io) {
	// setting unique encode index(int)
	int idx = -1;
	// encode name to encode index
	idx = NKFEncFindIndex(name);
	// set data to encode class instance
	NKFEncFromIndex(idx, enc, io);
}
/**
 * return wxNKFEncoding class per encoding ID
 */
void Util::NKFEncFromIndex(int idx, wxNKFEncoding* enc, int io) {

	/**
	 * base name & base id
	 */
	int baseId;
	wxString baseName;
	/**
	 * unique name & unique id
	 */
	int uniqueId = idx;
	wxString uniqueName;

	/*
	 *  set unique encode id and base encoding
	 */
	if (idx == ASCII || idx == ISO_8859_1 || idx == BINARY) {
		// NkfEncodingASCII
		baseId = ASCII;
		baseName = wxT("ASCII");

		// set id
		switch (idx) {
		case ASCII:
			uniqueName = wxT("US-ASCII");
			break;
		case ISO_8859_1:
			uniqueName = wxT("ISO-8859-1");
			break;
		case BINARY:
			uniqueName = wxT("BINARY");
			break;
		}

	} else if (idx == ISO_2022_JP || idx == CP50220 || idx == CP50221
			|| idx == CP50222 || idx == ISO_2022_JP_1 || idx == ISO_2022_JP_3
			|| idx == ISO_2022_JP_2004) {
		// NkfEncodingISO_2022_JP
		baseId = ISO_2022_JP;
		baseName = wxT("ISO-2022-JP");

		// set id
		switch (idx) {
		case ISO_2022_JP:
			uniqueName = wxT("ISO-2022-JP");
			break;
		case CP50220:
			uniqueName = wxT("CP50220");
			break;
		case CP50221:
			uniqueName = wxT("CP50221");
			break;
		case CP50222:
			uniqueName = wxT("CP50222");
			break;
		case ISO_2022_JP_1:
			uniqueName = wxT("ISO-2022-JP-1");
			break;
		case ISO_2022_JP_3:
			uniqueName = wxT("ISO-2022-JP-3");
			break;
		case ISO_2022_JP_2004:
			uniqueName = wxT("ISO-2022-JP-2004");
			break;
		}

	} else if (idx == SHIFT_JIS || idx == WINDOWS_31J || idx == CP10001
			|| idx == SHIFT_JISX0213 || idx == SHIFT_JIS_2004) {
		// NkfEncodingShift_JIS
		baseId = SHIFT_JIS;
		baseName = wxT("Shift_JIS");

		// set id
		switch (idx) {
		case SHIFT_JIS:
			uniqueName = wxT("Shift_JIS");
			break;
		case WINDOWS_31J:
			uniqueName = wxT("Windows-31J");
			break;
		case CP10001:
			uniqueName = wxT("CP10001");
			break;
		case SHIFT_JISX0213:
			uniqueName = wxT("Shift_JISX0213");
			break;
		case SHIFT_JIS_2004:
			uniqueName = wxT("Shift_JIS-2004");
			break;
		}

	} else if (idx == EUC_JP || idx == EUCJP_NKF || idx == CP51932
			|| idx == EUCJP_MS || idx == EUCJP_ASCII || idx == EUC_JISX0213
			|| idx == EUC_JIS_2004) {
		// NkfEncodingEUC_JP
		baseId = EUC_JP;
		baseName = wxT("EUC-JP");

		// set id
		switch (idx) {
		case EUC_JP:
			uniqueName = wxT("EUC-JP");
			break;
		case EUCJP_NKF:
			uniqueName = wxT("eucJP-nkf");
			break;
		case CP51932:
			uniqueName = wxT("CP51932");
			break;
		case EUCJP_MS:
			uniqueName = wxT("eucJP-MS");
			break;
		case EUCJP_ASCII:
			uniqueName = wxT("eucJP-ASCII");
			break;
		case EUC_JISX0213:
			uniqueName = wxT("EUC-JISX0213");
			break;
		case EUC_JIS_2004:
			uniqueName = wxT("EUC-JIS-2004");
			break;
		}

	} else if (idx == UTF_8 || idx == UTF_8N || idx == UTF_8_BOM
			|| idx == UTF8_MAC) {
		// NkfEncodingUTF_8
		baseId = UTF_8;
		baseName = wxT("UTF-8");

		// set id
		switch (idx) {
		case UTF_8:
			uniqueName = wxT("UTF-8");
			break;
		case UTF_8N:
			uniqueName = wxT("UTF-8N");
			break;
		case UTF_8_BOM:
			uniqueName = wxT("UTF-8-BOM");
			break;
		case UTF8_MAC:
			uniqueName = wxT("UTF8-MAC");
			break;
		}

	} else if (idx == UTF_16 || idx == UTF_16BE || idx == UTF_16BE_BOM
			|| idx == UTF_16LE || idx == UTF_16LE_BOM) {
		// NkfEncodingUTF_16
		baseId = UTF_16;
		baseName = wxT("UTF-16");

		// set id
		switch (idx) {
		case UTF_16:
			uniqueName = wxT("UTF-16");
			break;
		case UTF_16BE:
			uniqueName = wxT("UTF-16BE");
			break;
		case UTF_16BE_BOM:
			uniqueName = wxT("UTF-16BE-BOM");
			break;
		case UTF_16LE:
			uniqueName = wxT("UTF-16LE");
			break;
		case UTF_16LE_BOM:
			uniqueName = wxT("UTF-16LE-BOM");
			break;
		}

	} else if (idx == UTF_32 || idx == UTF_32BE || idx == UTF_32BE_BOM
			|| idx == UTF_32LE || idx == UTF_32LE_BOM) {
		// NkfEncodingUTF_32
		baseId = UTF_32;
		baseName = wxT("UTF-32");

		// set id
		switch (idx) {
		case UTF_32:
			uniqueName = wxT("UTF-32");
			break;
		case UTF_32BE:
			uniqueName = wxT("UTF-32BE");
			break;
		case UTF_32BE_BOM:
			uniqueName = wxT("UTF-32BE-BOM");
			break;
		case UTF_32LE:
			uniqueName = wxT("UTF-32LE");
			break;
		case UTF_32LE_BOM:
			uniqueName = wxT("UTF-32LE-BOM");
			break;
		}
	} else {
		// idx is abnormal
		baseId = -1;
		baseName = wxEmptyString;
		enc = 0;
	}

	// set data to encode instance
	if (SET_INPUT_MODE == io) {
		enc->inputMode = baseId;
		enc->inputBaseName = baseName;
		enc->iCharID = idx;
		enc->iCharName = uniqueName;
	}

	if (SET_OUTPUT_MODE == io) {
		enc->outputMode = baseId;
		enc->outputBaseName = baseName;
		enc->oCharID = idx;
		enc->iCharName = uniqueName;
	}
}
/**
 * get encode ID by name
 */
int Util::NKFEncFindIndex(const char *name) {

	if (name[0] == 'X' && *(name + 1) == '-')
		name += 2;

	// If NULL, force to error
	if (name == 0)
		return -1;

	int id = 0;

	switch (name[0]) {

	case '6':
		id = ASCII;
		break;

	case 'A':
		id = ASCII;
		break;

	case 'B':
		id = BINARY;
		break;

	case 'C': // case of 'C'
		switch (name[1]) {

		case 'P': // "CP**"
			if (StrncmpFromHead("CP50220", name)) {
				id = CP50220;
			} else if (StrncmpFromHead("CP50221", name)) {
				id = CP50221;
			} else if (StrncmpFromHead("CP50222", name)) {
				id = CP50222;
			} else if (StrncmpFromHead("CP932", name)) {
				id = WINDOWS_31J;
			} else if (StrncmpFromHead("CP10001", name)) {
				id = CP10001;
			} else if (StrncmpFromHead("CP51932", name)) {
				id = CP51932;
			}
			break;

		case 'S': // "CS**"
			if (StrncmpFromHead("CSISO2022JP", name)) {
				id = CP50221;
			} else if (StrncmpFromHead("CSWINDOWS31J", name)) {
				id = WINDOWS_31J;
			}
			break;
		}
		break;

	case 'E': // case of 'E' EUC**
		switch (name[3]) {

		case 'J': // "EUCJ**"
			if (StrncmpFromHead("EUCJP", name)) {
				id = EUC_JP;
			} else if (StrncmpFromHead("EUCJP-NKF", name)) {
				id = EUCJP_NKF;
			} else if (StrncmpFromHead("EUCJP-MS", name)) {
				id = EUCJP_MS;
			} else if (StrncmpFromHead("EUCJPMS", name)) {
				id = EUCJP_MS;
			} else if (StrncmpFromHead("EUCJP-ASCII", name)) {
				id = EUCJP_ASCII;
			}
			break;

		case '-': // "EUC-**"
			if (StrncmpFromHead("EUC-JP", name)) {
				id = EUC_JP;
			} else if (StrncmpFromHead("EUC-JP-MS", name)) {
				id = EUCJP_MS;
			} else if (StrncmpFromHead("EUC-JP-ASCII", name)) {
				id = EUCJP_ASCII;
			} else if (StrncmpFromHead("EUC-JISX0213", name)) {
				id = EUC_JISX0213;
			} else if (StrncmpFromHead("EUC-JIS-2004", name)) {
				id = EUC_JIS_2004;
			}
			break;
		}
		break;

	case 'I': // case of 'I' ISO**
		if (StrncmpFromHead("ISO-2022-JP", name)) {
			id = ISO_2022_JP;
		} else if (StrncmpFromHead("ISO2022JP-CP932", name)) {
			id = CP50220;
		} else if (StrncmpFromHead("ISO-2022-JP-1", name)) {
			id = ISO_2022_JP_1;
		} else if (StrncmpFromHead("ISO-2022-JP-3", name)) {
			id = ISO_2022_JP_3;
		} else if (StrncmpFromHead("ISO-2022-JP-2004", name)) {
			id = ISO_2022_JP_2004;
		}
		break;

	case 'M': // case of 'M'
		if (StrncmpFromHead("MS_Kanji", name)) {
			id = SHIFT_JIS;
		} else if (StrncmpFromHead("MS932", name)) {
			id = WINDOWS_31J;
		}
		break;

	case 'P':
		id = SHIFT_JIS;
		break;

	case 'R':
		id = ASCII;
		break;

	case 'S': // case of 'S'
		if (StrncmpFromHead("SHIFT_JIS", name)) {
			id = SHIFT_JIS;
		} else if (StrncmpFromHead("SJIS", name)) {
			id = SHIFT_JIS;
		} else if (StrncmpFromHead("SHIFT_JISX0213", name)) {
			id = SHIFT_JISX0213;
		} else if (StrncmpFromHead("SHIFT_JIS-2004", name)) {
			id = SHIFT_JIS_2004;
		}
		break;

	case 'U': // case of 'U'

		switch (name[4]) { // "UTF-*"

		case '8':
			if (StrncmpFromHead("UTF-8", name)) {
				id = UTF_8;
			} else if (StrncmpFromHead("UTF-8N", name)) {
				id = UTF_8N;
			} else if (StrncmpFromHead("UTF-8-BOM", name)) {
				id = UTF_8_BOM;
			} else if (StrncmpFromHead("UTF-8-MAC", name)) {
				id = UTF8_MAC;
			}
			break;

		case '-':
			id = UTF8_MAC;
			break;

		case '1':
			if (StrncmpFromHead("UTF-16", name)) {
				id = UTF_16;
			} else if (StrncmpFromHead("UTF-16BE", name)) {
				id = UTF_16BE;
			} else if (StrncmpFromHead("UTF-16BE-BOM", name)) {
				id = UTF_16BE_BOM;
			} else if (StrncmpFromHead("UTF-16LE", name)) {
				id = UTF_16LE;
			} else if (StrncmpFromHead("UTF-16LE-BOM", name)) {
				id = UTF_16LE_BOM;
			}
			break;

		case '3':
			if (StrncmpFromHead("UTF-32", name)) {
				id = UTF_32;
			} else if (StrncmpFromHead("UTF-32BE", name)) {
				id = UTF_32BE;
			} else if (StrncmpFromHead("UTF-32BE-BOM", name)) {
				id = UTF_32BE_BOM;
			} else if (StrncmpFromHead("UTF-32LE", name)) {
				id = UTF_32LE;
			} else if (StrncmpFromHead("UTF-32LE-BOM", name)) {
				id = UTF_32LE_BOM;
			}
			break;
		}
		break;

	case 'W':
		id = WINDOWS_31J;
		break;
	}

	return id;
}
/**
 * Initialize character code
 */
void Util::NKFDefaultEncoding(wxNKFEncoding* enc, int io) {

#ifdef DEFAULT_CODE_LOCALE
	NKFLocaleEncoding(enc, io);
#elif defined(DEFAULT_ENCIDX)
	Util::NKFEncFromIndex(DEFAULT_ENCIDX, enc, io);
#endif
}
/**
 * convert EUC-JP to UTF-8
 */
nkf_char Util::E2wConv(nkf_char c2, nkf_char c1,
		std::bitset<nkf_flag_num> nkfFlags) {

	const unsigned short *p;

	if (c2 == JIS_X_0201_1976_K) {
		if (nkfFlags[ms_ucs_map_f] == UCS_MAP_CP10001) {
			switch (c1) {
			case 0x20:
				return 0xA0;
			case 0x7D:
				return 0xA9;
			}
		}
		p = UTF8Table::euc_to_utf8_1byte;
	} else if (is_eucg3(c2)) {
		if (nkfFlags[ms_ucs_map_f] == UCS_MAP_ASCII && c2 == NKF_INT32_C(0x8F22)
				&& c1 == 0x43) {
			return 0xA6;
		}
		c2 = (c2 & 0x7f) - 0x21;
		if (0 <= c2 && c2 < sizeof_euc_to_utf8_2bytes)
			p = UTF8Table::x0212_to_utf8_2bytes[c2];
		else
			return 0;
	} else {
		c2 &= 0x7f;
		c2 = (c2 & 0x7f) - 0x21;
		if (0 <= c2 && c2 < sizeof_euc_to_utf8_2bytes)
			p = nkfFlags[ms_ucs_map_f] == UCS_MAP_ASCII ?
					UTF8Table::euc_to_utf8_2bytes[c2] :
				nkfFlags[ms_ucs_map_f] == UCS_MAP_CP10001 ?
						UTF8Table::euc_to_utf8_2bytes_mac[c2] :
						UTF8Table::euc_to_utf8_2bytes_ms[c2];
		else
			return 0;
	}
	if (!p)
		return 0;
	c1 = (c1 & 0x7f) - 0x21;
	if (0 <= c1 && c1 < sizeof_euc_to_utf8_1byte)
		return p[c1];
	return 0;
}
/**
 * convert Shift_JIS to EUC-JP
 */
nkf_char Util::S2eConv(nkf_char c2, nkf_char c1, nkf_char* p2, nkf_char* p1,
		std::bitset<nkf_flag_num> nkfFlags) {

	nkf_char val;
	static const char shift_jisx0213_s1a3_table[5][2] = { { 1, 8 }, { 3, 4 }, {
			5, 12 }, { 13, 14 }, { 15, 0 } };
	if (0xFC < c1)
		return 1;

	if (!nkfFlags[cp932inv_f] && is_ibmext_in_sjis(c2)) {
		UTF8Table* table;
		val = table->shiftjis_cp932[c2 - CP932_TABLE_BEGIN][c1 - 0x40];
		if (val) {
			c2 = val >> 8;
			c1 = val & 0xff;
		}
	}
	if (nkfFlags[cp932inv_f]
			&& CP932INV_TABLE_BEGIN <= c2&& c2 <= CP932INV_TABLE_END) {
		UTF8Table* table;
		val = table->cp932inv[c2 - CP932INV_TABLE_BEGIN][c1 - 0x40];
		if (val) {
			c2 = val >> 8;
			c1 = val & 0xff;
		}
	}
	if (!nkfFlags[x0213_f] && is_ibmext_in_sjis(c2)) {
		UTF8Table* table;
		val = table->shiftjis_x0212[c2 - 0xfa][c1 - 0x40];
		if (val) {
			if (val > 0x7FFF) {
				c2 = PREFIX_EUCG3 | ((val >> 8) & 0x7f);
				c1 = val & 0xff;
			} else {
				c2 = val >> 8;
				c1 = val & 0xff;
			}
			if (p2)
				*p2 = c2;
			if (p1)
				*p1 = c1;
			return 0;
		}
	}
	if (c2 >= 0x80) {
		if (nkfFlags[x0213_f] && c2 >= 0xF0) {
			if (c2 <= 0xF3 || (c2 == 0xF4 && c1 < 0x9F)) { /* k=1, 3<=k<=5, k=8, 12<=k<=15 */
				c2 = PREFIX_EUCG3 | 0x20
						| shift_jisx0213_s1a3_table[c2 - 0xF0][0x9E < c1];
			} else { /* 78<=k<=94 */
				c2 = PREFIX_EUCG3 | (c2 * 2 - 0x17B);
				if (0x9E < c1)
					c2++;
			}
		} else {
#define         SJ0162  0x00e1          /* 01 - 62 ku offset */
#define         SJ6394  0x0161          /* 63 - 94 ku offset */
			c2 = c2 + c2 - ((c2 <= 0x9F) ? SJ0162 : SJ6394);
			if (0x9E < c1)
				c2++;
		}
		if (c1 < 0x9F)
			c1 = c1 - ((c1 > DEL) ? SP : 0x1F);
		else {
			c1 = c1 - 0x7E;
		}
	}
	c2 = Util::X0212Unshift(c2);
	if (p2)
		*p2 = c2;
	if (p1)
		*p1 = c1;
	return 0;
}
/**
 * convert UTF-8 to EUC-JP
 */
nkf_char Util::W2eConv(nkf_char c2, nkf_char c1, nkf_char c0, nkf_char* p2,
		nkf_char* p1, std::bitset<nkf_flag_num> nkfFlags) {

	nkf_char ret = 0;

	if (!c1) {
		*p2 = 0;
		*p1 = c2;
	} else if (0xc0 <= c2 && c2 <= 0xef) {
		ret = Util::UnicodeToJISCommon(c2, c1, c0, p2, p1, nkfFlags);
		if (ret > 0) {
			if (p2)
				*p2 = 0;
			if (p1)
				*p1 = nkf_char_unicode_new(
						Util::NKFUTF8ToUnicode(c2, c1, c0, 0));
			ret = 0;
		}
	}
	return ret;
}
/**
 * convert UTF-8 to Shift_JIS
 */
nkf_char Util::E2sConv(nkf_char c2, nkf_char c1, nkf_char* p2, nkf_char* p1,
		std::bitset<nkf_flag_num> nkfFlags) {

	nkf_char ndx;
	if (is_eucg3(c2)) {
		ndx = c2 & 0x7f;
		if (nkfFlags[x0213_f]) {
			if ((0x21 <= ndx && ndx <= 0x2F)) {
				if (p2)
					*p2 = ((ndx - 1) >> 1) + 0xec - ndx / 8 * 3;
				if (p1)
					*p1 = c1 + ((ndx & 1) ? ((c1 < 0x60) ? 0x1f : 0x20) : 0x7e);
				return 0;
			} else if (0x6E <= ndx && ndx <= 0x7E) {
				if (p2)
					*p2 = ((ndx - 1) >> 1) + 0xbe;
				if (p1)
					*p1 = c1 + ((ndx & 1) ? ((c1 < 0x60) ? 0x1f : 0x20) : 0x7e);
				return 0;
			}
			return 1;
		}

		else if (nkf_isgraph(ndx)) {
			nkf_char val = 0;
			const unsigned short *ptr;
			ptr = UTF8Table::x0212_shiftjis[ndx - 0x21];

			if (ptr) {
				val = ptr[(c1 & 0x7f) - 0x21];
			}
			if (val) {
				c2 = val >> 8;
				c1 = val & 0xff;
				if (p2)
					*p2 = c2;
				if (p1)
					*p1 = c1;
				return 0;
			}
			c2 = Util::X0212Shift(c2);
		}
	}
	if (0x7F < c2)
		return 1;
	if (p2)
		*p2 = ((c2 - 1) >> 1) + ((c2 <= 0x5e) ? 0x71 : 0xb1);
	if (p1)
		*p1 = c1 + ((c2 & 1) ? ((c1 < 0x60) ? 0x1f : 0x20) : 0x7e);
	return 0;
}
/**
 * get default os locale
 */
void Util::NKFLocaleEncoding(wxNKFEncoding* enc, int io) {

	const char* encname;
#ifdef HAVE_LANGINFO_H
	encname = nl_langinfo(CODESET);
#elif defined(__WIN32__)
	static char buf[16];
	sprintf(buf, "CP%d", GetACP());
	encname = buf;
#endif
	if (encname)
		Util::NKFEncFind(encname, enc, io);
}
/**
 * convert Unicode to UTF-8
 */
void Util::NKFUnicodeToUTF8(nkf_char val, nkf_char* p1, nkf_char* p2,
		nkf_char* p3, nkf_char* p4) {

	val &= VALUE_MASK;
	if (val < 0x80) {
		*p1 = val;
		*p2 = 0;
		*p3 = 0;
		*p4 = 0;
	} else if (val < 0x800) {
		*p1 = 0xc0 | (val >> 6);
		*p2 = 0x80 | (val & 0x3f);
		*p3 = 0;
		*p4 = 0;
	} else if (nkf_char_unicode_bmp_p(val)) {
		*p1 = 0xe0 | (val >> 12);
		*p2 = 0x80 | ((val >> 6) & 0x3f);
		*p3 = 0x80 | (val & 0x3f);
		*p4 = 0;
	} else if (nkf_char_unicode_value_p(val)) {
		*p1 = 0xf0 | (val >> 18);
		*p2 = 0x80 | ((val >> 12) & 0x3f);
		*p3 = 0x80 | ((val >> 6) & 0x3f);
		*p4 = 0x80 | (val & 0x3f);
	} else {
		*p1 = 0;
		*p2 = 0;
		*p3 = 0;
		*p4 = 0;
	}
}
/**
 * convert UTF-8 to Unicode
 */
nkf_char Util::NKFUTF8ToUnicode(nkf_char c1, nkf_char c2, nkf_char c3,
		nkf_char c4) {

	nkf_char wc;
	if (c1 <= 0x7F) {
		/* single byte */
		wc = c1;
	} else if (c1 <= 0xC3) {
		/* trail byte or invalid */
		return -1;
	} else if (c1 <= 0xDF) {
		/* 2 bytes */
		wc = (c1 & 0x1F) << 6;
		wc |= (c2 & 0x3F);
	} else if (c1 <= 0xEF) {
		/* 3 bytes */
		wc = (c1 & 0x0F) << 12;
		wc |= (c2 & 0x3F) << 6;
		wc |= (c3 & 0x3F);
	} else if (c2 <= 0xF4) {
		/* 4 bytes */
		wc = (c1 & 0x0F) << 18;
		wc |= (c2 & 0x3F) << 12;
		wc |= (c3 & 0x3F) << 6;
		wc |= (c4 & 0x3F);
	} else {
		return -1;
	}
	return wc;
}
/**
 * convert Unicode to JIS
 */
int Util::UnicodeToJISCommon(nkf_char c2, nkf_char c1, nkf_char c0,
		nkf_char* p2, nkf_char* p1, std::bitset<nkf_flag_num> nkfFlags) {

	const unsigned short * const *pp;
	const unsigned short * const * const *ppp;
	static const char no_best_fit_chars_table_C2[] = { 1, 1, 1, 1, 1, 1, 1, 1,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 2, 1, 1, 2, 0, 0, 1, 1, 0, 1,
			0, 1, 2, 1, 1, 1, 1, 1, 1, 1 };
	static const char no_best_fit_chars_table_C2_ms[] = { 1, 1, 1, 1, 1, 1, 1,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0,
			1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0 };
	static const char no_best_fit_chars_table_932_C2[] = { 1, 1, 1, 1, 1, 1, 1,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0,
			1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0 };
	static const char no_best_fit_chars_table_932_C3[] = { 1, 1, 1, 1, 1, 1, 1,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1 };
	nkf_char ret = 0;

	if (c2 < 0x80) {
		*p2 = 0;
		*p1 = c2;
	} else if (c2 < 0xe0) {
		if (nkfFlags[no_best_fit_chars_f]) {
			if (nkfFlags[ms_ucs_map_f] == UCS_MAP_CP932) {
				switch (c2) {
				case 0xC2:
					if (no_best_fit_chars_table_932_C2[c1 & 0x3F])
						return 1;
					break;
				case 0xC3:
					if (no_best_fit_chars_table_932_C3[c1 & 0x3F])
						return 1;
					break;
				}
			} else if (!nkfFlags[cp932inv_f]) {
				switch (c2) {
				case 0xC2:
					if (no_best_fit_chars_table_C2[c1 & 0x3F])
						return 1;
					break;
				case 0xC3:
					if (no_best_fit_chars_table_932_C3[c1 & 0x3F])
						return 1;
					break;
				}
			} else if (nkfFlags[ms_ucs_map_f] == UCS_MAP_MS) {
				if (c2 == 0xC2 && no_best_fit_chars_table_C2_ms[c1 & 0x3F])
					return 1;
			} else if (nkfFlags[ms_ucs_map_f] == UCS_MAP_CP10001) {
				switch (c2) {
				case 0xC2:
					switch (c1) {
					case 0xA2:
					case 0xA3:
					case 0xA5:
					case 0xA6:
					case 0xAC:
					case 0xAF:
					case 0xB8:
						return 1;
					}
					break;
				}
			}
		}
		pp = nkfFlags[ms_ucs_map_f] == UCS_MAP_CP932 ?
				UTF8Table::utf8_to_euc_2bytes_932 :
				nkfFlags[ms_ucs_map_f] == UCS_MAP_MS ?
						UTF8Table::utf8_to_euc_2bytes_ms :
				nkfFlags[ms_ucs_map_f] == UCS_MAP_CP10001 ?
						UTF8Table::utf8_to_euc_2bytes_mac :
						UTF8Table::utf8_to_euc_2bytes;
		ret = UnicodeToJISCommon2(c2, c1, pp, sizeof_utf8_to_euc_2bytes, p2, p1,
				nkfFlags);

	} else if (c0 < 0xF0) {
		if (nkfFlags[no_best_fit_chars_f]) {
			if (nkfFlags[ms_ucs_map_f] == UCS_MAP_CP932) {
				if (c2 == 0xE3 && c1 == 0x82 && c0 == 0x94)
					return 1;
			} else if (nkfFlags[ms_ucs_map_f] == UCS_MAP_MS) {
				switch (c2) {
				case 0xE2:
					switch (c1) {
					case 0x80:
						if (c0 == 0x94 || c0 == 0x96 || c0 == 0xBE)
							return 1;
						break;
					case 0x88:
						if (c0 == 0x92)
							return 1;
						break;
					}
					break;
				case 0xE3:
					if (c1 == 0x80 || c0 == 0x9C)
						return 1;
					break;
				}
			} else if (nkfFlags[ms_ucs_map_f] == UCS_MAP_CP10001) {
				switch (c2) {
				case 0xE3:
					switch (c1) {
					case 0x82:
						if (c0 == 0x94)
							return 1;
						break;
					case 0x83:
						if (c0 == 0xBB)
							return 1;
						break;
					}
					break;
				}
			} else {
				switch (c2) {
				case 0xE2:
					switch (c1) {
					case 0x80:
						if (c0 == 0x95)
							return 1;
						break;
					case 0x88:
						if (c0 == 0xA5)
							return 1;
						break;
					}
					break;
				case 0xEF:
					switch (c1) {
					case 0xBC:
						if (c0 == 0x8D)
							return 1;
						break;
					case 0xBD:
						if (c0 == 0x9E && !nkfFlags[cp932inv_f])
							return 1;
						break;
					case 0xBF:
						if (0xA0 <= c0 && c0 <= 0xA5)
							return 1;
						break;
					}
					break;
				}
			}
		}
		ppp = nkfFlags[ms_ucs_map_f] == UCS_MAP_CP932 ?
				UTF8Table::utf8_to_euc_3bytes_932 :
				nkfFlags[ms_ucs_map_f] == UCS_MAP_MS ?
						UTF8Table::utf8_to_euc_3bytes_ms :
				nkfFlags[ms_ucs_map_f] == UCS_MAP_CP10001 ?
						UTF8Table::utf8_to_euc_3bytes_mac :
						UTF8Table::utf8_to_euc_3bytes;
		ret = Util::UnicodeToJISCommon2(c1, c0, ppp[c2 - 0xE0],
				sizeof_utf8_to_euc_C2, p2, p1, nkfFlags);
	} else
		return -1;
	if (!ret && !nkfFlags[cp932inv_f] && is_eucg3(*p2)) {
		nkf_char s2, s1;
		if (Util::E2sConv(*p2, *p1, &s2, &s1, nkfFlags) == 0) {
			Util::S2eConv(s2, s1, p2, p1, nkfFlags);
		} else {
			ret = 1;
		}
	}
	return ret;
}
/**
 * convert Unicode to JIS 2
 */
int Util::UnicodeToJISCommon2(nkf_char c1, nkf_char c0,
		const unsigned short * const * pp, nkf_char psize, nkf_char* p2,
		nkf_char* p1, std::bitset<nkf_flag_num> nkfFlags) {

	nkf_char c2;
	const unsigned short *p;
	unsigned short val;

	if (pp == 0)
		return 1;

	c1 -= 0x80;
	if (c1 < 0 || psize <= c1)
		return 1;
	p = pp[c1];
	if (p == 0)
		return 1;

	c0 -= 0x80;
	if (c0 < 0 || sizeof_utf8_to_euc_C2 <= c0)
		return 1;
	val = p[c0];
	if (val == 0)
		return 1;
	if (nkfFlags[no_cp932ext_f] && ((val >> 8) == 0x2D || /* NEC special characters */
	val > NKF_INT32_C(0xF300) /* IBM extended characters */
	))
		return 1;

	c2 = val >> 8;
	if (val > 0x7FFF) {
		c2 &= 0x7f;
		c2 |= PREFIX_EUCG3;
	}
	if (c2 == SO)
		c2 = JIS_X_0201_1976_K;
	c1 = val & 0xFF;
	if (p2)
		*p2 = c2;
	if (p1)
		*p1 = c1;
	return 0;
}
/**
 * convert UTF-16 to EUC-JP
 */
nkf_char Util::W16eConv(nkf_char val, nkf_char* p2, nkf_char* p1,
		std::bitset<nkf_flag_num> nkfFlags) {

	nkf_char c1, c2, c3, c4;
	nkf_char ret = 0;
	val &= VALUE_MASK;
	if (val < 0x80) {
		*p2 = 0;
		*p1 = val;
	} else if (nkf_char_unicode_bmp_p(val)) {
		Util::NKFUnicodeToUTF8(val, &c1, &c2, &c3, &c4);
		ret = Util::UnicodeToJISCommon(c1, c2, c3, p2, p1, nkfFlags);
		if (ret > 0) {
			*p2 = 0;
			*p1 = nkf_char_unicode_new(val);
			ret = 0;
		}
	} else {
		*p2 = 0;
		*p1 = nkf_char_unicode_new(val);
	}
	return ret;
}
/**
 * use X0212
 */
nkf_char Util::X0212Shift(nkf_char c) {
	nkf_char ret = c;
	c &= 0x7f;
	if (is_eucg3(ret)) {
		if (0x75 <= c && c <= 0x7f) {
			ret = c + (0x109 - 0x75);
		}
	} else {
		if (0x75 <= c && c <= 0x7f) {
			ret = c + (0x113 - 0x75);
		}
	}
	return ret;
}
/**
 * use X0212
 */
nkf_char Util::X0212Unshift(nkf_char c) {

	nkf_char ret = c;
	if (0x7f <= c && c <= 0x88) {
		ret = c + (0x75 - 0x7f);
	} else if (0x89 <= c && c <= 0x92) {
		ret = PREFIX_EUCG3 | 0x80 | (c + (0x75 - 0x89));
	}
	return ret;
}
/**
 * compare two strings and return bool
 */
bool Util::StrncmpFromHead(const char* charCode, const char* name) {
	size_t cmpLen = strlen(charCode);
	int ret = strncmp(charCode, name, cmpLen);
	return ret == 0 ? true : false;
}


/*
 * Copyright (c) 1987, Fujitsu LTD. (Itaru ICHIKAWA).
 * Copyright (c) 1996-2010, The nkf Project.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 * claim that you wrote the original software. If you use this software
 * in a product, an acknowledgment in the product documentation would be
 * appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 */

#include "LibNKF.h"

#define NKF_VERSION "2.1.2"
#define NKF_RELEASE_DATE "2011-09-08"
#define COPY_RIGHT \
    "Copyright (C) 1987, FUJITSU LTD. (I.Ichikawa).\n" \
    "Copyright (C) 1996-2011, The nkf Project."

LibNKF::LibNKF() {
	/**
	 * NKFのフラグセットを取得する
	 */
	FlagSet* flag = new FlagSet();
	nkfFlags = flag->GetFlagSet();
	delete flag;
	/**
	 * 変換後の文字列出力先を確保する
	 */
	oConvStr = new std::wstring();
	/**
	 * 入力文字コードと出力文字コードのクラスをセットする
	 */
	inputEncoding = new NKFNativeEncoding();
	outputEncoding = new NKFNativeEncoding();
	mimeout_mode = 0;
}
LibNKF::~LibNKF() {
	delete oConvStr;
	delete inputEncoding;
	delete outputEncoding;
}
/**
 * SetOption:オプションの判別と設定を行う
 *
 * return values:
 *    0: success
 *   -1: ArgumentError
 */
int LibNKF::SetOption(const std::string option) {

	unsigned char* cp = (unsigned char*) option.c_str();
	nkf_char i, j;
	unsigned char *p;
	unsigned char *cp_back = NULL;
	bool bitshift;

	if (nkfFlags[option_mode])
		return 0;

	while (*cp && *cp++ != '-')
		;
	while (*cp || cp_back) {
		if (!*cp) {
			cp = cp_back;
			cp_back = NULL;
			continue;
		}
		p = 0;
		switch (*cp++) {
		case '-': /* literal options */
			if (!*cp || *cp == SP) { /* ignore the rest of arguments */
				nkfFlags[option_mode] = TRUE;
				return 0;
			}
			for (i = 0;
					i < (int) (sizeof(long_option) / sizeof(long_option[0]));
					i++) {
				p = (unsigned char *) long_option[i].name;
				for (j = 0; *p && *p != '=' && *p == cp[j]; p++, j++)
					;
				if (*p == cp[j] || cp[j] == SP) {
					p = &cp[j] + 1;
					break;
				}
				p = 0;
			}
			if (p == 0) {
				return -1;
			}
			while (*cp && *cp != SP && cp++)
				;
			if (long_option[i].alias[0]) {
				cp_back = cp;
				cp = (unsigned char *) long_option[i].alias;
			} else {
				if (strcmp(long_option[i].name, "help") == 0) {
					LibNKF::ShowUsage();
					exit(EXIT_SUCCESS);
				}
				if (strcmp(long_option[i].name, "ic=") == 0) {
					Util::NKFEncFind((char *) p, inputEncoding);
					continue;
				}
				if (strcmp(long_option[i].name, "oc=") == 0) {
					Util::NKFEncFind((char *) p, outputEncoding);
					/* if (enc <= 0) continue; */
					if (!outputEncoding)
						continue;
					continue;
				}
				if (strcmp(long_option[i].name, "guess=") == 0) {
					if (p[0] == '0' || p[0] == '1') {
						nkfFlags[guess_f] = 1;
					} else {
						nkfFlags[guess_f] = 2;
					}
					continue;
				}
				if (strcmp(long_option[i].name, "overwrite") == 0) {
					nkfFlags[file_out_f] = TRUE;
					nkfFlags[overwrite_f] = TRUE;
					nkfFlags[preserve_time_f] = TRUE;
					continue;
				}
				if (strcmp(long_option[i].name, "overwrite=") == 0) {
					nkfFlags[file_out_f] = TRUE;
					nkfFlags[overwrite_f] = TRUE;
					nkfFlags[preserve_time_f] = TRUE;
					nkfFlags[backup_f] = TRUE;
					//nkfFlags[backup_suffix = (char *) p;
					continue;
				}
				if (strcmp(long_option[i].name, "in-place") == 0) {
					nkfFlags[file_out_f] = TRUE;
					nkfFlags[overwrite_f] = TRUE;
					nkfFlags[preserve_time_f] = FALSE;
					continue;
				}
				if (strcmp(long_option[i].name, "in-place=") == 0) {
					nkfFlags[file_out_f] = TRUE;
					nkfFlags[overwrite_f] = TRUE;
					nkfFlags[preserve_time_f] = FALSE;
					nkfFlags[backup_f] = TRUE;
					backup_suffix = (char *) p;
					continue;
				}

				if (strcmp(long_option[i].name, "cap-input") == 0) {
					nkfFlags[cap_f] = TRUE;
					continue;
				}
				if (strcmp(long_option[i].name, "url-input") == 0) {
					nkfFlags[url_f] = TRUE;
					continue;
				}

				if (strcmp(long_option[i].name, "numchar-input") == 0) {
					nkfFlags[numchar_f] = TRUE;
					continue;
				}

				if (strcmp(long_option[i].name, "no-output") == 0) {
					nkfFlags[noout_f] = TRUE;
					continue;
				}
				if (strcmp(long_option[i].name, "debug") == 0) {
					nkfFlags[debug_f] = TRUE;
					continue;
				}

				if (strcmp(long_option[i].name, "cp932") == 0) {

					nkfFlags[cp51932_f] = TRUE;
					nkfFlags[cp932inv_f] = -TRUE;
					nkfFlags[ms_ucs_map_f] = UCS_MAP_CP932;

					continue;
				}
				if (strcmp(long_option[i].name, "no-cp932") == 0) {

					nkfFlags[cp51932_f] = FALSE;
					nkfFlags[cp932inv_f] = FALSE;
					nkfFlags[ms_ucs_map_f] = UCS_MAP_ASCII;

					continue;
				}

				if (strcmp(long_option[i].name, "cp932inv") == 0) {
					nkfFlags[cp932inv_f] = -TRUE;
					continue;
				}

				if (strcmp(long_option[i].name, "x0212") == 0) {
					nkfFlags[x0212_f] = TRUE;
					continue;
				}

				if (strcmp(long_option[i].name, "no-cp932ext") == 0) {
					nkfFlags[no_cp932ext_f] = TRUE;
					continue;
				}
				if (strcmp(long_option[i].name, "no-best-fit-chars") == 0) {
					nkfFlags[no_best_fit_chars_f] = TRUE;
					continue;
				}
//				if (strcmp(long_option[i].name, "fb-skip") == 0) {
//					encode_fallback = NULL;
//					continue;
//				}
//				if (strcmp(long_option[i].name, "fb-html") == 0) {
//					encode_fallback = encode_fallback_html;
//					continue;
//				}
//				if (strcmp(long_option[i].name, "fb-xml") == 0) {
//					encode_fallback = encode_fallback_xml;
//					continue;
//				}
//				if (strcmp(long_option[i].name, "fb-java") == 0) {
//					encode_fallback = encode_fallback_java;
//					continue;
//				}
//				if (strcmp(long_option[i].name, "fb-perl") == 0) {
//					encode_fallback = encode_fallback_perl;
//					continue;
//				}
//				if (strcmp(long_option[i].name, "fb-subchar") == 0) {
//					encode_fallback = encode_fallback_subchar;
//					continue;
//				}
//				if (strcmp(long_option[i].name, "fb-subchar=") == 0) {
//					encode_fallback = encode_fallback_subchar;
//					unicode_subchar = 0;
//					if (p[0] != '0') {
//						/* decimal number */
//						for (i = 0; i < 7 && nkf_isdigit(p[i]); i++) {
//							unicode_subchar *= 10;
//							unicode_subchar += hex2bin(p[i]);
//						}
//					} else if (p[1] == 'x' || p[1] == 'X') {
//						/* hexadecimal number */
//						for (i = 2; i < 8 && nkf_isxdigit(p[i]); i++) {
//							unicode_subchar <<= 4;
//							unicode_subchar |= hex2bin(p[i]);
//						}
//					} else {
//						/* octal number */
//						for (i = 1; i < 8 && nkf_isoctal(p[i]); i++) {
//							unicode_subchar *= 8;
//							unicode_subchar += hex2bin(p[i]);
//						}
//					}
//					w16e_conv(unicode_subchar, &i, &j);
//					unicode_subchar = i << 8 | j;
//					continue;
//				}

				if (strcmp(long_option[i].name, "ms-ucs-map") == 0) {
					nkfFlags[ms_ucs_map_f] = UCS_MAP_MS;
					continue;
				}

				if (strcmp(long_option[i].name, "utf8mac-input") == 0) {
					nkfFlags[nfc_f] = TRUE;
					continue;
				}

				if (strcmp(long_option[i].name, "prefix=") == 0) {
					if (nkf_isgraph(p[0])) {
						for (i = 1; nkf_isgraph(p[i]); i++) {
							prefix_table[p[i]] = p[0];
						}
					}
					continue;
				}

				fprintf(stderr, "unsupported long option: --%s\n",
						long_option[i].name);

				return -1;
			}
			continue;
		case 'b': /* buffered mode */
			nkfFlags[unbuf_f] = FALSE;
			continue;
		case 'u': /* non bufferd mode */
			nkfFlags[unbuf_f] = TRUE;
			continue;
		case 't': /* transparent mode */
			if (*cp == '1') {
				/* alias of -t */
				cp++;
				nkfFlags[nop_f] = TRUE;
			} else if (*cp == '2') {
				/*
				 * -t with put/get
				 *
				 * nkf -t2MB hoge.bin | nkf -t2mB | diff -s - hoge.bin
				 *
				 */
				cp++;
				nkfFlags[nop_f] = 2;
			} else
				nkfFlags[nop_f] = TRUE;
			continue;
		case 'j': /* JIS output */
		case 'n':
			Util::NKFEncFromIndex(ISO_2022_JP, outputEncoding);
			continue;
		case 'e': /* AT&T EUC output */
			Util::NKFEncFromIndex(EUCJP_NKF, outputEncoding);
			continue;
		case 's': /* SJIS output */
			Util::NKFEncFromIndex(SHIFT_JIS, outputEncoding);
			continue;
		case 'l': /* ISO8859 Latin-1 support, no conversion */
			nkfFlags[iso8859_f] = TRUE; /* Only compatible with ISO-2022-JP */
			Util::NKFEncFromIndex(ISO_8859_1, inputEncoding);
			continue;
		case 'i': /* Kanji IN ESC-$-@/B */
			if (*cp == '@' || *cp == 'B')
				nkfFlags[kanji_intro] = *cp++;
			continue;
		case 'o': /* ASCII IN ESC-(-J/B/H */
			/* ESC ( H was used in initial JUNET messages */
			if (*cp == 'J' || *cp == 'B' || *cp == 'H')
				nkfFlags[ascii_intro] = *cp++;
			continue;
		case 'h':
			/*
			 bit:1   katakana->hiragana
			 bit:2   hiragana->katakana
			 */
			if ('9' >= *cp && *cp >= '0') {
				unsigned int shift = nkfFlags[hira_f];
				shift |= (*cp++ - '0');
				nkfFlags[hira_f] = shift;
			} else {
				unsigned int shift = nkfFlags[hira_f];
				shift |= 1;
				nkfFlags[hira_f] = shift;
			}
			continue;
		case 'r':
			nkfFlags[rot_f] = TRUE;
			continue;

//		case 'V':
//			LibNKF::ShowConfiguration();
//			exit(EXIT_SUCCESS);
//			break;
		case 'v':
			ShowVersion();
			exit(EXIT_SUCCESS);
			break;

		case 'w': /* UTF-{8,16,32} output */
			if (cp[0] == '8') {
				cp++;
				if (cp[0] == '0') {
					cp++;
					Util::NKFEncFromIndex(UTF_8N, outputEncoding);
				} else {
					nkfFlags[output_bom_f] = TRUE;
					Util::NKFEncFromIndex(UTF_8_BOM, outputEncoding);
				}
			} else {
				int enc_idx;
				if ('1' == cp[0] && '6' == cp[1]) {
					cp += 2;
					enc_idx = UTF_16;
				} else if ('3' == cp[0] && '2' == cp[1]) {
					cp += 2;
					enc_idx = UTF_32;
				} else {
					Util::NKFEncFromIndex(UTF_8, outputEncoding);
					continue;
				}
				if (cp[0] == 'L') {
					cp++;
					outputEncoding->endian = ENDIAN_LITTLE;
					nkfFlags[output_bom_f] = TRUE;
				} else if (cp[0] == 'B') {
					cp++;
					nkfFlags[output_bom_f] = TRUE;
				}
				if (cp[0] == '0') {
					nkfFlags[output_bom_f] = FALSE;
					cp++;
					enc_idx =
							enc_idx == UTF_16 ?
									(outputEncoding->endian == ENDIAN_LITTLE ?
											UTF_16LE : UTF_16BE) :
									(outputEncoding->endian == ENDIAN_LITTLE ?
											UTF_32LE : UTF_32BE);
				} else {
					enc_idx =
							enc_idx == UTF_16 ?
									(outputEncoding->endian == ENDIAN_LITTLE ?
											UTF_16LE_BOM : UTF_16BE_BOM) :
									(outputEncoding->endian == ENDIAN_LITTLE ?
											UTF_32LE_BOM : UTF_32BE_BOM);
				}
				Util::NKFEncFromIndex(enc_idx, outputEncoding);
			}
			continue;

		case 'W': /* UTF input */
			if (cp[0] == '8') {
				cp++;
				Util::NKFEncFromIndex(UTF_8, inputEncoding);
			} else {
				int enc_idx;
				if ('1' == cp[0] && '6' == cp[1]) {
					cp += 2;
					inputEncoding->endian = ENDIAN_BIG;
					enc_idx = UTF_16;
				} else if ('3' == cp[0] && '2' == cp[1]) {
					cp += 2;
					inputEncoding->endian = ENDIAN_BIG;
					enc_idx = UTF_32;
				} else {
					Util::NKFEncFromIndex(UTF_8, inputEncoding);
					continue;
				}
				if (cp[0] == 'L') {
					cp++;
					inputEncoding->endian = ENDIAN_LITTLE;
				} else if (cp[0] == 'B') {
					cp++;
					inputEncoding->endian = ENDIAN_BIG;
				}
				enc_idx = (
						enc_idx == UTF_16 ?
								(inputEncoding->endian == ENDIAN_LITTLE ?
										UTF_16LE : UTF_16BE) :
								(inputEncoding->endian == ENDIAN_LITTLE ?
										UTF_32LE : UTF_32BE));
				Util::NKFEncFromIndex(enc_idx, inputEncoding);
			}
			continue;

			/* Input code assumption */
		case 'J': /* ISO-2022-JP input */
			Util::NKFEncFromIndex(ISO_2022_JP, inputEncoding);
			continue;
		case 'E': /* EUC-JP input */
			Util::NKFEncFromIndex(EUCJP_NKF, inputEncoding);
			continue;
		case 'S': /* Shift_JIS input */
			Util::NKFEncFromIndex(SHIFT_JIS, inputEncoding);
			continue;
		case 'Z': /* Convert X0208 alphabet to asii */
			/* alpha_f
			 bit:0   Convert JIS X 0208 Alphabet to ASCII
			 bit:1   Convert Kankaku to one space
			 bit:2   Convert Kankaku to two spaces
			 bit:3   Convert HTML Entity
			 bit:4   Convert JIS X 0208 Katakana to JIS X 0201 Katakana
			 */
			while ('0' <= *cp && *cp <= '4') {
				unsigned int shift1 = 1 << (*cp++ - '0');
				unsigned int shift2 = shift1 |= nkfFlags[alpha_f];
				nkfFlags[alpha_f] = shift2;
			}
			bitshift = nkfFlags[alpha_f];
			bitshift |= 1;
			nkfFlags[alpha_f] = bitshift;

			continue;
		case 'x': /* Convert X0201 kana to X0208 or X0201 Conversion */
			nkfFlags[x0201_f] = FALSE; /* No X0201->X0208 conversion */
			/* accept  X0201
			 ESC-(-I     in JIS, EUC, MS Kanji
			 SI/SO       in JIS, EUC, MS Kanji
			 SS2         in EUC, JIS, not in MS Kanji
			 MS Kanji (0xa0-0xdf)
			 output  X0201
			 ESC-(-I     in JIS (0x20-0x5f)
			 SS2         in EUC (0xa0-0xdf)
			 0xa0-0xd    in MS Kanji (0xa0-0xdf)
			 */
			continue;
		case 'X': /* Convert X0201 kana to X0208 */
			nkfFlags[x0201_f] = TRUE;
			continue;
		case 'F': /* prserve new lines */
			nkfFlags[fold_preserve_f] = TRUE;
		case 'f': /* folding -f60 or -f */
			nkfFlags[fold_f] = TRUE;
			fold_len = 0;
			while ('0' <= *cp && *cp <= '9') { /* we don't use atoi here */
				fold_len *= 10;
				fold_len += *cp++ - '0';
			}
			if (!(0 < fold_len && fold_len < BUFSIZ))
				fold_len = DEFAULT_FOLD;
			if (*cp == '-') {
				fold_margin = 0;
				cp++;
				while ('0' <= *cp && *cp <= '9') { /* we don't use atoi here */
					fold_margin *= 10;
					fold_margin += *cp++ - '0';
				}
			}
			continue;
		case 'm': /* MIME support */
			/* mime_decode_f = TRUE; *//* this has too large side effects... */
			if (*cp == 'B' || *cp == 'Q') {
				nkfFlags[mime_decode_mode] = *cp++;
				nkfFlags[mimebuf_f] = FIXED_MIME;
			} else if (*cp == 'N') {
				nkfFlags[mime_f] = TRUE;
				cp++;
			} else if (*cp == 'S') {
				nkfFlags[mime_f] = STRICT_MIME;
				cp++;
			} else if (*cp == '0') {
				nkfFlags[mime_decode_f] = FALSE;
				nkfFlags[mime_f] = FALSE;
				cp++;
			} else {
				nkfFlags[mime_f] = STRICT_MIME;
			}
			continue;
		case 'M': /* MIME output */
			if (*cp == 'B') {
				mimeout_mode = 'B';
				nkfFlags[mimeout_f] = FIXED_MIME;
				cp++;
			} else if (*cp == 'Q') {
				mimeout_mode = 'Q';
				nkfFlags[mimeout_f] = FIXED_MIME;
				cp++;
			} else {
				nkfFlags[mimeout_f] = TRUE;
			}
			continue;
		case 'B': /* Broken JIS support */
			/*  bit:0   no ESC JIS
			 bit:1   allow any x on ESC-(-x or ESC-$-x
			 bit:2   reset to ascii on NL
			 */
			if ('9' >= *cp && *cp >= '0') {
				int bitshift1 = 1 << (*cp++ - '0');
				int bitshift2 = nkfFlags[broken_f];
				bitshift2 |= bitshift1;
				nkfFlags[broken_f] = bitshift2;
			} else {
				int bitshift1 = nkfFlags[broken_f];
				bitshift1 |= TRUE;
				nkfFlags[broken_f] = bitshift1;
			}
			continue;

		case 'O':/* for Output file */
			nkfFlags[file_out_f] = TRUE;
			continue;

		case 'c':/* add cr code */
			nkfFlags[eolmode_f] = CRLF;
			continue;
		case 'd':/* delete cr code */
			nkfFlags[eolmode_f] = LF;
			continue;
		case 'I': /* ISO-2022-JP output */
			nkfFlags[iso2022jp_f] = TRUE;
			continue;
		case 'L': /* line mode */
			if (*cp == 'u') { /* unix */
				nkfFlags[eolmode_f] = LF;
				cp++;
			} else if (*cp == 'm') { /* mac */
				nkfFlags[eolmode_f] = CR;
				cp++;
			} else if (*cp == 'w') { /* windows */
				nkfFlags[eolmode_f] = CRLF;
				cp++;
			} else if (*cp == '0') { /* no conversion  */
				nkfFlags[eolmode_f] = 0;
				cp++;
			}
			continue;

		case 'g':
			if ('2' <= *cp && *cp <= '9') {
				nkfFlags[guess_f] = 2;
				cp++;
			} else if (*cp == '0' || *cp == '1') {
				nkfFlags[guess_f] = 1;
				cp++;
			} else {
				nkfFlags[guess_f] = 1;
			}
			continue;

		case SP:
			/* module muliple options in a string are allowed for Perl moudle  */
			while (*cp && *cp++ != '-')
				;
			continue;
		default:

			fprintf(stderr, "unknown option: -%c\n", *(cp - 1));

			/* bogus option but ignored */
			return -1;
		}
	}
	return 0;
}
/**
 * 使い方の表示
 */
void LibNKF::ShowUsage() {
	fprintf(HELP_OUTPUT,
			"Usage:  nkf -[flags] [--] [in file] .. [out file for -O flag]\n"
					" j/s/e/w  Specify output encoding ISO-2022-JP, Shift_JIS, EUC-JP\n"
					"          UTF options is -w[8[0],{16,32}[{B,L}[0]]]\n"
					" J/S/E/W  Specify input encoding ISO-2022-JP, Shift_JIS, EUC-JP\n"
					"          UTF option is -W[8,[16,32][B,L]]\n"
					" J/S/E    Specify output encoding ISO-2022-JP, Shift_JIS, EUC-JP\n");
	fprintf(HELP_OUTPUT,
			" m[BQSN0] MIME decode [B:base64,Q:quoted,S:strict,N:nonstrict,0:no decode]\n"
					" M[BQ]    MIME encode [B:base64 Q:quoted]\n"
					" f/F      Folding: -f60 or -f or -f60-10 (fold margin 10) F preserve nl\n");
	fprintf(HELP_OUTPUT,
			" Z[0-4]   Default/0: Convert JISX0208 Alphabet to ASCII\n"
					"          1: Kankaku to one space  2: to two spaces  3: HTML Entity\n"
					"          4: JISX0208 Katakana to JISX0201 Katakana\n"
					" X,x      Convert Halfwidth Katakana to Fullwidth or preserve it\n");
	fprintf(HELP_OUTPUT, " O        Output to File (DEFAULT 'nkf.out')\n"
			" L[uwm]   Line mode u:LF w:CRLF m:CR (DEFAULT noconversion)\n");
	fprintf(HELP_OUTPUT, " --ic=<encoding>        Specify the input encoding\n"
			" --oc=<encoding>        Specify the output encoding\n"
			" --hiragana --katakana  Hiragana/Katakana Conversion\n"
			" --katakana-hiragana    Converts each other\n");
	fprintf(HELP_OUTPUT,
			" --{cap, url}-input     Convert hex after ':' or '%%'\n"
					" --numchar-input        Convert Unicode Character Reference\n"
					" --fb-{skip, html, xml, perl, java, subchar}\n"
					"                        Specify unassigned character's replacement\n");
	fprintf(HELP_OUTPUT, " --in-place[=SUF]       Overwrite original files\n"
			" --overwrite[=SUF]      Preserve timestamp of original files\n"
			" -g --guess             Guess the input code\n"
			" -v --version           Print the version\n"
			" --help/-V              Print this help / configuration\n");
	LibNKF::ShowVersion();
}
/**
 * バージョン情報の表示
 */
void LibNKF::ShowVersion() {
	fprintf(HELP_OUTPUT,
			"Network Kanji Filter Version " NKF_VERSION " (" NKF_RELEASE_DATE ") \n" COPY_RIGHT "\n");
}
/**
 * 文字コードを変換する処理のラッパーで、外部に見せるメソッド
 */
int LibNKF::Convert(const std::string inputFile, const std::string outputFile,
		const std::string option) {

	SetOption(option);
	FILE* in;
	in = fopen(inputFile.c_str(), "r");
	KanjiConvert(in);
	fclose(in);

	FILE* out;
	out = fopen(outputFile.c_str(), "w");
	fputws(oConvStr->c_str(), out);
	fclose(out);

	return 0;
}
/**
 * 文字コードを変換する処理のラッパーで、外部に見せるメソッド
 */
std::wstring LibNKF::Convert(const std::string inputFile,
		const std::string option) {

	SetOption(option);
	FILE* in;
	in = fopen(inputFile.c_str(), "r");
	KanjiConvert (in);
	fclose(in);
	return *oConvStr;
}
/**
 * このクラスの主要メソッド：
 * ファイルポインタを引数に文字コードを変換する
 */
int LibNKF::KanjiConvert(FILE* f) {
	nkf_char c1 = 0, c2 = 0, c3 = 0, c4 = 0;
	int shift_mode = 0; /* 0, 1, 2, 3 */
	int g2 = 0;
	int is_8bit = FALSE;

	//if (inputEncoding && !nkf_enc_asciicompat(inputEncoding)) {
	//	is_8bit = TRUE;
	//}

	// 設定されたフラグから使用するクラスとメソッドを決定する
	if (ModuleConnection() < 0) {
		fprintf(stderr, "no output encoding given\n");
		return -1;
	}
	// BOMのチェック
	CheckBom(f);

	/**
	 * 入力がUTF-16の場合ここで処理を行う
	 */
	if (inputEncoding->iconvName == "w_iconv32") {
		// UTF-16処理用クラスのインスタンスを作成
		UTF16Util* utf16 = new UTF16Util();
		utf16->SetInputEndian(inputEncoding->endian);

		// 4byte必要なUTF-16だった場合の処理
		while ((c1 = StdGetC(f)) != EOF && (c2 = StdGetC(f)) != EOF && (c3 =
				StdGetC(f)) != EOF && (c4 = StdGetC(f)) != EOF) {
			oConvStr->push_back(utf16->NKFIconvUTF16(c1, c2, c3, c4));
		}
		goto finished;

	} else if (inputEncoding->iconvName == "w_iconv16") {
		// UTF-16処理用クラスのインスタンスを作成
		UTF16Util* utf16 = new UTF16Util();
		utf16->SetInputEndian(inputEncoding->endian);

		// 2byte必要なUTF-16だった場合の処理
		while ((c1 = StdGetC(f)) != EOF && (c2 = StdGetC(f)) != EOF) {
			if (utf16->NKFIconvUTF16(c1, c2, 0,
					0) == NKF_ICONV_NEED_TWO_MORE_BYTES&& (c3 = LibNKF::StdGetC(f)) != EOF
					&& (c4 = StdGetC(f)) != EOF) {oConvStr->push_back(utf16->NKFIconvUTF16(c1, c2, c3, c4));
		}
	}
		goto finished;
	}

	/**
	 * UTF-16以外の文字コード変換ループの始まり
	 */
	while ((c1 = LibNKF::StdGetC(f)) != EOF) {
		if (!inputEncoding->id)
			CodeStatus(c1);
		if (c2) {
			/* second byte */
			if (c2 > DEL) {
				/* in case of 8th bit is on */
				if (!nkfFlags[estab_f] && !nkfFlags[mime_decode_mode]) {
					/**
					 * まだ文字コードが確定しておらず曖昧な状態
					 */
//					GuessConv* gIConv = new GuessConv();
//					if (gIConv->GuessIConv(f, c2, c1, flagPool, inputEncoding,
//							outputEncoding) == EOF) {
//						delete gIConv;
//						LAST;
//					} else {
//						delete gIConv;
//						SKIP
//						;
//					}
				} else {
					/* in case of already established */
					if (c1 < 0x40) {
						/* ignore bogus code */
						SKIP
						;
					} else {
						SEND;
					}
				}
			} else {
				/* 2nd byte of 7 bit code or SJIS */
				SEND;
			}
		} else if (nkf_char_unicode_p(c1)) {
			outputEncoding->Oconv(0, c1, nkfFlags, oConvStr);
			NEXT;
		} else {
			/* first byte */
			if (inputEncoding->inputMode == JIS_X_0208 && DEL <= c1
					&& c1 < 0x92) {
				/* CP5022x */
				MORE
				;
			} else if (!inputEncoding->name.empty()
					&& inputEncoding->name[0] == 'I' && 0xA1 <= c1
					&& c1 <= 0xDF) {
				/* JIS X 0201 Katakana in 8bit JIS */
				c2 = JIS_X_0201_1976_K;
				c1 &= 0x7f;
				SEND;
			} else if (c1 > DEL) {
				/* 8 bit code */
				if (!nkfFlags[estab_f] && !nkfFlags[iso8859_f]) {
					/* not established yet */
					MORE
					;
				} else { /* estab_f==TRUE */
					if (nkfFlags[iso8859_f]) {
						c2 = ISO_8859_1;
						c1 &= 0x7f;
						SEND;
					} else if ((inputEncoding->iconvName == "s_iconv"
							&& 0xA0 <= c1 && c1 <= 0xDF)
							|| (nkfFlags[ms_ucs_map_f] == UCS_MAP_CP10001
									&& (c1 == 0xFD || c1 == 0xFE))) {
						/* JIS X 0201 */
						c2 = JIS_X_0201_1976_K;
						c1 &= 0x7f;
						SEND;
					} else {
						/* already established */
						MORE
						;
					}
				}
			} else if (SP < c1 && c1 < DEL) {
				/* in case of Roman characters */
				if (shift_mode) {
					/* output 1 shifted byte */
					if (nkfFlags[iso8859_f]) {
						c2 = ISO_8859_1;
						SEND;
					} else if (nkf_byte_jisx0201_katakana_p(c1)) {
						/* output 1 shifted byte */
						c2 = JIS_X_0201_1976_K;
						SEND;
					} else {
						/* look like bogus code */
						SKIP
						;
					}
				} else if (inputEncoding->inputMode == JIS_X_0208
						|| inputEncoding->inputMode == JIS_X_0212
						|| inputEncoding->inputMode == JIS_X_0213_1
						|| inputEncoding->inputMode == JIS_X_0213_2) {
					/* in case of Kanji shifted */
					MORE
					;
				} else if (c1 == '=' && nkfFlags[mime_f] && !mime_decode_mode) {
					/* Check MIME code */
					if ((c1 = LibNKF::StdGetC(f)) == EOF) {
						outputEncoding->Oconv(0, '=', nkfFlags, oConvStr);
						LAST;
					} else if (c1 == '?') {
						/* =? is mime conversion start sequence */
						if (nkfFlags[mime_f] == STRICT_MIME) {
							/* check in real detail */
							//if (MimeBeginStrict(f) == EOF)
							//	LAST;
							//SKIP
							//;
						} //else if (MimeBegin(f) == EOF)
						  //	LAST;
						  //SKIP
						  //;
					} else {
						outputEncoding->Oconv(0, '=', nkfFlags, oConvStr);
						LibNKF::StdUnGetC(c1, f);
						SKIP
						;
					}
				} else {
					/* normal ASCII code */
					SEND;
				}
			} else if (c1 == SI && (!is_8bit || mime_decode_mode)) {
				shift_mode = 0;
				SKIP
				;
			} else if (c1 == SO && (!is_8bit || mime_decode_mode)) {
				shift_mode = 1;
				SKIP
				;
			} else if (c1 == ESC && (!is_8bit || mime_decode_mode)) {
				if ((c1 = LibNKF::StdGetC(f)) == EOF) {
					outputEncoding->Oconv(0, ESC, nkfFlags, oConvStr);
					LAST;
				} else if (c1 == '&') {
					/* IRR */
					if ((c1 = LibNKF::StdGetC(f)) == EOF) {
						LAST;
					} else {
						SKIP
						;
					}
				} else if (c1 == '$') {
					/* GZDMx */
					if ((c1 = LibNKF::StdGetC(f)) == EOF) {
						/* don't send bogus code
						 (*oconv)(0, ESC);
						 (*oconv)(0, '$'); */
						LAST;
					} else if (c1 == '@' || c1 == 'B') {
						/* JIS X 0208 */
						SetInputMode(JIS_X_0208);
						SKIP
						;
					} else if (c1 == '(') {
						/* GZDM4 */
						if ((c1 = LibNKF::StdGetC(f)) == EOF) {
							/* don't send bogus code
							 (*oconv)(0, ESC);
							 (*oconv)(0, '$');
							 (*oconv)(0, '(');
							 */
							LAST;
						} else if (c1 == '@' || c1 == 'B') {
							/* JIS X 0208 */
							SetInputMode(JIS_X_0208);
							SKIP
							;
						} else if (c1 == 'D') {
							SetInputMode(JIS_X_0212);
							SKIP
							;
						} else if (c1 == 'O' || c1 == 'Q') {
							SetInputMode(JIS_X_0213_1);
							SKIP
							;
						} else if (c1 == 'P') {
							SetInputMode(JIS_X_0213_2);
							SKIP
							;
						} else {
							/* could be some special code */
							outputEncoding->Oconv(0, ESC, nkfFlags, oConvStr);
							outputEncoding->Oconv(0, '$', nkfFlags, oConvStr);
							outputEncoding->Oconv(0, '(', nkfFlags, oConvStr);
							outputEncoding->Oconv(0, c1, nkfFlags, oConvStr);
							SKIP
							;
						}
					} else if (nkfFlags[broken_f] & 0x2) {
						/* accept any ESC-(-x as broken code ... */
						inputEncoding->inputMode = JIS_X_0208;
						shift_mode = 0;
						SKIP
						;
					} else {
						outputEncoding->Oconv(0, ESC, nkfFlags, oConvStr);
						outputEncoding->Oconv(0, '$', nkfFlags, oConvStr);
						outputEncoding->Oconv(0, c1, nkfFlags, oConvStr);
						SKIP
						;
					}
				} else if (c1 == '(') {
					/* GZD4 */
					if ((c1 = LibNKF::StdGetC(f)) == EOF) {
						/* don't send bogus code
						 (*oconv)(0, ESC);
						 (*oconv)(0, '('); */
						LAST;
					} else if (c1 == 'I') {
						/* JIS X 0201 Katakana */
						SetInputMode(JIS_X_0201_1976_K);
						SKIP
						;
					} else if (c1 == 'B' || c1 == 'J' || c1 == 'H') {
						/* ISO-646IRV:1983 or JIS X 0201 Roman or JUNET */
						SetInputMode(ASCII);
						SKIP
						;
					} else if (nkfFlags[broken_f] & 0x2) {
						SetInputMode(ASCII);
						SKIP
						;
					} else {
						outputEncoding->Oconv(0, ESC, nkfFlags, oConvStr);
						outputEncoding->Oconv(0, '(', nkfFlags, oConvStr);
						SEND;
					}
				} else if (c1 == '.') {
					/* G2D6 */
					if ((c1 = LibNKF::StdGetC(f)) == EOF) {
						LAST;
					} else if (c1 == 'A') {
						/* ISO-8859-1 */
						g2 = ISO_8859_1;
						SKIP
						;
					} else {
						outputEncoding->Oconv(0, ESC, nkfFlags, oConvStr);
						outputEncoding->Oconv(0, '.', nkfFlags, oConvStr);
						SEND;
					}
				} else if (c1 == 'N') {
					/* SS2 */
					c1 = LibNKF::StdGetC(f);
					if (g2 == ISO_8859_1) {
						c2 = ISO_8859_1;
						SEND;
					} else {
						LibNKF::StdUnGetC(c1, f);
						/* lonely ESC  */
						outputEncoding->Oconv(0, ESC, nkfFlags, oConvStr);
						SEND;
					}
				} else {
					/* lonely ESC  */
					outputEncoding->Oconv(0, ESC, nkfFlags, oConvStr);
					SEND;
				}
			} else if (c1 == ESC && inputEncoding->iconvName == "s_iconv") {
				/* ESC in Shift_JIS */
				if ((c1 = LibNKF::StdGetC(f)) == EOF) {
					outputEncoding->Oconv(0, ESC, nkfFlags, oConvStr);
					LAST;
				} else if (c1 == '$') {
					/* J-PHONE emoji */
					if ((c1 = LibNKF::StdGetC(f)) == EOF) {
						LAST;
					} else if (('E' <= c1 && c1 <= 'G')
							|| ('O' <= c1 && c1 <= 'Q')) {
						/*
						 NUM : 0 1 2 3 4 5
						 BYTE: G E F O P Q
						 C%7 : 1 6 0 2 3 4
						 C%7 : 0 1 2 3 4 5 6
						 NUM : 2 0 3 4 5 X 1
						 */
						static const nkf_char jphone_emoji_first_table[7] = {
								0xE1E0, 0xDFE0, 0xE2E0, 0xE3E0, 0xE4E0, 0xDFE0,
								0xE0E0 };
						c3 =
								nkf_char_unicode_new(jphone_emoji_first_table[c1 % 7]);
						if ((c1 = LibNKF::StdGetC(f)) == EOF)
							LAST;
						while (SP <= c1 && c1 <= 'z') {
							outputEncoding->Oconv(0, c1 + c3, nkfFlags,
									oConvStr);
							if ((c1 = LibNKF::StdGetC(f)) == EOF)
								LAST;
						}
						SKIP
						;
					} else {
						outputEncoding->Oconv(0, ESC, nkfFlags, oConvStr);
						outputEncoding->Oconv(0, '$', nkfFlags, oConvStr);
						SEND;
					}
				} else {
					/* lonely ESC  */
					outputEncoding->Oconv(0, ESC, nkfFlags, oConvStr);
					SEND;
				}
			} else if (c1 == LF || c1 == CR) {
				if (nkfFlags[broken_f] & 4) {
					inputEncoding->inputMode = ASCII;
					//SetIconv(FALSE, 0, nkfFlags, oConvStr);
					SEND;
				} else if (nkfFlags[mime_decode_f] && !mime_decode_mode) {
					if (c1 == LF) {
						if ((c1 = LibNKF::StdGetC(f)) != EOF && c1 == SP) {
							LibNKF::StdUnGetC(SP, f);
							continue;
						} else {
							LibNKF::StdUnGetC(c1, f);
						}
						c1 = LF;
						SEND;
					} else { /* if (c1 == CR)*/
						if ((c1 = LibNKF::StdGetC(f)) != EOF) {
							if (c1 == SP) {
								LibNKF::StdUnGetC(SP, f);
								continue;
							} else if (c1 == LF
									&& (c1 = LibNKF::StdGetC(f)) != EOF
									&& c1 == SP) {
								LibNKF::StdUnGetC(SP, f);
								continue;
							} else {
								LibNKF::StdUnGetC(c1, f);
							}
							LibNKF::StdUnGetC(LF, f);
						} else {
							LibNKF::StdUnGetC(c1, f);
						}
						c1 = CR;
						SEND;
					}
				}
			} else
				SEND;
		}
		/* send: */
		switch (inputEncoding->inputMode) {

		case ASCII:
			switch (inputEncoding->Iconv(c2, c1, 0, nkfFlags, oConvStr)) { /* can be EUC / SJIS / UTF-8 */
			case -2:
				/* 4 bytes UTF-8 */
				if ((c3 = LibNKF::StdGetC(f)) != EOF) {
					CodeStatus(c3);
					c3 <<= 8;
					if ((c4 = LibNKF::StdGetC(f)) != EOF) {
						CodeStatus(c4);
						inputEncoding->Iconv(c2, c1, c3 | c4, nkfFlags,
								oConvStr);
					}
				}
				break;
			case -1:
				/* 3 bytes EUC or UTF-8 */
				if ((c3 = LibNKF::StdGetC(f)) != EOF) {
					CodeStatus(c3);
					inputEncoding->Iconv(c2, c1, c3, nkfFlags, oConvStr);
				}
				break;
			}
			break;
		case JIS_X_0208:
		case JIS_X_0213_1:
			if (nkfFlags[ms_ucs_map_f] && 0x7F <= c2 && c2 <= 0x92 && 0x21 <= c1
					&& c1 <= 0x7E) {
				/* CP932 UDC */
				c1 = nkf_char_unicode_new((c2 - 0x7F) * 94 + c1 - 0x21 + 0xE000);
				c2 = 0;
			}
			outputEncoding->Oconv(c2, c1, nkfFlags, oConvStr); /* this is JIS, not SJIS/EUC case */
			break;
		case JIS_X_0212:
			outputEncoding->Oconv(PREFIX_EUCG3 | c2, c1, nkfFlags, oConvStr);
			break;
		case JIS_X_0213_2:
			outputEncoding->Oconv(PREFIX_EUCG3 | c2, c1, nkfFlags, oConvStr);
			break;
		default:
			outputEncoding->Oconv(inputEncoding->inputMode, c1, nkfFlags,
					oConvStr); /* other special case */
		}

		c2 = 0;
		c3 = 0;
		continue;
		/* goto next_word */
	}

	finished:
	/* epilogue */
	inputEncoding->Iconv(EOF, 0, 0, nkfFlags, oConvStr);

	if (inputEncoding->name.empty()) {
		if (is_8bit) {
//			struct input_code *p = input_code_list;
//			struct input_code *result = p;
//			while (p->name) {
//				if (p->score < result->score)
//					result = p;
//				++p;
//			}
//			set_input_codename(result->name);
//			debug(result->name);
		}
	}
	return 0;
}

/**
 * 設定されたフラグから文字コード変換に使うメソッドを決める
 */
int LibNKF::ModuleConnection() {
	// 入力された文字コードからフラグを設定する
	SetInputEncoding(inputEncoding);
	// 出力する文字コードが設定されていなければ設定する
	if (!outputEncoding->id)
		outputEncoding = Util::NKFDefaultEncoding();
	// デフォルトのエンコードが設定できなければ出力しないか推測するモード
	if (!outputEncoding->id) {
		if (nkfFlags[noout_f] || nkfFlags[guess_f]) {
			Util::NKFEncFromIndex(ISO_2022_JP, outputEncoding);
		} else {
			return -1;
		}
	}
	// 出力する文字コードのクラスを設定する
	SetOutputEncoding(outputEncoding);

	if (outputEncoding->id == UTF_8) {
		// エンコーディングがUTF-8だった場合出力モードはUTF-8
		inputEncoding->outputMode = UTF_8;
		outputEncoding->outputMode = UTF_8;
	}

	if (nkfFlags[x0201_f] == NKF_UNSPECIFIED) {
		nkfFlags[x0201_f] = X0201_DEFAULT;
	}

	/* 入力文字コードと処理するメソッドを設定する */
	if (inputEncoding) {
		if (nkfFlags[estab_f] != -TRUE) {
			nkfFlags[estab_f] = -TRUE;
		}
	} else {
		if (nkfFlags[estab_f] != FALSE) {
			nkfFlags[estab_f] = FALSE;
		}
		inputEncoding->iconvName = "e_iconv";
		inputEncoding->baseId = ISO_2022_JP;
	}

	return 0;
}

/**
 * 入力された文字コードからフラグを設定する
 */
void LibNKF::SetInputEncoding(NKFNativeEncoding *enc) {
	switch (enc->id) {

	case ISO_8859_1:
		nkfFlags[iso8859_f] = TRUE;
		break;
	case CP50221:
	case CP50222:
		if (nkfFlags[x0201_f] == NKF_UNSPECIFIED)
			nkfFlags[x0201_f] = FALSE; /* -x specified implicitly */
	case CP50220:
		nkfFlags[cp51932_f] = TRUE;
		nkfFlags[ms_ucs_map_f] = UCS_MAP_CP932;
		break;
	case ISO_2022_JP_1:
		nkfFlags[x0212_f] = TRUE;
		break;
	case ISO_2022_JP_3:
		nkfFlags[x0212_f] = TRUE;
		nkfFlags[x0213_f] = TRUE;
		break;
	case ISO_2022_JP_2004:
		nkfFlags[x0212_f] = TRUE;
		nkfFlags[x0213_f] = TRUE;
		break;
	case SHIFT_JIS:
		break;
	case WINDOWS_31J:
		if (nkfFlags[x0201_f] == NKF_UNSPECIFIED)
			nkfFlags[x0201_f] = FALSE; /* -x specified implicitly */
		nkfFlags[cp51932_f] = TRUE;
		nkfFlags[ms_ucs_map_f] = UCS_MAP_CP932;
		break;
		break;
	case CP10001:
		nkfFlags[cp51932_f] = TRUE;
		nkfFlags[ms_ucs_map_f] = UCS_MAP_CP10001;
		break;
	case EUC_JP:
		break;
	case EUCJP_NKF:
		break;
	case CP51932:
		if (nkfFlags[x0201_f] == NKF_UNSPECIFIED)
			nkfFlags[x0201_f] = FALSE; /* -x specified implicitly */
		nkfFlags[cp51932_f] = TRUE;
		nkfFlags[ms_ucs_map_f] = UCS_MAP_CP932;
		break;
	case EUCJP_MS:
		if (nkfFlags[x0201_f] == NKF_UNSPECIFIED)
			nkfFlags[x0201_f] = FALSE; /* -x specified implicitly */
		nkfFlags[cp51932_f] = FALSE;
		nkfFlags[ms_ucs_map_f] = UCS_MAP_MS;
		break;
	case EUCJP_ASCII:
		if (nkfFlags[x0201_f] == NKF_UNSPECIFIED)
			nkfFlags[x0201_f] = FALSE; /* -x specified implicitly */
		nkfFlags[cp51932_f] = FALSE;
		nkfFlags[ms_ucs_map_f] = UCS_MAP_ASCII;
		break;
	case SHIFT_JISX0213:
	case SHIFT_JIS_2004:
		nkfFlags[x0213_f] = TRUE;
		nkfFlags[cp51932_f] = FALSE;
		break;
	case EUC_JISX0213:
	case EUC_JIS_2004:
		nkfFlags[x0213_f] = TRUE;
		nkfFlags[cp51932_f] = FALSE;
		break;
	case UTF8_MAC:
		nkfFlags[nfc_f] = TRUE;
		break;
	case UTF_16:
	case UTF_16BE:
	case UTF_16BE_BOM:
		inputEncoding->endian = ENDIAN_BIG;
		break;
	case UTF_16LE:
	case UTF_16LE_BOM:
		inputEncoding->endian = ENDIAN_LITTLE;
		break;
	case UTF_32:
	case UTF_32BE:
	case UTF_32BE_BOM:
		inputEncoding->endian = ENDIAN_BIG;
		break;
	case UTF_32LE:
	case UTF_32LE_BOM:
		inputEncoding->endian = ENDIAN_LITTLE;
		break;
	}
}
/**
 * 出力する文字コード用にフラグを設定する
 */
void LibNKF::SetOutputEncoding(NKFNativeEncoding* enc) {
	switch (enc->id) {
	case CP50220:
		if (nkfFlags[cp932inv_f] == TRUE)
			nkfFlags[cp932inv_f] = FALSE;
		nkfFlags[ms_ucs_map_f] = UCS_MAP_CP932;
		break;
	case CP50221:
		if (nkfFlags[x0201_f] == NKF_UNSPECIFIED)
			nkfFlags[x0201_f] = FALSE; /* -x specified implicitly */
		if (nkfFlags[cp932inv_f] == TRUE)
			nkfFlags[cp932inv_f] = FALSE;
		nkfFlags[ms_ucs_map_f] = UCS_MAP_CP932;
		break;
	case ISO_2022_JP:
		if (nkfFlags[cp932inv_f] == TRUE)
			nkfFlags[cp932inv_f] = FALSE;
		break;
	case ISO_2022_JP_1:
		nkfFlags[x0212_f] = TRUE;
		if (nkfFlags[cp932inv_f] == TRUE)
			nkfFlags[cp932inv_f] = FALSE;
		break;
	case ISO_2022_JP_3:
		nkfFlags[x0212_f] = TRUE;
		nkfFlags[x0213_f] = TRUE;
		if (nkfFlags[cp932inv_f] == TRUE)
			nkfFlags[cp932inv_f] = FALSE;
		break;
	case SHIFT_JIS:
		break;
	case WINDOWS_31J:
		if (nkfFlags[x0201_f] == NKF_UNSPECIFIED)
			nkfFlags[x0201_f] = FALSE; /* -x specified implicitly */
		nkfFlags[ms_ucs_map_f] = UCS_MAP_CP932;
		break;
	case CP10001:
		nkfFlags[ms_ucs_map_f] = UCS_MAP_CP10001;
		break;
	case EUC_JP:
		nkfFlags[x0212_f] = TRUE;
		if (nkfFlags[cp932inv_f] == TRUE)
			nkfFlags[cp932inv_f] = FALSE;
		nkfFlags[ms_ucs_map_f] = UCS_MAP_ASCII;
		break;
	case EUCJP_NKF:
		nkfFlags[x0212_f] = FALSE;
		if (nkfFlags[cp932inv_f] == TRUE)
			nkfFlags[cp932inv_f] = FALSE;
		nkfFlags[ms_ucs_map_f] = UCS_MAP_ASCII;
		break;
	case CP51932:
		if (nkfFlags[x0201_f] == NKF_UNSPECIFIED)
			nkfFlags[x0201_f] = FALSE; /* -x specified implicitly */
		if (nkfFlags[cp932inv_f] == TRUE)
			nkfFlags[cp932inv_f] = FALSE;
		nkfFlags[ms_ucs_map_f] = UCS_MAP_CP932;
		break;
	case EUCJP_MS:
		if (nkfFlags[x0201_f] == NKF_UNSPECIFIED)
			nkfFlags[x0201_f] = FALSE; /* -x specified implicitly */
		nkfFlags[x0212_f] = TRUE;
		nkfFlags[ms_ucs_map_f] = UCS_MAP_MS;
		break;
	case EUCJP_ASCII:
		if (nkfFlags[x0201_f] == NKF_UNSPECIFIED)
			nkfFlags[x0201_f] = FALSE; /* -x specified implicitly */
		nkfFlags[x0212_f] = TRUE;
		nkfFlags[ms_ucs_map_f] = UCS_MAP_ASCII;
		break;
	case SHIFT_JISX0213:
	case SHIFT_JIS_2004:
		nkfFlags[x0213_f] = TRUE;
		if (nkfFlags[cp932inv_f] == TRUE)
			nkfFlags[cp932inv_f] = FALSE;
		break;
	case EUC_JISX0213:
	case EUC_JIS_2004:
		nkfFlags[x0212_f] = TRUE;
		nkfFlags[x0213_f] = TRUE;
		if (nkfFlags[cp932inv_f] == TRUE)
			nkfFlags[cp932inv_f] = FALSE;
		break;
	case UTF_8_BOM:
		nkfFlags[output_bom_f] = TRUE;
		break;
	case UTF_16:
	case UTF_16BE_BOM:
		nkfFlags[output_bom_f] = TRUE;
		break;
	case UTF_16LE:
		outputEncoding->endian = ENDIAN_LITTLE;
		nkfFlags[output_bom_f] = FALSE;
		break;
	case UTF_16LE_BOM:
		outputEncoding->endian = ENDIAN_LITTLE;
		nkfFlags[output_bom_f] = TRUE;
		break;
	case UTF_32:
	case UTF_32BE_BOM:
		nkfFlags[output_bom_f] = TRUE;
		break;
	case UTF_32LE:
		outputEncoding->endian = ENDIAN_LITTLE;
		nkfFlags[output_bom_f] = FALSE;
		break;
	case UTF_32LE_BOM:
		outputEncoding->endian = ENDIAN_LITTLE;
		nkfFlags[output_bom_f] = TRUE;
		break;
	}
}

/**
 * BOMが存在するかチェックし、存在すれば無視する
 */
void LibNKF::CheckBom(FILE* f) {

	int c2;
	switch (c2 = StdGetC(f)) {
	case 0x00:
		if ((c2 = StdGetC(f)) == 0x00) {
			if ((c2 = StdGetC(f)) == 0xFE) {
				if ((c2 = StdGetC(f)) == 0xFF) {
					if (!inputEncoding->id) {
						// もし文字コードに何も設定されていなければiconvメソッドを決めてリターン
						inputEncoding->iconvName = "w_iconv32";
						inputEncoding->endian = ENDIAN_BIG;
						return;
					}
					StdUnGetC(0xFF, f);
				} else {
					StdUnGetC(c2, f);
				}
				StdUnGetC(0xFE, f);
			} else if (c2 == 0xFF) {
				if ((c2 = StdGetC(f)) == 0xFE) {
					if (!inputEncoding->id) {
						// もし文字コードに何も設定されていなければiconvメソッドを決めてリターン
						inputEncoding->iconvName = "w_iconv32";
						inputEncoding->endian = ENDIAN_2143;
						return;
					}
					StdUnGetC(0xFF, f);
				} else {
					StdUnGetC(c2, f);
				}
				StdUnGetC(0xFF, f);
			} else {
				StdUnGetC(c2, f);
			}
			StdUnGetC(0x00, f);
		} else {
			StdUnGetC(c2, f);
		}
		StdUnGetC(0x00, f);
		break;
	case 0xEF:
		if ((c2 = StdGetC(f)) == 0xBB) {
			if ((c2 = StdGetC(f)) == 0xBF) {
				if (!inputEncoding->id) {
					// もし文字コードに何も設定されていなければiconvメソッドを決めてリターン
					inputEncoding->iconvName = "w_iconv";
					return;
				}
				StdUnGetC(0xBF, f);
			} else
				StdUnGetC(c2, f);
			StdUnGetC(0xBB, f);
		} else
			StdUnGetC(c2, f);
		StdUnGetC(0xEF, f);
		break;
	case 0xFE:
		if ((c2 = StdGetC(f)) == 0xFF) {
			if ((c2 = StdGetC(f)) == 0x00) {
				if ((c2 = StdGetC(f)) == 0x00) {
					if (!inputEncoding->id) {
						// もし文字コードに何も設定されていなければiconvメソッドを決めてリターン
						inputEncoding->iconvName = "w_iconv32";
						inputEncoding->endian = ENDIAN_3412;
						return;
					}
					StdUnGetC(0x00, f);
				} else
					StdUnGetC(c2, f);
				StdUnGetC(0x00, f);
			} else
				StdUnGetC(c2, f);
			if (!inputEncoding->id) {
				// もし文字コードに何も設定されていなければiconvメソッドを決めてリターン
				inputEncoding->iconvName = "w_iconv16";
				inputEncoding->endian = ENDIAN_BIG;
			}
			StdUnGetC(0xFF, f);
		} else
			StdUnGetC(c2, f);
		StdUnGetC(0xFE, f);
		break;
	case 0xFF:
		if ((c2 = StdGetC(f)) == 0xFE) {
			if ((c2 = StdGetC(f)) == 0x00) {
				if ((c2 = StdGetC(f)) == 0x00) {
					if (!inputEncoding->id) {
						// もし文字コードに何も設定されていなければiconvメソッドを決めてリターン
						inputEncoding->iconvName = "w_iconv32";
						inputEncoding->endian = ENDIAN_LITTLE;
						return;
					}
					StdUnGetC(0x00, f);
				} else
					StdUnGetC(c2, f);
				StdUnGetC(0x00, f);
			} else
				StdUnGetC(c2, f);
			if (!inputEncoding->id) {
				// もし文字コードに何も設定されていなければiconvメソッドを決めてリターン
				inputEncoding->iconvName = "w_iconv16";
				inputEncoding->endian = ENDIAN_LITTLE;
			}
			StdUnGetC(0xFE, f);
		} else
			StdUnGetC(c2, f);
		StdUnGetC(0xFF, f);
		break;
	default:
		StdUnGetC(c2, f);
		break;
	}
}
/**
 * 総当りで文字コードを調べる
 */
void LibNKF::CodeStatus(nkf_char c) {
	/**
	 * 最初に入力コードが何であるか総当りで調べる
	 * "EUC-JP","Shift_JIS","UTF-8","UTF-16","UTF-32"の順
	 */
	int action_flag = 1;
	InputCode* result = 0;
	std::string inputCodeList[] = { "EUC-JP", "Shift_JIS", "UTF-8", "UTF-16",
			"UTF-32" };

	/**
	 * InputCode->statが決まるまでループする
	 */
	for (int i = 0; i < 5; i++) {
		InputCode* p;
		p->name = inputCodeList[i];
		p->StatusFunc(p, c, nkfFlags);
		if (p->stat > 0) {
			action_flag = 0;
		} else if (p->stat == 0) {
			if (result) {
				action_flag = 0;
			} else {
				result = p;
			}
		}
		++i;
	}

	if (action_flag) {
		// resultが確定している場合
		if (result && !nkfFlags[estab_f]) {
			if (nkfFlags[estab_f] != TRUE) {
				nkfFlags[estab_f] = TRUE;
			}
			inputEncoding->baseName = result->name;
		}
	}
}
/**
 * 入力文字コードを設定する
 */
void LibNKF::SetInputMode(int mode) {
	// 入力文字コードを設定する
	inputEncoding->inputMode = mode;
	inputEncoding->baseName = "ISO-2022-JP";
}


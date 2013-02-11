/*
 * flagset.h
 *
 *  Created on: 2012/07/29
 * Contributor: Hiroyuki Nagata
 */

#ifndef FLAGSET_H_
#define FLAGSET_H_

#include <bitset>

#define         FALSE   0
#define         TRUE    1

#define         DEFAULT_J       'B'
#define         DEFAULT_R       'B'

/* MIME ENCODE */
#define         FIXED_MIME      7
#define         STRICT_MIME     8

#define NKF_UNSPECIFIED (-TRUE)

/* MIME_DECODE_DEFAULT */
#define MIME_DECODE_DEFAULT STRICT_MIME

/* Folding */
#define FOLD_MARGIN  10
#define DEFAULT_FOLD 60

/* UCS Mapping
 * 0: Shift_JIS, eucJP-ascii
 * 1: eucJP-ms
 * 2: CP932, CP51932
 * 3: CP10001
 */
#define UCS_MAP_ASCII   0
#define UCS_MAP_MS      1
#define UCS_MAP_CP932   2
#define UCS_MAP_CP10001 3

/* byte order */
enum byte_order {
	ENDIAN_BIG = 1, ENDIAN_LITTLE = 2, ENDIAN_2143 = 3, ENDIAN_3412 = 4
};

enum ConvFlags {
	/* if 0 read option, else quit read option */
	option_mode,
	/* buffering */
	unbuf_f,
	/* char code establish */
	estab_f,
	nop_f,
	/* binary or not */
	binmode_f,
	/* rot14/43 mode */
	rot_f,
	/* mime decode is explicitly on */
	mime_decode_f,
	/* MIME buffered input */
	mimebuf_f,
	/* convert ESC-less broken JIS */
	broken_f,
	/* ISO8859 through */
	iso8859_f,
	/* base64 mode */
	mimeout_f,
	/* replace non ISO-2022-JP with GETA */
	iso2022jp_f,
	/* UNICODE_NORMALIZATION */
	nfc_f,
	/* INPUT_OPTION */
	cap_f,
	url_f,
	/* NUMCHAR_OPTION */
	numchar_f,
	/* CHECK_OPTION */
	noout_f,
	debug_f,
	/* 0: OFF, 1: ON, 2: VERBOSE */
	guess_f,
	/* invert IBM extended characters to others */
	cp51932_f,
	/* invert NEC-selected IBM extended characters to IBM extended characters */
	cp932inv_f,
	x0212_f,
	x0213_f,
	ms_ucs_map_f,
	no_best_fit_chars_f,
	/* no NEC special, NEC-selected IBM extended and IBM extended characters */
	no_cp932ext_f,
	/* OVERWRITE */
	file_out_f,
	overwrite_f,
	preserve_time_f,
	backup_f,
	/* CR, LF, CRLF */
	eolmode_f,
	/* 0: unestablished, EOF: MIXED */
	input_eol,
	/* UTF8_OUTPUT_ENABLE */
	output_bom_f,
	output_endian,
	/* fold parameter */
	f_line,
	/* chars in line */
	f_prev,
	fold_preserve_f,
	/* preserve new lines */
	fold_f,
	/* Convert X0201 kana to X0208 or X0201 Conversion */
	x0201_f,
	kanji_intro,
	ascii_intro,
	hira_f,
	/* convert JIx0208 alphbet to ASCII */
	alpha_f,
	mime_f,
	/* MIME mode B base64, Q hex */
	mime_decode_mode,
	nkf_flag_num
};

class FlagSet {
public:
	/**
	 * 初期状態のフラグのセットを渡す
	 */
	std::bitset<nkf_flag_num> GetFlagSet();
};

#endif /* FLAGSET_H_ */

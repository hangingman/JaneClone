/*
 * wxnkfbaseconfig.h
 *
 *  Created on: 2012/07/15
 * Contributor: Hiroyuki Nagata
 */

#ifndef WXNKFBASECONFIG_H_
#define WXNKFBASECONFIG_H_

#include <cstdio>

#ifdef nkf_char
#elif defined(INT_IS_SHORT)
typedef long nkf_char;
#define NKF_INT32_C(n)   (n##L)
#else
typedef int nkf_char;
#define NKF_INT32_C(n)   (n)
#endif

/* DEFAULT_CODE_LOCALE */
#ifdef DEFAULT_CODE_LOCALE

#if defined(__WIN32__) /* not win32 should be posix */
# ifndef HAVE_LOCALE_H
#  define HAVE_LOCALE_H
# endif
#else
# ifndef HAVE_LANGINFO_H
#  define HAVE_LANGINFO_H
# endif
# ifndef HAVE_LOCALE_H
#  define HAVE_LOCALE_H
# endif
#endif

#ifdef HAVE_LANGINFO_H
#include <langinfo.h>
#endif
#ifdef HAVE_LOCALE_H
#include <locale.h>
#endif

#endif /* DEFAULT_CODE_LOCALE */

/* MIME_DECODE_DEFAULT */
#ifndef MIME_DECODE_DEFAULT
#define MIME_DECODE_DEFAULT STRICT_MIME
#endif
#ifndef X0201_DEFAULT
#define X0201_DEFAULT TRUE
#endif

/* encodings */
enum nkf_encodings {
	ASCII,
	ISO_8859_1,
	ISO_2022_JP,
	CP50220,
	CP50221,
	CP50222,
	ISO_2022_JP_1,
	ISO_2022_JP_3,
	ISO_2022_JP_2004,
	SHIFT_JIS,
	WINDOWS_31J,
	CP10001,
	EUC_JP,
	EUCJP_NKF,
	CP51932,
	EUCJP_MS,
	EUCJP_ASCII,
	SHIFT_JISX0213,
	SHIFT_JIS_2004,
	EUC_JISX0213,
	EUC_JIS_2004,
	UTF_8,
	UTF_8N,
	UTF_8_BOM,
	UTF8_MAC,
	UTF_16,
	UTF_16BE,
	UTF_16BE_BOM,
	UTF_16LE,
	UTF_16LE_BOM,
	UTF_32,
	UTF_32BE,
	UTF_32BE_BOM,
	UTF_32LE,
	UTF_32LE_BOM,
	BINARY,
	NKF_ENCODING_TABLE_SIZE,
	JIS_X_0201_1976_K = 0x1013, /* I *//* JIS C 6220-1969 */
	/* JIS_X_0201_1976_R = 0x1014, *//* J *//* JIS C 6220-1969 */
	/* JIS_X_0208_1978   = 0x1040, *//* @ *//* JIS C 6226-1978 */
	/* JIS_X_0208_1983   = 0x1087, *//* B *//* JIS C 6226-1983 */
	JIS_X_0208 = 0x1168, /* @B */
	JIS_X_0212 = 0x1159, /* D */
	/* JIS_X_0213_2000_1 = 0x1228, *//* O */
	JIS_X_0213_2 = 0x1229, /* P */
	JIS_X_0213_1 = 0x1233 /* Q */
};

/* ASCII CODE */
#define         BS      0x08
#define         TAB     0x09
#define         LF      0x0a
#define         CR      0x0d
#define         ESC     0x1b
#define         SP      0x20
#define         DEL     0x7f
#define         SI      0x0f
#define         SO      0x0e
#define         SS2     0x8e
#define         SS3     0x8f
#define         CRLF    0x0D0A

#define         GETA1   0x22
#define         GETA2   0x2e

/* Macros */
#define PREFIX_EUCG3    NKF_INT32_C(0x8F00)
#define CLASS_MASK      NKF_INT32_C(0xFF000000)
#define CLASS_UNICODE   NKF_INT32_C(0x01000000)
#define VALUE_MASK      NKF_INT32_C(0x00FFFFFF)
#define UNICODE_BMP_MAX NKF_INT32_C(0x0000FFFF)
#define UNICODE_MAX     NKF_INT32_C(0x0010FFFF)

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

/* I don't trust portablity of toupper */
#define nkf_toupper(c)  (('a'<=c && c<='z')?(c-('a'-'A')):c)
#define nkf_isoctal(c)  ('0'<=c && c<='7')
#define nkf_isdigit(c)  ('0'<=c && c<='9')
#define nkf_isxdigit(c)  (nkf_isdigit(c) || ('a'<=c && c<='f') || ('A'<=c && c <= 'F'))
#define nkf_isblank(c) (c == SP || c == TAB)
#define nkf_isspace(c) (nkf_isblank(c) || c == CR || c == LF)
#define nkf_isalpha(c) (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'))
#define nkf_isalnum(c) (nkf_isdigit(c) || nkf_isalpha(c))
#define nkf_isprint(c) (SP<=c && c<='~')
#define nkf_isgraph(c) ('!'<=c && c<='~')
#define hex2bin(c) (('0'<=c&&c<='9') ? (c-'0') : \
		    ('A'<=c&&c<='F') ? (c-'A'+10) : \
		    ('a'<=c&&c<='f') ? (c-'a'+10) : 0)
#define bin2hex(c) ("0123456789ABCDEF"[c&15])
#define is_eucg3(c2) (((unsigned short)c2 >> 8) == SS3)
#define nkf_noescape_mime(c) ((c == CR) || (c == LF) || \
			      ((c > SP) && (c < DEL) && (c != '?') && (c != '=') && (c != '_') \
			       && (c != '(') && (c != ')') && (c != '.') && (c != 0x22)))

#define is_ibmext_in_sjis(c2) (CP932_TABLE_BEGIN <= c2 && c2 <= CP932_TABLE_END)
#define nkf_byte_jisx0201_katakana_p(c) (SP <= c && c <= 0x5F)

#define		is_alnum(c)  \
    (('a'<=c && c<='z')||('A'<= c && c<='Z')||('0'<=c && c<='9'))

/* I don't trust portablity of toupper */
#define nkf_toupper(c)  (('a'<=c && c<='z')?(c-('a'-'A')):c)
#define nkf_isoctal(c)  ('0'<=c && c<='7')
#define nkf_isdigit(c)  ('0'<=c && c<='9')
#define nkf_isxdigit(c)  (nkf_isdigit(c) || ('a'<=c && c<='f') || ('A'<=c && c <= 'F'))
#define nkf_isblank(c) (c == SP || c == TAB)
#define nkf_isspace(c) (nkf_isblank(c) || c == CR || c == LF)
#define nkf_isalpha(c) (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'))
#define nkf_isalnum(c) (nkf_isdigit(c) || nkf_isalpha(c))
#define nkf_isprint(c) (SP<=c && c<='~')
#define nkf_isgraph(c) ('!'<=c && c<='~')
#define hex2bin(c) (('0'<=c&&c<='9') ? (c-'0') : \
		    ('A'<=c&&c<='F') ? (c-'A'+10) : \
		    ('a'<=c&&c<='f') ? (c-'a'+10) : 0)
#define bin2hex(c) ("0123456789ABCDEF"[c&15])
#define is_eucg3(c2) (((unsigned short)c2 >> 8) == SS3)
#define nkf_noescape_mime(c) ((c == CR) || (c == LF) || \
			      ((c > SP) && (c < DEL) && (c != '?') && (c != '=') && (c != '_') \
			       && (c != '(') && (c != ')') && (c != '.') && (c != 0x22)))

#define is_ibmext_in_sjis(c2) (CP932_TABLE_BEGIN <= c2 && c2 <= CP932_TABLE_END)
#define nkf_byte_jisx0201_katakana_p(c) (SP <= c && c <= 0x5F)

#define nkf_char_euc3_new(c) ((c) | PREFIX_EUCG3)
#define nkf_char_unicode_new(c) ((c) | CLASS_UNICODE)
#define nkf_char_unicode_p(c) ((c & CLASS_MASK) == CLASS_UNICODE)
#define nkf_char_unicode_bmp_p(c) ((c & VALUE_MASK) <= UNICODE_BMP_MAX)
#define nkf_char_unicode_value_p(c) ((c & VALUE_MASK) <= UNICODE_MAX)

/**
 * for debug build
 * usage: make CXXFLAGS='-DDEBUG_BUILD'
 */
#ifdef DEBUG_BUILD
#  define DEBUG(x) std::cerr << x
#else
#  define DEBUG(x) do {} while (0)
#endif

#endif /* WXNKFBASECONFIG_H_ */

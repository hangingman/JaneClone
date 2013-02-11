/*
 * wxnkfencoding.h
 *
 *  Created on: 2012/08/09
 * Contributor: Hiroyuki Nagata
 */

#ifndef WXNKFENCODING_H_
#define WXNKFENCODING_H_

#include <wx/datstrm.h>
#include <iostream>
#include "wxnkfbaseconfig.h"
#include "flagset.h"
#include "utf8table.h"

/**
 * Input and Output encode data class
 *
 * I refer original nkf's struct, nkf_encoding & nkf_native_encoding.
 * There are 8 base encodings,
 * "ASCII" "ISO-2022-JP" "Shift_JIS" "EUC-JP" "UTF-8" "UTF-16" "UTF-32".
 *
 * nkf change process method by above base encoding...
 */
class wxNKFEncoding {

public:
	/**
	 * unique character code id
	 */
	int iCharID;
	int oCharID;
	/**
	 * unique character code name
	 */
	wxString iCharName;
	wxString oCharName;
	/**
	 * base character code id
	 */
	int inputMode;
	int outputMode;
	/**
	 * base character code name
	 */
	wxString inputBaseName;
	wxString outputBaseName;
	/**
	 * endian
	 */
	int iEndian;
	int oEndian;

	/**
	 * According to inputMode, convert input char
	 */
	nkf_char Iconv(nkf_char c2, nkf_char c1, nkf_char c0,
			std::bitset<nkf_flag_num> nkfFlags, std::string* oConvStr);
	nkf_char Iconv(nkf_char c2, nkf_char c1, nkf_char c0,
			std::bitset<nkf_flag_num> nkfFlags, wxDataOutputStream* out);
	/**
	 * According to outputMode, convert output char
	 */
	void Oconv(nkf_char c2, nkf_char c1, std::bitset<nkf_flag_num> nkfFlags,
			std::string* oConvStr);
	void Oconv(nkf_char c2, nkf_char c1, std::bitset<nkf_flag_num> nkfFlags,
			wxDataOutputStream* out);

private:
	/**
	 * s_iconv
	 */
	nkf_char SIconv(nkf_char c2, nkf_char c1, nkf_char c0,
			std::bitset<nkf_flag_num> nkfFlags, std::string* oConvStr);
	nkf_char SIconv(nkf_char c2, nkf_char c1, nkf_char c0,
			std::bitset<nkf_flag_num> nkfFlags, wxDataOutputStream* out);
	/**
	 * e_iconv
	 */
	nkf_char EIconv(nkf_char c2, nkf_char c1, nkf_char c0,
			std::bitset<nkf_flag_num> nkfFlags, std::string* oConvStr);
	nkf_char EIconv(nkf_char c2, nkf_char c1, nkf_char c0,
			std::bitset<nkf_flag_num> nkfFlags, wxDataOutputStream* out);
	/**
	 * w_iconv
	 */
	nkf_char WIconv(nkf_char c1, nkf_char c2, nkf_char c3,
			std::bitset<nkf_flag_num> nkfFlags, std::string* oConvStr);
	nkf_char WIconv(nkf_char c1, nkf_char c2, nkf_char c3,
			std::bitset<nkf_flag_num> nkfFlags, wxDataOutputStream* out);
	/**
	 * w_iconv16
	 */
	nkf_char WIconv16(nkf_char c2, nkf_char c1, nkf_char c0,
			std::bitset<nkf_flag_num> nkfFlags, std::string* oConvStr);
	nkf_char WIconv16(nkf_char c2, nkf_char c1, nkf_char c0,
			std::bitset<nkf_flag_num> nkfFlags, wxDataOutputStream* out);
	/**
	 * w_iconv32
	 */
	nkf_char WIconv32(nkf_char c2, nkf_char c1, nkf_char c0,
			std::bitset<nkf_flag_num> nkfFlags, std::string* oConvStr);
	nkf_char WIconv32(nkf_char c2, nkf_char c1, nkf_char c0,
			std::bitset<nkf_flag_num> nkfFlags, wxDataOutputStream* out);
	/**
	 * j_oconv
	 */
	void JOconv(nkf_char c2, nkf_char c1, std::bitset<nkf_flag_num> nkfFlags,
			std::string* oConvStr);
	void JOconv(nkf_char c2, nkf_char c1, std::bitset<nkf_flag_num> nkfFlags,
			wxDataOutputStream* out);
	/**
	 * s_oconv
	 */
	void SOconv(nkf_char c2, nkf_char c1, std::bitset<nkf_flag_num> nkfFlags,
			std::string* oConvStr);
	void SOconv(nkf_char c2, nkf_char c1, std::bitset<nkf_flag_num> nkfFlags,
			wxDataOutputStream* out);
	/**
	 * e_oconv
	 */
	void EOconv(nkf_char c2, nkf_char c1, std::bitset<nkf_flag_num> nkfFlags,
			std::string* oConvStr);
	void EOconv(nkf_char c2, nkf_char c1, std::bitset<nkf_flag_num> nkfFlags,
			wxDataOutputStream* out);
	/**
	 * w_oconv
	 */
	void WOconv(nkf_char c2, nkf_char c1, std::bitset<nkf_flag_num> nkfFlags,
			std::string* oConvStr);
	void WOconv(nkf_char c2, nkf_char c1, std::bitset<nkf_flag_num> nkfFlags,
			wxDataOutputStream* out);
	/**
	 * w_oconv16
	 */
	void WOconv16(nkf_char c2, nkf_char c1, std::bitset<nkf_flag_num> nkfFlags,
			std::string* oConvStr);
	void WOconv16(nkf_char c2, nkf_char c1, std::bitset<nkf_flag_num> nkfFlags,
			wxDataOutputStream* out);
	/**
	 * w_oconv32
	 */
	void WOconv32(nkf_char c2, nkf_char c1, std::bitset<nkf_flag_num> nkfFlags,
			std::string* oConvStr);
	void WOconv32(nkf_char c2, nkf_char c1, std::bitset<nkf_flag_num> nkfFlags,
			wxDataOutputStream* out);
	/**
	 * escape sequence for mixing Ascii code
	 */
	void OutputAsciiEscapeSequence(int mode, std::string* oConvStr);
	void OutputAsciiEscapeSequence(int mode, wxDataOutputStream* out);
};

#endif /* WXNKFENCODING_H_ */

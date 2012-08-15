/*
 * wxnkf.h
 *
 *  Created on: 2012/08/09
 * Contributor: Hiroyuki Nagata
 */

#ifndef WXNKF_H_
#define WXNKF_H_

#include <wx/wx.h>
#include <wx/app.h>
#include <wx/stream.h>
#include <wx/filesys.h>
#include <wx/datstrm.h>
#include <wx/wfstream.h>
#include "flagset.h"
#include "utf8table.h"
#include "wxnkfencoding.h"
#include "util.h"
#include "utf16util.h"

/* HELP_OUTPUT */
#ifdef HELP_OUTPUT_STDERR
#define HELP_OUTPUT stderr
#else
#define HELP_OUTPUT stdout
#endif

/* all long option */
static const struct {
	const char *name;
	const char *alias;
} long_option[] = { { "ic=", "" }, { "oc=", "" }, { "base64", "jMB" }, { "euc",
		"e" }, { "euc-input", "E" }, { "fj", "jm" }, { "help", "" }, { "jis",
		"j" }, { "jis-input", "J" }, { "mac", "sLm" }, { "mime", "jM" }, {
		"mime-input", "m" }, { "msdos", "sLw" }, { "sjis", "s" }, {
		"sjis-input", "S" }, { "unix", "eLu" }, { "version", "v" }, { "windows",
		"sLw" }, { "hiragana", "h1" }, { "katakana", "h2" }, {
		"katakana-hiragana", "h3" }, { "guess=", "" }, { "guess", "g2" }, {
		"cp932", "" }, { "no-cp932", "" }, { "x0212", "" }, { "utf8", "w" }, {
		"utf16", "w16" }, { "ms-ucs-map", "" }, { "fb-skip", "" }, { "fb-html",
		"" }, { "fb-xml", "" }, { "fb-perl", "" }, { "fb-java", "" }, {
		"fb-subchar", "" }, { "fb-subchar=", "" }, { "utf8-input", "W" }, {
		"utf16-input", "W16" }, { "no-cp932ext", "" },
		{ "no-best-fit-chars", "" }, { "utf8mac-input", "" },
		{ "overwrite", "" }, { "overwrite=", "" }, { "in-place", "" }, {
				"in-place=", "" }, { "cap-input", "" }, { "url-input", "" }, {
				"numchar-input", "" }, { "no-output", "" }, { "debug", "" }, {
				"cp932inv", "" }, { "prefix=", "" }, };

class wxNKF {

public:
	/**
	 * constructor
	 */
	wxNKF();
	/**
	 * destructor
	 */
	~wxNKF();
	/**
	 * convert charcter code in file, with option
	 *
	 * path example
	 *
	 * msw : C:\\Users\\foo\\bar\\etc.txt
	 * gtk, osx : /usr/foo/bar/etc.txt
	 */
	int Convert(const wxString inputFilePath, const wxString outputFilePath,
			const wxString option);
	/**
	 * convert charcter code in string, with option
	 */
	wxString Convert(const wxString inputFilePath, const wxString option);
	/**
	 * show usage
	 */
	void ShowUsage();
	/**
	 * show version
	 */
	void ShowVersion();

private:
	/**
	 * macro for KanjiConvert method
	 */
	#define NEXT continue        /* no output, get next */
	#define SKIP c2=0;continue   /* no output, get next */
	#define MORE c2=c1;continue  /* need one more byte */
	#define SEND (void)0         /* output c1 and c2, get next */
	#define LAST break           /* end of loop, go closing  */
	/**
	 * set of nkf flag
	 */
	std::bitset<nkf_flag_num> nkfFlags;
	/**
	 * string for output
	 */
	std::wstring* oConvStr;
	/**
	 * Instance of class for IO char code setting and process
	 */
	wxNKFEncoding* wxEnc;
	/**
	 * SetOption : setting and judge options
	 *
	 * return values:
	 *    0: success
	 *   -1: ArgumentError
	 */
	int SetOption(const wxString option);
	/**
	 * define charcter code convert method by set flags
	 */
	int ModuleConnection();
	/**
	 * main method of this class convert char to file
	 */
	int KanjiConvert(wxInputStream* in, wxDataOutputStream* out);
	/**
	 * main method of this class convert char to string
	 */
	wxString KanjiConvert(wxInputStream* in);
	/**
	 * setting input encode
	 */
	void SetInputMode(int mode);
	/**
	 * check BOM existence. If it exist, ignore
	 */
	void CheckBom(wxInputStream* in);
	/**
	 * search character code
	 */
	void CodeStatus(nkf_char c);
	/**
	 * set flag for Input & Output
	 */
	void SetInputEncoding(wxNKFEncoding *enc);
	void SetOutputEncoding(wxNKFEncoding *enc);
	/**
	 * other class value etc.
	 */
	unsigned char prefix_table[256];
	char* backup_suffix;
	int fold_len;
	int fold_margin;
	int mimeout_mode; /* 0, -1, 'Q', 'B', 1, 2 */
	int shift_mode;/* 0 or 1 */
};

#endif /* WXNKF_H_ */

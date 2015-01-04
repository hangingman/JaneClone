//
// コマンドラインのシンプルなUI実装
//
// The MIT License (MIT)
//
// Copyright (c) <2014> chromabox <chromarockjp@gmail.com>
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>
#include <getopt.h>

#include "simple_terminal.hpp"
#include "twitter_json.hpp"

using namespace std;
using namespace TerminalColor;



SimpleTerminal::SimpleTerminal()
{
	// カラー表の初期化をする
	fg_color[BLACK]			= "30";
	fg_color[DARKRED]		= "31";
	fg_color[DARKGREEN]		= "32";
	fg_color[DARKYELLOW]	= "33";
	fg_color[DARKBLUE]		= "34";
	fg_color[DARKMAGENTA]	= "35";
	fg_color[DARKCYAN]		= "36";
	fg_color[GREY]			= "37";

	bg_color[BLACK]			= "40";
	bg_color[DARKRED]		= "41";
	bg_color[DARKGREEN]		= "42";
	bg_color[DARKYELLOW]	= "43";
	bg_color[DARKBLUE]		= "44";
	bg_color[DARKMAGENTA]	= "45";
	bg_color[DARKCYAN]		= "46";
	bg_color[GREY]			= "47";
	
	fg_color[DARKGREY]		= "90";
	fg_color[RED]			= "91";
	fg_color[GREEN]			= "92";
	fg_color[YELLOW]		= "93";
	fg_color[BLUE]			= "94";
	fg_color[MAGENTA	]	= "95";
	fg_color[CYAN]			= "96";
	fg_color[WHITE]			= "97";
	
	bg_color[DARKGREY]		= "100";
	bg_color[RED]			= "101";
	bg_color[GREEN]			= "102";
	bg_color[YELLOW]		= "103";
	bg_color[BLUE]			= "104";
	bg_color[MAGENTA	]	= "105";
	bg_color[CYAN]			= "106";
	bg_color[WHITE]			= "107";

}


SimpleTerminal::~SimpleTerminal()
{}


void SimpleTerminal::setAttribute(const std::string &tcolor,const std::string &bcolor,bool bold)
{
	if(! tcolor.empty()) setTextColor(tcolor);
	if(! bcolor.empty()) setBgColor(bcolor);
	if( bold )			setBold();
}

void SimpleTerminal::_PutString(const std::string &str)
{
	cout << ESCST;
	if(! attr.empty()){
		cout << attr;
		if((!fcolor.empty()) || (!bcolor.empty())){
			cout << ";";
		}
	}
	if(! fcolor.empty()){
		cout << fcolor;
		if(! bcolor.empty()){
			cout << ";";
		}
	}
	if(! bcolor.empty()){
		cout << bcolor;
	}
	cout << "m" << str;
}



void SimpleTerminal::Put(const std::string &str,const std::string &tcolor,const std::string &bcolor,bool bold)
{
	setAttribute(tcolor,bcolor,bold);
	_PutString(str);
}	

void SimpleTerminal::Puts(const std::string &str,const std::string &tcolor,const std::string &bcolor,bool bold)
{
	setAttribute(tcolor,bcolor,bold);
	_PutString(str);
	cout << TerminalColor::ESCST << TerminalColor::TERM_ATTR_OFF<< endl;
}	


#pragma once
//
// ターミナル出力関連のシンプルな実装
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
#include <string>
#include <map>
#include <ostream>

namespace TerminalColor
{
	static const std::string BLACK			= "black";
	static const std::string DARKRED		= "darkred";
	static const std::string DARKGREEN		= "darkgreen";
	static const std::string DARKYELLOW		= "darkyellow";
	static const std::string DARKBLUE		= "darkblue";
	static const std::string DARKMAGENTA	= "darkmagenta";
	static const std::string DARKCYAN		= "darkcyan";
	static const std::string GREY			= "grey";
	
	static const std::string DARKGREY		= "darkgrey";
	static const std::string RED			= "red";
	static const std::string GREEN			= "green";
	static const std::string YELLOW			= "yellow";
	static const std::string BLUE			= "blue";
	static const std::string MAGENTA		= "magenta";
	static const std::string CYAN			= "cyan";
	static const std::string WHITE			= "white";
	
	static const std::string ESCST			= "\033[";
	static const std::string TERM_ATTR_OFF	= "0m";
	static const std::string TERM_BOLD		= "1";
	
};// TerminalColor
	


class SimpleTerminal
{
public:
	typedef std::map<std::string, std::string> colors;
	
protected:
	colors fg_color;
	colors bg_color;
	
	std::string fcolor;
	std::string bcolor;
	std::string attr;

	inline void setAttribute(const std::string &tcolor,const std::string &bcolor,bool bold);
	void _PutString(const std::string &str);

public:
	SimpleTerminal();
	~SimpleTerminal();

	void Put(const std::string &str,const std::string &tcolor="",const std::string &bcolor="",bool bold=false);
	void Puts(const std::string &str,const std::string &tcolor="",const std::string &bcolor="",bool bold=false);

	inline void Reset(){
		fcolor="39";	bcolor="49";	attr.erase();
		std::cout << TerminalColor::ESCST << TerminalColor::TERM_ATTR_OFF;
	}
	
	inline void setTextColor(const std::string &color){
		fcolor = fg_color[color];
	}

	inline void setBgColor(const std::string &color){
		bcolor = bg_color[color];
	}

	inline void setBold(){
		attr = TerminalColor::TERM_BOLD;
	}
	
};



#pragma once
//
// minisetting ARG=VAL型の定義物を取り込むような簡易クラス
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

// [想定している書式]
//
// 基本形は 変数=値 の形を認識します
//
// 変数の前後、値の前後に空白系文字がある場合は空白除去されます。
// 変数の間の空白は除去されます。
// 値の間の空白は記憶され、そのまま値となります。(ARG=VAL a sp の場合はVAL a spと認識)
// 行をまたがることはできません。
//
// 変数の前に#がある場合は行末までコメント扱いとなり、その行は無視されます。
// 
// 変数は全て文字列として認識します。
// 変数の値にfalse trueを設定すると Booleanとしても認識されます。
// 

#include <string>
#include <map>
#include <sstream>
#include <istream>
#include <ostream>
#include <iostream>
#include <iomanip>
#include <cctype>

namespace minisetting
{
	using namespace std;
	
	// 変数の値クラス
	class value
	{
	public:
	private:
		string	sv;
		bool	bv;
		
	public:
		value();
		~value();
		value(const string & str);
		value(const char* str);
		value(bool val);
		value(int val);
		value(unsigned int val);
		value(long int val);
		value(unsigned long int val);
		value(const value& x);
		value& operator=(const value& x);
		
		template <typename Tx> Tx get();
	};

	inline value::value() : bv(false){}
	
	#define CONSTR_STR(TNAME) \
		inline value::value(TNAME str) : bv(false) \
		{ sv = str; if(sv == "true") bv = true; }
	
	#define CONSTR_NUM(TNAME) \
		inline value::value(TNAME val) \
		{ stringstream ss; ss << val; sv = ss.str(); bv = (bool)val;}

	CONSTR_STR(const char *)
	CONSTR_STR(const string &)
	CONSTR_NUM(int)
	CONSTR_NUM(unsigned int)
	CONSTR_NUM(long int)
	CONSTR_NUM(unsigned long int)

	#undef CONSTR_STR
	#undef CONSTR_NUM
	
	
	inline value::value(bool val)
	{
		bv = val;
		// 文字でもtrue/falseは設定しておく
		if(bv)	sv = "true";
		else	sv = "false";
	}

	inline value::~value(){}
	
	inline value::value(const value& x) : bv(x.bv) {
		 sv = x.sv;
	}
	
	// Get系
	// 値を実際に取り出す際は取り出したい型にあわせて
	// strv = hoge.get<string>(); などととしてください。
	
	template <> inline bool value::get<bool>(){
		return bv;
	}
	template <> inline string value::get<string>(){
		return sv;
	}
	#define GET_NUMERICAL(TNAME) \
		template <> inline TNAME value::get<TNAME>() \
		{istringstream ss(sv); TNAME v; ss >> v; return v; }
	
	GET_NUMERICAL(int)
	GET_NUMERICAL(unsigned int)
	GET_NUMERICAL(long int)
	GET_NUMERICAL(unsigned long int)

	#undef GET_NUMERICAL

	inline value& value::operator=(const value& x) {
		if (this != &x) {
			this->~value();
			new (this) value(x);
		}
		return *this;
	}

	// ARG=VALを格納するマップ。解析した場合の結果が入ります
	typedef std::map<std::string, value> object;

	// 解析用クラス
	class parsestream
	{
	protected:
		istream		*istr;
		
		bool skip_w();
		void skip_n();
		bool parse_val(const string &arg,object &obj);
		bool parse_arg(object &obj);
		void _parse(object &obj);

	public:
		parsestream(){};
		~parsestream(){};
		
		void parse(const std::string &src,object &obj);
		void parse(istream &ifile,object &obj);
	};
	
	inline bool parsestream::skip_w()
	{
		while(istr->good()){
			int c = istr->get();
			if((c == ' ') || (c == '\t')) continue;
			
			istr->putback(c);
			return true;
		}
		return false;
	}
	
	inline void parsestream::skip_n()
	{
		while(istr->good()) if(istr->get() == '\n') break;
	}

	inline bool parsestream::parse_val(const string &arg,object &obj)
	{
		string val,wrd;
		skip_w();
		while(istr->good()){
			int c = istr->get();
			if(c == '\t') continue;
			if(! istr->good() || (c == '\n')){
				string::size_type right = wrd.find_last_not_of("\t ");
				if(right != string::npos) wrd.erase(right+1);
				value element(wrd);
				obj[arg] = element;
				return true;
			}
			wrd += c;
		}
		return false;
	}

	inline bool parsestream::parse_arg(object &obj)
	{
		string arg;
		while(istr->good()){
			int c = istr->get();
			switch(c){
			case ' ': case '\t':
				continue;
			case '\n':
				return true;
			case '=':
				return parse_val(arg,obj);
			}
			if(isalnum(c) || (c == '_') || (c == '@') || (c == '$')){
				arg += c;
			}else{
				skip_n();
				return true;
			}
		}
		return false;
	}
	
	inline void parsestream::_parse(object &obj)
	{
		while(istr->good()){
			if(! skip_w()) break;
			int c = istr->get();
			if(c == '#'){
				// コメントはスキップ
				skip_n();
				continue;
			}
			if(c == '\n'){
				// たぶん空行
				continue;
			}
			istr->putback(c);
			if(! parse_arg(obj)) break;
		}
	}

	inline void parsestream::parse(istream &ifile,object &obj){
		istr = &ifile;
		_parse(obj);
	}
	inline void parsestream::parse(const std::string &src,object &obj)
	{
		istringstream tmpstream(src);;
		parse(tmpstream,obj);
	}
	
	// -----------------------------

	// 解析インターフェイス
	// src: 解析したい文字列
	// obj: 結果
	inline void parse(const std::string &src,object &obj){
		parsestream pstream;
		pstream.parse(src,obj);
	}
	// src: 解析したいファイルストリーム
	// obj: 結果
	inline void parse(istream &ifile, object &obj){
		parsestream pstream;
		pstream.parse(ifile,obj);
	}
	
	// -----------------------------

	// 補助関数
	// 値をストリームに出力する
	inline void putval(ostream &ofstr, object &obj,const std::string &arg){
		ofstr << arg << " = " << obj[arg].get<string>() << endl;
	}
	// コメントをストリームに出力する
	inline void putcomment(ostream &ofile, const std::string &mes){
		ofile << "# " << mes << endl;
	}
	
};



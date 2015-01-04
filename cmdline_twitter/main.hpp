#pragma once
//
// コマンドラインTwitterアプリメイン
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
#include "typedef.hpp"
#include "minisetting.hpp"
#include "twitter_client.hpp"

class cmdlineOption
{
	#define DIM_PARAM(TYPE,NAME) \
	protected: TYPE	NAME; \
	public: inline const TYPE get##NAME() {return NAME;} \
			inline void set##NAME(const TYPE &val) {NAME=val;}; 
	
	DIM_PARAM(bool,ReadTL)
	DIM_PARAM(bool,DelTW)
	DIM_PARAM(bool,RetweetTL)
	DIM_PARAM(bool,FavTL)
	DIM_PARAM(bool,PostTL)
	DIM_PARAM(bool,SearchTL)
	DIM_PARAM(bool,Auth)
	DIM_PARAM(bool,List)
	DIM_PARAM(bool,Test)
	DIM_PARAM(bool,Silent)
	DIM_PARAM(bool,Direct)
	DIM_PARAM(bool,StreamAPI)
	DIM_PARAM(bool,useScreenName)
	DIM_PARAM(bool,Verbose)
	
	DIM_PARAM(std::string,Status)
	DIM_PARAM(std::string,Aries)
	DIM_PARAM(std::string,ScreenUser)
	DIM_PARAM(std::string,IDStr)
	DIM_PARAM(std::string,ListName)

	#undef DIM_PARAM

	public:
	cmdlineOption(){
		ReadTL 			= false;
		DelTW 			= false;
		RetweetTL	 	= false;
		FavTL 			= false;
		PostTL 			= false;
		SearchTL 		= false;
		Auth 			= false;
		List 			= false;
		Test 			= false;
		Silent			= false;
		Direct			= false;
		StreamAPI		= false;
		useScreenName	= false;
		Verbose			= false;
	}
	~cmdlineOption(){};
	
	// スクリーンネームが指定されてない状態かどうかを返す
	inline bool isNotVaridScreenUser()
		{ return ( (!getuseScreenName()) && (getScreenUser().empty()) );}
	
};

class mainApp
{
protected:
	cmdlineOption		opt;
	TwitterClient		client;
	minisetting::object	setting;

	void usage(int argc, char *argv[]);
	bool parse_cmdline(int argc, char *argv[]);
	void do_Authentication();
	bool readAccessKey();
	bool initUserInfo();
	void doSimpleUIMode();
	
	void initSetting();
	void readSetting();
	void writeSetting();

public:
	mainApp(){}
	~mainApp(){}

	int DoMain(int argc, char *argv[]);

};
	

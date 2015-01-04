#pragma once
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

#include "main.hpp"
#include "simple_terminal.hpp"

class SimpleUI
{
public:
	
protected:
	cmdlineOption opt;
	TwitterClient client;
	minisetting::object	setting;
	SimpleTerminal term;

	void init(cmdlineOption &option,TwitterClient &cent,minisetting::object &uset);
	
	void formatStatus(std::string &textstr);
	void printUser(picojson::object &tweet,picojson::object &uobj);
	void printRetweet(picojson::object &tweet,picojson::object &uobj,picojson::object &robj);

	
	void printTweet(picojson::object &tweet);
	void printTimeline(picojson::array &timeline);
	void printDM(picojson::object &tweet);
	void printDMline(picojson::array &timeline);
	void printList(picojson::array &lists);

	
	
	bool initUserInfo();
	void putRequestError();

	void ReadMemtioonTimeline();
	void ReadHomeTimeline();
	void ReadUserTimeline(const std::string &name);
	void ReadTweet(const std::string &idstr);
	void RemoveTimeline(const std::string &idstr);
	void PostTimeline(const std::string &status);
	void ReplyTimeline(const std::string &status,const std::string &idstr);
	void RetweetTimeline(const std::string &idstr);
	void FavoriteTimeline(const std::string &idstr);
	void ReadDirectMessaeg();
	void ReadDirectPost();
	void PostDirectMessage(const std::string &sname,const std::string &text);
	void RemoveDirectMessage(const std::string &idstr);
	void PutUserLists(const std::string &name);
	void ReadListTimeline(const std::string &name,const std::string &listname);
	void SearchTimeline(const std::string &ques);
	void ReadUserStream(const std::string &trackword);

	
	void UserStreamDirectMessage(picojson::object &jobj);
	void UserStreamDeleteTL(picojson::object &jobj);
	void UserStreamEvent(picojson::object &jobj);


	bool UserStreamCallback(picojson::object &jobj);
	static bool UserStreamCallbackEntry(picojson::object &jobj,void* userdata);
	
	static size_t request_test_callbk(char* ptr,size_t size,size_t nmemb,void* userdata);
	void RequestTest();
	
public:
	SimpleUI();
	~SimpleUI();
	
	void Execute(cmdlineOption &option,TwitterClient &cent,minisetting::object &uset);
};



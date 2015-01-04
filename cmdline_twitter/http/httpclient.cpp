//
// httpクライアントの実装
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
#include "httpclient.hpp"
#include <iostream>
#include <sstream>
#include <cctype>
#include <cstdio>
#include <iomanip>

using namespace std;

void url_encode(const std::string &src,std::string &dst)
{
	size_t i,mx;
	const char *s = src.c_str();
	dst.clear();
	mx = src.size();
	for(i=0;i<mx;i++){
		if(isalnum(*s) ||
			(*s == '~') || (*s == '-') || (*s == '_') || (*s=='.')
		){
			dst += *s;
		}else{
			ostringstream stream;
			stream << setfill('0') << setw(2) << uppercase << hex << static_cast<unsigned int>(static_cast<unsigned char>(*s));
			dst += '%';
			dst += stream.str();
		}
		s++;
	}
}

// GETやPOSTで使うリクエストデータをkey=value&key2=value2...の形にする
// valueはパーセントエンコードされた形に変形される
size_t HTTPClient::buildRequestData(HTTPRequestData &data,std::string &rstr)
{
	HTTPRequestData::iterator p;
	HTTPRequestData::iterator q;
	string cvalue;
	size_t sum=0;
	rstr.clear();
	
	for(p=data.begin();p != data.end();p++){
		rstr += p->first;
		url_encode(p->second,cvalue);
		rstr += '=';
		rstr += cvalue;
		sum++;
		q=p;
		q++;
		// データの組が次もあるなら&をつける
		if(q != data.end())	rstr += '&';
	}
	return sum;
}


// Getリクエストを送る
//
// url    : 開くURL
// data   : 渡すデータ。keyとValueは勝手に構築される。Valueはエンコードされる
// callbk : リクエストを受けたときのユーザ定義のコールバック関数(NULLでもよい)
// udata  : コールバック関数の引数
// return
//   falseなら接続関連のエラー	
bool HTTPClient::getRequest(const std::string &url,HTTPRequestData &data,Func_http_callback callbk,void* udata)
{
	string strdata;
	buildRequestData(data,strdata);
	return getRequest(url,strdata,callbk,udata);
}

// Postリクエストを送る
//
// url    : 開くURL
// data   : 渡すデータ。keyとValueは勝手に構築される。Valueはエンコードされる
// callbk : リクエストを受けたときのユーザ定義のコールバック関数(NULLでもよい)
// udata  : コールバック関数の引数
// return
//   falseなら接続関連のエラー	
bool HTTPClient::postRequest(const std::string &url,HTTPRequestData &data,Func_http_callback callbk,void* udata)
{
	string strdata;
	buildRequestData(data,strdata);
	return postRequest(url,strdata,callbk,udata);
}

// カスタムリクエストを送る
//
// url    : 開くURL
// data   : 渡すデータ。keyとValueは勝手に構築される。Valueはエンコードされる
// req    : リクエスト名
// callbk : リクエストを受けたときのユーザ定義のコールバック関数(NULLでもよい)
// udata  : コールバック関数の引数
// return
//   falseなら接続関連のエラー	
bool HTTPClient::customRequest(const std::string &url,HTTPRequestData &data,const std::string &req,Func_http_callback callbk,void* udata)
{
	string strdata;
	buildRequestData(data,strdata);
	return customRequest(url,strdata,req,callbk,udata);
}



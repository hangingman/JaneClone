//
// httpクライアントの実装 (cURL版)
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
#include "httpcurl.hpp"
#include <cstring>
#include <cstdio>

using namespace std;

const int CURL_ERROR_BUFSIZE = 1024;

//#define _DEBUG

#ifdef _DEBUG
#define dprint(fmt, arg...) \
	printf(": [Debug] "fmt, ## arg);
#else
#define dprint(fmt, arg...) \
do{	\
	\
}while(0)
#endif


HTTPCurl::HTTPCurl()
{
	init();
	m_proxyport = 0;
	m_respcode = 0;
	m_encording_gzip = false;
}

HTTPCurl::~HTTPCurl()
{
	cleanup();
}

// 内部コールバック用関数
size_t HTTPCurl::callbk_internal_entry(char* ptr,size_t size,size_t nmemb,void* userdata)
{
	if(userdata == NULL) return 0;
	HTTPCurl* pThat = reinterpret_cast<HTTPCurl*>(userdata);
	return pThat->do_internalCallbk(ptr,size,nmemb);
}

// perfomeから呼ばれるコールバック関数。この中で受信結果をバッファに入れる
size_t HTTPCurl::do_internalCallbk(char * ptr,size_t size,size_t nmemb)
{
	if(ptr == NULL)		return 0;
	if(size == 0)		return 0;
	
	size_t wsize = size*nmemb;
	size_t oldsize = m_responce.size();
	
	dprint("read data %lu\n",wsize);	
	// TODO: もっときれいな方法があれば…
	m_responce.resize(m_responce.size() + wsize);
	memcpy(&m_responce[oldsize],ptr,wsize);
	return wsize;
}

// 初期化を行う
bool HTTPCurl::init()
{
	m_curl = curl_easy_init();
	if(m_curl == NULL){
		setError(true);
		return false;
	}
	error_buffer = new char[CURL_ERROR_BUFSIZE];
	if(error_buffer == NULL){
		setError(true);
		return false;
	}
	m_curl_header = NULL;
	curl_easy_setopt(m_curl,CURLOPT_SSL_VERIFYPEER,0);
	setError(false);

	// コールバック系をこの段階でセット
	curl_easy_setopt(m_curl,CURLOPT_ERRORBUFFER,error_buffer);
	return true;
}

// 破棄するときのクリーンをする
void HTTPCurl::cleanup()
{
	if(m_curl != NULL){
		curl_easy_cleanup(m_curl);
	}
	m_curl = NULL;
	if(error_buffer != NULL){
		delete []error_buffer;
	}
	error_buffer = NULL;
	cleanHeader();
	cleanResponce();
}

bool HTTPCurl::isInit()
{
	if(m_curl == NULL)	return false;
	return true;
}

void HTTPCurl::cleanHeader()
{
	if(m_curl_header != NULL){
		curl_slist_free_all(m_curl_header);
	}
	m_curl_header = NULL;
}

void HTTPCurl::appendHeader(const std::string &header)
{
	m_curl_header = curl_slist_append(m_curl_header, header.c_str());
}


// Proxyを実際にセットする。内部用。
void HTTPCurl::setInternalProxy()
{
	// 一度セットしてたら二度はしない
	if(m_proxyset)	return;
	if(! isInit())	return;
	
	// とりあえずクリア
	curl_easy_setopt(m_curl,CURLOPT_PROXY,NULL);
	curl_easy_setopt(m_curl,CURLOPT_PROXYUSERPWD,NULL);
	curl_easy_setopt(m_curl,CURLOPT_PROXYAUTH,static_cast<long>(CURLAUTH_ANY));
	
	// 実際にcURLにセット
	if(! m_proxyurl.empty()){
		curl_easy_setopt(m_curl,CURLOPT_PROXY,m_proxyurl.c_str());
	}
	if(m_proxyport != 0){
		curl_easy_setopt(m_curl,CURLOPT_PROXYPORT,m_proxyport);
	}
	if(! m_proxyuser.empty()){
		curl_easy_setopt(m_curl,CURLOPT_PROXYUSERNAME,m_proxyuser.c_str());
	}
	if(! m_proxypass.empty()){
		curl_easy_setopt(m_curl,CURLOPT_PROXYPASSWORD,m_proxypass.c_str());
	}
	m_proxyset = true;
}

// ヘッダをセットしてURLを開く
//
// url    : 開くURL
// return
//   falseなら接続関連のエラー
bool HTTPCurl::Open(const std::string &url,Func_http_callback callbk,void* udata)
{
	if(! isInit())	return false;
	setError(false);

	// 前準備をここでやる
	setInternalProxy();
	cleanResponce();
	
	// コールバック関数をセット
	if(callbk == NULL){
	    curl_easy_setopt(m_curl,CURLOPT_WRITEFUNCTION,callbk_internal_entry);
		curl_easy_setopt(m_curl,CURLOPT_WRITEDATA,this);
	}else{
	    curl_easy_setopt(m_curl,CURLOPT_WRITEFUNCTION,callbk);
		curl_easy_setopt(m_curl,CURLOPT_WRITEDATA,udata);
	}
	// ACCEPT_ENCODINGの設定
	// この辺はcURLにお任せする。普通だとinfrate,gzipになってるはず
	curl_easy_setopt(m_curl, CURLOPT_ACCEPT_ENCODING, "");
	// ヘッダを実際にセット
	if(m_curl_header != NULL){
		 curl_easy_setopt(m_curl,CURLOPT_HTTPHEADER,m_curl_header);
	}
	
	curl_easy_setopt(m_curl,CURLOPT_URL,url.c_str());
	// いよいよ開く
#ifdef _DEBUG	
	curl_easy_setopt(m_curl, CURLOPT_VERBOSE, 1L);
#endif
	
	dprint("open URL %s ...\n",url.c_str());	
	if(curl_easy_perform(m_curl) != CURLE_OK){
		// 接続などに失敗
		cleanHeader();
		setError(true);
		dprint("open fail...\n");
		return false;
	}
	// 成功
	curl_easy_getinfo(m_curl,CURLINFO_HTTP_CODE,&m_respcode);
	dprint("open ok code %lu\n",m_respcode);
	// ヘッダはこの段階で消す
	cleanHeader();
	return true;
}


// Getリクエストを送る
//
// url    : 開くURL
// data   : 渡すデータ。エンコードされない
// return
//   falseなら接続関連のエラー	
bool HTTPCurl::getRequest(const std::string &url,const std::string &data,Func_http_callback callbk,void* udata)
{
	string xurl=url;
	if(! data.empty()){
		xurl += '?';
		xurl += data;
	}
	curl_easy_setopt(m_curl,CURLOPT_HTTPGET,1);
	return Open(xurl,callbk,udata);
	
}


// Postリクエストを送る
//
// url    : 開くURL
// data   : POSTするデータ(key=val&key2=val...形式)
// return
//   falseなら接続関連のエラー	
bool HTTPCurl::postRequest(const std::string &url,const std::string &data,Func_http_callback callbk,void* udata)
{
	curl_easy_setopt(m_curl,CURLOPT_HTTPPOST,1);
	if(! data.empty()){
		curl_easy_setopt(m_curl,CURLOPT_COPYPOSTFIELDS,data.c_str());		
	}else{
		// POSTがNULLだとSEGFAULTが起こる、、とりあえず空文字をセットしておく
		curl_easy_setopt(m_curl,CURLOPT_COPYPOSTFIELDS,"");		
	}
	return Open(url,callbk,udata);
}

// カスタムリクエストを送る
//
// url    : 開くURL
// data   : データ
// req    : リクエスト名
// return
//   falseなら接続関連のエラー	
bool HTTPCurl::customRequest(const std::string &url,const std::string &data,const std::string &req,Func_http_callback callbk,void* udata)
{
	curl_easy_setopt(m_curl,CURLOPT_CUSTOMREQUEST,req.c_str());
	if(! data.empty()){
		curl_easy_setopt(m_curl,CURLOPT_COPYPOSTFIELDS,data.c_str());		
	}else{
		// 多分やらないと不味いと思う
		curl_easy_setopt(m_curl,CURLOPT_COPYPOSTFIELDS,"");		
	}
	return Open(url,callbk,udata);
}



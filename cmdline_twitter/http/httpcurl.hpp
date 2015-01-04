#pragma once
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

#include "httpclient.hpp"
#include <curl/curl.h>


class HTTPCurl : public HTTPClient
{
protected:
	CURL*	m_curl;
	char*	error_buffer;
	struct curl_slist* m_curl_header;
	
	bool	m_proxyset;
	
	virtual bool init();
	virtual void cleanup();
	
	void setInternalProxy();
	
	static size_t callbk_internal_entry(char* ptr,size_t size,size_t nmemb,void* userdata);
	size_t do_internalCallbk(char * ptr,size_t size,size_t nmemb);

public:
	HTTPCurl();
	virtual ~HTTPCurl();
	
	virtual bool isInit();
	virtual bool Open(const std::string &url,Func_http_callback callbk,void* udata);
	
	virtual void appendHeader(const std::string &header);
	virtual void cleanHeader();
	
	using HTTPClient::getRequest;
	virtual bool getRequest(const std::string &url,const std::string &data,Func_http_callback callbk=NULL,void* udata=NULL);
	using HTTPClient::postRequest;
	virtual bool postRequest(const std::string &url,const std::string &data,Func_http_callback callbk=NULL,void* udata=NULL);
	using HTTPClient::customRequest;
	virtual bool customRequest(const std::string &url,const std::string &data,const std::string &req,Func_http_callback callbk=NULL,void* udata=NULL);
};



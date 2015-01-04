#pragma once
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

#include "typedef.hpp"
#include <string>
#include <vector>
#include <map>

// HTTP用リクエストデータ定義
typedef std::map<std::string,std::string>	HTTPRequestData;



class HTTPClient
{
public:
	// HTTPから取得したデータをコールバックで受け取るためのユーザ定義関数
	typedef size_t (*Func_http_callback)(char* ptr,size_t size,size_t nmemb,void* userdata);

protected:
	std::vector<uint8_t>	m_responce;					// レスポンスデータ
	bool					m_berror;					// エラー時はtrue
	unsigned long			m_respcode;					// レスポンス時のHTTPCode

	// proxy
	std::string				m_proxyurl;					// ProxyのURL
	std::string				m_proxyuser;				// Proxyのユーザ
	std::string				m_proxypass;				// Proxyのパスワード
	long int				m_proxyport;				// Proxyのポート番号

	bool					m_encording_gzip;			// EncordingにGzipを使うかどうか
	
	virtual bool init()=0;
	virtual void cleanup()=0;

public:
	HTTPClient(){};
	virtual ~HTTPClient(){};
	
	virtual bool isInit()=0;
	virtual void appendHeader(const std::string &header)=0;
	virtual void cleanHeader()=0;

	virtual bool Open(const std::string &url,Func_http_callback callbk,void* udata)=0;
	
	static size_t buildRequestData(HTTPRequestData &data,std::string &rstr);
	virtual bool getRequest(const std::string &url,const std::string &data,Func_http_callback callbk=NULL,void* udata=NULL)=0;
	virtual bool getRequest(const std::string &url,HTTPRequestData &data,Func_http_callback callbk=NULL,void* udata=NULL);

	virtual bool postRequest(const std::string &url,const std::string &data,Func_http_callback callbk=NULL,void* udata=NULL)=0;
	virtual bool postRequest(const std::string &url,HTTPRequestData &data,Func_http_callback callbk=NULL,void* udata=NULL);
	
	virtual bool customRequest(const std::string &url,const std::string &data,const std::string &req,Func_http_callback callbk=NULL,void* udata=NULL)=0;
	virtual bool customRequest(const std::string &url,HTTPRequestData &data,const std::string &req,Func_http_callback callbk=NULL,void* udata=NULL);
	
	
	inline bool isError()
	{
		return m_berror;
	}
	inline void setError(bool berr)
	{
		m_berror=berr;
	}
	// 最後にOpenしたHTTPのResponceコードを返す
	inline unsigned long getLastResponceCode()
	{
		return m_respcode;
	}
	
	// Responceをそのままポインタで返す。使用には注意。
	inline const std::vector<uint8_t>* getResponceData()
	{return &m_responce;}

	// ResponceをString型で返す
	inline std::string getResponceString()
	{
		std::string res(m_responce.begin(),m_responce.end());
		return res;
	}
	inline void cleanResponce()
	{
		m_respcode = 0;
		m_responce.clear();
	}
	
	inline void setProxy(const std::string &url,const std::string &user,long int port,const std::string &name,const std::string &passwd)
	{
		m_proxyurl = url;
		m_proxyuser = user;
		m_proxypass = passwd;
		m_proxyport = port;
	}
	
	inline void setEncordingGZip(bool used){
		m_encording_gzip = used;
	}
	inline bool getEncordingGZip(){
		return m_encording_gzip;
	}
};

extern "C" {
extern void url_encode(const std::string &src,std::string &dst);
}


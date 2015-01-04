#pragma once
//
// OAUTH 1.0クライアントの実装
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
#include "httpclient.hpp"
#include "hmac.hpp"


typedef std::map<std::string,std::string> OAuthArgMap;
typedef std::map<std::string,std::string> OAuthOtherRes;

namespace OAuthConst
{
	// ヘッダに付与する認証用文字列
	static const std::string	AUTHORIZATION_HEADER	= "Authorization: OAuth ";
	
	// OAuthArgMap用のキー
	static const std::string	ARG_CALLBACK			= "oauth_callback";
	static const std::string	ARG_CONSUMER_KEY		= "oauth_consumer_key";
	static const std::string	ARG_NONCE				= "oauth_nonce";
	static const std::string	ARG_SIGNATURE_METHOD	= "oauth_signature_method";
	static const std::string	ARG_SIGNATURE			= "oauth_signature";
	static const std::string	ARG_TIMESTAMP			= "oauth_timestamp";
	static const std::string	ARG_TOKEN				= "oauth_token";
	static const std::string	ARG_TOKENSECRET			= "oauth_token_secret";
	static const std::string	ARG_VERIFIER			= "oauth_verifier";
	static const std::string	ARG_VERSION				= "oauth_version";
	static const std::string	ARG_CONFIRMED			= "oauth_callback_confirmed";

	static const std::string	VAL_VERSION_1			= "1.0";
	

}; // namespace OAuthConst


class OAuth
{
	
protected:
	std::string		m_Comsumer_Key;
	std::string		m_Comsumer_Secret;
	std::string		m_Access_Key;
	std::string		m_Access_Secret;
	std::string		m_Verifier;
	OAuthArgMap		m_ArgMap;

	void recalcTimestamp();
	void initialArgs();

	size_t ArgstoString(HTTPRequestData &udata,std::string &rstr);
	size_t ArgstoHeaderString(std::string &rstr);
	
public:
	OAuth();
	
	OAuth(const OAuth &rhs){
		m_Comsumer_Key			= rhs.m_Comsumer_Key;
		m_Comsumer_Secret		= rhs.m_Comsumer_Secret;
		m_Access_Key			= rhs.m_Access_Key;
		m_Access_Secret			= rhs.m_Access_Secret;
	};
	
	OAuth& operator=(const OAuth &rhs){
		m_Comsumer_Key			= rhs.m_Comsumer_Key;
		m_Comsumer_Secret		= rhs.m_Comsumer_Secret;
		m_Access_Key			= rhs.m_Access_Key;
		m_Access_Secret			= rhs.m_Access_Secret;
		return *this;
	}
	
	virtual ~OAuth();

	// ---------------------------------------------------------
	// interface
	bool TemporaryCredentials(HTTPClient* peer,const std::string &url,const std::string &callbk);

	bool MakeResourceOwnerAuthUrl(const std::string &url,const std::string &dest);
	bool MakeResourceOwnerHttpData(const std::string &url,std::string &dest);
	bool MakeResourceOwnerHttpData(HTTPRequestData &httpdata);
	
	bool ResourceOwnerAuth(HTTPClient* peer,const std::string &url);
	bool TokenCredentials(HTTPClient* peer,const std::string &url,OAuthOtherRes &other);
	// ---------------------------------------------------------
	// low-level interface
	bool createSignatureBaseString(const std::string &request,const std::string &url,HTTPRequestData &udata,std::string &rstr);
	bool makeResuestHeader(const std::string &request,const std::string &url,HTTPRequestData &udata,std::string &header);
	bool parsingAuthResponce(const std::string &responce,std::string &confirmed,OAuthOtherRes &other);
	// ---------------------------------------------------------
	
	inline void setComsumerKey(const std::string &s)		{m_Comsumer_Key = s;}
	inline void setComsumerSecret(const std::string &s)		{m_Comsumer_Secret = s;}
	inline void setAccessKey(const std::string &s)			{m_Access_Key = s;}
	inline void setAccessSecret(const std::string &s)		{m_Access_Secret = s;}
	inline void setVerifier(const std::string &s)			{m_Verifier = s;}
	
	inline std::string getComsumerKey()					{return  m_Comsumer_Key;}
	inline std::string getComsumerSecret()				{return  m_Comsumer_Secret;}
	inline std::string getAccessKey()					{return  m_Access_Key;}
	inline std::string getAccessSecret()				{return  m_Access_Secret;}
	inline std::string getVerifier()					{return  m_Verifier;}

	inline bool emptyComsumerPair(){
		return (m_Comsumer_Key.empty() || m_Comsumer_Secret.empty());
	}
	inline bool emptyAccessPair(){
		return (m_Comsumer_Key.empty() || m_Comsumer_Secret.empty());
	}

	
};



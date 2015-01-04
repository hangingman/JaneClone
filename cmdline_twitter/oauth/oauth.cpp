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
#include "oauth.hpp"
#include "base64.hpp"
#include <cstring>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <sys/time.h>

using namespace std;


//#define _DEBUG

#ifdef _DEBUG
#define dprint(dstr) \
	cout << dstr << endl
#else
#define dprint(dstr) \
do{	\
	\
}while(0)
#endif

//#define TIMETEST


// 現在のmicro秒だけ返す
static time_t get_time_usecond(void)
{
	struct timeval tv;
	gettimeofday(&tv,NULL);
	return tv.tv_usec;
}


OAuth::OAuth()
{
}

OAuth::~OAuth()
{
}


// timestampとnonceを生成してArgをセットする
// timestampはそのまま現在時刻秒数。
// nonceは今のところ適当な乱数
//
void OAuth::recalcTimestamp()
{
	
	time_t tm = time(NULL);

#ifdef TIMETEST	
	m_ArgMap[OAuthConst::ARG_TIMESTAMP]	= "1404954187";
	m_ArgMap[OAuthConst::ARG_NONCE]		= "1aeee8bc909ebb55752435f0677e1fef";
#else
	ostringstream stream;
	stream << static_cast<long int>(tm);
	m_ArgMap[OAuthConst::ARG_TIMESTAMP]	= stream.str();
	stream.str("");
	stream << static_cast<long int>(tm) << static_cast<long int>(get_time_usecond());
	m_ArgMap[OAuthConst::ARG_NONCE] = stream.str();
#endif
}


void OAuth::initialArgs()
{
	// 要素は削除
	m_ArgMap.clear();
	
	// とりあえず固定のものを設定する
	recalcTimestamp();
	m_ArgMap[OAuthConst::ARG_VERSION]			= OAuthConst::VAL_VERSION_1;
	m_ArgMap[OAuthConst::ARG_SIGNATURE_METHOD]	= hmac_sha1::get_name();
	m_ArgMap[OAuthConst::ARG_CONSUMER_KEY]		= getComsumerKey();
	m_ArgMap[OAuthConst::ARG_TOKEN]				= getAccessKey();
	m_ArgMap[OAuthConst::ARG_VERIFIER]			= getVerifier();
}

// ARGを署名用の文字列に変換する
// arg=val&...の形となる
// val はURLENCORDしないといけない
// valが空の場合は、対になるARGは無視される
size_t OAuth::ArgstoString(HTTPRequestData &udata,std::string &rstr)
{
	OAuthArgMap tmpmap;
	tmpmap = m_ArgMap;

	rstr.clear();

	// HTTPリクエストデータの追加もしないといけない
	HTTPRequestData::iterator h;
	for(h=udata.begin();h != udata.end();h++){
		tmpmap[h->first] = h->second;
	}

	OAuthArgMap::iterator p = tmpmap.begin();
	string cvalue;

	// std::mapはソートされるはずだから順繰りでいい
	// ソートされない実装の場合は改めてソートしないとまずい
	while(p != tmpmap.end()){
		if(p->second.empty()){
			p++; continue;
		}
		rstr += p->first;
		url_encode(p->second,cvalue);
		rstr += '=';
		rstr += cvalue;
		p++;
		// データの組が次もあるなら&をつける
		if(p != tmpmap.end())	rstr += '&';
	}
	return tmpmap.size();
}

// ARGをHTTPヘッダ用の文字列に変換する
// arg="val", ... の形となる
// URLENCODEはしない
// valが空の場合は、対になるARGは無視される
size_t OAuth::ArgstoHeaderString(std::string &rstr)
{
	OAuthArgMap::iterator p = m_ArgMap.begin();

	rstr.clear();
	// std::mapはソートされるはずだから順繰りでいい
	// ソートされない実装の場合は改めてソートしないとまずい
	while(p != m_ArgMap.end()){
		if(p->second.empty()){
			p++; continue;
		}		
		rstr += p->first;
		rstr += "=\"";
		rstr += p->second;
		rstr += "\"";
		p++;
		// データの組が次もあるなら[, ]をつける
		if(p != m_ArgMap.end())	rstr += ", ";
	}
	return m_ArgMap.size();
}

// URLとリクエストとデータを投げて、署名する
bool OAuth::createSignatureBaseString(const std::string &request,const std::string &url,HTTPRequestData &udata,std::string &rstr)
{
	string encurl;
	string workstr;
	string hdatstr;
	string basestr_enced;
	
	rstr.clear();
	
	if(request.empty()) return false;
	
	initialArgs();

	// URLをエンコード
	url_encode(url,encurl);

	// POSTとヘッダのデータを寄せ集める ----------------------
	
	// ヘッダになりそうなものを追加。POSTやGETデータも含めないといけない
	ArgstoString(udata,workstr);
	dprint("[ArgstoString]"); dprint(workstr.c_str());
	hdatstr += workstr;
	workstr.clear();
	// 最後にこれらを変換
	url_encode(hdatstr,workstr);
	// --------------------------------------------------------
	
	// HTTPメソッド & 変換済みURL & 変換済みヘッダおよびデータ の形にする
	basestr_enced = request + '&' + encurl + '&' + workstr;
	dprint("[EncString]");  dprint(basestr_enced.c_str());

	string signkey;
	string signval;
	uint8_t *digest;
	digest = new uint8_t[hmac_sha1::get_digest_size()];
	
	// 署名キーを作成する -------------------------------------
	// consumer_secret & token_secretの形にする
	signkey = getComsumerSecret() + '&';
	if(! getAccessSecret().empty()){
		signkey += getAccessSecret();
	}
	// --------------------------------------------------------
	// HMAC-SHA1でダイジェスト生成
	hmac_sha1::create_digest(
		basestr_enced.c_str(),
		basestr_enced.size(),
		signkey.c_str(),
		signkey.size(),
		digest
	);
	// BASE64で変換したものをURLエンコードする->signval
	url_encode(base64::encode(digest,hmac_sha1::get_digest_size()),rstr);
	dprint("[Result]"); dprint(rstr.c_str());

	delete []digest;
	return true;
}

// URLとリクエストとデータを投げて、署名後ヘッダ文字列を返す。これで認証を行う
// request : HTTPメソッド
// url     : リクエストURL
// udata   : POSTデータなどのデータリスト
// header  : 結果が返ってくるヘッダ文字列。これをこのままHTTPヘッダにセットするといい
//
bool OAuth::makeResuestHeader(const std::string &request,const std::string &url,HTTPRequestData &udata,std::string &header)
{
	string signstr;
	string workstr;
	header.clear();
	// 何はともあれ、署名を作成
	if(! createSignatureBaseString(
		request,
		url,
		udata,
		signstr)
	){
		dprint("signature create miss!!");
		return false;
	}
	// 署名をARGリストに加える
	m_ArgMap[OAuthConst::ARG_SIGNATURE]		= signstr;
	// ヘッダ文字列を作成
	ArgstoHeaderString(workstr);
	header = OAuthConst::AUTHORIZATION_HEADER + workstr;
	dprint("[Header]"); dprint(header.c_str());
	return true;
}

// Temporary Credential Request,Resource Owner Authorization,
// Token Requestで使用されるレスポンスデータを解析して、
// しかるべきデータを格納する
bool OAuth::parsingAuthResponce(const std::string &responce,std::string &confirmed,OAuthOtherRes &other)
{
	string arg,val,tmpstr;
	
	confirmed.clear();
	other.clear();
	
	size_t curr=0,found=0;
	size_t found2;
	
	while(found != string::npos){
		found = responce.find_first_of('&',curr);
		tmpstr = responce.substr(curr,found - curr);
		curr = found + 1;
		
		found2 = tmpstr.find_first_of('=');
		if(found2 == string::npos) continue;
		arg = tmpstr.substr(0,found2);
		val = tmpstr.substr(found2+1);
		
		// 目的のARG値があるかどうか調べる
		if(arg == OAuthConst::ARG_TOKEN){
			setAccessKey(val);
		}else if(arg == OAuthConst::ARG_TOKENSECRET){
			setAccessSecret(val);
		}else if(arg == OAuthConst::ARG_CONFIRMED){
			confirmed = val;
		}else if(arg == OAuthConst::ARG_VERIFIER){
			setVerifier(val);
		}else{
			// そのほかの要素はとりあえず登録しておく
			other[arg] = val;
		}
	}
	return true;
}

// Temporary Credentialリクエストを行う
// コンシューマーキーとシークレットは必須
// 成功したら一時的なアクセスキーとシークレットが自動的に設定される
//
// peer: HTTPクライアントクラス
// url:  リクエストURL
// callbk: コールバックURL (なくてもいい)
//
bool OAuth::TemporaryCredentials(HTTPClient* peer,const std::string &url,const std::string &callbk)
{
	dprint("[TemporaryCredentials]");
	if(emptyComsumerPair())							return false;
	
	HTTPRequestData httpdata;
	string header;

	// コールバックが空の場合はoobを設定する仕様
	if(callbk.empty()){
		m_ArgMap[OAuthConst::ARG_CALLBACK] = "oob";
	}else{
		m_ArgMap[OAuthConst::ARG_CALLBACK] = url;
	}
	dprint("[Make Request Header]");

	// 署名する
	if(!makeResuestHeader(
		"POST",
		url,
		httpdata,
		header)
	){
		dprint("[Error] TemporaryCredentials::makeResuestHeader");
		return false;
	}

	// 署名をヘッダに設定してURLへアクセス
	dprint("[Access peer]");
	peer->appendHeader(header);
	if(! peer->postRequest(url,httpdata)){
		dprint("[Error] TemporaryCredentials::peer.open");
		return false;
	}
	// レスポンスを解析して結果を格納
	string responce = peer->getResponceString();

	dprint("[Responce]");
	dprint(responce);
	
	string confirmed;
	OAuthOtherRes dummy;
	parsingAuthResponce(responce,confirmed,dummy);
	
	// confirmedは必須らしい
	if(confirmed != "true"){
		dprint("[Error] TemporaryCredentials:: CONFIRMED not found or isnot true");
		return false;
	}
	if(getAccessKey().empty()){
		dprint("[Error] TemporaryCredentials:: ARG_TOKEN not found");
		return false;
	}
	if(getAccessSecret().empty()){
		dprint("[Error] TemporaryCredentials:: ARG_TOKENSECRET not found");
		return false;
	}
	
	dprint("[TemporaryCredentials success]");
	return true;
}

// Resource Owner Authorizationを行うためのHTTPデータを設定する
// 
// httpdata: 設定されるHTTPData これを使ってGetリクエストをする。
//
bool OAuth::MakeResourceOwnerHttpData(HTTPRequestData &httpdata)
{
	if(emptyComsumerPair())							return false;
	if(emptyAccessPair())							return false;
	httpdata[OAuthConst::ARG_TOKEN] = getAccessKey();
	return true;
}

// Resource Owner Authorizationを行うためのHTTP URLを生成する
// これをそのままブラウザに渡せばよい形になる
//
// src: 元となるResource Owner AuthorizationのURL
// dest: 変換後のResource Owner AuthorizationのURL
//
bool OAuth::MakeResourceOwnerHttpData(const std::string &url,std::string &dest)
{
	HTTPRequestData dt;
	dest.clear();
	if(! MakeResourceOwnerHttpData(dt))	return false;
	// GET用のURLを生成する
	string hstr;
	HTTPClient::buildRequestData(dt,hstr);
	dest = url + '?' + hstr;
	
	return true;
}

// Resource Owner Authorizationリクエストを行う
// 成功したら認証コードが自動的に設定される
// あらかじめTemporary Credentialリクエストに成功しておかないといけない
//
// もしくは、Temporary Credentialで取得した一時的なアクセスキー
// とシークレットを設定しないといけない。
//
// peer: HTTPクライアントクラス
// url:  リクエストURL
//
bool OAuth::ResourceOwnerAuth(HTTPClient* peer,const std::string &url)
{
	HTTPRequestData httpdata;
	dprint("[ResourceOwnerAuth]");

	// HTTPデータを生成する
	if(! MakeResourceOwnerHttpData(httpdata)) return false;
	
	// そのままの形でURLにアクセス
	dprint("[Access peer]");
	if(! peer->getRequest(url,httpdata)){
		dprint("[Error] ResourceOwnerAuth:: getRequest ");
		return false;
	}

	// 結果を解析して認証コードを得る
	string responce = peer->getResponceString();
	dprint("[Responce]");
	dprint(responce);
	
	string dmy;
	OAuthOtherRes dummyres;
	parsingAuthResponce(responce,dmy,dummyres);
	
	if(getVerifier().empty()){
		dprint("[Error] ResourceOwnerAuth:: ARG_VERIFIER not found ");
		return false;
	}
	dprint("[ResourceOwnerAuth success]");
	return true;
}

// TokenCredentialsリクエストを行う
//
// 成功したら永続的に使用可能なアクセスキーとシークレットが設定される
// あらかじめTemporary Credentialリクエスト、 Resource Owner Authorizationに
// 成功しておかないといけない
//
// もしくは、認証コードをsetVerifier()で設定して、Temporary Credential
// で取得した一時的なアクセスキーとシークレットを設定しないといけない。
//
// peer: HTTPクライアントクラス
// url:  リクエストURL
//
bool OAuth::TokenCredentials(HTTPClient* peer,const std::string &url,OAuthOtherRes &otherres)
{
	dprint("[TokenCredentials]");
	
	if(getVerifier().empty())						return false;
	if(emptyComsumerPair())							return false;
	if(emptyAccessPair())							return false;
	
	HTTPRequestData httpdata;
	string header;

	// 署名する
	if(!makeResuestHeader(
		"POST",
		url,
		httpdata,
		header)
	){
		dprint("[Error] TokenCredentials::makeResuestHeader");
		return false;
	}

	// 署名をヘッダに設定してURLへアクセス
	dprint("[Access peer]");	
	peer->appendHeader(header);
	if(! peer->postRequest(url,httpdata)){
		dprint("[Error] TokenCredentials::peer.open");
		return false;
	}
	// レスポンスを解析して結果を格納	
	string responce = peer->getResponceString();
	
	dprint("[Responce]");
	dprint(responce);
	
	string dmy;
	parsingAuthResponce(responce,dmy,otherres);
	if(getAccessKey().empty()){
		dprint("[Error] TokenCredentials:: ARG_TOKEN not found");
		return false;
	}
	if(getAccessSecret().empty()){
		dprint("[Error] TokenCredentials:: ARG_TOKENSECRET not found");
		return false;
	}
	dprint("[TokenCredentials success]");	
	return true;
}




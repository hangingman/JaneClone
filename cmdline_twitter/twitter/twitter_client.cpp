//
// twitter クライアントの実装
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
#include "twitter_client.hpp"
#include "base64.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;
using namespace TwitterRest1_1;


template<typename X> inline std::string tostring(X val)
{
	ostringstream stream;
	stream << val;
	return stream.str();
}



TwitterClient::TwitterClient()
{}


TwitterClient::~TwitterClient()
{}


void TwitterClient::setComsumerPair(const std::string &key,const std::string &sec)
{
	m_auth.setComsumerKey(key);
	m_auth.setComsumerSecret(sec);
}

void TwitterClient::setUserAccessPair(const std::string &key,const std::string &sec)
{
	m_auth.setAccessKey(key);
	m_auth.setAccessSecret(sec);
}

void TwitterClient::getUserAccessPair(std::string &key,std::string &sec)
{
	key = m_auth.getAccessKey();
	sec = m_auth.getAccessSecret();
}



// アプリの認証を行うURLを取得する。
bool TwitterClient::Authentication_GetURL(std::string &rurl)
{
	string callbk;
	
	rurl.clear();
	m_lasterror.clear();
	// まずはTemporaryCredentialsをする
	if(! m_auth.TemporaryCredentials(
		&m_peer,
		OAUTH1_REQUEST_TOKEN,
		callbk)
	){
		m_lasterror = "TemporaryCredentials failed";
		vprint(m_lasterror);
		return false;
	}
	// Resource Owner AuthorizatioのためのURLを生成する
	// あらかじめブラウザからログインして承認してもらう必要があるため
	m_auth.MakeResourceOwnerHttpData(OAUTH1_AUTHORIZE,rurl);
	return true;

}

// Pinをセットしてアプリの認証を完了させる。
bool TwitterClient::Authentication_Finish(const std::string &pin)
{
	// Owner Authorizationを行い、PINの正当性を確認
	// 成功したらAccessKeyとSecretは以後永続的に使える	
	OAuthOtherRes twitter_res;
	
	m_lasterror.clear();
	m_auth.setVerifier(pin);
	if(! m_auth.TokenCredentials(
		&m_peer,
		OAUTH1_ACCESS_TOKEN,
		twitter_res)
	){
		m_lasterror = "TokenCredentials failed";
		vprint(m_lasterror);
		return false;
	}
	// 認証OKだったのでスクリーンネームとUSERID取得
	m_user_screen	= twitter_res[OAUTH_ANSWER_SCREENNAME];
	m_user_id		= twitter_res[OAUTH_ANSWER_USERID];
	return true;
}

// --------------------------------------------------------------------

// エラーエントリがあるかどうか。ある場合はしかるべき処理をする
bool TwitterClient::parseJson_Errors(picojson::value &jsonval)
{
	picojson::object errobject = jsonval.get<picojson::object>();
	if(! errobject["errors"].is<picojson::array>()){
		// Errorではないか、もしくはまともなJSONではなさそう…
		return false;
	}
	picojson::array errinfoary = errobject["errors"].get<picojson::array>();
	picojson::object errinfo = errinfoary[0].get<picojson::object>();
	
	m_lasterror = "Twitter return error \n";
	m_lasterror += "Code: ";
	m_lasterror += errinfo["code"].to_str();
	m_lasterror += "\nMessage: \n";
	m_lasterror += errinfo["message"].to_str();
	vprint(m_lasterror);
	return true;
}


// JSONの解析を実際に行う
bool TwitterClient::parseJson(picojson::value &jsonval)
{
	unsigned long httpcode = m_peer.getLastResponceCode();

	// 5xx系のエラーの場合はたぶん落ちてるとかそんなのなのでまともな
	// JSONを期待してはいけない。ここはエラーとしてJSON解析はしない
	if(httpcode >= 500){
		m_lasterror = "responce HTTP 5xx \n";
		m_lasterror = "HTTP Message \n";
		m_lasterror += m_peer.getResponceString();
		vprint(m_lasterror);
		return false;
	}
	
	// JSONで帰ってくるので解析をする
	string json_err;
	string responce = m_peer.getResponceString();
	
	picojson::parse(jsonval,responce.begin(),responce.end(),&json_err);
	if(!json_err.empty()){
		m_lasterror = "[JSON] parse err!!! ";
		m_lasterror += json_err;
		m_lasterror += responce;
		vprint(m_lasterror);
		return false;
	}
	// ここまできたらたぶんJSON解析成功なのでJSONのみを出す
	if(isVerbose()) vprint(jsonval.serialize(true));
	
	if(httpcode >= 400){
		// パラメータエラーのときはHTTPコードが400系を返す
		if(! parseJson_Errors(jsonval)){
			// まともなJSONではなさそう…
			m_lasterror = "Twitter responce error \n";
			m_lasterror += m_peer.getResponceString();
			vprint(m_lasterror);
			return false;
		}
		return false;
	}
	return true;
}

// JSONの解析を実際に行う(Stream API向け)
bool TwitterClient::parseJsonStreams(const std::string src,picojson::object &jobj)
{
	picojson::value jsonval;
	string json_err;
	
	picojson::parse(jsonval,src.begin(),src.end(),&json_err);
	if(!json_err.empty()){
		m_lasterror = "[JSON] parse err!!! ";
		m_lasterror += json_err;
		vprint(m_lasterror);
		return false;
	}
	// ここまできたらたぶんJSON解析成功なのでJSONのみを出す
	if(isVerbose()) vprint(jsonval.serialize(true));
	
	if(!jsonval.is<picojson::object>()){
		m_lasterror = "[JSON] is not object... ";
		m_lasterror += src;
		vprint(m_lasterror);
		return false;
	}
	// errorsエントリがあるかどうか
	if(parseJson_Errors(jsonval)){
		return false;
	}
	
	jobj = jsonval.get<picojson::object>();
	return true;
}




// GETリクエストを投げる共通関数。ついでにJSON解析までやってしまう。
bool TwitterClient::getRequest(const std::string url,HTTPRequestData &hdata,picojson::value &jsonval)
{
	string authdata;
	m_lasterror.clear();
	
	m_auth.makeResuestHeader(
		"GET",
		url,
		hdata,
		authdata
	);
	
	m_peer.appendHeader(authdata);
	if(!m_peer.getRequest(
		url,
		hdata)
	){
		m_lasterror = "HTTP Get request failed";
		vprint(m_lasterror);
		return false;
	}
	// JSONで帰ってくるので解析をする
	return parseJson(jsonval);
}

template<typename Tx>
bool TwitterClient::getRequestJson(const std::string url,HTTPRequestData &hdata,Tx &rval)
{
	picojson::value jsonval;
	if(! getRequest(url,hdata,jsonval)){
		return false;
	}
	
	if(! jsonval.is<Tx>()){
		m_lasterror = "JSON error mismatch data type";
		vprint(m_lasterror);
		return false;
	}
	rval = jsonval.get<Tx>();
	return true;
}

// 自分で通信のコールバックを制御できる版、ストリーム通信などに利用する
bool TwitterClient::getRequestRaw(const std::string url,HTTPRequestData &hdata,HTTPClient::Func_http_callback fn,void *cbdata)
{
	string authdata;
	m_lasterror.clear();
	
	m_auth.makeResuestHeader(
		"GET",
		url,
		hdata,
		authdata
	);
	
	m_peer.appendHeader(authdata);
	if(!m_peer.getRequest(
		url,
		hdata,
		fn,
		cbdata)
	){
		m_lasterror = "HTTP Get request(RAW) failed";
		vprint(m_lasterror);
		return false;
	}
	// ここにきたら通信終わりかもしれない
	return true;
}


// 内部コールバック用関数
size_t TwitterClient::callbk_stream_internal_entry(char* ptr,size_t size,size_t nmemb,void* userdata)
{
	size_t wsize = size * nmemb;
	if(userdata == NULL)	return 0;
	if(wsize == 0)			return 0;
	TwitterClientStreamCbData*	pData = reinterpret_cast<TwitterClientStreamCbData*>(userdata);
	TwitterClient* 				pThat = reinterpret_cast<TwitterClient*>(pData->myClass);
	return pThat->do_internalStreamCallbk(ptr,wsize,pData);
}

// perfomeから呼ばれるStramingAPI用コールバック関数。この中で受信結果を一時バッファにいれ、JSON値を引き渡す
size_t TwitterClient::do_internalStreamCallbk(char * ptr,size_t wsize,TwitterClientStreamCbData *udata)
{
	size_t oldsize = m_bufSteam.size();
	m_bufSteam.resize(m_bufSteam.size() + wsize);
	memcpy(&m_bufSteam[oldsize],ptr,wsize);
	
	vprint("Streaming readed");
	// 特に何もしなくても JSON要素は今のところ 0x0d 0x0a で区切られているので
	// 0x0d 0x0aを探して、それごとにJSON解析を通す
	size_t found=0;	
	found = m_bufSteam.find("\r\n");
	while(found != string::npos){
		string streams = m_bufSteam.substr(0,found);
//		vprint(streams);
		m_bufSteam.erase(0,found+2);
		found = m_bufSteam.find("\r\n");
		// 空の場合（よく送られてくる）は何もしない
		if(streams.empty()) continue;

		picojson::object jobj;
		if(! parseJsonStreams(streams,jobj)){
			return 0;
		}
		if(! udata->fn(jobj,udata->data)) return 0;
	}
	return wsize;
}

// ストリーム通信などでコールバックにJsonのオブジェクトを取得できるgetRequest
bool TwitterClient::getRequestStreaming(const std::string url,HTTPRequestData &hdata,Func_stream_callback fn,void *cbdata)
{
	TwitterClientStreamCbData scb;

	m_bufSteam.clear();
	
	scb.fn		= fn;
	scb.data	= cbdata;
	scb.myClass	= (void*)this;
	
	return getRequestRaw(
		url,
		hdata,
		callbk_stream_internal_entry,
		&scb
	);
}



// POSTリクエストを投げる共通関数。ついでにJSON解析までやってしまう。
bool TwitterClient::postRequest(const std::string url,HTTPRequestData &hdata,picojson::value &jsonval)
{
	string authdata;
	m_lasterror.clear();
	
	m_auth.makeResuestHeader(
		"POST",
		url,
		hdata,
		authdata
	);
	
	m_peer.appendHeader(authdata);
	if(!m_peer.postRequest(
		url,
		hdata)
	){
		m_lasterror = "HTTP Post request failed";
		vprint(m_lasterror);
	}
	// JSONで帰ってくるので解析をする
	return parseJson(jsonval);
}

template<typename Tx>
bool TwitterClient::postRequestJson(const std::string url,HTTPRequestData &hdata,Tx &rval)
{
	picojson::value jsonval;
	if(! postRequest(url,hdata,jsonval)){
		return false;
	}
	
	if(! jsonval.is<Tx>()){
		m_lasterror = "JSON error mismatch data type";
		vprint(m_lasterror);
		return false;
	}
	rval = jsonval.get<Tx>();
	return true;
}

// 自分で通信のコールバックを制御できる版、ストリーム通信などに利用する
bool TwitterClient::postRequestRaw(const std::string url,HTTPRequestData &hdata,HTTPClient::Func_http_callback fn,void *cbdata)
{
	string authdata;
	m_lasterror.clear();
	
	m_auth.makeResuestHeader(
		"POST",
		url,
		hdata,
		authdata
	);
	
	m_peer.appendHeader(authdata);
	if(!m_peer.postRequest(
		url,
		hdata,
		fn,
		cbdata)
	){
		m_lasterror = "HTTP Post request(RAW) failed";
		vprint(m_lasterror);
		return false;
	}
	// ここにきたら通信終わりかもしれない
	return true;
}


bool TwitterClient::testRequest(
	const std::string url,
	HTTPRequestData &hdata,
	bool getreq,
	picojson::value &jsonval,
	std::string &result,
	unsigned long &httpres)
{
	bool ret = true;
	if(getreq){
		if(! getRequest(
			url,
			hdata,
			jsonval)
		){
			ret = false;
		}
	}else{
		if(! postRequest(
			url,
			hdata,
			jsonval)
		){
			ret = false;
		}
	}
	result = m_peer.getResponceString();
	httpres = m_peer.getLastResponceCode();
	return ret;
}

bool TwitterClient::testRequestRaw(
	const std::string url,
	HTTPRequestData &hdata,
	bool getreq,
	HTTPClient::Func_http_callback fn,
	void *cbdata,
	unsigned long &httpres)
{
	bool ret = true;
	if(getreq){
		if(! getRequestRaw(
			url,
			hdata,
			fn,
			cbdata)
		){
			ret = false;
		}
	}else{
		if(! postRequestRaw(
			url,
			hdata,
			fn,
			cbdata)
		){
			ret = false;
		}
	}
	httpres = m_peer.getLastResponceCode();
	return ret;
}


// -------------------------------------------------------------------------------------------
bool TwitterClient::getMentionsTimeline(
	uint16_t count,
	const std::string &since_id,const std::string &max_id,
	picojson::array &rtimeline)
{
	HTTPRequestData	httpdata;
	
	httpdata[PARAM_COUNT] = tostring(count);
	
	if(! since_id.empty())	httpdata[PARAM_SINCE_ID]	= since_id;
	if(! max_id.empty())	httpdata[PARAM_MAX_ID]		= max_id;

	if(! getRequestJson(
		TL_RESOURCE_STATUSES_MEMTION,
		httpdata,
		rtimeline)
	){
		vprint("err getMentionsTimeline");
		return false;
	}
	return true;
}

// 指定ユーザの発言を取得する。
// useridとscreennameが同時に指定された場合は、useridを優先する
bool TwitterClient::getUserTimeline(
	const std::string &userid,const std::string &screenname,
	uint16_t count,
	const std::string &since_id,const std::string &max_id,
	bool include_rts,bool include_replies,
	picojson::array &rtimeline)
{
	HTTPRequestData	httpdata;
	
	if(! userid.empty()){
		httpdata[PARAM_USER_ID] = userid;
	}else if(! screenname.empty()){
		httpdata[PARAM_SCREEN_NAME] = screenname;
	}else{
		// どっちかのパラメータをいれること
		return false;
	}
	
	httpdata[PARAM_COUNT] = tostring(count);
	httpdata[PARAM_INCLUDE_RTS]	= (include_rts		? VALUE_TRUE : VALUE_FALSE);
	httpdata[PARAM_EXC_REPLIES]	= (include_replies	? VALUE_FALSE : VALUE_TRUE);
	
	if(! since_id.empty())	httpdata[PARAM_SINCE_ID]	= since_id;
	if(! max_id.empty())	httpdata[PARAM_MAX_ID]		= max_id;
	
	if(! getRequestJson(
		TL_RESOURCE_STATUSES_USERTL,
		httpdata,
		rtimeline)
	){
		vprint("err getUserTimeline");
		return false;
	}
	return true;
}

// 自分自身の発言をGET
bool TwitterClient::getMyTimeline(
	uint16_t count,
	const std::string &since_id,const std::string &max_id,
	bool include_rts,bool include_replies,
	picojson::array &rtimeline)
{
	// 今のところスクリーンネームも一意であるはずだけど
	// 今後のこともあると思うので
	return getUserTimeline(
		getMyUserID(),"",
		count,
		since_id,max_id,
		include_rts,include_replies,
		rtimeline
	);
}


// タイムラインの取得
bool TwitterClient::getHomeTimeline(uint16_t count,
	const std::string &since_id,const std::string &max_id,
	bool include_rts,bool include_replies,
	picojson::array &rtimeline)
{
	HTTPRequestData	httpdata;
	
	httpdata[PARAM_COUNT] = tostring(count);
	httpdata[PARAM_INCLUDE_RTS]	= (include_rts		? VALUE_TRUE : VALUE_FALSE);
	httpdata[PARAM_EXC_REPLIES]	= (include_replies	? VALUE_FALSE : VALUE_TRUE);
	
	if(! since_id.empty())	httpdata[PARAM_SINCE_ID]	= since_id;
	if(! max_id.empty())	httpdata[PARAM_MAX_ID]		= max_id;

	if(! getRequestJson(
		TL_RESOURCE_STATUSES_HOMETL,
		httpdata,
		rtimeline)
	){
		vprint("err getHomeTimeline");
		return false;
	}
	return true;
}


bool TwitterClient::showTweet(const std::string &idstr,picojson::object &tweet)
{
	HTTPRequestData	httpdata;

	httpdata[PARAM_ID]				= idstr;
	httpdata[PARAM_TRIM_USER] 		= VALUE_FALSE;
	httpdata["include_my_retweet"] 	= VALUE_TRUE;

	if(! getRequestJson(
		TW_RESOURCE_STATUSES_SHOW_ID,
		httpdata,
		tweet)
	){
		vprint("err showTweet");
		return false;
	}
	return true;
}


bool TwitterClient::destroyStatus(const std::string &idstr,picojson::object &tweet)
{
	HTTPRequestData	httpdata;
	picojson::value jsonval;

	string url = TW_RESOURCE_STATUSES_DEL_ID;
	url += idstr;
	url += JSON_ENDPOINT;
	
	if(! postRequestJson(
		url,
		httpdata,
		tweet)
	){
		vprint("err DestroyStatus");
		return false;
	}
	return true;
}


// タイムラインへ投稿
bool TwitterClient::postStatus(const std::string &status,
	const std::string &reply_id,
	picojson::object &tweet)
{
	HTTPRequestData	httpdata;
	
	httpdata["status"] = status;
	httpdata[PARAM_TRIM_USER] = VALUE_FALSE;
	if(!reply_id.empty()){
		httpdata["in_reply_to_status_id"] = reply_id;
	}
	
	if(! postRequestJson(
		TW_RESOURCE_STATUSES_UPDATE,
		httpdata,
		tweet)
	){
		vprint("err putStatus");
		return false;
	}
	return true;
}

// リツイートする
bool TwitterClient::retweetStatus(const std::string &idstr,picojson::object &tweet)
{
	HTTPRequestData	httpdata;

	string url = TW_RESOURCE_STATUSES_RETWEET;
	url += idstr;
	url += JSON_ENDPOINT;

	if(! postRequestJson(
		url,
		httpdata,
		tweet)
	){
		vprint("err retweetStatus");
		return false;
	}
	return true;
}


// タイムライン検索
bool TwitterClient::searchTweets(const std::string &q,const std::string &lang,const std::string &restype,
		const std::string & since_id,const std::string & max_id,picojson::object &result)
{
	HTTPRequestData	httpdata;
	
	httpdata[PARAM_COUNT] 	= "100";
	httpdata["q"]			= q;
	httpdata["lang"]		= lang;
	httpdata["result_type"]	= restype;
	
	if(! since_id.empty())	httpdata[PARAM_SINCE_ID]	= since_id;
	if(! max_id.empty())	httpdata[PARAM_MAX_ID]		= max_id;
	
	if(! getRequestJson(
		TW_SEARCH_TWEETS,
		httpdata,
		result)
	){
		vprint("err searchTweets");
		return false;
	}
	return true;
}


// お気に入りに追加する
bool TwitterClient::createFavorites(const std::string &idstr,picojson::object &tweet)
{
	HTTPRequestData	httpdata;

	httpdata[PARAM_ID]	= idstr;

	if(! postRequestJson(
		TW_FABORITES_CREATE,
		httpdata,
		tweet)
	){
		vprint("err createFavorites");
		return false;
	}
	return true;
}


// ダイレクトメッセージを取得する。
bool TwitterClient::getDirectMessage(
	uint16_t count,
	const std::string &since_id,const std::string &max_id,
	picojson::array &rtimeline)
{
	HTTPRequestData	httpdata;
		
	httpdata[PARAM_COUNT] = tostring(count);
	
	if(! since_id.empty())	httpdata[PARAM_SINCE_ID]	= since_id;
	if(! max_id.empty())	httpdata[PARAM_MAX_ID]		= max_id;
	
	httpdata[PARAM_INCLUDE_ENTITIES]	= VALUE_FALSE;
	httpdata[PARAM_SKIP_STATUS]			= VALUE_FALSE;
	
	if(! getRequestJson(
		TW_DIRECT_GET,
		httpdata,
		rtimeline)
	){
		vprint("err getDirectMessage");
		return false;
	}
	return true;
}

// 自分が投稿したダイレクトメッセージを取得する。
bool TwitterClient::getDirectPosting(
	uint16_t count,
	const std::string &since_id,const std::string &max_id,
	picojson::array &rtimeline)
{
	HTTPRequestData	httpdata;
		
	httpdata[PARAM_COUNT] = tostring(count);
	
	if(! since_id.empty())	httpdata[PARAM_SINCE_ID]	= since_id;
	if(! max_id.empty())	httpdata[PARAM_MAX_ID]		= max_id;
	
	httpdata[PARAM_INCLUDE_ENTITIES]	= VALUE_FALSE;
	
	if(! getRequestJson(
		TW_DIRECT_SENT,
		httpdata,
		rtimeline)
	){
		vprint("err getDirectPosting");
		return false;
	}
	return true;
}

// DirectMessageを投稿
bool TwitterClient::postDirectMessage(
	const std::string &userid,const std::string &screenname,	
	const std::string &text,
	picojson::object &tweet)
{
	HTTPRequestData	httpdata;
	
	if(! userid.empty()){
		httpdata[PARAM_USER_ID] = userid;
	}else if(! screenname.empty()){
		httpdata[PARAM_SCREEN_NAME] = screenname;
	}else{
		// どっちかのパラメータをいれること
		return false;
	}
	httpdata["text"] = text;
	
	if(! postRequestJson(
		TW_DIRECT_NEW,
		httpdata,
		tweet)
	){
		vprint("err postDirectMessage");
		return false;
	}
	return true;
}

// DirectMessageを投稿
bool TwitterClient::removeDirectMessage(
	const std::string &idstr,
	picojson::object &tweet)
{
	HTTPRequestData	httpdata;
	
	if(idstr.empty()){
		return false;
	}
	httpdata[PARAM_ID] = idstr;
	httpdata[PARAM_INCLUDE_ENTITIES]	= VALUE_FALSE;
	
	if(! postRequestJson(
		TW_DIRECT_DESTROY,
		httpdata,
		tweet)
	){
		vprint("err removeDirectMessage");
		return false;
	}
	return true;
}



bool TwitterClient::getMyList(picojson::array &rlists)
{
	return getUserList(
		getMyUserID(),"",
		rlists
	);	
}


bool TwitterClient::getUserList(const std::string &userid,const std::string &screenname,picojson::array &rlists)
{
	HTTPRequestData	httpdata;
	picojson::value jsonval;
	
	if(! userid.empty()){
		httpdata[PARAM_USER_ID] = userid;
	}else if(! screenname.empty()){
		httpdata[PARAM_SCREEN_NAME] = screenname;
	}else{
		// どっちかのパラメータをいれること
		return false;
	}

	if(! getRequestJson(
		TW_LISTS_LIST,
		httpdata,
		rlists)
	){
		vprint("err getUserList");
		return false;
	}
	return true;
}


	
bool TwitterClient::getMyListTimeline(const std::string &slug,uint16_t count,
						const std::string &since_id,const std::string &max_id,
						bool include_rts,
						picojson::array &rtimeline)
{
	return getUserListTimeline(
		slug,
		getMyUserID(),"",
		count,
		since_id,max_id,
		include_rts,
		rtimeline
	);
}

bool TwitterClient::getUserListTimeline(const std::string &slug,
						const std::string &userid,const std::string &screenname,
						uint16_t count,
						const std::string &since_id,const std::string &max_id,
						bool include_rts,
						picojson::array &rtimeline)
{
	HTTPRequestData	httpdata;
	picojson::value jsonval;
	
	if(slug.empty())	return false;
	
	httpdata[PARAM_SLUG] = slug;
	
	if(! userid.empty()){
		httpdata[PARAM_OWNER_ID] = userid;
	}else if(! screenname.empty()){
		httpdata[PARAM_OWMER_SCREEN_NAME] = screenname;
	}else{
		// どっちかのパラメータをいれること
		return false;
	}
	
	httpdata[PARAM_COUNT] = tostring(count);
	httpdata[PARAM_INCLUDE_RTS]	= (include_rts		? VALUE_TRUE : VALUE_FALSE);
	
	if(! since_id.empty())	httpdata[PARAM_SINCE_ID]	= since_id;
	if(! max_id.empty())	httpdata[PARAM_MAX_ID]		= max_id;

	if(! getRequestJson(
		TW_LISTS_STATUSES,
		httpdata,
		rtimeline)
	){
		vprint("err getUserListTimeline");
		return false;
	}
	return true;	
}

// 自分のユーザ情報の取得
// last_status : 最後の発言などを含める
// entities : ProfileのURL情報などを含める(効いてない？？)
bool TwitterClient::verifyAccount(picojson::object &userinfo,bool last_status,bool entities)
{
	HTTPRequestData	httpdata;
	picojson::value jsonval;

	httpdata["include_entities"]	= (entities ? VALUE_TRUE : VALUE_FALSE);
	httpdata["skip_status"] 		= (last_status ? VALUE_FALSE : VALUE_TRUE);
	
	if(! getRequestJson(
		TW_USERS_ACCOUNT_VERIFY,
		httpdata,
		userinfo)
	){
		vprint("err verifyAccount");
		return false;
	}
	m_user_id		= userinfo[PARAM_ID_STR].to_str();
	m_user_name		= userinfo[PARAM_NAME].to_str();
	m_user_screen	= userinfo[PARAM_SCREEN_NAME].to_str();
	return true;
}



// UserStreamingの開始。切断されるまで帰ってこないので注意すること
bool TwitterClient::getUserStreaming(
	bool all_replies,
	bool with_following,
	const std::string trackword,
	Func_stream_callback fn,
	void *cbdata)
{
	HTTPRequestData	httpdata;
	
	if(all_replies)			httpdata["replies"]		= "all";
	if(! trackword.empty())	httpdata["track"]		= trackword;
	
	httpdata["with"]			= (with_following	? "followings" : "user");

	if(! getRequestStreaming(
		TW_STREAMING_USER,
		httpdata,
		fn,
		cbdata)
	){
		vprint("err getUserStreaming");
		return false;
	}
	return true;
}



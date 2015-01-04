#pragma once
//
// Twitter クライアントの実装
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
#include "httpcurl.hpp"
#include "picojson.h"


// Twitter REST1.1API用定義
namespace TwitterRest1_1
{
	// OAUTH 1
	static const std::string	OAUTH1_REQUEST_TOKEN	= "https://api.twitter.com/oauth/request_token";	// TemporaryCredentials
	static const std::string	OAUTH1_AUTHORIZE		= "https://api.twitter.com/oauth/authorize";		// ResourceOwnerAuth
	static const std::string	OAUTH1_ACCESS_TOKEN		= "https://api.twitter.com/oauth/access_token";		// TokenCredentials
	// Resource
	static const std::string	TL_RESOURCE_STATUSES_MEMTION	= "https://api.twitter.com/1.1/statuses/mentions_timeline.json";
	static const std::string	TL_RESOURCE_STATUSES_USERTL		= "https://api.twitter.com/1.1/statuses/user_timeline.json";
	static const std::string	TL_RESOURCE_STATUSES_HOMETL		= "https://api.twitter.com/1.1/statuses/home_timeline.json";
	static const std::string	TL_RESOURCE_STATUSES_RTOFME		= "https://api.twitter.com/1.1/statuses/retweets_of_me.json";
	// Tweets
	// Resource
	static const std::string	TW_RESOURCE_STATUSES_RTS_ID		= "https://api.twitter.com/1.1/statuses/retweets/"; 	// use ID.json
	static const std::string	TW_RESOURCE_STATUSES_SHOW_ID	= "https://api.twitter.com/1.1/statuses/show.json";
	static const std::string	TW_RESOURCE_STATUSES_DEL_ID		= "https://api.twitter.com/1.1/statuses/destroy/"; 		// use ID.json
	static const std::string	TW_RESOURCE_STATUSES_UPDATE		= "https://api.twitter.com/1.1/statuses/update.json";
	static const std::string	TW_RESOURCE_STATUSES_RETWEET	= "https://api.twitter.com/1.1/statuses/retweet/"; 		// use ID.json
	
	// Search
	static const std::string	TW_SEARCH_TWEETS				= "https://api.twitter.com/1.1/search/tweets.json";
	
	// Streaming
	static const std::string	TW_STREAMING_USER				= "https://userstream.twitter.com/1.1/user.json";
	
	// Direct Message
	static const std::string	TW_DIRECT_GET				= "https://api.twitter.com/1.1/direct_messages.json";
	static const std::string	TW_DIRECT_SENT				= "https://api.twitter.com/1.1/direct_messages/sent.json";
	static const std::string	TW_DIRECT_SHOW				= "https://api.twitter.com/1.1/direct_messages/show.json";
	static const std::string	TW_DIRECT_DESTROY			= "https://api.twitter.com/1.1/direct_messages/destroy.json";
	static const std::string	TW_DIRECT_NEW				= "https://api.twitter.com/1.1/direct_messages/new.json";
	
	
	
	// Users
	static const std::string	TW_USERS_ACCOUNT_VERIFY			= "https://api.twitter.com/1.1/account/verify_credentials.json";
	
	// Favorites
	static const std::string	TW_FABORITES_CREATE				= "https://api.twitter.com/1.1/favorites/create.json";

	// Lists
	static const std::string	TW_LISTS_LIST					= "https://api.twitter.com/1.1/lists/list.json";
	static const std::string	TW_LISTS_STATUSES				= "https://api.twitter.com/1.1/lists/statuses.json";
	
	// Help
	static const std::string	TW_HELP_CONFIGURATION			= "https://api.twitter.com/1.1/help/configuration.json";
	static const std::string	TW_HELP_RATE_LIMIT				= "https://api.twitter.com/1.1/application/rate_limit_status.json";
	
	
	// Other
	static const std::string	OAUTH_ANSWER_SCREENNAME			= "screen_name";
	static const std::string	OAUTH_ANSWER_USERID				= "user_id";
	// Search type
	static const std::string	SEARCH_RESTYPE_MIXED			= "mixed";
	static const std::string	SEARCH_RESTYPE_RECENT			= "recent";
	static const std::string	SEARCH_RESTYPE_POPULAR			= "popular";
	
	// often PARAM and VALUE
	static const std::string	PARAM_COUNT						= "count";
	static const std::string	PARAM_SINCE_ID					= "since_id";
	static const std::string	PARAM_MAX_ID					= "max_id";
	static const std::string	PARAM_INCLUDE_RTS				= "include_rts";
	static const std::string	PARAM_EXC_REPLIES				= "exclude_replies";
	static const std::string	PARAM_USER_ID					= "user_id";
	static const std::string	PARAM_SLUG						= "slug";
	static const std::string	PARAM_OWNER_ID					= "owner_id";
	static const std::string	PARAM_OWMER_SCREEN_NAME			= "owner_screen_name";
	static const std::string	PARAM_TRIM_USER					= "trim_user";
	static const std::string	PARAM_INCLUDE_ENTITIES			= "include_entities";
	static const std::string	PARAM_SKIP_STATUS				= "skip_status";
	
	
	static const std::string	VALUE_TRUE						= "true";
	static const std::string	VALUE_FALSE						= "false";
	
	static const std::string	PARAM_ID						= "id";
	static const std::string	PARAM_ID_STR					= "id_str";
	static const std::string	PARAM_NAME						= "name";
	static const std::string	PARAM_SCREEN_NAME				= "screen_name";
	static const std::string	PARAM_USER						= "user";
	static const std::string	PARAM_RTSTAT					= "retweeted_status";
	static const std::string	PARAM_SENDER					= "sender";
	static const std::string	PARAM_RECIPIENT					= "recipient";
	static const std::string	PARAM_CREATEAT					= "created_at";
	
	static const std::string	JSON_ENDPOINT					= ".json";
	
}; // namespace TwitterRest1_1

// UserStream等から取得したデータをコールバックで受け取るためのユーザ定義関数
typedef bool (*Func_stream_callback)(picojson::object &obj,void* userdata);

typedef struct{
	Func_stream_callback		fn;					// 呼び出されるコールバック関数
	void*						data;				// コールバック関数の引数
	void*						myClass;			// TwitterClient実体
}TwitterClientStreamCbData;

class TwitterClient
{
public:
	
protected:
	HTTPCurl	m_peer;
	OAuth		m_auth;
	bool		m_verbose;
	
	std::string	m_user_name;				// ユーザ名
	std::string	m_user_screen;				// スクリーン名(@でついているあれ)
	std::string	m_user_id;					// ユーザID
	
	std::string	m_bufSteam;					// StreamingAPI用取得関数
	std::string	m_lasterror;				// エラー用文字列
	
	bool getRequest(const std::string url,HTTPRequestData &hdata,picojson::value &jsonval);
	template<typename Tx> bool getRequestJson(const std::string url,HTTPRequestData &hdata,Tx &rval);
	
	bool postRequest(const std::string url,HTTPRequestData &hdata,picojson::value &jsonval);
	template<typename Tx> bool postRequestJson(const std::string url,HTTPRequestData &hdata,Tx &rval);
	
	bool getRequestRaw(const std::string url,HTTPRequestData &hdata,HTTPClient::Func_http_callback fn,void *cbdata);
	bool postRequestRaw(const std::string url,HTTPRequestData &hdata,HTTPClient::Func_http_callback fn,void *cbdata);

	bool getRequestStreaming(const std::string url,HTTPRequestData &hdata,Func_stream_callback fn,void *cbdata);
	
	bool parseJson(picojson::value &jsonval);
	bool parseJson_Errors(picojson::value &jsonval);
	bool parseJsonStreams(const std::string src,picojson::object &jobj);
	
	
	// CALLBACK
	static size_t callbk_stream_internal_entry(char* ptr,size_t size,size_t nmemb,void* userdata);
	size_t do_internalStreamCallbk(char * ptr,size_t wsize,TwitterClientStreamCbData *udata);
	
	
public:
	TwitterClient();
	
	TwitterClient(const TwitterClient &rhs){
		m_auth			= rhs.m_auth;
		m_verbose		= rhs.m_verbose;
		m_user_name		= rhs.m_user_name;
		m_user_screen	= rhs.m_user_screen;
		m_user_id		= rhs.m_user_id;
		m_bufSteam		= rhs.m_bufSteam;
		m_lasterror		= rhs.m_lasterror;
	};
	
	TwitterClient& operator=(const TwitterClient &rhs){
		m_auth			= rhs.m_auth;
		m_verbose		= rhs.m_verbose;
		m_user_name		= rhs.m_user_name;
		m_user_screen	= rhs.m_user_screen;
		m_user_id		= rhs.m_user_id;
		m_bufSteam		= rhs.m_bufSteam;
		m_lasterror		= rhs.m_lasterror;
		return *this;
	}	
	virtual ~TwitterClient();
	

	void setComsumerPair(const std::string &key,const std::string &sec);
	void setUserAccessPair(const std::string &key,const std::string &sec);
	void getUserAccessPair(std::string &key,std::string &sec);

	// OAuth
	bool Authentication_GetURL(std::string &rurl);
	bool Authentication_Finish(const std::string &pin);
	
	// statuses
	bool getMentionsTimeline(uint16_t count,
							const std::string &since_id,const std::string &max_id,
							picojson::array &rtimeline);
	
	bool getUserTimeline(const std::string &userid,const std::string &screenname,
						uint16_t count,
						const std::string &since_id,const std::string &max_id,
						bool include_rts,bool include_replies,
						picojson::array &rtimeline);
	
	bool getMyTimeline(uint16_t count,
						const std::string &since_id,const std::string &max_id,
						bool include_rts,bool include_replies,
						picojson::array &rtimeline);
	
	bool getHomeTimeline(uint16_t count,
						const std::string &since_id,const std::string &max_id,
						bool include_rts,bool include_replies,
						picojson::array &rtimeline);
	
	// Tweets:Resource
	bool showTweet(const std::string &idstr,picojson::object &tweet);
	bool destroyStatus(const std::string &idstr,picojson::object &tweet);
	bool postStatus(const std::string &status,
					const std::string &reply_id,
					picojson::object &tweet);
	bool retweetStatus(const std::string &idstr,picojson::object &tweet);
	
	// Search
	bool searchTweets(const std::string &q,const std::string &lang,const std::string &restype,
		const std::string & since_id,const std::string & max_id,picojson::object &result);
	
	bool verifyAccount(picojson::object &userinfo,bool last_status=false,bool entities=false);
	
	// Favorites
	bool createFavorites(const std::string &idstr,picojson::object &tweet);
	
	// Direct Message
	bool getDirectMessage(uint16_t count,
						const std::string &since_id,const std::string &max_id,
						picojson::array &rtimeline);
	bool getDirectPosting(uint16_t count,
						const std::string &since_id,const std::string &max_id,
						picojson::array &rtimeline);
	bool postDirectMessage(
						const std::string &userid,const std::string &screenname,	
						const std::string &text,
						picojson::object &tweet);
	bool removeDirectMessage(
						const std::string &idstr,
						picojson::object &tweet);
	
	
	// List
	bool getMyList(picojson::array &rlists);
	bool getUserList(const std::string &userid,const std::string &screenname,picojson::array &rlists);

	bool getMyListTimeline(const std::string &slug,uint16_t count,
						const std::string &since_id,const std::string &max_id,
						bool include_rts,
						picojson::array &rtimeline);

	bool getUserListTimeline(const std::string &slug,
						const std::string &userid,const std::string &screenname,
						uint16_t count,
						const std::string &since_id,const std::string &max_id,
						bool include_rts,
						picojson::array &rtimeline);

	// Streaming
	bool getUserStreaming(bool all_replies,
						bool with_following,
						const std::string trackword,
						Func_stream_callback fn,
						void *cbdata);
	
	
	inline void copyAuth(TwitterClient &rhs){
		m_auth			= rhs.m_auth;
	};

	inline std::string getMyUserName()					{return  m_user_name;}
	inline std::string getMyUserScreenName()			{return  m_user_screen;}
	inline std::string getMyUserID()					{return  m_user_id;}
	
	inline void serVerbose(bool set)					{m_verbose = set;}
	bool		isVerbose()								{return m_verbose;}
	
	// TESTREQUEST
	bool testRequest(const std::string url,
		HTTPRequestData &hdata,
		bool getreq,
		picojson::value &jsonval,
		std::string &result,
		unsigned long &httpres
	);
	bool testRequestRaw(const std::string url,
		HTTPRequestData &hdata,
		bool getreq,
		HTTPClient::Func_http_callback fn,
		void *cbdata,
		unsigned long &httpres
	);
	
	inline std::string getLastErrorMessage()			{return m_lasterror;}
	
	inline void vprint(const std::string &str){
		if(! isVerbose()) return;
		std::cout << str << std::endl;
	}
};



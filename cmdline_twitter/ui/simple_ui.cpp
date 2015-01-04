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
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>
#include <getopt.h>

#include "simple_ui.hpp"
#include "twitter_json.hpp"

using namespace std;


inline void ReplaceString(std::string &src,const std::string &from,const std::string &to)
{
	string::size_type p;
	p = src.find(from);
	while(p != string::npos){
		src.replace(p,from.length(),to);
		p += to.length();
		p = src.find(from, p);
	}
}


SimpleUI::SimpleUI()
{

}

SimpleUI::~SimpleUI()
{
}


void SimpleUI::putRequestError()
{
	cout << "リクエストに失敗" << endl;
	cout << client.getLastErrorMessage() << endl;
}

// 内部コールバック用関数
size_t SimpleUI::request_test_callbk(char* ptr,size_t size,size_t nmemb,void* userdata)
{
	if(size == 0)		return 0;
	
	size_t wsize = size*nmemb;
	cout << "read data size " << wsize <<endl;
	cout << ptr << endl;
	
	return wsize;
}

void SimpleUI::RequestTest()
{
	string url;
	string responce;
	string param,value;
	HTTPRequestData	httpdata;
	picojson::value jsonval;
	int getpost;
	bool bget=false;
	unsigned long rescode=0;
	
	if(opt.getStreamAPI()){
		cout << "!!! Streaming APIモードです !!!" << endl;
	}
	cout << "APIのURLを指定してください" << endl;
	cin >> url;
	
	cout << "GETかPOSTか指定してください(Get=1 post=それ以外)" << endl;
	cin >> getpost;
	if(getpost==1){
		bget = true;
	}
	cout << "パラメータ、RETURN、値…の順番で入力してください。CTRL+D で終わります" << endl;
	
	while(1){
		cin >> param;
		if(cin.eof())break;
		if(cin.fail())break;
		cin >> value;
		if(cin.eof())break;
		if(cin.fail())break;
		
		httpdata[param] = value;
		cout << param << ":" << value << endl;
	}
	cout << "リクエストを送信しています..." << endl;
	
	if(! opt.getStreamAPI()){
		// REST版
		if(! client.testRequest(
			url,
			httpdata,
			bget,
			jsonval,
			responce,
			rescode)
		){
			putRequestError();
		}
		cout << "\n" << endl;
		cout << "\nHTTP Responce Data" << endl;	
		cout << responce << endl;
		cout << "\nHTTP Responce Code" << endl;	
		cout << rescode << endl;
	}else{
		// Stream版
		if(! client.testRequestRaw(
			url,
			httpdata,
			bget,
			request_test_callbk,
			NULL,
			rescode)
		){
			putRequestError();
		}
		cout << "\n" << endl;
		cout << "\nHTTP Responce Code" << endl;	
		cout << rescode << endl;
	}
}


// Twitterでは標準時刻(UTC)が記述されているのでこれを現地時刻に直す
inline static void get_local_time_string(const std::string &src,std::string &dst)
{
#if defined(__MSYS__)
	// TODO: MSYS2にはstrptimeが機能していないので今はこうしている…
	dst = src;
#else
	string tmstr;
	time_t tm;
	struct tm tm_src,tm_dest;
	char tmpstr[64];
	
	memset(&tm_src,0,sizeof(struct tm));
	memset(&tm_dest,0,sizeof(struct tm));
	
	// 文字形式のをまずは変換
	strptime(src.c_str(),"%a %b %d %T %z %Y",&tm_src);
	// 現地時間に直す
	tm = timegm(&tm_src);		// mktimeはtmがローカルである場合だけ使える。この場合はこれ
	localtime_r(&tm,&tm_dest);
	
	strftime(tmpstr,sizeof(tmpstr),"[%Y-%m-%d %T]",&tm_dest);
	dst = tmpstr;
#endif	//__MINGW32__
}

void SimpleUI::formatStatus(std::string &textstr)
{
	// Twitterでは&lt &gt &ampだけは変換されるというわけわからん仕様みたいなので元に戻す
	ReplaceString(textstr,"&lt;","<");
	ReplaceString(textstr,"&gt;",">");
	ReplaceString(textstr,"&amp;","&");
}





void SimpleUI::printUser(picojson::object &tweet,picojson::object &uobj)
{
	using namespace picojson;
	using namespace TwitterJson;
	using namespace TwitterRest1_1;
	std::string tmstr;

	get_local_time_string(tweet[PARAM_CREATEAT].to_str(),tmstr);
	
	if(setting["VIEW_SHORT"].get<bool>()){
		if(setting["VIEW_SHORT_NAMEONLY"].get<bool>()){
			term.Put(uobj["name"].to_str() 	,setting["COLOR_NAME"].get<string>());
		}else{
			term.Put(uobj[PARAM_SCREEN_NAME].to_str() 	,setting["COLOR_SCREENNAME"].get<string>());
		}
		
		if(setting["VIEW_STATUSID"].get<bool>()){
			term.Put(" "  + tweet["id_str"].to_str() ,setting["COLOR_ID"].get<string>());
		}
		term.Put(" " + tmstr + " ",setting["COLOR_TIME"].get<string>());
	}else{
		term.Put(uobj["name"].to_str() 	,setting["COLOR_NAME"].get<string>());
		term.Put(" @" + uobj[PARAM_SCREEN_NAME].to_str() ,setting["COLOR_SCREENNAME"].get<string>());
		if(setting["VIEW_STATUSID"].get<bool>()){
			term.Put(" "  + tweet["id_str"].to_str() ,setting["COLOR_ID"].get<string>());
		}
		term.Puts(" " + tmstr ,setting["COLOR_TIME"].get<string>());
	}
	
	term.Reset();
}

void SimpleUI::printRetweet(picojson::object &tweet,picojson::object &uobj,picojson::object &robj)
{
	using namespace picojson;
	using namespace TwitterJson;
	using namespace TwitterRest1_1;
	object rtusr;
	string textstr;
	
	if(! TwitterJson::getUser(robj,rtusr)) return;
	
	textstr = robj["text"].to_str();	// Statusはこちらを使う
	formatStatus(textstr);

	term.Put("RT: " ,setting["COLOR_RTMARK"].get<string>());
	
	if(setting["VIEW_SHORT"].get<bool>()){
		printUser(robj,rtusr);
	}else{
		// RT元
		printUser(robj,rtusr);
		// RTした人
		term.Put(" from: " ,setting["COLOR_RTMARK"].get<string>());	
		printUser(tweet,uobj);
	}
		// 本文表示
	term.Puts(textstr,setting["COLOR_RTSTATUS"].get<string>());
	term.Reset();
}


void SimpleUI::printTweet(picojson::object &tweet)
{
	using namespace picojson;
	using namespace TwitterJson;
	using namespace TwitterRest1_1;
	
	string textstr;
	object uobj;
	object robj;

	if(! TwitterJson::getUser(tweet,uobj)) return;
	
	// RTのときは別処理
	if(TwitterJson::getReTweet(tweet,robj)){
		printRetweet(tweet,uobj,robj);
		return;
	}
	
	// 通常ツイート
	textstr = tweet["text"].to_str();
	formatStatus(textstr);
	
	printUser(tweet,uobj);
	term.Puts(textstr,setting["COLOR_STATUS"].get<string>());
	term.Reset();
}

void SimpleUI::printDM(picojson::object &tweet)
{
	using namespace picojson;
	using namespace TwitterJson;
	using namespace TwitterRest1_1;
	
	string tmstr,textstr;
	object sender;		// DMを送ったユーザ情報取得
	object receiver;	// DMを受け取ったユーザ情報取得

	if(! TwitterJson::getSender(tweet,sender)) return;
	if(! TwitterJson::getRecipient(tweet,receiver)) return;
	// 時間を直す
	get_local_time_string(tweet[PARAM_CREATEAT].to_str(),tmstr);
	
	textstr = tweet["text"].to_str();
	formatStatus(textstr);
	// 実際に出力
	term.Puts("Fm: ",setting["COLOR_DMMARK"].get<string>());
	term.Put(sender["name"].to_str() ,setting["COLOR_NAME"].get<string>());	
	term.Put(" @" + sender[PARAM_SCREEN_NAME].to_str() ,setting["COLOR_SCREENNAME"].get<string>());
	term.Put(" "  + tweet["id_str"].to_str() ,setting["COLOR_ID"].get<string>());
	term.Puts(" " + tmstr ,setting["COLOR_TIME"].get<string>());

	term.Puts("To: ",setting["COLOR_DMMARK"].get<string>());
	term.Put(receiver["name"].to_str() ,setting["COLOR_NAME"].get<string>());	
	term.Puts(" @" + receiver[PARAM_SCREEN_NAME].to_str() ,setting["COLOR_SCREENNAME"].get<string>());
	
	term.Puts(textstr,setting["COLOR_DMSTATUS"].get<string>());
	term.Reset();
}



// タイムラインを実際に出力する。検索結果表示やらHOME表示やらに使ってる
void SimpleUI::printTimeline(picojson::array &timeline)
{
	using namespace picojson;
	using namespace TwitterJson;
	using namespace TwitterRest1_1;

	// Twitterからの戻りは先が最新なので、逆順に表示
	picojson::array::reverse_iterator it;
	
	for(it=timeline.rbegin();it!=timeline.rend();it++){
		if(! it->is<object>()) continue;
		object obj = it->get<object>();
		printTweet(obj);
	}
}


// DMを実際に出力する。検索結果表示やらHOME表示やらに使ってる
void SimpleUI::printDMline(picojson::array &timeline)
{
	using namespace picojson;
	using namespace TwitterJson;
	using namespace TwitterRest1_1;

	// Twitterからの戻りは先が最新なので、逆順に表示
	picojson::array::reverse_iterator it;
	
	for(it=timeline.rbegin();it!=timeline.rend();it++){
		if(! it->is<object>()) continue;
		object obj = it->get<object>();
		printDM(obj);
	}
}


void SimpleUI::printList(picojson::array &lists)
{
	using namespace picojson;
	using namespace TwitterJson;
	using namespace TwitterRest1_1;
	
	string tmstr,textstr;

	picojson::array::iterator it;
	
	for(it=lists.begin();it!=lists.end();it++){
		object obj = it->get<object>();

		term.Puts( obj["slug"].to_str() + "  "  + obj["member_count"].to_str() + "users ["
			+ obj["mode"].to_str() + "]" , setting["COLOR_LISTNAME"].get<string>());
		
		term.Puts(obj["description"].to_str(),setting["COLOR_LISTDETAIL"].get<string>());
	}
	term.Reset();
}


// ------------------------------------------------------------------------------

// ユーザ情報の初期化
bool SimpleUI::initUserInfo()
{
	picojson::object info;
	// ただユーザ名とスクリーンネームとIDが知りたいだけ
	if(! client.verifyAccount(info)){
		putRequestError();
		return false;
	}
	return true;
}


// タイムラインを読む
void SimpleUI::ReadMemtioonTimeline()
{
	picojson::array timeline;
	if(! client.getMentionsTimeline(
		setting["READHOME_COUNT"].get<int>(),
		"",
		"",
		timeline)
	){
		putRequestError();
		return;
	}
	printTimeline(timeline);
}

// タイムラインを読む
void SimpleUI::ReadHomeTimeline()
{
	picojson::array timeline;
	if(! client.getHomeTimeline(
		setting["READHOME_COUNT"].get<int>(),
		"",
		"",
		setting["READHOME_VIEWRT"].get<bool>(),			// RT
		setting["READHOME_VIEWMENTION"].get<bool>(),	// @
		timeline)
	){
		putRequestError();
		return;
	}
	printTimeline(timeline);
}

// 指定ユーザのタイムラインを読む
void SimpleUI::ReadUserTimeline(const std::string &name)
{
	picojson::array timeline;
	
	if(name.empty()){
		if(! client.getMyTimeline(
			setting["READUSER_COUNT"].get<int>(),
			"",
			"",
			setting["READUSER_VIEWRT"].get<bool>(),			// RT
			setting["READUSER_VIEWMENTION"].get<bool>(),	// @
			timeline)
		){
			putRequestError();
			return;
		}
	}else{
		if(! client.getUserTimeline(
			"",
			name,
			setting["READUSER_COUNT"].get<int>(),
			"",
			"",
			setting["READUSER_VIEWRT"].get<bool>(),			// RT
			setting["READUSER_VIEWMENTION"].get<bool>(),	// @
			timeline)
		){
			putRequestError();
			return;
		}
	}
	printTimeline(timeline);
}


void SimpleUI::ReadTweet(const std::string &idstr)
{
	picojson::object tweet;
	if(! client.showTweet(idstr,tweet)){
		putRequestError();
		return;
	}
	printTweet(tweet);
}


void SimpleUI::RemoveTimeline(const std::string &idstr)
{
	picojson::object tweet;
	if(! client.destroyStatus(idstr,tweet)){
		putRequestError();
		return;
	}
	if(! opt.getSilent())	printTweet(tweet);
}

// 投稿する
void SimpleUI::PostTimeline(const std::string &status)
{
	picojson::object tweet;
	if(! client.postStatus(status,"",tweet)){
		putRequestError();
		return;
	}
	if(! opt.getSilent())	printTweet(tweet);
}

// リプライする
void SimpleUI::ReplyTimeline(const std::string &status,const std::string &idstr)
{
	using namespace TwitterRest1_1;
	picojson::object tweet;
	
	// @付けてるかチェックする
	if(status.find_first_of('@') != string::npos){
		if(! client.postStatus(status,idstr,tweet)){
			putRequestError();
			return;
		}
	}else{
		// @の指定がなかった場合、IDの発言より自動的にユーザを取得する
		picojson::object srctweet;
		if(! client.showTweet(idstr,srctweet)){
			putRequestError();
			return;
		}
		picojson::object uobj;
		if(! TwitterJson::getUser(srctweet,uobj)) return;
		
		std::string newstatus = "@" + uobj[PARAM_SCREEN_NAME].to_str() + " " + status;
		if(! client.postStatus(newstatus,idstr,tweet)){
			putRequestError();
			return;
		}
	}
	if(! opt.getSilent())	printTweet(tweet);
}

// リツィートする
void SimpleUI::RetweetTimeline(const std::string &idstr)
{
	picojson::object tweet;
	if(! client.retweetStatus(idstr,tweet)){
		putRequestError();
		return;
	}
	if(! opt.getSilent())	printTweet(tweet);
}

// お気に入りに追加する
void SimpleUI::FavoriteTimeline(const std::string &idstr)
{
	picojson::object tweet;
	if(! client.createFavorites(idstr,tweet)){
		putRequestError();
		return;
	}
	if(! opt.getSilent())	printTweet(tweet);
}


// DMを読む
void SimpleUI::ReadDirectMessaeg()
{
	picojson::array timeline;
	if(! client.getDirectMessage(
		setting["READDM_COUNT"].get<int>(),
		"",
		"",
		timeline)
	){
		putRequestError();
		return;
	}
	printDMline(timeline);
}

// 自分が送ったDMを読む
void SimpleUI::ReadDirectPost()
{
	picojson::array timeline;
	if(! client.getDirectPosting(
		setting["READDM_COUNT"].get<int>(),
		"",
		"",
		timeline)
	){
		putRequestError();
		return;
	}
	printDMline(timeline);
}

// DMを送る
void SimpleUI::PostDirectMessage(const std::string &sname,const std::string &text)
{
	picojson::object tweet;
	if(! client.postDirectMessage(
		"",
		sname,
		text,
		tweet)
	){
		putRequestError();
		return;
	}
	if(! opt.getSilent()) printDM(tweet);
}


// DMを消す
void SimpleUI::RemoveDirectMessage(const std::string &idstr)
{
	picojson::object tweet;
	if(! client.removeDirectMessage(
		idstr,
		tweet)
	){
		putRequestError();
		return;
	}
	if(! opt.getSilent()) printDM(tweet);
}


// ユーザリストの表示
void SimpleUI::PutUserLists(const std::string &name)
{
	picojson::array lists;
	
	if(name.empty()){
		if(! client.getMyList(lists)
		){
			putRequestError();
			return;
		}
	}else{
		if(! client.getUserList(
			"",
			name,
			lists)
		){
			putRequestError();
			return;
		}
	}
	printList(lists);
}


// ユーザリストを読む
void SimpleUI::ReadListTimeline(const std::string &name,const std::string &listname)
{
	picojson::array timeline;

	if(name.empty()){
		if(! client.getMyListTimeline(
			listname,
			setting["READLIST_COUNT"].get<int>(),
			"",
			"",
			setting["READLIST_VIEWRT"].get<bool>(),			// RT
			timeline)
		){
			putRequestError();
			return;
		}
	}else{
		if(! client.getUserListTimeline(
			listname,
			"",
			name,
			setting["READLIST_COUNT"].get<int>(),
			"",
			"",
			setting["READLIST_VIEWRT"].get<bool>(),			// RT
			timeline)
		){
			putRequestError();
			return;
		}
	}
	printTimeline(timeline);
}


// キーワード検索
void SimpleUI::SearchTimeline(const std::string &ques)
{
	picojson::array timeline;
	picojson::object result;
	if(! client.searchTweets(
		ques,
		"ja",
		TwitterRest1_1::SEARCH_RESTYPE_RECENT,
		"",
		"",
		result)
	){
		putRequestError();
		return;
	}
	// Searchはstatusesとsearch_metadataの２エントリがある
	if(! result["statuses"].is<picojson::array>()){
		return ;
	}
	timeline = result["statuses"].get<picojson::array>();
	printTimeline(timeline);
}

// ---------------------------------------------------------------------

void SimpleUI::UserStreamDirectMessage(picojson::object &jobj)
{
	picojson::object nobj;	
	cout << "Direct Messageイベントが着ました ----------------------------" << endl;
	nobj = jobj["direct_message"].get<picojson::object>();
	// DMはそのままやってくるっぽい
	printDM(nobj);
	cout << "-------------------------------------------------------------" << endl;
}

void SimpleUI::UserStreamDeleteTL(picojson::object &jobj)
{
	picojson::object nobj;
	picojson::value tmpval = jobj["delete"].get("status");
	
	if (! tmpval.is<picojson::object>()) return;
	nobj = tmpval.get<picojson::object>();
	
	cout << "つい消しを検出 ----------------------------------------------" << endl;
	cout << "ユーザID " << nobj["user_id_str"].to_str() << " が 発言ID " << nobj["id_str"].to_str() << " を消しました" << endl;
	cout << "-------------------------------------------------------------" << endl;
}

void SimpleUI::UserStreamEvent(picojson::object &jobj)
{
	cout << "get event object " << endl;
	cout << jobj["event"].serialize(true) << endl;
}





bool SimpleUI::UserStreamCallbackEntry(picojson::object &jobj,void* userdata)
{
	SimpleUI *pThat = reinterpret_cast<SimpleUI *>(userdata);
	return pThat->UserStreamCallback(jobj);

}

// ユーザストリーム用コールバック関数。ユーザストリームからデータが来るごとに呼び出される
bool SimpleUI::UserStreamCallback(picojson::object &jobj)
{
	picojson::object nobj;
	if(! jobj["text"].is<picojson::null>()){
		printTweet(jobj);
		return true;
	}
	if(! jobj["friends"].is<picojson::null>()){
		return true;
	}
	if(! jobj["friends_str"].is<picojson::null>()){
		return true;
	}
	if(! jobj["direct_message"].is<picojson::null>()){
		UserStreamDirectMessage(jobj);
		return true;
	}
	if(! jobj["event"].is<picojson::null>()){
		UserStreamEvent(jobj);
		return true;
	}
	if(! jobj["delete"].is<picojson::null>()){
		UserStreamDeleteTL(jobj);
		return true;
	}
	if(! jobj["scrub_geo"].is<picojson::null>()){
		return true;
	}
	if(! jobj["limit"].is<picojson::null>()){
		cout << "get limit object " << endl;
		cout << jobj["limit"].serialize(true) << endl;
		return true;
	}
	if(! jobj["status_withheld"].is<picojson::null>()){
		return true;
	}
	if(! jobj["user_withheld"].is<picojson::null>()){
		return true;
	}
	if(! jobj["disconnect"].is<picojson::null>()){
		cout << "get disconnect object " << endl;
		cout << jobj["disconnect"].serialize(true) << endl;
		return true;
	}
	if(! jobj["warning"].is<picojson::null>()){
		cout << "get warning object " << endl;
		cout << jobj["warning"].serialize(true) << endl;
		return true;
	}
	cout << "get ??? object " << endl;
	
	return true;
}

// UserStreamを使ってタイムラインを取得する
void SimpleUI::ReadUserStream(const std::string &trackword)
{
	if(! client.getUserStreaming(
			false,				// リプライはユーザに関連するもののみ
			true,				// ユーザとフォローに関連するもののみ
			trackword,
			UserStreamCallbackEntry,
			this)
	){
		putRequestError();
		return;	
	}
}

void SimpleUI::init(cmdlineOption &option,TwitterClient &cent,minisetting::object &uset)
{
	opt = option;
	client = cent;
	setting = uset;
}

void SimpleUI::Execute(cmdlineOption &option,TwitterClient &cent,minisetting::object &uset)
{
	init(option,cent,uset);
	
	// とりあえず自分自身の情報を取得
	initUserInfo();
	if(opt.getTest()){
		RequestTest();
		return;
	}
	
	if(opt.getDirect()){
		if(opt.getPostTL()){
			if(opt.getScreenUser().empty()){
				cout << "スクリーンネームを必ず指定してください" << endl;
				return;
			}
			PostDirectMessage(opt.getScreenUser(),opt.getStatus());
		}else if(opt.getReadTL()){
			if(opt.isNotVaridScreenUser()){
				// スクリーンネームが指定されてない場合はHOMEを表示
				ReadDirectMessaeg();
			}else{
				// 何か指定されているがとりあえず自分の発言を返す
				ReadDirectPost();
			}
		}else if(opt.getDelTW()){
			if(opt.getIDStr().empty()){
				cout << "IDを必ず指定してください" << endl;
				return;
			}
			RemoveDirectMessage(opt.getIDStr());
		}
		return;
	}
	
	if(opt.getPostTL()){
		if(opt.getIDStr().empty()){
			PostTimeline(opt.getStatus());
		}else{
			ReplyTimeline(opt.getStatus(),opt.getIDStr());
		}
		return;
	}
	
	if(opt.getDelTW()){
		if(opt.getIDStr().empty()){
			std::string instr;
			// IDが指定されていない場合はとりあえず表示
			ReadUserTimeline("");
			// IDを指定させる
			cout << "発言を消すIDを指定してください" << endl;
			cin >> instr;
			opt.setIDStr(instr);
		}
		RemoveTimeline(opt.getIDStr());
		return;
	}
	if(opt.getRetweetTL()){
		if(opt.getIDStr().empty()){
			std::string instr;
			// IDが指定されていない場合はとりあえず表示
			ReadHomeTimeline();
			// IDを指定させる
			cout << "発言をリツイートしたいIDを指定してください" << endl;
			cin >> instr;
			opt.setIDStr(instr);
		}
		RetweetTimeline(opt.getIDStr());
		if(! opt.getFavTL())	return;		// RtしてFavしたいこともあるので
	}
	
	if(opt.getFavTL()){
		if(opt.getIDStr().empty()){
			std::string instr;
			// IDが指定されていない場合はとりあえず表示
			ReadHomeTimeline();
			// IDを指定させる
			cout << "発言をお気に入りに入れたいIDを指定してください" << endl;
			cin >> instr;
			opt.setIDStr(instr);
		}
		FavoriteTimeline(opt.getIDStr());
		return;
	}
	
	if(opt.getSearchTL()){
		SearchTimeline(opt.getStatus());
		return;
	}
	
	if(opt.getList()){
		if(opt.getListName().empty()){
			std::string instr;
			PutUserLists("");
			// IDを指定させる
			cout << "表示したいリスト名を指定してください" << endl;
			cin >> instr;
			opt.setListName(instr);
		}
		ReadListTimeline("",opt.getListName());
		return;
	}
	
	
	if(opt.getReadTL()){
		if(opt.getStreamAPI()){
			ReadUserStream("");
		}else if(! opt.getIDStr().empty()){
			// IDに何か指定されている場合は対象のIDを表示
			ReadTweet(opt.getIDStr());
		}else if(opt.isNotVaridScreenUser()){
			// スクリーンネームが指定されてない場合はHOMEを表示
			ReadHomeTimeline();
		}else if(opt.getScreenUser() == "@"){
			// 自分に対するメンション
			ReadMemtioonTimeline();
		}else{
			// スクリーンネームが何か指定されている場合
			ReadUserTimeline(opt.getScreenUser());
		}
		return;
	}
}


//
// コマンドラインTwitter
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

#include "main.hpp"
#include "simple_ui.hpp"
#include "keys/apikeys.hpp"

#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

using namespace std;

// バージョン
static const string THIS_VERSION	= "PACKAGE_VERSION";

// 設定ファイル保存先
static const string	DEFAULT_AUTH_FILE = ".authkey_";
static const string	APP_DIR = ".ctw";
static const string	DEFAULT_SETTING_FILE = "ctwrc";


static int makedir(const string &dirname,mode_t mode)
{
#if defined(__MINGW32__)
	return mkdir(dirname.c_str());
#else
	return mkdir(dirname.c_str(),mode);
#endif
}

// アプリ設定ファイル用ディレクトリを取得＆作成
static bool get_app_dir(string &dirname)
{
	struct stat st;
	dirname = getenv("HOME");
	dirname += '/';
	dirname += APP_DIR;
	
	if(stat(dirname.c_str(),&st) != 0){
		if(makedir(dirname.c_str(),0700) != 0){
			return false;
		}
	}
	dirname += '/';
	
	return true;

}


// 一連の認証動作を行う
void mainApp::do_Authentication()
{
	string rurl;
	string pincode;
	
	cout << "認証手続きをしています..." << endl;
	if(!client.Authentication_GetURL(rurl)){
		return;
	}
	cout << "以下のURLにアクセスして認証後、PINを入力してください" << endl;
	cout << rurl << endl;
	cin >> pincode;
	cout << "認証中です..." << endl;
	if(!client.Authentication_Finish(pincode)){
		cout << "認証に失敗しました。再度認証しなおしてください" << endl;
		return;
	}
	cout << "認証に成功しました" << endl;
	
	string key,sec,fname;
	ofstream fout;
	
	// ~/.ctw/以下に保存する
	if(! get_app_dir(fname)){
		cout << "HOME/.ctw/ ディレクトリを作成できません。" << endl;
		return;
	}
	fname += DEFAULT_AUTH_FILE;
	if(! opt.getAries().empty()) fname += opt.getAries();
	
	client.getUserAccessPair(key,sec);
	fout.open(fname.c_str(),ios::out);
	if(! fout.is_open()){
		cout << "設定ファイルを開けませんでした。" << endl;
		cout << "再度認証しなおしてください" << endl;
		
		return;
	}
	fout << key << endl;
	fout << sec << endl;
	fout.close();
	return;
}

// 保存している認証コードを読みこむ
bool mainApp::readAccessKey()
{
	string key,sec,fname;
	ifstream fin;
	
	// ~/.ctw/以下から読み込みする
	if(! get_app_dir(fname)){
		cout << "HOME/.ctw/ ディレクトリを作成できません。" << endl;
		return false;
	}
	fname += DEFAULT_AUTH_FILE;
	if(! opt.getAries().empty()) fname += opt.getAries();
	fin.open(fname.c_str());
	if(! fin.is_open()){
		cout << "設定ファイルを開けませんでした。" << endl;
		cout << "指定されたエイリアス名が間違っているかもしれません。" << endl;
		cout << "または、--authオプションで認証する必要があります" << endl;
		return false;
	}
	
	fin >> key;
	fin >> sec;
	fin.close();
	if(key.empty() || sec.empty()){
		cout << "設定ファイルが壊れています。再度認証してください" << endl;
		return false;
	}
	client.setUserAccessPair(key,sec);
	return true;
}

// 設定ファイル系の初期化
void mainApp::initSetting()
{
	setting["READHOME_COUNT"]		= minisetting::value(200);
	setting["READHOME_VIEWRT"]		= minisetting::value(true);
	setting["READHOME_VIEWMENTION"] = minisetting::value(true);
	
	setting["READUSER_COUNT"]		= minisetting::value(200);
	setting["READUSER_VIEWRT"]		= minisetting::value(true);
	setting["READUSER_VIEWMENTION"] = minisetting::value(true);

	setting["READDM_COUNT"]			= minisetting::value(200);

	setting["READLIST_COUNT"]		= minisetting::value(200);
	setting["READLIST_VIEWRT"]		= minisetting::value(true);
	
	setting["VIEW_SHORT"]			= minisetting::value(false);
	setting["VIEW_SHORT_NAMEONLY"]	= minisetting::value(true);
	setting["VIEW_STATUSID"]		= minisetting::value(true);
	
	setting["COLOR_NAME"]			= minisetting::value(TerminalColor::GREEN);
	setting["COLOR_SCREENNAME"]		= minisetting::value(TerminalColor::BLUE);
	setting["COLOR_ID"]  			= minisetting::value(TerminalColor::MAGENTA);
	setting["COLOR_TIME"]  			= minisetting::value(TerminalColor::DARKYELLOW);
	setting["COLOR_STATUS"]  		= minisetting::value(TerminalColor::GREY);
	setting["COLOR_RTMARK"]  		= minisetting::value(TerminalColor::GREEN);
	setting["COLOR_RTSTATUS"]  		= minisetting::value(TerminalColor::GREY);
	setting["COLOR_DMMARK"]  		= minisetting::value(TerminalColor::GREEN);
	setting["COLOR_DMSTATUS"]	 	= minisetting::value(TerminalColor::GREY);
	setting["COLOR_LISTNAME"] 	 	= minisetting::value(TerminalColor::GREEN);
	setting["COLOR_LISTDETAIL"]  	= minisetting::value(TerminalColor::GREY);
	
}

// 設定ファイルの読み込み
void mainApp::readSetting()
{
	using namespace minisetting;
	string fname;
	ifstream fin;
	
	initSetting();
	
	// ~/.ctw/以下から読み込みする
	if(! get_app_dir(fname)){
		cout << "HOME/.ctw/ ディレクトリを作成できません。" << endl;
		cout << "設定ファイルを読み込めませんでした。" << endl;
		cout << "デフォルトの設定を使用します" << endl;
		return;
	}
	fname += DEFAULT_SETTING_FILE;
	fin.open(fname.c_str());
	if(! fin.is_open()){
		cout << "設定ファイルを開けませんでした。" << endl;
		cout << "デフォルトの設定を使用します" << endl;
		writeSetting();
		return;
	}
	parse(fin,setting);
	fin.close();
}

// 設定ファイルが存在しない場合に新規に作成する
void mainApp::writeSetting()
{
	using namespace minisetting;
	string fname;
	ofstream fout;
	
	// ~/.ctw/以下に保存する
	if(! get_app_dir(fname)){
//		cout << "HOME/.ctw/ ディレクトリを作成できません。" << endl;
//		cout << "設定ファイルを書き込めませんでした。" << endl;
//		cout << "デフォルトを使用します" << endl;
		return;
	}
	fname += DEFAULT_SETTING_FILE;
	
	fout.open(fname.c_str(),ios::out);
	if(! fout.is_open()){
//		cout << "設定ファイルを書き込めませんでした。" << endl;
//		cout << "デフォルトを使用します" << endl;
		return;
	}
	
	putcomment(fout,"cmd line twitter Setting file");
	putcomment(fout,"");
	putcomment(fout,"");
	fout << endl;
	putcomment(fout,"ホームタイムライン読み込み設定");
	putcomment(fout,"ホームタイムラインの読み込み件数(20-200)");
	putval(fout,setting,"READHOME_COUNT");
	putcomment(fout,"ホームタイムラインでRTを表示するかどうか(true false)");
	putval(fout,setting,"READHOME_VIEWRT");
	putcomment(fout,"ホームタイムラインで@を表示するかどうか(true false)");
	putval(fout,setting,"READHOME_VIEWMENTION");
	fout << endl;
	putcomment(fout,"ユーザタイムライン読み込み設定");
	putcomment(fout,"ユーザタイムラインの読み込み件数(20-200)");
	putval(fout,setting,"READUSER_COUNT");
	putcomment(fout,"ユーザタイムラインでRTを表示するかどうか(true false)");
	putval(fout,setting,"READUSER_VIEWRT");
	putcomment(fout,"ユーザタイムラインで@を表示するかどうか(true false)");
	putval(fout,setting,"READUSER_VIEWMENTION");
	fout << endl;
	putcomment(fout,"ダイレクトメッセージ読み込み設定");
	putcomment(fout,"ダイレクトメッセージの読み込み件数(20-200)");
	putval(fout,setting,"READDM_COUNT");
	fout << endl;
	putcomment(fout,"リスト読み込み設定");
	putcomment(fout,"リストタイムラインの読み込み件数(20-200)");
	putval(fout,setting,"READLIST_COUNT");
	putcomment(fout,"リストタイムラインでRTを表示するかどうか(true false)");
	putval(fout,setting,"READLIST_VIEWRT");
	fout << endl;
	fout << endl;
	putcomment(fout,"表示設定");
	putcomment(fout,"短縮表示をおこなうかどうか(true false)");
	putval(fout,setting,"VIEW_SHORT");
	putcomment(fout,"短縮表示のとき、名前を登録名(スクリーンネーム以外)にする(true false)");
	putval(fout,setting,"VIEW_SHORT_NAMEONLY");
	putcomment(fout,"発言IDの表示をおこなうかどうか(true false)");
	putval(fout,setting,"VIEW_STATUSID");
	fout << endl;
	fout << endl;
	putcomment(fout,"色設定");
	putcomment(fout,"ユーザー名の色");
	putval(fout,setting,"COLOR_NAME");
	putcomment(fout,"スクリーンネームの色");
	putval(fout,setting,"COLOR_SCREENNAME");
	putcomment(fout,"発言IDの色");
	putval(fout,setting,"COLOR_ID");
	putcomment(fout,"発言内容の色");
	putval(fout,setting,"COLOR_STATUS");
	putcomment(fout,"RTマークの色");
	putval(fout,setting,"COLOR_RTMARK");
	putcomment(fout,"RT内容の色");
	putval(fout,setting,"COLOR_RTSTATUS");
	putcomment(fout,"DMマークの色");
	putval(fout,setting,"COLOR_DMMARK");
	putcomment(fout,"DM内容の色");
	putval(fout,setting,"COLOR_DMSTATUS");
	putcomment(fout,"リスト名の色");
	putval(fout,setting,"COLOR_LISTNAME");
	putcomment(fout,"リスト詳細の色");
	putval(fout,setting,"COLOR_LISTDETAIL");
	fout << endl;
	fout << endl;
	
	putcomment(fout,"end file");
	
	fout.close();
}





void mainApp::usage(int argc, char *argv[])
{
	fprintf(stdout,
	 "Command-line Twritter Version [%s]\n"
	 "Usage: %s [options]\n"
	 "初めて使用するときは -a オプションでこのアプリの認証を行ってください\n"
	 "\n"
	 "[Options]\n"
	 "-h | --help          Print this message\n"
	 "-a | --auth          [再]認証を行う\n"
	 "                     -u オプションでエイリアスを指定できます\n"
	 "-D | --Direct        DM関連の操作を行う\n"
	 "                     -p オプションと-n オプションでユーザ名指定でダイレクトメッセージを送る\n"
	 "                     -r オプションでダイレクトメッセージを読む\n"
	 "                     -r と -n オプションで\"\"と指定すると自分の発言を読む\n"
	 "                     -d と -i オプションでID指定でメッセージを消す\n"
	 "-p | --post status   タイムラインへ投稿\n"
	 "                     -i オプションでそのIDに対してのリプライ動作\n"
	 "                     (@は自分で付けてください。@省略時は@が自動付与されます)\n"
	 "-s | --search word   ワードで検索\n"
	 "-S | --Silent        サイレントモード (投稿やFavなどの結果を表示しない)\n"
	 "-r | --readtl        ホームのタイムラインを読む\n"
	 "                     -x オプションでUserStreamを使って読む(以後のオプションは無視)\n"
	 "                     -n オプションでユーザ名指定すると指定ユーザを読む\n"
	 "                     -n オプションで\"\"と指定すると自分の発言を読む\n"
	 "                     -n オプションで\"@\"と指定すると自分へのメンションを読む\n"
	 "                     -i オプションで単発のツイートを読む\n"
	 "-d | --del           発言の削除 -iでID指定\n"
	 "-R | --Retweet       リツイートする -iでID指定\n"
	 "-F | --Fav           お気に入りに追加する -iでID指定\n"
	 "-l | --list name     自分のリストnameの内容を読む\n"
	 "                     nameで\"\"と指定すると自分のリスト一覧を表示する\n"
	 "-n | --name          指定が必要な場合のユーザスクリーンネーム\n"
	 "-i | --id            指定が必要な場合の発言ID\n"
	 "-u | --user alies    エイリアス名指定:省略可(-a とも併用可能)\n"
	 "-x | --xstream       Streaming APIを使う(使用可能な場合)\n"
	 "-T | --Test          (テスト用)APIのエンドポイントを指定してAPIリクエストを行う\n"
	 "                     -x オプションでStreaming向け接続を行う\n"
	 "-v | --verbose       (デバッグ用)余計な文字を出力しまくる\n"
	 "\n"
	 "-u で指定できるエイリアス名は別アカウントで使いたい場合に便利です\n"
	 "実際のスクリーンネームでなくてもかまいません。\n"
	 "\n"
	 "[Example]\n"
	 " i am testman と投稿する:\n"
	 "ctw -p \" i am testman \" \n"
	 "\n"
	 " エイリアス egg で i am egg と投稿する:\n"
	 "ctw -u egg -p \" i am egg \" \n"
	 "\n"
	 " エイリアス egg として認証作業を行う:\n"
	 "ctw -u egg -a\n"
	 "",
	 THIS_VERSION.c_str(),argv[0]
	);
}

namespace CMDLINE_OPT
{
	enum {
		PUT_HELP	= 1,
		AUTH,
		DELTW,
		DIRECT,
		FAVORITES,
		POST,
		READTL,
		RETWEET,
		SCREEN,
		SEARCH,
		SILENT,
		USER,
		ID,
		LIST,
		STREAMAPI,
		TEST,
		VERBOSE,
	};
};

static const struct option
long_options[] = {
	{ "auth",		no_argument,		NULL, CMDLINE_OPT::AUTH			},
	{ "del",		no_argument,		NULL, CMDLINE_OPT::DELTW		},
	{ "Direct",		no_argument,		NULL, CMDLINE_OPT::DIRECT		},
	{ "Fav",		no_argument,		NULL, CMDLINE_OPT::FAVORITES	},
	{ "help",		no_argument,		NULL, CMDLINE_OPT::PUT_HELP		},
	{ "id",			required_argument,	NULL, CMDLINE_OPT::ID			},
	{ "list",		required_argument,	NULL, CMDLINE_OPT::LIST			},
	{ "name",		required_argument,	NULL, CMDLINE_OPT::SCREEN		},
	{ "post",		required_argument,	NULL, CMDLINE_OPT::POST			},
	{ "readtl",		no_argument,		NULL, CMDLINE_OPT::READTL		},
	{ "Retweet",	no_argument,		NULL, CMDLINE_OPT::RETWEET		},
	{ "search",		required_argument,	NULL, CMDLINE_OPT::SEARCH		},
	{ "Silent",		no_argument,		NULL, CMDLINE_OPT::SILENT		},
	{ "user",		required_argument,	NULL, CMDLINE_OPT::USER			},
	{ "xstream",	no_argument,		NULL, CMDLINE_OPT::STREAMAPI	},
	{ "Test",		no_argument,		NULL, CMDLINE_OPT::TEST			},
	{ "verbose",	no_argument,		NULL, CMDLINE_OPT::VERBOSE		},
	{ 0, 0, 0, 0 }
};


bool mainApp::parse_cmdline(int argc,char *argv[])
{
	while(1){
		int c = getopt_long_only(argc,argv,"",long_options,NULL);
		
		if(c == -1)		break;		// -1は解析終わり
		switch (c) {
		case 0:
			break;
		case CMDLINE_OPT::PUT_HELP:
			usage(argc, argv);
			return false;
			
		case CMDLINE_OPT::AUTH:
			opt.setAuth(true);
			break;

		case CMDLINE_OPT::DELTW:
			opt.setDelTW(true);
			break;

		case CMDLINE_OPT::DIRECT:
			opt.setDirect(true);
			break;
			
		case CMDLINE_OPT::POST:
			opt.setStatus(optarg);
			opt.setPostTL(true);
	        break;
			
		case CMDLINE_OPT::READTL:
			opt.setReadTL(true);
	        break;

		case CMDLINE_OPT::RETWEET:
			opt.setRetweetTL(true);
	        break;
			
		case CMDLINE_OPT::FAVORITES:
			opt.setFavTL(true);
	        break;
			
		case CMDLINE_OPT::SEARCH:
			opt.setStatus(optarg);
			opt.setSearchTL(true);
	        break;

		case CMDLINE_OPT::LIST:
			opt.setList(true);
			opt.setListName(optarg);
	        break;
			
		case CMDLINE_OPT::ID:
			opt.setIDStr(optarg);
	        break;

		case CMDLINE_OPT::SCREEN:
			opt.setuseScreenName(true);
			opt.setScreenUser(optarg);
	        break;
			
		case CMDLINE_OPT::USER:
			opt.setAries(optarg);
	        break;

		case CMDLINE_OPT::SILENT:
			opt.setSilent(true);
	        break;
			
		case CMDLINE_OPT::STREAMAPI:
			opt.setStreamAPI(true);
	        break;
			
		case CMDLINE_OPT::VERBOSE:
			opt.setVerbose(true);
	        break;

		case CMDLINE_OPT::TEST:
			opt.setTest(true);
	        break;
			
		default:
	        usage(argc, argv);
			return false;
		}
	}
	return true;
}

void mainApp::doSimpleUIMode()	
{
	SimpleUI ui;
	ui.Execute(opt,client,setting);
}

int mainApp::DoMain(int argc,char *argv[])
{
	if(argc == 1){
		usage(argc, argv);
		return 0;
	}
	tzset();
	// このアプリのコンシューマキーなどを設定
	client.setComsumerPair(AP_COMSUMER_KEY,AP_COMSUMER_SECRET);

	// コマンドライン解析
	if(! parse_cmdline(argc,argv)){
		return -1;
	}

	client.serVerbose(opt.getVerbose());
	if(opt.getAuth()){
		do_Authentication();
		// 認証の場合はいったんここで終わり
		return 0;
	}
	readSetting();
	
	// ここから先はユーザのアクセスキーが必要
	if(! readAccessKey()){
		return -1;
	}
	// TODO: simpleモードしか今はないが、対話モードができてきた
	//       場合はこの辺で処理をわけます
	doSimpleUIMode();
	return 0;
}

int main(int argc,char *argv[])
{
	mainApp app;
	return app.DoMain(argc,argv);
}



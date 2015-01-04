#pragma once

//
// Twitter JSON用定義
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

#include "picojson.h"
#include "twitter_client.hpp"


namespace TwitterJson
{
	inline bool getObject(const std::string &key,picojson::object &src,picojson::object &uobj){
		if(! src[key].is<picojson::object>()){
			return false;
		}
		uobj = src[key].get<picojson::object>();
		return true;
	}

	inline bool getUser(picojson::object &src,picojson::object &uobj)
	{ return getObject(TwitterRest1_1::PARAM_USER ,src,uobj);}
	
	inline bool getReTweet(picojson::object &src,picojson::object &uobj)
	{ return getObject(TwitterRest1_1::PARAM_RTSTAT ,src,uobj);}
	
	inline bool getSender(picojson::object &src,picojson::object &uobj)
	{ return getObject(TwitterRest1_1::PARAM_SENDER ,src,uobj);}
	
	inline bool getRecipient(picojson::object &src,picojson::object &uobj)
	{ return getObject(TwitterRest1_1::PARAM_RECIPIENT ,src,uobj);}
	
	
}; // namespace TwitterJson

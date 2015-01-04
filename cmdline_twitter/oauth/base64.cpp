//
// BASE64変換
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
#include "base64.hpp"
#include <algorithm>
using namespace std;

static const string s_convstr="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";


std::string base64::encode(const void* data,size_t datalen)
{
	uint8_t const *ws = reinterpret_cast<uint8_t const*>(data);
	uint8_t const *we = ws + datalen;
	uint8_t rd,cid,wid;
	string rstr;
	bool ending = false;
	
	// AAAAAABB BBBBCCCC CCDDDDDD EEEEEEFF ...
	// 00AAAAAA 00BBBBBB 00CCCCCC 00DDDDDD ...
	while(ws < we){
		rd = *ws;
		// R0
		cid = rd >> 2;
		wid = (rd & 0b00000011) << 4;
		rstr += s_convstr[cid];
		ws ++;
		
		if(ws >= we){
			rd = 0;
			ending = true;
		}else{
			rd = *ws;
		}
		// R1
		cid = wid | (rd >> 4);
		wid = (rd & 0b00001111) << 2;
		rstr += s_convstr[cid];
		if(ending){
			rstr += '=';
			rstr += '=';
			break;
		}
		ws ++;
		// R2 R3
		if(ws >= we){
			rd = 0;
			ending = true;
		}else{
			rd = *ws;
		}
		cid = wid | rd >> 6;
		wid = (rd & 0b00111111);
		rstr += s_convstr[cid];
		if(ending){
			rstr += '=';
			break;
		}
		rstr += s_convstr[wid];
		ws ++;
	}
	return rstr;
}

// HTTPのPOSTで送る場合などの亜種変換
std::string base64::encode_url(const void* data,size_t datalen)
{
	string rstr = base64::encode(data,datalen);
	replace(rstr.begin(), rstr.end(), '+', '-');
	replace(rstr.begin(), rstr.end(), '/', '_');
	return rstr;
}


static uint8_t decode_bit(uint8_t c,bool &ends,bool &fail)
{
	fail = false;
	ends = false;
	if((c >= 'A') && (c <= 'Z'))	return c - 'A';
	if((c >= 'a') && (c <= 'z'))	return c - 'a' + 26;
	if((c >= '0') && (c <= '9'))	return c - '0' + 52;
	if(c == '+')					return 62;
	if(c == '/')					return 63;
	ends = true;
	if(c == '=')					return 0;

	fail = true;
	return 0;

}

bool base64::decode(const void *in,size_t inlen,void *out,size_t &olen)
{
	using namespace std;
	uint8_t const *ws = reinterpret_cast<uint8_t const*>(in);
	uint8_t const *we = ws + inlen;
	uint8_t *ost = reinterpret_cast<uint8_t *>(out);
	uint8_t cid,rd[4];
	size_t i;
	bool ends[4],fail[4];
	olen = 0;
	
	// 00AAAAAA 00BBBBBB 00CCCCCC 00DDDDDD ...
	// AAAAAABB BBBBCCCC CCDDDDDD EEEEEEFF ...
	while(ws < we){
		if(ws + 4 > we){
			// invalid!!
			return false;
		}
		for(i=0;i<4;i++){
			rd[i] = decode_bit(ws[i],ends[i],fail[i]);
			if(fail[i])	return false;
		}
		if(ends[0]) return false;		// error...	
		if(ends[1]) return false;		// error...	
		// R0
		cid =  rd[0] << 2;
		cid |= (rd[1] >> 4);
		*ost = cid;	ost++; olen++;
		if(ends[2]) break;
//		if(ends[3]) break;
		
		// R1
		cid = rd[1] << 4;
		cid |= rd[2] >> 2;
		*ost = cid;	ost++; olen++;
		if(ends[3]) break;
		
		// R2
		cid = rd[2] << 6;
		cid |= rd[3];
		*ost = cid;	ost++; olen++;
		
		ws += 4;
	}
	return true;
}


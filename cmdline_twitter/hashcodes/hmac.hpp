#pragma once

//
// HMAC 鍵付ハッシングクラス
// HMAC: Keyed-Hashing for Message Authentication
// RFC2104 を参照のこと
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
#include <cstring>
#include <string>

#include "sha1.hpp"
#include "md5.hpp"

template<class _Thash>
class hmac
{
	enum {
		PAD_BLOCK		= 64,
	};
public:
	hmac(){};
	virtual ~hmac(){};
	
	static void create_digest(const void* data,size_t datalen,const void* key,size_t keylen,uint8_t *resdigest);
	static void create_digest(const void* data,size_t datalen,const void* key,size_t keylen,std::string &resdigest);
	
	static size_t get_digest_size(){
		return _Thash::DIGEST_LENGTH;
	}
	static const std::string get_name()
	{
		std::string name;
		_Thash	hasher;
		name = "HMAC-";
		name += hasher.get_name();
		return name;
	}
};

// HMACダイジェストを計算して結果をstringで返す
template<class _Thash>
void hmac<_Thash>::create_digest(const void* data,size_t datalen,const void* key,size_t keylen,std::string &resdigest)
{
	uint8_t digest[_Thash::DIGEST_LENGTH];
	create_digest(data,datalen,key,keylen,digest);
	tostring_digest(digest,_Thash::DIGEST_LENGTH,resdigest);
}

// HMACダイジェストを計算して結果をuint8の配列で返す
template<class _Thash>
void hmac<_Thash>::create_digest(const void* data,size_t datalen,const void* key,size_t keylen,uint8_t *resdigest)
{
	using namespace std;
	int i;
	
	_Thash	hasher;								// ハッシュクラス実体
	uint8_t	ipad[PAD_BLOCK];
	uint8_t	opad[PAD_BLOCK];
	uint8_t tmp_key[_Thash::BLOCK_SIZE];
	uint8_t tmp_digest[_Thash::DIGEST_LENGTH];

	// Keyの初期化
	memset(tmp_key,0, sizeof(tmp_key));

	// 前もって固定値で初期化
	memset(ipad,0, sizeof(uint8_t)*PAD_BLOCK);
	memset(opad,0, sizeof(uint8_t)*PAD_BLOCK);

	// STEP1: 
	// 鍵長がHASH BLOCK Byteよりも長い場合は、まずSHA1を計算する
	// また、HASH BLOCK バイト未満である場合は、途中まではコピーして鍵の終わりまで0で埋めてしまう
	if(keylen > hasher.get_block_size()){
		hasher.update(key,keylen);
		hasher.final(tmp_key);
		keylen = hasher.get_digest_size();			// 前もって0で埋まってるしこれでよい
	}else{
		memcpy(tmp_key,key,keylen);
	}
	
	memcpy(ipad,tmp_key,keylen);
	memcpy(opad,tmp_key,keylen);

	// STEP2: Step1で計算した鍵とipadとのXORをする
	//      : Step1で計算した鍵とopadとのXORをする
	for(i=0;i<PAD_BLOCK;i++){
		ipad[i] ^= 0x36; 
		opad[i] ^= 0x5C; 
	}
	
	// STEP3: Step2で計算したIPADにデータ追加
	hasher.reset();
	hasher.update(ipad,PAD_BLOCK);
	hasher.update(data,datalen);
	
	// STEP4 Step3で生成されたデータにSHA1を適用
	hasher.final(tmp_digest);
	
	// STEP5 Step2でXORしたOPADにSTEP4で生成したハッシュ値を追加
	hasher.reset();
	hasher.update(opad,PAD_BLOCK);
	hasher.update(tmp_digest,sizeof(tmp_digest));

	// STEP6 Step5で生成されたデータにSHA1を適用
	hasher.final(resdigest);
}


// HMAC派生クラス定義

typedef hmac<sha1>			hmac_sha1;
typedef hmac<md5>			hmac_md5;


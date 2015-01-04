//
// MD5 暗号化ハッシュ関数クラス
// RFC1321 を参照のこと
//
// Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
// rights reserved.
// License to copy and use this software is granted provided that it
// is identified as the "RSA Data Security, Inc. MD5 Message-Digest
// Algorithm" in all material mentioning or referencing this software
// or this function.
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
#include <cstring>

#include "byteorder.hpp"
#include "md5.hpp"

using namespace std;



// MD5用変換関数

static inline uint32_t F(uint8_t *p,uint32_t n)
{ return (uint32_t)( p[n+0] | p[n+1]<<8 | p[n+2]<<16 | p[n+3]<<24 );	}
	
static inline uint32_t SHIFT(uint32_t a,uint32_t s)
{ return (a<<s|a>>(32-s));	}


static inline void FF(uint32_t &a,uint32_t b,uint32_t c,uint32_t d,uint32_t x,uint32_t t,uint32_t s)
{ 
	a += x+t+((b&c)|(~b&d));
	a = b+SHIFT(a,s);
}

static inline void GG(uint32_t &a,uint32_t b,uint32_t c,uint32_t d,uint32_t x,uint32_t t,uint32_t s)
{ 
	a += x+t+((b&d)|(c&~d));
	a = b+SHIFT(a,s);
}

static inline void HH(uint32_t &a,uint32_t b,uint32_t c,uint32_t d,uint32_t x,uint32_t t,uint32_t s)
{
	a += x+t+(b^c^d);
	a =  b+SHIFT(a,s);
}

static inline void II(uint32_t &a,uint32_t b,uint32_t c,uint32_t d,uint32_t x,uint32_t t,uint32_t s)
{
	a += x+t+(c^(b|~d));
	a =  b+SHIFT(a,s);
}


md5::md5()
{
	reset();
}

md5::~md5()
{
}


bool md5::reset()
{
	// 最初のハッシュ値をセット
	m_hash[0] = 0x67452301UL;
	m_hash[1] = 0xEFCDAB89UL;
	m_hash[2] = 0x98BADCFEUL;
	m_hash[3] = 0x10325476UL;

	memset(m_blk,0,sizeof(BLOCK_SIZE));
	crypto_hash::reset();
	return true;
}


// ハッシュの計算を行う
void md5::process()
{
	enum{
		// Tn = 4294967296.0 * abs(sin(n))
		T1  = 0xD76AA478UL, T2  = 0xE8C7B756UL, T3  = 0x242070DBUL, T4  = 0xC1BDCEEEUL,
		T5  = 0xF57C0FAFUL, T6  = 0x4787C62AUL, T7  = 0xA8304613UL, T8  = 0xFD469501UL,
		T9  = 0x698098D8UL, T10 = 0x8B44F7AFUL, T11 = 0xFFFF5BB1UL, T12 = 0x895CD7BEUL,
		T13 = 0x6B901122UL, T14 = 0xFD987193UL, T15 = 0xA679438EUL, T16 = 0x49B40821UL,
		
		T17 = 0xF61E2562UL, T18 = 0xC040B340UL, T19 = 0x265E5A51UL, T20 = 0xE9B6C7AAUL,
		T21 = 0xD62F105DUL, T22 = 0x02441453UL, T23 = 0xD8A1E681UL, T24 = 0xE7D3FBC8UL,
		T25 = 0x21E1CDE6UL, T26 = 0xC33707D6UL, T27 = 0xF4D50D87UL, T28 = 0x455A14EDUL,
		T29 = 0xA9E3E905UL, T30 = 0xFCEFA3F8UL, T31 = 0x676F02D9UL, T32 = 0x8D2A4C8AUL,
		
		T33 = 0xFFFA3942UL, T34 = 0x8771F681UL, T35 = 0x6D9D6122UL, T36 = 0xFDE5380CUL,
		T37 = 0xA4BEEA44UL, T38 = 0x4BDECFA9UL, T39 = 0xF6BB4B60UL, T40 = 0xBEBFBC70UL,
		T41 = 0x289B7EC6UL, T42 = 0xEAA127FAUL, T43 = 0xD4EF3085UL, T44 = 0x04881D05UL,
		T45 = 0xD9D4D039UL, T46 = 0xE6DB99E5UL, T47 = 0x1FA27CF8UL, T48 = 0xC4AC5665UL,
		
		T49 = 0xF4292244UL, T50 = 0x432AFF97UL, T51 = 0xAB9423A7UL, T52 = 0xFC93A039UL,
		T53 = 0x655B59C3UL, T54 = 0x8F0CCC92UL, T55 = 0xFFEFF47DUL, T56 = 0x85845DD1UL,
		T57 = 0x6FA87E4FUL, T58 = 0xFE2CE6E0UL, T59 = 0xA3014314UL, T60 = 0x4E0811A1UL,
		T61 = 0xF7537E82UL, T62 = 0xBD3AF235UL, T63 = 0x2AD7D2BBUL, T64 = 0xEB86D391UL
	};
	
	// 一時領域
	uint32_t x0,x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,x14,x15;
	// 現在の値をコピー
	uint32_t a = m_hash[0];
	uint32_t b = m_hash[1];
	uint32_t c = m_hash[2];
	uint32_t d = m_hash[3];

	x0 = F(m_blk,  0<<2);
	x1 = F(m_blk,  1<<2);
	x2 = F(m_blk,  2<<2);
	x3 = F(m_blk,  3<<2);
	x4 = F(m_blk,  4<<2);
	x5 = F(m_blk,  5<<2);
	x6 = F(m_blk,  6<<2);
	x7 = F(m_blk,  7<<2);
	x8 = F(m_blk,  8<<2);
	x9 = F(m_blk,  9<<2);
	x10= F(m_blk, 10<<2);
	x11= F(m_blk, 11<<2);
	x12= F(m_blk, 12<<2);
	x13= F(m_blk, 13<<2);
	x14= F(m_blk, 14<<2);
	x15= F(m_blk, 15<<2);
	
	FF(a,b,c,d, x0,  T1 ,  7); FF(d,a,b,c, x1 , T2 , 12); FF(c,d,a,b, x2 , T3 , 17); FF(b,c,d,a, x3 , T4 , 22);
	FF(a,b,c,d, x4 , T5 ,  7); FF(d,a,b,c, x5 , T6 , 12); FF(c,d,a,b, x6 , T7 , 17); FF(b,c,d,a, x7 , T8 , 22);
	FF(a,b,c,d, x8 , T9 ,  7); FF(d,a,b,c, x9 , T10, 12); FF(c,d,a,b, x10, T11, 17); FF(b,c,d,a, x11, T12, 22);
	FF(a,b,c,d, x12, T13,  7); FF(d,a,b,c, x13, T14, 12); FF(c,d,a,b, x14, T15, 17); FF(b,c,d,a, x15, T16, 22);
	
	GG(a,b,c,d, x1 , T17,  5); GG(d,a,b,c, x6 , T18,  9); GG(c,d,a,b, x11, T19, 14); GG(b,c,d,a, x0 , T20, 20);
	GG(a,b,c,d, x5 , T21,  5); GG(d,a,b,c, x10, T22,  9); GG(c,d,a,b, x15, T23, 14); GG(b,c,d,a, x4 , T24, 20);
	GG(a,b,c,d, x9 , T25,  5); GG(d,a,b,c, x14, T26,  9); GG(c,d,a,b, x3 , T27, 14); GG(b,c,d,a, x8 , T28, 20);
	GG(a,b,c,d, x13, T29,  5); GG(d,a,b,c, x2 , T30,  9); GG(c,d,a,b, x7 , T31, 14); GG(b,c,d,a, x12, T32, 20);
	
	HH(a,b,c,d, x5 , T33,  4); HH(d,a,b,c, x8 , T34, 11); HH(c,d,a,b, x11, T35, 16); HH(b,c,d,a, x14, T36, 23);
	HH(a,b,c,d, x1 , T37,  4); HH(d,a,b,c, x4 , T38, 11); HH(c,d,a,b, x7 , T39, 16); HH(b,c,d,a, x10, T40, 23);
	HH(a,b,c,d, x13, T41,  4); HH(d,a,b,c, x0 , T42, 11); HH(c,d,a,b, x3 , T43, 16); HH(b,c,d,a, x6 , T44, 23);
	HH(a,b,c,d, x9 , T45,  4); HH(d,a,b,c, x12, T46, 11); HH(c,d,a,b, x15, T47, 16); HH(b,c,d,a, x2 , T48, 23);
	
	II(a,b,c,d, x0 , T49,  6); II(d,a,b,c, x7 , T50, 10); II(c,d,a,b, x14, T51, 15); II(b,c,d,a, x5 , T52, 21);
	II(a,b,c,d, x12, T53,  6); II(d,a,b,c, x3 , T54, 10); II(c,d,a,b, x10, T55, 15); II(b,c,d,a, x1 , T56, 21);
	II(a,b,c,d, x8 , T57,  6); II(d,a,b,c, x15, T58, 10); II(c,d,a,b, x6 , T59, 15); II(b,c,d,a, x13, T60, 21);
	II(a,b,c,d, x4 , T61,  6); II(d,a,b,c, x11, T62, 10); II(c,d,a,b, x2 , T63, 15); II(b,c,d,a, x9 , T64, 21);

	// 結果をハッシュ値に足す。blockIndexはクリアする
	m_hash[0] += a;
	m_hash[1] += b;
	m_hash[2] += c;
	m_hash[3] += d;
	m_ix = 0;
}

// ハッシュの結果を出力する。outはDIGEST_LENGTH以上でなければならない
bool md5::final(uint8_t* out)
{
	uint32_t i;	
	uint32_t* dst = reinterpret_cast<uint32_t *>(out);
	
	m_blk[m_ix++] =  0x80; // 終端文字 0x80
	
	// 56byteより大きい場合、64byteまで0x00埋めをして、一度算出
	if(m_ix > 56){
		memset(&m_blk[m_ix], 0, 64-m_ix);
		process();
	}
	// 56byte未満なら56byte(448bit)目まで0で穴埋め
	if(m_ix < 56) memset(&m_blk[m_ix], '\0', 56-m_ix);
	m_ix = 56;
	
	// 最後の64bit(8byte)の処理
	// データ長(読み込んだデータサイズ(bit)を64bitで表現)
	// 上位32bitにデータ長の下位32bit,）
	uint32_t n =  (uint32_t)m_count << 3;
	
	m_blk[m_ix++] = n      & 0xFF;
	m_blk[m_ix++] =(n>> 8) & 0xFF;
	m_blk[m_ix++] =(n>>16) & 0xFF;
	m_blk[m_ix++] = n>>24  & 0xFF;
	
	// 下位32bitにデータ長の上位32bit
	n = m_count >> 29;
	m_blk[m_ix++] = n      & 0xFF;
	m_blk[m_ix++] =(n>> 8) & 0xFF;
	m_blk[m_ix++] =(n>>16) & 0xFF;
	m_blk[m_ix++] = n>>24  & 0xFF;
	
	// 最終計算
	process();
	
	// ダイジェスト値を入れる。
	for(i=0;i<DIGEST_LENGTH/4;i++){
		dst[i] = m_hash[i];
	}
	
	// Wipeする
	reset();
	return true;
}






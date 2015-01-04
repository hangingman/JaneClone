//
// SHA-1 暗号化ハッシュ関数クラス
// FIPS PUB 180-1、US Secure Hash Algorithm 1 (SHA1)、RFC3174 を参照のこと
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
#include <cstring>

#include "byteorder.hpp"
#include "sha1.hpp"

using namespace std;

#define roundcommon()	\
	tmp = left_rotate32(a, 5) + f + e + k + w;	\
	e = d;	\
	d = c;	\
	c = left_rotate32(b,30);	\
	b = a;	\
	a = tmp;


// SHA1用変換関数

// w=0-19用
static inline void round20(uint32_t &a,uint32_t &b,uint32_t &c,uint32_t &d,uint32_t &e,uint32_t w)
{
	uint32_t f;
	uint32_t k;
	uint32_t tmp;
	
	f = (b & c) | (~b & d);
	k = 0x5A827999UL;
	roundcommon();
}

// w=20-39用
static inline void round40(uint32_t &a,uint32_t &b,uint32_t &c,uint32_t &d,uint32_t &e,uint32_t w)
{
	uint32_t f;
	uint32_t k;
	uint32_t tmp;
	
	f =  b ^ c ^ d;
	k = 0x6ED9EBA1UL;
	roundcommon();
}

// w=40-59用
static inline void round60(uint32_t &a,uint32_t &b,uint32_t &c,uint32_t &d,uint32_t &e,uint32_t w)
{
	uint32_t f;
	uint32_t k;
	uint32_t tmp;
	
	f =  (b & c) | (b & d) | (c & d);
	k = 0x8F1BBCDCUL;
	roundcommon();
}

// w=60-79用
static inline void round80(uint32_t &a,uint32_t &b,uint32_t &c,uint32_t &d,uint32_t &e,uint32_t w)
{
	uint32_t f;
	uint32_t k;
	uint32_t tmp;
	
	f = b ^ c ^ d;
	k = 0xCA62C1D6UL;
	roundcommon();
}


sha1::sha1()
{
	reset();
}

sha1::~sha1()
{
}


bool sha1::reset()
{
	// 最初のハッシュ値をセット
	m_hash[0] = 0x67452301UL;
	m_hash[1] = 0xefcdab89UL;
	m_hash[2] = 0x98badcfeUL;
	m_hash[3] = 0x10325476UL;
	m_hash[4] = 0xc3d2e1f0UL;

	memset(m_blk,0,sizeof(BLOCK_SIZE));
	crypto_hash::reset();
	return true;
}

// ハッシュの計算を行う
void sha1::process()
{
	uint32_t w[80];
	size_t i;
	for(i=0;i<16;i++){
		w[i] =	(m_blk[i * 4    ] << 24);
		w[i] |=	(m_blk[i * 4 + 1] << 16);
		w[i] |=	(m_blk[i * 4 + 2] << 8);
		w[i] |=	(m_blk[i * 4 + 3]     );
	}
	for(i=16;i<80;i++){
		w[i] =left_rotate32((w[i-3] ^ w[i-8] ^ w[i-14] ^ w[i-16]), 1);
	}
	
	// initialize hash value for this chunk
	uint32_t a = m_hash[0];
	uint32_t b = m_hash[1];
	uint32_t c = m_hash[2];
	uint32_t d = m_hash[3];
	uint32_t e = m_hash[4];

	// 計算と入れ替えを行う (ループを展開している)
	round20(a,b,c,d,e,w[0]);round20(a,b,c,d,e,w[1]);round20(a,b,c,d,e,w[2]);round20(a,b,c,d,e,w[3]);round20(a,b,c,d,e,w[4]);
	round20(a,b,c,d,e,w[5]);round20(a,b,c,d,e,w[6]);round20(a,b,c,d,e,w[7]);round20(a,b,c,d,e,w[8]);round20(a,b,c,d,e,w[9]);
	round20(a,b,c,d,e,w[10]);round20(a,b,c,d,e,w[11]);round20(a,b,c,d,e,w[12]);round20(a,b,c,d,e,w[13]);round20(a,b,c,d,e,w[14]);
	round20(a,b,c,d,e,w[15]);round20(a,b,c,d,e,w[16]);round20(a,b,c,d,e,w[17]);round20(a,b,c,d,e,w[18]);round20(a,b,c,d,e,w[19]);

	round40(a,b,c,d,e,w[20]);round40(a,b,c,d,e,w[21]);round40(a,b,c,d,e,w[22]);round40(a,b,c,d,e,w[23]);round40(a,b,c,d,e,w[24]);
	round40(a,b,c,d,e,w[25]);round40(a,b,c,d,e,w[26]);round40(a,b,c,d,e,w[27]);round40(a,b,c,d,e,w[28]);round40(a,b,c,d,e,w[29]);
	round40(a,b,c,d,e,w[30]);round40(a,b,c,d,e,w[31]);round40(a,b,c,d,e,w[32]);round40(a,b,c,d,e,w[33]);round40(a,b,c,d,e,w[34]);
	round40(a,b,c,d,e,w[35]);round40(a,b,c,d,e,w[36]);round40(a,b,c,d,e,w[37]);round40(a,b,c,d,e,w[38]);round40(a,b,c,d,e,w[39]);

	round60(a,b,c,d,e,w[40]);round60(a,b,c,d,e,w[41]);round60(a,b,c,d,e,w[42]);round60(a,b,c,d,e,w[43]);round60(a,b,c,d,e,w[44]);
	round60(a,b,c,d,e,w[45]);round60(a,b,c,d,e,w[46]);round60(a,b,c,d,e,w[47]);round60(a,b,c,d,e,w[48]);round60(a,b,c,d,e,w[49]);
	round60(a,b,c,d,e,w[50]);round60(a,b,c,d,e,w[51]);round60(a,b,c,d,e,w[52]);round60(a,b,c,d,e,w[53]);round60(a,b,c,d,e,w[54]);
	round60(a,b,c,d,e,w[55]);round60(a,b,c,d,e,w[56]);round60(a,b,c,d,e,w[57]);round60(a,b,c,d,e,w[58]);round60(a,b,c,d,e,w[59]);

	round80(a,b,c,d,e,w[60]);round80(a,b,c,d,e,w[61]);round80(a,b,c,d,e,w[62]);round80(a,b,c,d,e,w[63]);round80(a,b,c,d,e,w[64]);
	round80(a,b,c,d,e,w[65]);round80(a,b,c,d,e,w[66]);round80(a,b,c,d,e,w[67]);round80(a,b,c,d,e,w[68]);round80(a,b,c,d,e,w[69]);
	round80(a,b,c,d,e,w[70]);round80(a,b,c,d,e,w[71]);round80(a,b,c,d,e,w[72]);round80(a,b,c,d,e,w[73]);round80(a,b,c,d,e,w[74]);
	round80(a,b,c,d,e,w[75]);round80(a,b,c,d,e,w[76]);round80(a,b,c,d,e,w[77]);round80(a,b,c,d,e,w[78]);round80(a,b,c,d,e,w[79]);

	// 結果をハッシュ値に足す。blockIndexはクリアする
	m_hash[0] += a;
	m_hash[1] += b;
	m_hash[2] += c;
	m_hash[3] += d;
	m_hash[4] += e;
	m_ix = 0;
}

// ハッシュの結果を出力する。outはDIGEST_LENGTH以上でなければならない
bool sha1::final(uint8_t* out)
{
	static const uint8_t padding[BLOCK_SIZE] = {0x80, };
	uint64_t bit_count = m_count << 3;				// *8することでビット数になおす
	uint64_t big_bit_count;
	uint32_t padlen,index,i;
	uint32_t* big_dst = reinterpret_cast<uint32_t *>(out);
	
	if(m_corrupted) return false;
	// ビット数カウントはbig-endianでないといけない
	big_bit_count = htobe64(bit_count);
	// Paddingを埋める
	index = m_count & 0x3f;
	padlen = (index < 56) ? (56 - index) : ((BLOCK_SIZE+56) - index);
	update(padding,padlen);
	// 最終の8Byteはビットカウント数で埋める
	update(&big_bit_count,sizeof(uint64_t));
	
	// ダイジェスト値を入れる。big-endianで直して入れる
	for(i=0;i<DIGEST_LENGTH/4;i++){
		big_dst[i] = htobe32(m_hash[i]);
	}
	// Wipeする
	reset();
	return true;
}


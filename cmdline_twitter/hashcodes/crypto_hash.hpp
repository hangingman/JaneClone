#pragma once

// 暗号化ハッシュ関数クラスの親定義
// これを派生して個々のハッシュ関数クラスを定義すること
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



#include <string>
#include "typedef.hpp"


class crypto_hash
{
protected:
	bool		m_corrupted;				// 壊れフラグ
	uint64_t	m_count;					// 総データ長(Byte数)
	size_t		m_ix	;					// Block用Index
	
protected:
	virtual void process()=0;
	virtual uint8_t* get_blk()=0;
	virtual uint8_t* get_digest_buffer()=0;
	
public:
	
	virtual bool reset();
	virtual bool update(const void* data,std::size_t len);
	virtual bool final(uint8_t* out)=0;
	virtual bool final(std::string &ostr);
	
	virtual size_t get_digest_size()=0;
	virtual size_t get_block_size()=0;
	
	virtual const std::string get_name()=0;
	
	crypto_hash(){
		reset();
	};
	virtual ~crypto_hash(){};
};


void tostring_digest(uint8_t* out,size_t size,std::string &dst);


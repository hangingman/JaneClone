//
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

#include "crypto_hash.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>

// uint8型の配列を文字列に変換する
// ダイジェストの変換に利用される
//
// out:  uint8型の配列
// size: outの配列長
// dst:  結果をセットする文字列変数
void tostring_digest(uint8_t* out,size_t size,std::string &dst)
{
	using namespace std;
	ostringstream stream;
	size_t i;

	dst.empty();
	for(i=0;i<size;i++){
		stream << setw(2) << setfill('0') << hex << uppercase << static_cast<int>(out[i]);
	}
	dst = stream.str();
}


bool crypto_hash::reset()
{
	m_ix = 0;
	m_corrupted = false;
	m_count = 0;
	return true;
}

// ハッシュ関数にデータを追加する
bool crypto_hash::update(const void *data,size_t len)
{
	uint8_t const *dt = static_cast<uint8_t const*>(data);
	uint8_t *blk = get_blk();
	size_t maxblk = get_block_size();

	if(m_corrupted)		return false;
	
	for(size_t i=0;i<len;i++){
		blk[m_ix++] = dt[i] & 0xFF;
		m_count ++;
		if(m_count == 0){
			// oh my god!!!!
			m_corrupted = true;
			return false;
		}
		// ブロックサイズに達したら計算
		if(m_ix == maxblk){
			process();
		}
	}
	return true;
}

// ハッシュの結果を文字列型で出力する。
bool crypto_hash::final(std::string &ostr)
{
	using namespace std;
	
	uint8_t *digest = get_digest_buffer();
	bool ret = final(digest);

	if(! ret) return false;
	tostring_digest(digest,get_digest_size(),ostr);
	return ret;
}


#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <cstdio>
#include "hmac.hpp"

// チェックして文字列化して出力
bool put_check(std::string digest,std::string check)
{
	using namespace std;

	cout << digest << endl;
	if(check != digest){
		cout << "CheckFAIL" << endl;
		return false;
	}
	return true;
}

// SHA-1のテスト RFCに書いてある値でテストする
bool sha1_test()
{
	using namespace std;

	sha1 hasher;

	size_t i;
	string test1("abc");
	string test2("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq");
	string test3("a");
	string test4("0123456701234567012345670123456701234567012345670123456701234567");
	// digest values
	string chk1("A9993E364706816ABA3E25717850C26C9CD0D89D");
	string chk2("84983E441C3BD26EBAAE4AA1F95129E5E54670F1");
	string chk3("34AA973CD4C4DAA4F61EEB2BDBAD27316534016F");
	string chk4("DEA356A2CDDD90C7A7ECEDC5EBB563934F460452");
	
	string digest;
	hasher.update(test1.c_str(),test1.length());
	if(! hasher.final(digest))				return false;
	if(! put_check(digest,chk1))			return false;;
	
	
	hasher.reset();
	hasher.update(test2.c_str(),test2.length());
	if(! hasher.final(digest))				return false;
	if(! put_check(digest,chk2))			return false;;

	hasher.reset();
	for(i=0;i<1000000;i++){
		hasher.update(test3.c_str(),test3.length());
	}
	if(! hasher.final(digest))				return false;
	if(! put_check(digest,chk3))			return false;;

	hasher.reset();
	for(i=0;i<10;i++){
		hasher.update(test4.c_str(),test4.length());
	}
	if(! hasher.final(digest))				return false;
	if(! put_check(digest,chk4))			return false;;
		
	// OK
	return true;
}

// MD5のテスト RFCに書いてある値でテストする
bool md5_test()
{
	using namespace std;

	md5 hasher;
	string digest;

	string test1("");
	string test2("a");
	string test3("abc");
	string test4("message digest");
	string test5("abcdefghijklmnopqrstuvwxyz");
	string test6("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
	string test7("12345678901234567890123456789012345678901234567890123456789012345678901234567890");
	// digest values
	string chk1("D41D8CD98F00B204E9800998ECF8427E");
	string chk2("0CC175B9C0F1B6A831C399E269772661");
	string chk3("900150983CD24FB0D6963F7D28E17F72");
	string chk4("F96B697D7CB7938D525A2F31AAF161D0");
	string chk5("C3FCD3D76192E4007DFB496CCA67E13B");
	string chk6("D174AB98D277D9F5A5611C2C9F419D9F");
	string chk7("57EDF4A22BE3C955AC49DA2E2107B67A");

	hasher.update("",0);
	if(! hasher.final(digest))				return false;
	if(! put_check(digest,chk1))			return false;;

	hasher.reset();
	hasher.update(test2.c_str(),test2.length());
	if(! hasher.final(digest))				return false;
	if(! put_check(digest,chk2))			return false;;
	
	hasher.reset();
	hasher.update(test3.c_str(),test3.length());
	if(! hasher.final(digest))				return false;
	if(! put_check(digest,chk3))			return false;;

	hasher.reset();
	hasher.update(test4.c_str(),test4.length());
	if(! hasher.final(digest))				return false;
	if(! put_check(digest,chk4))			return false;;
		
	hasher.reset();
	hasher.update(test5.c_str(),test5.length());
	if(! hasher.final(digest))				return false;
	if(! put_check(digest,chk5))			return false;;

	hasher.reset();
	hasher.update(test6.c_str(),test6.length());
	if(! hasher.final(digest))				return false;
	if(! put_check(digest,chk6))			return false;;

	hasher.reset();
	hasher.update(test7.c_str(),test7.length());
	if(! hasher.final(digest))				return false;
	if(! put_check(digest,chk7))			return false;;
	// OK
	return true;
}


// HMAC-SHA1のテスト RFCに書いてある値でテストする
bool hmac_sha1_test()
{
	using namespace std;
	
	uint8_t key[80];
	uint8_t data[80];
	string digest;
	
	// digest values
	string chk1("B617318655057264E28BC0B6FB378C8EF146BE00");
	string chk2("EFFCDF6AE5EB2FA2D27416D5F184DF9C259A7C79");
	string chk3("125D7342B9AC11CD91A39AF48AA17B4F63F175D3");
	string chk4("4C9007F4026250C6BC8414F9BF50C86C2D7235DA");
	string chk5("4C1A03424B55E07FE7F27BE1D58BB9324A9A5A04");
	string chk6("AA4AE5E15272D00E95705637CE8A3B55ED402112");
	string chk7("E8E99D0F45237D786D6BBAA7965C7808BBFF1A91");
	
	
	// 1
	memset(key,0x0B,sizeof(key));	
	strcpy((char*)data,"Hi There");
	
	hmac_sha1::create_digest(data,8,key,20,digest);
	if(! put_check(digest,chk1))			return false;;
	
	// 2
	strcpy((char*)key,"Jefe");
	strcpy((char*)data,"what do ya want for nothing?");
	
	hmac_sha1::create_digest(data,28,key,4,digest);
	if(! put_check(digest,chk2))			return false;;

	// 3
	memset(key,0xAA,sizeof(key));
	memset(data,0xDD,sizeof(data));
	
	hmac_sha1::create_digest(data,50,key,20,digest);	
	if(! put_check(digest,chk3))			return false;;
	
	// 4
	for(int i=0;i<25;i++){
		key[i] = i+1;
	}
	memset(data,0xCD,sizeof(data));
	
	hmac_sha1::create_digest(data,50,key,25,digest);	
	if(! put_check(digest,chk4))			return false;;
	
	// 5
	memset(key,0x0C,sizeof(key));
	strcpy((char*)data,"Test With Truncation");
	
	hmac_sha1::create_digest(data,20,key,20,digest);	
	if(! put_check(digest,chk5))			return false;;
	
	// 6
	memset(key,0xAA,sizeof(key));
	strcpy((char*)data,"Test Using Larger Than Block-Size Key - Hash Key First");
	
	hmac_sha1::create_digest(data,54,key,80,digest);	
	if(! put_check(digest,chk6))			return false;;
	
	// 7
	memset(key,0xAA,sizeof(key));
	strcpy((char*)data,"Test Using Larger Than Block-Size Key and Larger Than One Block-Size Datat");
	
	hmac_sha1::create_digest(data,73,key,80,digest);	
	if(! put_check(digest,chk7))			return false;;
	
	return true;
}

// HMAC-MD5のテスト RFCに書いてある値でテストする
bool hmac_md5_test()
{
	using namespace std;
	
	uint8_t key[80];
	uint8_t data[80];
	string digest;
	
	// digest values
	string chk1("9294727A3638BB1C13F48EF8158BFC9D");
	string chk2("750C783E6AB0B503EAA86E310A5DB738");
	string chk3("56BE34521D144C88DBB8C733F0E8B3F6");
	string chk4("697EAF0ACA3A3AEA3A75164746FFAA79");
	string chk5("56461EF2342EDC00F9BAB995690EFD4C");
	string chk6("6B1AB7FE4BD7BF8F0B62E6CE61B9D0CD");
	string chk7("6F630FAD67CDA0EE1FB1F562DB3AA53E");
	
	
	// 1
	memset(key,0x0B,sizeof(key));	
	strcpy((char*)data,"Hi There");
	
	hmac_md5::create_digest(data,8,key,16,digest);
	if(! put_check(digest,chk1))			return false;;
	
	// 2
	strcpy((char*)key,"Jefe");
	strcpy((char*)data,"what do ya want for nothing?");
	
	hmac_md5::create_digest(data,28,key,4,digest);
	if(! put_check(digest,chk2))			return false;;

	// 3
	memset(key,0xAA,sizeof(key));
	memset(data,0xDD,sizeof(data));
	
	hmac_md5::create_digest(data,50,key,16,digest);	
	if(! put_check(digest,chk3))			return false;;
	
	// 4
	for(int i=0;i<25;i++){
		key[i] = i+1;
	}
	memset(data,0xCD,sizeof(data));
	
	hmac_md5::create_digest(data,50,key,25,digest);	
	if(! put_check(digest,chk4))			return false;;
	
	// 5
	memset(key,0x0C,sizeof(key));
	strcpy((char*)data,"Test With Truncation");
	
	hmac_md5::create_digest(data,20,key,16,digest);	
	if(! put_check(digest,chk5))			return false;;
	
	// 6
	memset(key,0xAA,sizeof(key));
	strcpy((char*)data,"Test Using Larger Than Block-Size Key - Hash Key First");
	
	hmac_md5::create_digest(data,54,key,80,digest);	
	if(! put_check(digest,chk6))			return false;;
	
	// 7
	memset(key,0xAA,sizeof(key));
	strcpy((char*)data,"Test Using Larger Than Block-Size Key and Larger Than One Block-Size Datat");
	
	hmac_md5::create_digest(data,73,key,80,digest);	
	if(! put_check(digest,chk7))			return false;;
	
	return true;
}



int main(void)
{
	using namespace std;
	cout << "Test SHA1 Cryptograph HASH function" << endl;
	if(! sha1_test()){
		cout << "Test FAIL! SHA1" << endl;
		return -1;
	}
	cout << "Test Success SHA1" << endl;
	
	cout << "test HMAC-SHA1" << endl;
	if(! hmac_sha1_test()){
		cout << "Test FAIL! HMAC-SHA1" << endl;
		return -1;
	}
	cout << "Test Success HMAC-SHA1" << endl;
	
	
	cout << "Test MD5 Cryptograph HASH function" << endl;
	if(! md5_test()){
		cout << "Test FAIL! MD5" << endl;
		return -1;
	}
	cout << "Test Success MD5" << endl;
	
	cout << "test HMAC-MD5" << endl;
	if(! hmac_md5_test()){
		cout << "Test FAIL! HMAC-MD5" << endl;
		return -1;
	}
	cout << "Test Success HMAC-MD5" << endl;
	
	return 0;
}





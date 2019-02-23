#ifndef __CC_AES_ZIP_H__
#define __CC_AES_ZIP_H__

#include <stdio.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include "cocos2d.h"

NS_CC_BEGIN

class AesZipUtils : public Ref
{
public:
	static AesZipUtils* getInstance();
	void initKV(const char* tmpK, const char* tmpIV);
	std::string aesEncrypt(const char* srcString, int len);	
	std::string aesDecrypt(const char* srcString, int len);

	unsigned char* ccInflate(const unsigned char* zipstring, int zipLen, int* inflateLen);
	unsigned char* ccCompress(const unsigned char* str, int strLen, int* zipLen);

	char* ccInflateChar(const char* zipstring, int zipLen, int* inflateLen);
	char* ccCompressChar(const char* str, int strLen, int* zipLen);

	char* json2netstring(const char* json);
	char* netstring2json(const char* netstring);

	//std::string base64Encrypt(const unsigned char* buf);	
	//unsigned char* base64Decrypt(const std::string& base64String);

	void releaseBuf(void* buf);
	void releaseBufChar(void* buf);

private:
	char* _inflateBuf;
	char* _base64Buf;
	AesZipUtils();
	~AesZipUtils();
	static AesZipUtils* _instance;
};

NS_CC_END
#endif

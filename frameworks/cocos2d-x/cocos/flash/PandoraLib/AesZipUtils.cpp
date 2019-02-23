
#include "AesZipUtils.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC )
    #include "AesBridge.h"
#else
    #include "cryptopp/aes.h"
    #include "cryptopp/filters.h"
    #include "cryptopp/modes.h"
#endif

#include "base/ZipUtils.h"
#include "zlib.h"
#include "base/base64.h"

using namespace std;
using namespace cocos2d;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC )

#else
using namespace CryptoPP;

#endif
const int kKeyLength = 16;
const int kBufSize = 2048*1024;
const int kBase64BufSize = 1024 * 1024;
char key[kKeyLength], iv[kKeyLength];


AesZipUtils* AesZipUtils::_instance = NULL;


AesZipUtils* AesZipUtils::getInstance()
{	
	if( _instance == 0)
	{
		_instance = new AesZipUtils();
	}
	return _instance;
}

AesZipUtils::AesZipUtils()
{
	initKV(NULL, NULL);
	_inflateBuf = new char[kBufSize];
	_base64Buf = new char[kBase64BufSize];
}

AesZipUtils::~AesZipUtils()
{
	delete _inflateBuf;
	delete _base64Buf;
}

void AesZipUtils::initKV(const char* tmpK, const char* tmpIV)
{
    memset( key, 0x00, kKeyLength );
    memset( iv, 0x00, kKeyLength );

	if(tmpK && tmpIV)
	{
		for (int j = 0; j < kKeyLength; ++j)
		{
			key[j] = tmpK[j];
		}

		for (int i = 0; i < kKeyLength; ++i)
		{
			iv[i] = tmpIV[i];
		}
	}else{
		char tmpK[] = "U1xsasdfYyxs;lkj";
		char tmpIV[] = "1234567890654321";
		for (int j = 0; j < kKeyLength; ++j)
		{
			key[j] = tmpK[j];
		}

		for (int i = 0; i < kKeyLength; ++i)
		{
			iv[i] = tmpIV[i];
		}

	}
}

string AesZipUtils::aesEncrypt(const char* srcString, int len)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC )
    return AesBridge::getInstance()->encryptBridge(srcString, len, key, iv);
#else
    string cipherText;
    CryptoPP::AES::Encryption aesEncryption((unsigned char*)key, CryptoPP::AES::DEFAULT_KEYLENGTH);
    CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption( aesEncryption, (unsigned char*)iv );
    CryptoPP::StreamTransformationFilter stfEncryptor(cbcEncryption, new CryptoPP::StringSink( cipherText ));
    stfEncryptor.Put( (const unsigned char*)srcString, len );
    stfEncryptor.MessageEnd();
    return cipherText;
#endif
}


string AesZipUtils::aesDecrypt(const char* srcString, int len)
{
   
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC )
    return AesBridge::getInstance()->decryptBridge(srcString, len, key, iv);
#else
    string cipherText;
    string decryptedText;
    CryptoPP::AES::Decryption aesDecryption((unsigned char*)key, CryptoPP::AES::DEFAULT_KEYLENGTH);
    CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption( aesDecryption, (unsigned char*)iv );
    CryptoPP::StreamTransformationFilter stfDecryptor(cbcDecryption, new CryptoPP::StringSink( decryptedText ));
    stfDecryptor.Put( (const unsigned char*)srcString, len);
    stfDecryptor.MessageEnd();
    return decryptedText;
#endif

}

char* AesZipUtils::json2netstring(const char* json)
{
	int zipLen = 0;
	char* compressBuf = ccCompressChar(json, (int)strlen(json), &zipLen);
	string codeBuf = aesEncrypt(compressBuf, zipLen);
	delete compressBuf;
	//char* base64Buf = NULL;
	memset(_base64Buf, 0, kBase64BufSize);
	base64EncodeEx((const unsigned char*)codeBuf.c_str(), (int)codeBuf.size(), _base64Buf);
	//int stringLen = strlen(base64Buf);
	//assert(stringLen == base64Len);
	return _base64Buf;
}
	
char* AesZipUtils::netstring2json(const char* netstring)
{
	unsigned char* codeBuf = NULL;
	size_t netStringLen = strlen(netstring);
	int codeBufLen = base64Decode((const unsigned char*)netstring, (int)strlen(netstring), &codeBuf);
	string mingwen = aesDecrypt((const char*)codeBuf, codeBufLen);
	delete codeBuf;
	int inflateLen = 0;
	char* inflateBuf = ccInflateChar(mingwen.c_str(), (int)mingwen.size(), &inflateLen);
	size_t strle = strlen(inflateBuf);
	assert(strlen(inflateBuf) == inflateLen);
	return inflateBuf;
}

char* AesZipUtils::ccInflateChar(const char* zipstring, int zipLen, int* inflateLen)
{
	return (char*)ccInflate((const unsigned char*)zipstring, zipLen, inflateLen);
}

char* AesZipUtils::ccCompressChar(const char* str, int strLen, int* zipLen)
{
	return (char*)ccCompress((const unsigned char*)str, strLen, zipLen);
}

unsigned char* AesZipUtils::ccInflate(const unsigned char* zipstring, int zipLen, int* inflateLen)
{
	//const int kMaxBuf = 512 * 1024;
	//unsigned char* buf = new unsigned char[kMaxBuf];
	//memset(buf, 0, kMaxBuf);
	memset(_inflateBuf, 0, kBufSize);
	//unsigned long bufLen = kMaxBuf;
	unsigned long bufLen = kBufSize;
	uncompress((unsigned char*)_inflateBuf, &bufLen, zipstring, zipLen);
	*inflateLen = (int)bufLen;
	return (unsigned char*)_inflateBuf;
}
	
unsigned char* AesZipUtils::ccCompress(const unsigned char* str, int strLen, int* zipLen)
{
	const int kMaxBuf = 1024 * 1024;
	unsigned char* buf = new unsigned char[kMaxBuf];
	memset(buf, 0, kMaxBuf);
	unsigned long bufLen = kMaxBuf;
	compress(buf, &bufLen, str, strLen);
	*zipLen = (int)bufLen;
	return buf;
}

void AesZipUtils::releaseBuf(void* buf)
{
	delete (char*)buf;
}

void AesZipUtils::releaseBufChar(void* buf)
{
	delete (char*)buf;
}

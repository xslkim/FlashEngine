
#include "AesBridge.h"
#import <CommonCrypto/CommonCryptor.h>


AesBridge* AesBridge::_instance = NULL;


AesBridge* AesBridge::getInstance()
{	
	if( _instance == 0)
	{
		_instance = new AesBridge();
	}
	return _instance;
}


std::string AesBridge::encryptBridge(const char* srcString, int len, const char* key, const char* iv)
{
    //NSData* data = [NSData dataWithBytes:srcString length:len];
    
    //NSUInteger dataLength = [data length];
    size_t bufferSize = len + kCCBlockSizeAES128;
    void *buffer = malloc(bufferSize);
    size_t numBytesEncrypted = 0;
    CCCryptorStatus cryptStatus = CCCrypt(kCCEncrypt,
                                          kCCAlgorithmAES128,
                                          kCCOptionPKCS7Padding | 0,
                                          key,
                                          kCCBlockSizeAES128,
                                          iv,
                                          srcString,
                                          len,
                                          buffer,
                                          bufferSize,
                                          &numBytesEncrypted);
    assert(cryptStatus == kCCSuccess);
    
    std::string str((char*)buffer, numBytesEncrypted);
    free(buffer);
    return str;
}



std::string AesBridge::decryptBridge(const char* srcString, int len, const char* key, const char* iv)
{
    
    size_t bufferSize = len + kCCBlockSizeAES128;
    void *buffer = malloc(bufferSize);
    size_t numBytesDecrypted = 0;
    CCCryptorStatus cryptStatus = CCCrypt(kCCDecrypt,
                                          kCCAlgorithmAES128,
                                          kCCOptionPKCS7Padding | 0,
                                          key,
                                          kCCBlockSizeAES128,
                                          iv,
                                          srcString,
                                          len,
                                          buffer,
                                          bufferSize,
                                          &numBytesDecrypted);
    
    assert(cryptStatus == kCCSuccess);
    std::string str((char*)buffer, numBytesDecrypted);
    free(buffer);
    return str;
}


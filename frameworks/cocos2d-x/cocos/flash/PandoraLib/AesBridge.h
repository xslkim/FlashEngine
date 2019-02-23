#ifndef __CC_AES_BRIDGE_H__
#define __CC_AES_BRIDGE_H__

#include <string>

class AesBridge
{
public:
	static AesBridge* getInstance();
	std::string encryptBridge(const char* srcString, int len, const char* key, const char* iv);
	std::string decryptBridge(const char* srcString, int len, const char* key, const char* iv);
private:
    static AesBridge* _instance;
};

#endif

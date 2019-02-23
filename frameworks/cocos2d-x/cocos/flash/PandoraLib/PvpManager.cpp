#include "PvpManager.h"
#include "md5.h"
#include <fstream>

#if(CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)

#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <arpa/inet.h>
#include <errno.h>

#define ETH_NAME "eth0"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC )
#include "UIVideoPlayer.h"
#include "IPAddress.h"
#endif

using namespace cocos2d;
using namespace network;


PvpManager* PvpManager::_instance = 0;


PvpManager* PvpManager::getInstance()
{	
	if( _instance == 0)
	{
		_instance = new PvpManager();
	}
	return _instance;
}

PvpManager::PvpManager()
{
	_startBattleHandler = 0;
	_handler = 0;
    _handler1 = 0;
    _handler2 = 0;

}



std::string PvpManager::getBattleData()
{
	return "";
}



void PvpManager::startCompute(const char* url)
{
}



void PvpManager::unregisterStartBattleHandler()
{
	if (_startBattleHandler)
	{
		ScriptEngineManager::getInstance()->getScriptEngine()->removeScriptHandler(_startBattleHandler);
		_startBattleHandler = 0;
	}
}

void PvpManager::registerStartBattleHandler(int handler)
{
	unregisterStartBattleHandler();
	_startBattleHandler = handler;
}


void PvpManager::setComputeReady()
{
    
}


std::string PvpManager::bakFunction(const char* str)
{
	char c;
	while(1)
	{
		c=*str++;
		if (c==0) return "no";  
		if (c&0x80)        
			if (*str & 0x80) return "yes";
	}
	return "no";
}

void PvpManager::registerHandler(int handler)
{
	unregisterHandler();
	_handler = handler;
}

void PvpManager::unregisterHandler()
{
	if (_handler)
	{
		ScriptEngineManager::getInstance()->getScriptEngine()->removeScriptHandler(_handler);
		_handler = 0;
	}
}

void PvpManager::registerHandler1(int handler)
{
    unregisterHandler1();
    _handler1 = handler;
}

void PvpManager::unregisterHandler1()
{
    if (_handler1)
    {
        ScriptEngineManager::getInstance()->getScriptEngine()->removeScriptHandler(_handler1);
        _handler1 = 0;
    }
}

void PvpManager::registerHandler2(int handler)
{
    unregisterHandler2();
    _handler2 = handler;
}

void PvpManager::unregisterHandler2()
{
    if (_handler2)
    {
        ScriptEngineManager::getInstance()->getScriptEngine()->removeScriptHandler(_handler2);
        _handler2 = 0;
    }
}

std::string PvpManager::bakFunction1(const char* str)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	//char ipadd[128] = {0};
	WSADATA wsaData;
	int ret=WSAStartup(MAKEWORD(2,2),&wsaData);
	if (ret!=0)
	{
		return "";
	}
	char hostname[256];
	ret=gethostname(hostname,sizeof(hostname));
	if (ret==SOCKET_ERROR)
	{
		return "";
	}
	HOSTENT* host=gethostbyname(hostname);
	if (host==NULL)
	{
		return "";
	}

	//4.转化为char*并拷贝返回
	//strcpy(ipadd,);
	std::string ipadd = inet_ntoa(*(in_addr*)*host->h_addr_list);
	return ipadd;
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
	std::string ipInfo = "";
	char hname[128];
	struct hostent *hent;
	int i;

	gethostname(hname, sizeof(hname));


	//hent = gethostent();
	hent = gethostbyname(hname);

	ipInfo += "hname:";
	ipInfo += hname;
	ipInfo +=  "  ";
	//printf("hostname: %s/naddress list: ", hent->h_name);

	//ipInfo += "hent";
	//ipInfo +=  hent->h_name;
	//ipInfo +=  "  ";

	//for(i = 0; hent->h_addr_list[i]; i++) {
	//	char buf[256] = {0};
	//	sprintf(buf, "%s", inet_ntoa(*(struct in_addr*)(hent->h_addr_list[i])));
	//	ipInfo += "ip:";
	//	ipInfo += buf;
	//}

	int sock;
	struct sockaddr_in sin;
	struct ifreq ifr;

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == -1)
	{
		perror("socket");
		return "not get";
	}

	strncpy(ifr.ifr_name, ETH_NAME, IFNAMSIZ);
	ifr.ifr_name[IFNAMSIZ - 1] = 0;

	if (ioctl(sock, SIOCGIFADDR, &ifr) < 0)
	{
		perror("ioctl");
		return "not get";
	}

	memcpy(&sin, &ifr.ifr_addr, sizeof(sin));
	char buf[256] = {0};
	sprintf(buf, "eth0: %s", inet_ntoa(sin.sin_addr));
	ipInfo += buf;
	return ipInfo;
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
	InitAddresses();
	GetIPAddresses();
	GetHWAddresses();

	int i;

	for (i=0; i<MAXADDRS; ++i)
	{
		static unsigned long localHost = 0x7F000001;            // 127.0.0.1
		unsigned long theAddr;

		theAddr = ip_addrs[i];

		if (theAddr == 0) break;
		if (theAddr == localHost) continue;

		std::string ipAdd = if_names[i];
		ipAdd += hw_addrs[i];
		ipAdd += ip_names[i];
		return ipAdd;
		//NSLog(@"Name: %s MAC: %s IP: %s\n", if_names[i], hw_addrs[i], ip_names[i]);
	}

	return "";
#endif
}

std::string PvpManager::bakFunction2(const char* str)
{
	MD5 md5;
	md5.update(str, strlen(str));
	md5.finalize();
    return md5.md5();
}

std::string PvpManager::bakFunction3(const char* str)
{
	//临时用于，设置Http请求的超时时间
	if(str != nullptr){
		int temp = atoi(str);
		int connectOutTime = temp / 100;
		int readOutTime = temp % 100;
		if (connectOutTime > 0 && connectOutTime <= 50){
			HttpClient::getInstance()->setTimeoutForConnect(connectOutTime);
		}
		
		if(readOutTime > 0 && readOutTime <= 50){
			HttpClient::getInstance()->setTimeoutForRead(readOutTime);
		}
	}
    return "";
}


std::string PvpManager::getComputerData()
{
    return "";
}

int PvpManager::getManagerState()
{
    return 0;
}

void PvpManager::setComputerResult(const char* result)
{
    
}

char* PvpManager::backupFun(Node* arg)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

    experimental::ui::VideoPlayer* videoPlayer = (experimental::ui::VideoPlayer*)arg;
    videoPlayer->addEventListener([&](Ref* ref, experimental::ui::VideoPlayer::EventType type){
		if(_handler){
            if( type == experimental::ui::VideoPlayer::EventType::COMPLETED || type == experimental::ui::VideoPlayer::EventType::PAUSED || type == experimental::ui::VideoPlayer::EventType::STOPPED){
				CommonScriptData data(_handler, "finish", this);
                ScriptEvent event(kCommonEvent,(void*)&data);
                ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&event);
            }
		}
        
    });
#endif

    return 0;
}

char* PvpManager::backupFun1(Node* arg)
{
	//make crash
	int* t = NULL;
	*t = 1;
    return 0;
}

char* PvpManager::backupFunction(Node* arg, const char* str)
{
    return 0;
}

char* PvpManager::backupFunction1(Node* arg, const char* str)
{
    return 0;
}

void PvpManager::talkingDataResumeSetAccount()
{
	if (_handler1)
	{
		CommonScriptData data(_handler1, "resume", this);
		ScriptEvent event(kCommonEvent,(void*)&data);
		ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&event);
	}

}








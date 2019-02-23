#include "DynamicUpdateMgr.h"
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
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
#include "IPAddress.h"
#endif
using namespace cocos2d;
using namespace network;

const int kNoticeDownloadSize = 512 * 1024;

static const int sDownloadNone = 0;
static const int sDownloading = 1;
static const int sDownloadFailure = 2;
static const int sDownloadNotEnoughSpace = 3;
static const int sDownloadSuccess = 4;


static std::string cdnResUrl = "http://cdn.icefire.xyz:8080/icefire/";
// static std::string cdnResUrl = "http://icefire.xyz:8080/icefire/";

static std::string newServerListMd5;

DynamicUpdateMgr* DynamicUpdateMgr::_instance = 0;


DynamicUpdateMgr* DynamicUpdateMgr::getInstance()
{	
	if( _instance == 0)
	{
		_instance = new DynamicUpdateMgr();
	}
	return _instance;
}

DynamicUpdateMgr::DynamicUpdateMgr()
{
	_totalUpdateSize = 0;
	_curUpdateSize = 0;
	_updateProcessHandler = 0;
	_eventBackHandler = 0;
	_handler = 0;
	_curState = 0;
	_curDownloadIndex = 0;
	_downloadRetryTime = 1;
	_downloadState = sDownloadNone;
	_curDownloadTag = 1;
	_changeBigCardTime = 0;
	_downloadFileTime = 0;


#if (MD5_RESOURCE == 1)

	std::string fileListMd5 = getOriginalFileListMd5();
	std::string path = cocos2d::FileUtils::getInstance()->getWritablePath() + fileListMd5;
	if( !cocos2d::FileUtils::getInstance()->isFileExistInternal(path) )
	{
        std::string originalPath = cocos2d::FileUtils::getInstance()->fullPathForFilename(fileListMd5);
        Data listFileData = CCFileUtils::getInstance()->getDataFromFile(originalPath.c_str());
        CC_ASSERT(listFileData.getSize() > 1024 * 10);
		std::string savePath = FileUtils::getInstance()->getWritablePath()  + fileListMd5;
		std::ofstream outfile(savePath.c_str(), std::ios::binary);
		CC_ASSERT(outfile);
		outfile.write((char*)listFileData.getBytes(), listFileData.getSize());
		outfile.flush();
		outfile.close();
		CCUserDefault::getInstance()->setStringForKey("lastFileListMd5", fileListMd5.c_str());
	}
	else
	{
		std::string saveFileListMd5 = CCUserDefault::getInstance()->getStringForKey("lastFileListMd5");
		if(saveFileListMd5.length() == 32)
		{
			fileListMd5 = saveFileListMd5;
		}
	}
	CCFileUtils::getInstance()->loadFileListMap(fileListMd5);
#endif
}


void DynamicUpdateMgr::downloadFile(std::string url)
{
	_url = url;
	HttpRequest* request = new  HttpRequest();
	request->setUrl(url.c_str());
	request->setRequestType(HttpRequest::Type::GET);
	request->setResponseCallback(this, httpresponse_selector(DynamicUpdateMgr::onDownloadFileBack));
	char buf[32] = {0};
	sprintf(buf, "%d", _curDownloadTag);
	request->setTag(buf);
	HttpClient::getInstance()->send(request);
	request->release();
	cocos2d::log("DynamicUpdate send download Request url:%s", url.c_str());
}


//void DynamicUpdateMgr::handleDownloadFailue()
//{
//	
//}
//
//void DynamicUpdateMgr::downloadFileDelay(float dt)
//{
//	downloadFileByIndex(_curDownloadIndex);
//	CCDirector::getInstance()->getScheduler()->unschedule(schedule_selector(DynamicUpdateMgr::downloadFileDelay), this);
//}


void DynamicUpdateMgr::onDownloadFileBack(network::HttpClient *sender, network::HttpResponse *response)
{
#if (MD5_RESOURCE == 1)
	cocos2d::log("DynamicUpdate onDownloadFileBack  _url:%s", _url.c_str());
    if (!response )
	{
		cocos2d::log("DynamicUpdate  !response");
		return;
	}
	HttpRequest* request = response->getHttpRequest();
	const char* tag = request->getTag();
	int downloadTag = atoi(tag);
	if(downloadTag != _curDownloadTag){
		cocos2d::log("DynamicUpdate  downloadTag != _curDownloadTag");
		return;
	}


	if( !response->isSucceed()) {
		//handleDownloadFailue();
		_downloadState = sDownloadFailure;
		cocos2d::log("DynamicUpdate  !response->isSucceed()");
        return;
    }
	size_t pos = _url.rfind("/");
	std::string name = _url.substr(pos + 1, _url.size());
	std::string path = FileUtils::getInstance()->getWritablePath() + name;
	std::ofstream outfile(path.c_str(), std::ios::binary);
	if(!outfile)
	{
		//CommonScriptData data(_eventBackHandler, "notEnoughSpace", this);
		//ScriptEvent event(kCommonEvent,(void*)&data);
		//ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&event);
		_downloadState = sDownloadNotEnoughSpace;
		cocos2d::log("DynamicUpdate  !outfile");
		return;
	}
	size_t fileSize = response->getResponseData()->size();
	std::vector<char>* buffer = response->getResponseData();
	outfile.write((const char*)(&(*buffer)[0]), fileSize);
     
	outfile.flush();
	outfile.close();
	

	bool writeSuccess = true;
	std::ifstream inFile(path.c_str(), std::ios::binary);
	if(!inFile)
	{
		cocos2d::log("DynamicUpdate  !openFileFailure !inFile");
		writeSuccess = false;
	}
	else
	{
		const int kBufferSize = 1024 * 32;
        char buffer[kBufferSize] = {0};
		MD5 md5;
		while( !inFile.eof()){
			inFile.read(buffer, kBufferSize);
			size_t count = inFile.gcount();
			md5.update(buffer, (int)count);
		}
		inFile.close();
		md5.finalize();
		std::string md5Str = md5.md5();
		if(name != md5Str)
		{
			md5Str += ".mp3";
			if(name != md5Str){
				cocos2d::log("DynamicUpdate md5 check failure");
				writeSuccess = false;
			}
		}
	}

	if(!writeSuccess){
		//handleDownloadFailue();
		_downloadState = sDownloadFailure;
	}else{
		_curUpdateSize += fileSize;
		_downloadState = sDownloadSuccess;
	}
#endif
}

void DynamicUpdateMgr::onDirectEnterGame(float dt){
	if (_eventBackHandler)
	{
		CCDirector::getInstance()->getScheduler()->unscheduleSelector(schedule_selector(DynamicUpdateMgr::onDirectEnterGame), this);
		CommonScriptData data(_eventBackHandler, "enterGame", this);
		ScriptEvent event(kCommonEvent,(void*)&data);
		ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&event);
	}
}

void DynamicUpdateMgr::checkUpdate(const char* url)
{
	//CCDirector::getInstance()->getScheduler()->scheduleSelector(schedule_selector(DynamicUpdateMgr::onDirectEnterGame), this, 0.1f, false);
	//return;
	
#if (MD5_RESOURCE == 1)
	std::string md5Url = url;
	bool debug = CCUserDefault::getInstance()->getBoolForKey("debugMode", false);
	if( debug )
	{ 
		md5Url += "_debug";
	}

	HttpRequest* request = new  HttpRequest();
	request->setUrl(md5Url.c_str());
	request->setRequestType(HttpRequest::Type::GET);
	request->setResponseCallback(this, httpresponse_selector(DynamicUpdateMgr::onGetCurListFileMd5Back));
	HttpClient::getInstance()->send(request);
	request->release();
#else
	CCDirector::getInstance()->getScheduler()->scheduleSelector(schedule_selector(DynamicUpdateMgr::onDirectEnterGame), this, 0.5f, false);
#endif
}


void DynamicUpdateMgr::onGetCurListFileMd5Back(network::HttpClient *sender, network::HttpResponse *response)
{
#if (MD5_RESOURCE == 1)
	CC_ASSERT(response);
	if (!response->isSucceed()) {
		log("response failed");
		log("error buffer: %s",response->getErrorBuffer());
        //onDirectEnterGame(0);

		CommonScriptData data(_eventBackHandler, "timeout", this);
		ScriptEvent event(kCommonEvent,(void*)&data);
		ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&event);
		return;
	}
	std::vector<char>* buffer = response->getResponseData();
	char stringBuffer[32 + 1] = {0};
	for(int i = 0; i < 32; ++i){
		stringBuffer[i] = (*buffer)[i];
	}
	//服务器端返回的 FileListMd5
	std::string serverFileListMd5 = stringBuffer;

	//当前客户端版本的 资源代码 md5
	std::string curClientFileListMd5 = getOriginalFileListMd5();
	std::string saveFileListMd5 = CCUserDefault::getInstance()->getStringForKey("lastFileListMd5");
	if(saveFileListMd5.length() == 32)
	{
		//有任何一次动态更新过，则saveFileListMd5 长度为16
		curClientFileListMd5 = saveFileListMd5;
	}
	if(curClientFileListMd5 == serverFileListMd5)
	{
		onDirectEnterGame(0);
	}
	else
	{
		HttpClient::getInstance()->setTimeoutForRead(20);
		HttpRequest* request = new  HttpRequest();
		std::string fileUrl = cdnResUrl;
		fileUrl += serverFileListMd5;
		request->setUrl(fileUrl.c_str());
		request->setRequestType(HttpRequest::Type::GET);
		request->setResponseCallback(this, httpresponse_selector(DynamicUpdateMgr::onGetCurListFileBack));
		request->setTag(serverFileListMd5.c_str());
		HttpClient::getInstance()->send(request);
		request->release();
		newServerListMd5 = serverFileListMd5;
	}

#endif
}

void DynamicUpdateMgr::onGetCurListFileBack(network::HttpClient *sender, network::HttpResponse *response)
{
	HttpClient::getInstance()->setTimeoutForRead(6);
#if (MD5_RESOURCE == 1)
	CC_ASSERT(response);
	if (!response->isSucceed()) {
		CommonScriptData data(_eventBackHandler, "timeout", this);
		ScriptEvent event(kCommonEvent,(void*)&data);
		ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&event);
		cocos2d::log("DynamicUpdate onGetCurListFileBack !response->isSucceed()");
		return;
	}
	std::string serverFileListMd5 = response->getHttpRequest()->getTag();
	
	std::string path = FileUtils::getInstance()->getWritablePath() + serverFileListMd5;

	std::ofstream outfile(path.c_str(), std::ios::binary);
	CC_ASSERT(outfile);

	size_t bufferLen = response->getResponseData()->size();
	std::vector<char>* buffer = response->getResponseData();
	outfile.write((const char*)(&(*buffer)[0]), bufferLen);

	outfile.flush();
	outfile.close();
	//CCUserDefault::getInstance()->setStringForKey("lastFileListMd5", serverFileListMd5);

	std::ifstream infile(path);
	CC_ASSERT(infile);
	std::string line;
	getline(infile, line);
	int count = atoi(line.c_str());
	for(int i = 0; i < count; ++i){
		std::string fileName;
		getline(infile, fileName);
		std::string md5;
		getline(infile, md5);
		std::string count;
		getline(infile, count);
		checkAndAddUpdateFile(fileName, md5, atoi(count.c_str()) );
	}
	_curUpdateSize = 0;
	if (_eventBackHandler)
	{
		if(_totalUpdateSize > kNoticeDownloadSize){
			float updateSizeM = (float)_totalUpdateSize / (float)(1024 * 1024);
			char updateSizeString[64] = {0};
			sprintf(updateSizeString, "%1.2f", updateSizeM);
			CommonScriptData data(_eventBackHandler, updateSizeString, this);
			ScriptEvent event(kCommonEvent,(void*)&data);
			ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&event);
		}
		else if(_totalUpdateSize <= 0)
		{
			CCUserDefault::getInstance()->setStringForKey("lastFileListMd5", newServerListMd5.c_str());
			CCFileUtils::getInstance()->loadFileListMap(newServerListMd5);
			onDirectEnterGame(0);
		}
		else
		{
			CommonScriptData data(_eventBackHandler, "invisibleDownload", this);
			ScriptEvent event(kCommonEvent,(void*)&data);
			ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&event);
		}
		
	}
#endif
}

void DynamicUpdateMgr::checkAndAddUpdateFile(std::string fileName, std::string md5, int size )
{
#if (MD5_RESOURCE == 1)
	bool fileExist = false;
	const std::unordered_map<std::string, std::string>& fileMap = CCFileUtils::getInstance()->getMd5FileMap();
	auto it = fileMap.find(fileName);
	if(it != fileMap.end()){
		std::string curMd5 = it->second;
		if(curMd5 == md5){
			fileExist = true;
		}
	}

	if( !fileExist ){
		std::string filePath = CCFileUtils::getInstance()->getWritablePath() + md5;
		if( CCFileUtils::getInstance()->isFileExistInternal(filePath)){
			fileExist = true;
		}
	}

	if( !fileExist ){
		std::string filePath = CCFileUtils::getInstance()->getFullPathForDirectoryAndFilename("md5resource/",  md5);
		if( CCFileUtils::getInstance()->isFileExistInternal(filePath)){
			fileExist = true;
		}
	}

	if(! fileExist){
		_updateResourceVector.push_back(md5);
		_totalUpdateSize += size;
	}
#endif
}


std::string DynamicUpdateMgr::getOriginalFileListMd5()
{
	std::string codeOriginalFileListMd5 = "c8c866cb9ef78828af8fae08f0fe132d";
	return codeOriginalFileListMd5; 
}





void DynamicUpdateMgr::downloadFileByIndex(int index){
	_downloadState = sDownloading;
	_downloadFileTime = 0;
	std::string url = cdnResUrl + "md5resource/" + _updateResourceVector[_curDownloadIndex];
	downloadFile(url);
}

void DynamicUpdateMgr::startDownload()
{
    HttpClient::getInstance()->setTimeoutForConnect(10);
    HttpClient::getInstance()->setTimeoutForRead(25);
	_curDownloadIndex = 0;
	_downloadRetryTime = 0;
	_changeBigCardTime = 0;
	downloadFileByIndex(_curDownloadIndex);
	CCDirector::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);
}

void DynamicUpdateMgr::update(float dt)
{
	#if (MD5_RESOURCE == 1)
	if(_downloadState == sDownloadNone){
		//do nothing
	}else if(_downloadState == sDownloading){
		_downloadFileTime += dt;
		//to do handle download time out

		_changeBigCardTime += dt;
		if(_changeBigCardTime > 6.f){
			CommonScriptData data(_eventBackHandler, "changeBigCard", this);
			ScriptEvent event(kCommonEvent,(void*)&data);
			ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&event);
			_changeBigCardTime = 0;
		}	

	}else if(_downloadState == sDownloadFailure){
		cocos2d::log("DynamicUpdate handleDownloadFailue _downloadRetryTime:%d", _downloadRetryTime);
		if(_downloadRetryTime > 1){
			CommonScriptData data(_eventBackHandler, "downloadTimeout", this);
			ScriptEvent event(kCommonEvent,(void*)&data);
			ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&event);
			_downloadState = sDownloadNone;
		}else{
			_downloadRetryTime += 1;
			downloadFileByIndex(_curDownloadIndex);
			//CCDirector::getInstance()->getScheduler()->scheduleSelector(schedule_selector(DynamicUpdateMgr::downloadFileDelay), this, 0.1f, false);
		}

	}else if(_downloadState == sDownloadSuccess){
		_curDownloadIndex += 1;
		if(_curDownloadIndex >= _updateResourceVector.size()){
			CCUserDefault::getInstance()->setStringForKey("lastFileListMd5", newServerListMd5.c_str());
			CCFileUtils::getInstance()->loadFileListMap(newServerListMd5);
			CommonScriptData data(_eventBackHandler, "enterGame", this);
			ScriptEvent event(kCommonEvent,(void*)&data);
			ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&event);
			HttpClient::getInstance()->setTimeoutForConnect(5);
			HttpClient::getInstance()->setTimeoutForRead(6);
			_downloadState = sDownloadNone;
			CCDirector::getInstance()->getScheduler()->unscheduleUpdate(this);
		}else{
			if( _updateProcessHandler ){
				int updateRate = (int)((float)_curUpdateSize * 100 / (float)_totalUpdateSize);
				char updateRateString[64] = {0};
				sprintf(updateRateString, "%d", updateRate);
				CommonScriptData data(_updateProcessHandler, updateRateString, this);
				ScriptEvent event(kCommonEvent,(void*)&data);
				ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&event);
			}
			_downloadRetryTime = 0;
			downloadFileByIndex(_curDownloadIndex);
		}
	}
#endif
}

void DynamicUpdateMgr::continueDownload()
{
	_downloadRetryTime = 0;
	downloadFileByIndex(_curDownloadIndex);
}

std::string DynamicUpdateMgr::bakFunction(const char* str)
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
#else
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


void DynamicUpdateMgr::unregisterEventBackHandler()
{
	if(_eventBackHandler)
	{
		ScriptEngineManager::getInstance()->getScriptEngine()->removeScriptHandler(_eventBackHandler);
		_eventBackHandler = 0;
	}
}


void DynamicUpdateMgr::registerEventBackHandler(int handler)
{
	unregisterEventBackHandler();
	_eventBackHandler = handler;
}



void DynamicUpdateMgr::registerUpdateProcessHandler(int handler)
{
	unregisterUpdateProcessHandler();
	_updateProcessHandler = handler;
}

void DynamicUpdateMgr::unregisterUpdateProcessHandler()
{
	if (_updateProcessHandler)
	{
		ScriptEngineManager::getInstance()->getScriptEngine()->removeScriptHandler(_updateProcessHandler);
		_updateProcessHandler = 0;
	}
}


void DynamicUpdateMgr::registerHandler(int handler)
{
	unregisterHandler();
	_handler = handler;
}

void DynamicUpdateMgr::unregisterHandler()
{
	if (_handler)
	{
		ScriptEngineManager::getInstance()->getScriptEngine()->removeScriptHandler(_handler);
		_handler = 0;
	}
}
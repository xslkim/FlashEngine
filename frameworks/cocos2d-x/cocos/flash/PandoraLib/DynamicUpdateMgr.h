#ifndef  __DynamicUpdateMgr__H__
#define  __DynamicUpdateMgr__H__

#include "cocos2d.h"
#include <map>
#include "network/HttpClient.h"

NS_CC_BEGIN

class DynamicUpdateMgr : public Ref
{
public:
	static DynamicUpdateMgr* getInstance();
	void startDownload();
	void continueDownload();
	void checkUpdate(const char* url);
	void registerEventBackHandler(int handler);
	void unregisterEventBackHandler();

	void registerUpdateProcessHandler(int handler); 
	void unregisterUpdateProcessHandler(); 

	std::string bakFunction(const char* str);
	void registerHandler(int handler); 
	void unregisterHandler(); 

	void update(float delta);

private:
	std::string getOriginalFileListMd5();
	void downloadFile(std::string url);
	void onDownloadFileBack(network::HttpClient *sender, network::HttpResponse *response);
	void onGetCurListFileMd5Back(network::HttpClient *sender, network::HttpResponse *response);
	void onGetCurListFileBack(network::HttpClient *sender, network::HttpResponse *response);
	void prepareFileListMap();
	//void handleDownloadFailue();
	//void downloadFileDelay(float);


	DynamicUpdateMgr();
	static DynamicUpdateMgr* _instance;
	
	int _eventBackHandler;
	int _updateProcessHandler;

	int _handler;
	int _curState;
	std::string _url;
	void onDirectEnterGame(float dt);
	void checkAndAddUpdateFile(std::string fileName, std::string md5, int size );
	std::vector<std::string> _updateResourceVector;
	std::string _curUpdateServerFileListMd5;
	int _totalUpdateSize;
	int _curUpdateSize;

	void downloadFileByIndex(int index);
	int _curDownloadIndex;
	int _downloadRetryTime;
	int _downloadState;
	int _curDownloadTag;
	float _downloadFileTime;
	float _changeBigCardTime;

};
NS_CC_END
#endif
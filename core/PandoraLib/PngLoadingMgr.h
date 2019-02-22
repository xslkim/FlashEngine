#ifndef  __PNG_LOADING_MGR_H__
#define  __PNG_LOADING_MGR_H__

#include "cocos2d.h"
#include <map>
using namespace cocos2d;

class PngLoadingMgr :public CCObject
{
public:
	static PngLoadingMgr* getInstance();
	void registerEndLoadingScriptHandler(int handler);
	void unregisterEndLoadingScriptHandler();
	void loadingCallBack(CCObject* obj);
	bool loadPng(const char* name);
private:
	PngLoadingMgr();
	static PngLoadingMgr* _instance;
	int m_nScriptHandler;
	bool _isLoading;
	std::string _curLoadingName;
};

#endif
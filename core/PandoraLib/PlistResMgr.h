#ifndef  __PLIST_RES_MGR_H__
#define  __PLIST_RES_MGR_H__

#include "cocos2d.h"
#include <map>

int lua_write_atom_file(const char* filepath, const char* data);

class PlistResMgr
{
public:
	static PlistResMgr* getInstance();
	cocos2d::CCTexture2D* getTexture(const std::string key);
	void releasePlist(const std::string key);
	void loadPlist(const std::string key);
	std::string getCCNodeClassName(cocos2d::CCNode * node);
	void onSystemBackKeyClick();
	void registerSystemBackHandler(int nHandler);
	void unregisterSystemBackHandler();
	void tryToCloseActivity(std::string message, std::string confirm , std::string cancel);
	void terminateProcess();

	void setEnableCacheTableView(bool value);
	bool getEnableCacheTableView() ;

private:
	PlistResMgr();
	static PlistResMgr* _instance;

	std::map<std::string, cocos2d::CCTexture2D*> _plistDictMap;
};

#endif
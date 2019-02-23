#ifndef  __PLIST_RES_MGR_H__
#define  __PLIST_RES_MGR_H__

#include "cocos2d.h"
#include <map>

NS_CC_BEGIN

class PlistResMgr : public Ref
{
public:
	static PlistResMgr* getInstance();
	cocos2d::CCTexture2D* getTexture(const std::string key);
	void releasePlist(const std::string key);
	void loadPlist(const std::string key);
	//std::string getCCNodeClassName(cocos2d::CCNode * node);
	//void onSystemBackKeyClick();
	//void registerSystemBackHandler(int nHandler);
	//void unregisterSystemBackHandler();

private:
	PlistResMgr();
	static PlistResMgr* _instance;

	std::map<std::string, cocos2d::CCTexture2D*> _plistDictMap;
};
NS_CC_END
#endif
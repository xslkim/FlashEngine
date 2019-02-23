#ifndef  __FlashCacheMgr_MGR_H_
#define  __FlashCacheMgr_MGR_H_

#include "cocos2d.h"
//#include "PandoraConfig.h"
//#include "flashMeta.pb.h"
#include "FlashDefine.h"

#include <vector>
#include <unordered_map>



class SwfSpriteBatchNode;
class FlashSpritePb;
class FlashMetaEx;
class FlashMeta;
class FlashMetaSymbol;
class FlashMetaData;
class FlashMetaFrame;

NS_CC_BEGIN


class CCSwfSprite;

#if (defined USE_FLASH_METADATA_CACHE && USE_FLASH_METADATA_CACHE == 1)

class FlashCacheMgr : public Ref
{
public:
	static FlashCacheMgr* getInstance();
	FlashDataItem* getItemBySimpeName(const std::string& name);
	FlashDataItem* getItem(std::string pathKey);
	FlashDataItem* getItemAndHold(std::string pathKey);
	void releateItem(std::string pathKey, bool forceRelease = false);
	//std::string getFlashTextureName(std::string pathKey);
	FlashDataItem* createItem(std::string pathKey);
	static std::map<std::string, std::string> g_flashCache;


private:
	std::string getPngName(const char* pathKey, const char* resName);
	std::string getPlistName(const char* pathKey, const char* resName);
	FlashMetaEx* loadSpritePbData(const char* name, std::string& tranName);

	static FlashCacheMgr* _instance;
	std::map<std::string, FlashDataItem*>_itemMap;
	int addFlashMetaData(const FlashMetaFrame&);

};

#else

class FlashCacheMgr : public Ref
{
public:
	static FlashCacheMgr* getInstance();
	FlashCacheMgr();
	FlashDataItem* getItemBySimpeName(const std::string& name);
	FlashDataItem* getItem(std::string pathKey);
	FlashDataItem* getItemAndHold(std::string pathKey);
	void releateItem(std::string pathKey, bool forceRelease = false);
	//std::string getFlashTextureName(std::string pathKey);
	FlashDataItem* createItem(std::string pathKey);
	void removeUnusedItems();
	void setAutoRemoveUnusedSize(int size);

	static std::map<std::string, std::string> g_flashCache;

private:
	std::string getPngName(const char* pathKey, const char* resName);
	std::string getPlistName(const char* pathKey, const char* resName);
	FlashMeta* loadSpritePbData(const char* name, std::string& tranName);
	
	static FlashCacheMgr* _instance;
	std::map<std::string, FlashDataItem*>_itemMap;
	std::unordered_map<std::string, std::string> _PlistNameMap;
	int _autoRemoveUnusedSize;

};
#endif

NS_CC_END
#endif
#ifndef  __FlashCacheMgr_MGR_H_
#define  __FlashCacheMgr_MGR_H_

#include "cocos2d.h"
//#include "PandoraConfig.h"
#include "flashMeta.pb.h"
#include <vector>


class SwfSpriteBatchNode;
class FlashSpritePb;
class CCSwfSprite;
struct FlashDataItem
{
	FlashDataItem()
	{
		itemReference = 1; 
		pbData = NULL;
	}
	FlashMeta* pbData;
	std::string _transName;
	
	void retain()
	{
		itemReference++; 
	}

	void release()
	{
		itemReference--; 
	}
	unsigned int getReference(){ return itemReference; }
private:
	unsigned int itemReference;
};

const FlashMetaSymbol& getFlashMetaSymbol(const std::string name, const FlashMeta* meta);
std::string getNameFromPathAndName(std::string pathAndName);

class FlashSpritePb;
class FlashMeta;
class FlashCacheMgr
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
	FlashMeta* loadSpritePbData(const char* name, std::string& tranName);
	
	static FlashCacheMgr* _instance;
	std::map<std::string, FlashDataItem*>_itemMap;
};


#endif
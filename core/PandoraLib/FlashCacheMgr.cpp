#include "FlashCacheMgr.h"
#include "flashMeta.pb.h"
#include "CCSwfSprite.h"
#include "cocos2d.h"
#include "PlistResMgr.h"
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include "FlashUtils.h"

using namespace cocos2d;

FlashCacheMgr* FlashCacheMgr::_instance = 0;
std::map<std::string, std::string> FlashCacheMgr::g_flashCache;

FlashCacheMgr* FlashCacheMgr::getInstance()
{	
	if( _instance == 0)
	{
		_instance = new FlashCacheMgr();
	}
	return _instance;
}

FlashDataItem* FlashCacheMgr::getItemBySimpeName(const std::string& name)
{
	std::map<std::string, FlashDataItem*>::iterator it = _itemMap.find(name);
	if( it != _itemMap.end())
	{
		return (*it).second;
	}
	return NULL;
}

FlashDataItem* FlashCacheMgr::getItem(std::string pathKey)
{
	char flashNameBuf[256] = {0};
	toFileName(flashNameBuf, pathKey.c_str() );
	std::string flashName = flashNameBuf;
	std::map<std::string, FlashDataItem*>::iterator it = _itemMap.find(flashName);
	if( it != _itemMap.end())
	{
		return (*it).second;
	}
	return NULL;
}
 
FlashDataItem* FlashCacheMgr::getItemAndHold(std::string pathKey)
{
	FlashDataItem* item = NULL;
	char flashNameBuf[256] = {0};
	toFileName(flashNameBuf, pathKey.c_str() );
	std::string flashName = flashNameBuf;
	std::map<std::string, FlashDataItem*>::iterator it = _itemMap.find(flashName);
	if( it != _itemMap.end())
	{
		item = (*it).second;
		item->retain();
	}
	return item;
}

std::string FlashCacheMgr::getPngName(const char* pathKey, const char* resName)
{
	char pathBuf[256] = {0};
	toPath(pathBuf, pathKey);
	std::string plistName = pathBuf;
	plistName += resName;
	plistName += ".png";
	return plistName;
}

std::string FlashCacheMgr::getPlistName(const char* pathKey, const char* resName)
{
	char pathBuf[256] = {0};
	toPath(pathBuf, pathKey);
	std::string plistName = pathBuf;
	plistName += resName;
	plistName += ".plist";
	return plistName;
}

void FlashCacheMgr::releateItem(std::string pathKey, bool forceRelease)
{
	char flashNameBuf[256] = {0};
	toFileName(flashNameBuf, pathKey.c_str() );
	std::string flashName = flashNameBuf;
	std::map<std::string, FlashDataItem*>::iterator it = _itemMap.find(flashName);
	if( it != _itemMap.end())
	{
		FlashDataItem* item = (*it).second;
		item->release();
		if(item->getReference() > 0 && !forceRelease)
		{
			return;
		}
		std::string plistName = getPlistName(pathKey.c_str(), item->pbData->spritename().c_str());
        //std::string plistNameEx = getPlistName( getFlashFileName(pathKey.c_str()), item->pbData->spritename().c_str());
		CC_SAFE_DELETE(item->pbData);

        
		if(PlistResMgr::getInstance()->getTexture(plistName) == NULL)
		{
            //if(PlistResMgr::getInstance()->getTexture(plistNameEx) != NULL)
            //{
            //    PlistResMgr::getInstance()->releasePlist(plistNameEx);
            //}
            //else 
            {
                CCAssert(false, "");
            }
		}
        else 
        {
			//plistName = transformLinkPath(plistName);
            PlistResMgr::getInstance()->releasePlist(plistName);
        }


		CC_SAFE_DELETE(item);
		_itemMap.erase(it);
		return;
	}
	//CCAssert(false, "");
}

//static std::string g_tempPathKey; 


//std::string FlashCacheMgr::getFlashTextureName(std::string pathKey)
//{
//	g_tempPathKey = pathKey;
//	g_tempPbData = loadSpritePbData(pathKey.c_str());
//	std::string pngName = getPngName(pathKey.c_str(), g_tempPbData->spritename().c_str() );
//	return pngName;
//}

FlashDataItem* FlashCacheMgr::createItem(std::string pathKey)
{
	FlashDataItem* item = getItem(pathKey);
	if( item != NULL )
	{
		return item;
	}

	item = new FlashDataItem();

	//if( g_tempPathKey == pathKey)
	//{
	//	CCAssert( g_tempPbData != NULL, "");
	//	item->pbData = g_tempPbData;
	//}
	//else
	//{

	//	FlashMeta* pbData = loadSpritePbData(pathKey.c_str());
	//	item->pbData = pbData;	
	//}

	//if(pathKey == "UI/shop/shop")
	//{
	//	int qwer = 234;
	//}

	FlashMeta* pbData = loadSpritePbData(pathKey.c_str(), item->_transName);
	item->pbData = pbData;	

	std::string name = getPlistName(pathKey.c_str(), item->pbData->spritename().c_str() );
	g_flashCache[pathKey] = name;
	if( PlistResMgr::getInstance()->getTexture(name) == NULL)
	{
		PlistResMgr::getInstance()->loadPlist(name);
	}

	char flashNameBuf[256] = {0};
	toFileName(flashNameBuf, pathKey.c_str() );
	std::string flashName = flashNameBuf;


	_itemMap.insert(std::map<std::string, FlashDataItem*>::value_type(flashName, item) );
	return item;
}

	std::string getNameFromPathAndName(std::string pathAndName)
	{
		int len = pathAndName.size() - 1;
		for(;;)
		{
			if(pathAndName[len] == '\\' || pathAndName[len] == '/')
			{
				break;
			}
			len -= 1;
			if(len < 0 )
			{
				break;
			}
		}
		char buf[256] = {0};
		strcpy(buf,  pathAndName.c_str() + len + 1);
		return buf;
	}

FlashMeta* FlashCacheMgr::loadSpritePbData(const char* name, std::string& tranName)
{
	std::string fpkName = name;
	fpkName += ".fpk";
	std::string fullPathAndName = CCFileUtils::sharedFileUtils()->fullPathForFilename(fpkName.c_str());
	tranName = fpkName.substr(0, fpkName.find_last_of("."));
    unsigned long nSize;
    unsigned char* pBuffer = CCFileUtils::sharedFileUtils()->getFileData(fullPathAndName.c_str(), "rb", &nSize);
	FlashMeta* pbData = new FlashMeta();
	//if (!pbData->ParseFromIstream(&in_file))
    if( !pbData->ParseFromArray(pBuffer, nSize) )
	{
		CCLog("%s %d", "can not read pbfile with file size:", nSize);
        CCAssert(false, "");
		// TODO:: error log
		return NULL;
	}
    CC_SAFE_DELETE(pBuffer);
	return pbData;
}


const FlashMetaSymbol& getFlashMetaSymbol(const std::string name, const FlashMeta* meta)
{
	for(int i = 0; i < meta->symbols_size(); ++i)
	{
		const FlashMetaSymbol& symbol = meta->symbols(i);
		if(symbol.name() == name)
		{
			return symbol;
		}
	}

	std::string xmlName = name + ".xml";
	for(int i = 0; i < meta->symbols_size(); ++i)
	{
		const FlashMetaSymbol& symbol = meta->symbols(i);
		if(symbol.name() == xmlName)
		{
			return symbol;
		}
	}

	cocos2d::CCLog("wwf Error getFlashMetaSymbol name%s", name.c_str());

	CCAssert(false, "");
	return meta->symbols(0);
}









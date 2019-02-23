#include "FlashUtils.h"
#include "cocos2d.h"
#include "flashMeta.pb.h"
#include "flashMetaEx.pb.h"
#include <fstream>

void toFileName(char* buf, const char* pathAndName)
{
	assert( buf && pathAndName);
	int len = strlen(pathAndName);
	while(len > 0)
	{
		len--;
		if(pathAndName[len] == '/' || pathAndName[len] == '\\')
		{
			strcpy(buf, pathAndName + len + 1);
			return;
		}
	}
	strcpy(buf, pathAndName);
}

void toPath(char* buf, const char* pathAndName)
{
	assert( buf && pathAndName);
	int len = strlen(pathAndName);
	while(len > 0)
	{
		len--;
		if(pathAndName[len] == '/' || pathAndName[len] == '\\')
		{
			break;
		}
	}

	if( len == 0 )
	{
		buf[0] =0;
	}
	else 
	{
		memcpy(buf, pathAndName, len+1);
	}

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

#if (defined USE_FLASH_METADATA_CACHE && USE_FLASH_METADATA_CACHE == 1)
const FlashMetaSymbolEx& getFlashMetaSymbol(const std::string name, const FlashMetaEx* meta)
{
	for(int i = 0; i < meta->symbols_size(); ++i)
	{
		const FlashMetaSymbolEx& symbol = meta->symbols(i);
		if(symbol.name() == name)
		{
			return symbol;
		}
	}

	std::string xmlName = name + ".xml";
	for(int i = 0; i < meta->symbols_size(); ++i)
	{
		const FlashMetaSymbolEx& symbol = meta->symbols(i);
		if(symbol.name() == xmlName)
		{
			return symbol;
		}
	}

	cocos2d::CCLog("wwf Error getFlashMetaSymbol name%s", name.c_str());

	CCAssert(false, "");
	return meta->symbols(0);
}
#else
const FlashMetaSymbol& getFlashMetaSymbol(const std::string name, const FlashMeta* meta)
{
	const int symbolSize = meta->symbols_size();
	const FlashMetaSymbol* tmp;
	char *xmlName = (char*)malloc(sizeof(char)*name.size()+5);
	strcpy(xmlName,name.c_str());
	strcat(xmlName,".xml");

	for(int i = 0; i < symbolSize; ++i)
	{
		tmp = &(meta->symbols(i));
		if (strcmp(tmp->name().c_str(),xmlName)==0)
		{
			free(xmlName);
			return *tmp;
		}
	}

	free(xmlName);

	for (int i = 0; i < symbolSize; ++i)
	{
		tmp = &(meta->symbols(i));
		if (strcmp(tmp->name().c_str(),name.c_str())==0)
		{
			return *tmp;
		}
	}
	
	cocos2d::CCLog("wwf Error getFlashMetaSymbol name%s", name.c_str());

	CCAssert(false, "");
	return meta->symbols(0);
}

//const FlashMetaSymbol& getFlashMetaSymbol(const std::string name, const FlashMeta* meta)
//{
//	for(int i = 0; i < meta->symbols_size(); ++i)
//	{
//		const FlashMetaSymbol& symbol = meta->symbols(i);
//		if(symbol.name() == name)
//		{
//			return symbol;
//		}
//	}
//
//	std::string xmlName = name + ".xml";
//	for(int i = 0; i < meta->symbols_size(); ++i)
//	{
//		const FlashMetaSymbol& symbol = meta->symbols(i);
//		if(symbol.name() == xmlName)
//		{
//			return symbol;
//		}
//	}
//
//	cocos2d::CCLog("wwf Error getFlashMetaSymbol name%s", name.c_str());
//
//	CCAssert(false, "");
//	return meta->symbols(0);
//}

#endif

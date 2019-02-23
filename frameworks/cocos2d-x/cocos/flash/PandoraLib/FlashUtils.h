#ifndef __FLASH_UTILS_H__
#define __FLASH_UTILS_H__

#include <string>
#include "FlashDefine.h"

class FlashMeta;
class FlashMetaSymbol;
class FlashMetaEx;
class FlashMetaSymbolEx;


#if (defined USE_FLASH_METADATA_CACHE && USE_FLASH_METADATA_CACHE == 1)
struct FlashDataItem
{
	FlashDataItem()
	{
		itemReference = 1;
		pbData = NULL;
	}
	FlashMetaEx* pbData;
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

const FlashMetaSymbolEx& getFlashMetaSymbol(const std::string name, const FlashMetaEx* meta);
#else
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
#endif


std::string getNameFromPathAndName(std::string pathAndName);

void toFileName(char* buf, const char* pathAndName);
void toPath(char* buf, const char* pathAndName);

inline int	iabs(int i) { if (i < 0) return -i; else return i; }
inline int	imax(int a, int b) { if (a < b) return b; else return a; }
inline int	imin(int a, int b) { if (a < b) return a; else return b; }

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
//inline float	fmin(float a, float b) { if (a < b) return a; else return b; }
//inline float	fmax(float a, float b) { if (a < b) return b; else return a; }
#endif

#endif
#include "FlashUtils.h"
#include "cocos2d.h"
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
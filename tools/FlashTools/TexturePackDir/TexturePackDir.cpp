// TexturePackDir.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <vector>
#include <direct.h>

void findFile(const char* path, std::vector<std::string>& pathAndNameList, std::vector<std::string>& pngNameList)
{
	char szFind[MAX_PATH];
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	char szFile[MAX_PATH];

	strcpy(szFind,path);
	strcat(szFind,"\\*.*");

	hFind=FindFirstFile(szFind,&FindFileData);
	if(INVALID_HANDLE_VALUE == hFind)    return;



	while(TRUE)
	{

		if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			std::string curPath = path;
			curPath += "/";
			curPath += FindFileData.cFileName;
			int len = curPath.size();
			if( curPath[len - 1] != '.')
			{
				findFile(curPath.c_str(), pathAndNameList, pngNameList);
			}
		}
		else
		{
			bool isPng = false;
			std::string fileName = FindFileData.cFileName;
			if(fileName.size() > 3)
			{
				char buf[256] = {0};
				memcpy( buf, fileName.c_str() + (fileName.size() - 3), 3);
				std::string exName = buf;
				if(exName == "png")
				{
					isPng = true;
				}
			}

			if( isPng )
			{
				std::string curPath = path;
				curPath += "/";
				curPath += FindFileData.cFileName;
				pathAndNameList.push_back(curPath.c_str());
				pngNameList.push_back(FindFileData.cFileName);
			}
		}
		if(!FindNextFile(hFind,&FindFileData))
			break;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	std::string srcPath = argv[1];
	std::string targetPath = argv[2];
	std::vector<std::string> pathAndNameList;
	std::vector<std::string> pngNameList;
	findFile(srcPath.c_str(), pathAndNameList, pngNameList);
	for(int i = 0; i < pathAndNameList.size(); ++i)
	{
		std::string srcPathAndName = pathAndNameList[i];
		std::string destPathAndName = targetPath + '/' + pngNameList[i];
		char cmdBuf[1024] = {0};
		sprintf_s(cmdBuf, ".\\bin\\TexturePacker.exe --format cocos2d --sheet %s  --opt RGBA4444 --dither-fs-alpha %s", destPathAndName.c_str(), srcPathAndName.c_str());
		//sprintf_s(cmdBuf, "D:\\clients\\tools\\bin\\TexturePacker.exe --format cocos2d --sheet %s  --opt RGBA4444 --dither-fs-alpha %s", destPathAndName.c_str(), srcPathAndName.c_str());
		system(cmdBuf);
	}


	return 0;
}


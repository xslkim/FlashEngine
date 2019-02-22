// fileMove.cpp : Defines the entry point for the console application.
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
	std::string curPath = argv[1];
	std::string outDirName = argv[2];
	std::vector<std::string>inDirNameArray;
	int index = 3;
	while(true)
	{
		if( argv[index] != NULL )
		{
			std::string inDirName = argv[index];
			inDirNameArray.push_back(inDirName);
		}
		else
		{
			break;
		}
		index++;
	}

	std::string outDir = curPath;
	outDir += outDirName;
	_mkdir(outDir.c_str());

	for(int i = 0; i < inDirNameArray.size(); ++i)
	{
		std::string inDirName = inDirNameArray[i];
		std::string inDirPath = curPath + inDirName;
		inDirPath += "/LIBRARY";
		std::vector<std::string>inPathAndNameList;
		std::vector<std::string>inPngNameList;
		findFile(inDirPath.c_str(), inPathAndNameList, inPngNameList);
		for(int j = 0; j < inPathAndNameList.size(); ++j)
		{

			std::string inDirPathAndName = inPathAndNameList[j];
			std::ifstream in_file(inDirPathAndName.c_str(), std::ios::in | std::ios::binary | std::ios::_Nocreate);
			std::string outDirPathAndName = outDir + "/";
			outDirPathAndName += inDirName;
			outDirPathAndName += '_';
			outDirPathAndName += inPngNameList[j]; 
			std::ofstream out_file(outDirPathAndName.c_str(), std::ios::out | std::ios::binary );
			char buf[1024];
			while(!in_file.eof())
			{
				in_file.read(buf, 1024);
				out_file.write(buf, in_file.gcount());
			}
			in_file.close();
			out_file.close();
		}
	}



	return 0;
}


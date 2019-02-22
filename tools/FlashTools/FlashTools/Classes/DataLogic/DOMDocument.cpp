#include "DOMDocument.h"
#include "DataLogicMgr.h"
#include "FlashCCSprite.h"
#include "DOMSymbolItem.h"
#include "utils.h"
#include <io.h>


DOMDocument::DOMDocument()
{
	_flashRate = 24;
	_screenHeight = 1280;
	_screenWidth = 720;
}

DOMDocument::~DOMDocument()
{
	for(int i = 0; i < _mediaCCSpriteVector.size(); ++i)
	{
		delete _mediaCCSpriteVector[i];
	}

	for(int i = 0; i < _symbolItemVector.size(); ++i)
	{
		delete _symbolItemVector[i];
	}
}

std::string getPath(string pathAndName)
{
	int pos = pathAndName.size() - 1;
	for( ; pos >= 0; pos--)
	{
		if(pathAndName[pos] == '/' || pathAndName[pos] == '\\')
		{
			break;
		}
	}

	std::string path = pathAndName.substr(0, pos);
	return path + "/";
}

std::string getName(string pathAndName)
{
	int pos = pathAndName.size() - 1;
	for( ; pos >= 0; pos--)
	{
		if(pathAndName[pos] == '/' || pathAndName[pos] == '\\')
		{
			break;
		}
	}

	std::string name = pathAndName.substr(pos + 1);
	return name;
}

std::string DOMDocument::myfindFile(const char* path, const char* name)
{
	char szFind[MAX_PATH];
	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind;
	char szFile[MAX_PATH];

	strcpy(szFind,path);
	strcat(szFind,"\\*.*");

	hFind=FindFirstFileA(szFind,&FindFileData);
	if(INVALID_HANDLE_VALUE == hFind){
		return "";
	}

	while(TRUE)
	{

		if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			std::string curPath = path;
			curPath += FindFileData.cFileName;
			curPath += "/";
			int len = curPath.size();
			if( curPath[len - 2] != '.')
			{
				char buf[512] = {0};
				strcpy(buf, curPath.c_str());
				std::string pathname = myfindFile(buf, name);
				if(pathname  != "" )
				{
					return pathname;
				}
			}
		}
		else
		{
			std::string fileName = FindFileData.cFileName;
			if(fileName.find(name) != -1)
			{
				return path + fileName;
			}
		}

		if(!FindNextFileA(hFind,&FindFileData))
		{
			break;
		}
	}

	return "";
}

s_media_data DOMDocument::processMediaData(TiXmlElement* element)
{
	s_media_data data;

	data.name = element->Attribute("name");

	if(data.name.find("hudun") != -1)
	{
		int qwer = 234;
	}

	std::string path = getDataLogicMgr()->getPath();
	std::string sourceExternalFilepath = element->Attribute("sourceExternalFilepath");
	std::string pathAndName = path + sourceExternalFilepath;

	if( _access(pathAndName.c_str(), 0) != 0 )
	{
		size_t pos = sourceExternalFilepath.find("LIBRARY");
		if(pos == -1)
		{
			std::string pathName = element->Attribute("name");
			pathAndName = path + "./LIBRARY/";
			pathAndName += pathName;
		}
		else
		{
			std::string fileName = sourceExternalFilepath.substr(pos);
			pathAndName = path + fileName;
			if( _access(pathAndName.c_str(), 0) != 0 )
			{
				std::string pathEx = getPath(pathAndName);
				std::string name = getName(data.name);
				std::string pathName = pathEx + name;
				if( _access(pathName.c_str(), 0) != 0 )
				{
					pathAndName = pathEx + data.name;
				}
				else
				{
					pathAndName = pathName;
				}
			}
		}
 		
	}

	data.pathAndName = pathAndName;
	if( _access(pathAndName.c_str(), 0) != 0 )
	{
		std::string name = getFileNameEx(data.name.c_str());
		data.pathAndName = myfindFile(path.c_str(), name.c_str());
	}

	int width = 0;
	element->Attribute("frameRight", &width);
	data.width = (float)width / FLATE_PIXEL_RATE;
	//CCAssert(data.width >0);

	int height = 0;
	element->Attribute("frameBottom", &height);
	data.height = (float)height / FLATE_PIXEL_RATE;

	return data;
}

void DOMDocument::loadScreen(std::string pathAndName)
{
	TiXmlDocument* xmldoc = new TiXmlDocument(pathAndName.c_str());
	bool success = xmldoc->LoadFile();
	CCAssert( success);
	TiXmlElement* rootElement = xmldoc->RootElement();
	rootElement->Attribute("height", &_screenHeight);
	rootElement->Attribute("width", &_screenWidth);
}

std::string getFatherPath(const char* path)
{
	int len = strlen(path) - 2;
	for(;;)
	{
		if(path[len] == '\\' || path[len] == '/')
		{
			break;
		}
		len -= 1;
	}
	char buf[256] = {0};
	memcpy(buf,  path, len+1);
	return buf;
}

bool DOMDocument::LoadXml(std::string pathAndName)
{
	TiXmlDocument* xmldoc = new TiXmlDocument(pathAndName.c_str());
	bool success = xmldoc->LoadFile();
	CCAssert( success);
	TiXmlElement* rootElement = xmldoc->RootElement();
	rootElement->Attribute("frameRate", &_flashRate);
	TiXmlElement* firstChildElement = rootElement->FirstChildElement();
	std::string firstElementName = firstChildElement->Value();
	TiXmlElement* mediaElement = NULL;
	if(firstElementName == "media")
	{
		mediaElement = firstChildElement;
	}
	else
	{
		mediaElement = firstChildElement->NextSiblingElement("media");
	}
	
	int mediaIndex = 0;
	for(TiXmlElement* mediaItemElement = mediaElement->FirstChildElement(); mediaItemElement != NULL; mediaItemElement = mediaItemElement->NextSiblingElement())
	{
		std::string mediaItemName = mediaItemElement->Value();
		if( mediaItemName == "DOMBitmapItem")
		{
			


			std::string resName = mediaItemElement->Attribute("name");
			_mediaDataIndexMap[resName] = mediaIndex++;
			s_media_data data = processMediaData(mediaItemElement);
			_mediaDataVector.push_back( data );


			const char* smooth = mediaItemElement->Attribute("allowSmoothing");
			smooth = false;
			if(smooth)
			{
				std::string smooth_str = smooth;
				if(smooth_str == "true")
				{
					std::string path = getDataLogicMgr()->getPath();
					path = getFatherPath(path.c_str());
					std::string name = getFileNameEx(resName.c_str());
					std::string folderName = getDataLogicMgr()->getPlistName();
					std::string fpkName = getFileName( getDataLogicMgr()->getPath().c_str() );
					std::string pathAndName = path + folderName + "\\" + fpkName + "_" + name;
					std::string cmd = ("del " + pathAndName);
					printf(cmd.c_str());
					system(cmd.c_str());

					CxImage* image = new CxImage();
					image->Load(data.pathAndName.c_str(), CXIMAGE_FORMAT_PNG);
					EdgeData edgeData;
					edgeData.name = data.pathAndName;
					edgeData.width = image->GetWidth();
					edgeData.height = image->GetHeight();
					_edgeDataVector.push_back(edgeData);
				}
			}
			else
			{

			
				CxImage* image = new CxImage();
				image->Load(data.pathAndName.c_str(), CXIMAGE_FORMAT_PNG);

				EdgeData edgeData;
				edgeData.name = data.pathAndName;
				edgeData.width = 0; //image->GetWidth();
				edgeData.height = 0; //image->GetHeight();
				//edgeData.left = getLeftOff(image);
				//edgeData.right = getRightOff(image);
				//edgeData.top = getTopOff(image);
				//edgeData.button = getBottomOff(image);

				_edgeDataVector.push_back(edgeData);
			}

			if(getDataLogicMgr()->isPlay())
			{
				FlashCCSprite* spt = FlashCCSprite::spriteWithFile(data.pathAndName.c_str());
				_mediaCCSpriteVector.push_back(spt);
			}
		}
		else
		{
			CC_ASSERT(false);
		}
	}

	TiXmlElement* symbolsElement = mediaElement->NextSiblingElement("symbols");
	int symbolsIndex = 0;
	for(TiXmlElement* symbolsItemElement = symbolsElement->FirstChildElement(); symbolsItemElement != NULL; symbolsItemElement = symbolsItemElement->NextSiblingElement())
	{
		std::string symbolsValue = symbolsItemElement->Value();
		if( symbolsValue == "Include")
		{
			std::string resName = symbolsItemElement->Attribute("href");
			if (resName.find("box_attack") != -1)
			{
				continue;
			}

			if (resName.find("box_hurt") != -1)
			{
				continue;
			}

			std::string path = getDataLogicMgr()->getPath();
			std::string pathAndName = path + "LIBRARY/";
			pathAndName = pathAndName + resName; 
			char buf[256];
			strcpy( buf, resName.c_str() );
			buf[ strlen(buf) - 4 ] = 0;
			std::string keyName = buf;
			_symbolItemIndexMap[keyName] = symbolsIndex++;
			DOMSymbolItem* symbolItem = new DOMSymbolItem();
			if(resName == "cardc1.xml")
			{
				int qwer = 234;
			}
			symbolItem->LoadXml(pathAndName.c_str(), resName);
			_symbolItemVector.push_back(symbolItem);
		}
		else
		{
			CCAssert(false);
		}
	}

	TiXmlElement* timelinesElement = getChildElement(rootElement, "timelines");
	CCAssert( timelinesElement != NULL);
	TiXmlElement* DOMTimelineElement = getChildElement(timelinesElement, "DOMTimeline");
	CCAssert( DOMTimelineElement != NULL);
	TiXmlElement* layersElement = getChildElement(DOMTimelineElement, "layers");
	CCAssert( layersElement != NULL);

	TiXmlElement* DOMlayerElement = getChildElement(layersElement, "DOMLayer");
	for(;;)
	{
		const char* layerType = DOMlayerElement->Attribute("layerType");
		if( layerType == NULL )
		{
			break;
		}
		else
		{
			std::string strLayer = layerType;
			if(strLayer == "guide")
			{
				DOMlayerElement = DOMlayerElement->NextSiblingElement();
			}
		}
	}


	TiXmlElement* framesElement = getChildElement(DOMlayerElement, "frames");
	CCAssert( framesElement != NULL);
	int animantionCount = 0;
	for( TiXmlElement* DOMFrameElement = framesElement->FirstChildElement(); DOMFrameElement != NULL; DOMFrameElement = DOMFrameElement->NextSiblingElement())
	{
		TiXmlElement* elementsElement = getChildElement(DOMFrameElement, "elements");
		CCAssert( elementsElement != NULL);

		instanceData insData;
		TiXmlElement* DOMSymbolInstanceElement = getChildElement(elementsElement, "DOMSymbolInstance");
		if( DOMSymbolInstanceElement == NULL)
		{
			_animationDataVector.push_back(insData);
			_animationName.push_back("no animination");
			animantionCount++;
			continue;
		}

		
		insData.resName = DOMSymbolInstanceElement->Attribute("libraryItemName");
		

		TiXmlElement* matrixElement = getChildElement(DOMSymbolInstanceElement, "matrix");
		if(matrixElement)
		{
			TiXmlElement* valueMatrixElement = matrixElement->FirstChildElement();
			setAttribDouble(valueMatrixElement, "a", insData.matrix.a);
			setAttribDouble(valueMatrixElement, "b", insData.matrix.b);
			setAttribDouble(valueMatrixElement, "c", insData.matrix.c);
			setAttribDouble(valueMatrixElement, "d", insData.matrix.d);
			setAttribDouble(valueMatrixElement, "tx", insData.matrix.tx);
			setAttribDouble(valueMatrixElement, "ty", insData.matrix.ty);
		}


		TiXmlElement* transformationPointElement = getChildElement(DOMSymbolInstanceElement, "transformationPoint");
		if(transformationPointElement)
		{
			TiXmlElement* pointElement = transformationPointElement->FirstChildElement();
			setAttribDouble(pointElement, "x", insData.transformationPoint.x);
			setAttribDouble(pointElement, "y", insData.transformationPoint.y);
		}
		insData.changeMatrixRotate();
		_animationDataVector.push_back(insData);
		const char* animinationName = DOMFrameElement->Attribute("name");

		std::string animinationNameString;
		if(animinationName != NULL)
		{
			animinationNameString = animinationName;
		}
		_animationName.push_back(animinationNameString);
		animantionCount++;
	}


	return true;
}

bool DOMDocument::LoadFpk(std::string pathAndName)
{

	return true;
}

FlashCCSprite* DOMDocument::getSprite( std::string name )
{
	std::map<std::string, int>::iterator it = _mediaDataIndexMap.find(name);
	CCAssert( it != _mediaDataIndexMap.end() );
	int index = it->second;
	return _mediaCCSpriteVector[index];
}

EdgeData DOMDocument::getEdgeData(std::string name)
{
	std::map<std::string, int>::iterator it = _mediaDataIndexMap.find(name);
	CCAssert( it != _mediaDataIndexMap.end() );
	int index = it->second;
	return _edgeDataVector[index];
}

const DOMSymbolItem* DOMDocument::getSymbolItem( const char* name)
{
	std::map<std::string, int>::iterator it = _symbolItemIndexMap.find(name);
	CCAssert( it != _symbolItemIndexMap.end() );
	int index = it->second;
	return _symbolItemVector[index];
}
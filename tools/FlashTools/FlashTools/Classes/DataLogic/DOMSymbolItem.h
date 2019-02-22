#ifndef __DOMSymbolItem_H__
#define __DOMSymbolItem_H__
#include "tinyxml.h"
#include <string>
#include <map>
#include <vector>



class DOMFrame;
class DOMLayer;
class DOMSymbolItem
{
public:
	DOMSymbolItem();
	~DOMSymbolItem();
	void LoadXml(const char* pathAndName, std::string onlyName);
	std::string getName(){ return _symbolName; }
	void LoadPb(const char* fileName);
	int getFrameCount()const { return _maxFrame; }
	int getDOMLayerCount()const{ return _DOMFrameVectorMap.size(); }
	const DOMLayer* getDOMLayer(int index)const{ return _DOMFrameVectorMap[index]; }
private:
	std::string _symbolName;
	std::vector<DOMLayer*>_DOMFrameVectorMap;
	int _maxFrame;
};

#endif // __DOMSymbolItem_H__

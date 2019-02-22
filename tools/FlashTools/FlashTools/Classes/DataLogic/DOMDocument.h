#ifndef __DOMDocument_H__
#define __DOMDocument_H__

#include <string>
#include <map>
#include <vector>
#include "tinyxml.h"
#include "utils.h"

class FlashCCSprite;
class DOMSymbolItem;
class DOMDocument
{
public:
	DOMDocument();
	~DOMDocument();
	bool LoadXml(std::string path);
	bool LoadFpk(std::string pathAndName);

	FlashCCSprite* getSprite( std::string name );
	const DOMSymbolItem* getSymbolItem( const char* name);
	EdgeData getEdgeData(std::string name);

	int getAnimationCount(){ return _animationDataVector.size(); }
	instanceData getAnimationData(int index)
	{
		return _animationDataVector[index]; 
	}

	std::string getAnimationName(int index)
	{
		return _animationName[index]; 
	}

	double getFlashRate(){ return _flashRate; }

	std::vector<DOMSymbolItem*> getSymbolItemVector(){ return _symbolItemVector; }
	std::vector<s_media_data> _mediaDataVector;

	int getScreenHeight(){ return _screenHeight; }
	int getScreenWidth(){ return _screenWidth; }
	void loadScreen(std::string pathAndName);

private:
	s_media_data processMediaData(TiXmlElement* element);
	std::string myfindFile(const char* path, const char* name);
private:
	std::map<std::string, int> _mediaDataIndexMap;
	std::vector<FlashCCSprite*> _mediaCCSpriteVector;
	std::vector<EdgeData> _edgeDataVector;
	

	std::map<std::string, int> _symbolItemIndexMap;
	std::vector<DOMSymbolItem*> _symbolItemVector;

	std::vector<instanceData> _animationDataVector;
	std::vector<std::string> _animationName;

	int _screenHeight;
	int _screenWidth;
	double _flashRate;
};



#endif // __DOMDocument_H__

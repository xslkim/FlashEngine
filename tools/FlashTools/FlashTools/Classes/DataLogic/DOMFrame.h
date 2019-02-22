#ifndef __DOMFRAME_H__
#define __DOMFRAME_H__
#include "tinyxml.h"
#include <string>
#include "utils.h"


class DOMFrame
{
public:
	DOMFrame();
	~DOMFrame();
	void LoadXml(TiXmlElement* element);
	void LoadPb(const char* fileName);
	DOMFrameData getFrameData()const { return _frameData;}
	bool isBitmap()const { _isBitmap; }
private:
	void loadInstance(TiXmlElement* instanceElement);
	void loadBitMap(TiXmlElement* instanceElement);
	DOMFrameData _frameData;
	bool _isBitmap;

};

#endif // __DOMSymbolItem_H__

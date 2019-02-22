#ifndef __DOMLAYER_H__
#define __DOMLAYER_H__
#include "tinyxml.h"
#include <string>
#include "utils.h"




class DOMFrame;
class DOMLayer
{
public:
	DOMLayer();
	~DOMLayer();
	void LoadXml(TiXmlElement* element);
	void LoadPb(const char* fileName);
	void processFrameData();
	void processFrameBoxAttack();
	void processFrameBoxHurt();
	int getFrameCount()const 
	{
		return _frameVector.size(); 
	}
	FrameData getFrameData(int frame)const { return _frameVector[frame]; }
	std::vector<BoxKeyFrame> _BoxKeyFrameAttack;
	std::vector<BoxKeyFrame> _BoxKeyFrameHurt;
	bool IsBoxLayer()const { return _isBoxLayer; }
private:
	bool _isBoxLayer;
	std::vector<DOMFrame*> _DOMFrameVector;
	std::vector<FrameData> _frameVector;
};

#endif // __DOMLAYER_H__

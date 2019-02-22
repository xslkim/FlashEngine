#ifndef __ArtLabelTTF_H__
#define __ArtLabelTTF_H__
#include "cocos2d.h"

using namespace cocos2d;

class ArtLabelTTF :public CCNode
{
public:
	static ArtLabelTTF* create(const char* text, const char* font=NULL, int size = 24);
	static ArtLabelTTF* create(const char* text, bool enableCache, const char* font=NULL, int size = 24);
	static ArtLabelTTF* create(const char *text, const char *font, int size,const CCSize& dimensions, CCTextAlignment hAlignment);
	static ArtLabelTTF * turnNodeToArtLabelTTF(CCNode * node);
	ArtLabelTTF();
	virtual ~ArtLabelTTF();
	void setText(const char* text);
	const char* getText();
	void setFont(const char* font);
	const char* getFont() {return _font.c_str();}
	void setSize(int size);
	int getSize() {return _size;}
	void setCenterColor(ccColor3B color);
	void setAroundColor(ccColor3B color);
	virtual const ccColor3B& getCenterColor(void) {return _centerColor;}
	virtual const ccColor3B& getAroundColor(void) {return _aroundColor;}
	void construct();
	void setHorizontalAlignment(int alignment);
	void setVerticalAlignment(int verticalAlignment);
	CCTextAlignment getHorizontalAlignment() {return _alignment;}
	CCVerticalTextAlignment getVerticalAlignment() {return _verticalAlignment;}
	void setDimensions(const CCSize &dim);
	CCSize getDimensions() {return _dim;}
	void setTextAnchorPoint(const CCPoint& point);
	const CCSize & getTextContentSize();
	virtual CCTexture2D* getTexture(void);
	virtual std::string getClassName() {return "ArtLabelTTF";}
	void visit();
	void setCacheEnabled(bool enableCache);
private:
	//void offsetPosition();
	ccColor3B _centerColor;
	ccColor3B _aroundColor;
	CCLabelTTF* _center;
	//CCLabelTTF* _around[8];
	CCLabelTTF* _around;
	std::string _text;
	std::string _font;
	int			_size;
	CCTextAlignment _alignment;
	CCVerticalTextAlignment _verticalAlignment;
	CCSize _dim;
	CCPoint _textAnchorPoint;
	int _borderWidth;
	bool _enableCache;
};


#endif
#include "ArtLabelTTF.h"
//#include "PandoraUtils.h"
#define DefaultFontName "Arial"
static const char* DefaultFont = NULL;;
using namespace cocos2d;

ArtLabelTTF* ArtLabelTTF::create(const char* text, const char* font, int size)
{
	ArtLabelTTF* label = new ArtLabelTTF();
	label->setFont(font);
	label->setSize(size);
	label->setText(text);
	label->construct();
	label->autorelease();
	return label;
}

ArtLabelTTF* ArtLabelTTF::create(const char* text, bool enableCache, const char* font, int size)
{
	ArtLabelTTF* label = new ArtLabelTTF();
	label->setFont(font);
	label->setCacheEnabled(enableCache);
	label->setSize(size);
	label->setText(text);
	label->construct();
	label->autorelease();
	return label;
}

ArtLabelTTF* ArtLabelTTF::create(const char *text, const char *font, int size,const CCSize& dimensions, CCTextAlignment hAlignment)
{
	ArtLabelTTF* label = new ArtLabelTTF();
	label->setFont(font);
	label->setSize(size);
	label->setText(text);
	label->setDimensions(dimensions);
	label->setHorizontalAlignment(hAlignment);
	label->construct();
	label->autorelease();
	return label;
}
ArtLabelTTF::ArtLabelTTF()
{
	if(DefaultFont == NULL)
	{
		DefaultFont = DefaultFontName;
	}
	_center = NULL;
	_around = NULL;
	_size = 24;
	_aroundColor = ccc3(34, 10, 3);
	_centerColor = ccc3(255,255,255);
	_dim = CCSizeZero;
	_alignment = kCCTextAlignmentLeft;
	_verticalAlignment = kCCVerticalTextAlignmentTop;
	_textAnchorPoint = ccp(0.5,0.5);
	_borderWidth = 1;
	_enableCache = false;
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	_className = "ArtLabelTTF";
#endif
}
	
ArtLabelTTF::~ArtLabelTTF()
{
	//_center;
	
}

void ArtLabelTTF::setText(const char* text)
{
	_text = text;
}

const char* ArtLabelTTF::getText()
{
	return _text.c_str();
}

void ArtLabelTTF::setFont(const char* font)
{
	if(font == NULL)
	{
		_font = DefaultFont;
	}
	else
	{
		_font = font;
	}
}
void ArtLabelTTF::setSize(int size)
{
	_size = size;
}

void ArtLabelTTF::setCenterColor(ccColor3B color)
{
	_centerColor = color;
}
	
void ArtLabelTTF::setAroundColor(ccColor3B color)
{
	_aroundColor = color;
}


void ArtLabelTTF::construct()
{
	if(_center != NULL)
	{
		removeChild(_center, true);
	}
	if(_around != NULL)
	{
		removeChild(_around, true);
	}

	_around = CCLabelTTF::create(_text.c_str(), _font.c_str(), _size,_dim,_alignment, _enableCache);
	_around->setColor(_aroundColor);
	_around->setHorizontalAlignment(_alignment);
	_around->setVerticalAlignment(_verticalAlignment);
	_around->setAnchorPoint(_textAnchorPoint);
	addChild(_around);
	_center = CCLabelTTF::create(_text.c_str(), _font.c_str(), _size,_dim,_alignment, _enableCache);
	_center->setColor(_centerColor);
	_center->setHorizontalAlignment(_alignment);
	_center->setVerticalAlignment(_verticalAlignment);
	_center->setAnchorPoint(_textAnchorPoint);
	addChild(_center);
	//offsetPosition();
}
/*
void ArtLabelTTF::offsetPosition()
{
	int _borderWidth=1;
	_center->setPosition(ccp(0,0));
	_around[0]->setPosition(ccp(-_borderWidth, 0));
	_around[1]->setPosition(ccp(_borderWidth, 0));
	_around[2]->setPosition(ccp(0, _borderWidth));
	_around[3]->setPosition(ccp(0, -_borderWidth));
	_around[4]->setPosition(ccp(-_borderWidth, _borderWidth));
	_around[5]->setPosition(ccp(_borderWidth, _borderWidth));
	_around[6]->setPosition(ccp(-_borderWidth, -_borderWidth));
	_around[7]->setPosition(ccp(_borderWidth, -_borderWidth));
}
*/
ArtLabelTTF * ArtLabelTTF::turnNodeToArtLabelTTF(CCNode * node)
{
	return (ArtLabelTTF *) node;
}

void ArtLabelTTF::setHorizontalAlignment(int alignment)
{
	_alignment = (CCTextAlignment)alignment;
}
void ArtLabelTTF::setVerticalAlignment(int verticalAlignment)
{
	_verticalAlignment = (CCVerticalTextAlignment)verticalAlignment;
}
void ArtLabelTTF::setDimensions(const CCSize &dim)
{
	if (dim.width != _dim.width || dim.height != _dim.height)
	{
		_dim = dim;
	}
}

void ArtLabelTTF::setTextAnchorPoint(const CCPoint& point)
{
	_textAnchorPoint = point;
}

const CCSize & ArtLabelTTF::getTextContentSize()
{
	return _center->getContentSize();
}

CCTexture2D* ArtLabelTTF::getTexture(void)
{
	return _center->getTexture();
}

void ArtLabelTTF::visit()
{
	// quick return if not visible. children won't be drawn.
	if (!m_bVisible)
	{
		return;
	}
	kmGLPushMatrix();

	if (m_pGrid && m_pGrid->isActive())
	{
		m_pGrid->beforeDraw();
	}

	this->transform();
	_around->setPosition(ccp(-_borderWidth, 0));
	_around->visit();
	_around->setPosition(ccp(_borderWidth, 0));
	_around->visit();
	_around->setPosition(ccp(0, _borderWidth));
	_around->visit();
	_around->setPosition(ccp(0, -_borderWidth));
	_around->visit();
	_around->setPosition(ccp(-_borderWidth, _borderWidth));
	_around->visit();
	_around->setPosition(ccp(_borderWidth, _borderWidth));
	_around->visit();
	_around->setPosition(ccp(-_borderWidth, -_borderWidth));
	_around->visit();
	_around->setPosition(ccp(_borderWidth, -_borderWidth));
	_around->visit();
	_center->visit();

	// reset for next frame
	m_uOrderOfArrival = 0;

	if (m_pGrid && m_pGrid->isActive())
	{
		m_pGrid->afterDraw(this);
	}

	kmGLPopMatrix();
}

void ArtLabelTTF::setCacheEnabled(bool enableCache)
{
	_enableCache = enableCache;
}
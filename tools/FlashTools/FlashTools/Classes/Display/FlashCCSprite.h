#ifndef __FLASH_CCSPRITE_H__
#define __FLASH_CCSPRITE_H__
#include "cocos2d.h"
#include "utils.h"
#include "Matrix.h"

class FlashCCSprite :public cocos2d::CCSprite
{
public:
	static FlashCCSprite* spriteWithSpriteFrameName(const char *pszSpriteFrameName);
	static FlashCCSprite* spriteWithFile(const char* pszFileName);
	virtual void visit();
	virtual void draw(void);
	virtual void updateTransform(void);
	void updateVertex(Matrix m);
	void updateColor(double a, double r, double g, double b);
	void setVertex(cocos2d::ccVertex3F tl, cocos2d::ccVertex3F bl, cocos2d::ccVertex3F tr, cocos2d::ccVertex3F br);
private:
	void draw(cocos2d::ccVertex3F tl, cocos2d::ccVertex3F bl, cocos2d::ccVertex3F tr, cocos2d::ccVertex3F br);
	cocos2d::ccVertex3F _tl;
	cocos2d::ccVertex3F _bl;
	cocos2d::ccVertex3F _tr;
	cocos2d::ccVertex3F _br;
};


#endif
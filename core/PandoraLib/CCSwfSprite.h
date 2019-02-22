#ifndef __CCSWFSPRITE_H__
#define __CCSWFSPRITE_H__
#include "cocos2d.h"
//#include "PandoraConfig.h"
#include "flashMeta.pb.h"
#include "Matrix.h"

class CCSwfSprite :public cocos2d::CCSprite
{
public:
	static CCSwfSprite* spriteWithSpriteFrameName(const char *pszSpriteFrameName);
	static CCSwfSprite* create(cocos2d::CCSpriteFrame* sptframe);
	static CCSwfSprite* create(cocos2d::CCSpriteFrame* sptframe, cocos2d::CCSize rc);
	static CCSwfSprite* create(const char *picName);
	CCSwfSprite();
	virtual ~CCSwfSprite();
	virtual void visit();
	void updateTransform(Matrix mat, const FlashMetaData& metaData, int screenHeight);
	void setVertex(cocos2d::ccVertex3F tl, cocos2d::ccVertex3F bl, cocos2d::ccVertex3F tr, cocos2d::ccVertex3F br);

	GLubyte getFlashOpacity(void);
	void setFlashOpacity(GLubyte opacity);
	cocos2d::ccColor3B getFlashColor(void);
	void setFlashColor(cocos2d::ccColor3B color3);

	void setIsMirror(bool mirror);
	bool isMirror();

	bool isUsed;

	cocos2d::ccVertex3F _tl;
	cocos2d::ccVertex3F _bl;
	cocos2d::ccVertex3F _tr;
	cocos2d::ccVertex3F _br;
private:
	cocos2d::ccVertex3F ratatePoint(cocos2d::ccVertex3F point, float angle);
	void draw(cocos2d::ccVertex3F tl, cocos2d::ccVertex3F bl, cocos2d::ccVertex3F tr, cocos2d::ccVertex3F br);

	GLubyte m_nFlashOpacity;
	cocos2d::ccColor3B m_sFlashColor;
	bool m_isMirror;
};


#endif
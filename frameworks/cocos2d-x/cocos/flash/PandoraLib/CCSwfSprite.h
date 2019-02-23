#ifndef __CCSWFSPRITE_H__
#define __CCSWFSPRITE_H__
#include "cocos2d.h"
#include "FlashUtils.h"
//#include "PandoraConfig.h"
#include "flashMeta.pb.h"
#include "Matrix.h"

NS_CC_BEGIN

class CCSwfSprite :public Sprite
{
public:
	static CCSwfSprite* spriteWithSpriteFrameName(const char *pszSpriteFrameName);
	static CCSwfSprite* create(cocos2d::SpriteFrame* sptframe);
	static CCSwfSprite* create(cocos2d::SpriteFrame* sptframe, cocos2d::Size rc);
	static CCSwfSprite* create(const char *picName);
	CCSwfSprite();
	virtual ~CCSwfSprite();
	//virtual void visit();
#if (defined USE_FLASH_METADATA_CACHE && USE_FLASH_METADATA_CACHE == 1)
	void updateTransform(Matrix mat, const FlashMetaDataEx& metaData, int screenHeight);
#else
	void updateTransform(Matrix mat, const FlashMetaData& metaData, int screenHeight);
#endif
	void setVertex(cocos2d::Vertex3F tl, cocos2d::Vertex3F bl, Vertex3F tr, Vertex3F br);

	GLubyte getFlashOpacity(void);
	void setFlashOpacity(GLubyte opacity);
	Color3B getFlashColor(void);
	void setFlashColor(Color3B color3);

	void setIsMirror(bool mirror);
	bool isMirror();
	void setGrey(bool grey);
	void setBright(bool bright);
	void setRed(int);
	void setGreen();
	void setGolden();
	void setBurn();
	void setOriginColor();

	bool isUsed;
    void visit(Renderer* renderer, const Mat4 &parentTransform, uint32_t parentFlags);
    void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags);

	bool _changeColor;

	Vertex3F _tl;
	Vertex3F _bl;
	Vertex3F _tr;
	Vertex3F _br;
    //virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags);
private:
	cocos2d::ccVertex3F ratatePoint(cocos2d::ccVertex3F point, float angle);
	//void draw(cocos2d::ccVertex3F tl, cocos2d::ccVertex3F bl, cocos2d::ccVertex3F tr, cocos2d::ccVertex3F br);

	GLubyte m_nFlashOpacity;
	cocos2d::ccColor3B m_sFlashColor;
	bool m_isMirror;
};
NS_CC_END

#endif
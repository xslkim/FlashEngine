#ifndef __FLASH_SPRITE__
#define __FLASH_SPRITE__


#include "2d/CCNode.h"
#include "base/CCProtocols.h"
#include "renderer/CCTextureAtlas.h"
#include "base/ccTypes.h"
#include <string>
#include "FlashDefine.h"

#ifdef EMSCRIPTEN
#include "CCGLBufferedNode.h"
#endif // EMSCRIPTEN
#include "physics/CCPhysicsBody.h"
#include "renderer/CCQuadCommand.h"
#include "renderer/CCCustomCommand.h"

#include <map>


struct FlashDataItem;
class SwfSpriteBatchNode;
class FlashSpritePb;
class FlashMeta;
class FlashMetaData;
class FlashMetaDataEx;

NS_CC_BEGIN
class CCSwfSprite;
class FlashSpriteDelegate;
class SpriteFrame;
class Sprite;

struct FlashEvent
{
	FlashEvent():anim(0),frame(0){}
	FlashEvent(const char* n, int a, int f)
		:name(n)
		,anim(a)
		,frame(f)
	{}
	int anim;
	int frame;
	std::string name;
};

//class FlashValidFlag;
class FlashSprite :public Node
{
public:
	FlashSprite(void);
	virtual ~FlashSprite(void);
	bool loadFile(std::string name);
	void setLoop(bool loop);
	int getFrameCount();

	void setBackGroundNode( Node* bgNode);
	Node* getBackGroundNode();

	GLubyte getOpacity() const;
	void setOpacity(unsigned char opacity);
	
	Color3B getColor(void){ return m_sColor; }
	void setColor(Color3B color3){ m_sColor = color3; }
	void setPosition(float x,float y);
	void setPosition(Point var);
	//const Vec2& getPosition(void){ return Node::getPosition(); };

	void setGray(bool gray){ _isGray = gray; }
	bool isGray(){ return _isGray; }

	void setBright(bool bright){ _isBright = bright; }
	bool isBright(){ return _isBright; }

	void setRed(){ _red = 5; }
	void setGreen(bool green){ _isGreen = green; }
	void setGolden(bool Golden){ _isGolden = Golden; }
	void setBurn(bool Burn){ _isBurn = Burn; }
	
	void setRedDebug(int red){ _redDebug = red; }

	void setIsMirror(bool mirror);
	bool isMirror(){ return m_isMirror;}

	void setReverse(bool reverse);
	bool isReverse(){ return m_isReverse;}

	void setIsRun(bool run){ _isRun = run; }
	void setCurFrame(int frame);

	void setDelegate(FlashSpriteDelegate* delegate);
	void changeAnimation(int anim);
	bool isAnimationExist(int anim);
	int getCurAnimation();
	virtual void setScale(float scale);

	void setPlayRate(float rate);
	float getPlayRate();
    
    //add by lc
    virtual void setScaleX(float scale);
    virtual void setScaleY(float scale);

	void setVisible(bool visible);
    
	virtual void setAnchorPoint(Point var);
    //virtual void visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags);
	void step(float dt);
	virtual void onEnter();
	virtual void onExit();
	static FlashSprite* create(const char *pszFileName);

	//xul add clip rect support
	void setClipRect(Rect rect){ m_rClipRect = rect; m_bHasClipRect = true; };
	void clearClipRect(){ m_bHasClipRect = false; }
	//void _visit();

	void setOffsetPosition(float x, float y, float backTime);

	Rect getClipRect(){ return m_rClipRect; }
	void prepareSprite();

	void clearChangeInstanceMap();
	void addChangeInstance(std::string, SpriteFrame* spt);

	void addFrameEvent(const char* name, int anim, int frame);
	void registerFlashScriptHandler(int handler);
	void registerEndAnimationScriptHandler(int handler);
	void unregisterFlashScriptHandler();
	void unregisterEndAnimationScriptHandler();
	static void setAnimationTimeRate(float rate);

	static float g_hero_monster_scale;
protected:
	static float s_animationTimeRate;
	virtual void frameChange(int form, int to){}
	virtual void onAnimationStart(){}
	virtual void onAnimationEnd(){}
	void stepReverse(float dt, const FlashDataItem*);
	float _playRate;
	//xul add clip rect support
	bool   m_bHasClipRect;
	Rect m_rClipRect;
	const FlashEvent* getEvent(int anim, int farme);
	

	void calculateAnchorPoint();
#if (defined USE_FLASH_METADATA_CACHE && USE_FLASH_METADATA_CACHE == 1)
	void drawMetaItem(const FlashMetaDataEx& insMetaData, int frame, FlashDataItem* item);
	void drawMetaBitmap(const FlashMetaDataEx& data, FlashDataItem* item, const std::string insName);
	std::string getFlashInstanceName(const FlashMetaDataEx& metaData);
	std::string getFlashItemName(const FlashMetaDataEx& metaData);
	bool hasSingleframeByMetaData(const FlashMetaDataEx& metaData);
	bool singleframeByMetaData(const FlashMetaDataEx& metaData);
	bool hasPlayonceByMetaData(const FlashMetaDataEx& metaData);
	bool playonceByMetaData(const FlashMetaDataEx& metaData);
	//Matrix getMatrix(const FlashMetaDataEx& metaData);
	//std::string getFlashItemName(FlashMetaDataEx* metaData);
	float conAlpha(const FlashMetaDataEx& dataA, const FlashMetaDataEx& dataB);
#else
	void drawMetaItem(const FlashMetaData& insMetaData, int frame, FlashDataItem* item);
	void drawMetaBitmap(const FlashMetaData& data, FlashDataItem* item, const std::string insName);
#endif

	//void updateDisplayedOpacity(GLubyte parentOpacity);
	
	//FlashSpritePb* _pbData;
	int _curAnimination;
	int _curFrame;
	int _curFrameSize;
	bool _loop;
	bool _isRun;
	bool _lock;
	FlashSpriteDelegate* _delegate;
	float _curTime;
	std::string _pathName;
	std::string _name;

	GLubyte m_nOpacity;
	Color3B m_sColor;
	bool m_isMirror;
	bool m_isReverse;

	std::vector<CCSwfSprite*> _changeSwfSpriteVector;
	std::map<std::string, SpriteFrame*> _changeInstanceMap;

	std::multimap<std::string, cocos2d::CCSwfSprite*> _spriteMap;
	void initSprite();
	void addSprite(const std::string& name, CCSwfSprite* spt);
	CCSwfSprite* getSprite(const std::string& name);
	SpriteFrame* getChangeSpriteFrame(const std::string& name);
	Sprite* _displayNode;
	int _screenHeight;
	std::vector<FlashEvent>_eventVector;
	int m_nFlashScriptHandler;
	int m_nEndAnimationScriptHandler;
	//FlashValidFlag* _flashValid;
	bool m_bTargetEndAnimationFunction;
	float _positionOffsetBackTime;
	float _offsetBackSpeedx;
	float _offsetBackSpeedy;
	Node* _bgNode;
	bool _firstChangeAnimation;
	bool _isGray;
	bool _isBright;
	int _red;
	bool _isGreen;
	bool _isGolden;
	bool _isBurn;
	int _redDebug;
};

NS_CC_END

#endif


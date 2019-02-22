#ifndef __FLASH_SPRITE__
#define __FLASH_SPRITE__


#include "cocos2d.h"
//#include "PandoraConfig.h"
#include "flashMeta.pb.h"
#include <map>

using namespace cocos2d;

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

struct FlashDataItem;
class FlashSpriteDelegate;
class SwfSpriteBatchNode;
class FlashSpritePb;
class CCSwfSprite;
class FlashSprite :public cocos2d::CCNode
{
public:
	FlashSprite(void);
	virtual ~FlashSprite(void);
	bool loadFile(std::string name);
	void setLoop(bool loop);

	unsigned char getOpacity(void){ return m_nOpacity;}
	void setOpacity(unsigned char opacity){ m_nOpacity = opacity; }
	cocos2d::ccColor3B getColor(void){ return m_sColor; }
	void setColor(cocos2d::ccColor3B color3){ m_sColor = color3; }
	void setPosition(float x,float y);
	void setPosition(cocos2d::CCPoint var);
	const cocos2d::CCPoint& getPosition(void){ return CCNode::getPosition(); };

	void setIsMirror(bool mirror);
	bool isMirror(){ return m_isMirror;}

	void setIsRun(bool run){ _isRun = run; }
	void setCurFrame(int frame);

	void setDelegate(FlashSpriteDelegate* delegate);
	void changeAnimation(int anim);
	bool isAnimationExist(int anim);
	int getCurAnimation();
	virtual void setScale(float scale);
    
    //add by lc
    virtual void setScaleX(float scale);
    virtual void setScaleY(float scale);

	void setVisible(bool visible);
    
	virtual void setAnchorPoint(cocos2d::CCPoint var);
	virtual void visit();
	void step(float dt);
	virtual void onEnter();
	virtual void onExit();
	static FlashSprite* create(const char *pszFileName);

	//xul add clip rect support
	void setClipRect(cocos2d::CCRect rect){ m_rClipRect = rect; m_bHasClipRect = true; };
	void clearClipRect(){ m_bHasClipRect = false; }
	void _visit();

	cocos2d::CCRect getClipRect(){ return m_rClipRect; }
	void prepareSprite();

	void clearChangeInstanceMap();
	void addChangeInstance(std::string, CCSpriteFrame* spt);

	void addFrameEvent(const char* name, int anim, int frame);
	void registerFlashScriptHandler(int handler);
	void registerEndAnimationScriptHandler(int handler);
	void unregisterFlashScriptHandler();
	void unregisterEndAnimationScriptHandler();

private:

	//xul add clip rect support
	bool   m_bHasClipRect;
	cocos2d::CCRect m_rClipRect;
	const FlashEvent* getEvent(int anim, int farme);
	void drawMetaItem(const FlashMetaData& insMetaData, int frame, FlashDataItem* item);
	void drawMetaBitmap(const FlashMetaData& data, FlashDataItem* item, const std::string insName);

	void calculateAnchorPoint();
	
	//FlashSpritePb* _pbData;
	int _curAnimination;
	int _curFrame;
	bool _loop;
	bool _isRun;
	bool _lock;
	FlashSpriteDelegate* _delegate;
	float _curTime;
	std::string _pathName;
	std::string _name;

	GLubyte m_nOpacity;
	cocos2d::ccColor3B m_sColor;
	bool m_isMirror;

	std::vector<CCSwfSprite*> _changeSwfSpriteVector;
	std::map<std::string, CCSpriteFrame*> _changeInstanceMap;

	std::multimap<std::string, CCSwfSprite*> _spriteMap;
	void initSprite();
	void addSprite(const std::string& name, CCSwfSprite* spt);
	CCSwfSprite* getSprite(const std::string& name);
	CCSpriteFrame* getChangeSpriteFrame(const std::string& name);
	CCNode* _displayNode;
	int _screenHeight;
	std::vector<FlashEvent>_eventVector;
	int m_nFlashScriptHandler;
	int m_nEndAnimationScriptHandler;
};

#endif


#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "utils.h"
#include "flashMeta.pb.h"

class FlashCCSprite;
class DOMSymbolItem;
class HelloWorld : public cocos2d::CCLayer
{
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  

	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::CCScene* scene();

	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);
	
	// a selector callback
	virtual void menuCloseCallback(CCObject* pSender);

	virtual void menuLeftCallback(CCObject* pSender);

	virtual void menuRightCallback(CCObject* pSender);

	virtual void menuUpCallback(CCObject* pSender);

	virtual void menuDownCallback(CCObject* pSender);

	virtual void menuPlayCallback(CCObject* pSender);

	virtual void menuStopCallback(CCObject* pSender);

	virtual void visit(void);

	void step(cocos2d::ccTime dt);

private:
	void drawSymbolItem1(instanceData insData, int frame);
	void drawSymbolItem2(instanceData insData, int frame);
	void drawSymbolItem3(instanceData insData, int frame);
	void drawSymbolItem4(instanceData insData, int frame);
	void drawBitmap(Matrix transMatrix, instanceData insData);

	void drawMetaItem(const FlashMetaData& insMetaData, int frame);
	void drawMetaBitmap(const FlashMetaData& data);

	void forwardFrame(bool loop);
	void backwardFrame();

	int _curAnimination;
	int _maxAniminationCount;
	int _curFrame;
	bool _isPlaying;

	CCMenuItemImage* _stopMenuItem;
	CCMenuItemImage* _playMenuItem;


	cocos2d::CCLabelTTF* _animationNameLabel;
	cocos2d::CCLabelTTF* _curFrameLabel;
	// implement the "static node()" method manually
	LAYER_NODE_FUNC(HelloWorld);

	cocos2d::CCPoint _curPoint;
	cocos2d::CCPoint _startPoint;
	bool _isTouch;

	//pack flash meta with new format
};

void packCacheMeta();
void packNewMeta();
void packNewMetaBox();

#endif // __HELLOWORLD_SCENE_H__

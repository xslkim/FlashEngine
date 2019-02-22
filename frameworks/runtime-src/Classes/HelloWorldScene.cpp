#include "HelloWorldScene.h"
#include "../../cocos2d-x/cocos/flash/PandoraLib/FlashSprite.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();


    auto closeItem = MenuItemFont::create("close",  CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

	//auto effect = FlashSprite::create("animation/UI_maoxianicon_fx");
	//effect->changeAnimation(0);

	auto ly = Layer::create();
	ly->setColor(Color3B(0,255,0));
	ly->setOpacity(255);
	addChild(ly);


	auto spt = FlashSprite::create("res/hero/r11_ailiya");
	spt->setPosition(ccp(300, 0));
	spt->setRedDebug(100);
	spt->changeAnimation(2);
	addChild(spt);


	spt = FlashSprite::create("res/hero/r11_ailiya");
	spt->setPosition(ccp(300, 125));
	//spt->setRedDebug(80);
	spt->setGray(true);
	spt->changeAnimation(2);
	addChild(spt);

	spt = FlashSprite::create("res/hero/r11_ailiya");
	spt->setPosition(ccp(300, 250));
	spt->setRedDebug(60);
	spt->changeAnimation(2);
	addChild(spt);

	spt = FlashSprite::create("res/hero/r11_ailiya");
	spt->setPosition(ccp(300, 375));
	spt->setRedDebug(40);
	spt->changeAnimation(2);
	addChild(spt);

	spt = FlashSprite::create("res/hero/r11_ailiya");
	spt->setPosition(ccp(300, 500));
	spt->setRedDebug(20);
	spt->changeAnimation(2);
	addChild(spt);

	spt = FlashSprite::create("res/hero/r11_ailiya");
	spt->setPosition(ccp(300, 625));
	spt->changeAnimation(2);
	addChild(spt);

	_spt = spt;

    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    //Director::getInstance()->end();
	_spt->setOffsetPosition(10, 0, .1f);
	_spt->setRed();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

#include "AppDelegate.h"
#include "CCLuaEngine.h"
#include "SimpleAudioEngine.h"
#include "cocos2d.h"
#include "Runtime.h"
#include "ConfigParser.h"
#include "HelloWorldScene.h"

//#include "iosIAP_auto.hpp"
//#include "flash/PandoraLib/AesZipUtils.h"

using namespace CocosDenshion;

USING_NS_CC;
using namespace std;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
    SimpleAudioEngine::end();
}

bool AppDelegate::applicationDidFinishLaunching()
{
	cocos2d::DynamicUpdateMgr::getInstance();
    
#if (COCOS2D_DEBUG>0  && CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    initRuntime();
#endif

    if (!ConfigParser::getInstance()->isInit()) {
            ConfigParser::getInstance()->readConfig();
        }

    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();    
    if(!glview) {
        Size viewSize = ConfigParser::getInstance()->getInitViewSize();
        string title = ConfigParser::getInstance()->getInitViewName();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
        extern void createSimulator(const char* viewName, float width, float height,bool isLandscape = true, float frameZoomFactor = 1.0f);
        bool isLanscape = ConfigParser::getInstance()->isLanscape();
        createSimulator(title.c_str(),viewSize.width,viewSize.height,isLanscape);
#else
        glview = GLView::createWithRect(title.c_str(), Rect(0,0,viewSize.width,viewSize.height));
        director->setOpenGLView(glview);
#endif
    }

   
    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
   
    auto engine = LuaEngine::getInstance();
    ScriptEngineManager::getInstance()->setScriptEngine(engine);

    LuaStack* stack = engine->getLuaStack();
	std::string key = "#yyxs168#";
	std::string sign = "icefirexxtea";
	CCDirector::sharedDirector()->setXXTEAKeyAndSign(key, sign);
	stack->setXXTEAKeyAndSign(key.c_str(), key.size(), sign.c_str(), sign.size());
    
	

    //register custom function
    //LuaStack* stack = engine->getLuaStack();
    //register_custom_function(stack->getLuaState());

	//char* netString = AesZipUtils::getInstance()->json2netstring("asd123456789asd123456789asd123456789asd123456789asd123456789asd123456789");
	//char* json = AesZipUtils::getInstance()->netstring2json(netString);

	//auto scene = HelloWorld::createScene();
	//CCDirector::sharedDirector()->runWithScene(scene);
	//return true;
    //lua_State *L = stack->getLuaState();
    //lua_getglobal(L, "_G");
    //register_all_iosIAP(L);
    //lua_settop(L, 0);
    
    
#if (COCOS2D_DEBUG>0 && CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    if (startRuntime())
        return true;
#endif

    engine->executeScriptFile(ConfigParser::getInstance()->getEntryFile().c_str());
	

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();
	
#if CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();
	
#if CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
#endif
}

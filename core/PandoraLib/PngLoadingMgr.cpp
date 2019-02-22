#include "PngLoadingMgr.h"
#include "CCLuaEngine.h"
#include "script_support/CCScriptSupport.h"

using namespace cocos2d;

PngLoadingMgr* PngLoadingMgr::_instance = 0;


PngLoadingMgr* PngLoadingMgr::getInstance()
{	
	if( _instance == 0)
	{
		_instance = new PngLoadingMgr();
	}
	return _instance;
}

PngLoadingMgr::PngLoadingMgr()
{
	m_nScriptHandler = 0;
	_isLoading = false;
}

void PngLoadingMgr::loadingCallBack(CCObject* obj)
{
	_isLoading = false;
	if( m_nScriptHandler != 0)
	{
		CCLOG("cocos2d : PngLoadingMgr callback with resName %s , handler  = %d\n", _curLoadingName.c_str(), m_nScriptHandler);
		CCLuaEngine* engine = (CCLuaEngine*)CCScriptEngineManager::sharedManager()->getScriptEngine();
		CCLuaStack* stack = engine->getLuaStack();
		stack->pushString(_curLoadingName.c_str());
		stack->executeFunctionByHandler(m_nScriptHandler, 1);
		stack->clean();
	}
    CCTexture2D* tex = CCTextureCache::sharedTextureCache()->addImage(_curLoadingName.c_str());
    CCAssert(tex != NULL, "");
    
}

bool PngLoadingMgr::loadPng(const char* name)
{
	if(_isLoading)
	{
		return false;
	}
	_curLoadingName = name;
	CCTextureCache::sharedTextureCache()->addImageAsync(name, this, callfuncO_selector(PngLoadingMgr::loadingCallBack));  
	return true;
}


void PngLoadingMgr::registerEndLoadingScriptHandler(int handler)
{
	unregisterEndLoadingScriptHandler();
	m_nScriptHandler = handler;
}


void PngLoadingMgr::unregisterEndLoadingScriptHandler()
{
	if (m_nScriptHandler)
	{
		CCScriptEngineManager::sharedManager()->getScriptEngine()->removeScriptHandler(m_nScriptHandler);
		m_nScriptHandler = 0;
	}
}

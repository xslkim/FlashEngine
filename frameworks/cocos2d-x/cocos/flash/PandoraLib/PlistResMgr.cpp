#include "PlistResMgr.h"
//#include "PandoraUtils.h"
//#include "Poseidon/StartupConfig.h"

using namespace cocos2d;

PlistResMgr* PlistResMgr::_instance = 0;


PlistResMgr* PlistResMgr::getInstance()
{	
	if( _instance == 0)
	{
		_instance = new PlistResMgr();
	}
	return _instance;
}

PlistResMgr::PlistResMgr()
{

}

CCTexture2D* PlistResMgr::getTexture(const std::string key)
{
	std::map<std::string, CCTexture2D*>::iterator it = _plistDictMap.find(key);
	if( it != _plistDictMap.end())
	{
		return it->second;
	}
	return NULL;
}

void PlistResMgr::releasePlist(const std::string key)
{
	std::map<std::string, CCTexture2D*>::iterator it = _plistDictMap.find(key);
	CCAssert( it != _plistDictMap.end(), "");
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(key.c_str());
	CCTexture2D* tex = it->second;
	//CCAssert(tex->isSingleReference(), "");
	tex->release();
	CCTextureCache::sharedTextureCache()->removeTexture(tex);
	_plistDictMap.erase(it);
}

void PlistResMgr::loadPlist(const std::string key)
{
	if(_plistDictMap.find(key) != _plistDictMap.end())
	{
		return;
	}

	CCAssert( getTexture(key) == NULL, "");
	std::string name = key;
	std::string plistName = key; // = CCFileUtils::sharedFileUtils()->fullPathFromRelativeFile(key.c_str());
    CCSpriteFrameCache::getInstance()->addSpriteFramesWithFile(plistName);
    
    std::string texName = plistName.substr(0, plistName.length() - 6) + ".png";
    Texture2D* tex = CCTextureCache::getInstance()->getTextureForKey(texName);
    CCASSERT(tex != nullptr, "");
    tex->retain();
    if (tex == nullptr) {
        tex = _plistDictMap[key.c_str()];
    }
    _plistDictMap[key] = tex;

// 	CCTexture2D* tex = (CCTexture2D*)CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(plistName.c_str());
// 	if(tex == NULL)
// 	{
// 		tex = _plistDictMap[key.c_str()];
// 	}
// 	tex->retain();
// 
// 
// 	_plistDictMap[key] = tex;
}

//std::string PlistResMgr::getCCNodeClassName(cocos2d::CCNode * node)
//{
//	return node->getClassName();
//}

//void PlistResMgr::onSystemBackKeyClick()
//{
//	CCDirector::sharedDirector()->onSystemBackButtonClick();
//}

//void PlistResMgr::registerSystemBackHandler(int nHandler)
//{
//	CCDirector::sharedDirector()->registerSystemBackButtonHandler(nHandler);
//}
//
//void PlistResMgr::unregisterSystemBackHandler()
//{
//	CCDirector::sharedDirector()->unregisterSystemBackButtonHandler();
//}
#include "PlistResMgr.h"

//#include "PandoraUtils.h"
//#include "Poseidon/StartupConfig.h"

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/android-commons/jni/JniUtils.h"
#endif

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include <unistd.h>
	int lua_write_atom_file(const char* filepath, const char* data)
	{
		size_t len = strlen(data);
		size_t write_len = 0;
		write_len = 0;
		FILE* fp =fopen(filepath, "wb");
		if (fp) {
			write_len= fwrite(data, 1, len, fp);
			if(write_len == len){
				fflush(fp);
				int fd = fileno(fp);
				fsync(fd);
			}

			fclose(fp);
			if(write_len != len) {
				remove(filepath);
				write_len=0;
			}
		}
		return 1;//这里才是正确返回
	}
#else
	int lua_write_atom_file(const char* filepath, const char* data)
	{
		size_t len = strlen(data);
		size_t write_len = 0;
		FILE* fp =fopen(filepath, "wb");
		if (fp) {
			fwrite(data, 1, len, fp);
			fflush(fp);
			fclose(fp);
		}
		return 1;//这里才是正确返回
	}
#endif



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

	CCTexture2D* tex = (CCTexture2D*)CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(plistName.c_str());
	if(tex == NULL)
	{
		tex = _plistDictMap[key.c_str()];
	}
	tex->retain();


	_plistDictMap[key] = tex;
}

std::string PlistResMgr::getCCNodeClassName(cocos2d::CCNode * node)
{
	return node->getClassName();
}

void PlistResMgr::onSystemBackKeyClick()
{
	CCDirector::sharedDirector()->onSystemBackButtonClick();
}

void PlistResMgr::registerSystemBackHandler(int nHandler)
{
	CCDirector::sharedDirector()->registerSystemBackButtonHandler(nHandler);
}

void PlistResMgr::unregisterSystemBackHandler()
{
	CCDirector::sharedDirector()->unregisterSystemBackButtonHandler();
}

void PlistResMgr::tryToCloseActivity(std::string message, std::string confirm , std::string cancel)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t
		, "com/happyelements/arda/MainActivity"
		, "tryToCloseActivity"
		, "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V"))
	{
		jstring jmessage = t.env->NewStringUTF(message.c_str());
		jstring jconfirm = t.env->NewStringUTF(confirm.c_str());
		jstring jcancel = t.env->NewStringUTF(cancel.c_str());
		t.env->CallStaticVoidMethod(t.classID, t.methodID, jmessage, jconfirm, jcancel);
		t.env->DeleteLocalRef(jmessage);
		t.env->DeleteLocalRef(jconfirm);
		t.env->DeleteLocalRef(jcancel);
		t.env->DeleteLocalRef(t.classID);
	}

#endif
}

void PlistResMgr::terminateProcess()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo t;

    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/lib/Cocos2dxHelper", "terminateProcess", "()V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
#endif
}

void PlistResMgr::setEnableCacheTableView(bool value)
{
	CCDirector::sharedDirector()->setEnableCacheTableView(value);
}

bool PlistResMgr::getEnableCacheTableView()
{
	return CCDirector::sharedDirector()->getEnableCacheTableView();
}
#include "FlashSprite.h"
#include <string>
#include <fstream>
#include "FlashUtils.h"
#include "FlashSpriteDelegate.h"
#include "NotificationDirector.h"
#include "CCSwfSprite.h"
#include "FlashCacheMgr.h"
#include "flashMeta.pb.h"
#include "CCSwfSprite.h"
#include "Matrix.h"
#include "CCLuaEngine.h"
#include "script_support/CCScriptSupport.h"

static float g_left = 999999999.f;
static float g_right = -999999999.f;
static float g_top = -999999999.f;
static float g_bottom = 999999999.f;

USING_NS_CC;

Matrix getMatrix(const FlashMetaData& metaData)
{
	Matrix m;
	if( metaData.has_ma() )
	{
		m.a = metaData.ma();
	}

	if( metaData.has_mb() )
	{
		m.b = metaData.mb();
	}

	if( metaData.has_mc() )
	{
		m.c = metaData.mc();
	}

	if( metaData.has_md() )
	{
		m.d = metaData.md();
	}

	if( metaData.has_mtx() )
	{
		m.tx = metaData.mtx();
	}

	if( metaData.has_mty() )
	{
		m.ty = metaData.mty();
	}
	return m;
}

void setFlashMetaData(FlashMetaData& metaData, Matrix matrix)
{
	if(matrix.a != 1)
		metaData.set_ma(matrix.a);

	if(matrix.b != 0)
		metaData.set_mb(matrix.b);

	if(matrix.c != 0)
		metaData.set_mc(matrix.c);

	if(matrix.d != 1)
		metaData.set_md(matrix.d);

	if(matrix.tx != 0)
		metaData.set_mtx(matrix.tx);

	if(matrix.ty != 0)
		metaData.set_mty(matrix.ty);

}

float conAlpha(const FlashMetaData& dataA, const FlashMetaData& dataB)
{
	float alphaA = 1.f;
	float alphaB = 1.f;
	if( dataA.has_a() )
		alphaA = dataA.a();
	if( dataB.has_a() )
		alphaB = dataB.a();
	return alphaA * alphaB;
}

float conRed(const FlashMetaData& dataA, const FlashMetaData& dataB)
{
	float redA = 1.f;
	float redB = 1.f;
	if( dataA.has_r() )
		redA = dataA.r();
	if( dataB.has_r() )
		redB = dataB.r();
	return redA * redB;
}

float conGreen(const FlashMetaData& dataA, const FlashMetaData& dataB)
{
	float greenA = 1.f;
	float greenB = 1.f;
	if( dataA.has_g() )
		greenA = dataA.g();
	if( dataB.has_g() )
		greenB = dataB.g();
	return greenA * greenB;
}

float conBlue(const FlashMetaData& dataA, const FlashMetaData& dataB)
{
	float blueA = 1.f;
	float blueB = 1.f;
	if( dataA.has_b() )
		blueA = dataA.b();
	if( dataB.has_b() )
		blueB = dataB.b();
	return blueA * blueB;
}


static const bool kIsUsingBatchNode = true;

FlashSprite::FlashSprite(void)
:_loop(true)
//,_flashDataItem(NULL)
,_curAnimination(-1)
,_curFrame(0)
,_isRun(true)
,_delegate(NULL)
,_lock(false)
,m_nFlashScriptHandler(0)
,m_nEndAnimationScriptHandler(0)
{
	m_nOpacity = 255;
	m_sColor = ccWHITE;
	m_isMirror = false;
	m_bHasClipRect = false;
	_screenHeight = 960;
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	_className = "FlashSprite";
#endif
}

FlashSprite::~FlashSprite(void)
{
	unregisterFlashScriptHandler();
	unregisterEndAnimationScriptHandler();
	_displayNode->removeAllChildrenWithCleanup(true);
	for(std::multimap<std::string, CCSwfSprite*>::iterator swfIt = _spriteMap.begin(); swfIt != _spriteMap.end(); swfIt++)
	{
		(*swfIt).second->release();
	}
	_spriteMap.clear();
	clearChangeInstanceMap();
	FlashCacheMgr::getInstance()->releateItem(_pathName);
}

FlashSprite* FlashSprite::create(const char *pszFileName)
{
	//std::string fileName = getFlashFileName(pszFileName);
	FlashSprite *pobSprite = new FlashSprite();
	if (pobSprite && pobSprite->loadFile(pszFileName))
	{
		pobSprite->autorelease();
		return pobSprite;
	}
	CC_SAFE_DELETE(pobSprite);
	return NULL;
}



void FlashSprite::onEnter()
{
	CCNode::onEnter();
	schedule(schedule_selector(FlashSprite::step), 0);
}

void FlashSprite::onExit()
{
	CCNode::onExit();
	unschedule(schedule_selector(FlashSprite::step));
}

void FlashSprite::setLoop(bool loop)
{
	//CCAssert(!_lock, "");
	_loop = loop;
}

void FlashSprite::setDelegate(FlashSpriteDelegate* delegate)
{
	_delegate = delegate;
}

const FlashEvent* FlashSprite::getEvent(int anim, int farme)
{
	for( std::vector<FlashEvent>::iterator it = _eventVector.begin(); it != _eventVector.end(); it++)
	{
		if( it->anim == anim && it->frame == farme )
		{
			return &(*it);
		}
	}
	return NULL;
}

void FlashSprite::drawMetaItem(const FlashMetaData& insMetaData, int frame, FlashDataItem* item)
{
	const FlashMetaSymbol& symbol = getFlashMetaSymbol(insMetaData.name(), item->pbData);

	int layerMax = 0;
	for(int layer = symbol.layers_size() - 1; layer >= 0; layer--)
	{
		const FlashMetaLayer& metaLayer = symbol.layers(layer);
		int layerFrame = metaLayer.frames_size();
		if( layerMax < layerFrame )
		{
			layerMax = layerFrame;
		}
	}
	int curFrame = frame % layerMax;
	if( insMetaData.has_singleframe() && insMetaData.singleframe() )
	{
		curFrame = 0;
	}
	else if(insMetaData.has_playonce() && insMetaData.playonce() && frame >= layerMax)
	{
		curFrame = layerMax - 1;
	}

	for(int layer = symbol.layers_size() - 1; layer >= 0; layer--)
	{
		const FlashMetaLayer& metaLayer = symbol.layers(layer);
		int layerFrame = metaLayer.frames_size();
		int curLayerFrame = curFrame;
		if( curLayerFrame > layerFrame - 1)
		{
			continue;
		}
		const FlashMetaFrame& metaFrame = metaLayer.frames(curLayerFrame);

		for(int i = 0; i < metaFrame.symbols_size(); ++i)
		{
			const FlashMetaData& metaData = metaFrame.symbols(i);

			Matrix m = getMatrix(insMetaData);
			Matrix insMat = getMatrix(metaData);
			m.concatenate(insMat);

			FlashMetaData nextMetaData;
			nextMetaData.CopyFrom(metaData);
			nextMetaData.set_ma(m.a);
			nextMetaData.set_mb(m.b);
			nextMetaData.set_mc(m.c);
			nextMetaData.set_md(m.d);
			nextMetaData.set_mtx(m.tx);
			nextMetaData.set_mty(m.ty);

			nextMetaData.set_a( conAlpha(metaData, insMetaData) );
			nextMetaData.set_r( conRed(metaData, insMetaData) );
			nextMetaData.set_g( conGreen(metaData, insMetaData) );
			nextMetaData.set_b( conBlue(metaData, insMetaData) );

			if(insMetaData.has_isbrightness() && insMetaData.isbrightness())
			{
				nextMetaData.set_isbrightness(true);
			}
			drawMetaItem(nextMetaData, curLayerFrame - metaFrame.startframe(), item);
		}

		for(int i = 0; i < metaFrame.bitmaps_size(); ++i)
		{
			const FlashMetaData& metaData = metaFrame.bitmaps(i);
			Matrix m = getMatrix(insMetaData);
			Matrix insMat = getMatrix(metaData);
			m.concatenate(insMat);
			FlashMetaData nextMetaData;
			nextMetaData.CopyFrom(metaData);
			nextMetaData.set_ma(m.a);
			nextMetaData.set_mb(m.b);
			nextMetaData.set_mc(m.c);
			nextMetaData.set_md(m.d);
			nextMetaData.set_mtx(m.tx);
			nextMetaData.set_mty(m.ty);

			nextMetaData.set_a( conAlpha(metaData, insMetaData) );
			nextMetaData.set_r( conRed(metaData, insMetaData) );
			nextMetaData.set_g( conGreen(metaData, insMetaData) );
			nextMetaData.set_b( conBlue(metaData, insMetaData) );

			if(insMetaData.has_isbrightness() && insMetaData.isbrightness())
			{
				nextMetaData.set_isbrightness(true);
			}
			drawMetaBitmap(nextMetaData, item, insMetaData.insname());
		}
	}
}

std::string StringGetFileExt(std::string path)
{
	if( path.rfind('.') == std::string::npos)
	{
		return "";
	}
	else
	{
		std::string str = path.substr(path.rfind('.') + 1);
		return str;
	} 
}

void FlashSprite::drawMetaBitmap(const FlashMetaData& data, FlashDataItem* item, const std::string insName)
{
	std::string name_key = data.name() + insName;


	CCSwfSprite* sprite = getSprite(name_key);
	if( sprite == NULL )
	{
		std::string flashName = getNameFromPathAndName(item->_transName);
		std::string pngName = getNameFromPathAndName(data.name());
		if(StringGetFileExt(pngName).empty())
		{
			pngName += ".png";
		}
		std::string name = flashName + "_";
		name += pngName;
		
		if(!insName.empty())
		{
			CCSpriteFrame* spriteFrame = getChangeSpriteFrame(insName);
			if( spriteFrame != NULL)
			{
				CCSwfSprite* tempSprite = CCSwfSprite::create(spriteFrame);
				tempSprite->retain();
				_changeSwfSpriteVector.push_back(tempSprite);
				sprite = tempSprite;

				CCSize size = CCSizeMake( data.width(), data.height());
				if( size.width == 0 || size.height == 0)
				{
					CCSpriteFrame* sp = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name.c_str());
					size = sp->getOriginalSize();
				}
				sprite->setContentSize(size);
				CCAssert( sprite != NULL, "");
			}
			else
			{
				sprite = CCSwfSprite::spriteWithSpriteFrameName(name.c_str());;
				sprite->retain();
				addSprite(name_key, sprite);
			}
		}
		else
		{
			sprite = CCSwfSprite::spriteWithSpriteFrameName(name.c_str());;
			sprite->retain();
			addSprite(name_key, sprite);
		}
		
	}




	sprite->isUsed = true;

	sprite->setScaleX(m_fScaleX);
	sprite->setScaleY(m_fScaleY);
	sprite->setIsMirror(m_isMirror);
	//sprite->setRotation(m_fRotation);
	sprite->setOpacity(m_nOpacity * data.a());
	
	sprite->setColor(ccc3( data.r()* m_sColor.r, data.g()*m_sColor.g, data.b()*m_sColor.b));

	if( sprite->getTexture()->hasPremultipliedAlpha() )
	{
		if(data.isbrightness())
		{
			ccBlendFunc fun = {GL_ONE, GL_ONE};
			sprite->setBlendFunc(fun);
		}
		else
		{
			ccBlendFunc fun = {GL_ONE, GL_ONE_MINUS_SRC_ALPHA};
			sprite->setBlendFunc(fun);
		}
	}
	else
	{
		if(data.isbrightness())
		{
			ccBlendFunc fun = {GL_SRC_ALPHA, GL_ONE};
			sprite->setBlendFunc(fun);
		}
		else
		{
			ccBlendFunc fun = {GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA};
			sprite->setBlendFunc(fun);
		}
	}
	

	Matrix mat(data.ma(), data.mb(), data.mc(), data.md(), data.mtx(), data.mty() );
	sprite->updateTransform(mat, data, _screenHeight);

	if( sprite->_bl.x < g_left )
	{
		g_left = sprite->_bl.x;
	}
	if( sprite->_br.x > g_right )
	{
		g_right = sprite->_br.x;
	}
	if( sprite->_tr.y > g_top)
	{
		g_top = sprite->_tr.y;
	}
	if( sprite->_br.y < g_bottom)
	{
		g_bottom = sprite->_br.y;
	}

	_displayNode->addChild(sprite);
}


void FlashSprite::prepareSprite()
{
	g_left = 999999999.f;
	g_right = -999999999.f;
	g_top = -999999999.f;
	g_bottom = 999999999.f;
	FlashDataItem* item = FlashCacheMgr::getInstance()->getItemBySimpeName(_name);
	_displayNode->removeAllChildrenWithCleanup(true);
	initSprite();
	const FlashMetaData& data = item->pbData->anims(_curAnimination);
	drawMetaItem(data, _curFrame, item);
}

void FlashSprite::clearChangeInstanceMap()
{
	for(std::map<std::string, CCSpriteFrame*>::iterator swfIt = _changeInstanceMap.begin(); swfIt != _changeInstanceMap.end(); swfIt++)
	{
		(*swfIt).second->release();
	}
	_changeInstanceMap.clear();

	for( int i = 0; i < _changeSwfSpriteVector.size(); ++i)
	{
		_changeSwfSpriteVector[i]->release();
	}
	_changeSwfSpriteVector.clear();

}

void FlashSprite::addChangeInstance(std::string name, CCSpriteFrame* spt)
{
	
	std::map<std::string, CCSpriteFrame*>::iterator it = _changeInstanceMap.find(name);
	if(it == _changeInstanceMap.end())
	{
		spt->retain();
		_changeInstanceMap.insert( std::map<std::string, CCSpriteFrame*>::value_type(name, spt) );
	}
	else
	{
		if(it->second != spt)
		{
			it->second->release();
			spt->retain();
			it->second = spt;
		}
	}
	
	//_changeInstanceMap[name] = spt;
}

void FlashSprite::addFrameEvent(const char* name, int anim, int frame)
{
	FlashEvent event(name, anim, frame);
	_eventVector.push_back(event);
}

void FlashSprite::registerFlashScriptHandler(int handler)
{
	unregisterFlashScriptHandler();
	m_nFlashScriptHandler = handler;
}

void FlashSprite::registerEndAnimationScriptHandler(int handler)
{
	unregisterEndAnimationScriptHandler();
	m_nEndAnimationScriptHandler = handler;
}

void FlashSprite::unregisterEndAnimationScriptHandler()
{
	if (m_nEndAnimationScriptHandler)
	{
		CCScriptEngineManager::sharedManager()->getScriptEngine()->removeScriptHandler(m_nEndAnimationScriptHandler);
		m_nEndAnimationScriptHandler = 0;
	}
}

void FlashSprite::unregisterFlashScriptHandler()
{
	if (m_nFlashScriptHandler)
	{
		CCScriptEngineManager::sharedManager()->getScriptEngine()->removeScriptHandler(m_nFlashScriptHandler);
		m_nFlashScriptHandler = 0;
	}
}


bool FlashSprite::loadFile(std::string name)
{
	_pathName = name;

	char flashNameBuf[256] = {0};
	toFileName(flashNameBuf, name.c_str() );
	_name = flashNameBuf;
	FlashDataItem* item = FlashCacheMgr::getInstance()->getItemAndHold(name);
	if(item == NULL)
	{
		CCLog("warning! flashSprite do not loaded before %s\n", name.c_str());
		item = FlashCacheMgr::getInstance()->createItem(name);
	}

	if( item->pbData->has_screenheight())
	{
		_screenHeight = item->pbData->screenheight();
	}
	_displayNode = CCNode::create();
	addChild(_displayNode);
	return true;
}


void FlashSprite::changeAnimation(int anim)
{
	FlashDataItem* item = FlashCacheMgr::getInstance()->getItemBySimpeName(_name);
	if(NULL == item)
	{
		return;
	}

    if(anim >= item->pbData->anims_size()){
        CCLog("\n\n\nERROR: main acter missing animation (%d)\n\n\n", anim);
        return;
    }
    
	if(_curFrame == 0 && anim == _curAnimination)
	{
		return;	
	}
	
	_curTime = 0;
	_curFrame = 0;
	_curAnimination = anim;
	prepareSprite();
	calculateAnchorPoint();
}

bool FlashSprite::isAnimationExist(int anim)
{
	FlashDataItem* item = FlashCacheMgr::getInstance()->getItemBySimpeName(_name);
	const FlashMetaData& data = item->pbData->anims(anim);
	std::string name = data.name();
	if(name == "")
	{
		return false;
	}
	else
	{
		return true;
	}
}

int FlashSprite::getCurAnimation()
{
	return _curAnimination;
}

void FlashSprite::setCurFrame(int frame)
{
	FlashDataItem* item = FlashCacheMgr::getInstance()->getItemBySimpeName(_name);
	if(NULL == item)
	{
		return;
	}
	const FlashMetaData& data = item->pbData->anims(_curAnimination);
	const FlashMetaSymbol& symbol = getFlashMetaSymbol(data.name(), item->pbData);
	int frame_size = symbol.max_frame();
	if( frame < frame_size)
	{
		_curFrame = frame;
	}
}


void FlashSprite::step(float dt)
{

	FlashDataItem* item = FlashCacheMgr::getInstance()->getItemBySimpeName(_name);
	if(NULL == item)
	{
		return;
	}

	if(!_isRun)
	{
		return;
	}

	int tempCurrentFrame = _curFrame;

	_curTime += dt;

	float rate = 1.f / item->pbData->speed();
	int curFrame = _curTime / rate;
	const FlashMetaData& data = item->pbData->anims(_curAnimination);
	const FlashMetaSymbol& symbol = getFlashMetaSymbol(data.name(), item->pbData);
	int frame_size = symbol.max_frame();
	
	if( !_loop && _curFrame == frame_size -1)
	{
		return;
	}

	if( curFrame >= _curFrame)
	{
		if( curFrame >= frame_size - 1 )
		{
			if( _loop )
			{
				_curTime = 0;
				_curFrame = 0;
			}
			else
			{
				_curFrame = frame_size - 1;
				_curTime -= dt;
			}

			if(_delegate)
			{
				_lock = true;
				_delegate->onAnimationEnd(this);
				_lock = false;
			}

			if(m_nEndAnimationScriptHandler)
			{
				CCLOG("cocos2d : flash callback with animation %d finish , handler  = %d\n", _curAnimination, m_nEndAnimationScriptHandler);
				CCLuaEngine* engine = (CCLuaEngine*)CCScriptEngineManager::sharedManager()->getScriptEngine();
				CCLuaStack* stack = engine->getLuaStack();
				stack->pushInt(_curAnimination);
				stack->executeFunctionByHandler(m_nEndAnimationScriptHandler, 1);
				stack->clean();
			}

		}
		else
		{
			_curFrame = curFrame;
		}
	}
	else
	{
		CCAssert(false, "");
	}



	if( tempCurrentFrame != _curFrame )
	{
		if(m_nFlashScriptHandler)
		{
			while(tempCurrentFrame != _curFrame)
			{
				tempCurrentFrame++;
				if( tempCurrentFrame > frame_size - 1 )
				{
					if(_curFrame < tempCurrentFrame)
					{
						tempCurrentFrame = 0;
					}
					else
					{
						break;
					}
					
				}

				const FlashEvent* event = getEvent(_curAnimination, tempCurrentFrame);
				if( event != NULL)
				{
					CCLuaEngine* engine = (CCLuaEngine*)CCScriptEngineManager::sharedManager()->getScriptEngine();
					CCLuaStack* stack = engine->getLuaStack();
					stack->pushString(event->name.c_str());
					stack->executeFunctionByHandler(m_nFlashScriptHandler, 1);
					stack->clean();
				}
			}
		}

		prepareSprite();
	}
}

void FlashSprite::setAnchorPoint(CCPoint var)
{
	CCAssert(false, "flashSprite anchor point calculate by its self");
	CCNode::setAnchorPoint(var);
	_displayNode->setAnchorPoint(var);
}

//used for lua
void FlashSprite::setPosition(float x,float y)
{
	CCNode::setPosition(x,y);
}

void FlashSprite::setPosition(cocos2d::CCPoint var)
{
	CCNode::setPosition(var);
}

void FlashSprite::setVisible(bool visible)
{
	CCNode::setVisible(visible);
}
void FlashSprite::calculateAnchorPoint()
{
//	FlashDataItem* item = FlashCacheMgr::getInstance()->getItemBySimpeName(_name);
//	if(NULL == item)
//	{
//		return;
//	}
//	setContentSize( CCSize((g_right - g_left)*RESOURCE_SCREEN_TO_LOGIC_SCREEN_SCALE, (g_top - g_bottom)*RESOURCE_SCREEN_TO_LOGIC_SCREEN_SCALE) );
//
//	float anchorX = -g_left;
//	anchorX = anchorX / (g_right - g_left);
//	if(anchorX < 0)
//	{
//		anchorX = 0;
//	}
//	if( anchorX > 1)
//	{
//		anchorX = 1;
//	}
//
//	float anchorY =  -g_bottom;
//	anchorY = anchorY / (g_top - g_bottom);
//	if( anchorY  < 0)
//	{
//		anchorY = 0;
//	}
//	if( anchorY > 1)
//	{
//		anchorY = 1;
//	}
//
//	cocos2d::CCPoint point(anchorX, anchorY);
//	if( ! CCPoint::CCPointEqualToPoint(point, m_tAnchorPoint) ) 
//	{
//		m_tAnchorPoint = point;
//		m_tAnchorPointInPixels = ccp( m_tContentSizeInPixels.width * m_tAnchorPoint.x, m_tContentSizeInPixels.height * m_tAnchorPoint.y );
//		m_bIsTransformDirty = m_bIsInverseDirty = true;
//#ifdef CC_NODE_TRANSFORM_USING_AFFINE_MATRIX
//		m_bIsTransformGLDirty = true;
//#endif
//	}

}

void FlashSprite::setScale(float scale)
{
	if(m_fScaleX == m_fScaleY && m_fScaleX == scale)
	{
		return;
	}
	m_fScaleX = m_fScaleY = scale;
	_displayNode->setScale(scale);
}

void FlashSprite::setScaleX(float scale)
{
	if(m_fScaleX == scale)
	{
		return;
	}
	m_fScaleX = scale;
	_displayNode->setScaleX(scale);
}


void FlashSprite::setScaleY(float scale)
{
	if(m_fScaleY == scale)
	{
		return;
	}
	m_fScaleY = scale;
	_displayNode->setScaleY(scale);
}


//xul
void FlashSprite::visit()
{
	if(m_bHasClipRect)
	{
		glEnable(GL_SCISSOR_TEST);

		//CCPoint displayOrigin = RPccp(m_rClipRect.origin.x, m_rClipRect.origin.y);
		//CCPoint displaySize = RPccp(m_rClipRect.size.width, m_rClipRect.size.height);
		CCPoint displayOrigin = ccp(m_rClipRect.origin.x, m_rClipRect.origin.y);
		CCPoint displaySize = ccp(m_rClipRect.size.width, m_rClipRect.size.height);
		CCDirector::sharedDirector()->getOpenGLView()->setScissorInPoints(displayOrigin.x, displayOrigin.y,displaySize.x, displaySize.y);
		//glScissor(m_rClipRect.origin.x, m_rClipRect.origin.y, m_rClipRect.size.width,  m_rClipRect.size.height );
		_visit();
		glDisable(GL_SCISSOR_TEST);
	}else
	{
		_visit();
	}
}

void FlashSprite::_visit()
{
	if( !isVisible())
	{
		return;
	}

	FlashDataItem* item = FlashCacheMgr::getInstance()->getItemBySimpeName(_name);
	if(NULL == item)
	{
		return;
	}

	if( m_bTransformDirty || m_bInverseDirty )
	{
		_displayNode->setPosition( this->getPosition() );
		_displayNode->setRotation( this->getRotation() );
	}

	_displayNode->visit();
}

void FlashSprite::setIsMirror(bool mirror)
{
	if(m_isMirror == mirror)
	{
		return;
	}
	m_isMirror = mirror; 
	prepareSprite();
}

void FlashSprite::initSprite()
{
	for(std::multimap<std::string, CCSwfSprite*>::iterator it = _spriteMap.begin(); it != _spriteMap.end(); it++)
	{
		(*it).second->isUsed = false;
	}

	for( int i = 0; i < _changeSwfSpriteVector.size(); ++i)
	{
		_changeSwfSpriteVector[i]->release();
	}
	_changeSwfSpriteVector.clear();
}

void FlashSprite::addSprite(const std::string& name, CCSwfSprite* spt)
{
	_spriteMap.insert( std::map<std::string, CCSwfSprite*>::value_type(name, spt) );
}

CCSwfSprite* FlashSprite::getSprite(const std::string& name)
{
	std::multimap<std::string, CCSwfSprite*>::iterator pos;
	for (pos = _spriteMap.lower_bound(name);
		pos != _spriteMap.upper_bound(name); ++pos)
	{
		if(!((*pos).second->isUsed) )
		{
			return (*pos).second;
		}
	}
	return NULL;
}

CCSpriteFrame* FlashSprite::getChangeSpriteFrame(const std::string& name)
{
	std::map<std::string, CCSpriteFrame*>::iterator pos = _changeInstanceMap.find(name);
	if(pos != _changeInstanceMap.end())
	{
		return (*pos).second;
	}
	return NULL;
}

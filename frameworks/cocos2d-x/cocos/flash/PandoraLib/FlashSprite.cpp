#include "FlashSprite.h"
#include <string>
#include <fstream>
#include "FlashUtils.h"
#include "FlashSpriteDelegate.h"
#include "NotificationDirector.h"
#include "CCSwfSprite.h"
#include "FlashCacheMgr.h"
#include "flashMeta.pb.h"
#include "flashMetaEx.pb.h"
#include "CCSwfSprite.h"
#include "Matrix.h"

#include <time.h>

//extern bool pvpMode;

NS_CC_BEGIN

static float g_left = 999999999.f;
static float g_right = -999999999.f;
static float g_top = -999999999.f;
static float g_bottom = 999999999.f;

float FlashSprite::g_hero_monster_scale = 0.95f;
#if (defined USE_FLASH_METADATA_CACHE && USE_FLASH_METADATA_CACHE == 1)

Matrix getMatrix(const FlashMetaDataEx& metaData, std::string name)
{
	Matrix m;
	if(metaData.has_tempmatrix()){
		const FlashMetaMatrixEx& matrixEx = metaData.tempmatrix();
		if( matrixEx.has_ma() ){
			m.a = matrixEx.ma();
		}
		if( matrixEx.has_mb() ){
			m.b = matrixEx.mb();
		}
		if( matrixEx.has_mc() ){
			m.c = matrixEx.mc();
		}
		if( matrixEx.has_md() ){
			m.d = matrixEx.md();
		}
	}
	else
	{
		unsigned int matrixId = (unsigned int)(metaData.matrix_name() / 1000);
		FlashDataItem* item = FlashCacheMgr::getInstance()->getItemBySimpeName(name);
		const FlashMetaMatrixEx& matrixEx = item->pbData->matrixpool(matrixId);
		if( matrixEx.has_ma() ){
			m.a = matrixEx.ma();
		}
		if( matrixEx.has_mb() ){
			m.b = matrixEx.mb();
		}
		if( matrixEx.has_mc() ){
			m.c = matrixEx.mc();
		}
		if( matrixEx.has_md() ){
			m.d = matrixEx.md();
		}
	}

	if( metaData.has_tx() )
	{
		m.tx = metaData.tx();
	}

	if( metaData.has_ty() )
	{
		m.ty = metaData.ty();
	}
	return m;
}
#else
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
#endif




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

#if (defined USE_FLASH_METADATA_CACHE && USE_FLASH_METADATA_CACHE == 1)
float FlashSprite::conAlpha(const FlashMetaDataEx& dataA, const FlashMetaDataEx& dataB)
{
	FlashDataItem* item = FlashCacheMgr::getInstance()->getItemBySimpeName(_name);
	unsigned int matrixIdA = (unsigned int)(dataA.matrix_name() / 1000);
	const FlashMetaMatrixEx& matrixExA = item->pbData->matrixpool(matrixIdA);

	unsigned int matrixIdB = (unsigned int)(dataA.matrix_name() / 1000);
	const FlashMetaMatrixEx& matrixExB = item->pbData->matrixpool(matrixIdA);

	float alphaA = 1.f;
	float alphaB = 1.f;
	if( matrixExA.has_a() )
		alphaA = matrixExA.a();
	if( matrixExB.has_a() )
		alphaB = matrixExB.a();
	return alphaA * alphaB;
}

float conRed(const FlashMetaDataEx& dataA, const FlashMetaDataEx& dataB)
{/*
	float redA = 1.f;
	float redB = 1.f;
	if( dataA.has_r() )
		redA = dataA.r();
	if( dataB.has_r() )
		redB = dataB.r();
	return redA * redB;*/
	return 1;
}

float conGreen(const FlashMetaDataEx& dataA, const FlashMetaDataEx& dataB)
{/*
	float greenA = 1.f;
	float greenB = 1.f;
	if( dataA.has_g() )
		greenA = dataA.g();
	if( dataB.has_g() )
		greenB = dataB.g();
	return greenA * greenB;*/
	return 1;
}

float conBlue(const FlashMetaDataEx& dataA, const FlashMetaDataEx& dataB)
{/*
	float blueA = 1.f;
	float blueB = 1.f;
	if( dataA.has_b() )
		blueA = dataA.b();
	if( dataB.has_b() )
		blueB = dataB.b();
	return blueA * blueB;*/
	return 1;
}
#else
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
#endif



float FlashSprite::s_animationTimeRate = 1.f;

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
,m_isReverse(false)
,_positionOffsetBackTime(0)
,_offsetBackSpeedx(0)
,_offsetBackSpeedy(0)
{
	m_nOpacity = 255;
	m_sColor = ccWHITE;
	m_isMirror = false;
	m_bHasClipRect = false;
	_screenHeight = 960;
	_bgNode = NULL;
	//_flashValid = new FlashValidFlag();
	//_flashValid->_isValid = true;
	_playRate = 1.f;
	m_bTargetEndAnimationFunction = false;
	Node::setCascadeOpacityEnabled(true);
	_firstChangeAnimation = false;
	_isGray = false;
	_isBright = false;
	_red = 0;
	_isGreen = false;
	_isGolden = false;
	_isBurn = false;
	_redDebug = 0;
}

void FlashSprite::setAnimationTimeRate(float rate)
{
	FlashSprite::s_animationTimeRate = rate;
}

FlashSprite::~FlashSprite(void)
{
	if( _bgNode )
	{
		_bgNode->release();
	}
	unregisterFlashScriptHandler();
	unregisterEndAnimationScriptHandler();
	_displayNode->removeAllChildrenWithCleanup(true);
    this->removeAllChildrenWithCleanup(true);

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
		pobSprite->setScale(1);
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
	//_flashValid->_isValid = false;
	//_flashValid->autorelease();
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

#if (defined USE_FLASH_METADATA_CACHE && USE_FLASH_METADATA_CACHE == 1)

void FlashSprite::drawMetaItem(const FlashMetaDataEx& insMetaData, int frame, FlashDataItem* item)
{

	const FlashMetaSymbolEx& symbol = getFlashMetaSymbol(getFlashItemName(insMetaData), item->pbData);

	int layerMax = 0;
	for(int layer = symbol.layers_size() - 1; layer >= 0; layer--)
	{
		const FlashMetaLayerEx& metaLayer = symbol.layers(layer);
		int layerFrame = metaLayer.frames_size();
		if( layerMax < layerFrame )
		{
			layerMax = layerFrame;
		}
	}
	int curFrame = frame % layerMax;
	if( hasSingleframeByMetaData(insMetaData) && singleframeByMetaData(insMetaData) )
	{
		curFrame = 0;
	}
	else if(hasPlayonceByMetaData(insMetaData) && playonceByMetaData(insMetaData) && frame >= layerMax)
	{
		curFrame = layerMax - 1;
	}

	for(int layer = symbol.layers_size() - 1; layer >= 0; layer--)
	{
		const FlashMetaLayerEx& metaLayer = symbol.layers(layer);

		int layerFrame = metaLayer.frames_size();
		int curLayerFrame = curFrame;
		if( curLayerFrame > layerFrame - 1)
		{
			continue;
		}

		const FlashMetaFrameEx& frame = metaLayer.frames(curLayerFrame);

		for(int i = 0; i < frame.symbols_size(); ++i)
		{
			const FlashMetaDataEx& metaData = frame.symbols(i);

			Matrix m = getMatrix(insMetaData, _name);
			Matrix insMat = getMatrix(metaData, _name);
			m.concatenate(insMat);

			FlashMetaDataEx nextMetaData;
			nextMetaData.CopyFrom(metaData);
			FlashMetaMatrixEx* nextMatrix = nextMetaData.mutable_tempmatrix();
			nextMatrix->set_ma(m.a);
			nextMatrix->set_mb(m.b);
			nextMatrix->set_mc(m.c);
			nextMatrix->set_md(m.d);
			nextMetaData.set_tx(m.tx);
			nextMetaData.set_ty(m.ty);

			nextMatrix->set_a( conAlpha(metaData, insMetaData) );
			nextMatrix->set_r( conRed(metaData, insMetaData) );
			nextMatrix->set_g( conGreen(metaData, insMetaData) );
			nextMatrix->set_b( conBlue(metaData, insMetaData) );

			//if(insMetaData.has_isbrightness() && insMetaData.isbrightness())
			//{
			//	nextMetaData.set_isbrightness(true);
			//}
			drawMetaItem(nextMetaData, curLayerFrame - frame.startframe(), item);
		}

		for(int i = 0; i < frame.bitmaps_size(); ++i)
		{
			const FlashMetaDataEx& metaData = frame.bitmaps(i);

			Matrix m = getMatrix(insMetaData, _name);
			Matrix insMat = getMatrix(metaData, _name);
			m.concatenate(insMat);

			FlashMetaDataEx nextMetaData;
			nextMetaData.CopyFrom(metaData);
			FlashMetaMatrixEx* nextMatrix = nextMetaData.mutable_tempmatrix();
			nextMatrix->set_ma(m.a);
			nextMatrix->set_mb(m.b);
			nextMatrix->set_mc(m.c);
			nextMatrix->set_md(m.d);
			nextMetaData.set_tx(m.tx);
			nextMetaData.set_ty(m.ty);

			nextMatrix->set_a( conAlpha(metaData, insMetaData) );
			nextMatrix->set_r( conRed(metaData, insMetaData) );
			nextMatrix->set_g( conGreen(metaData, insMetaData) );
			nextMatrix->set_b( conBlue(metaData, insMetaData) );

			//if(insMetaData.has_isbrightness() && insMetaData.isbrightness())
			//{
			//	nextMetaData.set_isbrightness(true);
			//}
			drawMetaBitmap(nextMetaData, item, getFlashInstanceName(insMetaData));
		}
	}
}
#else

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

			//if(insMetaData.has_isbrightness() && insMetaData.isbrightness())
			//{
			//	nextMetaData.set_isbrightness(true);
			//}
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

			//if(insMetaData.has_isbrightness() && insMetaData.isbrightness())
			//{
			//	nextMetaData.set_isbrightness(true);
			//}
			drawMetaBitmap(nextMetaData, item, insMetaData.insname());
		}
	}
}
#endif

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

#if (defined USE_FLASH_METADATA_CACHE && USE_FLASH_METADATA_CACHE == 1)
void FlashSprite::drawMetaBitmap(const FlashMetaDataEx& data, FlashDataItem* item, const std::string insName)
{
	std::string name_key = getFlashItemName(data) + insName;


	CCSwfSprite* sprite = getSprite(name_key);
	if( sprite == NULL )
	{
		std::string flashName = getNameFromPathAndName(item->_transName);
		std::string pngName = getNameFromPathAndName(getFlashItemName(data));
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

				const FlashMetaMatrixEx& matrix = item->pbData->matrixpool( (int)(data.matrix_name()/1000) );

				CCSize size = CCSizeMake( matrix.width(), matrix.height());
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

	if(!insName.empty())
	{
		CCSpriteFrame* spriteFrame = getChangeSpriteFrame(insName);
		std::string flashName = getNameFromPathAndName(item->_transName);
		std::string pngName = getNameFromPathAndName(getFlashItemName(data));
		if(StringGetFileExt(pngName).empty())
		{
			pngName += ".png";
		}
		std::string name = flashName + "_";
		name += pngName;

		if( spriteFrame != NULL)
		{
			CCSwfSprite* tempSprite = CCSwfSprite::create(spriteFrame);
			tempSprite->retain();
			_changeSwfSpriteVector.push_back(tempSprite);
			sprite = tempSprite;

			const FlashMetaMatrixEx& matrix = item->pbData->matrixpool( (int)(data.matrix_name()/1000) );

			CCSize size = CCSizeMake( matrix.width(), matrix.height());
			if( size.width == 0 || size.height == 0)
			{
				CCSpriteFrame* sp = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name.c_str());
				size = sp->getOriginalSize();
			}
			sprite->setContentSize(size);
			CCAssert( sprite != NULL, "");
		}
	}


	float a = 1.f;
	if(data.has_tempmatrix()){
		a = data.tempmatrix().a();
	}else{
		const FlashMetaMatrixEx& matrixEx = item->pbData->matrixpool((unsigned int)(data.matrix_name() / 1000));
		a = matrixEx.a();
	}
	

	sprite->isUsed = true;

	sprite->setScaleX(_scaleX);
	sprite->setScaleY(_scaleY);
	sprite->setIsMirror(m_isMirror);
	sprite->setOpacity(m_nOpacity * a);
	if(_isBright){
		sprite->setBright(_isBright);
	}else if(_isGray){
		sprite->setGrey(_isGray);
	}else if(_red != 0){
		sprite->setRed(_red);
	}

	if(data.has_tempmatrix()){
		const FlashMetaMatrixEx& matrix = data.tempmatrix();
		Matrix mat(matrix.ma(), matrix.mb(), matrix.mc(), matrix.md(), data.tx(), data.ty() );
		sprite->updateTransform(mat, data, _screenHeight);
	}else{
		const FlashMetaMatrixEx& matrix = item->pbData->matrixpool( (int)(data.matrix_name()/1000) );
		Matrix mat(matrix.ma(), matrix.mb(), matrix.mc(), matrix.md(), data.tx(), data.ty() );
		sprite->updateTransform(mat, data, _screenHeight);
	}
	
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
#else
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

	if(!insName.empty())
	{
		CCSpriteFrame* spriteFrame = getChangeSpriteFrame(insName);
		std::string flashName = getNameFromPathAndName(item->_transName);
		std::string pngName = getNameFromPathAndName(data.name());
		if(StringGetFileExt(pngName).empty())
		{
			pngName += ".png";
		}
		std::string name = flashName + "_";
		name += pngName;

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
	}




	sprite->isUsed = true;

	sprite->setScaleX(_scaleX);
	sprite->setScaleY(_scaleY);
	sprite->setIsMirror(m_isMirror);
	//sprite->setRotation(m_fRotation);
	sprite->setOpacity(m_nOpacity * data.a());
	if(_isBright){
		sprite->setBright(_isBright);
	}else if(_isGray){
		sprite->setGrey(_isGray);
	}else if(_red >= 1){
		sprite->setRed(_red * 20);
	}else if(_isGolden){
		sprite->setGolden();
	}else if(_isBurn){
		sprite->setBurn();
	}else if(_isGreen){
		sprite->setGreen();
	}else{
		sprite->setOriginColor();
	}


	if(_redDebug > 0){
		sprite->setRed(_redDebug);
	}
	
	
	//sprite->setColor(ccc3( data.r()* m_sColor.r, data.g()*m_sColor.g, data.b()*m_sColor.b));

//	if( sprite->getTexture()->hasPremultipliedAlpha() )
//	{
//		if(data.isbrightness())
//		{
//			ccBlendFunc fun = {GL_ONE, GL_ONE};
//			sprite->setBlendFunc(fun);
//		}
//		else
//		{
//			ccBlendFunc fun = {GL_ONE, GL_ONE_MINUS_SRC_ALPHA};
//			sprite->setBlendFunc(fun);
//		}
//	}
//	else
//	{
//		if(data.isbrightness())
//		{
//			ccBlendFunc fun = {GL_SRC_ALPHA, GL_ONE};
//			sprite->setBlendFunc(fun);
//		}
//		else
//		{
//			ccBlendFunc fun = {GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA};
//			sprite->setBlendFunc(fun);
//		}
//	}
	

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
    //this->addChild(sprite);
}
#endif


GLubyte FlashSprite::getOpacity()const
{
	return m_nOpacity;
}

void FlashSprite::setOpacity(unsigned char opacity)
{
	if (m_nOpacity == opacity)
	{
		return;
	}
	m_nOpacity = opacity;
	CCNode::setOpacity(opacity);
	Vector<Node*>& nodeVec = _displayNode->getChildren();
	for (int i = 0; i < nodeVec.size(); i++)
	{
		Node* node = nodeVec.at(i);
		node->setOpacity(m_nOpacity);
	}
}


void FlashSprite::prepareSprite()
{


	g_left = 999999999.f;
	g_right = -999999999.f;
	g_top = -999999999.f;
	g_bottom = 999999999.f;
	FlashDataItem* item = FlashCacheMgr::getInstance()->getItemBySimpeName(_name);
	_displayNode->removeAllChildrenWithCleanup(true);
	if(_bgNode != NULL)
	{
		_displayNode->addChild(_bgNode);
	}
    //this->removeAllChildrenWithCleanup(true);
	initSprite();
#if (defined USE_FLASH_METADATA_CACHE && USE_FLASH_METADATA_CACHE == 1)
	const FlashMetaDataEx& data = item->pbData->anims(_curAnimination);
	drawMetaItem(data, _curFrame, item);
#else
	const FlashMetaData& data = item->pbData->anims(_curAnimination);
	drawMetaItem(data, _curFrame, item);
#endif
	_red--;
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
        ScriptEngineManager::getInstance()->getScriptEngine()->removeScriptHandler(m_nEndAnimationScriptHandler);
		m_nEndAnimationScriptHandler = 0;
	}
}

void FlashSprite::unregisterFlashScriptHandler()
{
	if (m_nFlashScriptHandler)
	{
		ScriptEngineManager::getInstance()->getScriptEngine()->removeScriptHandler(m_nFlashScriptHandler);
		m_nFlashScriptHandler = 0;
	}
}

void FlashSprite::setOffsetPosition(float x, float y, float backTime){
	_displayNode->setPosition(x, y);
	_positionOffsetBackTime = backTime;
	_offsetBackSpeedx = -x / _positionOffsetBackTime;
	_offsetBackSpeedy = -y / _positionOffsetBackTime;
}

#if (defined USE_FLASH_METADATA_CACHE && USE_FLASH_METADATA_CACHE == 1)
bool FlashSprite::loadFile(std::string name)
{
	_pathName = name;

	char flashNameBuf[256] = {0};
	toFileName(flashNameBuf, name.c_str() );
	_name = flashNameBuf;
	FlashDataItem* item = FlashCacheMgr::getInstance()->getItemAndHold(name);
	if(item == NULL)
	{
		//CCLog("warning! flashSprite do not loaded before %s\n", name.c_str());
		item = FlashCacheMgr::getInstance()->createItem(name);
	}

	if( item->pbData->has_screenheight())
	{
		_screenHeight = item->pbData->screenheight();
	}
	_displayNode = Sprite::create();
	addChild(_displayNode);
	return true;
}
#else
bool FlashSprite::loadFile(std::string name)
{
	_pathName = name;

	char flashNameBuf[256] = {0};
	toFileName(flashNameBuf, name.c_str() );
	_name = flashNameBuf;
	FlashDataItem* item = FlashCacheMgr::getInstance()->getItemAndHold(name);
	if(item == NULL)
	{
		//CCLog("warning! flashSprite do not loaded before %s\n", name.c_str());
		item = FlashCacheMgr::getInstance()->createItem(name);
	}

	if( item->pbData->has_screenheight())
	{
		_screenHeight = item->pbData->screenheight();
	}
	_displayNode = Sprite::create();
	addChild(_displayNode);
	return true;
}
#endif

void FlashSprite::changeAnimation(int anim)
{
	FlashDataItem* item = FlashCacheMgr::getInstance()->getItemBySimpeName(_name);
	if(NULL == item)
	{
		return;
	}

    if(anim >= item->pbData->anims_size()){
        CCLog("\n\n\nERROR: main acter missing animation (%d)\n\n\n", anim);
        //return;
		anim = item->pbData->anims_size() - 1;
    }
    
	if(_curFrame == 0 && anim == _curAnimination)
	{
		return;	
	}
	
	_curTime = 0;
	_curAnimination = anim;
	m_bTargetEndAnimationFunction = false;
#if (defined USE_FLASH_METADATA_CACHE && USE_FLASH_METADATA_CACHE == 1)
	const FlashMetaDataEx& data = item->pbData->anims(_curAnimination);
	const FlashMetaSymbolEx& symbol = getFlashMetaSymbol(getFlashItemName(data), item->pbData);
#else
	const FlashMetaData& data = item->pbData->anims(_curAnimination);
	const FlashMetaSymbol& symbol = getFlashMetaSymbol(data.name(), item->pbData);
#endif
	int frame_size = symbol.max_frame();
	
	if(m_isReverse)
	{
		_curFrame = frame_size - 1;
	}
	else
	{
		_curFrame = 0;
	}
	prepareSprite();
	calculateAnchorPoint();
	onAnimationStart();
	_curFrameSize = frame_size;
	_firstChangeAnimation = true;
}



int FlashSprite::getFrameCount()
{
	return _curFrameSize;
}

bool FlashSprite::isAnimationExist(int anim)
{
	FlashDataItem* item = FlashCacheMgr::getInstance()->getItemBySimpeName(_name);
	if(anim < 0 || anim >= item->pbData->anims_size() ){
		return false;
	}

#if (defined USE_FLASH_METADATA_CACHE && USE_FLASH_METADATA_CACHE == 1)
	const FlashMetaDataEx& data = item->pbData->anims(anim);
	std::string name = getFlashItemName(data);
#else
	const FlashMetaData& data = item->pbData->anims(anim);
	std::string name = data.name();
#endif
	
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
	if(_curFrame == frame )
	{
		return;
	}

	FlashDataItem* item = FlashCacheMgr::getInstance()->getItemBySimpeName(_name);
	if(NULL == item)
	{
		return;
	}

#if (defined USE_FLASH_METADATA_CACHE && USE_FLASH_METADATA_CACHE == 1)
	const FlashMetaDataEx& data = item->pbData->anims(_curAnimination);
	const FlashMetaSymbolEx& symbol = getFlashMetaSymbol(getFlashItemName(data), item->pbData);
#else
	const FlashMetaData& data = item->pbData->anims(_curAnimination);
	const FlashMetaSymbol& symbol = getFlashMetaSymbol(data.name(), item->pbData);
#endif
	int frame_size = symbol.max_frame();
	if( frame < frame_size)
	{
		_curFrame = frame;

		if(m_isReverse)
		{
			float rate = 1.f / item->pbData->speed();
			float time = (frame_size - _curFrame) * rate;
			_curTime = time;
		}
		else
		{
			float rate = 1.f / item->pbData->speed();
			float time = _curFrame * rate;
			_curTime = time;
		}
		prepareSprite();
	}
}




void FlashSprite::setPlayRate(float rate)
{
	_playRate = rate;
}
float FlashSprite::getPlayRate()
{
	return _playRate;
}

void FlashSprite::stepReverse(float dt, const FlashDataItem* item)
{
	_curTime += dt;
#if (defined USE_FLASH_METADATA_CACHE && USE_FLASH_METADATA_CACHE == 1)
	const FlashMetaDataEx& data = item->pbData->anims(_curAnimination);
	const FlashMetaSymbolEx& symbol = getFlashMetaSymbol(getFlashItemName(data), item->pbData);
#else
	const FlashMetaData& data = item->pbData->anims(_curAnimination);
	const FlashMetaSymbol& symbol = getFlashMetaSymbol(data.name(), item->pbData);
#endif
	int frame_size = symbol.max_frame();
	int tempCurrentFrame = _curFrame;

	float rate = 1.f / item->pbData->speed();
	int curFrame = (frame_size - 1) - (_curTime / rate);

	_curFrameSize = frame_size;

	if(_curFrame == curFrame)
	{
		return;
	}
	CCAssert(curFrame >= _curFrame, "");


	if( !m_bTargetEndAnimationFunction && curFrame < 0 )
	{
		if( _loop )
		{
			_curTime = 0;
			_curFrame = frame_size - 1;
			m_bTargetEndAnimationFunction = false;
			onAnimationStart();
		}
		else
		{
			m_bTargetEndAnimationFunction = true;
			onAnimationEnd();
			_curFrame = 0;
			_curTime = ( (frame_size - 1) - _curFrame) * rate;
		}

		if(_delegate)
		{
			_lock = true;
			_delegate->onAnimationEnd(this);
			_lock = false;
		}

		if(m_nEndAnimationScriptHandler)
		{
			FlashScriptData scriptData(m_nEndAnimationScriptHandler, this);
			ScriptEvent scriptEvent(kFlashSpriteEvent,(void*)&scriptData);
			ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&scriptEvent);
		}

	}



	if( curFrame < frame_size )
	{
		_curFrame = curFrame;
	}
	else
	{
		return;
	}

	prepareSprite();
}


void FlashSprite::step(float dt)
{
	if(_firstChangeAnimation){
		if (dt > 1.f/30)
		{
			dt = 1.f/30;
			_firstChangeAnimation = false;
		}
	}

	retain();

	float curX = _displayNode->getPosition().x;
	float curY = _displayNode->getPosition().y;
	curX += _offsetBackSpeedx * dt;
	curY += _offsetBackSpeedy * dt;
	_positionOffsetBackTime -= dt;
	if(_positionOffsetBackTime <= 0){
		curX = 0;
		curY = 0;
		_positionOffsetBackTime = 0;
	}
	_displayNode->setPosition(curX, curY);

	dt *= _playRate >= 0 ? _playRate * s_animationTimeRate : -_playRate;
	FlashDataItem* item = FlashCacheMgr::getInstance()->getItemBySimpeName(_name);
	if(NULL == item)
	{
		release();
		return;
	}

	if(!_isRun)
	{
		release();
		return;
	}

	if(m_isReverse)
	{
		stepReverse(dt, item);
		release();
		return;
	}

	_curTime += dt;
	int tempCurrentFrame = _curFrame;

	float rate = 1.f / item->pbData->speed();
	int curFrame = _curTime / rate;
#if (defined USE_FLASH_METADATA_CACHE && USE_FLASH_METADATA_CACHE == 1)
	const FlashMetaDataEx& data = item->pbData->anims(_curAnimination);
	const FlashMetaSymbolEx& symbol = getFlashMetaSymbol(getFlashItemName(data), item->pbData);
#else
	const FlashMetaData& data = item->pbData->anims(_curAnimination);
	const FlashMetaSymbol& symbol = getFlashMetaSymbol(data.name(), item->pbData);
#endif
	int frame_size = symbol.max_frame();
	_curFrameSize = frame_size;

	if(_curFrame == curFrame)
	{
		release();
		return;
	}
	CCAssert(curFrame >= _curFrame, "");


	if( !m_bTargetEndAnimationFunction && curFrame >= frame_size )
	{
		if( _loop )
		{
			_curTime = 0;
			_curFrame = 0;
			m_bTargetEndAnimationFunction = false;
			onAnimationStart();
		}
		else
		{
			onAnimationEnd();
			m_bTargetEndAnimationFunction = true;
			_curFrame = frame_size - 1;
			_curTime = _curFrame * rate;
		}

		if(_delegate)
		{
			_lock = true;
			_delegate->onAnimationEnd(this);
			_lock = false;
		}

		if(m_nEndAnimationScriptHandler)
		{
			FlashScriptData scriptData(m_nEndAnimationScriptHandler, this);
			ScriptEvent scriptEvent(kFlashSpriteEvent,(void*)&scriptData);
			ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&scriptEvent);
		}

	}

	if( curFrame < frame_size )
	{
		_curFrame = curFrame;
	}
	else
	{
		release();
		return;
	}


	if( tempCurrentFrame != _curFrame )
	{
		frameChange(tempCurrentFrame, _curFrame);
		if(m_nFlashScriptHandler)
		{
			while(tempCurrentFrame != _curFrame)
			{
				tempCurrentFrame++;
				if(tempCurrentFrame > _curFrame)
				{
					break;
				}

				if( tempCurrentFrame > frame_size - 1 )
				{
					break;
				}

				const FlashEvent* event = getEvent(_curAnimination, tempCurrentFrame);
				if( event != NULL)
				{
					FlashScriptData scriptData(m_nFlashScriptHandler, this, event->name, event->anim, event->frame);
					ScriptEvent scriptEvent(kFlashSpriteEvent,(void*)&scriptData);
					ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&scriptEvent);
					if (!m_nFlashScriptHandler)
					{
						break;
					}

				}
			}
		}

		prepareSprite();
	}

	release();
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

}


void FlashSprite::setScale(float scale)
{
	
	if (-1 != _pathName.find("hero") || -1 != _pathName.find("monster"))
	{
		scale = scale * FlashSprite::g_hero_monster_scale;
	}
	if(_scaleX == _scaleY && _scaleX == scale)
	{
		return;
	}
	_scaleX = _scaleY = scale;
	
	_displayNode->setScale(scale);
}

void FlashSprite::setScaleX(float scale)
{
	if(_scaleX == scale)
	{
		return;
	}
	_scaleX = scale;
	_displayNode->setScaleX(scale);
}


void FlashSprite::setScaleY(float scale)
{
	if(_scaleY == scale)
	{
		return;
	}
	_scaleY = scale;
	_displayNode->setScaleY(scale);
}

void FlashSprite::setBackGroundNode( Node* bgNode)
{
	_bgNode = bgNode;
	_bgNode->retain();
}

Node* FlashSprite::getBackGroundNode()
{
	return _bgNode;
}

void FlashSprite::setReverse(bool reverse)
{
	m_isReverse = reverse;
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
	std::multimap<std::string, CCSwfSprite*>::iterator lower_bound = _spriteMap.lower_bound(name);
	std::multimap<std::string, CCSwfSprite*>::iterator upper_bound = _spriteMap.upper_bound(name);
	for (pos = lower_bound;
		pos != upper_bound; ++pos)
	{
		if(!((*pos).second->isUsed) )
		{
			return (*pos).second;
		}
	}
	return NULL;
}
//CCSwfSprite* FlashSprite::getSprite(const std::string& name)
//{
//	std::multimap<std::string, CCSwfSprite*>::iterator pos;
//	for (pos = _spriteMap.lower_bound(name);
//		pos != _spriteMap.upper_bound(name); ++pos)
//	{
//		if(!((*pos).second->isUsed) )
//		{
//			return (*pos).second;
//		}
//	}
//	return NULL;
//}

CCSpriteFrame* FlashSprite::getChangeSpriteFrame(const std::string& name)
{
	std::map<std::string, CCSpriteFrame*>::iterator pos = _changeInstanceMap.find(name);
	if(pos != _changeInstanceMap.end())
	{
		return (*pos).second;
	}
	return NULL;
}

#if (defined USE_FLASH_METADATA_CACHE && USE_FLASH_METADATA_CACHE == 1)

std::string FlashSprite::getFlashItemName(const FlashMetaDataEx& metaData){
	FlashDataItem* item = FlashCacheMgr::getInstance()->getItem(_name);
	std::string name = item->pbData->namepool( metaData.matrix_name() % 1000 );
	return name;
}

std::string FlashSprite::getFlashInstanceName(const FlashMetaDataEx& metaData){
	FlashDataItem* item = FlashCacheMgr::getInstance()->getItem(_name);
	if (metaData.has_insnameid())
	{
		return item->pbData->namepool( metaData.insnameid() );
	}
	return "";
}

bool FlashSprite::hasSingleframeByMetaData(const FlashMetaDataEx& metaData){
	FlashDataItem* item = FlashCacheMgr::getInstance()->getItem(_name);
	const FlashMetaMatrixEx& matrix = item->pbData->matrixpool( metaData.matrix_name() / 1000 );
	return matrix.has_singleframe();
}

bool FlashSprite::singleframeByMetaData(const FlashMetaDataEx& metaData){
	FlashDataItem* item = FlashCacheMgr::getInstance()->getItem(_name);
	const FlashMetaMatrixEx& matrix = item->pbData->matrixpool( metaData.matrix_name() / 1000 );
	return matrix.singleframe();
}

bool FlashSprite::hasPlayonceByMetaData(const FlashMetaDataEx& metaData){
	FlashDataItem* item = FlashCacheMgr::getInstance()->getItem(_name);
	const FlashMetaMatrixEx& matrix = item->pbData->matrixpool( metaData.matrix_name() / 1000 );
	return matrix.has_playonce();
}

bool FlashSprite::playonceByMetaData(const FlashMetaDataEx& metaData){
	FlashDataItem* item = FlashCacheMgr::getInstance()->getItem(_name);
	const FlashMetaMatrixEx& matrix = item->pbData->matrixpool( metaData.matrix_name() / 1000 );
	return matrix.playonce();
}

#endif


NS_CC_END
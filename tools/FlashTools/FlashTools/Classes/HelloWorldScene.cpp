#include "HelloWorldScene.h"
#include "DOMDocument.h"
#include "utils.h"
#include "DataLogicMgr.h"
#include "DOMDocument.h"
#include "DOMSymbolItem.h"
#include "DOMLayer.h"
#include "DOMFrame.h"
#include "FlashCCSprite.h"
#include "DOMSymbolItem.h"
#include "BoxFlashInfo.pb.h"
#include <hash_map>

//bool isUserFlashMeta = true;

USING_NS_CC;
bool isFrameMetaDataSame(const FlashMetaData* a, const FlashMetaData* b);

CCScene* HelloWorld::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::node();
	
	// 'layer' is an autorelease object
	HelloWorld *layer = HelloWorld::node();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

extern LPTSTR g_lpCmdLine;

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	if ( !CCLayer::init() )
	{
		return false;
	}

	CCSprite* spt = CCSprite::spriteWithFile("bg.png");
	spt->setPosition(ccp(320, 480));
	spt->setScale(4.f);
	addChild(spt);

	m_bIsTouchEnabled = true;
	CCMenuItemImage *pCloseItem = CCMenuItemImage::itemFromNormalImage(
										"CloseNormal.png",
										"CloseSelected.png",
										this,
										menu_selector(HelloWorld::menuCloseCallback) );
	pCloseItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - 20, CCDirector::sharedDirector()->getWinSize().height - 20) );

	CCMenuItemImage *pLeftItem = CCMenuItemImage::itemFromNormalImage(
		"LeftNormal.png",
		"LeftSelected.png",
		this,
		menu_selector(HelloWorld::menuLeftCallback) );
	pLeftItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width/2 - 120, CCDirector::sharedDirector()->getWinSize().height - 20) );

	CCMenuItemImage *pRightItem = CCMenuItemImage::itemFromNormalImage(
		"RightNormal.png",
		"RightSelected.png",
		this,
		menu_selector(HelloWorld::menuRightCallback) );
	pRightItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width/2 + 120, CCDirector::sharedDirector()->getWinSize().height - 20) );


	const int kPlayPosition = 200;

	CCMenuItemImage *pUpItem = CCMenuItemImage::itemFromNormalImage(
		"UpNormal.png",
		"UpSelected.png",
		this,
		menu_selector(HelloWorld::menuUpCallback) );
	pUpItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - 40, kPlayPosition + 80) );


	CCMenuItemImage *pDownItem = CCMenuItemImage::itemFromNormalImage(
		"DownNormal.png",
		"DownSelected.png",
		this,
		menu_selector(HelloWorld::menuDownCallback) );
	pDownItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width -40, kPlayPosition - 80) );



	_stopMenuItem = CCMenuItemImage::itemFromNormalImage(
		"StopNormal.png",
		"StopSelected.png",
		this,
		menu_selector(HelloWorld::menuStopCallback) );
	_stopMenuItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - 40, kPlayPosition -15) );


	_playMenuItem = CCMenuItemImage::itemFromNormalImage(
		"PlayNormal.png",
		"PlaySelected.png",
		this,
		menu_selector(HelloWorld::menuPlayCallback) );
	_playMenuItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width -40, kPlayPosition -15) );




	CCMenu* pMenu = CCMenu::menuWithItems(pCloseItem, pLeftItem, pRightItem, pUpItem, pDownItem, _playMenuItem, _stopMenuItem, NULL);
	pMenu->setPosition( CCPointZero );
	this->addChild(pMenu, 1);

	if( DataLogicMgr::getInstance()->isLoadXml() )
	{
		DataLogicMgr::getInstance()->loadXml( getDataLogicMgr()->getPath() );
	}
	schedule(schedule_selector(HelloWorld::step), 0.0417f);

	DOMDocument* doc = getDataLogicMgr()->getDocument();
	std::string name = doc->getAnimationName(0);

	_animationNameLabel = cocos2d::CCLabelTTF::labelWithString(name.c_str(), "", 24);
	_animationNameLabel->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width/2, 20 ));
	addChild(_animationNameLabel);
	
	_curFrameLabel = cocos2d::CCLabelTTF::labelWithString("0", "", 24);
	_curFrameLabel->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width -40, kPlayPosition + 25) );
	addChild(_curFrameLabel);
	

	_maxAniminationCount = DataLogicMgr::getInstance()->getMaxAnimination();
	_curAnimination = 0;
	_curFrame = 0;

	_isPlaying = true;
	_playMenuItem->setIsVisible(false);
#if (defined USE_FLASH_METADATA_CACHE && USE_FLASH_METADATA_CACHE == 1)
	packCacheMeta();
#else
	packNewMeta();
#endif
	packNewMetaBox();

	return true;
}

//FlashMetaData* g_lastFrameMetaData = NULL;
FlashMeta* g_flashMeta = new FlashMeta();

#if (defined USE_FLASH_METADATA_CACHE && USE_FLASH_METADATA_CACHE == 1)
FlashMetaEx* g_flashMetaEx = new FlashMetaEx();


std::hash_map<std::string, int> _addStringHashMap;
int addString(std::string string){
	auto it = _addStringHashMap.find(string);
	if(it != _addStringHashMap.end()){
		return it->second;
	}
	g_flashMetaEx->add_namepool(string);
	int index = g_flashMetaEx->namepool_size() - 1;
	_addStringHashMap.insert(std::pair<std::string, int>(string, index));
	return index;
}

std::hash_map<std::string, int> _addMatrixHashMap;

int addMatrix(const instanceData& insData, bool isBitMap = false){
	FlashMetaMatrixEx* tempData = new FlashMetaMatrixEx();
	if(insData.matrix.a != 1)
		tempData->set_ma(insData.matrix.a);

	if(insData.matrix.b != 0)
		tempData->set_mb(insData.matrix.b);

	if(insData.matrix.c != 0)
		tempData->set_mc(insData.matrix.c);

	if(insData.matrix.d != 1)
		tempData->set_md(insData.matrix.d);

	//if(insData.matrix.tx != 0)
	//	tempData->set_mtx(insData.matrix.tx);

	//if(insData.matrix.ty != 0)
	//	tempData->set_mty(insData.matrix.ty);

	if(insData.a != 1)
		tempData->set_a(insData.a);
	if(insData.r != 1)
		tempData->set_a(insData.r);
	if(insData.g != 1)
		tempData->set_a(insData.g);
	if(insData.b != 1)
		tempData->set_a(insData.b);

	if(insData.playOnce)
		tempData->set_playonce(true);
	if(insData.singleFrame)
		tempData->set_singleframe(true);
	//if(insData.isBrightness)
	//	tempData->set_isbrightness(true);



	if(isBitMap)
	{
		DOMDocument* doc = getDataLogicMgr()->getDocument();
		EdgeData edata = doc->getEdgeData( insData.resName );
		if(edata.width != 0)
			tempData->set_width(edata.width);
		if(edata.height != 0)
			tempData->set_height(edata.height);
	}

	std::string tempStr = tempData->SerializeAsString();
	auto it = _addMatrixHashMap.find(tempStr);
	if(it != _addMatrixHashMap.end()){
		return it->second;
	}
	
	FlashMetaMatrixEx* matrix = g_flashMetaEx->add_matrixpool();
	matrix->CopyFrom(*tempData);
	delete tempData;

	int index = g_flashMetaEx->matrixpool_size() - 1;
	_addMatrixHashMap.insert(std::pair<std::string, int>(tempStr, index));
	return index;
}
/*
std::hash_map<std::string, int> _addTxTyHashMap;
int addTxTy(float tx, float ty){
	char txbuf[16] = {0};
	char tybuf[16] = {0};
	sprintf(txbuf, "%f", tx);
	sprintf(tybuf, "%f", ty);
	std::string key = txbuf;
	key += tybuf;
	auto it = _addTxTyHashMap.find(key);
	if(it != _addTxTyHashMap.end()){
		return it->second;
	}
	
	g_flashMetaEx->add_txpool(tx);
	g_flashMetaEx->add_typool(ty);
	int index = g_flashMetaEx->txpool_size() - 1;
	_addTxTyHashMap.insert(std::pair<std::string, int>(key, index));
	return index;
}
*/
void setFlashMetaDataEx(FlashMetaDataEx* data, const instanceData& insData, bool isBitMap = false)
{
	//FlashMetaDataEx* tempData = new FlashMetaDataEx();
	int nameId = addString(insData.resName);
	unsigned int matrixId = addMatrix(insData) * 1000;
	unsigned int matrix_name = matrixId + nameId;
	data->set_matrix_name(matrix_name);

	if( insData.matrix.tx != 0 || insData.matrix.ty != 0 ){
		//int txtyId = addTxTy(insData.matrix.tx, insData.matrix.ty);
		//data->set_txtyid(txtyId);
		//data->set_txtyid((int)data);
		data->set_tx(insData.matrix.tx);
		data->set_ty(insData.matrix.ty);
	}

	if(!insData.insName.empty())
	{
		data->set_insnameid( addString(insData.insName.c_str()) );
	}
}


#endif

void setFlashMetaData(FlashMetaData* data, const instanceData& insData, bool isBitMap = false)
{
	FlashMetaData* tempData = new FlashMetaData();
	tempData->set_name(insData.resName);
	if(insData.matrix.a != 1)
		tempData->set_ma(insData.matrix.a);

	if(insData.matrix.b != 0)
		tempData->set_mb(insData.matrix.b);

	if(insData.matrix.c != 0)
		tempData->set_mc(insData.matrix.c);

	if(insData.matrix.d != 1)
		tempData->set_md(insData.matrix.d);

	if(insData.matrix.tx != 0)
		tempData->set_mtx(insData.matrix.tx);

	if(insData.matrix.ty != 0)
		tempData->set_mty(insData.matrix.ty);

	if(insData.a != 1)
		tempData->set_a(insData.a);
	if(insData.r != 1)
		tempData->set_a(insData.r);
	if(insData.g != 1)
		tempData->set_a(insData.g);
	if(insData.b != 1)
		tempData->set_a(insData.b);

	if(insData.playOnce)
		tempData->set_playonce(true);
	if(insData.singleFrame)
		tempData->set_singleframe(true);
	if(insData.isBrightness)
		tempData->set_isbrightness(true);

	if(!insData.insName.empty())
	{
		tempData->set_insname( insData.insName.c_str() );
	}

	if(isBitMap)
	{
		DOMDocument* doc = getDataLogicMgr()->getDocument();
		EdgeData edata = doc->getEdgeData( insData.resName );
		if(edata.width != 0)
			tempData->set_width(edata.width);
		if(edata.height != 0)
			tempData->set_height(edata.height);

		//if(edata.left != 0)
		//	tempData->set_left(edata.left);
		//if(edata.right != 0)
		//	tempData->set_right(edata.right);
		//if(edata.top != 0)
		//	tempData->set_top(edata.top);
		//if(edata.button != 0)
		//	tempData->set_button(edata.button);
	}
	
	//if(g_lastFrameMetaData != NULL && isFrameMetaDataSame(g_lastFrameMetaData, tempData))
	//{
	//	FlashMetaData* tempTempData = new FlashMetaData();
	//	tempTempData->set_name(insData.resName);
	//	tempTempData->set_samewithprev(true);
	//	data->CopyFrom(*tempTempData);	
	//	delete tempTempData;
	//}
	//else
	//{
		//tempData->set_samewithprev(false);
		data->CopyFrom(*tempData);	
	//}
	delete tempData;
	//g_lastFrameMetaData->CopyFrom(*data);
}


BoxFlashInfo* g_boxFlashInfo = new BoxFlashInfo();

#if (defined USE_FLASH_METADATA_CACHE && USE_FLASH_METADATA_CACHE == 1)

std::hash_map<std::string, int> _addFlashMetaFrameHashMap;
int addFlashMetaFrameEx(FlashMetaFrameEx* frameEx){
	std::string tempStr = frameEx->SerializeAsString();
	auto it = _addFlashMetaFrameHashMap.find(tempStr);
	if(it != _addFlashMetaFrameHashMap.end()){
		return it->second;
	}


	//FlashMetaFrameEx* frame = g_flashMetaEx->add_framepool();
	//frame->CopyFrom(*frameEx);
	//int index = g_flashMetaEx->framepool_size() - 1;
	//_addFlashMetaFrameHashMap.insert(std::pair<std::string, int>(tempStr, index));

	return 0;
}


void packCacheMeta()
{

	DOMDocument* doc = getDataLogicMgr()->getDocument();
	g_flashMetaEx->set_speed(doc->getFlashRate());
	g_flashMetaEx->set_screenheight( getDataLogicMgr()->getDocument()->getScreenHeight() );
	std::vector<DOMSymbolItem*> itemVector = doc->getSymbolItemVector();
	for(int i = 0; i < itemVector.size(); ++i)
	{
		DOMSymbolItem* item = itemVector[i];
		FlashMetaSymbolEx* symbol = g_flashMetaEx->add_symbols();
		symbol->set_name(item->getName());
		symbol->set_max_frame(item->getFrameCount());
		for(int layerIndex = 0; layerIndex < item->getDOMLayerCount(); ++layerIndex)
		{
			const DOMLayer* layer = item->getDOMLayer(layerIndex);
			if(layer->IsBoxLayer())
			{
				continue;
			}

			FlashMetaLayerEx* metaLayer = symbol->add_layers();

			for(int frameIndex = 0; frameIndex < layer->getFrameCount(); ++frameIndex)
			{
				FrameData frameData = layer->getFrameData(frameIndex);
				if(frameData.instanceBitMapVector.size() != 0)
				{
					//CCAssert(frameData.instanceSymbolVector.size() == 0);
				}
				if(frameData.instanceSymbolVector.size() != 0)
				{
					//CCAssert(frameData.instanceBitMapVector.size() == 0);
				}

				FlashMetaFrameEx* tempFrameEx = new FlashMetaFrameEx();
				for(int symIndex = 0; symIndex < frameData.instanceSymbolVector.size(); ++ symIndex)
				{
					FlashMetaDataEx* metaData = tempFrameEx->add_symbols();
					setFlashMetaDataEx(metaData, frameData.instanceSymbolVector[symIndex]);
				}
				for(int mapIndex = 0; mapIndex < frameData.instanceBitMapVector.size(); ++mapIndex)
				{
					FlashMetaDataEx* metaData = tempFrameEx->add_bitmaps();
					setFlashMetaDataEx(metaData, frameData.instanceBitMapVector[mapIndex], true);
				}

				if(frameData.startFrame != 0){
					tempFrameEx->set_startframe(frameData.startFrame);
				}
				
				tempFrameEx->CheckInitialized();
				//metaLayer->add_frames( addFlashMetaFrameEx(tempFrameEx) );
				FlashMetaFrameEx* frame = metaLayer->add_frames();
				frame->CopyFrom(*tempFrameEx);
				delete tempFrameEx;

			}

		}
	}


	for(int i= 0; i < doc->getAnimationCount(); ++i)
	{
		instanceData data = doc->getAnimationData(i);

		FlashMetaDataEx* metaData = g_flashMetaEx->add_anims();
		setFlashMetaDataEx(metaData, data);
		
	}

	for(int i = 0; i < doc->_mediaDataVector.size(); ++i)
	{
		s_media_data data = doc->_mediaDataVector[i];
		g_flashMetaEx->add_bitmapnames(data.name);
	}


}


#endif


void packNewMeta()
{
	DOMDocument* doc = getDataLogicMgr()->getDocument();
	g_flashMeta->set_speed(doc->getFlashRate());
	g_flashMeta->set_screenheight( getDataLogicMgr()->getDocument()->getScreenHeight() );
	std::vector<DOMSymbolItem*> itemVector = doc->getSymbolItemVector();
	for(int i = 0; i < itemVector.size(); ++i)
	{
		DOMSymbolItem* item = itemVector[i];
		FlashMetaSymbol* symbol = g_flashMeta->add_symbols();
		symbol->set_name(item->getName());
		symbol->set_max_frame(item->getFrameCount());
		for(int layerIndex = 0; layerIndex < item->getDOMLayerCount(); ++layerIndex)
		{
			const DOMLayer* layer = item->getDOMLayer(layerIndex);
			if(layer->IsBoxLayer())
			{
				continue;
			}

			FlashMetaLayer* metaLayer = symbol->add_layers();

			for(int frameIndex = 0; frameIndex < layer->getFrameCount(); ++frameIndex)
			{
				FrameData frameData = layer->getFrameData(frameIndex);
				if(frameData.instanceBitMapVector.size() != 0)
				{
					//CCAssert(frameData.instanceSymbolVector.size() == 0);
				}
				if(frameData.instanceSymbolVector.size() != 0)
				{
					//CCAssert(frameData.instanceBitMapVector.size() == 0);
				}


				FlashMetaFrame* metaFrame = metaLayer->add_frames();
				cocos2d::CCLog("packNewMeta symbolId:%d       layerId:%d       frameId:%d        startFrame:%d\n", i, layerIndex, frameIndex, frameData.startFrame);
				metaFrame->set_startframe(frameData.startFrame);

				//CC_SAFE_DELETE(g_lastFrameMetaData);
				//g_lastFrameMetaData = new FlashMetaData();
				for(int symIndex = 0; symIndex < frameData.instanceSymbolVector.size(); ++ symIndex)
				{
					FlashMetaData* metaData = metaFrame->add_symbols();
					setFlashMetaData(metaData, frameData.instanceSymbolVector[symIndex]);
				}

				//CC_SAFE_DELETE(g_lastFrameMetaData);
				//g_lastFrameMetaData = new FlashMetaData();
				for(int mapIndex = 0; mapIndex < frameData.instanceBitMapVector.size(); ++mapIndex)
				{
					FlashMetaData* metaData = metaFrame->add_bitmaps();
					setFlashMetaData(metaData, frameData.instanceBitMapVector[mapIndex], true);
				}

			}

		}
	}


	for(int i= 0; i < doc->getAnimationCount(); ++i)
	{
		instanceData data = doc->getAnimationData(i);
		Matrix m = data.matrix;
		FlashMetaData* metaData = g_flashMeta->add_anims();
		metaData->set_name(data.resName);
		//metaData->set_samewithprev(false);
		metaData->set_ma(m.a);
		metaData->set_mb(m.b);
		metaData->set_mc(m.c);
		metaData->set_md(m.d);
		metaData->set_mtx(m.tx);
		metaData->set_mty(m.ty);

		metaData->set_a( data.a );
		metaData->set_r( data.r );
		metaData->set_g( data.g );
		metaData->set_b( data.b );

		if(data.isBrightness )
		{
			metaData->set_isbrightness(true);
		}
		
	}

	for(int i = 0; i < doc->_mediaDataVector.size(); ++i)
	{
		s_media_data data = doc->_mediaDataVector[i];
		g_flashMeta->add_bitmapnames(data.name);
	}
	

}


DOMSymbolItem* getDomSymbolItemByName(std::string name)
{
	DOMDocument* doc = getDataLogicMgr()->getDocument();
	std::vector<DOMSymbolItem*> itemVector = doc->getSymbolItemVector();
	for(int i = 0; i < itemVector.size(); ++i)
	{
		DOMSymbolItem* item = itemVector[i];
		std::string itemName = item->getName().substr(0, item->getName().length()-4);
		if(itemName == name)
		{
			return item;
		}
	}
	assert(false);
	return NULL;
}

void setTrans(BoxTrans* trans, Matrix m)
{
	if (m.a != 0){trans->set_ma(m.a);}
	if (m.b != 0){trans->set_mb(m.b);}
	if (m.c != 0){trans->set_mc(m.c);}
	if (m.d != 0){trans->set_md(m.d);}
	if (m.tx != 0){trans->set_mtx(m.tx);}
	if (m.ty != 0){trans->set_mty(m.ty);}
}

void packNewMetaBox()
{
	DOMDocument* doc = getDataLogicMgr()->getDocument();
	std::vector<DOMSymbolItem*> itemVector = doc->getSymbolItemVector();

	for(int i= 0; i < doc->getAnimationCount(); ++i)
	{
		instanceData data = doc->getAnimationData(i);
		
		BoxActionInfo* actionInfo = g_boxFlashInfo->add_actioninfo();
		Matrix m = data.matrix;
		BoxTrans* aniTran = actionInfo->mutable_trans();
		setTrans(aniTran, m);
		actionInfo->set_actionname(data.resName);
		if(data.resName == "")
		{
			continue;
		}
		DOMSymbolItem* item = getDomSymbolItemByName(data.resName);
		for(int layerIndex = 0; layerIndex < item->getDOMLayerCount(); ++layerIndex)
		{
			const DOMLayer* layer = item->getDOMLayer(layerIndex);
			for(int attacIndex = 0; attacIndex < layer->_BoxKeyFrameAttack.size(); ++attacIndex)
			{
				BoxKeyFrame keyFrame = layer->_BoxKeyFrameAttack[attacIndex];
				BoxDomFrame* boxDomFrame = actionInfo->add_attackdomframe();
				boxDomFrame->set_startframe(keyFrame.start);
				boxDomFrame->set_duration(keyFrame.duration);
				for(int tranIndex = 0; tranIndex < keyFrame.tranMatrix.size(); ++tranIndex)
				{
					Matrix mm = keyFrame.tranMatrix[tranIndex];
					BoxTrans* trans = boxDomFrame->add_trans();
					setTrans(trans, mm);
				}
			}
			
			for(int hurtIndex = 0; hurtIndex < layer->_BoxKeyFrameHurt.size(); ++hurtIndex)
			{
				BoxKeyFrame keyFrame = layer->_BoxKeyFrameHurt[hurtIndex];
				BoxDomFrame* boxDomFrame = actionInfo->add_hurtdomframe();
				boxDomFrame->set_startframe(keyFrame.start);
				boxDomFrame->set_duration(keyFrame.duration);
				for(int tranIndex = 0; tranIndex < keyFrame.tranMatrix.size(); ++tranIndex)
				{
					Matrix mm = keyFrame.tranMatrix[tranIndex];
					BoxTrans* trans = boxDomFrame->add_trans();
					setTrans(trans, mm);
				}
			}

		}

	}

}


void HelloWorld::drawBitmap(Matrix transMatrix, instanceData insData)
{
	if( insData.resName == "01.png")
	{
		int qwer = 1234;
		CCLog( "curFrame=%d  alpha %f", _curFrame, insData.a);
	}

	DOMDocument* doc = getDataLogicMgr()->getDocument();
	FlashCCSprite* sprite = doc->getSprite(insData.resName);
	sprite->setPositionInPixels(_curPoint);
	transMatrix.concatenate(insData.matrix);
	sprite->updateVertex( transMatrix );
	sprite->updateColor(insData.a, insData.r, insData.g, insData.b);
	if(insData.isBrightness)
	{
		ccBlendFunc oldFun = sprite->getBlendFunc();
		ccBlendFunc fun = {GL_ONE, GL_ONE};
		sprite->setBlendFunc(fun);
		sprite->draw();
		sprite->setBlendFunc(oldFun);
	}
	else
	{
		sprite->draw();
	}
}

void HelloWorld::drawSymbolItem1(instanceData insData, int frame)
{
	if(insData.resName == "")
	{
		return;
	}

	DOMDocument* doc = getDataLogicMgr()->getDocument();
	const DOMSymbolItem* symbol = doc->getSymbolItem(insData.resName.c_str());

	int layerMax = 0;
	for(int layer = symbol->getDOMLayerCount() - 1; layer >= 0; layer--)
	{
		const DOMLayer* domLayer = symbol->getDOMLayer(layer);
		int layerFrame = domLayer->getFrameCount();
		if( layerMax < layerFrame )
		{
			layerMax = layerFrame;
		}
	}
	int curFrame = frame % layerMax;
	if( insData.singleFrame )
	{
		curFrame = 0;
	}
	else if(insData.playOnce && frame >= layerMax)
	{
		curFrame = layerMax - 1;
	}

	for(int layer = symbol->getDOMLayerCount() - 1; layer >= 0; layer--)
	{
		const DOMLayer* domLayer = symbol->getDOMLayer(layer);
		int layerFrame = domLayer->getFrameCount();
		int curLayerFrame = curFrame;
		if( curLayerFrame > layerFrame - 1)
		{
			//curLayerFrame = layerFrame - 1;
			continue;
		}

		FrameData frameData = domLayer->getFrameData(curLayerFrame);

		for(int i = 0; i < frameData.instanceSymbolVector.size(); ++i)
		{
			instanceData symbolInsData = frameData.instanceSymbolVector[i];
			Matrix m = insData.matrix;
			m.concatenate(symbolInsData.matrix);
			symbolInsData.matrix = m;
			symbolInsData.a *= insData.a;
			symbolInsData.r *= insData.r;
			symbolInsData.g *= insData.g;
			symbolInsData.b *= insData.b;
			if( insData.isBrightness )
			{
				symbolInsData.isBrightness = true;
			}
			drawSymbolItem2(symbolInsData, curLayerFrame - frameData.startFrame);
		}

		for(int i = 0; i < frameData.instanceBitMapVector.size(); ++i)
		{
			instanceData bitmapIns = frameData.instanceBitMapVector[i];
			bitmapIns.a *= insData.a;
			bitmapIns.r *= insData.r;
			bitmapIns.g *= insData.g;
			bitmapIns.b *= insData.b;
			if( insData.isBrightness )
			{
				bitmapIns.isBrightness = true;
			}
			drawBitmap(insData.matrix, bitmapIns);
		}
	}
}

void HelloWorld::drawSymbolItem2(instanceData insData, int frame)
{
	DOMDocument* doc = getDataLogicMgr()->getDocument();
	const DOMSymbolItem* symbol = doc->getSymbolItem(insData.resName.c_str());

	int layerMax = 0;
	for(int layer = symbol->getDOMLayerCount() - 1; layer >= 0; layer--)
	{
		const DOMLayer* domLayer = symbol->getDOMLayer(layer);
		int layerFrame = domLayer->getFrameCount();
		if( layerMax < layerFrame )
		{
			layerMax = layerFrame;
		}
	}

	int curFrame = frame % layerMax;
	if( insData.singleFrame )
	{
		curFrame = 0;
	}
	else if(insData.playOnce && frame >= layerMax )
	{
		curFrame = layerMax - 1;
	}

	for(int layer = symbol->getDOMLayerCount() - 1; layer >= 0; layer--)
	{
		const DOMLayer* domLayer = symbol->getDOMLayer(layer);
		int layerFrame = domLayer->getFrameCount();
		int curLayerFrame = curFrame;
		if( curLayerFrame > layerFrame - 1)
		{
			continue;
		}

		FrameData frameData = domLayer->getFrameData(curLayerFrame);

		for(int i = 0; i < frameData.instanceSymbolVector.size(); ++i)
		{
			instanceData symbolInsData = frameData.instanceSymbolVector[i];
			Matrix m = insData.matrix;
			m.concatenate(symbolInsData.matrix);
			symbolInsData.matrix = m;
			symbolInsData.a *= insData.a;
			symbolInsData.r *= insData.r;
			symbolInsData.g *= insData.g;
			symbolInsData.b *= insData.b;
			if( insData.isBrightness )
			{
				symbolInsData.isBrightness = true;
			}
			drawSymbolItem3(symbolInsData, curLayerFrame - frameData.startFrame);
		}

		for(int i = 0; i < frameData.instanceBitMapVector.size(); ++i)
		{
			instanceData bitmapIns = frameData.instanceBitMapVector[i];
			bitmapIns.a *= insData.a;
			bitmapIns.r *= insData.r;
			bitmapIns.g *= insData.g;
			bitmapIns.b *= insData.b;
			if( insData.isBrightness )
			{
				bitmapIns.isBrightness = true;
			}
			drawBitmap(insData.matrix, bitmapIns);
		}
	}
}

void HelloWorld::drawSymbolItem3(instanceData insData, int frame)
{
	DOMDocument* doc = getDataLogicMgr()->getDocument();
	const DOMSymbolItem* symbol = doc->getSymbolItem(insData.resName.c_str());

	int layerMax = 0;
	for(int layer = symbol->getDOMLayerCount() - 1; layer >= 0; layer--)
	{
		const DOMLayer* domLayer = symbol->getDOMLayer(layer);
		int layerFrame = domLayer->getFrameCount();
		if( layerMax < layerFrame )
		{
			layerMax = layerFrame;
		}
	}

	int curFrame = frame % layerMax;
	if( insData.singleFrame )
	{
		curFrame = 0;
	}
	else if(insData.playOnce && frame >= layerMax)
	{
		curFrame = layerMax - 1;
	}

	for(int layer = symbol->getDOMLayerCount() - 1; layer >= 0; layer--)
	{
		const DOMLayer* domLayer = symbol->getDOMLayer(layer);
		int layerFrame = domLayer->getFrameCount();
		int curLayerFrame = curFrame;
		if( curLayerFrame > layerFrame - 1)
		{
			continue;
		}

		FrameData frameData = domLayer->getFrameData(curLayerFrame);

		for(int i = 0; i < frameData.instanceSymbolVector.size(); ++i)
		{
			instanceData symbolInsData = frameData.instanceSymbolVector[i];
			Matrix m = insData.matrix;
			m.concatenate(symbolInsData.matrix);
			symbolInsData.matrix = m;
			symbolInsData.a *= insData.a;
			symbolInsData.r *= insData.r;
			symbolInsData.g *= insData.g;
			symbolInsData.b *= insData.b;
			if( insData.isBrightness )
			{
				symbolInsData.isBrightness = true;
			}
			drawSymbolItem4(symbolInsData, curLayerFrame - frameData.startFrame);
		}

		for(int i = 0; i < frameData.instanceBitMapVector.size(); ++i)
		{
			instanceData bitmapIns = frameData.instanceBitMapVector[i];
			bitmapIns.a *= insData.a;
			bitmapIns.r *= insData.r;
			bitmapIns.g *= insData.g;
			bitmapIns.b *= insData.b;
			if( insData.isBrightness )
			{
				bitmapIns.isBrightness = true;
			}
			drawBitmap(insData.matrix, bitmapIns);
		}
	}
}

void HelloWorld::drawSymbolItem4(instanceData insData, int frame)
{
	DOMDocument* doc = getDataLogicMgr()->getDocument();
	const DOMSymbolItem* symbol = doc->getSymbolItem(insData.resName.c_str());

	int layerMax = 0;
	for(int layer = symbol->getDOMLayerCount() - 1; layer >= 0; layer--)
	{
		const DOMLayer* domLayer = symbol->getDOMLayer(layer);
		int layerFrame = domLayer->getFrameCount();
		if( layerMax < layerFrame )
		{
			layerMax = layerFrame;
		}
	}
	
	int curFrame = frame % layerMax;
	if( insData.singleFrame )
	{
		curFrame = 0;
	}
	else if(insData.playOnce && frame >= layerMax)
	{
		curFrame = layerMax - 1;
	}

	for(int layer = symbol->getDOMLayerCount() - 1; layer >= 0; layer--)
	{
		const DOMLayer* domLayer = symbol->getDOMLayer(layer);
		int layerFrame = domLayer->getFrameCount();
		int curLayerFrame = curFrame;
		if( curLayerFrame > layerFrame - 1)
		{
			continue;
		}

		FrameData frameData = domLayer->getFrameData(curLayerFrame);

		for(int i = 0; i < frameData.instanceSymbolVector.size(); ++i)
		{
			CCAssert(false, "");
		}

		for(int i = 0; i < frameData.instanceBitMapVector.size(); ++i)
		{
			instanceData bitmapIns = frameData.instanceBitMapVector[i];
			bitmapIns.a *= insData.a;
			bitmapIns.r *= insData.r;
			bitmapIns.g *= insData.g;
			bitmapIns.b *= insData.b;
			if( insData.isBrightness )
			{
				bitmapIns.isBrightness = true;
			}
			drawBitmap(insData.matrix, bitmapIns);
		}
	}
}



void HelloWorld::forwardFrame(bool loop)
{
	DOMDocument* doc = getDataLogicMgr()->getDocument();
	instanceData insData = doc->getAnimationData(_curAnimination);
	if(insData.resName == "")
	{
		return;
	}
	const DOMSymbolItem* animinationSymbolItem = doc->getSymbolItem(insData.resName.c_str());
	int maxFrameCount = animinationSymbolItem->getFrameCount();
	++_curFrame;
	if(_curFrame >= maxFrameCount)
	{
		if(loop)
		{
			_curFrame = 0;
		}
		else
		{
			_curFrame = maxFrameCount -1;
		}
	}
}

void HelloWorld::backwardFrame()
{
	--_curFrame;
	if(_curFrame < 0)
	{
		_curFrame = 0;
	}
}

void HelloWorld::step(cocos2d::ccTime dt)
{
	
	if(_isPlaying)
	{
		forwardFrame(true);
	}
	char buf[256] = {0};
	sprintf(buf, "%d", _curFrame);
	_curFrameLabel->setString(buf);
}

const FlashMetaSymbol& getFlashMetaSymbol(const std::string name, const FlashMeta* meta)
{
	for(int i = 0; i < meta->symbols_size(); ++i)
	{
		const FlashMetaSymbol& symbol = meta->symbols(i);
		if(symbol.name() == name)
		{
			return symbol;
		}
	}

	std::string xmlName = name + ".xml";
	for(int i = 0; i < meta->symbols_size(); ++i)
	{
		const FlashMetaSymbol& symbol = meta->symbols(i);
		if(symbol.name() == xmlName)
		{
			return symbol;
		}
	}

	CCAssert(false, "");
}


void HelloWorld::drawMetaBitmap(const FlashMetaData& data)
{

	DOMDocument* doc = getDataLogicMgr()->getDocument();
	FlashCCSprite* sprite = doc->getSprite(data.name());
	sprite->setPositionInPixels(_curPoint);
	Matrix mat(data.ma(), data.mb(), data.mc(), data.md(), data.mtx(), data.mty() );

	sprite->updateVertex( mat );
	sprite->updateColor(data.a(), data.r(), data.g(), data.b());
	if(data.isbrightness())
	{
		ccBlendFunc oldFun = sprite->getBlendFunc();
		ccBlendFunc fun = {GL_ONE, GL_ONE};
		sprite->setBlendFunc(fun);
		sprite->draw();
		sprite->setBlendFunc(oldFun);
	}
	else
	{
		sprite->draw();
	}
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

void HelloWorld::drawMetaItem(const FlashMetaData& insMetaData, int frame)
{
	if(insMetaData.name() == "")
	{
		return;
	}


	const FlashMetaSymbol& symbol = getFlashMetaSymbol(insMetaData.name(), g_flashMeta);

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
			drawMetaItem(nextMetaData, curLayerFrame - metaFrame.startframe());
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
			drawMetaBitmap(nextMetaData);
		}
	}
}


void HelloWorld::visit(void)
{
	CCLayer::visit();
	//if(isUserFlashMeta)
	//{
		const FlashMetaData& data = g_flashMeta->anims(_curAnimination);
		drawMetaItem(data, _curFrame);
	///
	//else
	//{
	//	DOMDocument* doc = getDataLogicMgr()->getDocument();
	//	instanceData insData = doc->getAnimationData(_curAnimination);
	//	drawSymbolItem1(insData, _curFrame);
	//}
}

void HelloWorld::menuLeftCallback(CCObject* pSender)
{
	--_curAnimination;
	if(_curAnimination < 0)
	{
		_curAnimination = _maxAniminationCount - 1;
	}
	_curFrame = 0;

	DOMDocument* doc = getDataLogicMgr()->getDocument();
	std::string name = doc->getAnimationName(_curAnimination);
	_animationNameLabel->setString(name.c_str());
	_curPoint.x = 0;
	_curPoint.y = 0;
}

void HelloWorld::menuRightCallback(CCObject* pSender)
{
	++_curAnimination;
	_curAnimination = _curAnimination % _maxAniminationCount;
	_curFrame = 0;

	DOMDocument* doc = getDataLogicMgr()->getDocument();
	std::string name = doc->getAnimationName(_curAnimination);
	_animationNameLabel->setString(name.c_str());
	_curPoint.x = 0;
	_curPoint.y = 0;
}

void HelloWorld::menuUpCallback(CCObject* pSender)
{
	if(!_isPlaying)
	{
		forwardFrame(false);
	}
}

void HelloWorld::menuDownCallback(CCObject* pSender)
{
	if(!_isPlaying)
	{
		backwardFrame();
	}
}

void HelloWorld::menuPlayCallback(CCObject* pSender)
{
	if(!_isPlaying)
	{
		_isPlaying = true;
		_playMenuItem->setIsVisible(false);
		_stopMenuItem->setIsVisible(true);
	}
}

void HelloWorld::menuStopCallback(CCObject* pSender)
{
	if(_isPlaying)
	{
		_isPlaying = false;
		_playMenuItem->setIsVisible(true);
		_stopMenuItem->setIsVisible(false);
	}
}


void HelloWorld::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	CCSetIterator it = pTouches->begin();
	CCTouch* touch = (CCTouch*)(*it);

	CCPoint location = touch->locationInView( touch->view() );
	CCPoint convertedLocation = CCDirector::sharedDirector()->convertToGL(location);

	if(!_isTouch)
	{
		_isTouch = true;
		_startPoint = convertedLocation;
	}
}

void HelloWorld::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
	CCSetIterator it = pTouches->begin();
	CCTouch* touch = (CCTouch*)(*it);

	CCPoint location = touch->locationInView( touch->view() );
	CCPoint convertedLocation = CCDirector::sharedDirector()->convertToGL(location);

	if(_isTouch)
	{
		CCPoint offsetPoint = ccpSub(convertedLocation, _startPoint);
		_curPoint = ccpAdd(offsetPoint, _curPoint);
		_startPoint = convertedLocation;
	}

}

void HelloWorld::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
	CCSetIterator it = pTouches->begin();
	CCTouch* touch = (CCTouch*)(*it);

	CCPoint location = touch->locationInView( touch->view() );
	CCPoint convertedLocation = CCDirector::sharedDirector()->convertToGL(location);

	_isTouch = false;
}

void HelloWorld::ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
{
	CCSetIterator it = pTouches->begin();
	CCTouch* touch = (CCTouch*)(*it);

	CCPoint location = touch->locationInView( touch->view() );
	CCPoint convertedLocation = CCDirector::sharedDirector()->convertToGL(location);

	_isTouch = false;
}


void HelloWorld::menuCloseCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}


bool isFrameMetaDataSame(const FlashMetaData* a, const FlashMetaData* b)
{
	if( a->name() != b->name())
	{
		return false;
	}

	if(a->has_ma() && b->has_ma() ){
		if(a->ma() != b->mb())
			return false;
	}
	else
		return false;

	if(a->has_mb() && b->has_mb() ){
		if(a->mb() != b->mb())
			return false;
	}
	else
		return false;

	if(a->has_mc() && b->has_mc() ){
		if(a->mc() != b->mc())
			return false;
	}
	else
		return false;

	if(a->has_md() && b->has_md() ){
		if(a->md() != b->md())
			return false;
	}
	else
		return false;

	if(a->has_mtx() && b->has_mtx() ){
		if(a->mtx() != b->mtx())
			return false;
	}
	else
		return false;

	if(a->has_mty() && b->has_mty() ){
		if(a->mty() != b->mty())
			return false;
	}
	else
		return false;

	if(a->has_a() && b->has_a() ){
		if(a->a() != b->a())
			return false;
	}
	else
		return false;

	if(a->has_r() && b->has_r() ){
		if(a->r() != b->r())
			return false;
	}
	else
		return false;

	if(a->has_g() && b->has_g() ){
		if(a->g() != b->g())
			return false;
	}
	else
		return false;

	if(a->has_b() && b->has_b() ){
		if(a->b() != b->b())
			return false;
	}
	else
		return false;

	if(a->has_playonce() && b->has_playonce() ){
		if(a->playonce() != b->playonce())
			return false;
	}
	else
		return false;

	if(a->has_singleframe() && b->has_singleframe() ){
		if(a->singleframe() != b->singleframe())
			return false;
	}
	else
		return false;

	if(a->has_isbrightness() && b->has_isbrightness() ){
		if(a->isbrightness() != b->isbrightness())
			return false;
	}
	else
		return false;

	//if(a->has_left() && b->has_left() ){
	//	if(a->left() != b->left())
	//		return false;
	//}
	//else
	//	return false;

	//if(a->has_right() && b->has_right() ){
	//	if(a->right() != b->right())
	//		return false;
	//}
	//else
	//	return false;

	//if(a->has_top() && b->has_top() ){
	//	if(a->top() != b->top())
	//		return false;
	//}
	//else
	//	return false;

	//if(a->has_button() && b->has_button() ){
	//	if(a->button() != b->button())
	//		return false;
	//}
	//else
	//	return false;

	return true;
}
#include "main.h"

#include "../Classes/AppDelegate.h"
#include <tchar.h>
#include <string>
#include "DataLogicMgr.h"
#include "utils.h"
#include <windows.h> 
#include "DOMDocument.h"
#include "DOMSymbolItem.h"
#include "DOMLayer.h"
#include "DOMFrame.h"
#include "flashMeta.pb.h"
#include "BoxFlashInfo.pb.h"
#include "flashMetaEx.pb.h"
#include "HelloWorldScene.h"
#include <fstream>

using namespace std; 


int g_clipCount = 0;

static int g_curFrame = 0;





//extern bool isUserFlashMeta;
extern FlashMetaEx* g_flashMetaEx;
extern FlashMeta* g_flashMeta;
extern BoxFlashInfo* g_boxFlashInfo;

int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	
	std::string strCmdLine = //WChar2Ansi(TEXT("f:\\branch\\endLess\\Project\\Resource\\monster\\edragonegg\\ edragonegg_png"));
	   WChar2Ansi(lpCmdLine);
	
	getDataLogicMgr()->initCmdLine(strCmdLine.c_str());

	DataLogicMgr::getInstance()->loadScreen( getDataLogicMgr()->getPath() );

	if( getDataLogicMgr()->isPlay())
	{
		AppDelegate app;
		cocos2d::CCApplication::sharedApplication().run();
	}
	else
	{
		getDataLogicMgr()->loadXml( getDataLogicMgr()->getPath() );

		//if(isUserFlashMeta)
		//{
			std::string plistName =  getDataLogicMgr()->getPlistName();
			CCAssert( plistName != "");
			CCAssert( plistName != "show" );
			g_flashMeta->set_spritename(plistName);
			g_boxFlashInfo->set_spritename(plistName);
#if (defined USE_FLASH_METADATA_CACHE && USE_FLASH_METADATA_CACHE == 1)
			g_flashMetaEx->set_spritename(plistName);
			packCacheMeta();
#else
			packNewMeta();
#endif
			packNewMetaBox();
			std::string fpkPathAndName = getDataLogicMgr()->getFpkPath();
			std::string boxPathAndName = getDataLogicMgr()->getFpkPath();
			std::string fpkName = getFileName( getDataLogicMgr()->getPath().c_str() );
			fpkPathAndName += fpkName;
			boxPathAndName = fpkPathAndName + ".bb";
			fpkPathAndName += ".fpk";
			std::fstream output( fpkPathAndName.c_str(), std::ios::out | std::ios::trunc | std::ios::binary );
#if (defined USE_FLASH_METADATA_CACHE && USE_FLASH_METADATA_CACHE == 1)
			g_flashMetaEx->CheckInitialized();
			if (!g_flashMetaEx->SerializeToOstream(&output)) 
			{
				assert(false);
			}
#else
			g_flashMeta->CheckInitialized();
			if (!g_flashMeta->SerializeToOstream(&output)) 
			{
				assert(false);
			}
#endif


			std::fstream outputBB( boxPathAndName.c_str(), std::ios::out | std::ios::trunc | std::ios::binary );
			g_boxFlashInfo->CheckInitialized();
			if (!g_boxFlashInfo->SerializeToOstream(&outputBB)) 
			{
				assert(false);
			}



		//}
		//else
		//{
		//	g_pFlashSprite = new FlashSpritePb();
		//	std::string plistName =  getDataLogicMgr()->getPlistName();
		//	CCAssert( plistName != "");
		//	CCAssert( plistName != "show" );
		//	g_pFlashSprite->set_spritename(plistName);
		//	DOMDocument* doc = getDataLogicMgr()->getDocument();
		//	double flashRate = doc->getFlashRate();
		//	float frameTime = 1.f / flashRate;
		//	g_pFlashSprite->set_frametime( frameTime );
		//	for(int ani = 0; ani < doc->getAnimationCount(); ++ani)
		//	{
		//		g_pCurAnimination = g_pFlashSprite->add_anims();

		//		instanceData insData = doc->getAnimationData(ani);
		//		if(insData.resName == "")
		//		{
		//			continue;
		//		}

		//		const DOMSymbolItem* animinationSymbolItem = doc->getSymbolItem(insData.resName.c_str());
		//		int maxFrameCount = animinationSymbolItem->getFrameCount();
		//		for(int frame = 0; frame < maxFrameCount; frame++)
		//		{
		//			g_curFrame = frame;
		//			g_pCurFrame = g_pCurAnimination->add_frames();
		//			processSymbolItem(insData, frame);
		//		}
		//	}
		//	std::string fpkPathAndName = getDataLogicMgr()->getFpkPath();
		//	std::string fpkName = getFileName( getDataLogicMgr()->getPath().c_str() );
		//	fpkPathAndName += fpkName;
		//	fpkPathAndName += ".fpk";
		//	std::fstream output( fpkPathAndName.c_str(), std::ios::out | std::ios::trunc | std::ios::binary );
		//	g_pFlashSprite->CheckInitialized();
		//	if (!g_pFlashSprite->SerializeToOstream(&output)) 
		//	{
		//		assert(false);
		//	}
		//}
		
	}
	return 0;
}

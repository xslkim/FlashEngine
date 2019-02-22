#include "DOMSymbolItem.h"
#include "DOMLayer.h"
#include "DOMFrame.h"

DOMSymbolItem::DOMSymbolItem()
{
	_maxFrame = 0;
}

DOMSymbolItem::~DOMSymbolItem()
{
	for(int i = 0; i < _DOMFrameVectorMap.size(); ++i)
	{
		delete _DOMFrameVectorMap[i];
	}
}

void DOMSymbolItem::LoadXml(const char* pathAndName, std::string onlyName)
{
	_symbolName = onlyName;
	TiXmlDocument* xmldoc = new TiXmlDocument(pathAndName);
	bool success = xmldoc->LoadFile();
	CCAssert( success);
	TiXmlElement* rootElement = xmldoc->RootElement();
	const char* symbolType = rootElement->Attribute("symbolType");
	if(symbolType != NULL)
	{
		std::string strSymbolType = symbolType;
		if(strSymbolType == "symbolType")
		{
			return;
		}
	}
	

	TiXmlElement* firstChildElement = rootElement->FirstChildElement();
	std::string timelineValue = firstChildElement->Value();
	CCAssert( timelineValue == "timeline");

	TiXmlElement* DOMTimelineElement = firstChildElement->FirstChildElement();
	std::string DOMTimelineValue = DOMTimelineElement->Value();
	CCAssert( DOMTimelineValue == "DOMTimeline");

	TiXmlElement* layersElement = DOMTimelineElement->FirstChildElement();
	std::string layersValue = layersElement->Value();
	CCAssert( layersValue == "layers");

	for(TiXmlElement* DOMlayerElement = layersElement->FirstChildElement(); DOMlayerElement != NULL; DOMlayerElement = DOMlayerElement->NextSiblingElement() )
	{
		std::string DOMlayerValue = DOMlayerElement->Value();
		CCAssert( DOMlayerValue == "DOMLayer");
		std::string layerName = DOMlayerElement->Attribute("name");
		const char* layerType = DOMlayerElement->Attribute("layerType");
		if( layerType != NULL)
		{
			std::string type = layerType;
			if( type == "folder")
			{
				continue;
			}
		}
		
		TiXmlElement* framesElement = DOMlayerElement->FirstChildElement();
		if( framesElement == NULL )
		{
			continue;
		}

		if (layerName == "Rshoulder" )
		{
			int qwer = 1234;
		}

		std::string framesValue = framesElement->Value();
		CCAssert( framesValue == "frames");
		DOMLayer* domLayer = new DOMLayer();
		domLayer->LoadXml(framesElement);
		if (layerName == "attack")
		{
			domLayer->processFrameBoxAttack();
		}
		else if (layerName == "hurt")
		{
			domLayer->processFrameBoxHurt();
		}
		else
		{
			domLayer->processFrameData();
		}
		_DOMFrameVectorMap.push_back(domLayer );
		if(domLayer->getFrameCount() > _maxFrame)
		{
			_maxFrame = domLayer->getFrameCount();
		}
		//debug_out("***********************LAYER********************\n");
		//for(int i = 0; i < domLayer->getFrameCount(); ++i)
		//{
		//	debug_out("**Frame %d\n", i);
		//	FrameData data = domLayer->getFrameData(i);
		//	data.instanceSymbolVector[0].matrix.print();
		//}


	}
	

}

void DOMSymbolItem::LoadPb(const char* fileName)
{

}
#include "DOMFrame.h"


DOMFrame::DOMFrame()
{
	
}

DOMFrame::~DOMFrame()
{

}

void DOMFrame::loadBitMap(TiXmlElement* instanceElement)
{
	DOMBitmapInstance bitmapIns;
	bitmapIns.insData.resName = instanceElement->Attribute("libraryItemName");

	TiXmlElement* matrixElement = getChildElement(instanceElement, "matrix");
	if(matrixElement)
	{
		TiXmlElement* valueMatrixElement = matrixElement->FirstChildElement();
		setAttribDouble(valueMatrixElement, "a", bitmapIns.insData.matrix.a);
		setAttribDouble(valueMatrixElement, "b", bitmapIns.insData.matrix.b);
		setAttribDouble(valueMatrixElement, "c", bitmapIns.insData.matrix.c);
		setAttribDouble(valueMatrixElement, "d", bitmapIns.insData.matrix.d);
		setAttribDouble(valueMatrixElement, "tx", bitmapIns.insData.matrix.tx);
		setAttribDouble(valueMatrixElement, "ty", bitmapIns.insData.matrix.ty);
	}

	TiXmlElement* transformationPointElement = getChildElement(instanceElement, "transformationPoint");
	if(transformationPointElement)
	{
		TiXmlElement* pointElement = transformationPointElement->FirstChildElement();
		setAttribDouble(pointElement, "x", bitmapIns.insData.transformationPoint.x);
		setAttribDouble(pointElement, "y", bitmapIns.insData.transformationPoint.y);
	}

	TiXmlElement* colorElement = getChildElement(instanceElement, "color");
	if(colorElement)
	{
		TiXmlElement* color = getChildElement(colorElement, "Color");
		setAttribDouble(color, "alphaMultiplier", bitmapIns.insData.a);
		setAttribDouble(color, "redMultiplier", bitmapIns.insData.r);
		setAttribDouble(color, "greenMultiplier", bitmapIns.insData.g);
		setAttribDouble(color, "blueMultiplier", bitmapIns.insData.b);
		double brightness = 0;
		color->Attribute("brightness", &brightness);
		if(brightness > 0.1f)
		{
			bitmapIns.insData.isBrightness = true;
		}
	}

	bitmapIns.insData.changeMatrixRotate();
	_frameData.bitMapVector.push_back(bitmapIns);
}

void DOMFrame::loadInstance(TiXmlElement* instanceElement)
{
	DOMSymbolInstance symbolIns;
	symbolIns.insData.resName = instanceElement->Attribute("libraryItemName");
	const char* instName = instanceElement->Attribute("name");
	if(instName != NULL)
	{
		symbolIns.insData.insName = instName;
	}


	const char* loop = instanceElement->Attribute("loop");
	if( loop != NULL)
	{
		std::string strLoop = loop;
		if( strLoop == "play once" )
		{
			symbolIns.insData.playOnce = true;
		}

		if( strLoop == "single frame")
		{
			symbolIns.insData.singleFrame = true;
		}
	}

	TiXmlElement* matrixElement = getChildElement(instanceElement, "matrix");
	if(matrixElement)
	{
		TiXmlElement* valueMatrixElement = matrixElement->FirstChildElement();
		setAttribDouble(valueMatrixElement, "a", symbolIns.insData.matrix.a);
		setAttribDouble(valueMatrixElement, "b", symbolIns.insData.matrix.b);
		setAttribDouble(valueMatrixElement, "c", symbolIns.insData.matrix.c);
		setAttribDouble(valueMatrixElement, "d", symbolIns.insData.matrix.d);
		setAttribDouble(valueMatrixElement, "tx", symbolIns.insData.matrix.tx);
		setAttribDouble(valueMatrixElement, "ty", symbolIns.insData.matrix.ty);
	}

	TiXmlElement* transformationPointElement = getChildElement(instanceElement, "transformationPoint");
	if(transformationPointElement)
	{
		TiXmlElement* pointElement = transformationPointElement->FirstChildElement();
		setAttribDouble(pointElement, "x", symbolIns.insData.transformationPoint.x);
		setAttribDouble(pointElement, "y", symbolIns.insData.transformationPoint.y);
	}

	TiXmlElement* colorElement = getChildElement(instanceElement, "color");
	if(colorElement)
	{
		TiXmlElement* color = getChildElement(colorElement, "Color");

		setAttribDouble(color, "alphaMultiplier", symbolIns.insData.a);
		setAttribDouble(color, "redMultiplier", symbolIns.insData.r);
		setAttribDouble(color, "greenMultiplier", symbolIns.insData.g);
		setAttribDouble(color, "blueMultiplier", symbolIns.insData.b);
		double brightness = 0;
		color->Attribute("brightness", &brightness);
		if(brightness > 0.1f)
		{
			symbolIns.insData.isBrightness = true;
		}
		//brightness
	}

	symbolIns.insData.changeMatrixRotate();
	_frameData.symbolVector.push_back(symbolIns);
}

void DOMFrame::LoadXml(TiXmlElement* element)
{
	const char* exist = element->Attribute("index", &_frameData.index);
	CCAssert(exist);
	element->Attribute("duration", &_frameData.duration);

	setAttribString( _frameData.motionTweenRotate, element->Attribute("motionTweenRotate") );
	if(_frameData.motionTweenRotate.size() > 0)
	{
		_frameData.motionTweenRotateTimes = 0;
		element->Attribute("motionTweenRotateTimes", &_frameData.motionTweenRotateTimes );
	}
	
	setAttribString( _frameData.motionTweenSnap, element->Attribute("motionTweenSnap") );
	setAttribString( _frameData.tweenType, element->Attribute("tweenType"));

	//TiXmlElement* elementElement = element->FirstChildElement();
	//std::string elementValue = elementElement->Value();
	//CCAssert( elementValue == "elements");
	TiXmlElement* elementElement = getChildElement(element, "elements");

	for( TiXmlElement* instanceElement = elementElement->FirstChildElement(); instanceElement != NULL; instanceElement = instanceElement->NextSiblingElement() )
	{
		std::string instanceValue = instanceElement->Value();
		if( instanceValue == "DOMSymbolInstance")
		{
			loadInstance(instanceElement);
		}
		else if( instanceValue == "DOMBitmapInstance")
		{
			loadBitMap(instanceElement);
		}
		else if( instanceValue == "DOMShape" )
		{

		}
		else if( instanceValue == "DOMGroup" )
		{
			TiXmlElement* memberElement = getChildElement(instanceElement, "members");
			CCAssert( memberElement != NULL );
			for(TiXmlElement* childElement = memberElement->FirstChildElement();
				childElement != NULL; childElement = childElement->NextSiblingElement() )
			{
				std::string childValue = childElement->Value();
				if( childValue == "DOMSymbolInstance")
				{
					loadInstance(childElement);
				}
				else if( childValue == "DOMBitmapInstance")
				{
					loadBitMap(childElement);
				}
				else if( childValue == "DOMShape" )
				{

				}
				else
				{
					CCAssert(false);
				}

			}
		}
		else
		{
			CCAssert(false);
		}

	}
}

void DOMFrame::LoadPb(const char* fileName)
{

}
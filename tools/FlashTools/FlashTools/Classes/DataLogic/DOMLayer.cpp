#include "DOMLayer.h"
#include "DOMFrame.h"
#include <vector>

DOMLayer::DOMLayer()
{
	_isBoxLayer = false;
}

DOMLayer::~DOMLayer()
{
	for(int i = 0; i < _DOMFrameVector.size(); ++i)
	{
		delete _DOMFrameVector[i];
	}
}

void DOMLayer::LoadXml(TiXmlElement* element)
{
	for(TiXmlElement* DOMFrameElement = element->FirstChildElement(); DOMFrameElement != NULL; DOMFrameElement = DOMFrameElement->NextSiblingElement() )
	{
		std::string DOMFrameValue = DOMFrameElement->Value();
		CCAssert(DOMFrameValue == "DOMFrame");
		DOMFrame* domFrame = new DOMFrame();
		domFrame->LoadXml( DOMFrameElement );
		_DOMFrameVector.push_back(domFrame);
	}
}

void DOMLayer::LoadPb(const char* fileName)
{

}

struct StartFrameS
{
	StartFrameS(int f, std::string n)
	{
		name = n;
		frame = f;
	}
	std::string name;
	int frame;
};

int getStartFrame(std::vector<StartFrameS> vec, std::string curName)
{
	int f = -1;
	for(int i = vec.size() -1; i >= 0; i--)
	{
		StartFrameS s = vec[i];
		if(s.name == curName)
		{
			f = s.frame;
		}
		else
		{
			break;
		}
	}
	return f;
}

void DOMLayer::processFrameBoxAttack()
{
	_isBoxLayer = true;
	_BoxKeyFrameAttack.clear();
	for(std::vector<DOMFrame*>::iterator it = _DOMFrameVector.begin(); it != _DOMFrameVector.end(); it++)
	{
		DOMFrameData domFrameData = (*it)->getFrameData();
		BoxKeyFrame kf;
		kf.start = domFrameData.index;
		kf.duration = domFrameData.duration;
		for (int i = 0; i < domFrameData.symbolVector.size(); ++i)
		{
			DOMSymbolInstance instznce = domFrameData.symbolVector[i];
			Matrix m; 
			memcpy(&m, &instznce.insData.matrix, sizeof(Matrix));
			kf.tranMatrix.push_back(m);
		}
		if (kf.tranMatrix.size() > 0)
		{
			_BoxKeyFrameAttack.push_back(kf);
		}
	}
}

void DOMLayer::processFrameBoxHurt()
{
	_isBoxLayer = true;
	_BoxKeyFrameHurt.clear();
	for(std::vector<DOMFrame*>::iterator it = _DOMFrameVector.begin(); it != _DOMFrameVector.end(); it++)
	{
		DOMFrameData domFrameData = (*it)->getFrameData();
		BoxKeyFrame kf;
		kf.start = domFrameData.index;
		kf.duration = domFrameData.duration;
		for (int i = 0; i < domFrameData.symbolVector.size(); ++i)
		{
			DOMSymbolInstance instznce = domFrameData.symbolVector[i];
			Matrix m; 
			memcpy(&m, &instznce.insData.matrix, sizeof(Matrix));
			kf.tranMatrix.push_back(m);
		}
		if(kf.tranMatrix.size() > 0)
		{
			_BoxKeyFrameHurt.push_back(kf);
		}
	}

}

void DOMLayer::processFrameData()
{
	int curFrame = 0;
	_frameVector.clear();
	std::vector<StartFrameS>startFrameVector;
	
	for(std::vector<DOMFrame*>::iterator it = _DOMFrameVector.begin(); it != _DOMFrameVector.end(); )
	{
		DOMFrameData domFrameData = (*it)->getFrameData();
		if( domFrameData.symbolVector.size() == 0 && domFrameData.bitMapVector.size() == 0) //当前是空白关键帧
		{
			for(int i = 0; i < domFrameData.duration; ++i)
			{
				FrameData data;
				data.startFrame = domFrameData.index;
				_frameVector.push_back(data);
				curFrame++;
			}
			it++;
			StartFrameS ss(domFrameData.index, "");
			startFrameVector.push_back(ss);
			continue;
		}

		if( domFrameData.tweenType.size() > 0 && domFrameData.bitMapVector.size() == 0) //当前是有补间的帧
		{
			CCAssert( domFrameData.tweenType == "motion" );
			CCAssert(domFrameData.index == curFrame );
			CCAssert( domFrameData.symbolVector.size() == 1 );
			DOMSymbolInstance fromSymbolIns = domFrameData.symbolVector[0];
			it++;

			if(it == _DOMFrameVector.end() || domFrameData.duration == 1) //补间的范围为1
			{
				FrameData data;
				instanceData insData;
				insData.matrix = fromSymbolIns.insData.matrix;
				insData.insName = fromSymbolIns.insData.insName;
				insData.matrix.calculatePara();
				insData.resName = fromSymbolIns.insData.resName;
				insData.transformationPoint = fromSymbolIns.insData.transformationPoint;
				insData.a = fromSymbolIns.insData.a;
				insData.r = fromSymbolIns.insData.r;
				insData.g = fromSymbolIns.insData.g;
				insData.b = fromSymbolIns.insData.b;
				int angle = PiToAngle( insData.matrix.get_rotation() );

				data.instanceSymbolVector.push_back(insData);
				_frameVector.push_back(data);
				curFrame++;

				int startFrame = getStartFrame( startFrameVector, insData.resName);
				if(startFrame != -1)
				{
					data.startFrame = startFrame;
				}
				else
				{
					data.startFrame = domFrameData.index;
				}

				if(domFrameData.symbolVector.size() == 1)
				{
					StartFrameS ss(domFrameData.index, domFrameData.symbolVector[0].insData.resName);
					startFrameVector.push_back(ss);
				}
				else
				{
					StartFrameS ss(domFrameData.index, "");
					startFrameVector.push_back(ss);
				}
				
				continue;
			}

			DOMFrameData nextDomFrameData = (*it)->getFrameData();

			CCAssert(nextDomFrameData.index == curFrame + domFrameData.duration );
			bool nextFrameExist = true;

			CCAssert( nextDomFrameData.bitMapVector.size() == 0 );
			if( nextDomFrameData.symbolVector.size() == 0 )
			{
				nextFrameExist = false;
			}
			else
			{
				CCAssert(nextDomFrameData.symbolVector.size() != 0);
			}

			if(nextDomFrameData.symbolVector.size() == 0){
				nextFrameExist = false;
			}

			
			if(nextFrameExist)
			{
				DOMSymbolInstance toSymbolIns = nextDomFrameData.symbolVector[0];
				double fromX = fromSymbolIns.insData.matrix.tx;
				double fromY = fromSymbolIns.insData.matrix.ty;
				double toX = toSymbolIns.insData.matrix.tx;
				double toY = toSymbolIns.insData.matrix.ty;

				double fromA = fromSymbolIns.insData.a;
				double fromR = fromSymbolIns.insData.r;
				double fromG = fromSymbolIns.insData.g;
				double fromB = fromSymbolIns.insData.b;
				double toA = toSymbolIns.insData.a;
				double toR = toSymbolIns.insData.r;
				double toG = toSymbolIns.insData.g;
				double toB = toSymbolIns.insData.b;
				double intervalA = (toA - fromA) / domFrameData.duration;
				double intervalR = (toR - fromR) / domFrameData.duration;
				double intervalG = (toG - fromG) / domFrameData.duration;
				double intervalB = (toB - fromB) / domFrameData.duration;

				double fromScaleX = fromSymbolIns.insData.matrix.get_x_scale();
				double fromScaleY = fromSymbolIns.insData.matrix.get_y_scale();
				double toScaleX = toSymbolIns.insData.matrix.get_x_scale();
				double toScaleY = toSymbolIns.insData.matrix.get_y_scale();
				double intervalScaleX = (toScaleX - fromScaleX) / domFrameData.duration;
				double intervalScaleY = (toScaleY - fromScaleY) / domFrameData.duration;

				double fromRotate = fromSymbolIns.insData.matrix.get_rotation();
				double toRotate = toSymbolIns.insData.matrix.get_rotation();

				int vFromRotate = PiToAngle(fromRotate);
				int vToRotate = PiToAngle(toRotate);

				double anchRotate = toRotate - fromRotate;
				if( fromRotate < -0.00001 && toRotate > 0.00001 )
				{
					double clockWiseAngle = -fromRotate;
					clockWiseAngle += toRotate;

					double unClockWiseAngle = M_PI + fromRotate;
					unClockWiseAngle += M_PI - toRotate;

					if( clockWiseAngle < unClockWiseAngle )
					{
						anchRotate = clockWiseAngle;
					}
					else
					{
						anchRotate = -unClockWiseAngle;
					}
				}
				else if( toRotate < -0.0001 && fromRotate > 0.0001 )
				{
					double clockWiseAngle = M_PI - fromRotate;
					clockWiseAngle += M_PI  + toRotate;

					double unClockWiseAngle = fromRotate;
					unClockWiseAngle += -toRotate;

					if( clockWiseAngle < unClockWiseAngle)
					{
						anchRotate = clockWiseAngle;
					}
					else
					{
						anchRotate = -unClockWiseAngle;
					}
				}
				else
				{
					double clockWiseAngle = M_PI - fromRotate;
					clockWiseAngle += M_PI  + toRotate;

					double unClockWiseAngle = fromRotate;
					unClockWiseAngle += -toRotate;

					if( clockWiseAngle < unClockWiseAngle)
					{
						anchRotate = clockWiseAngle;
					}
					else
					{
						anchRotate = -unClockWiseAngle;
					}
				}

				double intervalRotate = anchRotate / domFrameData.duration;

				Point formAnchPoint = fromSymbolIns.insData.transformationPoint;

				double anchScaleX = toScaleX - fromScaleX;
				double anchScaleY = toScaleY - fromScaleY;
				Matrix anchMatrix;
				anchMatrix.set_scale_rotation(1.f + anchScaleX, 1.f + anchScaleY, anchRotate);
				Point toAnchPoint;
				anchMatrix.transform( &toAnchPoint, formAnchPoint);
				double offsetAnchX = formAnchPoint.x - toAnchPoint.x;
				double offsetAnchY = formAnchPoint.y - toAnchPoint.y;


				double intervalX = ((toX - offsetAnchX) - fromX) / domFrameData.duration;
				double intervalY = ((toY - offsetAnchY) - fromY) / domFrameData.duration;


				double intervalTweenRotate = 0;
				if( domFrameData.motionTweenRotate == "clockwise" )
				{
					double totalRotate = domFrameData.motionTweenRotateTimes * M_PI * 2;
					intervalTweenRotate = totalRotate / domFrameData.duration;
				}
				else if(domFrameData.motionTweenRotate == "counter-clockwise")
				{
					double totalRotate = domFrameData.motionTweenRotateTimes * M_PI * 2 * -1;
					intervalTweenRotate = totalRotate / domFrameData.duration;
				}



				Point lastTransPoint;
				for(int i = 0; i < domFrameData.duration; ++i)
				{
					FrameData data;
					int startFrame = getStartFrame( startFrameVector, fromSymbolIns.insData.resName);
					if(startFrame != -1)
					{
						data.startFrame = startFrame;
					}
					else
					{
						data.startFrame = domFrameData.index;
					}

			

					instanceData insData;
					double scaleX = fromScaleX + i*intervalScaleX;
					double scaleY = fromScaleY + i*intervalScaleY;
					double rotate = fromRotate + i*intervalRotate;
					Matrix m;
					m.set_scale_rotation(scaleX, scaleY, rotate);

					m.tx = fromX + i*intervalX;
					m.ty = fromY + i*intervalY;

					double intervalAnchRotate = anchRotate / domFrameData.duration * i;
					double intervalAnchScaleX = anchScaleX / domFrameData.duration * i;
					double intervalAnchScaleY = anchScaleY / domFrameData.duration * i;
					Point curAnchPoint;
					Matrix anchMatrix;
					anchMatrix.set_scale_rotation( 1 + intervalAnchScaleX, 1 + intervalAnchScaleY, intervalAnchRotate );
					anchMatrix.transform_vector(&curAnchPoint, formAnchPoint);
					double offsetAnchX = formAnchPoint.x - curAnchPoint.x;
					double offsetAnchY = formAnchPoint.y - curAnchPoint.y;


					m.tx += offsetAnchX;
					m.ty += offsetAnchY;	

					m.calculatePara();

					insData.resName = fromSymbolIns.insData.resName;
					insData.transformationPoint = fromSymbolIns.insData.transformationPoint;
					insData.a = fromA + intervalA*i;
					insData.r = fromR + intervalR*i;
					insData.g = fromG + intervalG*i;
					insData.b = fromB + intervalB*i;

					Matrix tweenRotateMatrix;
					if( intervalTweenRotate != 0)
					{
						tweenRotateMatrix.set_scale_rotation(1.f, 1.f, intervalTweenRotate*i);
						Point formAnchPointEx = fromSymbolIns.insData.transformationPoint;
						Point toAnchPointEx;
						tweenRotateMatrix.transform( &toAnchPointEx, formAnchPointEx);
						double offsetAnchXEx = formAnchPointEx.x - toAnchPointEx.x;
						double offsetAnchYEx = formAnchPointEx.y - toAnchPointEx.y;

						m.concatenate(tweenRotateMatrix);
						m.tx += offsetAnchXEx;
						m.ty += offsetAnchYEx;	

					}

					insData.matrix = m;
					insData.insName = fromSymbolIns.insData.insName;
					data.instanceSymbolVector.push_back(insData);
					_frameVector.push_back(data);
					curFrame++;
				}

				if(domFrameData.symbolVector.size() == 1)
				{
					StartFrameS ss(domFrameData.index, domFrameData.symbolVector[0].insData.resName);
					startFrameVector.push_back(ss);
				}
				else
				{
					StartFrameS ss(domFrameData.index, "");
					startFrameVector.push_back(ss);
				}
			}
			else
			{
				for(int i = 0; i < domFrameData.duration; ++i)
				{

					FrameData data;
					int startFrame = getStartFrame( startFrameVector, fromSymbolIns.insData.resName);
					if(startFrame != -1)
					{
						data.startFrame = startFrame;
					}
					else
					{
						data.startFrame = domFrameData.index;
					}
					int angle = -1;
					for(int bitMapIndex = 0; bitMapIndex < domFrameData.bitMapVector.size(); bitMapIndex++)
					{
						instanceData insData = domFrameData.bitMapVector[bitMapIndex].insData;
						angle = PiToAngle( insData.matrix.get_rotation() );
						data.instanceBitMapVector.push_back(insData);
					}
					for(int bitSymbolIndex = 0; bitSymbolIndex < domFrameData.symbolVector.size(); bitSymbolIndex++)
					{
						instanceData insData = domFrameData.symbolVector[bitSymbolIndex].insData;
						angle = PiToAngle( insData.matrix.get_rotation() );
						data.instanceSymbolVector.push_back(insData);
					}
					_frameVector.push_back(data);
					curFrame++;
				}

				StartFrameS ss(domFrameData.index, "");
				startFrameVector.push_back(ss);
			}

			

		}
		else
		{
			for(int i = 0; i < domFrameData.duration; ++i)
			{
				FrameData data;

				if(domFrameData.symbolVector.size() == 1)
				{
					StartFrameS ss(domFrameData.index, domFrameData.symbolVector[0].insData.resName);
					int startFrame = getStartFrame( startFrameVector, domFrameData.symbolVector[0].insData.resName);
					if(startFrame != -1)
					{
						data.startFrame = startFrame;
					}
					else
					{
						data.startFrame = domFrameData.index;
					}
				}
				else
				{
					data.startFrame = domFrameData.index;
				}
				int angle = -1;
				for(int bitMapIndex = 0; bitMapIndex < domFrameData.bitMapVector.size(); bitMapIndex++)
				{
					instanceData insData = domFrameData.bitMapVector[bitMapIndex].insData;
					angle = PiToAngle( insData.matrix.get_rotation() );
					data.instanceBitMapVector.push_back(insData);
				}
				for(int bitSymbolIndex = 0; bitSymbolIndex < domFrameData.symbolVector.size(); bitSymbolIndex++)
				{
					instanceData insData = domFrameData.symbolVector[bitSymbolIndex].insData;
					angle = PiToAngle( insData.matrix.get_rotation() );
					data.instanceSymbolVector.push_back(insData);
				}
				_frameVector.push_back(data);
				curFrame++;
			}

			StartFrameS ss(domFrameData.index, "");
			startFrameVector.push_back(ss);

			it++;		
		}
		

	}

}



//old use rotate
//
//void DOMLayer::processFrameData()
//{
//	int curFrame = 0;
//	_frameVector.clear();
//	for(std::vector<DOMFrame*>::iterator it = _DOMFrameVector.begin(); it != _DOMFrameVector.end(); )
//	{
//		DOMFrameData domFrameData = (*it)->getFrameData();
//		if( domFrameData.symbolVector.size() == 0 && domFrameData.bitMapVector.size() == 0)
//		{
//			for(int i = 0; i < domFrameData.duration; ++i)
//			{
//				FrameData data;
//				data.startFrame = domFrameData.index;
//				_frameVector.push_back(data);
//				curFrame++;
//			}
//			it++;		
//			continue;
//		}
//
//		if( domFrameData.tweenType.size() > 0 && domFrameData.bitMapVector.size() == 0)
//		{
//			CCAssert( domFrameData.tweenType == "motion" );
//			CCAssert(domFrameData.index == curFrame );
//			CCAssert( domFrameData.symbolVector.size() == 1 );
//			DOMSymbolInstance fromSymbolIns = domFrameData.symbolVector[0];
//
//			it++;
//
//			if(it == _DOMFrameVector.end())
//			{
//				FrameData data;
//				instanceData insData;
//				insData.matrix = fromSymbolIns.insData.matrix;
//				insData.matrix.calculatePara();
//				insData.resName = fromSymbolIns.insData.resName;
//				insData.transformationPoint = fromSymbolIns.insData.transformationPoint;
//				data.instanceSymbolVector.push_back(insData);
//				_frameVector.push_back(data);
//				break;;
//			}
//
//			DOMFrameData nextDomFrameData = (*it)->getFrameData();
//			CCAssert(nextDomFrameData.index == curFrame + domFrameData.duration );
//			CCAssert( nextDomFrameData.bitMapVector.size() == 0 );
//			CCAssert( nextDomFrameData.symbolVector.size() == 1 );
//			DOMSymbolInstance toSymbolIns = nextDomFrameData.symbolVector[0];
//
//			//CCAssert( abs(fromSymbolIns.insData.transformationPoint.x - toSymbolIns.insData.transformationPoint.x) < 2);
//			//CCAssert( abs(fromSymbolIns.insData.transformationPoint.y - toSymbolIns.insData.transformationPoint.y) < 2);
//
//
//			double fromX = fromSymbolIns.insData.matrix.tx;
//			double fromY = fromSymbolIns.insData.matrix.ty;
//			double toX = toSymbolIns.insData.matrix.tx;
//			double toY = toSymbolIns.insData.matrix.ty;
//			//double intervalX = (toX - fromX) / domFrameData.duration;
//			//double intervalY = (toY - fromY) / domFrameData.duration;
//
//			double fromScaleX = fromSymbolIns.insData.matrix.get_x_scale();
//			double fromScaleY = fromSymbolIns.insData.matrix.get_y_scale();
//			double toScaleX = toSymbolIns.insData.matrix.get_x_scale();
//			double toScaleY = toSymbolIns.insData.matrix.get_y_scale();
//			double intervalScaleX = (toScaleX - fromScaleX) / domFrameData.duration;
//			double intervalScaleY = (toScaleY - fromScaleY) / domFrameData.duration;
//
//			double fromRotate = fromSymbolIns.insData.matrix.get_rotation();
//			double toRotate = toSymbolIns.insData.matrix.get_rotation();
//			double intervalRotate = (toRotate - fromRotate) / domFrameData.duration;
//
//			Point formAnchPoint = fromSymbolIns.insData.transformationPoint;
//			double anchRotate = toRotate - fromRotate;
//			double anchScaleX = toScaleX - fromScaleX;
//			double anchScaleY = toScaleY - fromScaleY;
//			Matrix anchMatrix;
//			anchMatrix.set_scale_rotation(1.f + anchScaleX, 1.f + anchScaleY, anchRotate);
//			Point toAnchPoint;
//			anchMatrix.transform( &toAnchPoint, formAnchPoint);
//			double offsetAnchX = formAnchPoint.x - toAnchPoint.x;
//			double offsetAnchY = formAnchPoint.y - toAnchPoint.y;
//			offsetAnchX = 0;
//			offsetAnchY = 0;
//
//			double intervalX = ((toX - offsetAnchX) - fromX) / domFrameData.duration;
//			double intervalY = ((toY - offsetAnchY) - fromY) / domFrameData.duration;
//
//
//			Point lastTransPoint;
//			for(int i = 0; i < domFrameData.duration; ++i)
//			{
//				FrameData data;
//				data.startFrame = domFrameData.index;
//				data.intervalMatrix.set_scale_rotation(intervalScaleX, intervalScaleY, intervalRotate);
//				instanceData insData;
//				double scaleX = fromScaleX + i*intervalScaleX;
//				double scaleY = fromScaleY + i*intervalScaleY;
//				double rotate = fromRotate + i*intervalRotate;
//				Matrix m;
//				m.set_scale_rotation(scaleX, scaleY, rotate);
//				m.tx = fromX + i*intervalX;
//				m.ty = fromY + i*intervalY;
//
//				double intervalAnchRotate = anchRotate / domFrameData.duration * i;
//				double intervalAnchScaleX = anchScaleX / domFrameData.duration * i;
//				double intervalAnchScaleY = anchScaleY / domFrameData.duration * i;
//				Point curAnchPoint;
//				Matrix anchMatrix;
//				anchMatrix.set_scale_rotation( 1 + intervalAnchScaleX, 1 + intervalAnchScaleY, intervalAnchRotate );
//				anchMatrix.transform_vector(&curAnchPoint, formAnchPoint);
//				double offsetAnchX = formAnchPoint.x - curAnchPoint.x;
//				double offsetAnchY = formAnchPoint.y - curAnchPoint.y;
//
//				offsetAnchX = 0;
//				offsetAnchY = 0;
//
//				m.tx += offsetAnchX;
//				m.ty += offsetAnchY;	
//				
//				//m.print();
//				m.calculatePara();
//				insData.matrix = m;
//				insData.resName = fromSymbolIns.insData.resName;
//				insData.transformationPoint = fromSymbolIns.insData.transformationPoint;
//				data.instanceSymbolVector.push_back(insData);
//				_frameVector.push_back(data);
//				curFrame++;
//			}
//
//		}
//		else
//		{
//			for(int i = 0; i < domFrameData.duration; ++i)
//			{
//				FrameData data;
//				data.startFrame = domFrameData.index;
//				for(int bitMapIndex = 0; bitMapIndex < domFrameData.bitMapVector.size(); bitMapIndex++)
//				{
//					instanceData insData;
//					insData.matrix = domFrameData.bitMapVector[bitMapIndex].insData.matrix;
//					insData.matrix.calculatePara();
//					insData.resName = domFrameData.bitMapVector[bitMapIndex].insData.resName;
//					insData.transformationPoint = domFrameData.bitMapVector[bitMapIndex].insData.transformationPoint;
//					data.instanceBitMapVector.push_back(insData);
//				}
//
//				for(int bitSymbolIndex = 0; bitSymbolIndex < domFrameData.symbolVector.size(); bitSymbolIndex++)
//				{
//					instanceData insData;
//					data.startFrame = domFrameData.index;
//					insData.matrix = domFrameData.symbolVector[bitSymbolIndex].insData.matrix;
//					insData.matrix.calculatePara();
//					insData.resName = domFrameData.symbolVector[bitSymbolIndex].insData.resName;
//					insData.transformationPoint = domFrameData.symbolVector[bitSymbolIndex].insData.transformationPoint;
//					data.instanceSymbolVector.push_back(insData);
//				}
//
//				_frameVector.push_back(data);
//				curFrame++;
//			}
//
//			it++;		
//		}
//		
//
//	}
//
//}
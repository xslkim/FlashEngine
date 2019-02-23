#include "BoxFlash.h"
#include "Matrix.h"
#include "FlashUtils.h"
#include "BoxFlashInfo.pb.h"



NS_CC_BEGIN

std::map<std::string, BoxFlashInfo*> BoxFlash::s_boxInfoMap;

Matrix getMatrix(const BoxTrans& tran)
{
	Matrix m = Matrix();
	if(tran.has_ma()){ m.a = tran.ma();}
	if(tran.has_mb()){ m.b = tran.mb();}
	if(tran.has_mc()){ m.c = tran.mc();}
	if(tran.has_md()){ m.d = tran.md();}
	if(tran.has_mtx()){ m.tx = tran.mtx();}
	if(tran.has_mty()){ m.ty = tran.mty();}
	return m;
}

BoxFlash* BoxFlash::create(std::string name)
{
	BoxFlash *pobSprite = new BoxFlash();
	if (pobSprite && pobSprite->loadFile(name))
	{
		pobSprite->loadBoxFile(name);
		pobSprite->autorelease();
		pobSprite->setScale(1);
		return pobSprite;
	}
	CC_SAFE_DELETE(pobSprite);
	return NULL;
}

BoxFlash::BoxFlash()
{
	_info = NULL;
	_scriptAttackHandle = 0;
	_lastAttackFrame = -1;
}



void BoxFlash::registerAttackScriptHandler(int handler)
{
	unregisterAttackScriptHandler();
	_scriptAttackHandle = handler;
}
void BoxFlash::unregisterAttackScriptHandler()
{
	if (_scriptAttackHandle)
	{
		ScriptEngineManager::getInstance()->getScriptEngine()->removeScriptHandler(_scriptAttackHandle);
		_scriptAttackHandle = 0;
	}
}

void BoxFlash::onAnimationStart()
{
	_lastAttackFrame = -1;
}

void BoxFlash::frameChange(int form, int to)
{
	if (_scriptAttackHandle != 0)
	{

		while(form != to)
		{

			form++;
			if(form > _curFrame)
			{
				break;
			}

			if( form > _curFrameSize - 1 )
			{
				break;
			}

			if(_lastAttackFrame != form && isAttackFrame(_curAnimination, form))
			{
				_lastAttackFrame = form;
				FlashScriptData scriptData(_scriptAttackHandle, this);
				ScriptEvent scriptEvent(kFlashSpriteEvent,(void*)&scriptData);
				ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&scriptEvent);
				if (!_scriptAttackHandle)
				{
					break;
				}
			}
		}
	}
}


void BoxFlash::loadBoxFile(std::string name)
{
	char flashNameBuf[256] = {0};
	toFileName(flashNameBuf, name.c_str() );
	std::string sName = flashNameBuf;
	std::map<std::string, BoxFlashInfo*>::iterator it = BoxFlash::s_boxInfoMap.find(sName);
	if ( it != BoxFlash::s_boxInfoMap.end())
	{
		_info = it->second;
	}
	else
	{
		std::string fullPathAndName = CCFileUtils::getInstance()->fullPathForFilename(name + ".bb");
		ssize_t nSize;
		unsigned char* pBuffer = CCFileUtils::getInstance()->getFileData(fullPathAndName, "rb", &nSize);
		_info = new BoxFlashInfo();
		if( !_info->ParseFromArray(pBuffer, nSize) )
		{
			CCAssert(false, "");
		}
		BoxFlash::s_boxInfoMap.insert( std::pair<std::string, BoxFlashInfo*>(sName, _info));
		free(pBuffer);
	}
}

bool isHit(const Rect& a, const Rect& b)
{
	if(a.origin.x > b.origin.x + b.size.width 
		|| a.origin.x + a.size.width < b.origin.x
		|| a.origin.y > b.origin.y + b.size.height
		|| a.origin.y + a.size.height < b.origin.y)
	{
		return false;
	}
	return true;
}

bool BoxFlash::isHurtByOther(BoxFlash* other, int offset_z)
{
	std::vector<Rect> attackRect = other->getBoxesRect(true, offset_z);
	std::vector<Rect> hurtRect = getBoxesRect(false, offset_z);
	for (int i = 0; i < attackRect.size(); ++i)
	{
		for (int j = 0; j < hurtRect.size(); ++j)
		{
			if (isHit(attackRect[i], hurtRect[j]))
			{
				return true;
			}
		}
	}
	return false;
}

const BoxDomFrame* getAttackBoxDomFrame(const BoxActionInfo& actionInfo, int frame)
{
	for(int i = 0; i < actionInfo.attackdomframe_size(); ++i)
	{
		const BoxDomFrame& domFrame = actionInfo.attackdomframe(i);
		if (frame >= domFrame.startframe() && frame < domFrame.startframe() + domFrame.duration())
		{
			const BoxDomFrame* outFrame = new BoxDomFrame(domFrame);
			return outFrame;
		}
	}
	return NULL;
}

const BoxDomFrame* getHurtBoxDomFrame(const BoxActionInfo& actionInfo, int frame)
{
	for(int i = 0; i < actionInfo.hurtdomframe_size(); ++i)
	{
		const BoxDomFrame& domFrame = actionInfo.hurtdomframe(i);
		if (frame > domFrame.startframe() && frame < domFrame.startframe() + domFrame.duration())
		{
			const BoxDomFrame* outFrame = new BoxDomFrame(domFrame);
			return outFrame;
		}
	}
	return NULL;
}

std::vector<Rect> BoxFlash::getBoxesRect(bool isAttack, int offset_z)
{
	return getBoxesRect(isAttack, _curAnimination, _curFrame, offset_z);
}

Rect BoxFlash::getAttackDistance()
{
	Rect ct;
	for(int i = _curFrame; i < _curFrameSize; ++i)
	{
		if(isAttackFrame(_curAnimination, _curFrame))
		{
			std::vector<Rect> v = getBoxesRect(true, _curAnimination, i, 0);
			CCAssert(v.size() > 0, "errot getAttackDistance");
			ct = v[0];
			break;
		}
	}
	return ct;
}

Rect BoxFlash::getAttackRect()
{
	std::vector<Rect> v = getBoxesRect(true, 0);
	Rect ct;
	if(v.size() > 0)
	{
		ct = v[0];
	}
	return ct;
}

Rect BoxFlash::getHurtRect()
{
	std::vector<Rect> v = getBoxesRect(false, 0);
	Rect ct;
	if(v.size() > 0)
	{
		ct = v[0];
	}
	return ct;
}

bool BoxFlash::isAttackFrame(int action, int frame)
{
	const BoxActionInfo& actionInfo = _info->actioninfo(action);
	const BoxDomFrame* domFrame = getAttackBoxDomFrame(actionInfo, frame);; 
	bool flg = (domFrame != NULL);
	delete domFrame;
	return flg;
}

#define CC_CALLBACK_0(__selector__,__target__, ...) std::bind(&__selector__,__target__, ##__VA_ARGS__)
#define CC_CALLBACK_1(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, ##__VA_ARGS__)
#define CC_CALLBACK_2(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, ##__VA_ARGS__)
#define CC_CALLBACK_3(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, ##__VA_ARGS__)


void BoxFlash::draw(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
	FlashSprite::draw(renderer, transform, flags);


	//_customCommand.init(_globalZOrder);
    //_customCommand.func = CC_CALLBACK_0(BoxFlash::onBoxDraw, this, transform, flags);
    //renderer->addCommand(&_customCommand);
}

void BoxFlash::onBoxDraw(const Mat4 &transform, uint32_t flags)
{
	Director* director = Director::getInstance();
	director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, Mat4::IDENTITY);
	if(getBoxesRect(true).size() > 0)
	{
		Rect rc = getBoxesRect(true)[0];
		glLineWidth(5);
		DrawPrimitives::setDrawColor4B(255,0,0,255);
		DrawPrimitives::drawLine( rc.origin, ccp(rc.origin.x, rc.origin.y + rc.size.height));
		DrawPrimitives::drawLine( ccp(rc.origin.x, rc.origin.y + rc.size.height), ccp(rc.origin.x + rc.size.width, rc.origin.y + rc.size.height) );
		DrawPrimitives::drawLine( ccp(rc.origin.x + rc.size.width, rc.origin.y + rc.size.height),  ccp(rc.origin.x + rc.size.width, rc.origin.y) );
		DrawPrimitives::drawLine( rc.origin,  ccp(rc.origin.x + rc.size.width, rc.origin.y) );
	}

	if(getBoxesRect(false).size() > 0)
	{
		Rect rc = getBoxesRect(false)[0];
		glLineWidth(1);
		DrawPrimitives::setDrawColor4B(0,255,0,255);
		DrawPrimitives::drawLine( rc.origin, ccp(rc.origin.x, rc.origin.y + rc.size.height));
		DrawPrimitives::drawLine( ccp(rc.origin.x, rc.origin.y + rc.size.height), ccp(rc.origin.x + rc.size.width, rc.origin.y + rc.size.height) );
		DrawPrimitives::drawLine( ccp(rc.origin.x + rc.size.width, rc.origin.y + rc.size.height),  ccp(rc.origin.x + rc.size.width, rc.origin.y) );
		DrawPrimitives::drawLine( rc.origin,  ccp(rc.origin.x + rc.size.width, rc.origin.y) );
	}
	
    director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}



std::vector<Rect> BoxFlash::getBoxesRect(bool isAttack, int action, int frame, int offset_z)
{
	int screenHeight = Director::getInstance()->getWinSizeInPixels().height;
	Size size  =  Size(100, 100);
	MyPoint tr(size.width, 0);
	MyPoint bl(0, size.height);

	const BoxActionInfo& actionInfo = _info->actioninfo(action);
	std::vector<Rect> vec;
	const BoxDomFrame* domFrame = NULL; 
	if (isAttack)
	{
		domFrame = getAttackBoxDomFrame(actionInfo, frame);
	}
	else
	{
		domFrame = getHurtBoxDomFrame(actionInfo, frame);
	}
	if (domFrame != NULL)
	{
		Matrix sceneM = getMatrix(actionInfo.trans());
		for(int i = 0; i < domFrame->trans_size(); ++i)
		{
			Matrix sceneMM = sceneM;
			Matrix m = getMatrix(domFrame->trans(i));
			//m.concatenate(sceneM);
			//Matrix mat = m;
			sceneM.concatenate(m);
			Matrix mat = sceneM;

			MyPoint tr_r, bl_r;
			mat.transform(&tr_r, tr);
			mat.transform(&bl_r, bl);

			tr_r.y = screenHeight - tr_r.y + offset_z;
			bl_r.y = screenHeight - bl_r.y + offset_z;

			if(m_isMirror)
			{
				float temp = bl_r.x;
				bl_r.x = -tr_r.x;
				tr_r.x = -temp;
			}
			
			Rect rect = Rect(bl_r.x, bl_r.y, tr_r.x - bl_r.x, tr_r.y - bl_r.y);
			rect.origin += _position;
			int w = rect.size.width * _scaleX;
			int h = rect.size.height * _scaleY;
			int offsetW = (rect.size.width - w)/2;
			int offsetH = (rect.size.height - h)/2;
			rect.origin.x += offsetW;
			rect.origin.y += offsetH;
			rect.size.width *= _scaleX;
			rect.size.height *= _scaleY;
			vec.push_back(rect);
			
		}
		
		CC_SAFE_DELETE(domFrame);
	}
	return vec;
}


NS_CC_END
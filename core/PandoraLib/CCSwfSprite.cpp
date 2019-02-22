#include "CCSwfSprite.h"
//#include "CCSpriteFrame.h"
//#include "CCSpriteFrameCache.h"
#include "CCDirector.h"
//#include "CCSpriteFrame.h"
//#include "PandoraConfig.h"
#include "Matrix.h"
//#include "PandoraUtils.h"

using namespace cocos2d;

CCSwfSprite* CCSwfSprite::spriteWithSpriteFrameName(const char *pszSpriteFrameName)
{
	CCSpriteFrame *pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(pszSpriteFrameName);

	char msg[256] = {0};
	sprintf(msg, "Invalid spriteFrameName: %s", pszSpriteFrameName);
    //CCLOG("%s",pszSpriteFrameName);
	CCAssert(pFrame != NULL, msg);
	CCSwfSprite *pobSprite = new CCSwfSprite();
	if (pobSprite && pobSprite->initWithSpriteFrame(pFrame))
	{
		pobSprite->autorelease();
		return pobSprite;
	}
	CC_SAFE_DELETE(pobSprite);
	return NULL;
}

CCSwfSprite* CCSwfSprite::create(CCSpriteFrame* sptframe)
{
	CCSwfSprite *pobSprite = new CCSwfSprite();
	if (pobSprite && pobSprite->initWithSpriteFrame(sptframe))
	{
		pobSprite->autorelease();
		return pobSprite;
	}
	CC_SAFE_DELETE(pobSprite);
	return NULL;
}

CCSwfSprite* CCSwfSprite::create(cocos2d::CCSpriteFrame* sptframe, cocos2d::CCSize size)
{
	CCSwfSprite *pobSprite = new CCSwfSprite();
	if (pobSprite && pobSprite->initWithSpriteFrame(sptframe))
	{
		pobSprite->autorelease();
		pobSprite->setContentSize(size);
		return pobSprite;
	}
	CC_SAFE_DELETE(pobSprite);
	return NULL;
}

GLubyte CCSwfSprite::getFlashOpacity(void)
{
	return m_nFlashOpacity;
}

void CCSwfSprite::setFlashOpacity(GLubyte opacity)
{
	m_nFlashOpacity = opacity; 
}

cocos2d::ccColor3B CCSwfSprite::getFlashColor(void)
{
	return m_sFlashColor; 
}

void CCSwfSprite::setFlashColor(cocos2d::ccColor3B color3)
{
	m_sFlashColor = color3; 
}

void CCSwfSprite::setIsMirror(bool mirror)
{
	if(mirror == m_isMirror)
		return;
	m_bDirty = m_bRecursiveDirty = false;
	m_isMirror = mirror; 
}

bool CCSwfSprite::isMirror()
{
	return m_isMirror;
}

CCSwfSprite* CCSwfSprite::create(const char *picName)
{
	CCSwfSprite *pobSprite = new CCSwfSprite();
	if (pobSprite && pobSprite->initWithFile(picName))
	{
		pobSprite->autorelease();
		return pobSprite;
	}
	CC_SAFE_DELETE(pobSprite);
	return NULL;
}

CCSwfSprite::CCSwfSprite()
{
	isUsed = false;
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	_className = "CCSwfSprite";
#endif
}

CCSwfSprite::~CCSwfSprite()
{

}

void CCSwfSprite::setVertex(cocos2d::ccVertex3F tl, cocos2d::ccVertex3F bl, cocos2d::ccVertex3F tr, cocos2d::ccVertex3F br)
{
	_tl = tl;
	_bl = bl;
	_tr = tr;
	_br = br;
}


void CCSwfSprite::visit()
{
	//CCSprite::visit();
	draw(_tl, _bl, _tr, _br);
}


#define CHANGE_TO_FLOAT(x) ((float)x/255.f)


void CCSwfSprite::updateTransform(Matrix mat, const FlashMetaData& metaData, int screenHeight)
{
	CCSize size  =  this->getContentSize();
	size.width *= CC_CONTENT_SCALE_FACTOR();
	size.height *= CC_CONTENT_SCALE_FACTOR();
	Point tl(0, 0);
	Point tr(size.width, 0);
	Point bl(0, size.height);
	Point br(size.width, size.height);

	Point tl_r, tr_r, bl_r, br_r;

	mat.transform(&tl_r, tl);
	mat.transform(&tr_r, tr);
	mat.transform(&bl_r, bl);
	mat.transform(&br_r, br);

	//TargetPlatform platform = getPlatformTarget();

	tl_r.y = screenHeight - tl_r.y;
	tr_r.y = screenHeight - tr_r.y;
	bl_r.y = screenHeight - bl_r.y;
	br_r.y = screenHeight - br_r.y;
	

	//tl_r.x += metaData.left();
	//tl_r.y -= metaData.top();

	//tr_r.x -= metaData.right();
	//tr_r.y -= metaData.top();
	//
	//bl_r.x += metaData.left();
	//bl_r.y += metaData.button();

	//br_r.x -= metaData.right();
	//br_r.y += metaData.button();

	tl_r.x /= CC_CONTENT_SCALE_FACTOR();
	tl_r.y /= CC_CONTENT_SCALE_FACTOR();

	tr_r.x /= CC_CONTENT_SCALE_FACTOR();
	tr_r.y /= CC_CONTENT_SCALE_FACTOR();

	bl_r.x /= CC_CONTENT_SCALE_FACTOR();
	bl_r.y /= CC_CONTENT_SCALE_FACTOR();

	br_r.x /= CC_CONTENT_SCALE_FACTOR();
	br_r.y /= CC_CONTENT_SCALE_FACTOR();
	

	tl_r.setCocos2dPoint( _tl );
	tr_r.setCocos2dPoint( _tr );
	bl_r.setCocos2dPoint( _bl );
	br_r.setCocos2dPoint( _br );

	//if( m_isMirror )
	//{
	//	m_sQuad.bl.vertices = vertex3(-_bl.x * m_fScaleX, _bl.y * m_fScaleY, _bl.z);
	//	m_sQuad.br.vertices = vertex3(-_br.x * m_fScaleX, _br.y * m_fScaleY, _br.z);
	//	m_sQuad.tl.vertices = vertex3(-_tl.x * m_fScaleX, _tl.y * m_fScaleY, _tl.z);
	//	m_sQuad.tr.vertices = vertex3(-_tr.x * m_fScaleX, _tr.y * m_fScaleY, _tr.z);
	//}
	//else
	//{
	//	m_sQuad.bl.vertices = vertex3(_bl.x * m_fScaleX, _bl.y * m_fScaleY, _bl.z);
	//	m_sQuad.br.vertices = vertex3(_br.x * m_fScaleX, _br.y * m_fScaleY, _br.z);
	//	m_sQuad.tl.vertices = vertex3(_tl.x * m_fScaleX, _tl.y * m_fScaleY, _tl.z);
	//	m_sQuad.tr.vertices = vertex3(_tr.x * m_fScaleX, _tr.y * m_fScaleY, _tr.z);
	//}
}
void CCSwfSprite::draw(cocos2d::ccVertex3F tl, cocos2d::ccVertex3F bl, cocos2d::ccVertex3F tr, cocos2d::ccVertex3F br)
{
	CC_PROFILER_START_CATEGORY(kCCProfilerCategorySprite, "CCSprite - draw");

	CCAssert(!m_pobBatchNode, "If CCSprite is being rendered by CCSpriteBatchNode, CCSprite#draw SHOULD NOT be called");

	CC_NODE_DRAW_SETUP();

	ccGLBlendFunc( m_sBlendFunc.src, m_sBlendFunc.dst );

	if (m_pobTexture != NULL)
	{
		ccGLBindTexture2D( m_pobTexture->getName() );
	}
	else
	{
		ccGLBindTexture2D(0);
	}

	if( m_isMirror )
	{
		tl.x = -tl.x;
		bl.x = -bl.x;
		tr.x = -tr.x;
		br.x = -br.x;
	}

	m_sQuad.tl.vertices = tl;
	m_sQuad.bl.vertices = bl;
	m_sQuad.tr.vertices = tr;
	m_sQuad.br.vertices = br;

	//
	// Attributes
	//

	ccGLEnableVertexAttribs( kCCVertexAttribFlag_PosColorTex );

#define kQuadSize sizeof(m_sQuad.bl)
	long offset = (long)&m_sQuad;

	// vertex
	int diff = offsetof( ccV3F_C4B_T2F, vertices);
	glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, kQuadSize, (void*) (offset + diff));

	// texCoods
	diff = offsetof( ccV3F_C4B_T2F, texCoords);
	glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, kQuadSize, (void*)(offset + diff));

	// color
	diff = offsetof( ccV3F_C4B_T2F, colors);
	glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, kQuadSize, (void*)(offset + diff));


	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	CHECK_GL_ERROR_DEBUG();


#if CC_SPRITE_DEBUG_DRAW == 1
	// draw bounding box
	CCPoint vertices[4]={
		ccp(m_sQuad.tl.vertices.x,m_sQuad.tl.vertices.y),
		ccp(m_sQuad.bl.vertices.x,m_sQuad.bl.vertices.y),
		ccp(m_sQuad.br.vertices.x,m_sQuad.br.vertices.y),
		ccp(m_sQuad.tr.vertices.x,m_sQuad.tr.vertices.y),
	};
	ccDrawPoly(vertices, 4, true);
#elif CC_SPRITE_DEBUG_DRAW == 2
	// draw texture box
	CCSize s = this->getTextureRect().size;
	CCPoint offsetPix = this->getOffsetPosition();
	CCPoint vertices[4] = {
		ccp(offsetPix.x,offsetPix.y), ccp(offsetPix.x+s.width,offsetPix.y),
		ccp(offsetPix.x+s.width,offsetPix.y+s.height), ccp(offsetPix.x,offsetPix.y+s.height)
	};
	ccDrawPoly(vertices, 4, true);
#endif // CC_SPRITE_DEBUG_DRAW

	CC_INCREMENT_GL_DRAWS(1);

	CC_PROFILER_STOP_CATEGORY(kCCProfilerCategorySprite, "CCSprite - draw");
}

cocos2d::ccVertex3F CCSwfSprite::ratatePoint(cocos2d::ccVertex3F point, float angle)
{
	float anglePi = CC_DEGREES_TO_RADIANS(angle);
	cocos2d::ccVertex3F out = point;
	out.x = point.x * cos(anglePi) - point.y * sin(anglePi);
	out.y = point.y * cos(anglePi) + point.x * sin(anglePi);
	return out;
}



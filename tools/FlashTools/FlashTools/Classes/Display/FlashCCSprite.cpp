#include "FlashCCSprite.h"
#include "CCSpriteFrame.h"
#include "CCSpriteFrameCache.h"
#include "CCDirector.h"
#include "DataLogicMgr.h"

using namespace cocos2d;

FlashCCSprite* FlashCCSprite::spriteWithSpriteFrameName(const char *pszSpriteFrameName)
{
	CCSpriteFrame *pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(pszSpriteFrameName);

	char msg[256] = {0};
	sprintf(msg, "Invalid spriteFrameName: %s", pszSpriteFrameName);
	CCAssert(pFrame != NULL, msg);
	FlashCCSprite *pobSprite = new FlashCCSprite();
	if (pobSprite && pobSprite->initWithSpriteFrame(pFrame))
	{
		//pobSprite->autorelease();
		return pobSprite;
	}
	CC_SAFE_DELETE(pobSprite);
	return NULL;
}

FlashCCSprite* FlashCCSprite::spriteWithFile(const char* pszFileName)
{
	FlashCCSprite *pobSprite = new FlashCCSprite();
	if (pobSprite && pobSprite->initWithFile(pszFileName))
	{
		//pobSprite->autorelease();
		return pobSprite;
	}
	CC_SAFE_DELETE(pobSprite);
	return NULL;
}

void FlashCCSprite::updateVertex(Matrix m)
{
	CCSize size = getContentSize();

	Point tl(0, 0);
	Point tr(size.width, 0);
	Point bl(0, size.height);
	Point br(size.width, size.height);

	Point tl_r, tr_r, bl_r, br_r;

	m.transform(&tl_r, tl);
	m.transform(&tr_r, tr);
	m.transform(&bl_r, bl);
	m.transform(&br_r, br);

	int screenHeight = getDataLogicMgr()->getScreenHeight();

	tl_r.y = screenHeight - tl_r.y;
	tr_r.y = screenHeight - tr_r.y;
	bl_r.y = screenHeight - bl_r.y;
	br_r.y = screenHeight - br_r.y;
	
	

	tl_r.setCocos2dPoint( _tl );
	tr_r.setCocos2dPoint( _tr );
	bl_r.setCocos2dPoint( _bl );
	br_r.setCocos2dPoint( _br );

}

void FlashCCSprite::updateColor(double a, double r, double g, double b)
{
	this->setOpacity( a*255 );
	this->setColor(ccc3( r*255, g*255, b*255));
}

void FlashCCSprite::setVertex(cocos2d::ccVertex3F tl, cocos2d::ccVertex3F bl, cocos2d::ccVertex3F tr, cocos2d::ccVertex3F br)
{
	_tl = tl;
	_bl = bl;
	_tr = tr;
	_br = br;
}


void FlashCCSprite::visit()
{
	//CCSprite::visit();
	draw(_tl, _bl, _tr, _br);
}

void FlashCCSprite::draw(void)
{
	draw(_tl, _bl, _tr, _br);
}

void FlashCCSprite::updateTransform(void)
{
	CCAssert(m_bUsesBatchNode);

	if (! m_bDirty)
	{
		return;
	}

	// Optimization: if it is not visible, then do nothing
	if (! m_bIsVisible)
	{
		m_sQuad.br.vertices = m_sQuad.tl.vertices = m_sQuad.tr.vertices = m_sQuad.bl.vertices = vertex3(0,0,0);
		m_pobTextureAtlas->updateQuad(&m_sQuad, m_uAtlasIndex);
		m_bDirty = m_bRecursiveDirty = false;
		return;
	}

	
	//float scale = CCDirector::sharedDirector()->getContentScaleFactor();
	//if( scale == 2 )
	//{
	//	m_sQuad.bl.vertices = vertex3(_bl.x, _bl.y, _bl.z);
	//	m_sQuad.br.vertices = vertex3(_br.x, _br.y, _br.z);
	//	m_sQuad.tl.vertices = vertex3(_tl.x, _tl.y, _tl.z);
	//	m_sQuad.tr.vertices = vertex3(_tr.x, _tr.y, _tr.z);

	//}
	//else
	//{

	//	m_sQuad.bl.vertices = vertex3(_bl.x/2 * scale, _bl.y/2 * scale, _bl.z);
	//	m_sQuad.br.vertices = vertex3(_br.x/2 * scale, _br.y/2 * scale, _br.z);
	//	m_sQuad.tl.vertices = vertex3(_tl.x/2 * scale, _tl.y/2 * scale, _tl.z);
	//	m_sQuad.tr.vertices = vertex3(_tr.x/2 * scale, _tr.y/2 * scale, _tr.z);
	//
	//}

	m_sQuad.bl.vertices = vertex3(_bl.x, _bl.y, _bl.z);
	m_sQuad.br.vertices = vertex3(_br.x, _br.y, _br.z);
	m_sQuad.tl.vertices = vertex3(_tl.x, _tl.y, _tl.z);
	m_sQuad.tr.vertices = vertex3(_tr.x, _tr.y, _tr.z);

	m_pobTextureAtlas->updateQuad(&m_sQuad, m_uAtlasIndex);
	m_bDirty = m_bRecursiveDirty = false;
}

void FlashCCSprite::draw(cocos2d::ccVertex3F tl, cocos2d::ccVertex3F bl, cocos2d::ccVertex3F tr, cocos2d::ccVertex3F br)
{
	CCAssert(! m_bUsesBatchNode);

	// Default GL states: GL_TEXTURE_2D, GL_VERTEX_ARRAY, GL_COLOR_ARRAY, GL_TEXTURE_COORD_ARRAY
	// Needed states: GL_TEXTURE_2D, GL_VERTEX_ARRAY, GL_COLOR_ARRAY, GL_TEXTURE_COORD_ARRAY
	// Unneeded states: -
	bool newBlend = m_sBlendFunc.src != CC_BLEND_SRC || m_sBlendFunc.dst != CC_BLEND_DST;
	if (newBlend)
	{
		glBlendFunc(m_sBlendFunc.src, m_sBlendFunc.dst);
	}

#define kQuadSize sizeof(m_sQuad.bl)
	if (m_pobTexture)
	{
		glBindTexture(GL_TEXTURE_2D, m_pobTexture->getName());
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	tl.x += m_tPositionInPixels.x;
	tl.y += m_tPositionInPixels.y;

	bl.x += m_tPositionInPixels.x;
	bl.y += m_tPositionInPixels.y;

	tr.x += m_tPositionInPixels.x;
	tr.y += m_tPositionInPixels.y;

	br.x += m_tPositionInPixels.x;
	br.y += m_tPositionInPixels.y;

	m_sQuad.tl.vertices = tl;
	m_sQuad.bl.vertices = bl;
	m_sQuad.tr.vertices = tr;
	m_sQuad.br.vertices = br;

	long offset = (long)&m_sQuad;

	// vertex
	int diff = offsetof(cocos2d::ccV3F_C4B_T2F, vertices);
	glVertexPointer(3, GL_FLOAT, kQuadSize, (void*)(offset + diff));

	// color
	diff = offsetof( cocos2d::ccV3F_C4B_T2F, colors);
	glColorPointer(4, GL_UNSIGNED_BYTE, kQuadSize, (void*)(offset + diff));

	// tex coords
	diff = offsetof( cocos2d::ccV3F_C4B_T2F, texCoords);
	glTexCoordPointer(2, GL_FLOAT, kQuadSize, (void*)(offset + diff));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	if( newBlend )
	{
		glBlendFunc(CC_BLEND_SRC, CC_BLEND_DST);
	}

#if CC_SPRITE_DEBUG_DRAW == 1
	// draw bounding box
	CCSize s = m_tContentSize;
	CCPoint vertices[4] = {
		ccp(0,0), ccp(s.width,0),
		ccp(s.width,s.height), ccp(0,s.height)
	};
	ccDrawPoly(vertices, 4, true);
#elif CC_SPRITE_DEBUG_DRAW == 2
	// draw texture box
	CCSize s = m_obRect.size;
	CCPoint offsetPix = getOffsetPositionInPixels();
	CCPoint vertices[4] = {
		ccp(offsetPix.x,offsetPix.y), ccp(offsetPix.x+s.width,offsetPix.y),
		ccp(offsetPix.x+s.width,offsetPix.y+s.height), ccp(offsetPix.x,offsetPix.y+s.height)
	};
	ccDrawPoly(vertices, 4, true);
#endif // CC_SPRITE_DEBUG_DRAW

}
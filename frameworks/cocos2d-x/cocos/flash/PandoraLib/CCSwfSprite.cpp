#include "CCSwfSprite.h"
//#include "CCSpriteFrame.h"
//#include "CCSpriteFrameCache.h"
//#include "CCDirector.h"
//#include "CCSpriteFrame.h"
//#include "PandoraConfig.h"
#include "Matrix.h"
#include "FlashUtils.h"
//#include "PandoraUtils.h"

NS_CC_BEGIN

CCSwfSprite* CCSwfSprite::spriteWithSpriteFrameName(const char *pszSpriteFrameName)
{
	SpriteFrame *pFrame = CCSpriteFrameCache::getInstance()->getSpriteFrameByName(pszSpriteFrameName);

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

CCSwfSprite* CCSwfSprite::create(SpriteFrame* sptframe)
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

CCSwfSprite* CCSwfSprite::create(cocos2d::SpriteFrame* sptframe, cocos2d::Size size)
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

void CCSwfSprite::setGrey(bool grey)
{
	if(grey)
	{
		setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_GREY));
	}else{
		setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP));
	}
}

void CCSwfSprite::setBright(bool grey)
{
	if(grey)
	{
		setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_BRIGHT));
	}else{
		setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP));
	}
}

void CCSwfSprite::setRed(int red)
{
	_changeColor = false;
	if(red == 100){
		setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_RED100));
	}else if(red == 80){
		setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_RED80));
	}else if(red == 60){
		setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_RED60));
	}else if(red == 40){
		setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_RED40));
	}else if(red == 20){
		setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_RED20));
	}else{
		setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP));
	}
}
void CCSwfSprite::setGreen()
{
	_changeColor = false;
	setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_GREEN100));
}
void CCSwfSprite::setGolden()
{
	_changeColor = false;
	setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_GOLDEN100));
}

void CCSwfSprite::setBurn()
{
	_changeColor = false;
	setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_BURN100));
}

void CCSwfSprite::setOriginColor()
{
	if (_changeColor == false)
	{
		_changeColor = true;
		setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP));
	}
}

GLubyte CCSwfSprite::getFlashOpacity(void)
{
	return m_nFlashOpacity;
}

void CCSwfSprite::setFlashOpacity(GLubyte opacity)
{
	m_nFlashOpacity = opacity; 
}

cocos2d::Color3B CCSwfSprite::getFlashColor(void)
{
	return m_sFlashColor; 
}

void CCSwfSprite::setFlashColor(cocos2d::Color3B color3)
{
	m_sFlashColor = color3; 
}

void CCSwfSprite::setIsMirror(bool mirror)
{
 	if(mirror == m_isMirror)
 		return;
 	_dirty = _recursiveDirty = false;
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
	_changeColor = true;
}

CCSwfSprite::~CCSwfSprite()
{

}

void CCSwfSprite::setVertex(cocos2d::Vec3 tl, cocos2d::Vec3 bl, cocos2d::Vec3 tr, cocos2d::Vec3 br)
{
	_tl = tl;
	_bl = bl;
	_tr = tr;
	_br = br;
}


// void CCSwfSprite::visit()
// {
// 	//CCSprite::visit();
// 	draw(_tl, _bl, _tr, _br);
// }


#define CHANGE_TO_FLOAT(x) ((float)x/255.f)

#if (defined USE_FLASH_METADATA_CACHE && USE_FLASH_METADATA_CACHE == 1)

void CCSwfSprite::updateTransform(Matrix mat, const FlashMetaDataEx& metaData, int screenHeight)
{
 	Size size  =  this->getContentSize();
 	size.width *= CC_CONTENT_SCALE_FACTOR();
 	size.height *= CC_CONTENT_SCALE_FACTOR();
 	MyPoint tl(0, 0);
 	MyPoint tr(size.width, 0);
 	MyPoint bl(0, size.height);
 	MyPoint br(size.width, size.height);
 
 	MyPoint tl_r, tr_r, bl_r, br_r;
 
 	mat.transform(&tl_r, tl);
 	mat.transform(&tr_r, tr);
 	mat.transform(&bl_r, bl);
 	mat.transform(&br_r, br);
 
 	//TargetPlatform platform = getPlatformTarget();
 
 	tl_r.y = screenHeight - tl_r.y;
 	tr_r.y = screenHeight - tr_r.y;
 	bl_r.y = screenHeight - bl_r.y;
 	br_r.y = screenHeight - br_r.y;
 	
 
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
    
    if (m_isMirror) {
        _tl.x = -_tl.x;
		_bl.x = -_bl.x;
		_tr.x = -_tr.x;
		_br.x = -_br.x;
    }
    
    // Don't update Z.
    _quad.bl.vertices = _bl;
    _quad.br.vertices = _br;
    _quad.tl.vertices = _tl;
    _quad.tr.vertices = _tr;

}

#else

void CCSwfSprite::updateTransform(Matrix mat, const FlashMetaData& metaData, int screenHeight)
{
	Size size  =  this->getContentSize();
	size.width *= CC_CONTENT_SCALE_FACTOR();
	size.height *= CC_CONTENT_SCALE_FACTOR();
	MyPoint tl(0, 0);
	MyPoint tr(size.width, 0);
	MyPoint bl(0, size.height);
	MyPoint br(size.width, size.height);

	MyPoint tl_r, tr_r, bl_r, br_r;

	mat.transform(&tl_r, tl);
	mat.transform(&tr_r, tr);
	mat.transform(&bl_r, bl);
	mat.transform(&br_r, br);

	//TargetPlatform platform = getPlatformTarget();

	tl_r.y = screenHeight - tl_r.y;
	tr_r.y = screenHeight - tr_r.y;
	bl_r.y = screenHeight - bl_r.y;
	br_r.y = screenHeight - br_r.y;


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

	if (m_isMirror) {
		_tl.x = -_tl.x;
		_bl.x = -_bl.x;
		_tr.x = -_tr.x;
		_br.x = -_br.x;
	}

	// Don't update Z.
	_quad.bl.vertices = _bl;
	_quad.br.vertices = _br;
	_quad.tl.vertices = _tl;
	_quad.tr.vertices = _tr;

}

#endif


void CCSwfSprite::visit(Renderer* renderer, const Mat4 &parentTransform, uint32_t parentFlags)
{
    uint32_t flags = processParentFlags(parentTransform, parentFlags);
    Director* director = Director::getInstance();
    director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, _modelViewTransform);
    draw(renderer, parentTransform, flags);
    director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}

void CCSwfSprite::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
    // Don't do calculate the culling if the transform was not updated
    _insideBounds = true; //(flags & FLAGS_TRANSFORM_DIRTY) ? renderer->checkVisibility(transform, _contentSize) : _insideBounds;
    
    if(_insideBounds)
    {
        _quadCommand.init(_globalZOrder, _texture->getName(), getGLProgramState(), _blendFunc, &_quad, 1, transform);
        renderer->addCommand(&_quadCommand);
#if CC_SPRITE_DEBUG_DRAW
        _customDebugDrawCommand.init(_globalZOrder);
        _customDebugDrawCommand.func = CC_CALLBACK_0(Sprite::drawDebugData, this);
        renderer->addCommand(&_customDebugDrawCommand);
#endif //CC_SPRITE_DEBUG_DRAW
    }
}

//void CCSwfSprite::draw(cocos2d::Vec3 tl, cocos2d::Vec3 bl, cocos2d::Vec3 tr, cocos2d::Vec3 br)
//{
// 	CC_PROFILER_START_CATEGORY(kCCProfilerCategorySprite, "CCSprite - draw");
// 
// 	CCAssert(!m_pobBatchNode, "If CCSprite is being rendered by CCSpriteBatchNode, CCSprite#draw SHOULD NOT be called");
// 
// 	CC_NODE_DRAW_SETUP();
// 
// 	ccGLBlendFunc( m_sBlendFunc.src, m_sBlendFunc.dst );
// 
// 	if (m_pobTexture != NULL)
// 	{
// 		ccGLBindTexture2D( m_pobTexture->getName() );
// 	}
// 	else
// 	{
// 		ccGLBindTexture2D(0);
// 	}
// 
// 	if( m_isMirror )
// 	{
// 		tl.x = -tl.x;
// 		bl.x = -bl.x;
// 		tr.x = -tr.x;
// 		br.x = -br.x;
// 	}
// 
// 	m_sQuad.tl.vertices = tl;
// 	m_sQuad.bl.vertices = bl;
// 	m_sQuad.tr.vertices = tr;
// 	m_sQuad.br.vertices = br;
// 
// 	//
// 	// Attributes
// 	//
// 
// 	ccGLEnableVertexAttribs( kCCVertexAttribFlag_PosColorTex );
// 
// #define kQuadSize sizeof(m_sQuad.bl)
// 	long offset = (long)&m_sQuad;
// 
// 	// vertex
// 	int diff = offsetof( ccV3F_C4B_T2F, vertices);
// 	glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, kQuadSize, (void*) (offset + diff));
// 
// 	// texCoods
// 	diff = offsetof( ccV3F_C4B_T2F, texCoords);
// 	glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, kQuadSize, (void*)(offset + diff));
// 
// 	// color
// 	diff = offsetof( ccV3F_C4B_T2F, colors);
// 	glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, kQuadSize, (void*)(offset + diff));
// 
// 
// 	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
// 
// 	CHECK_GL_ERROR_DEBUG();
// 
// 
// #if CC_SPRITE_DEBUG_DRAW == 1
// 	// draw bounding box
// 	CCPoint vertices[4]={
// 		ccp(m_sQuad.tl.vertices.x,m_sQuad.tl.vertices.y),
// 		ccp(m_sQuad.bl.vertices.x,m_sQuad.bl.vertices.y),
// 		ccp(m_sQuad.br.vertices.x,m_sQuad.br.vertices.y),
// 		ccp(m_sQuad.tr.vertices.x,m_sQuad.tr.vertices.y),
// 	};
// 	ccDrawPoly(vertices, 4, true);
// #elif CC_SPRITE_DEBUG_DRAW == 2
// 	// draw texture box
// 	CCSize s = this->getTextureRect().size;
// 	CCPoint offsetPix = this->getOffsetPosition();
// 	CCPoint vertices[4] = {
// 		ccp(offsetPix.x,offsetPix.y), ccp(offsetPix.x+s.width,offsetPix.y),
// 		ccp(offsetPix.x+s.width,offsetPix.y+s.height), ccp(offsetPix.x,offsetPix.y+s.height)
// 	};
// 	ccDrawPoly(vertices, 4, true);
// #endif // CC_SPRITE_DEBUG_DRAW
// 
// 	CC_INCREMENT_GL_DRAWS(1);
// 
// 	CC_PROFILER_STOP_CATEGORY(kCCProfilerCategorySprite, "CCSprite - draw");
//}

cocos2d::Vec3 CCSwfSprite::ratatePoint(cocos2d::Vec3 point, float angle)
{
	float anglePi = CC_DEGREES_TO_RADIANS(angle);
	cocos2d::Vec3 out = point;
	out.x = point.x * cos(anglePi) - point.y * sin(anglePi);
	out.y = point.y * cos(anglePi) + point.x * sin(anglePi);
	return out;
}
NS_CC_END


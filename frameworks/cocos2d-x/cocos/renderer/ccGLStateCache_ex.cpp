/****************************************************************************
Copyright (c) 2011      Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (C) 2013-2014 Chukong Technologies Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "renderer/ccGLStateCache.h"

#include "renderer/CCGLProgram.h"
#include "base/CCDirector.h"
#include "base/ccConfig.h"
#include "base/CCConfiguration.h"

NS_CC_BEGIN

static const int MAX_ATTRIBUTES = 16;
static const int MAX_ACTIVE_TEXTURE = 16;

namespace
{
    static GLuint s_currentProjectionMatrix = -1;
    static uint32_t s_attributeFlags = 0;  // 32 attributes max

#if CC_ENABLE_GL_STATE_CACHE

    static GLuint    s_currentShaderProgram = -1;
    static GLuint    s_currentBoundTexture[MAX_ACTIVE_TEXTURE] =  {(GLuint)-1,(GLuint)-1,(GLuint)-1,(GLuint)-1, (GLuint)-1,(GLuint)-1,(GLuint)-1,(GLuint)-1, (GLuint)-1,(GLuint)-1,(GLuint)-1,(GLuint)-1, (GLuint)-1,(GLuint)-1,(GLuint)-1,(GLuint)-1, };
    static GLenum    s_blendingSource = -1;
    static GLenum    s_blendingDest = -1;
    static int       s_GLServerState = 0;
    static GLuint    s_VAO = 0;
    static GLenum    s_activeTexture = -1;

#endif // CC_ENABLE_GL_STATE_CACHE
}

// GL State Cache functions

namespace GL {

void invalidateStateCache( void )
{
    Director::getInstance()->resetMatrixStack();
    s_currentProjectionMatrix = -1;
    s_attributeFlags = 0;

#if CC_ENABLE_GL_STATE_CACHE
    s_currentShaderProgram = -1;
    for( int i=0; i < MAX_ACTIVE_TEXTURE; i++ )
    {
        s_currentBoundTexture[i] = -1;
    }

    s_blendingSource = -1;
    s_blendingDest = -1;
    s_GLServerState = 0;
    s_VAO = 0;
    
#endif // CC_ENABLE_GL_STATE_CACHE
}

void deleteProgram( GLuint program )
{

}

void useProgram( GLuint program )
{

}

static void SetBlending(GLenum sfactor, GLenum dfactor)
{
	
}

void blendFunc(GLenum sfactor, GLenum dfactor)
{

}

void blendResetToCache(void)
{
	
}

void bindTexture2D(GLuint textureId)
{
   
}

void bindTexture2DN(GLuint textureUnit, GLuint textureId)
{

}


void deleteTexture(GLuint textureId)
{

}

void deleteTextureN(GLuint textureUnit, GLuint textureId)
{
    
}

void activeTexture(GLenum texture)
{

}

void bindVAO(GLuint vaoId)
{
   
}

// GL Vertex Attrib functions

void enableVertexAttribs(uint32_t flags)
{
   
}

// GL Uniforms functions

void setProjectionMatrixDirty( void )
{

}

} // Namespace GL

NS_CC_END

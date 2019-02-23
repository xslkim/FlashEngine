/****************************************************************************
Copyright 2011 Jeff Lamarche
Copyright 2012 Goffredo Marocchi
Copyright 2012 Ricardo Quesada
Copyright 2012 cocos2d-x.org
Copyright 2013-2014 Chukong Technologies Inc.

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
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN false EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "renderer/CCGLProgramState.h"

#include "renderer/CCGLProgram.h"
#include "renderer/CCGLProgramStateCache.h"
#include "renderer/CCGLProgramCache.h"
#include "renderer/ccGLStateCache.h"
#include "renderer/CCTexture2D.h"
#include "base/CCEventCustom.h"
#include "base/CCEventListenerCustom.h"
#include "base/CCEventType.h"
#include "base/CCDirector.h"

NS_CC_BEGIN

//
//
// UniformValue
//
//

UniformValue::UniformValue()
: _useCallback(false)
, _uniform(nullptr)
, _glprogram(nullptr)
{
}

UniformValue::UniformValue(Uniform *uniform, GLProgram* glprogram)
: _useCallback(false)
, _uniform(uniform)
, _glprogram(glprogram)
{
}

UniformValue::~UniformValue()
{
	if (_useCallback)
		delete _value.callback;
}

void UniformValue::apply()
{
   
}

void UniformValue::setCallback(const std::function<void(GLProgram*, Uniform*)> &callback)
{
	// delete previously set callback
	// XXX TODO: memory will leak if the user does:
	//    value->setCallback();
	//    value->setFloat();
	if (_useCallback)
		delete _value.callback;

    _value.callback = new std::function<void(GLProgram*, Uniform*)>();
	*_value.callback = callback;

    _useCallback = true;
}

void UniformValue::setFloat(float value)
{
    CCASSERT (_uniform->type == GL_FLOAT, "");
    _value.floatValue = value;
    _useCallback = false;
}

void UniformValue::setTexture(GLuint textureId, GLuint textureUnit)
{
   
}
void UniformValue::setInt(int value)
{
  
}

void UniformValue::setVec2(const Vec2& value)
{
   
}

void UniformValue::setVec3(const Vec3& value)
{
   
}

void UniformValue::setVec4(const Vec4& value)
{
   
}

void UniformValue::setMat4(const Mat4& value)
{
  
}

//
//
// VertexAttribValue
//
//

VertexAttribValue::VertexAttribValue()
: _useCallback(false)
, _vertexAttrib(nullptr)
, _enabled(false)
{
}

VertexAttribValue::VertexAttribValue(VertexAttrib *vertexAttrib)
: _useCallback(false)
, _vertexAttrib(vertexAttrib)
, _enabled(false)
{
}

VertexAttribValue::~VertexAttribValue()
{
	if (_useCallback)
		delete _value.callback;
}

void VertexAttribValue::apply()
{
   
}

void VertexAttribValue::setCallback(const std::function<void(VertexAttrib*)> &callback)
{
	
}

void VertexAttribValue::setPointer(GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLvoid *pointer)
{
   
}

//
//
// GLProgramState
//
//

GLProgramState* GLProgramState::create(GLProgram *glprogram)
{
    return nullptr;
}

GLProgramState* GLProgramState::getOrCreateWithGLProgramName(const std::string &glProgramName )
{
    return nullptr;
}


GLProgramState* GLProgramState::getOrCreateWithGLProgram(GLProgram *glprogram)
{
    GLProgramState* ret = GLProgramStateCache::getInstance()->getGLProgramState(glprogram);
    return ret;
}

GLProgramState::GLProgramState()
: _vertexAttribsFlags(0)
, _glprogram(nullptr)
, _textureUnitIndex(1)
, _uniformAttributeValueDirty(true)
{

}

GLProgramState::~GLProgramState()
{

}

bool GLProgramState::init(GLProgram* glprogram)
{
   
    return true;
}

void GLProgramState::resetGLProgram()
{
  
}

void GLProgramState::apply(const Mat4& modelView)
{
    applyGLProgram(modelView);

    applyAttributes();

    applyUniforms();
}

void GLProgramState::applyGLProgram(const Mat4& modelView)
{
   
}
void GLProgramState::applyAttributes(bool applyAttribFlags)
{
   
}
void GLProgramState::applyUniforms()
{
   
}

void GLProgramState::setGLProgram(GLProgram *glprogram)
{
  
}

UniformValue* GLProgramState::getUniformValue(GLint uniformLocation)
{
  return nullptr;
}

UniformValue* GLProgramState::getUniformValue(const std::string &name)
{
	
	return nullptr;
}

VertexAttribValue* GLProgramState::getVertexAttribValue(const std::string &name)
{
    return nullptr;
}

// VertexAttrib Setters
void GLProgramState::setVertexAttribCallback(const std::string &name, const std::function<void(VertexAttrib*)> &callback)
{
   
}

void GLProgramState::setVertexAttribPointer(const std::string &name, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLvoid *pointer)
{
  
}

// Uniform Setters

void GLProgramState::setUniformCallback(const std::string &uniformName, const std::function<void(GLProgram*, Uniform*)> &callback)
{
  
}

void GLProgramState::setUniformCallback(GLint uniformLocation, const std::function<void(GLProgram*, Uniform*)> &callback)
{
   
}

void GLProgramState::setUniformFloat(const std::string &uniformName, float value)
{
  
}

void GLProgramState::setUniformFloat(GLint uniformLocation, float value)
{
   
}

void GLProgramState::setUniformInt(const std::string &uniformName, int value)
{
   
}

void GLProgramState::setUniformInt(GLint uniformLocation, int value)
{
  

}

void GLProgramState::setUniformVec2(const std::string &uniformName, const Vec2& value)
{
  
}

void GLProgramState::setUniformVec2(GLint uniformLocation, const Vec2& value)
{
   
}

void GLProgramState::setUniformVec3(const std::string &uniformName, const Vec3& value)
{
   
}

void GLProgramState::setUniformVec3(GLint uniformLocation, const Vec3& value)
{
   
}

void GLProgramState::setUniformVec4(const std::string &uniformName, const Vec4& value)
{
   
}

void GLProgramState::setUniformVec4(GLint uniformLocation, const Vec4& value)
{
  
}

void GLProgramState::setUniformMat4(const std::string &uniformName, const Mat4& value)
{
  
}

void GLProgramState::setUniformMat4(GLint uniformLocation, const Mat4& value)
{
  
}

// Textures

void GLProgramState::setUniformTexture(const std::string &uniformName, Texture2D *texture)
{
   
}

void GLProgramState::setUniformTexture(GLint uniformLocation, Texture2D *texture)
{
   
}

void GLProgramState::setUniformTexture(const std::string &uniformName, GLuint textureId)
{
   
}

void GLProgramState::setUniformTexture(GLint uniformLocation, GLuint textureId)
{
   
}

NS_CC_END

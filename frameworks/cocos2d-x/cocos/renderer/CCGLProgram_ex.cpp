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

#include "renderer/CCGLProgram.h"

#ifndef WIN32
#include <alloca.h>
#endif

#include "base/CCDirector.h"
#include "base/ccMacros.h"
#include "base/uthash.h"
#include "renderer/ccGLStateCache.h"
#include "platform/CCFileUtils.h"
#include "CCGL.h"

#include "deprecated/CCString.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
#include "CCPrecompiledShaders.h"
#endif

NS_CC_BEGIN

typedef struct _hashUniformEntry
{
    GLvoid*         value;       // value
    unsigned int    location;    // Key
    UT_hash_handle  hh;          // hash entry
} tHashUniformEntry;

const char* GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR = "ShaderPositionTextureColor";
const char* GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP = "ShaderPositionTextureColor_noMVP";
const char* GLProgram::SHADER_NAME_POSITION_TEXTURE_ALPHA_TEST = "ShaderPositionTextureColorAlphaTest";
const char* GLProgram::SHADER_NAME_POSITION_TEXTURE_ALPHA_TEST_NO_MV = "ShaderPositionTextureColorAlphaTest_NoMV";
const char* GLProgram::SHADER_NAME_POSITION_COLOR = "ShaderPositionColor";
const char* GLProgram::SHADER_NAME_POSITION_COLOR_NO_MVP = "ShaderPositionColor_noMVP";
const char* GLProgram::SHADER_NAME_POSITION_TEXTURE = "ShaderPositionTexture";
const char* GLProgram::SHADER_NAME_POSITION_TEXTURE_U_COLOR = "ShaderPositionTexture_uColor";
const char* GLProgram::SHADER_NAME_POSITION_TEXTURE_A8_COLOR = "ShaderPositionTextureA8Color";
const char* GLProgram::SHADER_NAME_POSITION_U_COLOR = "ShaderPosition_uColor";
const char* GLProgram::SHADER_NAME_POSITION_LENGTH_TEXTURE_COLOR = "ShaderPositionLengthTextureColor";

const char* GLProgram::SHADER_NAME_LABEL_DISTANCEFIELD_NORMAL = "ShaderLabelDFNormal";
const char* GLProgram::SHADER_NAME_LABEL_DISTANCEFIELD_GLOW = "ShaderLabelDFGlow";
const char* GLProgram::SHADER_NAME_LABEL_NORMAL = "ShaderLabelNormal";
const char* GLProgram::SHADER_NAME_LABEL_OUTLINE = "ShaderLabelOutline";

const char* GLProgram::SHADER_NAME_POSITION_TEXTURE_GREY = "ShaderPositionTextureGrey";

const char* GLProgram::SHADER_NAME_POSITION_TEXTURE_BRIGHT = "ShaderPositionTextureBright";

const char* GLProgram::SHADER_NAME_POSITION_TEXTURE_RED100 = "ShaderPositionTextureRed100";
const char* GLProgram::SHADER_NAME_POSITION_TEXTURE_RED80 = "ShaderPositionTextureRed80";
const char* GLProgram::SHADER_NAME_POSITION_TEXTURE_RED60 = "ShaderPositionTextureRed60";
const char* GLProgram::SHADER_NAME_POSITION_TEXTURE_RED40 = "ShaderPositionTextureRed40";
const char* GLProgram::SHADER_NAME_POSITION_TEXTURE_RED20 = "ShaderPositionTextureRed20";



const char* GLProgram::SHADER_3D_POSITION = "Shader3DPosition";
const char* GLProgram::SHADER_3D_POSITION_TEXTURE = "Shader3DPositionTexture";
const char* GLProgram::SHADER_3D_SKINPOSITION_TEXTURE = "Shader3DSkinPositionTexture";

const char* GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_I2A = "ShaderPositionTextureColorI2A";

const char* GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP_GRAY = "ShaderPositionTextureColor_noMVP_Gray";


// uniform names
const char* GLProgram::UNIFORM_NAME_P_MATRIX = "CC_PMatrix";
const char* GLProgram::UNIFORM_NAME_MV_MATRIX = "CC_MVMatrix";
const char* GLProgram::UNIFORM_NAME_MVP_MATRIX  = "CC_MVPMatrix";
const char* GLProgram::UNIFORM_NAME_TIME = "CC_Time";
const char* GLProgram::UNIFORM_NAME_SIN_TIME = "CC_SinTime";
const char* GLProgram::UNIFORM_NAME_COS_TIME = "CC_CosTime";
const char* GLProgram::UNIFORM_NAME_RANDOM01 = "CC_Random01";
const char* GLProgram::UNIFORM_NAME_SAMPLER0 = "CC_Texture0";
const char* GLProgram::UNIFORM_NAME_SAMPLER1 = "CC_Texture1";
const char* GLProgram::UNIFORM_NAME_SAMPLER2 = "CC_Texture2";
const char* GLProgram::UNIFORM_NAME_SAMPLER3 = "CC_Texture3";
const char* GLProgram::UNIFORM_NAME_ALPHA_TEST_VALUE = "CC_alpha_value";

// Attribute names
const char* GLProgram::ATTRIBUTE_NAME_COLOR = "a_color";
const char* GLProgram::ATTRIBUTE_NAME_POSITION = "a_position";
const char* GLProgram::ATTRIBUTE_NAME_TEX_COORD = "a_texCoord";
const char* GLProgram::ATTRIBUTE_NAME_NORMAL = "a_normal";
const char* GLProgram::ATTRIBUTE_NAME_BLEND_WEIGHT = "a_blendWeight";
const char* GLProgram::ATTRIBUTE_NAME_BLEND_INDEX = "a_blendIndex";

GLProgram* GLProgram::createWithByteArrays(const GLchar* vShaderByteArray, const GLchar* fShaderByteArray)
{
    auto ret = new (std::nothrow) GLProgram();
    if(ret && ret->initWithByteArrays(vShaderByteArray, fShaderByteArray)) {
        ret->link();
        ret->updateUniforms();
        ret->autorelease();
        return ret;
    }

    CC_SAFE_DELETE(ret);
    return nullptr;
}

GLProgram* GLProgram::createWithFilenames(const std::string& vShaderFilename, const std::string& fShaderFilename)
{
    auto ret = new (std::nothrow) GLProgram();
    if(ret && ret->initWithFilenames(vShaderFilename, fShaderFilename)) {
        ret->link();
        ret->updateUniforms();
        ret->autorelease();
        return ret;
    }

    CC_SAFE_DELETE(ret);
    return nullptr;
}

GLProgram::GLProgram()
: _program(0)
, _vertShader(0)
, _fragShader(0)
, _hashForUniforms(nullptr)
, _flags()
{
    memset(_builtInUniforms, 0, sizeof(_builtInUniforms));
}

GLProgram::~GLProgram()
{
  
   
}

bool GLProgram::initWithByteArrays(const GLchar* vShaderByteArray, const GLchar* fShaderByteArray)
{
    return true;
}


bool GLProgram::initWithFilenames(const std::string &vShaderFilename, const std::string &fShaderFilename)
{
   return true;
}

void GLProgram::bindPredefinedVertexAttribs()
{
   
}

void GLProgram::parseVertexAttribs()
{
   
}

void GLProgram::parseUniforms()
{
  
}

Uniform* GLProgram::getUniform(const std::string &name)
{
    return nullptr;
}

VertexAttrib* GLProgram::getVertexAttrib(const std::string &name)
{
  
    return nullptr;
}

std::string GLProgram::getDescription() const
{
    return StringUtils::format("<GLProgram = "
                                      CC_FORMAT_PRINTF_SIZE_T
                                      " | Program = %i, VertexShader = %i, FragmentShader = %i>",
                                      (size_t)this, _program, _vertShader, _fragShader);
}

bool GLProgram::compileShader(GLuint * shader, GLenum type, const GLchar* source)
{
    return true;
}

GLint GLProgram::getAttribLocation(const std::string &attributeName) const
{
    return 0;
}

GLint GLProgram::getUniformLocation(const std::string &attributeName) const
{
    return 0;
}

void GLProgram::bindAttribLocation(const std::string &attributeName, GLuint index) const
{
   
}

void GLProgram::updateUniforms()
{

}

bool GLProgram::link()
{
    return true;
}

void GLProgram::use()
{
  
}

std::string GLProgram::logForOpenGLObject(GLuint object, GLInfoFunction infoFunc, GLLogFunction logFunc) const
{
   return "";
}

std::string GLProgram::getVertexShaderLog() const
{
    return this->logForOpenGLObject(_vertShader, (GLInfoFunction)&glGetShaderiv, (GLLogFunction)&glGetShaderInfoLog);
}

std::string GLProgram::getFragmentShaderLog() const
{
    return this->logForOpenGLObject(_fragShader, (GLInfoFunction)&glGetShaderiv, (GLLogFunction)&glGetShaderInfoLog);
}

std::string GLProgram::getProgramLog() const
{
    return this->logForOpenGLObject(_program, (GLInfoFunction)&glGetProgramiv, (GLLogFunction)&glGetProgramInfoLog);
}

// Uniform cache

bool GLProgram::updateUniformLocation(GLint location, const GLvoid* data, unsigned int bytes)
{
    return true;
}

GLint GLProgram::getUniformLocationForName(const char* name) const
{
	return 0;
}

void GLProgram::setUniformLocationWith1i(GLint location, GLint i1)
{
  
}

void GLProgram::setUniformLocationWith2i(GLint location, GLint i1, GLint i2)
{
   
}

void GLProgram::setUniformLocationWith3i(GLint location, GLint i1, GLint i2, GLint i3)
{
   
}

void GLProgram::setUniformLocationWith4i(GLint location, GLint i1, GLint i2, GLint i3, GLint i4)
{
   
}

void GLProgram::setUniformLocationWith2iv(GLint location, GLint* ints, unsigned int numberOfArrays)
{
  
}

void GLProgram::setUniformLocationWith3iv(GLint location, GLint* ints, unsigned int numberOfArrays)
{
  
}

void GLProgram::setUniformLocationWith4iv(GLint location, GLint* ints, unsigned int numberOfArrays)
{
  
}

void GLProgram::setUniformLocationWith1f(GLint location, GLfloat f1)
{
   
}

void GLProgram::setUniformLocationWith2f(GLint location, GLfloat f1, GLfloat f2)
{
  
}

void GLProgram::setUniformLocationWith3f(GLint location, GLfloat f1, GLfloat f2, GLfloat f3)
{
   
}

void GLProgram::setUniformLocationWith4f(GLint location, GLfloat f1, GLfloat f2, GLfloat f3, GLfloat f4)
{
   
}

void GLProgram::setUniformLocationWith2fv(GLint location, const GLfloat* floats, unsigned int numberOfArrays)
{
   
}

void GLProgram::setUniformLocationWith3fv(GLint location, const GLfloat* floats, unsigned int numberOfArrays)
{
  
}

void GLProgram::setUniformLocationWith4fv(GLint location, const GLfloat* floats, unsigned int numberOfArrays)
{
   
}

void GLProgram::setUniformLocationWithMatrix2fv(GLint location, const GLfloat* matrixArray, unsigned int numberOfMatrices) {
   
}

void GLProgram::setUniformLocationWithMatrix3fv(GLint location, const GLfloat* matrixArray, unsigned int numberOfMatrices) {
  
}


void GLProgram::setUniformLocationWithMatrix4fv(GLint location, const GLfloat* matrixArray, unsigned int numberOfMatrices)
{
   
}

void GLProgram::setUniformsForBuiltins()
{
   
}

void GLProgram::setUniformsForBuiltins(const Mat4 &matrixMV)
{
   
}

void GLProgram::reset()
{
  
}

NS_CC_END

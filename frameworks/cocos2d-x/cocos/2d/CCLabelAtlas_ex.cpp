/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2014 Chukong Technologies Inc.

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
#include "CCLabelAtlas.h"
#include "renderer/CCTextureAtlas.h"
#include "2d/CCDrawingPrimitives.h"
#include "platform/CCFileUtils.h"
#include "base/ccConfig.h"
#include "base/CCDirector.h"
#include "renderer/CCTextureCache.h"
#include "renderer/CCGLProgramCache.h"
#include "renderer/CCGLProgram.h"
#include "renderer/ccGLStateCache.h"
#include "math/TransformUtils.h"

#include "deprecated/CCString.h"

#if CC_LABELATLAS_DEBUG_DRAW
#include "renderer/CCRenderer.h"
#include "base/CCDirector.h"
#endif

NS_CC_BEGIN

//CCLabelAtlas - Creation & Init

LabelAtlas* LabelAtlas::create()
{
    LabelAtlas* ret = new LabelAtlas();
    if (ret)
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_RELEASE_NULL(ret);
    }
    
    return ret;
}

LabelAtlas* LabelAtlas::create(const std::string& string, const std::string& charMapFile, int itemWidth, int itemHeight, int startCharMap)
{
    LabelAtlas* ret = new LabelAtlas();
    if(ret && ret->initWithString(string, charMapFile, itemWidth, itemHeight, startCharMap))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool LabelAtlas::initWithString(const std::string& string, const std::string& charMapFile, int itemWidth, int itemHeight, int startCharMap)
{
    Texture2D *texture = Director::getInstance()->getTextureCache()->addImage(charMapFile);
	return initWithString(string, texture, itemWidth, itemHeight, startCharMap);
}

bool LabelAtlas::initWithString(const std::string& string, Texture2D* texture, int itemWidth, int itemHeight, int startCharMap)
{
    if (AtlasNode::initWithTexture(texture, itemWidth, itemHeight, static_cast<int>(string.size())))
    {
        _mapStartChar = startCharMap;
        this->setString(string);
        return true;
    }
    return false;
}

LabelAtlas* LabelAtlas::create(const std::string& string, const std::string& fntFile)
{    
    LabelAtlas *ret = new LabelAtlas();
    if (ret)
    {
        if (ret->initWithString(string, fntFile))
        {
            ret->autorelease();
        }
        else 
        {
            CC_SAFE_RELEASE_NULL(ret);
        }
    }
    
    return ret;
}

bool LabelAtlas::initWithString(const std::string& theString, const std::string& fntFile)
{
    std::string pathStr = FileUtils::getInstance()->fullPathForFilename(fntFile);
    std::string relPathStr = pathStr.substr(0, pathStr.find_last_of("/"))+"/";
    
    ValueMap dict = FileUtils::getInstance()->getValueMapFromFile(pathStr.c_str());

    CCASSERT(dict["version"].asInt() == 1, "Unsupported version. Upgrade cocos2d version");

    std::string textureFilename = relPathStr + dict["textureFilename"].asString();

    unsigned int width = dict["itemWidth"].asInt() / CC_CONTENT_SCALE_FACTOR();
    unsigned int height = dict["itemHeight"].asInt() / CC_CONTENT_SCALE_FACTOR();
    unsigned int startChar = dict["firstChar"].asInt();


    this->initWithString(theString, textureFilename.c_str(), width, height, startChar);

    return true;
}

//CCLabelAtlas - Atlas generation
void LabelAtlas::updateAtlasValues()
{
    
}

//CCLabelAtlas - LabelProtocol
void LabelAtlas::setString(const std::string &label)
{
    _string = label;
}

const std::string& LabelAtlas::getString(void) const
{
    return _string;
}

void LabelAtlas::updateColor()
{
}

//CCLabelAtlas - draw
#if CC_LABELATLAS_DEBUG_DRAW
void LabelAtlas::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
}

void LabelAtlas::drawDebugData(const Mat4& transform, bool transformUpdated)
{
}
#endif

std::string LabelAtlas::getDescription() const
{
    return StringUtils::format("<LabelAtlas | Tag = %d, Label = '%s'>", _tag, _string.c_str());
}

NS_CC_END

/****************************************************************************
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

#include "renderer/CCRenderer.h"

#include <algorithm>

#include "renderer/CCQuadCommand.h"
#include "renderer/CCBatchCommand.h"
#include "renderer/CCCustomCommand.h"
#include "renderer/CCGroupCommand.h"
#include "renderer/CCGLProgramCache.h"
#include "renderer/ccGLStateCache.h"
#include "renderer/CCMeshCommand.h"
#include "base/CCConfiguration.h"
#include "base/CCDirector.h"
#include "base/CCEventDispatcher.h"
#include "base/CCEventListenerCustom.h"
#include "base/CCEventType.h"

NS_CC_BEGIN

// helper
static bool compareRenderCommand(RenderCommand* a, RenderCommand* b)
{
    return a->getGlobalOrder() < b->getGlobalOrder();
}

// queue

void RenderQueue::push_back(RenderCommand* command)
{
    float z = command->getGlobalOrder();
    if(z < 0)
        _queueNegZ.push_back(command);
    else if(z > 0)
        _queuePosZ.push_back(command);
    else
        _queue0.push_back(command);
}

ssize_t RenderQueue::size() const
{
    return _queueNegZ.size() + _queue0.size() + _queuePosZ.size();
}

void RenderQueue::sort()
{
    // Don't sort _queue0, it already comes sorted
    std::sort(std::begin(_queueNegZ), std::end(_queueNegZ), compareRenderCommand);
    std::sort(std::begin(_queuePosZ), std::end(_queuePosZ), compareRenderCommand);
}

RenderCommand* RenderQueue::operator[](ssize_t index) const
{
    if(index < static_cast<ssize_t>(_queueNegZ.size()))
        return _queueNegZ[index];

    index -= _queueNegZ.size();

    if(index < static_cast<ssize_t>(_queue0.size()))
        return _queue0[index];

    index -= _queue0.size();

    if(index < static_cast<ssize_t>(_queuePosZ.size()))
        return _queuePosZ[index];

    CCASSERT(false, "invalid index");
    return nullptr;
}

void RenderQueue::clear()
{
    _queueNegZ.clear();
    _queue0.clear();
    _queuePosZ.clear();
}

//
//
//
static const int DEFAULT_RENDER_QUEUE = 0;

//
// constructors, destructors, init
//
Renderer::Renderer()
:_lastMaterialID(0)
,_lastBatchedMeshCommand(nullptr)
,_numQuads(0)
,_glViewAssigned(false)
,_isRendering(false)
#if CC_ENABLE_CACHE_TEXTURE_DATA
,_cacheTextureListener(nullptr)
#endif
{
    _groupCommandManager = new GroupCommandManager();
    
    _commandGroupStack.push(DEFAULT_RENDER_QUEUE);
    
    RenderQueue defaultRenderQueue;
    _renderGroups.push_back(defaultRenderQueue);
    _batchedQuadCommands.reserve(BATCH_QUADCOMMAND_RESEVER_SIZE);
}

Renderer::~Renderer()
{
    _renderGroups.clear();
    _groupCommandManager->release();
    
#if CC_ENABLE_CACHE_TEXTURE_DATA
    Director::getInstance()->getEventDispatcher()->removeEventListener(_cacheTextureListener);
#endif
}

void Renderer::initGLView()
{

}

void Renderer::setupIndices()
{
   
}

void Renderer::setupBuffer()
{
   
}

void Renderer::setupVBOAndVAO()
{
   
}

void Renderer::setupVBO()
{
  
}

void Renderer::mapBuffers()
{
   
}

void Renderer::addCommand(RenderCommand* command)
{
   
}

void Renderer::addCommand(RenderCommand* command, int renderQueue)
{
   
}

void Renderer::pushGroup(int renderQueueID)
{
   
}

void Renderer::popGroup()
{
   
}
int Renderer::createRenderQueue()
{
   return 0;
}

void Renderer::visitRenderQueue(const RenderQueue& queue)
{
}

void Renderer::render()
{
   
}

void Renderer::clean()
{
    
}

void Renderer::convertToWorldCoordinates(V3F_C4B_T2F_Quad* quads, ssize_t quantity, const Mat4& modelView)
{

}

void Renderer::drawBatchedQuads()
{
   
}

void Renderer::flush()
{
   
}

void Renderer::flush2D()
{
    
}

void Renderer::flush3D()
{
   
}

// helpers

bool Renderer::checkVisibility(const Mat4 &transform, const Size &size)
{
   return false;
}

NS_CC_END

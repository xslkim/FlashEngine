
#include "CCClippingRegionNode.h"
#include "CCDirector.h"
#include "CCEGLView.h"


CCClippingRegionNode* CCClippingRegionNode::create(CCNode* drawNode, float x, float y, float width, float height)
{
    CCClippingRegionNode* node = new CCClippingRegionNode(drawNode);
    node->setClippingRegion(CCRect(x, y, width, height));
    node->autorelease();
    return node;
}


CCClippingRegionNode::CCClippingRegionNode(CCNode* drawNode)
	: m_clippingEnabled(true)
	, m_drawNode(drawNode)
{
	m_drawNode->retain();
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	_className = "CCClippingRegionNode";
#endif
}

CCClippingRegionNode::~CCClippingRegionNode()
{
	m_drawNode->release();
}

void CCClippingRegionNode::setClippingRegion(CCRect clippingRegion)
{
	m_clippingRegion = clippingRegion;
	setContentSize(clippingRegion.size);
}

CCRect CCClippingRegionNode::getClippingRegion(void) 
{
	return m_clippingRegion;
}

void CCClippingRegionNode::onEnter()
{
	CCNode::onEnter();
	m_drawNode->onEnter();
}


bool CCClippingRegionNode::isClippingEnabled(void) 
{
	return m_clippingEnabled;
}

void CCClippingRegionNode::setClippingEnabled(bool enabled) 
{
	m_clippingEnabled = enabled;
}

void CCClippingRegionNode::visit() 
{
	if (m_clippingEnabled)
	{
		glEnable(GL_SCISSOR_TEST);

		//pos值用convertToWorldSpace之后的值，如果直接用m_clippingRegion.origin，会出现切割区域不随父节点移动的问题
		const CCPoint pos = convertToWorldSpace(CCPoint(m_clippingRegion.origin.x, m_clippingRegion.origin.y));
		//CCPoint pos = m_clippingRegion.origin;
		CCDirector::sharedDirector()->getOpenGLView()->setScissorInPoints(pos.x * m_fScaleX,
			pos.y * m_fScaleX,
			m_clippingRegion.size.width * m_fScaleY,
			m_clippingRegion.size.height * m_fScaleY);
	}

	m_drawNode->visit();

	if (m_clippingEnabled)
	{
		glDisable(GL_SCISSOR_TEST);
	}
}


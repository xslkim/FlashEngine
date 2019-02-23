#ifndef __FLASH_SPRITE_DELEGATE_H__
#define	__FLASH_SPRITE_DELEGATE_H__


NS_CC_BEGIN
class FlashSprite;
class FlashSpriteDelegate
{
public:
	virtual void onAnimationEnd(FlashSprite* flashSprite) = 0;
};
NS_CC_END

#endif



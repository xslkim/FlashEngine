#ifndef __FLASH_SPRITE_DELEGATE_H__
#define	__FLASH_SPRITE_DELEGATE_H__

class FlashSprite;
class FlashSpriteDelegate
{
public:
	virtual void onAnimationEnd(FlashSprite* flashSprite) = 0;
};

#endif



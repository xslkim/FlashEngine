
#ifndef __CC_BOX_SPRITE_H__
#define __CC_BOX_SPRITE_H__

#include "FlashSprite.h"
#include <map>
#include <vector>
class BoxFlashInfo;
NS_CC_BEGIN


class BoxFlash : public FlashSprite
{
public:
	static BoxFlash* create(std::string name);
	bool isHurtByOther(BoxFlash* other, int offset = 0);
	Rect getAttackDistance();
	Rect getAttackRect();
	Rect getHurtRect();
	std::vector<Rect> getBoxesRect(bool isAttack, int offset_z = 0);
	std::vector<Rect> getBoxesRect(bool isAttack, int action, int frame, int offset_z = 0);
	void registerAttackScriptHandler(int handler);
	void unregisterAttackScriptHandler();

	virtual void draw(Renderer *renderer, const Mat4& transform, uint32_t flags);
protected:
	BoxFlash();
	void onBoxDraw(const Mat4 &transform, uint32_t flags);
	bool isAttackFrame(int action, int frame);
	virtual void onAnimationStart();
	virtual void frameChange(int form, int to);
	static std::map<std::string, BoxFlashInfo*> s_boxInfoMap;
	void loadBoxFile(std::string name);
	int _scriptAttackHandle;
	int _lastAttackFrame;
	BoxFlashInfo* _info;
	CustomCommand _customCommand;
};

NS_CC_END
#endif


#ifndef  __Random__H__
#define  __Random__H__

#include "cocos2d.h"

NS_CC_BEGIN

class RandomMgr : public Ref
{
public:
	static RandomMgr* getInstance();

	//return 0.0 to 1.0
	float getFloat();

	//return 1 to max
	unsigned int getInt(int a);

	//return min to max
	int getInt(int a, int b);

	void setSeed(unsigned int seed);
private:
	static RandomMgr* _instance;
	RandomMgr();
	unsigned int nextRandom();
	char* _buf;
	int _bufferSize;
	int _indexes[4];
};
NS_CC_END
#endif
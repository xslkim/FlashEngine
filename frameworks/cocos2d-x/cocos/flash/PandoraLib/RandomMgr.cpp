#include "RandomMgr.h"

using namespace cocos2d;
using namespace network;

RandomMgr* RandomMgr::_instance = 0;


RandomMgr* RandomMgr::getInstance()
{	
	if( _instance == 0)
	{
		_instance = new RandomMgr();
	}
	return _instance;
}

RandomMgr::RandomMgr()
{
	Data data = cocos2d::CCFileUtils::sharedFileUtils()->getDataFromFile("res/random.bin");
	_bufferSize = data.getSize();
	_buf = new char[_bufferSize];
	memcpy(_buf, data.getBytes(), _bufferSize);
}


//return 0.0 to 1.0
float RandomMgr::getFloat(){
	unsigned int number = nextRandom() % 10000;
	float out = number / 10000.f ;
	return out;
}

//return 1 to max
unsigned int RandomMgr::getInt(int a){
	if(a <= 0) return 1;
	unsigned int out = nextRandom() % a + 1 ;
	return out;
}

//return min to max
int RandomMgr::getInt(int a, int b){
	if(a > b) return 0;
	if(a == b) return a;
	unsigned int rang = b - a + 1;
	unsigned int out = nextRandom() % rang;
	return a + out;
}

unsigned int RandomMgr::nextRandom(){
	for(int i = 0; i < 4; ++i){
		_indexes[i] += 1;
		if (_indexes[i] >= _bufferSize){
			_indexes[i] = 0;
		}
	}
	int number = _buf[_indexes[0]];
	number += (_buf[_indexes[1] ] << 8);
	number += (_buf[_indexes[1] ] << 16);
	number += (_buf[_indexes[1] ] << 24);
	return number;
}

void RandomMgr::setSeed(unsigned seed){
	_indexes[0] = seed % 0xFF;
	_indexes[1] = (seed >> 8) % 0xFF;
	_indexes[2] = (seed >> 16) % 0xFF;
	_indexes[3] = (seed >> 24) % 0xFF;
}
#ifndef __UTILS_H__
#define __UTILS_H__

#include <tchar.h>
#include <string>
#include <Windows.h>
#include "cocos2d.h"
#include "tinyxml.h"
#include "Matrix.h"
#include "ximage.h"
#include "flashMeta.pb.h"
#include "flashMetaEx.pb.h"

#define USE_FLASH_METADATA_CACHE 0


using namespace std;
using namespace cocos2d;

std::string WChar2Ansi(LPCWSTR pwszSrc);

string ws2s(wstring& inputws);

std::wstring Ansi2WChar(LPCSTR pszSrc, int nLen);

std::wstring s2ws(const string& s);

bool isPlaying(const char* comLine);

std::string decryptPlistName(const char* comLine);

bool gIsLoadXml(const char* pathAndName);

std::string getPathAndFileName(const char* comLine, bool isPlay);

std::string getFileName(const char* path);

std::string getFileNameEx(const char* path);

void setAttribString(std::string& string, const char* p);

TiXmlElement* getChildElement(TiXmlElement* element, const char* name);
void setAttribInt(TiXmlElement* element, const char* name, int& value);
void setAttribfloat(TiXmlElement* element, const char* name, float& value);
void setAttribDouble(TiXmlElement* element, const char* name, double& value);

#define FLATE_PIXEL_RATE 20.f
#define TWIPS_TO_PIXELS(x)	((x) / FLATE_PIXEL_RATE)
#define PIXELS_TO_TWIPS(x)	((x) * FLATE_PIXEL_RATE)

inline int	iabs(int i) { if (i < 0) return -i; else return i; }
inline int	imax(int a, int b) { if (a < b) return b; else return a; }
inline double	fmax(double a, double b) { if (a < b) return b; else return a; }
inline int	imin(int a, int b) { if (a < b) return a; else return b; }
inline double	fmin(double a, double b) { if (a < b) return a; else return b; }
inline double flerp(double a, double b, double f) { return (b - a) * f + a; }


#ifdef _WINDOWS
#define DBG_TRACE(X)	 OutputDebugString(L ## X)
#define DBG_STRING(X)	 OutputDebugStringA(X)
#else
#define DBG_TRACE(X)	 
#define DBG_STRING(X)	 
#endif

double angleToPi(double angle);
double PiToAngle(double pi);

double getAngle(Point point);

void debug_out(const char * strOutputString, ...);


struct s_media_data
{
	int id;
	std::string name;
	std::string pathAndName;
	float width;
	float height;
};
 

struct instanceData
{
	instanceData()
	{
		memset(&matrix, 0, sizeof(CCAffineTransform));
		isBrightness = false;
		playOnce = false;
		singleFrame = false;
		matrix.a = 1;
		matrix.d = 1;
		a = 1;
		r = 1;
		g = 1;
		b = 1;
	}
	void changeMatrixRotate();
	std::string resName;
	std::string insName;
	bool playOnce;
	bool singleFrame;
	bool isBrightness;
	Matrix matrix;
	Point transformationPoint;
	double a;
	double r;
	double g;
	double b;
};

struct DOMSymbolInstance
{
	instanceData insData;
};



struct DOMBitmapInstance
{
	instanceData insData;
};

//enum DOMFrameAttrib
//{
//	kDOMFrameAttrib_index = 0;
//	kDOMFrameAttrib_duration;
//	kDOMFrameAttrib_motionTweenRotateTimes;
//	kDOMFrameAttrib_motionTweenSnap;
//	kDOMFrameAttrib_Count;
//};

struct BoxKeyFrame
{
	int start;
	int duration;
	std::vector<Matrix> tranMatrix;
};

struct FrameData
{
	FrameData(){ startFrame = 0;}
	int startFrame;
	std::vector<instanceData> instanceBitMapVector;
	std::vector<instanceData> instanceSymbolVector;
};

struct DOMFrameData
{
	DOMFrameData()
	{
		index = -1;
		duration = 1;
	}
	int index;
	int duration;
	std::string motionTweenRotate;
	int motionTweenRotateTimes;
	std::string motionTweenSnap;
	std::string tweenType;
	std::vector<DOMSymbolInstance> symbolVector;
	std::vector<DOMBitmapInstance> bitMapVector;
};

std::string getClipname(std::string insDataName, const char* path);


struct EdgeData
{
	EdgeData():left(0),right(0),top(0),button(0) {}
	std::string name;
	int width;
	int height;
	int left;
	int right;
	int top;
	int button;
};

float getLeftOff(CxImage* image);
float getRightOff(CxImage* image);
float getBottomOff(CxImage* image);
float getTopOff(CxImage* image);


Matrix getMatrix(const FlashMetaData& metaData);


void setFlashMetaData(FlashMetaData& metaData, Matrix mat);
void setFlashMetaData(FlashMetaData& metaData, const instanceData& insData);

#endif
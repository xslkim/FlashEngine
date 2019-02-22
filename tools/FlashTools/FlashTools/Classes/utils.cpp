#include "utils.h"
#include <string>
#include <Windows.h>


using namespace std;
// Converting a WChar string to a Ansi string 
std::string WChar2Ansi(LPCWSTR pwszSrc) 
{ 
	int nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL); 
	if (nLen<= 0) 
		return std::string(""); 

	char* pszDst = new char[nLen]; 
	if (NULL == pszDst) 
		return std::string(""); 

	WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL); 
	pszDst[nLen -1] = 0; 

	std::string strTemp(pszDst); 
	delete [] pszDst; 

	return strTemp; 
} 

string ws2s(wstring& inputws) 
{ 
	return WChar2Ansi(inputws.c_str()); 
} 

// Converting a Ansi string to WChar string 
std::wstring Ansi2WChar(LPCSTR pszSrc, int nLen) 
{ 
	int nSize = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pszSrc, nLen, 0, 0); 
	if (nSize <= 0) 
		return NULL; 

	WCHAR *pwszDst = new WCHAR[nSize+1]; 
	if( NULL == pwszDst) 
		return NULL; 

	MultiByteToWideChar(CP_ACP, 0,(LPCSTR)pszSrc, nLen, pwszDst, nSize); 
	pwszDst[nSize] = 0; 

	if (pwszDst[0] == 0xFEFF) // skip Oxfeff 
	{ 
		for(int i = 0; i < nSize; i ++) 
		{ 
			pwszDst[i] = pwszDst[i+1]; 
		} 
	} 

	wstring wcharString(pwszDst); 
	delete pwszDst; 

	return wcharString; 
} 


std::wstring s2ws(const string& s) 
{ 
	return Ansi2WChar(s.c_str(),s.size()); 
} 

double angleToPi(double angle)
{
	angle = angle / 180;
	return angle * M_PI;
}

double PiToAngle(double pi)
{
	double rate =  (double)M_PI / 180;
	return pi / rate;
}

double getAngle(Point point)
{
	float destRotate = 0;
	if(point.x == 0)
	{
		if(point.y > 0)
			destRotate = M_PI_2;
		else if(point.y < 0)
			destRotate = -M_PI_2;
		else
			CCAssert(false, "");
	}
	else
	{
		if( point.y == 0)
		{
			if( point.x > 0 )
				destRotate = 0;
			else
				destRotate = M_PI;
		}
		else
		{
			destRotate = atan(point.y/point.x);
			if( point.x < 0 && point.y > 0) //2ÏóÏÞ
			{
				destRotate += M_PI;
			}
			else if(point.x < 0 && point.y < 0)
			{
				destRotate = destRotate - M_PI;
			}
		}
	}
	return destRotate;
}

void debug_out(const char * strOutputString, ...)
{
	char strBuffer[4096]={0};
	va_list vlArgs;
	va_start(vlArgs,strOutputString);
	_vsnprintf(strBuffer,sizeof(strBuffer)-1,strOutputString,vlArgs);
	//vsprintf(strBuffer,strOutputString,vlArgs);
	va_end(vlArgs);
	OutputDebugStringA(strBuffer);
}

bool isPlaying(const char* comLine)
{
	std::string strShow = decryptPlistName(comLine);
	if(strShow == "show" )
	{
		return true;
	}
	return false;
}

std::string decryptPlistName(const char* comLine)
{
	int len = strlen(comLine) - 1;
	for(;;)
	{
		if(comLine[len] == ' ')
		{
			break;
		}
		len--;
	}
	char buf[256] = {0};
	strcpy(buf, comLine + len+1);
	return buf;
}

bool gIsLoadXml(const char* pathAndName)
{
	int len = strlen(pathAndName);
	bool loadXml = false;
	if(pathAndName[len-1] == '/' || pathAndName[len-1] == '\\')
	{
		loadXml = true;
	}
	return loadXml;
}

void instanceData::changeMatrixRotate()
{
	//double scaleX = matrix.get_x_scale();
	//double scaleY = matrix.get_y_scale();
	//double biasX = matrix.getBiasX();
	//double biasY = matrix.getBiasY();
	//if(matrix.a > 0 && matrix.d > 0)
	//{
	//	matrix.setScaleBias(scaleX, scaleY, -biasX, -biasY);
	//}
	//else if( matrix.a < 0 && matrix.d < 0)
	//{
	//	matrix.setScaleBias(scaleX, scaleY, -biasX, -biasY);
	//}
}

std::string getPathAndFileName(const char* comLine, bool isPlay )
{
	int len = strlen(comLine) - 1;
	for(;;)
	{
		if(comLine[len] == ' ')
		{
			break;
		}
		len--;
	}
	char buf[256] = {0};
	memcpy(buf, comLine, len);
	return buf;
}

std::string getFileName(const char* path)
{
	int len = strlen(path) - 2;
	for(;;)
	{
		if(path[len] == '\\' || path[len] == '/')
		{
			break;
		}
		len -= 1;
	}
	char buf[256] = {0};
	strcpy(buf,  path + len + 1);
	len = strlen(buf);
	buf[len - 1] = 0;
	return buf;
}

std::string getFileNameEx(const char* path)
{
	int len = strlen(path) - 2;
	for(;;)
	{
		if(path[len] == '\\' || path[len] == '/')
		{
			break;
		}
		len -= 1;
		if(len <= 0)
		{
			return path;
		}
	}
	char buf[256] = {0};
	strcpy(buf,  path + len + 1);
	len = strlen(buf);
	buf[len] = 0;
	return buf;
}

void setAttribString(std::string& string, const char* p)
{
	if(NULL != p)
	{
		string = p;
	}
}

TiXmlElement* getChildElement(TiXmlElement* element, const char* name)
{
	TiXmlElement* firstChildElement = element->FirstChildElement();
	TiXmlElement* childElement = NULL;
	if(firstChildElement != NULL)
	{
		std::string matrixValue = firstChildElement->Value();
		if(matrixValue == name)
		{
			childElement = firstChildElement;
		}
		else
		{
			childElement = firstChildElement->NextSiblingElement(name);
		}
	}
	return childElement;
}

void setAttribInt(TiXmlElement* element,  const char* name, int& value)
{
	int temp;
	if(element->Attribute(name, &temp))
	{
		value = temp;
	}
}

void setAttribfloat(TiXmlElement* element, const char* name, float& value)
{
	double temp;
	if(element->Attribute(name, &temp))
	{
		value = temp;
	}
}

void setAttribDouble(TiXmlElement* element, const char* name, double& value)
{
	double temp;
	if(element && element->Attribute(name, &temp))
	{
		value = temp;
	}
}


float getLeftOff(CxImage* image)
{
	if(image->m_left_off >= 0)
	{
		return image->m_left_off;
	}
	image->m_left_off = 0;
	for(int x = 0; x < image->GetWidth(); ++x)
	{
		bool empty = true;
		for(int y = 0; y < image->GetHeight(); ++y)
		{
			RGBQUAD color = image->GetPixelColor(x, y, true);
			if( color.rgbReserved != 0 )
			{
				empty = false;
				break;
			}
		}
		if( empty )
		{
			image->m_left_off += 1;
		}
		else
		{
			break;
		}
	}
	if( image->m_left_off == image->GetWidth() )
	{
		image->m_left_off = 0;
	}


	return image->m_left_off;
}

float getRightOff(CxImage* image)
{
	if(image->m_right_off >= 0)
	{
		return image->m_right_off;
	}
	image->m_right_off = 0;
	for(int x = image->GetWidth()-1; x >= 0; --x)
	{
		bool empty = true;
		for(int y = 0; y < image->GetHeight(); ++y)
		{
			RGBQUAD color = image->GetPixelColor(x, y, true);
			if( color.rgbReserved != 0 )
			{
				empty = false;
				break;
			}
		}
		if( empty )
		{
			image->m_right_off += 1;
		}
		else
		{
			break;
		}
	}

	if( image->m_right_off == image->GetWidth() )
	{
		image->m_right_off = 0;
	}


	return image->m_right_off;
}

float getBottomOff(CxImage* image)
{
	if(image->m_bottom_off >= 0)
	{
		return image->m_bottom_off;
	}
	image->m_bottom_off = 0;
	for( int y = 0; y < image->GetHeight(); ++y )
	{
		bool empty = true;
		for( int x = 0; x < image->GetWidth(); ++x )
		{
			RGBQUAD color = image->GetPixelColor(x, y, true);
			if(color.rgbReserved != 0)
			{
				empty = false;
				break;
			}
		}
		if(empty)
		{
			image->m_bottom_off += 1;
		}
		else
		{
			break;
		}
	}

	if( image->m_bottom_off == image->GetHeight() )
	{
		image->m_bottom_off = 0;
	}

	return image->m_bottom_off;
}

float getTopOff(CxImage* image)
{
	if(image->m_top_off >= 0)
	{
		return image->m_top_off;
	}
	image->m_top_off = 0;
	for( int y = image->GetHeight() - 1; y >=0 ; --y )
	{
		bool empty = true;
		for( int x = 0; x < image->GetWidth(); ++x )
		{
			RGBQUAD color = image->GetPixelColor(x, y, true);
			if(color.rgbReserved != 0)
			{
				empty = false;
				break;
			}
		}
		if(empty)
		{
			image->m_top_off += 1;
		}
		else
		{
			break;
		}
	}

	if( image->m_top_off == image->GetHeight() )
	{
		image->m_top_off = 0;
	}

	return image->m_top_off;
}

std::string getClipname(std::string insDataName, const char* path)
{
	int len = insDataName.size() - 1;
	for(;;)
	{
		if(insDataName[len] == '\\' || insDataName[len] == '/')
		{
			break;
		}
		len -= 1;
		if(len < 0 )
		{
			break;
		}
	}
	char buf[256] = {0};
	strcpy(buf,  insDataName.c_str() + len + 1);
	len = strlen(buf);
	std::string pngName = buf;
	std::string flashName = getFileName( path );
	std::string clipName = flashName + "_";
	clipName += pngName;

	return clipName.c_str();
}


Matrix getMatrix(const FlashMetaData& metaData)
{
	Matrix m;
	if( metaData.has_ma() )
	{
		m.a = metaData.ma();
	}

	if( metaData.has_mb() )
	{
		m.b = metaData.mb();
	}

	if( metaData.has_mc() )
	{
		m.c = metaData.mc();
	}

	if( metaData.has_md() )
	{
		m.d = metaData.md();
	}

	if( metaData.has_mtx() )
	{
		m.tx = metaData.mtx();
	}

	if( metaData.has_mty() )
	{
		m.ty = metaData.mty();
	}
	return m;
}

void setFlashMetaData(FlashMetaData& metaData, Matrix matrix)
{
	if(matrix.a != 1)
		metaData.set_ma(matrix.a);

	if(matrix.b != 0)
		metaData.set_mb(matrix.b);

	if(matrix.c != 0)
		metaData.set_mc(matrix.c);

	if(matrix.d != 1)
		metaData.set_md(matrix.d);

	if(matrix.tx != 0)
		metaData.set_mtx(matrix.tx);

	if(matrix.ty != 0)
		metaData.set_mty(matrix.ty);
	
}


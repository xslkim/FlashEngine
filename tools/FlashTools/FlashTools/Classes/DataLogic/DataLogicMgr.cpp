#include "DataLogicMgr.h"
#include "DOMDocument.h"
#include "utils.h"
#include <iostream>

DataLogicMgr* DataLogicMgr::_instance = 0;

DataLogicMgr* DataLogicMgr::getInstance()
{	
	if( _instance == 0)
	{
		_instance = new DataLogicMgr();
	}
	return _instance;
}

DataLogicMgr* getDataLogicMgr()
{
	return DataLogicMgr::getInstance(); 
}

DataLogicMgr::DataLogicMgr()
{
	_document = new DOMDocument();
}

DataLogicMgr::~DataLogicMgr()
{
	
}

int DataLogicMgr::getScreenHeight()
{
	return _document->getScreenHeight(); 
}

int DataLogicMgr::getScreenWidth()
{
	return _document->getScreenWidth(); 
}

void DataLogicMgr::initCmdLine(const char* comLine)
{
	_isPlay = isPlaying(comLine);
	if(!_isPlay)
	{
		_plistName = decryptPlistName(comLine);
	}
	_path = getPathAndFileName(comLine, _isPlay);
	_isLoadXml = gIsLoadXml(_path.c_str());
}

bool DataLogicMgr::isPlay()
{
	return _isPlay; 
	//return false;
}

void DataLogicMgr::loadScreen(std::string path)
{
	std::string docName = path + "DOMDocument.xml";
	_document->loadScreen(docName);
}

bool DataLogicMgr::loadXml(std::string path)
{
	
	std::string docName = path + "DOMDocument.xml";

	if(!_document->LoadXml(docName.c_str()))
	{
		return false;
	}

	Matrix srcMatrix;

	srcMatrix.a = 0.621337890625;
	srcMatrix.b = 0.77874755859375;
	srcMatrix.c = -0.778671264648438;
	srcMatrix.d = 0.621307373046875;
	srcMatrix.print();

	Point xTest(1, 0);
	Point xTestOut;
	srcMatrix.transform_vector(&xTestOut, xTest);
	double xAngleChange = getAngle(xTestOut);
	double xAngleChangeA = PiToAngle(xAngleChange);
	

	Point yTest(0, 1);
	Point yTestOut;
	srcMatrix.transform_vector(&yTestOut, yTest);
	double yAngle = getAngle(yTestOut);
	double yAngleA = PiToAngle(yAngle);
	double yAngleChange = yAngle - M_PI_2;
	if( yAngleChange < - M_PI)
	{
		yAngleChange = yAngleChange + (2*M_PI);
	}
	double yAngleChangeA = PiToAngle(yAngleChange);
	

	Matrix matX, matY;
	matX.a = cos(xAngleChange);
	matX.b = -sin(xAngleChange);

	matY.c = sin(yAngleChange);
	matY.d = cos(yAngleChange);

	Matrix outMatrix = matX;
	outMatrix.c = matY.c;
	outMatrix.d = matY.d;
	outMatrix.print();

	double scaleX = outMatrix.get_x_scale();
	double scaleY = outMatrix.get_y_scale();
	//double biasX = outMatrix.getBiasX();
	//double biasY = outMatrix.getBiasY();

	//Matrix testMatrix;
	//testMatrix.setScaleBias(scaleX, scaleY, -biasX, -biasY);
	//testMatrix.print();

	int qwer = 1234;
	






	
	return true;
}

std::string DataLogicMgr::getFpkPath()
{
	int len = _path.length() - 2;
	for(;;)
	{
		if(_path[len] == '\\' || _path[len] == '/')
		{
			break;
		}
		len -= 1;
	}
	char buf[256] = {0};
	memcpy(buf, _path.c_str(), len + 1);
	return buf; 
}

int DataLogicMgr::getMaxAnimination()
{
	return _document->getAnimationCount();
}

bool DataLogicMgr::loadFpk(std::string pathAndName)
{
	return false;
}


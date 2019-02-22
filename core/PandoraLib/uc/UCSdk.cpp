/*
 * UCSdk.cpp
 * 此类定义的变量和逻辑应根据游戏逻辑需要改写，此处仅作参考。
 *
 *  Created on: 2013-4-15
 *      Author: user
 */
#include "UCSdk.h"

#include "CUCGameSdk.h"


using namespace ucgamesdk;


//联调环境参数
int UCSdk::s_cpid = 1;//22429;
int UCSdk::s_gameid = 230;//534364 ;
int UCSdk::s_serverid = 235;//2401;
std::string UCSdk::s_servername = "测试服";//"战姬天下";

////正式环境参数
//int UCSdk::s_cpid = "";
//int UCSdk::s_gameid = "";
//int UCSdk::s_serverid = "";
//std::string UCSdk::s_servername = "正式环境参数";


bool UCSdk::s_debugMode = true;
int UCSdk::s_loglevel = CUCGameSdk::LOGLEVEL_DEBUG;
int UCSdk::s_orientation = CUCGameSdk::ORIENTATION_LANDSCAPE;
bool UCSdk::s_enablePayHistory = true;
bool UCSdk::s_enableLogout = true;


bool UCSdk::s_inited = false;
int UCSdk::s_initRetryTimes = 0;
bool UCSdk::s_logined = false;
std::string UCSdk::s_sid = "";

bool UCSdk::s_isUCVip = false;

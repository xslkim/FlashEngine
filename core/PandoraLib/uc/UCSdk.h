/*
 * UCSdk.h
 * 此类定义的变量和逻辑应根据游戏逻辑需要改写，此处仅作参考。
 *
 *  Created on: 2013-4-15
 *      Author: user
 */

#ifndef UCSDK_H_
#define UCSDK_H_

#include <string>

#define INIT_MAX_RETRY_TIMES 10

class UCSdk {
public:
	static int s_cpid;
	static int s_gameid;
	static int s_serverid;
	static std::string s_servername;

	static bool s_debugMode;
	static int s_loglevel;
	static int s_orientation;
	static bool s_enablePayHistory;
	static bool s_enableLogout;

	static bool s_inited;
	static int s_initRetryTimes;
	static bool s_logined;
	static std::string s_sid;

	static bool s_isUCVip;

};

#endif /* UCSDK_H_ */


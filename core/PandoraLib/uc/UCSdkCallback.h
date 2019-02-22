/*
 * UCSdkCallback.h
 * 此类定义接收SDK的回调的逻辑处理，应根据游戏逻辑需要改写，此处仅作参考。
 *
 *  Created on: 2013-4-12
 *      Author: user
 */

#ifndef UCSDKCALLBACK_H_
#define UCSDKCALLBACK_H_


class UCSdkCallback {
public:
	static void init_callback(int code, const char *pszmsg);
	static void login_callback(int code, const char *pszmsg);

	static int gameUserLogin_callback(const char *pszUsername, const char *pszPassword, char **lpSid);

	static void logout_callback(int code, const char *pszmsg);
	static void floatMenu_callback(int code, const char *pszmsg);

	static void pay_callback(int code, const char *pszOrderId, float orderAmount, int payWayId, const char *pszPayWayName);
	static void uPointCharge_callback(int code, const char *pszmsg);

	static void userCenter_callback(int code, const char *pszmsg);
	static void enterUI_callback(int code, const char *pszmsg);

	static void isUCVip_callback(int code, bool isUCVip);
	static void getUCVipInfo_callback(int code, const char *pszJsonData);
};

#endif /* UCSDKCALLBACK_H_ */

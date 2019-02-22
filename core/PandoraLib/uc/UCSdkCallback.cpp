/*
 * SdkCallback.cpp
 * 此类定义接收SDK的回调的逻辑处理，应根据游戏逻辑需要改写，此处仅作参考。
 *
 *  Created on: 2013-4-12
 *      Author: user
 */

#include "UCSdkCallback.h"
#include "UCSdk.h"
#include "CUCStatusCode.h"
#include "CUCGameSdk.h"

#include "json/json_lib.h"

#include "cocos2d.h"
#include <string>

using namespace ucgamesdk;
using namespace cocos2d;


void UCSdkCallback::init_callback(int code, const char* pszmsg) {
	CCLog("received init_callback: code=%d, msg=%s", code, pszmsg);

	if (code == CUCStatusCode::SUCCESS) {
		UCSdk::s_inited = true;
		CCLog("init succeeded");
		CCMessageBox("init succeeded","回调提示");

	} else {
		CCLog("Failed initing UC game sdk, code=%d, msg=%s", code, pszmsg);
		CCMessageBox(pszmsg ,"回调提示");
		//初始化失败处理
		UCSdk::s_initRetryTimes ++;
		if (UCSdk::s_initRetryTimes < INIT_MAX_RETRY_TIMES) {
			CCLog("retry initing sdk...");
			CUCGameSdk::initSDK(UCSdk::s_debugMode, UCSdk::s_loglevel, UCSdk::s_cpid, UCSdk::s_gameid, UCSdk::s_serverid,
					UCSdk::s_servername.c_str(), UCSdk::s_enablePayHistory, UCSdk::s_enableLogout, UCSdkCallback::init_callback);
		}
	}
}

void UCSdkCallback::login_callback(int code, const char* pszmsg) {
	CCLog("received login_callback: code=%d, msg=%s", code, pszmsg);

	if (code == CUCStatusCode::SUCCESS) {
		UCSdk::s_logined = true;
		UCSdk::s_sid = CUCGameSdk::getSid();
		CCLog("login succeeded: sid=%s", UCSdk::s_sid.c_str());


		// create float button
		CUCGameSdk::createFloatButton(UCSdkCallback::floatMenu_callback);

		// show float button
		CUCGameSdk::showFloatButton(100, 30, true);

		CUCGameSdk::notifyZone("66区-风起云涌", "R29924", "Role-大漠孤烟");

	} else if (code == CUCStatusCode::LOGIN_EXIT) {
		//登录界面退出，返回到游戏画面
		CCLog("login UI exit, back to game UI");
	} else {
		CCLog("Failed login, code=%d, msg=%s", code, pszmsg);
		CCMessageBox(pszmsg,"回调提示" );
		//登录失败
		//
	}
}


int UCSdkCallback::gameUserLogin_callback(const char *pszUsername, const char *pszPassword, char **lpSid) {
	int gameUserLoginResultCode = CUCStatusCode::LOGIN_GAME_USER_AUTH_FAIL;

	// 此处以用户名和密码相同来模拟游戏服务器的用户验证，实际开发时应调用游戏服务器的接口进行用户验证，并从游戏服务器获取在服务器端绑定了UC账号后的 sid 。
	if ( strcmp(pszUsername, pszPassword) == 0 ) {
		std::string sid = "38ad7c64-36df-41d3-9c15-2bca5ff9b282140876";
		strcpy(*lpSid, sid.c_str());
		gameUserLoginResultCode = CUCStatusCode::SUCCESS;
	} else {
		// 根据验证失败的情况，设置返回值。
		gameUserLoginResultCode = CUCStatusCode::LOGIN_GAME_USER_AUTH_FAIL;
		//gameUserLoginResultCode = CUCStatusCode::LOGIN_GAME_USER_NETWORK_FAIL;
		//gameUserLoginResultCode = CUCStatusCode::LOGIN_GAME_USER_OTHER_FAIL;
	}

	return gameUserLoginResultCode;
}

void UCSdkCallback::logout_callback(int code, const char* pszmsg) {
	CCLog("received logout_callback: code=%d, msg=%s", code, pszmsg);

	if (code == CUCStatusCode::SUCCESS) {
		//当前登录用户已退出，应将游戏切换到未登录的状态。
		UCSdk::s_logined = false;
		UCSdk::s_sid = "";
		CCMessageBox(pszmsg,"回调提示" );
	} else {
		//unknown error
		CCLog("unknown error: code=%d, msg=%s", code, pszmsg);
		CCMessageBox(pszmsg,"回调提示" );
	}
}

void UCSdkCallback::floatMenu_callback(int code, const char* pszmsg) {
	CCLog("received floatMenu_callback: code=%d, msg=%s", code, pszmsg);

	if (code == CUCStatusCode::SDK_OPEN) {
		//打开了SDK界面
		CCLog("user opened the SDK UI");
		CCMessageBox(pszmsg,"回调提示" );
	} else if (code == CUCStatusCode::SDK_CLOSE) {
		//SDK界面已关闭，回到游戏画面，游戏应根据实际需要决定是否进行画面刷新
		CCLog("user closed the SDK UI, backed to game UI");
		CCMessageBox("user closed the SDK UI, backed to game UI","回调提示" );
	}
}

void UCSdkCallback::pay_callback(int code, const char* pszOrderId,
		float orderAmount, int payWayId, const char* pszPayWayName) {
	CCLog("received order_callback: code=%d", code);
	CCMessageBox("received order_callback","回调提示" );
	if (code == CUCStatusCode::SUCCESS) {
		//充值下单成功，游戏应对下单结果进行处理，一般需把订单号、下单金额、支付渠道ID、支付渠道名称等信息上传到游戏服务器进行保存
		CCLog("received order_callback order info: code=%d, orderId=%s, orderAmount=%.2f, payWayId=%d, payWayName=%s",
				code, pszOrderId, orderAmount, payWayId, pszPayWayName);
	} else if (code == CUCStatusCode::PAY_USER_EXIT) {
		//充值界面已关闭，回到游戏画面，游戏应根据实际需要决定是否进行画面刷新
	} else {
		//充值调用失败
	}

	//游戏根据需要进行订单处理，一般需要把订单号传回游戏服务器，在服务器上保存
}

void UCSdkCallback::userCenter_callback(int code, const char* pszmsg) {
	CCLog("received userCenter_callback: code=%d, msg=%s", code, pszmsg);

	if (code == CUCStatusCode::SUCCESS) {
		//用户退出了九游社区（个人中心）界面，返回游戏画面，游戏应根据实际需要决定是否进行画面刷新
		CCLog("user closed the user center UI, backed to game UI");
	} else {
		//fail
		CCLog("unknown error: code=%d, msg=%s", code, pszmsg);
	}
}

void UCSdkCallback::uPointCharge_callback(int code, const char* pszmsg) {
	CCLog("received uPointCharge_callback: code=%d, msg=%s", code, pszmsg);

	if (code == CUCStatusCode::SDK_CLOSE) {
		//U点充值完成，返回游戏画面，游戏应根据实际需要决定是否进行画面刷新
		CCLog("user closed the user center UI, backed to game UI");
	} else if (code == CUCStatusCode::NO_INIT) {
		CCLog("not inited");
	} else if (code == CUCStatusCode::NO_LOGIN) {
		CCLog("not logined");
	} else {
		//unknown error
		CCLog("unknown error: code=%d, msg=%s", code, pszmsg);
	}
}

void UCSdkCallback::enterUI_callback(int code, const char* pszmsg) {
	CCLog("received enterUI_callback: code=%d, msg=%s", code, pszmsg);
}

void UCSdkCallback::isUCVip_callback(int code, bool isUCVip) {
	CCLog("received isUCVip_callback: code=%d, isVCVip=%d", code, isUCVip);
	CCMessageBox(isUCVip?"isUCVip=true":"isUCVip=false","回调提示");

	if (code == CUCStatusCode::SUCCESS) {
		UCSdk::s_isUCVip = isUCVip;
	} else {
		//fail
		CCLog("failed get whether or not current user is a UC VIP member: code=%d", code);
	}
}

void UCSdkCallback::getUCVipInfo_callback(int code, const char* pszJsonData) {
	CCLog("received getUCVipInfo_callback: code=%d, jsonData=%s", code, pszJsonData);

	if (code == CUCStatusCode::SUCCESS) {
		CCMessageBox(pszJsonData,"回调提示");
		Json::Value jsonUCVipInfo;
		Json::Reader reader;
		Json::FastWriter writer;

		if ( reader.parse(pszJsonData, jsonUCVipInfo, false) ) {
			std::string str = writer.write(jsonUCVipInfo);
			CCLog("got the current user's UC VIP member info: %s", str.c_str());

			int status = jsonUCVipInfo["status"].asInt();
			int grade = jsonUCVipInfo["grade"].asInt();
			std::string validFrom = jsonUCVipInfo["validFrom"].asString();
			std::string validTo = jsonUCVipInfo["validTo"].asString();

			CCLog("status=%d, grade=%d, validFrom=%s, validTo=%s", status, grade, validFrom.c_str(), validTo.c_str());

			Json::Value privilegeList = jsonUCVipInfo.get("privilegeList", Json::Value::null);
			if ( privilegeList.isArray() ) {
				int size = privilegeList.size();
				Json::Value privilege;

				int enjoy = 0;
				int pId = 0;

				for (int i=0; i < size; i++) {
					privilege = privilegeList[i];
					enjoy = privilege["enjoy"].asInt();
					pId = privilege["pId"].asInt();

					CCLog("privilege list[%d]: enjoy=%d, pId=%d", i, enjoy, pId);
				}
			}

		}

	} else {
		//fail
		CCLog("failed get current user's UC VIP member info: code=%d", code);
		CCMessageBox("failed get current user's UC VIP member info","回调提示");
	}
}

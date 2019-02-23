#include "base/ccConfig.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#ifndef COCOS_SCRIPTING_LUA_BINDINGS_IOSIAP_MANUAL_H
#define COCOS_SCRIPTING_LUA_BINDINGS_IOSIAP_MANUAL_H

#ifdef __cplusplus
extern "C" {
#endif
#include "tolua++.h"
#ifdef __cplusplus
}
#endif

TOLUA_API int register_all_iosIAP_manual(lua_State* L);

#endif //#ifndef COCOS_SCRIPTING_LUA_BINDINGS_LUA_COCOS2DX_EXPERIMENTAL_WEBVIEW_MANUAL_H
#endif //#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

#include "base/ccConfig.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#ifndef __iosIAP_h__
#define __iosIAP_h__

#ifdef __cplusplus
extern "C" {
#endif
#include "tolua++.h"
#ifdef __cplusplus
}
#endif

int register_all_iosIAP(lua_State* tolua_S);








#endif // __iosIAP_h__
#endif //#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

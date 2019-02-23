#include "iosIAP_manual.hpp"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "IOSiAP_Bridge.h"
#include "tolua_fix.h"
#include "LuaBasicConversions.h"
#include "CCLuaValue.h"
static int lua_iosIAP_registerHandler(lua_State* tolua_S)
{
    if (NULL == tolua_S)
        return 0;
    
    int argc = 0;
    IOSiAP_Bridge* self = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"IOSiAP_Bridge",0,&tolua_err)) goto tolua_lerror;
#endif
    
    self = static_cast<IOSiAP_Bridge*>(tolua_tousertype(tolua_S,1,0));
#if COCOS2D_DEBUG >= 1
    if (nullptr == self) {
        tolua_error(tolua_S,"invalid 'self' in function 'tolua_IOSiAP_Bridge_registerHandler'\n", NULL);
        return 0;
    }
#endif
    
    argc = lua_gettop(tolua_S) - 1;
    
    if (argc == 1)
    {
#if COCOS2D_DEBUG >= 1
        if(!toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err))
            goto tolua_lerror;
#endif
        
        LUA_FUNCTION handler = toluafix_ref_function(tolua_S,2,0);
        self->registerHandler(handler);
        
        return 0;
    }
    
    CCLOG("'registerHandler' has wrong number of arguments: %d, was expecting %d\n", argc, 1);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'registerHandler'.",&tolua_err);
    return 0;
#endif
}



static void extendIOSIAP(lua_State* L)
{
    lua_pushstring(L, "IOSiAP_Bridge");
    lua_rawget(L, LUA_REGISTRYINDEX);
    //CCLOG("extendTalkingDataGameAnalytics");
    if (lua_istable(L,-1))
    {
    CCLOG("extendTalkingDataGameAnalytics1111111111");
        tolua_function(L, "registerHandler", lua_iosIAP_registerHandler);
//        tolua_function(L,"setAccountName",lua_TalkingDataGameAnalytics_TDCCAccount_setAccountName);
//        tolua_function(L, "setAccountType", lua_TalkingDataGameAnalytics_TDCCAccount_setAccountType);
//        tolua_function(L, "setGender", lua_TalkingDataGameAnalytics_TDCCAccount_setGender);
    }
    lua_pop(L, 1);
}

int register_all_iosIAP_manual(lua_State* L)
{
    if (nullptr == L)
        return 0;
    
    
    
    extendIOSIAP(L);
    
    return 0;
}

#endif

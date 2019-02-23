#include "TalkingDataGameAnalytics_auto.hpp"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "TalkingData.h"
#include "tolua_fix.h"
#include "LuaBasicConversions.h"

int lua_TalkingDataGameAnalytics_TDCCAccount_setAccount(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"TDCCAccount",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        const char* arg0;
        std::string arg0_tmp; ok &= luaval_to_std_string(tolua_S, 2, &arg0_tmp); arg0 = arg0_tmp.c_str();
        if(!ok)
            return 0;
        TDCCAccount* ret = TDCCAccount::setAccount(arg0);
		tolua_pushusertype(tolua_S,(void*)ret,"TDCCAccount");
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "setAccount",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_TalkingDataGameAnalytics_TDCCAccount_setAccount'.",&tolua_err);
#endif
    return 0;
}
int lua_TalkingDataGameAnalytics_TDCCAccount_setAccountName(lua_State* tolua_S)
{
    int argc = 0;
    TDCCAccount* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"TDCCAccount",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (TDCCAccount*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_TalkingDataGameAnalytics_TDCCAccount_setAccountName'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        const char* arg0;

        std::string arg0_tmp; ok &= luaval_to_std_string(tolua_S, 2, &arg0_tmp); arg0 = arg0_tmp.c_str();
        if(!ok)
            return 0;
        cobj->setAccountName(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setAccountName",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_TalkingDataGameAnalytics_TDCCAccount_setAccountName'.",&tolua_err);
#endif

    return 0;
}
int lua_TalkingDataGameAnalytics_TDCCAccount_setAccountType(lua_State* tolua_S)
{
    int argc = 0;
    TDCCAccount* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"TDCCAccount",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (TDCCAccount*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_TalkingDataGameAnalytics_TDCCAccount_setAccountType'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);
        if(!ok)
            return 0;
        cobj->setAccountType((TDCCAccount::TDCCAccountType)arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setAccountType",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_TalkingDataGameAnalytics_TDCCAccount_setAccountType'.",&tolua_err);
#endif

    return 0;
}

int lua_TalkingDataGameAnalytics_TDCCAccount_setGender(lua_State* tolua_S)
{
    int argc = 0;
    TDCCAccount* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"TDCCAccount",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (TDCCAccount*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_TalkingDataGameAnalytics_TDCCAccount_setGender'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);
        if(!ok)
            return 0;
        cobj->setGender((TDCCAccount::TDCCGender)arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setGender",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_TalkingDataGameAnalytics_TDCCAccount_setGender'.",&tolua_err);
#endif

    return 0;
}

static void extendTalkingDataGameAnalytics(lua_State* L)
{
    lua_pushstring(L, "TDCCAccount");
    lua_rawget(L, LUA_REGISTRYINDEX);
    CCLOG("extendTalkingDataGameAnalytics");
    if (lua_istable(L,-1))
    {
    CCLOG("extendTalkingDataGameAnalytics1111111111");
        tolua_function(L, "setAccount", lua_TalkingDataGameAnalytics_TDCCAccount_setAccount);
        tolua_function(L,"setAccountName",lua_TalkingDataGameAnalytics_TDCCAccount_setAccountName);
        tolua_function(L, "setAccountType", lua_TalkingDataGameAnalytics_TDCCAccount_setAccountType);
        tolua_function(L, "setGender", lua_TalkingDataGameAnalytics_TDCCAccount_setGender);
    }
    lua_pop(L, 1);
}

int register_all_TalkingDataGameAnalytics_manual(lua_State* L)
{
    if (nullptr == L)
        return 0;
    
    extendTalkingDataGameAnalytics(L);
    
    return 0;
}

#endif

#include "TalkingDataGameAnalytics_auto.hpp"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "TalkingData.h"
#include "tolua_fix.h"
#include "LuaBasicConversions.h"



int lua_TalkingDataGameAnalytics_TDCCTalkingDataGA_setVerboseLogEnabled(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"TDCCTalkingDataGA",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
            return 0;
        TDCCTalkingDataGA::setVerboseLogEnabled();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "setVerboseLogEnabled",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_TalkingDataGameAnalytics_TDCCTalkingDataGA_setVerboseLogEnabled'.",&tolua_err);
#endif
    return 0;
}
int lua_TalkingDataGameAnalytics_TDCCTalkingDataGA_onKill(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"TDCCTalkingDataGA",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
            return 0;
        TDCCTalkingDataGA::onKill();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "onKill",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_TalkingDataGameAnalytics_TDCCTalkingDataGA_onKill'.",&tolua_err);
#endif
    return 0;
}
int lua_TalkingDataGameAnalytics_TDCCTalkingDataGA_onStart(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"TDCCTalkingDataGA",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 2)
    {
        const char* arg0;
        const char* arg1;
        std::string arg0_tmp; ok &= luaval_to_std_string(tolua_S, 2, &arg0_tmp); arg0 = arg0_tmp.c_str();
        std::string arg1_tmp; ok &= luaval_to_std_string(tolua_S, 3, &arg1_tmp); arg1 = arg1_tmp.c_str();
        if(!ok)
            return 0;
        TDCCTalkingDataGA::onStart(arg0, arg1);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "onStart",argc, 2);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_TalkingDataGameAnalytics_TDCCTalkingDataGA_onStart'.",&tolua_err);
#endif
    return 0;
}
int lua_TalkingDataGameAnalytics_TDCCTalkingDataGA_getDeviceId(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"TDCCTalkingDataGA",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
            return 0;
        const char* ret = TDCCTalkingDataGA::getDeviceId();
        tolua_pushstring(tolua_S,(const char*)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "getDeviceId",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_TalkingDataGameAnalytics_TDCCTalkingDataGA_getDeviceId'.",&tolua_err);
#endif
    return 0;
}
int lua_TalkingDataGameAnalytics_TDCCTalkingDataGA_onEvent(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"TDCCTalkingDataGA",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        const char* arg0;
        std::string arg0_tmp; ok &= luaval_to_std_string(tolua_S, 2, &arg0_tmp); arg0 = arg0_tmp.c_str();
        if(!ok)
            return 0;
        TDCCTalkingDataGA::onEvent(arg0);
        return 0;
    }
    if (argc == 2)
    {
        const char* arg0;
        std::map<std::basic_string<char>, std::basic_string<char>, std::less<std::basic_string<char> >, std::allocator<std::pair<const std::basic_string<char>, std::basic_string<char> > > >* arg1;
        std::string arg0_tmp; ok &= luaval_to_std_string(tolua_S, 2, &arg0_tmp); arg0 = arg0_tmp.c_str();
        ok &= luaval_to_object<std::map<std::basic_string<char>, std::basic_string<char>, std::less<std::basic_string<char> >, std::allocator<std::pair<const std::basic_string<char>, std::basic_string<char> > > >>(tolua_S, 3, "std::map<std::basic_string<char>, std::basic_string<char>, std::less<std::basic_string<char> >, std::allocator<std::pair<const std::basic_string<char>, std::basic_string<char> > > >*",&arg1);
        if(!ok)
            return 0;
        TDCCTalkingDataGA::onEvent(arg0, arg1);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "onEvent",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_TalkingDataGameAnalytics_TDCCTalkingDataGA_onEvent'.",&tolua_err);
#endif
    return 0;
}
int lua_TalkingDataGameAnalytics_TDCCTalkingDataGA_setVerboseLogDisabled(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"TDCCTalkingDataGA",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
            return 0;
        TDCCTalkingDataGA::setVerboseLogDisabled();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "setVerboseLogDisabled",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_TalkingDataGameAnalytics_TDCCTalkingDataGA_setVerboseLogDisabled'.",&tolua_err);
#endif
    return 0;
}
int lua_TalkingDataGameAnalytics_TDCCTalkingDataGA_setLatitude(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"TDCCTalkingDataGA",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 2)
    {
        double arg0;
        double arg1;
        ok &= luaval_to_number(tolua_S, 2,&arg0);
        ok &= luaval_to_number(tolua_S, 3,&arg1);
        if(!ok)
            return 0;
        TDCCTalkingDataGA::setLatitude(arg0, arg1);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "setLatitude",argc, 2);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_TalkingDataGameAnalytics_TDCCTalkingDataGA_setLatitude'.",&tolua_err);
#endif
    return 0;
}
static int lua_TalkingDataGameAnalytics_TDCCTalkingDataGA_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (TDCCTalkingDataGA)");
    return 0;
}

int lua_register_TalkingDataGameAnalytics_TDCCTalkingDataGA(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"TDCCTalkingDataGA");
    tolua_cclass(tolua_S,"TDCCTalkingDataGA","TDCCTalkingDataGA","",nullptr);

    tolua_beginmodule(tolua_S,"TDCCTalkingDataGA");
        tolua_function(tolua_S,"setVerboseLogEnabled", lua_TalkingDataGameAnalytics_TDCCTalkingDataGA_setVerboseLogEnabled);
        tolua_function(tolua_S,"onKill", lua_TalkingDataGameAnalytics_TDCCTalkingDataGA_onKill);
        tolua_function(tolua_S,"onStart", lua_TalkingDataGameAnalytics_TDCCTalkingDataGA_onStart);
        tolua_function(tolua_S,"getDeviceId", lua_TalkingDataGameAnalytics_TDCCTalkingDataGA_getDeviceId);
        tolua_function(tolua_S,"onEvent", lua_TalkingDataGameAnalytics_TDCCTalkingDataGA_onEvent);
        tolua_function(tolua_S,"setVerboseLogDisabled", lua_TalkingDataGameAnalytics_TDCCTalkingDataGA_setVerboseLogDisabled);
        tolua_function(tolua_S,"setLatitude", lua_TalkingDataGameAnalytics_TDCCTalkingDataGA_setLatitude);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(TDCCTalkingDataGA).name();
    g_luaType[typeName] = "TDCCTalkingDataGA";
    g_typeCast["TDCCTalkingDataGA"] = "TDCCTalkingDataGA";
    return 1;
}

int lua_TalkingDataGameAnalytics_TDCCAccount_setAge(lua_State* tolua_S)
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
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_TalkingDataGameAnalytics_TDCCAccount_setAge'", nullptr);
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
        cobj->setAge(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setAge",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_TalkingDataGameAnalytics_TDCCAccount_setAge'.",&tolua_err);
#endif

    return 0;
}
int lua_TalkingDataGameAnalytics_TDCCAccount_setGameServer(lua_State* tolua_S)
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
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_TalkingDataGameAnalytics_TDCCAccount_setGameServer'", nullptr);
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
        cobj->setGameServer(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setGameServer",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_TalkingDataGameAnalytics_TDCCAccount_setGameServer'.",&tolua_err);
#endif

    return 0;
}
int lua_TalkingDataGameAnalytics_TDCCAccount_setLevel(lua_State* tolua_S)
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
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_TalkingDataGameAnalytics_TDCCAccount_setLevel'", nullptr);
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
        cobj->setLevel(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setLevel",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_TalkingDataGameAnalytics_TDCCAccount_setLevel'.",&tolua_err);
#endif

    return 0;
}
static int lua_TalkingDataGameAnalytics_TDCCAccount_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (TDCCAccount)");
    return 0;
}

int lua_register_TalkingDataGameAnalytics_TDCCAccount(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"TDCCAccount");
    tolua_cclass(tolua_S,"TDCCAccount","TDCCAccount","",nullptr);

    tolua_beginmodule(tolua_S,"TDCCAccount");
        tolua_function(tolua_S,"setAge",lua_TalkingDataGameAnalytics_TDCCAccount_setAge);
        tolua_function(tolua_S,"setGameServer",lua_TalkingDataGameAnalytics_TDCCAccount_setGameServer);
        tolua_function(tolua_S,"setLevel",lua_TalkingDataGameAnalytics_TDCCAccount_setLevel);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(TDCCAccount).name();
    g_luaType[typeName] = "TDCCAccount";
    g_typeCast["TDCCAccount"] = "TDCCAccount";
    return 1;
}

int lua_TalkingDataGameAnalytics_TDCCMission_onFailed(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"TDCCMission",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 2)
    {
        const char* arg0;
        const char* arg1;
        std::string arg0_tmp; ok &= luaval_to_std_string(tolua_S, 2, &arg0_tmp); arg0 = arg0_tmp.c_str();
        std::string arg1_tmp; ok &= luaval_to_std_string(tolua_S, 3, &arg1_tmp); arg1 = arg1_tmp.c_str();
        if(!ok)
            return 0;
        TDCCMission::onFailed(arg0, arg1);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "onFailed",argc, 2);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_TalkingDataGameAnalytics_TDCCMission_onFailed'.",&tolua_err);
#endif
    return 0;
}
int lua_TalkingDataGameAnalytics_TDCCMission_onBegin(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"TDCCMission",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        const char* arg0;
        std::string arg0_tmp; ok &= luaval_to_std_string(tolua_S, 2, &arg0_tmp); arg0 = arg0_tmp.c_str();
        if(!ok)
            return 0;
        TDCCMission::onBegin(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "onBegin",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_TalkingDataGameAnalytics_TDCCMission_onBegin'.",&tolua_err);
#endif
    return 0;
}
int lua_TalkingDataGameAnalytics_TDCCMission_onCompleted(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"TDCCMission",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        const char* arg0;
        std::string arg0_tmp; ok &= luaval_to_std_string(tolua_S, 2, &arg0_tmp); arg0 = arg0_tmp.c_str();
        if(!ok)
            return 0;
        TDCCMission::onCompleted(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "onCompleted",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_TalkingDataGameAnalytics_TDCCMission_onCompleted'.",&tolua_err);
#endif
    return 0;
}
static int lua_TalkingDataGameAnalytics_TDCCMission_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (TDCCMission)");
    return 0;
}

int lua_register_TalkingDataGameAnalytics_TDCCMission(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"TDCCMission");
    tolua_cclass(tolua_S,"TDCCMission","TDCCMission","",nullptr);

    tolua_beginmodule(tolua_S,"TDCCMission");
        tolua_function(tolua_S,"onFailed", lua_TalkingDataGameAnalytics_TDCCMission_onFailed);
        tolua_function(tolua_S,"onBegin", lua_TalkingDataGameAnalytics_TDCCMission_onBegin);
        tolua_function(tolua_S,"onCompleted", lua_TalkingDataGameAnalytics_TDCCMission_onCompleted);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(TDCCMission).name();
    g_luaType[typeName] = "TDCCMission";
    g_typeCast["TDCCMission"] = "TDCCMission";
    return 1;
}

int lua_TalkingDataGameAnalytics_TDCCVirtualCurrency_onChargeRequest(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"TDCCVirtualCurrency",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 6)
    {
        const char* arg0;
        const char* arg1;
        double arg2;
        const char* arg3;
        double arg4;
        const char* arg5;
        std::string arg0_tmp; ok &= luaval_to_std_string(tolua_S, 2, &arg0_tmp); arg0 = arg0_tmp.c_str();
        std::string arg1_tmp; ok &= luaval_to_std_string(tolua_S, 3, &arg1_tmp); arg1 = arg1_tmp.c_str();
        ok &= luaval_to_number(tolua_S, 4,&arg2);
        std::string arg3_tmp; ok &= luaval_to_std_string(tolua_S, 5, &arg3_tmp); arg3 = arg3_tmp.c_str();
        ok &= luaval_to_number(tolua_S, 6,&arg4);
        std::string arg5_tmp; ok &= luaval_to_std_string(tolua_S, 7, &arg5_tmp); arg5 = arg5_tmp.c_str();
        if(!ok)
            return 0;
        TDCCVirtualCurrency::onChargeRequest(arg0, arg1, arg2, arg3, arg4, arg5);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "onChargeRequest",argc, 6);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_TalkingDataGameAnalytics_TDCCVirtualCurrency_onChargeRequest'.",&tolua_err);
#endif
    return 0;
}
int lua_TalkingDataGameAnalytics_TDCCVirtualCurrency_onReward(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"TDCCVirtualCurrency",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 2)
    {
        double arg0;
        const char* arg1;
        ok &= luaval_to_number(tolua_S, 2,&arg0);
        std::string arg1_tmp; ok &= luaval_to_std_string(tolua_S, 3, &arg1_tmp); arg1 = arg1_tmp.c_str();
        if(!ok)
            return 0;
        TDCCVirtualCurrency::onReward(arg0, arg1);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "onReward",argc, 2);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_TalkingDataGameAnalytics_TDCCVirtualCurrency_onReward'.",&tolua_err);
#endif
    return 0;
}
int lua_TalkingDataGameAnalytics_TDCCVirtualCurrency_onChargeSuccess(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"TDCCVirtualCurrency",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        const char* arg0;
        std::string arg0_tmp; ok &= luaval_to_std_string(tolua_S, 2, &arg0_tmp); arg0 = arg0_tmp.c_str();
        if(!ok)
            return 0;
        TDCCVirtualCurrency::onChargeSuccess(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "onChargeSuccess",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_TalkingDataGameAnalytics_TDCCVirtualCurrency_onChargeSuccess'.",&tolua_err);
#endif
    return 0;
}
static int lua_TalkingDataGameAnalytics_TDCCVirtualCurrency_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (TDCCVirtualCurrency)");
    return 0;
}

int lua_register_TalkingDataGameAnalytics_TDCCVirtualCurrency(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"TDCCVirtualCurrency");
    tolua_cclass(tolua_S,"TDCCVirtualCurrency","TDCCVirtualCurrency","",nullptr);

    tolua_beginmodule(tolua_S,"TDCCVirtualCurrency");
        tolua_function(tolua_S,"onChargeRequest", lua_TalkingDataGameAnalytics_TDCCVirtualCurrency_onChargeRequest);
        tolua_function(tolua_S,"onReward", lua_TalkingDataGameAnalytics_TDCCVirtualCurrency_onReward);
        tolua_function(tolua_S,"onChargeSuccess", lua_TalkingDataGameAnalytics_TDCCVirtualCurrency_onChargeSuccess);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(TDCCVirtualCurrency).name();
    g_luaType[typeName] = "TDCCVirtualCurrency";
    g_typeCast["TDCCVirtualCurrency"] = "TDCCVirtualCurrency";
    return 1;
}
TOLUA_API int register_all_TalkingDataGameAnalytics(lua_State* tolua_S)
{
	tolua_open(tolua_S);
	
	tolua_module(tolua_S,"tdga",0);
	tolua_beginmodule(tolua_S,"tdga");

	lua_register_TalkingDataGameAnalytics_TDCCAccount(tolua_S);
	lua_register_TalkingDataGameAnalytics_TDCCVirtualCurrency(tolua_S);
	lua_register_TalkingDataGameAnalytics_TDCCMission(tolua_S);
	lua_register_TalkingDataGameAnalytics_TDCCTalkingDataGA(tolua_S);

	tolua_endmodule(tolua_S);
	return 1;
}

#endif

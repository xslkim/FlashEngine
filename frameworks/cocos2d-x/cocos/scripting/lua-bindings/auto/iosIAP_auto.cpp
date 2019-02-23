#include "iosIAP_auto.hpp"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "IOSiAP_Bridge.h"
#include "tolua_fix.h"
#include "LuaBasicConversions.h"



int lua_iosIAP_IOSiAP_Bridge_unregisterHandler(lua_State* tolua_S)
{
    int argc = 0;
    IOSiAP_Bridge* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"IOSiAP_Bridge",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (IOSiAP_Bridge*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_iosIAP_IOSiAP_Bridge_unregisterHandler'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->unregisterHandler();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "unregisterHandler",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_iosIAP_IOSiAP_Bridge_unregisterHandler'.",&tolua_err);
#endif

    return 0;
}
int lua_iosIAP_IOSiAP_Bridge_onRequestProductsFinish(lua_State* tolua_S)
{
    int argc = 0;
    IOSiAP_Bridge* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"IOSiAP_Bridge",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (IOSiAP_Bridge*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_iosIAP_IOSiAP_Bridge_onRequestProductsFinish'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->onRequestProductsFinish();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "onRequestProductsFinish",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_iosIAP_IOSiAP_Bridge_onRequestProductsFinish'.",&tolua_err);
#endif

    return 0;
}
int lua_iosIAP_IOSiAP_Bridge_onPaymentEvent(lua_State* tolua_S)
{
    int argc = 0;
    IOSiAP_Bridge* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"IOSiAP_Bridge",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (IOSiAP_Bridge*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_iosIAP_IOSiAP_Bridge_onPaymentEvent'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 3) 
    {
        std::string arg0;
        IOSiAPPaymentEvent arg1;
        int arg2;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);

        ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1);

        ok &= luaval_to_int32(tolua_S, 4,(int *)&arg2);
        if(!ok)
            return 0;
        cobj->onPaymentEvent(arg0, arg1, arg2);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "onPaymentEvent",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_iosIAP_IOSiAP_Bridge_onPaymentEvent'.",&tolua_err);
#endif

    return 0;
}
int lua_iosIAP_IOSiAP_Bridge_onRequestProductsError(lua_State* tolua_S)
{
    int argc = 0;
    IOSiAP_Bridge* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"IOSiAP_Bridge",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (IOSiAP_Bridge*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_iosIAP_IOSiAP_Bridge_onRequestProductsError'", nullptr);
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
        cobj->onRequestProductsError(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "onRequestProductsError",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_iosIAP_IOSiAP_Bridge_onRequestProductsError'.",&tolua_err);
#endif

    return 0;
}
int lua_iosIAP_IOSiAP_Bridge_requestProducts(lua_State* tolua_S)
{
    int argc = 0;
    IOSiAP_Bridge* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"IOSiAP_Bridge",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (IOSiAP_Bridge*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_iosIAP_IOSiAP_Bridge_requestProducts'", nullptr);
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
        cobj->requestProducts(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "requestProducts",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_iosIAP_IOSiAP_Bridge_requestProducts'.",&tolua_err);
#endif

    return 0;
}
int lua_iosIAP_IOSiAP_Bridge_constructor(lua_State* tolua_S)
{
    int argc = 0;
    IOSiAP_Bridge* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif



    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj = new IOSiAP_Bridge();
        tolua_pushusertype(tolua_S,(void*)cobj,"IOSiAP_Bridge");
        tolua_register_gc(tolua_S,lua_gettop(tolua_S));
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "IOSiAP_Bridge",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_error(tolua_S,"#ferror in function 'lua_iosIAP_IOSiAP_Bridge_constructor'.",&tolua_err);
#endif

    return 0;
}

static int lua_iosIAP_IOSiAP_Bridge_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (IOSiAP_Bridge)");
    return 0;
}

int lua_register_iosIAP_IOSiAP_Bridge(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"IOSiAP_Bridge");
    tolua_cclass(tolua_S,"IOSiAP_Bridge","IOSiAP_Bridge","IOSiAPDelegate",nullptr);

    tolua_beginmodule(tolua_S,"IOSiAP_Bridge");
        tolua_function(tolua_S,"new",lua_iosIAP_IOSiAP_Bridge_constructor);
        tolua_function(tolua_S,"unregisterHandler",lua_iosIAP_IOSiAP_Bridge_unregisterHandler);
        tolua_function(tolua_S,"onRequestProductsFinish",lua_iosIAP_IOSiAP_Bridge_onRequestProductsFinish);
        tolua_function(tolua_S,"onPaymentEvent",lua_iosIAP_IOSiAP_Bridge_onPaymentEvent);
        tolua_function(tolua_S,"onRequestProductsError",lua_iosIAP_IOSiAP_Bridge_onRequestProductsError);
        tolua_function(tolua_S,"requestProducts",lua_iosIAP_IOSiAP_Bridge_requestProducts);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(IOSiAP_Bridge).name();
    g_luaType[typeName] = "IOSiAP_Bridge";
    g_typeCast["IOSiAP_Bridge"] = "IOSiAP_Bridge";
    return 1;
}
TOLUA_API int register_all_iosIAP(lua_State* tolua_S)
{
	tolua_open(tolua_S);
	
	tolua_module(tolua_S,"iap",0);
	tolua_beginmodule(tolua_S,"iap");

	lua_register_iosIAP_IOSiAP_Bridge(tolua_S);

	tolua_endmodule(tolua_S);
	return 1;
}

#endif

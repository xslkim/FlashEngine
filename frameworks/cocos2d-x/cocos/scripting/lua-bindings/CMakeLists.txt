set(LUABINDING_SRC
	  auto/lua_cocos2dx_auto.cpp
	  auto/lua_cocos2dx_extension_auto.cpp
	  auto/lua_cocos2dx_studio_auto.cpp
	  auto/lua_cocos2dx_ui_auto.cpp
	  auto/lua_cocos2dx_spine_auto.cpp
	  auto/lua_cocos2dx_physics_auto.cpp
	  auto/lua_cocos2dx_experimental_auto.cpp
	  auto/lua_cocos2dx_experimental_video_auto.cpp
	  manual/CCLuaEngine.cpp
	  manual/CCLuaStack.cpp
	  manual/lua_debugger.c
	  manual/CCLuaValue.cpp
	  manual/Cocos2dxLuaLoader.cpp
	  manual/CCBProxy.cpp
	  manual/Lua_web_socket.cpp
	  manual/LuaOpengl.cpp
	  manual/LuaScriptHandlerMgr.cpp
	  manual/LuaBasicConversions.cpp
	  manual/LuaSkeletonAnimation.cpp
	  manual/lua_cocos2dx_manual.cpp
	  manual/lua_cocos2dx_extension_manual.cpp
	  manual/lua_cocos2dx_coco_studio_manual.cpp
	  manual/lua_cocos2dx_ui_manual.cpp
	  manual/lua_cocos2dx_spine_manual.cpp
	  manual/lua_cocos2dx_physics_manual.cpp
	  manual/lua_cocos2dx_experimental_manual.cpp
	  manual/lua_cocos2dx_experimental_video_manual.cpp
	  manual/lua_cocos2dx_deprecated.cpp
	  manual/lua_xml_http_request.cpp
	  manual/tolua_fix.cpp
	  manual/cjson/fpconv.c
	  manual/cjson/lua_cjson.c
	  manual/cjson/strbuf.c
	  manual/lua_extensions_ex.cpp
)

include_directories(
  auto
  manual
  ../../editor-support/cocosbuilder
  ../../editor-support/cocostudio
  ../../editor-support/spine
  ../../ui
  ../../2d
  ../../3d
  ../../../external/lua
  ../../../external/lua/lua
  ../../../external/lua/luajit/include
  ../../../external/lua/tolua
  ../../../external/lua/luasocket
  ../../../extensions
  ../../../external/xxtea
)


add_library(luabinding STATIC
  ${LUABINDING_SRC}
)

target_link_libraries(luabinding
  tolua
  lua
  spine
  xxtea
  luasocket
)

set_target_properties(luabinding
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)


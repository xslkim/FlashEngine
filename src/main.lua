
require "Cocos2d"
require "Cocos2dConstants"

local cclog = function(...)
    print(string.format(...))
end

-- for CCLuaEngine traceback
function __G__TRACKBACK__(msg)
    cclog("----------------------------------------")
    cclog("LUA ERROR: " .. tostring(msg) .. "\n")
    cclog(debug.traceback())
    cclog("----------------------------------------")
    if Utils.pvpMode then
        --结束进程
        cc.PvpManager:getInstance():backupFun1(self)
    end
    return msg
end

local function main()
    collectgarbage("collect")
    -- avoid memory leak
    collectgarbage("setpause", 100)
    collectgarbage("setstepmul", 5000)

    cc.Director:getInstance():setDisplayStats(false)

    cc.FileUtils:getInstance():addSearchPath("src")
    cc.FileUtils:getInstance():addSearchPath("res")
    cc.Director:getInstance():getOpenGLView():setDesignResolutionSize(1280, 720, 2)

	require("TestScene")
    local scene = TestScene.create()
    cc.Director:getInstance():replaceScene(scene)

end


local status, msg = xpcall(main, __G__TRACKBACK__)
if not status then
    error(msg)
end

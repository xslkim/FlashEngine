require "Cocos2d"


TestScene = class("TestScene",function()
    return cc.Scene:create()
end)

function TestScene.create()
    local scene = TestScene.new()
    scene:init()
    return scene;
end


function TestScene:init()

    local layer = cc.Layer:create()


    local spt = cc.FlashSprite:create("res/hero/r11_ailiya")
    spt:setPosition(300, 200)
    spt:changeAnimation(0)
    spt:setGray(true)
    layer:addChild(spt)
    
    local spt = cc.FlashSprite:create("res/hero/r11_ailiya")
    spt:setPosition(300, 350)
    spt:changeAnimation(1)
    --spt:setGray(true)
    layer:addChild(spt)
	
	local spt = cc.FlashSprite:create("res/hero/r3_zhanmu")
    spt:setPosition(600, 200)
    spt:changeAnimation(0)
    layer:addChild(spt)
	
	
	local spt = cc.FlashSprite:create("res/hero/r3_zhanmu")
    spt:setPosition(600, 350)
    spt:changeAnimation(1)
    layer:addChild(spt)
	
	local spt = cc.FlashSprite:create("res/hero/r3_zhanmu")
    spt:setPosition(600, 500)
    spt:changeAnimation(2)
    layer:addChild(spt)
	
	
	local spt = cc.FlashSprite:create("res/hero/r3_zhanmu")
    spt:setPosition(900, 200)
    spt:changeAnimation(3)
    layer:addChild(spt)
	
	
	local spt = cc.FlashSprite:create("res/hero/r3_zhanmu")
    spt:setPosition(900, 350)
    spt:changeAnimation(4)
    layer:addChild(spt)
	
	local spt = cc.FlashSprite:create("res/hero/r3_zhanmu")
    spt:setPosition(900, 500)
    spt:changeAnimation(5)
    layer:addChild(spt)
	
	
    self:addChild(layer)
end
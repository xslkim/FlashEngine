# FlashEngine
- Create 2d Animation user flash. 
- Export flash animation to cocos2d animation.
- Integration animation to cocos2d engine.

# Run Demo
Direct run runtime/win32/icefire.exe in windows.
- lua code: src/
    ```lua
      local spt = cc.FlashSprite:create("res/hero/r11_ailiya") --load animation data
      spt:setPosition(300, 200)
      spt:changeAnimation(0)  -- each animation has lot of actions
      layer:addChild(spt)  
    ```
- cocos2d animation: res/hero/    
    generate by flash animation 
  ```
  r11_ailiya.bb --collide data
  r11_ailiya.fpk --animation data
  r11_ailiya.plist & .png --render resource
  ```
 # Export Flash animation
 
 
 

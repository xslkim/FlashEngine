
--------------------------------
-- @module MeshSprite
-- @extend Node,TextureProtocol
-- @parent_module cc

--------------------------------
-- @function [parent=#MeshSprite] reset 
-- @param self
        
--------------------------------
-- @function [parent=#MeshSprite] setTexture 
-- @param self
-- @param #cc.Texture2D texture2d
        
--------------------------------
-- @function [parent=#MeshSprite] getTexture 
-- @param self
-- @return Texture2D#Texture2D ret (return value: cc.Texture2D)
        
--------------------------------
-- @function [parent=#MeshSprite] getTexCoord 
-- @param self
-- @param #int int
-- @param #int int
-- @return vec2_table#vec2_table ret (return value: vec2_table)
        
--------------------------------
-- @function [parent=#MeshSprite] setBlendFunc 
-- @param self
-- @param #cc.BlendFunc blendfunc
        
--------------------------------
-- @function [parent=#MeshSprite] setTexCoord 
-- @param self
-- @param #int int
-- @param #int int
-- @param #vec2_table vec2
        
--------------------------------
-- @function [parent=#MeshSprite] getBlendFunc 
-- @param self
-- @return BlendFunc#BlendFunc ret (return value: cc.BlendFunc)
        
--------------------------------
-- @function [parent=#MeshSprite] getVertice 
-- @param self
-- @param #int int
-- @param #int int
-- @return vec2_table#vec2_table ret (return value: vec2_table)
        
--------------------------------
-- @function [parent=#MeshSprite] tintWithColor 
-- @param self
-- @param #color3b_table color3b
        
--------------------------------
-- @function [parent=#MeshSprite] setVertice 
-- @param self
-- @param #int int
-- @param #int int
-- @param #vec2_table vec2
        
--------------------------------
-- @overload self, int, int, cc.Texture2D         
-- @overload self, int, int, string         
-- @function [parent=#MeshSprite] create
-- @param self
-- @param #int int
-- @param #int int
-- @param #string str
-- @return MeshSprite#MeshSprite ret (retunr value: cc.MeshSprite)

--------------------------------
-- @function [parent=#MeshSprite] draw 
-- @param self
-- @param #cc.Renderer renderer
-- @param #mat4_table mat4
-- @param #unsigned int int
        
--------------------------------
-- @function [parent=#MeshSprite] isOpacityModifyRGB 
-- @param self
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- @function [parent=#MeshSprite] setOpacity 
-- @param self
-- @param #unsigned char char
        
--------------------------------
-- @function [parent=#MeshSprite] setOpacityModifyRGB 
-- @param self
-- @param #bool bool
        
--------------------------------
-- @function [parent=#MeshSprite] update 
-- @param self
-- @param #float float
        
--------------------------------
-- @function [parent=#MeshSprite] getOpacity 
-- @param self
-- @return unsigned char#unsigned char ret (return value: unsigned char)
        
return nil

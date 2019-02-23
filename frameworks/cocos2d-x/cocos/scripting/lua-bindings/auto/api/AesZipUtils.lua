
--------------------------------
-- @module AesZipUtils
-- @extend Ref
-- @parent_module cc

--------------------------------
-- @function [parent=#AesZipUtils] json2netstring 
-- @param self
-- @param #char char
-- @return char#char ret (return value: char)
        
--------------------------------
-- @function [parent=#AesZipUtils] initKV 
-- @param self
-- @param #char char
-- @param #char char
        
--------------------------------
-- @function [parent=#AesZipUtils] ccInflate 
-- @param self
-- @param #unsigned char char
-- @param #int int
-- @param #int int
-- @return unsigned char#unsigned char ret (return value: unsigned char)
        
--------------------------------
-- @function [parent=#AesZipUtils] ccInflateChar 
-- @param self
-- @param #char char
-- @param #int int
-- @param #int int
-- @return char#char ret (return value: char)
        
--------------------------------
-- @function [parent=#AesZipUtils] ccCompressChar 
-- @param self
-- @param #char char
-- @param #int int
-- @param #int int
-- @return char#char ret (return value: char)
        
--------------------------------
-- @function [parent=#AesZipUtils] aesEncrypt 
-- @param self
-- @param #char char
-- @param #int int
-- @return string#string ret (return value: string)
        
--------------------------------
-- @function [parent=#AesZipUtils] releaseBuf 
-- @param self
-- @param #void void
        
--------------------------------
-- @function [parent=#AesZipUtils] aesDecrypt 
-- @param self
-- @param #char char
-- @param #int int
-- @return string#string ret (return value: string)
        
--------------------------------
-- @function [parent=#AesZipUtils] ccCompress 
-- @param self
-- @param #unsigned char char
-- @param #int int
-- @param #int int
-- @return unsigned char#unsigned char ret (return value: unsigned char)
        
--------------------------------
-- @function [parent=#AesZipUtils] releaseBufChar 
-- @param self
-- @param #void void
        
--------------------------------
-- @function [parent=#AesZipUtils] netstring2json 
-- @param self
-- @param #char char
-- @return char#char ret (return value: char)
        
--------------------------------
-- @function [parent=#AesZipUtils] getInstance 
-- @param self
-- @return AesZipUtils#AesZipUtils ret (return value: cc.AesZipUtils)
        
return nil

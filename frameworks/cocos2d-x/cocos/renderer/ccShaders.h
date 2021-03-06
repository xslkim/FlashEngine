/****************************************************************************
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2012 		cocos2d-x.org
Copyright (c) 2013-2014 Chukong Technologies Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
#ifndef __CCSHADER_H__
#define __CCSHADER_H__

#include "CCGL.h"
#include "base/CCPlatformMacros.h"

NS_CC_BEGIN

/**
 * @addtogroup shaders
 * @{
 */

extern CC_DLL const GLchar * ccPosition_uColor_frag;
extern CC_DLL const GLchar * ccPosition_uColor_vert;

extern CC_DLL const GLchar * ccPositionColor_frag;
extern CC_DLL const GLchar * ccPositionColor_vert;

extern CC_DLL const GLchar * ccPositionTexture_frag;
extern CC_DLL const GLchar * ccPositionTexture_vert;

extern CC_DLL const GLchar * ccPositionTextureA8Color_frag;
extern CC_DLL const GLchar * ccPositionTextureA8Color_vert;

extern CC_DLL const GLchar * ccPositionTextureColor_frag;
extern CC_DLL const GLchar * ccPositionTextureColor_vert;

extern CC_DLL const GLchar * ccPositionTextureColor_noMVP_frag;
extern CC_DLL const GLchar * ccPositionTextureColor_noMVP_vert;

extern CC_DLL const GLchar * ccPositionTextureColorAlphaTest_frag;

extern CC_DLL const GLchar * ccPositionTexture_uColor_frag;
extern CC_DLL const GLchar * ccPositionTexture_uColor_vert;

extern CC_DLL const GLchar * ccPositionColorLengthTexture_frag;
extern CC_DLL const GLchar * ccPositionColorLengthTexture_vert;

extern CC_DLL const GLchar * ccLabelDistanceFieldNormal_frag;
extern CC_DLL const GLchar * ccLabelDistanceFieldGlow_frag;
extern CC_DLL const GLchar * ccLabelNormal_frag;
extern CC_DLL const GLchar * ccLabelOutline_frag;
extern CC_DLL const GLchar * ccLabelNormalI8_frag;

extern CC_DLL const GLchar * ccLabel_vert;

extern CC_DLL const GLchar * cc3D_PositionTex_vert;
extern CC_DLL const GLchar * cc3D_SkinPositionTex_vert;
extern CC_DLL const GLchar * cc3D_ColorTex_frag;
extern CC_DLL const GLchar * cc3D_Color_frag;

extern CC_DLL const GLchar * ccPositionTextureGray_frag;
extern CC_DLL const GLchar * ccPositionTextureGray_vert;

extern CC_DLL const GLchar * ccPositionTextureBright_frag;
extern CC_DLL const GLchar * ccPositionTextureBright_vert;

extern CC_DLL const GLchar * ccPositionTextureRed100_frag;
extern CC_DLL const GLchar * ccPositionTextureRed100_vert;

extern CC_DLL const GLchar * ccPositionTextureRed80_frag;
extern CC_DLL const GLchar * ccPositionTextureRed80_vert;

extern CC_DLL const GLchar * ccPositionTextureRed60_frag;
extern CC_DLL const GLchar * ccPositionTextureRed60_vert;

extern CC_DLL const GLchar * ccPositionTextureRed40_frag;
extern CC_DLL const GLchar * ccPositionTextureRed40_vert;

extern CC_DLL const GLchar * ccPositionTextureRed20_frag;
extern CC_DLL const GLchar * ccPositionTextureRed20_vert;

extern CC_DLL const GLchar * ccPositionTextureGreen100_frag;
extern CC_DLL const GLchar * ccPositionTextureGreen100_vert;

extern CC_DLL const GLchar * ccPositionTextureGolden100_frag;
extern CC_DLL const GLchar * ccPositionTextureGolden100_vert;

extern CC_DLL const GLchar * ccPositionTextureBurn100_frag;
extern CC_DLL const GLchar * ccPositionTextureBurn100_vert;

extern CC_DLL const GLchar * ccPositionTextureColorI2A_frag;

extern CC_DLL const GLchar * ccPositionTextureColor_noMVP_Gray_frag;
// end of shaders group
/// @}

NS_CC_END

#endif /* __CCSHADER_H__ */

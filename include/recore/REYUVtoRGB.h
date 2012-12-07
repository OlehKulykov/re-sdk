
#ifndef __REYUVTORGB_H__
#define __REYUVTORGB_H__

#include "RECommonHeader.h"

/*
 Realization of REYUVtoRGB methods using code provided by 2008-11 Robin Watts <theorarm@wss.co.uk>
 You can ignore this code using config flag __RE_CORE_NO_YUV_TO_RGB_CONVERSION__
 Content of 'COPYING', 'README' file and 'HEADER TEXT' listed below.
 */


/* HEADER TEXT */
/* YUV-> RGB conversion code.
 *
 * Copyright (C) 2011 Robin Watts (robin@wss.co.uk) for Pinknoise
 * Productions Ltd.
 *
 * Licensed under the BSD license. See 'COPYING' for details of
 * (non-)warranty.
 *
 */


/* COPYING */
/*
 This YUV2RGB code is Copyright (C) 2008-11 Robin Watts
 <theorarm@wss.co.uk>.
 
 The software is released under the BSD license.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION
 OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 
 In particular, I warrant absolutely nothing about how patent free
 this method is. It is your responsibility to ensure that this code
 does not infringe any patents that apply in your area before you
 ship it.
 */


/* README */
/*
 The code contained here is Copyright (C) 2008-2009 Robin Watts
 <robin@wss.co.uk>
 
 This code provides optimised ARM code (and the C equivalents) for fast
 conversion of YUV420, YUV422 and YUV444 code to RGB565.
 
 For ARM platforms, use the ARM code, as this will result in significantly
 faster results (due to no register spillage in the loops, improved
 scheduling etc).
 
 For other platforms, use the C code - this is deliberately kept as close
 as possible to the ARM code.
 
 yuv2rgb.h is the overall header. #include this.
 
 yuv2rgb16tab.c contains the definition of the standard table used by
 all the routines (and passed in as a parameter). You can use this, or
 define your own table.
 
 Future versions of this software may include routines to generate tables
 with given weights.
 
 The latest version of this software should always be available from
 <http://www.wss.co.uk/pinknoise/yuv2rgb>
 */

/// Function define for one of the conversions methods.
typedef void (*REYUVtoRGBConversionFunc)(REUByte * dst_ptr,
const REUByte * y_ptr,
const REUByte * u_ptr,
const REUByte * v_ptr,
REInt32 width,
REInt32 height,
REInt32 y_span,
REInt32 uv_span,
REInt32 dst_span,
const REUInt32 * tables,
REInt32 dither);

/// Class usign for converting YUV image data to RGB(A)
class __RE_PUBLIC_CLASS_API__ REYUVtoRGB
{
public:
	/// Returns pointer to array for converting to BGR565 table.
    static const REUInt32 * GetYUVtoBGR565Table();
	
	/// Returns pointer to array for converting to RGB565 table.
    static const REUInt32 * GetYUVtoRGB565Table();
    
	/// Converts YUV420 to RGB565.
    static void YUV420toRGB565(REUByte * dst_ptr,
                               const REUByte * y_ptr,
                               const REUByte * u_ptr,
                               const REUByte * v_ptr,
                               REInt32 width,
                               REInt32 height,
                               REInt32 y_span,
                               REInt32 uv_span,
                               REInt32 dst_span,
                               const REUInt32 * tables,
                               REInt32 dither);
    
	/// Converts YUV422 to RGB565.
    static void YUV422toRGB565(REUByte * dst_ptr,
                               const REUByte * y_ptr,
                               const REUByte * u_ptr,
                               const REUByte * v_ptr,
                               REInt32 width,
                               REInt32 height,
                               REInt32 y_span,
                               REInt32 uv_span,
                               REInt32 dst_span,
                               const REUInt32 * tables,
                               REInt32 dither);
    
	/// Converts YUV444 to RGB565.
    static void YUV444toRGB565(REUByte * dst_ptr,
                               const REUByte * y_ptr,
                               const REUByte * u_ptr,
                               const REUByte * v_ptr,
                               REInt32 width,
                               REInt32 height,
                               REInt32 y_span,
                               REInt32 uv_span,
                               REInt32 dst_span,
                               const REUInt32 * tables,
                               REInt32 dither);
    
	/// Converts YUV420 to RGB888.
    static void YUV420toRGB888(REUByte * dst_ptr,
                               const REUByte * y_ptr,
                               const REUByte * u_ptr,
                               const REUByte * v_ptr,
                               REInt32 width,
                               REInt32 height,
                               REInt32 y_span,
                               REInt32 uv_span,
                               REInt32 dst_span,
                               const REUInt32 * tables,
                               REInt32 dither);
    
	/// Converts YUV422 to RGB888.
    static void YUV422toRGB888(REUByte * dst_ptr,
                               const REUByte * y_ptr,
                               const REUByte * u_ptr,
                               const REUByte * v_ptr,
                               REInt32 width,
                               REInt32 height,
                               REInt32 y_span,
                               REInt32 uv_span,
                               REInt32 dst_span,
                               const REUInt32 * tables,
                               REInt32 dither);
    
	/// Converts YUV444 to RGB888.
    static void YUV444toRGB888(REUByte * dst_ptr,
                               const REUByte * y_ptr,
                               const REUByte * u_ptr,
                               const REUByte * v_ptr,
                               REInt32 width,
                               REInt32 height,
                               REInt32 y_span,
                               REInt32 uv_span,
                               REInt32 dst_span,
                               const REUInt32 * tables,
                               REInt32 dither);
    
	/// Converts YUV420 to RGB8888.
    static void YUV420toRGB8888(REUByte * dst_ptr,
                                const REUByte * y_ptr,
                                const REUByte * u_ptr,
                                const REUByte * v_ptr,
                                REInt32 width,
                                REInt32 height,
                                REInt32 y_span,
                                REInt32 uv_span,
                                REInt32 dst_span,
                                const REUInt32 * tables,
                                REInt32 dither);
    
	/// Converts YUV422 to RGB8888.
    static void YUV422toRGB8888(REUByte * dst_ptr,
                                const REUByte * y_ptr,
                                const REUByte * u_ptr,
                                const REUByte * v_ptr,
                                REInt32 width,
                                REInt32 height,
                                REInt32 y_span,
                                REInt32 uv_span,
                                REInt32 dst_span,
                                const REUInt32 * tables,
                                REInt32 dither);
    
	/// Converts YUV444 to RGB8888.
    static void YUV444toRGB8888(REUByte * dst_ptr,
                                const REUByte * y_ptr,
                                const REUByte * u_ptr,
                                const REUByte * v_ptr,
                                REInt32 width,
                                REInt32 height,
                                REInt32 y_span,
                                REInt32 uv_span,
                                REInt32 dst_span,
                                const REUInt32 * tables,
                                REInt32 dither);
};


#endif /* __REYUVTORGB_H__ */



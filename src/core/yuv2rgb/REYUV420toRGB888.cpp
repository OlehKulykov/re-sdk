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



#include "../../../include/recore/REYUVtoRGB.h"

#if defined(HAVE_RECORE_SDK_CONFIG_H) 
#include "recore_sdk_config.h"
#endif


#ifndef __RE_CORE_NO_YUV_TO_RGB_CONVERSION__

enum
{
    FLAGS = 0x40080100
};

#define READUV(U,V) (tables[256 + (U)] + tables[512 + (V)])
#define READY(Y)    tables[Y]
#define FIXUP(Y)                 \
{                             \
int tmp = (Y) & FLAGS;       \
if (tmp != 0)                \
{                            \
tmp  -= tmp>>8;          \
(Y)  |= tmp;             \
tmp   = FLAGS & ~(Y>>1); \
(Y)  += tmp>>8;          \
}                            \
}

#define STORE(Y,DSTPTR)           \
{                              \
REUInt32 Y2       = (Y);      \
REUByte  *DSTPTR2 = (DSTPTR); \
(DSTPTR2)[0] = (Y2);          \
(DSTPTR2)[1] = (Y2)>>22;      \
(DSTPTR2)[2] = (Y2)>>11;      \
}

#endif /* __RE_CORE_NO_YUV_TO_RGB_CONVERSION__ */


void REYUVtoRGB::YUV420toRGB888(REUByte  *dst_ptr,
                                const REUByte  *y_ptr,
                                const REUByte  *u_ptr,
                                const REUByte  *v_ptr,
                                REInt32   width,
                                REInt32   height,
                                REInt32   y_span,
                                REInt32   uv_span,
                                REInt32   dst_span,
                                const REUInt32 *tables,
                                REInt32   dither)
{
	
#ifndef __RE_CORE_NO_YUV_TO_RGB_CONVERSION__	
	
    height -= 1;
    while (height > 0)
    {
        height -= width<<16;
        height += 1<<16;
        while (height < 0)
        {
            /* Do 2 column pairs */
            REUInt32 uv, y0, y1;
            
            uv  = READUV(*u_ptr++,*v_ptr++);
            y1  = uv + READY(y_ptr[y_span]);
            y0  = uv + READY(*y_ptr++);
            FIXUP(y1);
            FIXUP(y0);
            STORE(y1, &dst_ptr[dst_span]);
            STORE(y0, dst_ptr);
            dst_ptr += 3;
            y1  = uv + READY(y_ptr[y_span]);
            y0  = uv + READY(*y_ptr++);
            FIXUP(y1);
            FIXUP(y0);
            STORE(y1, &dst_ptr[dst_span]);
            STORE(y0, dst_ptr);
            dst_ptr += 3;
            height += (2<<16);
        }
        if ((height>>16) == 0)
        {
            /* Trailing column pair */
            REUInt32 uv, y0, y1;
            
            uv = READUV(*u_ptr,*v_ptr);
            y1 = uv + READY(y_ptr[y_span]);
            y0 = uv + READY(*y_ptr++);
            FIXUP(y1);
            FIXUP(y0);
            STORE(y0, &dst_ptr[dst_span]);
            STORE(y1, dst_ptr);
            dst_ptr += 3;
        }
        dst_ptr += dst_span*2-width*3;
        y_ptr   += y_span*2-width;
        u_ptr   += uv_span-(width>>1);
        v_ptr   += uv_span-(width>>1);
        height = (height<<16)>>16;
        height -= 2;
    }
    if (height == 0)
    {
        /* Trail row */
        height -= width<<16;
        height += 1<<16;
        while (height < 0)
        {
            /* Do a row pair */
            REUInt32 uv, y0, y1;
            
            uv  = READUV(*u_ptr++,*v_ptr++);
            y1  = uv + READY(*y_ptr++);
            y0  = uv + READY(*y_ptr++);
            FIXUP(y1);
            FIXUP(y0);
            STORE(y1, dst_ptr);
            dst_ptr += 3;
            STORE(y0, dst_ptr);
            dst_ptr += 3;
            height += (2<<16);
        }
        if ((height>>16) == 0)
        {
            /* Trailing pix */
            REUInt32 uv, y0;
            
            uv = READUV(*u_ptr++,*v_ptr++);
            y0 = uv + READY(*y_ptr++);
            FIXUP(y0);
            STORE(y0, dst_ptr);
            dst_ptr += 3;
        }
    }
	
#endif /* __RE_CORE_NO_YUV_TO_RGB_CONVERSION__ */
	
}




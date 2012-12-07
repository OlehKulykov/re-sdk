

#ifndef __WEBP_WRAP1_C__
#define __WEBP_WRAP1_C__

#include "webp_config.h"

#ifdef __RE_USING_ADITIONAL_WEBP_LIBRARY__


#include "webp/src/dec/alpha.c"
#include "webp/src/dec/buffer.c"
#include "webp/src/dec/frame.c"
#include "webp/src/dec/idec.c"
#include "webp/src/dec/io.c"
#include "webp/src/dec/layer.c"
#include "webp/src/dec/quant.c"
#include "webp/src/dec/tree.c"
#include "webp/src/dec/vp8.c"
#include "webp/src/dec/vp8l.c"
#include "webp/src/dec/webp.c"
#include "webp/src/dsp/cpu.c"
#include "webp/src/dsp/dec.c"

#if defined(__ARM_NEON__)
#include "webp/src/dsp/dec_neon.c"
#endif

#include "webp/src/dsp/dec_sse2.c"
#include "webp/src/dsp/lossless.c"
#include "webp/src/dsp/upsampling.c"
#include "webp/src/dsp/upsampling_sse2.c"
#include "webp/src/enc/backward_references.c"
#include "webp/src/enc/config.c"
#include "webp/src/enc/histogram.c"
#include "webp/src/utils/bit_reader.c"
#include "webp/src/utils/bit_writer.c"
#include "webp/src/utils/color_cache.c"
#include "webp/src/utils/filters.c"
#include "webp/src/utils/huffman.c"
#include "webp/src/utils/quant_levels.c"
#include "webp/src/utils/rescaler.c"
#include "webp/src/utils/thread.c"
#include "webp/src/utils/utils.c"




#endif


#endif


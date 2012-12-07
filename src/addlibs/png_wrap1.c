

#ifndef __PNG_WRAP1_C__
#define __PNG_WRAP1_C__

#include "png_config.h"

#ifdef __RE_USING_ADITIONAL_PNG_LIBRARY__

#ifndef png_libpng_ver
#define png_libpng_ver png_get_header_ver(NULL)
#endif


#include "libpng/png.c"
#include "libpng/pngerror.c"
#include "libpng/pngget.c"
#include "libpng/pngmem.c"
#include "libpng/pngpread.c"
#include "libpng/pngread.c"
#include "libpng/pngrio.c"
#include "libpng/pngrtran.c"
#include "libpng/pngrutil.c"
#include "libpng/pngset.c"
#include "libpng/pngtrans.c"
#include "libpng/pngwio.c"
#include "libpng/pngwrite.c"
#include "libpng/pngwtran.c"
#include "libpng/pngwutil.c"

#endif 


#endif /* __PNG_WRAP1_C__ */


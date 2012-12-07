

#ifndef __ADD_LIB_FREETYPE_CONFIG_H__
#define __ADD_LIB_FREETYPE_CONFIG_H__

// NOTE: 
//      here you can define or undefine params for freetype.library
//      may also include platform specific config header file
// 

#include "../../include/reconfig/REConfigSelector.h"

#ifdef __RE_USING_ADITIONAL_FREETYPE_LIBRARY__

#ifndef __RE_OS_WINDOWS__
#warning "add search user path to project: -I/addlibs/freetype/include/"
#endif

#ifndef FT2_BUILD_LIBRARY
#define FT2_BUILD_LIBRARY
#endif


#ifndef FT_CONFIG_OPTION_SYSTEM_ZLIB
#define FT_CONFIG_OPTION_SYSTEM_ZLIB
#endif


#if (defined (__RE_OS_MACOSX__) || defined (__RE_OS_IPHONE__))

#ifndef DARWIN_NO_CARBON
#define DARWIN_NO_CARBON
#endif

#endif

#endif /* __RE_USING_ADITIONAL_FREETYPE_LIBRARY__ */


#endif


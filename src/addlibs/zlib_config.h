

#ifndef __ADD_LIB_ZLIB_CONFIG_H__
#define __ADD_LIB_ZLIB_CONFIG_H__

// NOTE: 
//      here you can define or undefine params for zlib.library
//      may also include platform specific config header file
// 

#include "../../include/reconfig/REConfigSelector.h"

#ifdef __RE_USING_ADITIONAL_ZLIB_LIBRARY__

#ifndef __RE_OS_WINDOWS__
#warning "add search user path to project: -I/addlibs/zlib/"
#endif

#endif

#endif


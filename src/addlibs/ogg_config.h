

#ifndef __ADD_LIB_OGG_CONFIG_H__
#define __ADD_LIB_OGG_CONFIG_H__

// NOTE: 
//      here you can define or undefine params for ogg.library
//      may also include platform specific config header file
// 

#include "../../include/reconfig/REConfigSelector.h"

#ifdef __RE_USING_ADITIONAL_OGG_LIBRARY__

#ifndef __RE_OS_WINDOWS__
#warning "add search user path to project: -I/addlibs/libogg/include/"
#endif

#endif /* __RE_USING_ADITIONAL_OGG_LIBRARY__ */

#endif


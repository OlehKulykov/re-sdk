

#ifndef __ADD_LIB_THEORA_CONFIG_H__
#define __ADD_LIB_THEORA_CONFIG_H__

// NOTE: 
//      here you can define or undefine params for theora.library
//      may also include platform specific config header file
// 

#include "../../include/reconfig/REConfigSelector.h"

#ifdef __RE_USING_ADITIONAL_THEORA_LIBRARY__

#define THEORA_DISABLE_ENCOD

#ifdef __RE_OS_MACOSX__

#ifndef __MACOSX__
#define __MACOSX__
#endif

#endif

#ifndef __RE_OS_WINDOWS__
#warning "add search user path to project: -I/addlibs/libtheora/include/"
#endif

#endif /* __RE_USING_ADITIONAL_THEORA_LIBRARY__ */

#endif


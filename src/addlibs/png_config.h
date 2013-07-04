

#ifndef __ADD_LIB_PNG_CONFIG_H__
#define __ADD_LIB_PNG_CONFIG_H__

// NOTE: 
//      here you can define or undefine params for png.library
//      may also include platform specific config header file
// 

#include "../../include/reconfig/REConfigSelector.h"

#ifdef __RE_USING_ADITIONAL_PNG_LIBRARY__


#ifdef __RE_OS_LINUX__
#define PNG_SKIP_SETJMP_CHECK
#endif



#endif

#endif


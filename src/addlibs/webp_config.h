

#ifndef __ADD_LIB_WEBP_CONFIG_H__
#define __ADD_LIB_WEBP_CONFIG_H__

// NOTE: 
//      here you can define or undefine params for webp.library
//      may also include platform specific config header file
// 

#include "../../include/reconfig/REConfigSelector.h"

#ifdef __RE_USING_ADITIONAL_WEBP_LIBRARY__

#ifndef WEBP_EXPERIMENTAL_FEATURES
#define WEBP_EXPERIMENTAL_FEATURES
#endif 

#ifdef WEBP_USE_THREAD
#undef WEBP_USE_THREAD
#endif

#endif 



#endif


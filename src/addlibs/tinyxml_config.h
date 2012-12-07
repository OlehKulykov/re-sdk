

#ifndef __ADD_LIB_TINYXML_CONFIG_H__
#define __ADD_LIB_TINYXML_CONFIG_H__

// NOTE: 
//      here you can define or undefine params for tinyxml.library
//      may also include platform specific config header file
// 

#include "../../include/reconfig/REConfigSelector.h"

#ifdef __RE_USING_ADITIONAL_TINYXML_LIBRARY__

#ifdef TIXML_USE_STL
#undef TIXML_USE_STL
#endif

#endif


#endif


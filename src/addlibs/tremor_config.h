

#ifndef __ADD_LIB_TREMOR_CONFIG_H__
#define __ADD_LIB_TREMOR_CONFIG_H__

// NOTE: 
//      here you can define or undefine params for vorbis.library
//      may also include platform specific config header file
// 

#include "../../include/reconfig/REConfigSelector.h"

#ifdef __RE_USING_ADITIONAL_TREMOR_LIBRARY__

#ifdef __RE_OS_IPHONE__
//#define _ARM_ASSEM_
//#define __NO_MATH_INLINES
//#define _LOW_ACCURACY_
#endif

#endif

#endif


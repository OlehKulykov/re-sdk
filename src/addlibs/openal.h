

#ifndef __ADD_LIB_OPENAL_H__
#define __ADD_LIB_OPENAL_H__



#include "openal_config.h"

#if ( defined(__RE_OS_ANDROID__) )

#include "openal_soft/include/AL/al.h"
#include "openal_soft/include/AL/alc.h"
#include "openal_soft/include/AL/alext.h"


#endif




#if ( defined(__RE_OS_MACOSX__) || defined(__RE_OS_IPHONE__) )

#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#ifdef __RE_OS_MACOSX__
#include <OpenAL/MacOSX_OALExtensions.h>
#endif

#endif /* __RE_OS_MACOSX__ OR __RE_OS_IPHONE__ */


#endif



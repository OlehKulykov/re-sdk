

#ifndef __ADD_LIB_OPENJPEG_CONFIG_H__
#define __ADD_LIB_OPENJPEG_CONFIG_H__

// NOTE:
//      here you can define or undefine params for openjpeg.library
//      may also include platform specific config header file
// 

#include "../../include/reconfig/REConfigSelector.h"

#ifdef __RE_USING_ADITIONAL_OPENJPEG_LIBRARY__


#ifndef OPJ_STATIC
#define OPJ_STATIC
#endif


#ifndef PACKAGE_VERSION
#define PACKAGE_VERSION "1.5.0"
#endif

#ifndef HAVE_INTTYPES_H
#define HAVE_INTTYPES_H 1
#endif

#ifndef __RE_OS_BADA__
#ifndef HAVE_MEMORY_H
#define HAVE_MEMORY_H 1
#endif
#endif

#ifndef HAVE_STDINT_H
#define HAVE_STDINT_H 1
#endif

#ifndef HAVE_STDLIB_H
#define HAVE_STDLIB_H 1
#endif

#ifndef HAVE_STRINGS_H
#define HAVE_STRINGS_H 1
#endif

#ifndef HAVE_STRING_H
#define HAVE_STRING_H 1
#endif

#ifndef HAVE_SYS_STAT_H
#define HAVE_SYS_STAT_H 1
#endif

#ifndef HAVE_SYS_TYPES_H
#define HAVE_SYS_TYPES_H 1
#endif

#ifndef __RE_OS_WINDOWS__
#ifndef HAVE_UNISTD_H
#define HAVE_UNISTD_H 1
#endif
#endif

#endif 


#endif /* __ADD_LIB_OPENJPEG_CONFIG_H__ */


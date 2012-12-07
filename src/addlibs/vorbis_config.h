

#ifndef __ADD_LIB_VORBIS_CONFIG_H__
#define __ADD_LIB_VORBIS_CONFIG_H__

// NOTE: 
//      here you can define or undefine params for vorbis.library
//      may also include platform specific config header file
// 

#include "../../include/reconfig/REConfigSelector.h"

#ifdef __RE_USING_ADITIONAL_VORBIS_LIBRARY__

#ifdef __RE_OS_MACOSX__

#ifndef __MACOSX__
#define __MACOSX__
#endif

#endif


#ifdef __RE_OS_IPHONE__
/* Define to 1 if you have <alloca.h> and it should be used (not on Ultrix).
 */
#define HAVE_ALLOCA_H 1

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1
#endif




#ifndef __RE_OS_WINDOWS__
#warning "add search user path to project: -I/addlibs/libvorbis/include/"
#endif

#endif /* __RE_USING_ADITIONAL_VORBIS_LIBRARY__ */

#endif


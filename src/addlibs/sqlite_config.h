

#ifndef __ADD_LIB_SQLITE_CONFIG_H__
#define __ADD_LIB_SQLITE_CONFIG_H__

// NOTE: 
//      here you can define or undefine params for sqlite.library
//      may also include platform specific config header file
// 

#include "../../include/reconfig/REConfigSelector.h"

#ifdef __RE_USING_ADITIONAL_SQLITE_LIBRARY__

#ifdef __RE_OS_MACOSX__


#endif /* __RE_OS_MACOSX__ */

#ifdef __RE_OS_BADA__

#define SQLITE_THREADSAFE 0
#define SQLITE_ENABLE_COLUMN_METADATA
#define SQLITE_OMIT_LOAD_EXTENSION
#define SQLITE_OMIT_COMPILEOPTION_DIAGS
#define SQLITE_OMIT_DEPRECATED
#define SQLITE_OMIT_TRACE
#define SQLITE_OMIT_EXPLAIN
#define SQLITE_OMIT_AUTHORIZATION
#define SQLITE_OMIT_PROGRESS_CALLBACK
#define SQLITE_OMIT_SHARED_CACHE

#define _POSIX_THREADS
#define _UNIX98_THREAD_MUTEX_ATTRIBUTES
#define SQLITE_OS_UNIX 1

#define SQLITE_OMIT_WAL								// no '<sys/mmem.h>' header in Bada OS
#define SQLITE_OMIT_UTF16
#define SQLITE_OMIT_LOAD_EXTENSION
#define SQLITE_MUTEX_OMIT

#define SQLITE_ENABLE_COLUMN_METADATA
#define SQLITE_ENABLE_FTS3
#define SQLITE_ENABLE_FTS3_PARENTHESIS
#define SQLITE_ENABLE_RTREE
#define SQLITE_SOUNDEX

#include <sys/_default_fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#endif /* __RE_OS_BADA__ */


#endif /* __RE_USING_ADITIONAL_SQLITE_LIBRARY__ */

#endif /* __ADD_LIB_SQLITE_CONFIG_H__ */


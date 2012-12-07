

#ifndef __LZMA_WRAP1_C__
#define __LZMA_WRAP1_C__

#include "lzma_config.h"

#ifdef __RE_USING_ADITIONAL_LZMA_LIBRARY__

#ifndef __RE_OS_WINDOWS__

typedef void* HANDLE;
typedef void* LPVOID;
typedef void* CRITICAL_SECTION;
typedef int BOOL;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef long LONG;

#define TRUE (BOOL)1

#define FALSE (BOOL)0

#define _7ZIP_ST

#define INFINITE 1
#endif /* NOT __RE_OS_WINDOWS__ */



#ifdef __RE_OS_IPHONE__

#ifndef __LITTLE_ENDIAN__
#define __LITTLE_ENDIAN__
#define __REMOVE___LITTLE_ENDIAN__
#endif /* __LITTLE_ENDIAN__ */

#ifndef MY_CPU_X86
#define MY_CPU_X86
#define __REMOVE_MY_CPU_X86
#endif /* MY_CPU_X86 */

#ifndef _M_ARM
#define _M_ARM
#define __REMOVE__M_ARM
#endif /* _M_ARM */


#endif /* __RE_OS_IPHONE__ */



#include "lzma/C/Alloc.c"
#include "lzma/C/LzFind.c"
#include "lzma/C/LzmaDec.c"
#include "lzma/C/LzmaEnc.c"
#include "lzma/C/LzmaLib.c"
//#include "lzma/C/LzFindMt.c"
//#include "lzma/C/Threads.c"


#ifdef __RE_OS_WINDOWS__
#include "lzma/C/7zCrc.c"
#include "lzma/C/7zCrcOpt.c"
#include "lzma/C/Bra86.c"
#include "lzma/C/LzFindMt.c"
#include "lzma/C/Threads.c"
#include "lzma/C/CpuArch.c"
#endif



#ifdef __REMOVE___LITTLE_ENDIAN__
#undef __LITTLE_ENDIAN__
#undef __REMOVE___LITTLE_ENDIAN__
#endif

#ifdef __REMOVE_MY_CPU_X86
#undef MY_CPU_X86
#undef __REMOVE_MY_CPU_X86
#endif

#ifdef __REMOVE__M_ARM
#undef _M_ARM
#undef __REMOVE__M_ARM
#endif

#endif /* __RE_USING_ADITIONAL_LZMA_LIBRARY__ */

#endif /* __LZMA_WRAP1_C__ */




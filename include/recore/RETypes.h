/*
 *   Copyright 2012 Kulykov Oleh
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */


#ifndef __RETYPES_H__
#define __RETYPES_H__

#include "RECoreOSSelector.h"
#include "RECoreSettings.h"
#include "RECorePlatformSelector.h"
#include "RECoreIncludes.h"


#include <stdint.h>
typedef uint8_t REUByte;
typedef uint16_t REUInt16;
typedef uint32_t REUInt32;
typedef uint64_t REUInt64;
typedef int8_t REByte;
typedef int16_t REInt16;
typedef int32_t REInt32;
typedef int64_t REInt64;


#ifdef __RE_OS_IPHONE__
#endif /* __RE_OS_IPHONE__ */


#ifdef __RE_OS_MACOSX__
#endif /* __RE_OS_MACOSX__ */


#ifdef __RE_OS_WINDOWS__
#endif /* __RE_OS_WINDOWS__ */


#ifdef __RE_OS_ANDROID__
#endif /* __RE_OS_ANDROID__ */

#ifdef __RE_OS_LINUX__
#endif /* __RE_OS_LINUX__ */


#ifdef __RE_OS_BADA__
#endif /* __RE_OS_BADA__ */


#if defined(_UINTPTR_T) 
typedef uintptr_t REUIdentifier;
#elif defined(_UINTPTR_T_DEFINED) 
typedef uintptr_t REUIdentifier;
#else
#if defined(__RE_64BIT_PLATFORM__) 
typedef REUInt64 REUIdentifier;
#elif defined(__RE_32BIT_PLATFORM__) 
typedef REUInt32 REUIdentifier;
#endif
#endif


typedef float REFloat32;
typedef double REFloat64;


typedef REFloat64 RETimeInterval;

typedef bool REBOOL;

#ifndef NULL
#define NULL 0
#endif 

#ifndef UINT32_MAX
#define UINT32_MAX (4294967295u)
#endif

#ifndef REUInt32Max
#define REUInt32Max UINT32_MAX
#endif

#ifndef USHRT_MAX
#define USHRT_MAX (65535u)
#endif

#ifndef REUInt16Max
#define REUInt16Max USHRT_MAX
#endif

#ifndef INT_MAX
#define INT_MAX (2147483647)
#endif

#ifndef INT_MIN
#define INT_MIN (−2147483648)
#endif


#define RENotFound REUInt32Max

#define REIndexNotFound REUInt32Max


#endif //__RETYPES_H__




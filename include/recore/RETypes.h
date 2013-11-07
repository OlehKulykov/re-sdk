/*
 *   Copyright 2012 - 2013 Kulykov Oleh
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


typedef uint8_t REUByte; 
typedef uint16_t REUInt16; 
typedef uint32_t REUInt32; 
typedef uint64_t REUInt64; 
typedef int8_t REByte; 
typedef int16_t REInt16; 
typedef int32_t REInt32; 
typedef int64_t REInt64; 
typedef uintptr_t REUIdentifier; 
typedef bool REBOOL; 

#if defined(SIZEOF_FLOAT_T)
typedef float_t REFloat32; 
#else
typedef float REFloat32; 
#endif

#if defined(SIZEOF_DOUBLE_T)
typedef double_t REFloat64; 
#else
typedef double REFloat64; 
#endif

typedef REFloat64 RETimeInterval; 



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




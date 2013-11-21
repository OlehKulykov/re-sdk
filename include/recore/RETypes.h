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


/**
 @brief 8 bit unsigned byte type.
 @detailed 8 bit unsigned byte with 1 byte size.
 */
typedef uint8_t REUByte; 


/**
 @brief 16 bit unsigned integer type.
 @detailed 16 bit unsigned integer with 2 byte size.
 */
typedef uint16_t REUInt16; 


/**
 @brief 32 bit unsigned integer type.
 @detailed 32 bit unsigned integer with 4 byte size.
 */
typedef uint32_t REUInt32; 


/**
 @brief 64 bit unsigned integer type.
 @detailed 64 bit unsigned integer with 8 byte size.
 */
typedef uint64_t REUInt64; 


/**
 @brief 8 bit signed byte type.
 @detailed 8 bit signed byte with 1 byte size.
 */
typedef int8_t REByte; 


/**
 @brief 16 bit signed integer type.
 @detailed 16 bit signed integer with 2 byte size.
 */
typedef int16_t REInt16; 


/**
 @brief 32 bit signed integer type.
 @detailed 32 bit signed integer with 4 byte size.
 */
typedef int32_t REInt32; 


/**
 @brief 64 bit signed integer type.
 @detailed 64 bit signed integer with 8 byte size.
 */
typedef int64_t REInt64; 


/**
 @brief unsigned identifier integer type.
 @detailed unsigned identifier integer type size is same as pointer.
 */
typedef uintptr_t REUIdentifier; 


/**
 @brief Boolean type.
 @detailed Boolean type with possible values: 'true' or 'false'
 */
typedef bool REBOOL; 



#if defined(SIZEOF_FLOAT_T)
/**
 @brief 32 bit float type.
 @detailed 32 bit float type with 4 byte size.
 */
typedef float_t REFloat32; 
#else
/**
 @brief 32 bit float type.
 @detailed 32 bit float type with 4 byte size.
 */
typedef float REFloat32; 
#endif


#if defined(SIZEOF_DOUBLE_T)
/**
 @brief 64 bit float type.
 @detailed 64 bit float type with 8 byte size.
 */
typedef double_t REFloat64; 
#else
/**
 @brief 64 bit float type.
 @detailed 64 bit float type with 8 byte size.
 */
typedef double REFloat64; 
#endif


/**
 @brief Time interval type.
 @detailed Time interval type with 8 byte size. Used for holding seconds with fractional part
 */
typedef REFloat64 RETimeInterval; 



/**
 @brief Define for NULL pointer.
 */
#ifndef NULL
#define NULL 0
#endif 


/**
 @brief Define for 32 bit unsigned integer maximum value.
 */
#ifndef UINT32_MAX
#define UINT32_MAX (4294967295u)
#endif


/**
 @brief Define for 32 bit unsigned integer maximum value.
 */
#ifndef REUInt32Max
#define REUInt32Max UINT32_MAX
#endif


/**
 @brief Define for 16 bit unsigned integer maximum value.
 */
#ifndef USHRT_MAX
#define USHRT_MAX (65535u)
#endif


/**
 @brief Define for 16 bit unsigned integer maximum value.
 */
#ifndef REUInt16Max
#define REUInt16Max USHRT_MAX
#endif


/**
 @brief Define for default signed integer(int) maximum value.
 */
#ifndef INT_MAX
#define INT_MAX (2147483647)
#endif


/**
 @brief Define for default signed integer(int) minimum value.
 */
#ifndef INT_MIN
#define INT_MIN (−2147483648)
#endif


/**
 @brief Define for indicating that something can't be found or undefined.
 */
#define RENotFound REUInt32Max


/**
 @brief Define for indicating that index can't be found.
 */
#define REIndexNotFound REUInt32Max


#endif //__RETYPES_H__




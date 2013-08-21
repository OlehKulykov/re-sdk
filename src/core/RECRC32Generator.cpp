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


#include "../../include/recore/RECRC32Generator.h"
#include "../../include/recore/private/REStringUtilsPrivate.h"



#if !defined(__RE_RECORE_NO_ZLIB_COMPRESSION_SUPPORT__)

#if defined(HAVE_ZLIB_H)
#include <zlib.h>
#define USE_CRC32_FROM_ZLIB_LIBRARY 1 
#else
#include "../addlibs/zlib.h"
#define USE_CRC32_FROM_ZLIB_LIBRARY 1 
#endif

#endif


#ifndef USE_CRC32_FROM_ZLIB_LIBRARY

class RECRC32GeneratorPrivate 
{	
public:
	static REUInt32 crc32HalfByte(const void* data, REUInt32 length, REUInt32 previousCrc32 = 0);
};

/// compute CRC32 (half-byte algoritm)
REUInt32 RECRC32GeneratorPrivate::crc32HalfByte(const void* data, REUInt32 length, REUInt32 previousCrc32)
{
	REUInt32 crc = ~previousCrc32; // same as previousCrc32 ^ 0xFFFFFFFF
	const REUByte* current = (const REUByte*) data;

	static const REUInt32 crc32Lookup16[16] =
	{
		0x00000000,0x1DB71064,0x3B6E20C8,0x26D930AC,0x76DC4190,0x6B6B51F4,0x4DB26158,0x5005713C,
		0xEDB88320,0xF00F9344,0xD6D6A3E8,0xCB61B38C,0x9B64C2B0,0x86D3D2D4,0xA00AE278,0xBDBDF21C
	};
	
	while (length-- > 0)
	{
		crc = crc32Lookup16[(crc ^  *current      ) & 0x0F] ^ (crc >> 4);
		crc = crc32Lookup16[(crc ^ (*current >> 4)) & 0x0F] ^ (crc >> 4);
		current++;
	}
	
	return (~crc); // same as crc ^ 0xFFFFFFFF
}

#endif

const REUInt32 generateFromData(const void * data, const REUInt32 dataLength)
{
	if (data && dataLength) 
	{
#if defined(USE_CRC32_FROM_ZLIB_LIBRARY)		
		return (REUInt32)crc32(0, (const Bytef*)data, dataLength);
#else
		return RECRC32GeneratorPrivate::crc32HalfByte(data, dataLength);
#endif			
	}
	return 0;
}

const REUInt32 RECRC32Generator::generateFromString(const char * someString, 
													const REUInt32 stringLenght)
{
	const REUInt32 len = REStringUtilsPrivate::actualUTF8StringLength(someString, stringLenght);
	if (len > 0) 
	{
#if defined(USE_CRC32_FROM_ZLIB_LIBRARY)		
		return (REUInt32)crc32(0, (const Bytef*)someString, len);
#else
		return RECRC32GeneratorPrivate::crc32HalfByte(someString, len);
#endif		
	}	
	return 0;
}



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


#include "../../include/recore/RELZMACompression.h"

#if defined(HAVE_RECORE_SDK_CONFIG_H) 
#include "recore_sdk_config.h"
#endif

#if !defined(__RE_RECORE_NO_LZMA_COMPRESSION_SUPPORT__)

#if defined(HAVE_LZMA_LZMA_H) || defined(HAVE_LZMA_H) || defined(HAVE_LZMALIB_H)

#if defined(HAVE_LZMA_LZMA_H)
#include <lzma/lzma.h>
#ifndef IS_RE_USING_LZMA
#define IS_RE_USING_LZMA 1
#endif
#endif

#if defined(HAVE_LZMA_H)
#include <lzma.h>
#ifndef IS_RE_USING_LZMA
#define IS_RE_USING_LZMA 1
#endif
#endif

#if defined(HAVE_LZMALIB_H)
#include <LzmaLib.h>
#ifndef IS_RE_USING_LZMA
#define IS_RE_USING_LZMA 1
#endif
#endif

#else 

#define __RE_RECORE_NO_LZMA_COMPRESSION_SUPPORT__ 1

#endif /* defined(HAVE_LZMA_LZMA_H) || defined(HAVE_LZMA_H) || defined(HAVE_LZMALIB_H) */

#endif /* !defined(__RE_RECORE_NO_LZMA_COMPRESSION_SUPPORT__) */



/// Compress data from buffer 'inBuffer' to buffer 'compressedBuffer' with 'compressionLevel' in range [0.0f, 1.0f].
RETypedPtr RELZMACompression::compress(const REBuffer & inBuffer, const REFloat32 compressionLevel)
{
	if (inBuffer.size() == 0)
	{
		return RETypedPtr();
	}

#if defined(IS_RE_USING_LZMA)
	RETypedPtr ptr(new REBuffer, REPtrTypeBuffer);
	REBuffer * compressedBuffer = ptr.buffer();
	if (compressedBuffer == NULL) 
	{
		return RETypedPtr();
	}
	
	REUInt32 outSize = (REUInt32)(((size_t)inBuffer.size() / 20) * 21) + (1 << 16);
	if (outSize == 0)
    {
		return RETypedPtr();
	}
	size_t destLen = outSize;
	outSize += (LZMA_PROPS_SIZE + sizeof(REUInt32));
	if (!compressedBuffer->resize(outSize, false))
	{
		return RETypedPtr();
	}
	
	int level = 0;
	if (compressionLevel < 0.0f) 
	{
		level = 0;
	}
	else if (compressionLevel > 1.0f) 
	{
		level = 9;
	}
	else
	{
		level = (int)(compressionLevel * 9.0f);
	}
	
	
	unsigned char * sizePtr = (unsigned char *)compressedBuffer->buffer();
	unsigned char * props = sizePtr + sizeof(REUInt32);
	unsigned char * dest = props + LZMA_PROPS_SIZE;
	size_t outPropsSize = LZMA_PROPS_SIZE;
	const int comprResult = LzmaCompress(dest, 
										 &destLen, 
										 (const unsigned char *)inBuffer.buffer(), 
										 (size_t)inBuffer.size(), 
										 props, 
										 &outPropsSize,
										 level, // compr
										 1 << 24,
										 3,
										 0,
										 2,
										 32,
										 1);
	if (comprResult == SZ_OK)
    {
		REUInt32 * int32Ptr = (REUInt32 *)sizePtr;
		*int32Ptr = (REUInt32)inBuffer.size();
		if (compressedBuffer->resize((REUInt32)destLen + LZMA_PROPS_SIZE + sizeof(REUInt32), true))
		{
			return ptr;
		}
	}
#endif /* IS_RE_USING_LZMA */
	
	return RETypedPtr();
}

/// Decompress data from buffer 'inBuffer' to buffer 'compressedBuffer'.
RETypedPtr RELZMACompression::decompress(const REBuffer & inBuffer)
{
	if (inBuffer.size() == 0)
	{
		return RETypedPtr();
	}
	
#if defined(IS_RE_USING_LZMA)
	RETypedPtr ptr(new REBuffer(), REPtrTypeBuffer);
	REBuffer * unCompressedBuffer = ptr.buffer();
	if (unCompressedBuffer == NULL) 
	{
		return RETypedPtr();
	}
	if (inBuffer.size() <= (sizeof(REUInt32) + LZMA_PROPS_SIZE)) 
	{
		return RETypedPtr();
	}
	
	unsigned char * sizePtr = (unsigned char *)inBuffer.buffer();
	unsigned char * props = sizePtr + sizeof(REUInt32);
	unsigned char * inBuff = props + LZMA_PROPS_SIZE;

	REUInt32 * int32Ptr = (REUInt32 *)sizePtr;
	if (!unCompressedBuffer->resize((*int32Ptr), false))
	{
		return RETypedPtr();
	}
	
 	size_t dstLen = unCompressedBuffer->size();
	size_t srcLen = inBuffer.size() - LZMA_PROPS_SIZE - sizeof(REUInt32);
	int res = LzmaUncompress((unsigned char *)unCompressedBuffer->buffer(), 
							 &dstLen, 
							 inBuff, 
							 &srcLen, 
							 props, 
							 LZMA_PROPS_SIZE);
	if (res == SZ_OK) 
	{
		if ((REUInt32)dstLen == unCompressedBuffer->size()) 
		{
			return ptr;
		}
	}
#endif /* IS_RE_USING_LZMA */

	return RETypedPtr();
}




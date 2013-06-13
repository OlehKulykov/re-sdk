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

#ifndef __RE_RECORE_NO_LZMA_COMPRESSION_SUPPORT__

#if defined(__RE_USING_ADITIONAL_LZMA_LIBRARY__)
#include "../addlibs/lzma.h"
#elif defined(__RE_USING_SYSTEM_LZMA_LIBRARY__)
#include <LzmaLib.h>
#else
#define __RE_RECORE_NO_LZMA_COMPRESSION_SUPPORT__
#endif

#endif


/// Compress data from buffer 'inBuffer' to buffer 'compressedBuffer' with 'compressionLevel' in range [0.0f, 1.0f].
REBOOL RELZMACompression::Compress(const REBuffer & inBuffer, REBuffer * compressedBuffer, const REFloat32 compressionLevel)
{
	if (compressedBuffer == NULL) 
	{
		return false;
	}
#ifndef __RE_RECORE_NO_LZMA_COMPRESSION_SUPPORT__
	compressedBuffer->Clear();
	
	REUInt32 outSize = (((size_t)inBuffer.GetSize() / 20) * 21) + (1 << 16);
	if (outSize == 0)
    {
		return false;
	}
	size_t destLen = outSize;
	outSize += (LZMA_PROPS_SIZE + sizeof(REUInt32));
	if (!compressedBuffer->Resize(outSize, false))
	{
		compressedBuffer->Clear();
		return false;
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
	
	
	unsigned char * sizePtr = (unsigned char *)compressedBuffer->GetBuffer();
	unsigned char * props = sizePtr + sizeof(REUInt32);
	unsigned char * dest = props + LZMA_PROPS_SIZE;
	size_t outPropsSize = LZMA_PROPS_SIZE;
	const int comprResult = LzmaCompress(dest, &destLen, 
										 (const unsigned char *)inBuffer.GetBuffer(), (size_t)inBuffer.GetSize(), 
										 props, &outPropsSize,
										 level, // compr
										 1 << 24,
										 3,
										 0,
										 2,
										 32,
										 1);
	if (comprResult == SZ_OK )
    {
		REUInt32 * int32Ptr = (REUInt32 *)sizePtr;
		*int32Ptr = (REUInt32)inBuffer.GetSize();
		compressedBuffer->Resize((REUInt32)destLen + LZMA_PROPS_SIZE + sizeof(REUInt32), true);
		return true;
	}
#endif	
	
	compressedBuffer->Clear();
	return false;
}

/// Decompress data from buffer 'inBuffer' to buffer 'compressedBuffer'.
REBOOL RELZMACompression::Decompress(const REBuffer & inBuffer, REBuffer * unCompressedBuffer)
{
	if (unCompressedBuffer == NULL) 
	{
		return false;
	}
#ifndef __RE_RECORE_NO_LZMA_COMPRESSION_SUPPORT__
	if (inBuffer.GetSize() <= (sizeof(REUInt32) + LZMA_PROPS_SIZE)) 
	{
		unCompressedBuffer->Clear();
		return false;
	}
	
	unsigned char * sizePtr = (unsigned char *)inBuffer.GetBuffer();
	unsigned char * props = sizePtr + sizeof(REUInt32);
	unsigned char * inBuff = props + LZMA_PROPS_SIZE;

	REUInt32 * int32Ptr = (REUInt32 *)sizePtr;
	if (!unCompressedBuffer->Resize((*int32Ptr), false))
	{
		return false;
	}
	
 	size_t dstLen = unCompressedBuffer->GetSize();
	size_t srcLen = inBuffer.GetSize() - LZMA_PROPS_SIZE - sizeof(REUInt32);
	int res = LzmaUncompress((unsigned char *)unCompressedBuffer->GetBuffer(), &dstLen, 
							 inBuff, &srcLen, 
							 props, LZMA_PROPS_SIZE);
	if (res == SZ_OK) 
	{
		if ((REUInt32)dstLen == unCompressedBuffer->GetSize()) 
		{
			return true;
		}
	}
#endif	
	
	unCompressedBuffer->Clear();
	return false;
}




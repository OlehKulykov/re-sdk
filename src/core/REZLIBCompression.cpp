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


#include "../../include/recore/REZLIBCompression.h"
#include "../../include/recore/REMem.h"

#if defined(HAVE_RECORE_SDK_CONFIG_H) 
#include "recore_sdk_config.h"
#endif

#if !defined(__RE_RECORE_NO_ZLIB_COMPRESSION_SUPPORT__)

#if defined(HAVE_ZLIB_H)
#include <zlib.h>
#ifndef IS_RE_USING_ZLIB
#define IS_RE_USING_ZLIB 1
#endif
#else
#include "../addlibs/zlib.h"
#ifndef IS_RE_USING_ZLIB
#define IS_RE_USING_ZLIB 1
#endif
#endif

#endif /* !defined(__RE_RECORE_NO_ZLIB_COMPRESSION_SUPPORT__) */


RETypedPtr REZLIBCompression::compress(const REBuffer & inBuffer, const REFloat32 compressionLevel)
{
	if (inBuffer.size() == 0)
	{
		return RETypedPtr();
	}
	
#if defined(IS_RE_USING_ZLIB)
	RETypedPtr ptr(new REBuffer(), REPtrTypeBuffer);
	REBuffer * compressedBuffer = ptr.buffer();
	if (compressedBuffer == NULL) 
	{
		return RETypedPtr();
	}
	
	z_stream strm;
	memset(&strm, 0, sizeof(z_stream));
	strm.next_in = (Bytef *)inBuffer.buffer();
	strm.avail_in = inBuffer.size();
	
	const uInt BUFSIZE = 32 * 1024;
	Bytef tempBuffer[BUFSIZE];
	strm.next_out = tempBuffer;
	strm.avail_out = BUFSIZE;
	
	if (compressionLevel < 0.0f) 
	{
		deflateInit(&strm, Z_NO_COMPRESSION);
	}
	else if (compressionLevel > 1.0f) 
	{
		deflateInit(&strm, Z_BEST_COMPRESSION);
	}
	else
	{
		deflateInit(&strm, (int)(compressionLevel * 9.0f));
	}
	
	while (strm.avail_in != 0)
	{
		if (deflate(&strm, Z_NO_FLUSH) != Z_OK) 
		{
			deflateEnd(&strm);
			return RETypedPtr();
		}
		if (strm.avail_out == 0)
		{
			if (!compressedBuffer->append(tempBuffer, BUFSIZE))
			{
				deflateEnd(&strm);
				return RETypedPtr();
			}
			strm.next_out = tempBuffer;
			strm.avail_out = BUFSIZE;
		}
	}
	
	int deflate_res = Z_OK;
	while (deflate_res == Z_OK)
	{
		if (strm.avail_out == 0)
		{
			if (!compressedBuffer->append(tempBuffer, BUFSIZE))
			{
				deflateEnd(&strm);
				return RETypedPtr();
			}
			strm.next_out = tempBuffer;
			strm.avail_out = BUFSIZE;
		}
		deflate_res = deflate(&strm, Z_FINISH);
	}
	
	if (deflate_res != Z_STREAM_END)
	{
		deflateEnd(&strm);
		return RETypedPtr();
	}
	
	if (!compressedBuffer->append(tempBuffer, BUFSIZE - strm.avail_out))
	{
		deflateEnd(&strm);
		return RETypedPtr();
	}
		
	deflateEnd(&strm);
	return ptr;
	
#else
	
	return RETypedPtr();
	
#endif /* IS_RE_USING_ZLIB */
}

RETypedPtr REZLIBCompression::decompress(const REBuffer & inBuffer)
{
	if (inBuffer.size() == 0)
	{
		return RETypedPtr();
	}
	
#if defined(IS_RE_USING_ZLIB)
	RETypedPtr ptr(new REBuffer(), REPtrTypeBuffer);
	REBuffer * unCompressedBuffer = ptr.buffer();
	if (unCompressedBuffer == NULL) 
	{
		return RETypedPtr();
	}
	
    z_stream strm;
	memset(&strm, 0, sizeof(z_stream));
    strm.avail_in = inBuffer.size();
	strm.next_in = (Bytef*)inBuffer.buffer();
    int ret = inflateInit(&strm);
    if (ret != Z_OK)
	{
        return RETypedPtr();
	}
	
	const uInt CHUNK = 32 * 1024;
	Bytef out[CHUNK];
	do 
	{			
		strm.avail_out = CHUNK;
		strm.next_out = out;
		
		ret = inflate(&strm, Z_NO_FLUSH);
		if (ret == Z_STREAM_ERROR) 
		{
			inflateEnd(&strm);
			return RETypedPtr();
		}
		switch (ret) 
		{
			case Z_NEED_DICT:
				ret = Z_DATA_ERROR;
			case Z_DATA_ERROR:
			case Z_MEM_ERROR:
				inflateEnd(&strm);
				return RETypedPtr();
		}
		const int have = CHUNK - strm.avail_out;
		if (have > 0) 
		{
			if (!unCompressedBuffer->append(out, (REUInt32)have))
			{
				inflateEnd(&strm);
				return RETypedPtr();
			}
		}
		else
		{
			inflateEnd(&strm);
			return RETypedPtr();
		}
	} while (strm.avail_out == 0);
	
    inflateEnd(&strm);
	if (ret == Z_STREAM_END) 
	{
		return ptr;
	}
	
#endif /* IS_RE_USING_ZLIB */
	
	return RETypedPtr();
}




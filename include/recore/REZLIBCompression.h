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


#ifndef __REZLIBCOMPRESSION_H__
#define __REZLIBCOMPRESSION_H__

#include "RECommonHeader.h"
#include "REBuffer.h"

/*
 
 __RE_RECORE_NO_ZLIB_COMPRESSION_SUPPORT__
 
 */

/// Class using for data compression/decompression using zlib library.
/// Compression and decompression avaiable with undefined flag __RE_RECORE_NO_ZLIB_COMPRESSION_SUPPORT__
/// and requare using zlib library
class __RE_PUBLIC_CLASS_API__ REZLIBCompression
{
public:
	/// Compress data from buffer 'inBuffer' to buffer 'compressedBuffer' with 'compressionLevel' in range [0.0f, 1.0f].
	static REBOOL Compress(const REBuffer & inBuffer, REBuffer * compressedBuffer, const REFloat32 compressionLevel);
	
	/// Decompress data from buffer 'inBuffer' to buffer 'compressedBuffer'.
	static REBOOL Decompress(const REBuffer & inBuffer, REBuffer * unCompressedBuffer);
};


#endif /* __REZLIBCOMPRESSION_H__ */



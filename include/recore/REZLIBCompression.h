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


#ifndef __REZLIBCOMPRESSION_H__
#define __REZLIBCOMPRESSION_H__

#include "RECommonHeader.h"
#include "REBuffer.h"
#include "RETypedPtr.h"

/*
 
 __RE_RECORE_NO_ZLIB_COMPRESSION_SUPPORT__
 
 */

/// Class using for data compression/decompression using zlib library.
/// Compression and decompression avaiable with undefined flag __RE_RECORE_NO_ZLIB_COMPRESSION_SUPPORT__
/// and requare using zlib library
class __RE_PUBLIC_CLASS_API__ REZLIBCompression
{
public:
	/// Compress data buffer 'inBuffer'with 'compressionLevel' in range [0.0f, 1.0f].
	/// Return result containes pointer with REBuffer or with NULL.
	static RETypedPtr compress(const REBuffer & inBuffer, const REFloat32 compressionLevel);
	
	/// Decompress data from buffer 'inBuffer'.
	/// Return result containes pointer with REBuffer or with NULL.
	static RETypedPtr decompress(const REBuffer & inBuffer);
};


#endif /* __REZLIBCOMPRESSION_H__ */



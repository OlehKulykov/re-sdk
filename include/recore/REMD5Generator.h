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


#ifndef __REMD5GENERATOR_H__
#define __REMD5GENERATOR_H__

#include "RECommonHeader.h"
#include "REString.h"

/*
 ===============================================================================
 
 Calculates a checksum for a block of data
 using the MD5 message-digest algorithm.
 
 ===============================================================================
 */

/*
 Taken from Doom 3 source code
 */

#ifndef __RE_OS_IPHONE__
typedef struct _reMD5Context
{
	REUInt32 state[4];
	REUInt32 bits[2];
	REUByte inBuff[64];
} REMD5Context;
#endif

/// Class generator of MD5 values.
class __RE_PUBLIC_CLASS_API__ REMD5Generator 
{
private:
#ifndef __RE_OS_IPHONE__	
	int _isBigEndianSystem;
	
	void final(REMD5Context * ctx, REUByte digest[16]);
	void update(REMD5Context * ctx, REUByte const * buf, REUInt32 len);
	void init(REMD5Context * ctx);
	void transform(REUInt32 state[4], REUInt32 inBuff[16]);
	void revBytesSwap(void * bp, const REUInt32 elsize, REUInt32 elcount);
	REUInt32 longSwap(const REUInt32 l)
	{
		const REUByte b1 = (REUByte)(l & 255);
		const REUByte b2 = (REUByte)((l >> 8) & 255);
		const REUByte b3 = (REUByte)((l >> 16) & 255);
		const REUByte b4 = (REUByte)((l >> 24) & 255);
		return ((REUInt32)b1 << 24) + ((REUInt32)b2 << 16) + ((REUInt32)b3 << 8) + b4;
	}
#endif	
public:
	/// Returns md5 integer value from data with length.
	const REUInt32 getFromData(const void * data, const REUInt32 dataLength);
	
	/// Returns md5 string value from data with length.
	const REString getStringFromData(const void * data, const REUInt32 dataLength);
	
	/// Returns md5 integer value from string.
	static const REUInt32 generateFromString(const char * s);
	
	/// Constructor.
	REMD5Generator();
	
	/// Desctructor.
	~REMD5Generator();
};


#endif /* __REMD5GENERATOR_H__ */



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


#include "../../include/recore/REMD5Generator.h"
#include "../../include/recore/REMem.h"
#include "../../include/recore/REString.h"
#include "../../include/recore/REMutableString.h"

#if defined(HAVE_RECORE_SDK_CONFIG_H) 
#include "recore_sdk_config.h"
#endif

#ifdef __RE_OS_IPHONE__
#include <CommonCrypto/CommonDigest.h>
#endif

/*
 MD5 Message Digest Algorithm. (RFC1321)
 */

/*
 
 This code implements the MD5 message-digest algorithm.
 The algorithm is due to Ron Rivest.  This code was
 written by Colin Plumb inBuff 1993, no copyright is claimed.
 This code is inBuff the public domain; do with it what you wish.
 
 Equivalent code is available from RSA Data Security, Inc.
 This code has been tested against that, and is equivalent,
 except that you don't need to include two pages of legalese
 with every copy.
 
 To compute the message digest of a chunk of bytes, declare an
 MD5Context structure, pass it to MD5Init, call MD5Update as
 needed on buffers full of bytes, and then call MD5Final, which
 will fill a supplied 16-byte array with the digest.
 
 */


#ifndef __RE_OS_IPHONE__
/* The four core functions - MD5ChecksumF1 is optimized somewhat */
/* #define MD5ChecksumF1(x, y, z) (x & y | ~x & z) */
#define MD5ChecksumF1(x, y, z) (z ^ (x & (y ^ z)))
#define MD5ChecksumF2(x, y, z) MD5ChecksumF1(z, x, y)
#define MD5ChecksumF3(x, y, z) (x ^ y ^ z)
#define MD5ChecksumF4(x, y, z) (y ^ (x | ~z))

/* This is the central step inBuff the MD5 algorithm. */
#define MD5ChecksumSTEP(f, width, x, y, z, data, s) ( width += f(x, y, z) + data,  width = (width<<s) | (width>>(32-s)),  width += x )

/*
 =================
 Transform
 
 The core of the MD5 algorithm, this alters an existing MD5 hash to
 reflect the addition of 16 longwords of new data.  MD5Update blocks
 the data and converts bytes into longwords for this routine.
 =================
 */

void REMD5Generator::final(REMD5Context * ctx, REUByte digest[16])
{
	REUInt32 count = (ctx->bits[0] >> 3) & 0x3F;
	REUByte * p = ctx->inBuff + count;
	*p++ = 0x80;
	
	count = 64 - 1 - count;
	
	if ( count < 8 ) 
	{
		memset( p, 0, count );
		this->transform( ctx->state, (REUInt32 *)ctx->inBuff );
		
		memset( ctx->inBuff, 0, 56 );
	} 
	else 
	{
		memset( p, 0, count - 8 );
	}
	
	/* Append length inBuff bits and transform */
	REUInt32 val0 = ctx->bits[0];
	REUInt32 val1 = ctx->bits[1];
	
	if (_isBigEndianSystem)
	{
        ((REUInt32 *)ctx->inBuff)[14] = longSwap(val0);
        ((REUInt32 *)ctx->inBuff)[15] = longSwap(val1);
	}
	else
	{
		((REUInt32 *)ctx->inBuff)[14] = val0;
		((REUInt32 *)ctx->inBuff)[15] = val1;
	}
	
	this->transform( ctx->state, (REUInt32 *)ctx->inBuff );
	memcpy( digest, ctx->state, 16 );
	memset(ctx, 0, sizeof( REMD5Context ) );
}

void REMD5Generator::update(REMD5Context * ctx, REUByte const * buf, REUInt32 len)
{	
	REUInt32 t = ctx->bits[0];
	if ( ( ctx->bits[0] = t + ((REUInt32)len << 3) ) < t ) 
	{
		ctx->bits[1]++;
	}
	ctx->bits[1] += (len >> 29);
	t = (t >> 3) & 0x3f; 
	
	if ( t ) 
	{
		REUByte * p = (REUByte *)ctx->inBuff + t;
		t = 64 - t;
		if ( len < t ) 
		{
			memcpy(p, buf, len);
			return;
		}
		memcpy(p, buf, t);
        this->transform( ctx->state, (REUInt32 *)ctx->inBuff );
		buf += t;
		len -= t;
	}
	
	while( len >= 64 ) 
	{
		memcpy(ctx->inBuff, buf, 64);
		this->transform(ctx->state, (REUInt32 *) ctx->inBuff );
		buf += 64;
		len -= 64;
	}
	
	memcpy(ctx->inBuff, buf, len );
}

void REMD5Generator::init(REMD5Context * ctx)
{
	ctx->state[0] = 0x67452301;
	ctx->state[1] = 0xefcdab89;
	ctx->state[2] = 0x98badcfe;
	ctx->state[3] = 0x10325476;
	
	ctx->bits[0] = 0;
	ctx->bits[1] = 0;
}

void REMD5Generator::revBytesSwap(void * bp, const REUInt32 elsize, REUInt32 elcount)
{
	REUByte * p = (REUByte *)bp;
	if ( elsize == 2 ) 
	{
		REUByte * q = p + 1;
		while ( elcount-- ) 
		{
			*p ^= *q;
			*q ^= *p;
			*p ^= *q;
			p += 2;
			q += 2;
		}
		return;
	}
	
	while ( elcount-- ) 
	{
		REUByte * q = (p + elsize) - 1;
		while ( p < q ) 
		{
			*p ^= *q;
			*q ^= *p;
			*p ^= *q;
			++p;
			--q;
		}
		p += (elsize >> 1);
	}		
}


void REMD5Generator::transform(REUInt32 state[4], REUInt32 inBuff[16])
{
	REUInt32 a = state[0];
    REUInt32 b = state[1];
    REUInt32 c = state[2];
    REUInt32 d = state[3];
	
	if (_isBigEndianSystem)
	{
		this->revBytesSwap( inBuff, sizeof(REUInt32), 16 );
	}
	
    MD5ChecksumSTEP(MD5ChecksumF1, a, b, c, d, inBuff[0] + 0xd76aa478, 7);
    MD5ChecksumSTEP(MD5ChecksumF1, d, a, b, c, inBuff[1] + 0xe8c7b756, 12);
    MD5ChecksumSTEP(MD5ChecksumF1, c, d, a, b, inBuff[2] + 0x242070db, 17);
    MD5ChecksumSTEP(MD5ChecksumF1, b, c, d, a, inBuff[3] + 0xc1bdceee, 22);
    MD5ChecksumSTEP(MD5ChecksumF1, a, b, c, d, inBuff[4] + 0xf57c0faf, 7);
    MD5ChecksumSTEP(MD5ChecksumF1, d, a, b, c, inBuff[5] + 0x4787c62a, 12);
    MD5ChecksumSTEP(MD5ChecksumF1, c, d, a, b, inBuff[6] + 0xa8304613, 17);
    MD5ChecksumSTEP(MD5ChecksumF1, b, c, d, a, inBuff[7] + 0xfd469501, 22);
    MD5ChecksumSTEP(MD5ChecksumF1, a, b, c, d, inBuff[8] + 0x698098d8, 7);
    MD5ChecksumSTEP(MD5ChecksumF1, d, a, b, c, inBuff[9] + 0x8b44f7af, 12);
    MD5ChecksumSTEP(MD5ChecksumF1, c, d, a, b, inBuff[10] + 0xffff5bb1, 17);
    MD5ChecksumSTEP(MD5ChecksumF1, b, c, d, a, inBuff[11] + 0x895cd7be, 22);
    MD5ChecksumSTEP(MD5ChecksumF1, a, b, c, d, inBuff[12] + 0x6b901122, 7);
    MD5ChecksumSTEP(MD5ChecksumF1, d, a, b, c, inBuff[13] + 0xfd987193, 12);
    MD5ChecksumSTEP(MD5ChecksumF1, c, d, a, b, inBuff[14] + 0xa679438e, 17);
    MD5ChecksumSTEP(MD5ChecksumF1, b, c, d, a, inBuff[15] + 0x49b40821, 22);
	
    MD5ChecksumSTEP(MD5ChecksumF2, a, b, c, d, inBuff[1] + 0xf61e2562, 5);
    MD5ChecksumSTEP(MD5ChecksumF2, d, a, b, c, inBuff[6] + 0xc040b340, 9);
    MD5ChecksumSTEP(MD5ChecksumF2, c, d, a, b, inBuff[11] + 0x265e5a51, 14);
    MD5ChecksumSTEP(MD5ChecksumF2, b, c, d, a, inBuff[0] + 0xe9b6c7aa, 20);
    MD5ChecksumSTEP(MD5ChecksumF2, a, b, c, d, inBuff[5] + 0xd62f105d, 5);
    MD5ChecksumSTEP(MD5ChecksumF2, d, a, b, c, inBuff[10] + 0x02441453, 9);
    MD5ChecksumSTEP(MD5ChecksumF2, c, d, a, b, inBuff[15] + 0xd8a1e681, 14);
    MD5ChecksumSTEP(MD5ChecksumF2, b, c, d, a, inBuff[4] + 0xe7d3fbc8, 20);
    MD5ChecksumSTEP(MD5ChecksumF2, a, b, c, d, inBuff[9] + 0x21e1cde6, 5);
    MD5ChecksumSTEP(MD5ChecksumF2, d, a, b, c, inBuff[14] + 0xc33707d6, 9);
    MD5ChecksumSTEP(MD5ChecksumF2, c, d, a, b, inBuff[3] + 0xf4d50d87, 14);
    MD5ChecksumSTEP(MD5ChecksumF2, b, c, d, a, inBuff[8] + 0x455a14ed, 20);
    MD5ChecksumSTEP(MD5ChecksumF2, a, b, c, d, inBuff[13] + 0xa9e3e905, 5);
    MD5ChecksumSTEP(MD5ChecksumF2, d, a, b, c, inBuff[2] + 0xfcefa3f8, 9);
    MD5ChecksumSTEP(MD5ChecksumF2, c, d, a, b, inBuff[7] + 0x676f02d9, 14);
    MD5ChecksumSTEP(MD5ChecksumF2, b, c, d, a, inBuff[12] + 0x8d2a4c8a, 20);
	
    MD5ChecksumSTEP(MD5ChecksumF3, a, b, c, d, inBuff[5] + 0xfffa3942, 4);
    MD5ChecksumSTEP(MD5ChecksumF3, d, a, b, c, inBuff[8] + 0x8771f681, 11);
    MD5ChecksumSTEP(MD5ChecksumF3, c, d, a, b, inBuff[11] + 0x6d9d6122, 16);
    MD5ChecksumSTEP(MD5ChecksumF3, b, c, d, a, inBuff[14] + 0xfde5380c, 23);
    MD5ChecksumSTEP(MD5ChecksumF3, a, b, c, d, inBuff[1] + 0xa4beea44, 4);
    MD5ChecksumSTEP(MD5ChecksumF3, d, a, b, c, inBuff[4] + 0x4bdecfa9, 11);
    MD5ChecksumSTEP(MD5ChecksumF3, c, d, a, b, inBuff[7] + 0xf6bb4b60, 16);
    MD5ChecksumSTEP(MD5ChecksumF3, b, c, d, a, inBuff[10] + 0xbebfbc70, 23);
    MD5ChecksumSTEP(MD5ChecksumF3, a, b, c, d, inBuff[13] + 0x289b7ec6, 4);
    MD5ChecksumSTEP(MD5ChecksumF3, d, a, b, c, inBuff[0] + 0xeaa127fa, 11);
    MD5ChecksumSTEP(MD5ChecksumF3, c, d, a, b, inBuff[3] + 0xd4ef3085, 16);
    MD5ChecksumSTEP(MD5ChecksumF3, b, c, d, a, inBuff[6] + 0x04881d05, 23);
    MD5ChecksumSTEP(MD5ChecksumF3, a, b, c, d, inBuff[9] + 0xd9d4d039, 4);
    MD5ChecksumSTEP(MD5ChecksumF3, d, a, b, c, inBuff[12] + 0xe6db99e5, 11);
    MD5ChecksumSTEP(MD5ChecksumF3, c, d, a, b, inBuff[15] + 0x1fa27cf8, 16);
    MD5ChecksumSTEP(MD5ChecksumF3, b, c, d, a, inBuff[2] + 0xc4ac5665, 23);
	
    MD5ChecksumSTEP(MD5ChecksumF4, a, b, c, d, inBuff[0] + 0xf4292244, 6);
    MD5ChecksumSTEP(MD5ChecksumF4, d, a, b, c, inBuff[7] + 0x432aff97, 10);
    MD5ChecksumSTEP(MD5ChecksumF4, c, d, a, b, inBuff[14] + 0xab9423a7, 15);
    MD5ChecksumSTEP(MD5ChecksumF4, b, c, d, a, inBuff[5] + 0xfc93a039, 21);
    MD5ChecksumSTEP(MD5ChecksumF4, a, b, c, d, inBuff[12] + 0x655b59c3, 6);
    MD5ChecksumSTEP(MD5ChecksumF4, d, a, b, c, inBuff[3] + 0x8f0ccc92, 10);
    MD5ChecksumSTEP(MD5ChecksumF4, c, d, a, b, inBuff[10] + 0xffeff47d, 15);
    MD5ChecksumSTEP(MD5ChecksumF4, b, c, d, a, inBuff[1] + 0x85845dd1, 21);
    MD5ChecksumSTEP(MD5ChecksumF4, a, b, c, d, inBuff[8] + 0x6fa87e4f, 6);
    MD5ChecksumSTEP(MD5ChecksumF4, d, a, b, c, inBuff[15] + 0xfe2ce6e0, 10);
    MD5ChecksumSTEP(MD5ChecksumF4, c, d, a, b, inBuff[6] + 0xa3014314, 15);
    MD5ChecksumSTEP(MD5ChecksumF4, b, c, d, a, inBuff[13] + 0x4e0811a1, 21);
    MD5ChecksumSTEP(MD5ChecksumF4, a, b, c, d, inBuff[4] + 0xf7537e82, 6);
    MD5ChecksumSTEP(MD5ChecksumF4, d, a, b, c, inBuff[11] + 0xbd3af235, 10);
    MD5ChecksumSTEP(MD5ChecksumF4, c, d, a, b, inBuff[2] + 0x2ad7d2bb, 15);
    MD5ChecksumSTEP(MD5ChecksumF4, b, c, d, a, inBuff[9] + 0xeb86d391, 21);
	
	if (_isBigEndianSystem)
	{
		this->revBytesSwap( inBuff, sizeof(REUInt32), 16 );
	}
	
    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;	
}
#endif

const REUInt32 REMD5Generator::fromData(const void * data, const REUInt32 dataLength)
{
	if (data && dataLength) 
	{
#ifndef __RE_OS_IPHONE__		
		REMD5Context ctx;
		this->init( &ctx );
		this->update( &ctx, (REUByte *)data, dataLength );
		
		REUInt32 digest[4];
		this->final( &ctx, (REUByte *)digest );
		
		return (digest[0] ^ digest[1] ^ digest[2] ^ digest[3]);
#else
		union
		{
			unsigned char ub[16];
			REUInt32 ui[4];
		} ub;
		CC_MD5(data, (CC_LONG)dataLength, ub.ub);
		return (ub.ui[0] ^ ub.ui[1] ^ ub.ui[2] ^ ub.ui[3]);
#endif		
	}
	return 0;
}

const REString REMD5Generator::stringFromData(const void * data, const REUInt32 dataLength)
{
	if (data && dataLength) 
	{
#ifndef __RE_OS_IPHONE__		
		REMD5Context ctx;
		this->init( &ctx );
		this->update( &ctx, (REUByte *)data, dataLength );
		
		REUByte ub[16];
		this->final( &ctx, (REUByte *)ub );
		
		REMutableString retString;
		retString.appendFormat("%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x", 
							   ub[0], ub[1], ub[2], ub[3], ub[4], ub[5], ub[6], ub[7], 
							   ub[8], ub[9], ub[10], ub[11], ub[12], ub[13], ub[14], ub[15]);	
		return retString;
#else
		unsigned char ub[16];
		CC_MD5(data, (CC_LONG)dataLength, ub);
		REMutableString retString;
		retString.appendFormat("%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x", 
							   ub[0], ub[1], ub[2], ub[3], ub[4], ub[5], ub[6], ub[7], 
							   ub[8], ub[9], ub[10], ub[11], ub[12], ub[13], ub[14], ub[15]);	
		return retString;
#endif		
	}
	return REString("");
}

REMD5Generator::REMD5Generator() 
#ifndef __RE_OS_IPHONE__
:_isBigEndianSystem(0)
#endif
{
#ifndef __RE_OS_IPHONE__	
	REUInt16 digitForTest = 0;
	REUByte * digitArray = (REUByte*)&digitForTest;
	digitArray[0] = 1;
	if (digitForTest == 1)
	{
		// is little endian
		_isBigEndianSystem = 0;
	}
#endif	
}

REMD5Generator::~REMD5Generator()
{
	
}

const REUInt32 REMD5Generator::generateFromString(const char * s)
{
	if (s) 
	{
		REMD5Generator g;
		return g.fromData(s, (REUInt32)strlen(s));
	}
	return 0;
}




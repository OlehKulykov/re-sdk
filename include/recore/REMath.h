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


#ifndef __REMATH_H__
#define __REMATH_H__

#include "RECommonHeader.h"


#include <math.h>
#include <float.h>

/* pi */
#ifndef MATH_PI
/// PI value.
#define MATH_PI (3.14159265358979323846264338327950288) 
#endif

/* pi/2 */
#ifndef MATH_PI_DIV_2
/// PI devided by 2 value.
#define MATH_PI_DIV_2 (1.57079632679489661923132169163975144) 
#endif

/* pi/4 */
#ifndef MATH_PI_DIV_4
/// PI devided by 4 value.
#define MATH_PI_DIV_4 (0.785398163397448309615660845819875721) 
#endif

/* 1/pi */
#ifndef MATH_1_DIV_PI
/// 1 devided PI value.
#define MATH_1_DIV_PI (0.318309886183790671537767526745028724)
#endif

/* 2/pi */
#ifndef MATH_2_DIV_PI
/// 2 devided PI value.
#define MATH_2_DIV_PI (0.636619772367581343075535053490057448) 
#endif

/* 2*pi */
#ifndef MATH_2_MUL_PI
/// PI multiplied by 2 value.
#define MATH_2_MUL_PI (6.283185307179586476925286766559)
#endif

/* 180/pi */
#ifndef MATH_RADIAN
/// Radian value also 180 devided by PI value.
#define MATH_RADIAN (57.295779513082320876798154814105) 
#endif

/* 180/pi */
#ifndef MATH_180_DIV_PI
/// 180 devided by PI value.
#define MATH_180_DIV_PI MATH_RADIAN 
#endif

/* pi/180 */
#ifndef MATH_PI_DIV_180
/// PI devided by 180 value.
#define MATH_PI_DIV_180 (0.017453292519943295769236907684886) 
#endif

/* 2/sqrt(pi) */
#ifndef MATH_2_SQRT_PI
/// 2 divided by sqrt of PI value.
#define MATH_2_SQRT_PI (1.12837916709551257389615890312154517) 
#endif

/* e */
#ifndef MATH_E
/// Base of the natural logarithm or  Napier's constant or Euler.
/// circle length divided by diameter.
#define MATH_E (2.71828182845904523536028747135266250) 
#endif

/* log 2e */
#ifndef MATH_LOG2E
/// logarithm 2 of E value.
#define MATH_LOG2E (1.44269504088896340735992468100189214) 
#endif

/* log 10e */
#ifndef MATH_LOG10E
/// logarithm 10 of E value.
#define MATH_LOG10E (0.434294481903251827651128918916605082)
#endif

/* log e2 */
#ifndef MATH_LN2
/// logarithm of E2 value.
#define MATH_LN2 (0.693147180559945309417232121458176568) 
#endif

/* log e10 */
#ifndef MATH_LN10
/// logarithm of E10 value.
#define MATH_LN10 (2.30258509299404568401799145468436421) 
#endif

/* sqrt(2) */
#ifndef MATH_SQRT_2
/// SQRT of 2 value.
#define MATH_SQRT_2 (1.41421356237309504880168872420969808) 
#endif

/* 1/sqrt(2) */
#ifndef MATH_1_SQRT_2
/// 1 divided by SQRT of 2 value.
#define MATH_1_SQRT_2 (0.707106781186547524400844362104849039) 
#endif

#ifndef FLT_EPSILON
/// Float32 epsilon value.
#define FLT_EPSILON (1E-5f)
#endif

#ifndef DBL_EPSILON
/// Double(float64) epsilon value.
#define DBL_EPSILON (1E-9)
#endif

#ifndef IS_FLOATS_EQUAL
/// Check equality of two float32.
#define IS_FLOATS_EQUAL(a,b) ((fabsf(a-b))<(FLT_EPSILON)?(true):(false))
#endif

#ifndef IS_DOUBLES_EQUAL
/// /// Check equality of two doubles(float64).
#define IS_DOUBLES_EQUAL(a,b) ((fabs(a-b))<(DBL_EPSILON)?(true):(false))
#endif

#ifndef DEG_TO_RAD
/// Converts degree to radian angle.
#define DEG_TO_RAD(a) (MATH_PI_DIV_180 * a)
#endif

#ifndef RAD_TO_DEG
/// Converts radian to degree angle.
#define RAD_TO_DEG(a) (MATH_180_DIV_PI * a)
#endif

#ifndef MATH_ROUND
/// Rounds float value.
#define MATH_ROUND(r) ((r>0.0)?floor(r+0.5):ceil(r-0.5))
#endif

#ifndef IS_DIVISIBLE_BY_2
/// Checks integer for divisible by 2.
#define IS_DIVISIBLE_BY_2(v) !(v&0x1)
#endif

#ifndef IS_DIVISIBLE_BY_4
/// Checks integer for divisible by 4.
#define IS_DIVISIBLE_BY_4(v) !(v&0x3)
#endif

#ifndef IS_DIVISIBLE_BY_8
/// Checks integer for divisible by 8.
#define IS_DIVISIBLE_BY_8(v) !(v&0x7)
#endif

#ifndef IS_DIVISIBLE_BY_16
/// Checks integer for divisible by 16.
#define IS_DIVISIBLE_BY_16(v) !(v&0xF)
#endif


#ifndef MAX
/// Gettes max value.
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif

#ifndef MIN
/// Gettes min value.
#define MIN(a,b) (((a)<(b))?(a):(b))
#endif

#if defined (__ARM_NEON__)
#include <arm_neon.h>
#endif

//TODO: may by optimize speed for architectures...
class __RE_PUBLIC_CLASS_API__ REMath
{
public:
	/// Template for getter of max of 2 values.
	template <class T> static const T & Max(const T & v1, const T & v2) 
	{
		return (v1 > v2) ? v1 : v2;
	}
	
	/// Template for getter of max of 3 values.
	template <class T> static const T & Max(const T & v1, const T & v2, const T & v3) 
	{
		const T & m1 = REMath::Max<T>(v1, v2);
		return REMath::Max<T>(m1, v3);
	}
	
	/// Template for getter of max of 4 values.
	template <class T> static const T & Max(const T & v1, const T & v2, const T & v3, const T & v4) 
	{
		const T & m1 = REMath::Max<T>(v1, v2, v3);
		return REMath::Max<T>(m1, v4);
	}
	
	/// Template for getter of min of 2 values.
	template <class T> static const T & Min(const T & v1, const T & v2) 
	{
		return (v1 < v2) ? v1 : v2;
	}
	
	/// Template for getter of min of 3 values.
	template <class T> static const T & Min(const T & v1, const T & v2, const T & v3) 
	{
		const T & m1 = REMath::Min<T>(v1, v2);
		return REMath::Min<T>(m1, v3);
	}
	
	/// Template for getter of min of 4 values.
	template <class T> static const T & Min(const T & v1, const T & v2, const T & v3, const T & v4) 
	{
		const T & m1 = REMath::Min<T>(v1, v2, v3);
		return REMath::Min<T>(m1, v4);
	}	
};

#ifndef ROUND_TO_SIGN_DIGITS
/// Rounds float to number of significant digits.
#define ROUND_TO_SIGN_DIGITS(fv,dn) (floor(fv*pow(10.0,(int)dn)+0.5)/pow(10.0,(int)dn))
#endif

#endif /* __REMATH_H__ */ 



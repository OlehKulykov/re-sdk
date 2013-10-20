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


#ifndef __REPOINT2_H__
#define __REPOINT2_H__

#include "RECommonHeader.h"

#if defined(__ARM_NEON__) || defined(HAVE_ARM_NEON_H) 
#include <arm_neon.h>
#endif

class REString;

typedef struct _REPoint2Struct
{
	union 
	{ 
		struct 
		{
			REFloat32 x; 
			REFloat32 y; 
		};
		
#if defined(__ARM_NEON__) || defined(HAVE_ARM_NEON_H) 
		float32x2_t armNeonPoint;
#endif	
		
		REFloat32 coords[2]; 
	};
} REPoint2Struct;

/// Class using for presenting 2D Point with 'x' and 'y' float coordinates.
class __RE_PUBLIC_CLASS_API__ REPoint2
{
public:
	union
	{
		struct 
		{
			/// X coordinate.
			REFloat32 x;
			
			/// Y coordinate.
			REFloat32 y;
		};
#if defined(__ARM_NEON__) || defined(HAVE_ARM_NEON_H) 
		/// used for arm optimizations
		float32x2_t armNeonPoint;
#endif		
		/// array of 'x' and 'y'
		REFloat32 coords[2];
	};
	
	/// Copy operator. This point will be same as anotherPoint struct.
	REPoint2 & operator=(const REPoint2Struct & anotherPoint)
	{
#if defined(__ARM_NEON__) || defined(HAVE_ARM_NEON_H) 
		armNeonPoint = anotherPoint.armNeonPoint;
#else		
		x = anotherPoint.x;
		y = anotherPoint.y;
#endif		
		return (*this);
	}
	
	/// Copy operator. This point will be same as anotherPoint.
	REPoint2 & operator=(const REPoint2 & anotherPoint)
	{
#if defined(__ARM_NEON__) || defined(HAVE_ARM_NEON_H) 
		armNeonPoint = anotherPoint.armNeonPoint;
#else		
		x = anotherPoint.x;
		y = anotherPoint.y;
#endif		
		return (*this);
	}
	
	/// Constructs 2D Point with 'newX' - X coordinate and 'newY' - Y coordinate.
	REPoint2(const REFloat32 newX, const REFloat32 newY) : x(newX), y (newY) { }

	/// Constructs 2D Point same as anotherPoint struct.
	REPoint2(const REPoint2Struct & anotherPoint) : 
#if defined(__ARM_NEON__) || defined(HAVE_ARM_NEON_H) 
		armNeonPoint(anotherPoint.armNeonPoint) { }
#else	
		x(anotherPoint.x), y(anotherPoint.y) { }
#endif	
	
	/// Constructs 2D Point same as anotherPoint.
	REPoint2(const REPoint2 & anotherPoint) : 
#if defined(__ARM_NEON__) || defined(HAVE_ARM_NEON_H) 
		armNeonPoint(anotherPoint.armNeonPoint) { }
#else	
		x(anotherPoint.x), y(anotherPoint.y) { }
#endif	
	
	/// Constructs 2D Point and sets 'x' and 'y' coordinates to 0.
	REPoint2() : x(0.0f), y(0.0f) { }
	~REPoint2() { }
	
	
/// Objective-c additions
#if (defined(CG_EXTERN) || defined(CG_INLINE)) && defined(CGFLOAT_TYPE)

	CGPoint getCGPoint() const 
	{
		CGPoint p;
		p.x = (CGFloat)x; 
		p.y = (CGFloat)y;
		return p;
	}
	
	REPoint2 & operator=(const CGPoint & anotherPoint)
	{
		x = (REFloat32)anotherPoint.x;
		y = (REFloat32)anotherPoint.y;
		return (*this);
	}

	REPoint2(const CGPoint & anotherPoint) : 
		x((REFloat32)anotherPoint.x), 
		y((REFloat32)anotherPoint.y) 
	{
		
	}
	
#endif
	
	static REPoint2 fromString(const char * string);
	
	static REPoint2 fromString(const REString & string);
	
	static REString toString(const REPoint2 & point);
};


#endif /* __REPOINT2_H__ */


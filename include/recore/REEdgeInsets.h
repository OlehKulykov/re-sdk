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


#ifndef __REEDGEINSETS_H__
#define __REEDGEINSETS_H__

#include "RECommonHeader.h"
#include "RERect.h"
 
#if defined(__ARM_NEON__)
#include <arm_neon.h>
#endif

/// Class present rectangular offset.
class __RE_PUBLIC_CLASS_API__ REEdgeInsets
{
public:
	union
	{
		struct 
		{
			/// offset from top side.
			REFloat32 top;
			
			/// offset from left side.
			REFloat32 left;
			
			/// offset from bottom side.
			REFloat32 bottom;
			
			/// offset from right side.
            REFloat32 right;
		};
#if defined(__ARM_NEON__)		
		/// arm neon vector with top, left, bottom and right values.
		float32x4_t armNeonVector;
#endif	
		/// array with top, left, bottom and right values.
		REFloat32 tlbr[4];
	};
	
	/// Returns adjusted rectangle.
	RERect GetAdjustedRect(const RERect & rect) const
	{
		return RERect(rect.x + left,
					  rect.y + top,
					  rect.width - left - right,
					  rect.height - top - bottom);
	}
	
	/// Assing operator.
	REEdgeInsets & operator=(const REEdgeInsets & insets)
	{
#if defined(__ARM_NEON__)
		armNeonVector = insets.armNeonVector;
#else
        top = insets.top;
        left = insets.left;
        bottom = insets.bottom;
        right = insets.right;
#endif
		return (*this);
	}
	
	/// Construct insets from another insets object.
	REEdgeInsets(const REEdgeInsets & insets) :
#if defined(__ARM_NEON__)
		armNeonVector(insets.armNeonVector)
#else
		top(insets.top), 
		left(insets.left), 
		bottom(insets.bottom),
		right(insets.right)
#endif
	{ 
	
	}
	
	/// Construct insets with top, left, bottom and right values.
	REEdgeInsets(const REFloat32 newTop, const REFloat32 newLeft, const REFloat32 newBottom, const REFloat32 newRight) : 
		top(newTop), 
		left(newLeft), 
		bottom(newBottom), 
		right(newRight) 
	{
		
	}

#if defined(__ARM_NEON__)
	/// Construct insets object from arm neon vector.
	REEdgeInsets(const float32x4_t & anotherArmNeonVector) :
		armNeonVector(anotherArmNeonVector) { }
#endif
	
	/// Constructs zero value insets object.
	REEdgeInsets() : top(0.0f), left(0.0f), bottom(0.0f), right(0.0f) { }
	
	/// Destructor.
	~REEdgeInsets() { }
};

#endif /* __REEDGEINSETS_H__ */


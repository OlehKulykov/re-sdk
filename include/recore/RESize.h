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


#ifndef __RESIZE_H__
#define __RESIZE_H__

#include "RECommonHeader.h"

#if defined(__ARM_NEON__)
#include <arm_neon.h>
#endif

/// Size structure.
typedef struct _reSizeStruct
{
	union
	{
		struct
		{
			/// Width value.
			REFloat32 width;
			
			/// Height value.
			REFloat32 height;
		};
#if defined(__ARM_NEON__)
		/// used for arm optimizations.
		float32x2_t armNeonVector;
#endif	
		/// Array with width and height.
		REFloat32 size[2];
	};
}
/// Size structure.
RESizeStruct;


/// Size class.
class __RE_PUBLIC_CLASS_API__ RESize 
{
public:
	union
	{
		struct
		{
			/// Width value.
			REFloat32 width;
			
			/// Height value.
			REFloat32 height;
		};
#if defined(__ARM_NEON__)
		/// used for arm optimizations.
		float32x2_t armNeonVector;
#endif
		/// Array with width and height.
		REFloat32 size[2];
	};
	
	/// Check is size is zero value.
	REBOOL IsNull() const { return ( (width == 0.0f) && (height == 0.0f) ); }
	
	/// Sets size to zero values.
	RESize & SetToNull() { width = 0.0f; height = 0.0f; return (*this); }
	
	/// Basic assignment from size struct.
	RESize & operator=(const RESizeStruct & anotherSize)
	{
#if defined(__ARM_NEON__)
		armNeonVector = anotherSize.armNeonVector;
#else
		width = anotherSize.width;
		height = anotherSize.height;
#endif
		return (*this);
	}
	
	/// Basic assignment from another size.
	RESize & operator=(const RESize & anotherSize)
	{
#if defined(__ARM_NEON__)
		armNeonVector = anotherSize.armNeonVector;
#else
		width = anotherSize.width;
		height = anotherSize.height;
#endif
		return (*this);
	}

#if defined(__ARM_NEON__)
	/// Constructs size with values from arm neon vector.
	RESize(const float32x2_t & armNVec) : armNeonVector(armNVec) { }
#endif
	
	/// Constructs size with another size struct.
	RESize(const RESizeStruct & anotherSize) : width(anotherSize.width), height(anotherSize.height) { }
	
	/// Constructs size with another size.
	RESize(const RESize & anotherSize) : width(anotherSize.width), height(anotherSize.height) { }
	
	/// Constructs size with width and height values.
	RESize(const REFloat32 newWidth, const REFloat32 newHeight) : width(newWidth), height(newHeight) { }
	
	/// Constructs size with zero values.
	RESize() : width(0.0f), height(0.0f) { }
	
	/// Desctructor.
	~RESize() { }
};

#endif /* __RESIZE_H__ */


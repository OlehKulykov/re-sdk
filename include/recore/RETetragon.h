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


#ifndef __RETETRAGON_H__
#define __RETETRAGON_H__

#include "RECommonHeader.h"
#include "REPoint2.h"
#include "REMem.h"

class REString;
class RERect;
typedef struct _reRectStruct RERectStruct;

#if defined(__ARM_NEON__) || defined(HAVE_ARM_NEON_H) 
#include <arm_neon.h>
#endif

/// Class of tetragon.
/// Class fields arranged in the order for fast rendering in OpenGL(ES)
/// as coordinates array.
class __RE_PUBLIC_CLASS_API__ RETetragon
{
public:
	union
	{
		struct
		{
			/// Top left X coordinate.
			REFloat32 topLeftX;
			
			/// Top left Y coordinate.
			REFloat32 topLeftY;
			
			/// Top right X coordinate.
			REFloat32 topRightX;
			
			/// Top right Y coordinate.
			REFloat32 topRightY;
			
			/// Bottom left X coordinate.
			REFloat32 bottomLeftX;
			
			/// Bottom left Y coordinate.
			REFloat32 bottomLeftY;
			
			/// Bottom right X coordinate.
			REFloat32 bottomRightX;
			
			/// Bottom right Y coordinate.
			REFloat32 bottomRightY;
		};
#if defined(__ARM_NEON__) || defined(HAVE_ARM_NEON_H) 	
		struct
		{
			/// arm neon top coordinates vector.
			float32x4_t armNeonTop;
			
			/// arm neon bottom coordinates vector.
			float32x4_t armNeonBottom;
		};
#endif
		/// array of coordinates.
		REFloat32 arr[8];
	};
	
	/// Returns center point of tetragon.
	REPoint2 center() const;
	
	/// Returns bounding rectangle.
	RERect boundRect() const;
	
	/// Translates tetragon by X, Y values.
	RETetragon & translate(const REFloat32 x, const REFloat32 y)
	{
//#if defined(__ARM_NEON__) || defined(HAVE_ARM_NEON_H) 
//	TODO: __ARM_NEON__ optimization.
//#else
		topLeftX += x;
		topRightX += x;
		bottomLeftX += x;
		bottomRightX += x;
		
		topLeftY += y;
		topRightY += y;
		bottomLeftY += y;
		bottomRightY += y;
//#endif
		return (*this);
	}
	
	/// Sets tetragon from rectangle struct.
	void set(const RERectStruct & rect);
	
	/// Sets tetragon from rectangle.
	void set(const RERect & rect);
	
	/// Sets tetragon from another tetragon.
	void set(const RETetragon & tetr);
	
	/// Basic assignment operator from rectangle struct.
	RETetragon & operator=(const RERectStruct & anotherRect);
	
	/// Basic assignment operator from rectangle.
	RETetragon & operator=(const RERect & anotherRect);
	
	/// Basic assignment operator from another tetragon.
	RETetragon & operator=(const RETetragon & tetr);
	
	/// Constructs tetragon from rectangle struct.
	RETetragon(const RERectStruct & rect)
	{
		this->set(rect);
	}
	
	/// Constructs tetragon from rectangle.
	RETetragon(const RERect & rect)
	{
		this->set(rect);
	}
	
	/// Constructs tetragon from another tetragon.
	RETetragon(const RETetragon & tetr)
	{
		memcpy(arr, tetr.arr, 8 * sizeof(REFloat32));
	}
	
	/// Constructs tetragon with zero value coordinates.
	RETetragon() 
	{
		memset(arr, 0, 8 * sizeof(REFloat32)); 
	}
	
	/// Destructor.
	~RETetragon() { }
	
	static RETetragon fromString(const char * string);
	
	static RETetragon fromString(const REString & string);
	
	static REString toString(const RETetragon & tetragon);
};

#endif /* __RETETRAGON_H__ */


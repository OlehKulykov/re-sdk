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


#ifndef __RERANGE_H__ 
#define __RERANGE_H__ 

#include "RECommonHeader.h"

#if defined(__ARM_NEON__)
#include <arm_neon.h>
#endif

/// Class of range.
class __RE_PUBLIC_CLASS_API__ RERange 
{
public:
	union
	{
		struct
		{
			/// Location position of range.
			REUInt32 location;
			
			/// Length of range.
			REUInt32 length;
		};
#if defined(__ARM_NEON__)
		/// Arm neon range values vector.
		float32x2_t armNeonRange;
#endif			
		/// Array or range values.
		REUInt32 range[2];
	};
	
	/// Check is range exists.
	/// Range exists if location not equal to RENotFound and
	/// length not equal to zero.
	REBOOL IsExists() const 
	{
		return ( (location != RENotFound) && (length != 0) ); 
	}
	
	/// Check is range exists.
	/// Range not exists if location is equal to RENotFound and
	/// length is equal to zero.
	REBOOL IsNotExists() const
	{
		return ( (location == RENotFound) && (length == 0) ); 
	}
	
	/// Sets range to not existent.
	RERange & ToNonExistent()
	{
		location = RENotFound;
		length = 0;
		return (*this);
	}
	
	/// Sets range with new location and length.
	RERange & Set(const REUInt32 newLocation, const REUInt32 newLenght)
	{
		location = newLocation;
		length = newLenght;
		return (*this);
	}
	
	/// Check is range qual to another range.
	REBOOL IsEqual(const RERange & anotherRange) 
	{
		return ((location == anotherRange.location) && (length == anotherRange.length)); 
	}
	
	/// Basic asignment operator.
	RERange & operator=(const RERange & anotherRange) 
	{
#if defined(__ARM_NEON__)
		armNeonRange = anotherRange.armNeonRange;
#else
		location = anotherRange.location;
		length = anotherRange.length;
#endif
		return (*this); 
	}

#if defined(__ARM_NEON__)
	/// Constructs range with arm neon range values vector.
	RERange(const float32x2_t & anotherRangeArmNeonRange) : 
		armNeonRange(anotherRangeArmNeonRange)
	{
		
	}
#endif		
	
	/// Constructs range with values from another range object.
	RERange(const RERange & anotherRange) : 
		location(anotherRange.location), 
		length(anotherRange.length) 
	{
		
	}
	
	/// Constructs range with new location and length.
	RERange(const REUInt32 newLocation, const REUInt32 newLength) :
		location(newLocation), 
		length(newLength)
	{
		
	}
	
	/// Constructs non existent range.
	RERange() : 
		location(RENotFound), 
		length(0) 
	{
		
	}
	
	/// Destructor.
	~RERange() { }
};

#endif /* __RERANGE_H__  */


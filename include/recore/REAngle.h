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


#ifndef __REANGLE_H__
#define __REANGLE_H__

#include "RECommonHeader.h"
#include "REMath.h"

/// Class of degree angle.
class __RE_PUBLIC_CLASS_API__ REDegreeAngle 
{
public:
	/// Value of degree angle
	REFloat32 degreeAngle;
	
	/// Overloaded basic assignment operator.
	REDegreeAngle & operator=(const REDegreeAngle & anotherAngle)
	{
		degreeAngle = anotherAngle.degreeAngle;
		return (*this);
	}
	
	/// Returns float value of angle in degrees.
	const REFloat32 GetDegree() const { return degreeAngle; }
	
	/// Returns float value of angle in radians.
	const REFloat32 GetRadian() const { return DEG_TO_RAD(degreeAngle); }

	/// Set value in degrees.
	REDegreeAngle & SetDegree(const REFloat32 degAngle) 
	{
		if ( fabsf(degAngle) >= 360.0f ) 
		{
			degreeAngle = fmodf(degAngle, 360.0f); 
		}
		else 
		{ 
			degreeAngle = degAngle; 
		}
		return (*this);
	}
	
	/// Set value in radians.
	REDegreeAngle & SetRadian(const REFloat32 radAngle)
	{
		if (fabsf(radAngle) >= MATH_2_MUL_PI)
		{
			this->SetDegree(fmodf(radAngle, MATH_2_MUL_PI));
		}
		else
		{
			this->SetDegree(RAD_TO_DEG(radAngle));
		}
		return (*this);
	}
	
	/// Constructs angle object with another degree angle.
	REDegreeAngle(const REDegreeAngle & degAngle) : degreeAngle(degAngle.degreeAngle) { }
	
	/// Constructs angle object with float value of degree angle
	REDegreeAngle(const REFloat32 degAngle) : degreeAngle(degAngle) { }
	
	/// Constructs angle object zero angle value.
	REDegreeAngle() : degreeAngle(0.0f) { }
	
	/// Destructor.
	virtual ~REDegreeAngle() { }
};

 
#endif /* __REANGLE_H__ */


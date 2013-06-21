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


#ifndef __IREAFFINETRANSFORMED_H__
#define __IREAFFINETRANSFORMED_H__

#include "RECommonHeader.h"

/// Class interface for Affine Transformed objects
class __RE_PUBLIC_CLASS_API__ IREAffineTransformed
{
public:
	/// Translate object by 'x' - horizontaly and 'y' - verticaly.
	virtual IREAffineTransformed & translate(const REFloat32 x, const REFloat32 y) = 0;
	
	/// Rotate object arround center by 'radianAngle' in radians.
	virtual IREAffineTransformed & rotate(const REFloat32 radianAngle) = 0;
	
	/// Rotate object arround center by 'radianAngle' in radians arround 'anchorPointX' and 'anchorPointY' coordinates.
	virtual IREAffineTransformed & rotate(const REFloat32 radianAngle, const REFloat32 anchorPointX, const REFloat32 anchorPointY) = 0;	
	
	/// Scale object by 'sx' and 'sy' values.
	virtual IREAffineTransformed & scale(const REFloat32 sx, const REFloat32 sy) = 0;
	
	/// Set object transform values to identity.
	virtual IREAffineTransformed & toIdentity() = 0;
	
	/// Desctructor.
	virtual ~IREAffineTransformed() { }
};


#endif /* __REAFFINETRANSFORM_H__ */


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


#ifndef __RE2DALGORITHM_H__
#define __RE2DALGORITHM_H__

#include "RECommonHeader.h"
#include "REMath.h"


class REPoint2;

/// Class of with some 2D algorithms.
class __RE_PUBLIC_CLASS_API__ RE2DAlgorithm
{
public:
	/// Galacticomm's floating point algorithm used to determine whether a point is inside a polygon.
	/// 'pointX' and 'pointY' - point for checking
	/// 'numberOfPolygonPoints' number(count) of polygon points
	/// 'polygonPointsX' and 'polygonPointsY' pointer to arrays of polygon coordinates, 
	/// each array must be at lest 'numberOfPolygonPoints' elements.
	static REBOOL IsPointInPolygonFloat32(const REFloat32 pointX, 
										  const REFloat32 pointY, 
										  const REUInt32 numberOfPolygonPoints,
										  const REFloat32 * polygonPointsX,
										  const REFloat32 * polygonPointsY);
	
	/// Galacticomm's integer point algorithm used to determine whether a point is inside a polygon.
	/// 'pointX' and 'pointY' - point for checking
	/// 'numberOfPolygonPoints' number(count) of polygon points
	/// 'polygonPointsX' and 'polygonPointsY' pointer to arrays of polygon coordinates, 
	/// each array must be at lest 'numberOfPolygonPoints' elements.
	static REBOOL IsPointInPolygonInt32(const REInt32 pointX, 
										const REInt32 pointY, 
										const REUInt32 numberOfPolygonPoints,
										const REInt32 * polygonPointsX,
										const REInt32 * polygonPointsY);
	
	/// Check equality of two 32 bit floats
	static REBOOL IsEqualFloat32(const REFloat32 firstValue, const REFloat32 secondValue)
	{
		if (fabsf(firstValue - secondValue) < FLT_EPSILON) { return true; }
		return false;
	}
	
	/// Check equality of two 64 bit floats
	static REBOOL IsEqualFloat64(const REFloat64 firstValue, const REFloat64 secondValue)
	{
		if (fabs(firstValue - secondValue) < DBL_EPSILON) { return true; }
		return false;
	}
	
	/// Rounds 32 bit float value to number of significant digits: 3.14159(2) => 3.14000
	static REFloat32 RoundNumSignificantDigitsFloat32(const REFloat32 float32Value, const REInt32 numberOfDigits)
	{
		const REFloat32 p = powf(10.0f, numberOfDigits);
		return (REFloat32)floorf(float32Value * p + 0.5f) / p;
	}
	
	/// Rounds 64 bit float value to number of significant digits: 3.14159(2) => 3.14000
	static REFloat64 RoundNumSignificantDigitsFloat64(const REFloat64 float64Value, const REInt32 numberOfDigits)
	{
		const REFloat64 p = pow(10.0, numberOfDigits);
		return (REFloat64)floor(float64Value * p + 0.5) / p;
	}
	
	/// Return length of line that presented with two points.
	/// ['x1', 'y1'] - start line point and ['x2', 'y2'] - end line point.
	static REFloat32 GetLineLengthFloat32(const REFloat32 x1, 
										  const REFloat32 y1, 
										  const REFloat32 x2, 
										  const REFloat32 y2)
	{
		const REFloat64 mulX = x1 - x2;
		const REFloat64 mulY = y1 - y2;
		return (REFloat32)sqrt(((mulX * mulX) + (mulY * mulY)));
	}

	/// Return length of line that presented with two points.
	/// 'firstPoint' - start line point and 'secondPoint' - end line point.
	static REFloat32 GetLineLengthFloat32(const REPoint2 & firstPoint, const REPoint2 & secondPoint);
	
	/// Returns point of crossing two lines.
	/// First line presented as 'p11' - start line point and 'p12' - end line point.
	/// Second line presented as 'p21' - start line point and 'p22' - end line point.
	/// If lines is not crossed(lines is same or lines is paralleled) than returns point[FLT_MAX, FLT_MAX]
	static REPoint2 GetLinesCrossPoint(const REPoint2 & p11, 
										const REPoint2 & p12, 
										const REPoint2 & p21, 
										const REPoint2 & p22);
};


#endif /* __RE2DALGORITHM_H__ */



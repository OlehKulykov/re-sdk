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


#include "../../include/recore/RE2DAlgorithm.h"
#include "../../include/recore/REPoint2.h"


REBOOL RE2DAlgorithm::isPointInPolygonFloat32(const REFloat32 pointX, 
											  const REFloat32 pointY, 
											  const REUInt32 numberOfPolygonPoints,
											  const REFloat32 * polygonPointsX,
											  const REFloat32 * polygonPointsY)
{
	if (polygonPointsX && polygonPointsY && (numberOfPolygonPoints > 3)) 
	{
		REFloat32 x1, y1;
		REFloat32 x2, y2;
		REBOOL inside = false;
		
		REFloat32 xold = polygonPointsX[numberOfPolygonPoints - 1];
		REFloat32 yold = polygonPointsY[numberOfPolygonPoints - 1];
		for (REUInt32 i = 0 ; i < numberOfPolygonPoints; i++) 
		{
			REFloat32 xnew = polygonPointsX[i];
			REFloat32 ynew = polygonPointsY[i];
			if (xnew > xold) 
			{
				x1 = xold;
				x2 = xnew;
				y1 = yold;
				y2 = ynew;
			}
			else 
			{
				x1 = xnew;
				x2 = xold;
				y1 = ynew;
				y2 = yold;
			}
			
			if (((xnew < pointX) == (pointX <= xold)) && (((pointY-y1)*(x2-x1)) < ((y2-y1)*(pointX-x1))))
			{
				inside = !inside;
			}
			xold = xnew;
			yold = ynew;
		}
		return inside;
	}
	return false;
}

REBOOL RE2DAlgorithm::isPointInPolygonInt32(const REInt32 pointX, 
											const REInt32 pointY, 
											const REUInt32 numberOfPolygonPoints,
											const REInt32 * polygonPointsX,
											const REInt32 * polygonPointsY)
{
	if (polygonPointsX && polygonPointsY && (numberOfPolygonPoints > 3)) 
	{
		REInt32 x1, y1;
		REInt32 x2, y2;
		REBOOL inside = false;
		
		REInt32 xold = polygonPointsX[numberOfPolygonPoints - 1];
		REInt32 yold = polygonPointsY[numberOfPolygonPoints - 1];
		for (REUInt32 i = 0 ; i < numberOfPolygonPoints; i++) 
		{
			REInt32 xnew = polygonPointsX[i];
			REInt32 ynew = polygonPointsY[i];
			if (xnew > xold) 
			{
				x1 = xold;
				x2 = xnew;
				y1 = yold;
				y2 = ynew;
			}
			else 
			{
				x1 = xnew;
				x2 = xold;
				y1 = ynew;
				y2 = yold;
			}
			
			if (((xnew < pointX) == (pointX <= xold)) && 
				((((REFloat64)pointY-(REFloat64)y1)*((REFloat64)x2-(REFloat64)x1)) < (((REFloat64)y2-(REFloat64)y1)*((REFloat64)pointX-(REFloat64)x1))))
			{
				inside = !inside;
			}
			xold = xnew;
			yold = ynew;
		}
		return inside;
	}
	return false;
}

/// Check equality of two 32 bit floats
REBOOL RE2DAlgorithm::isEqualFloat32(const REFloat32 firstValue, const REFloat32 secondValue)
{
	return (fabsf(firstValue - secondValue) < FLT_EPSILON);	
}

/// Check equality of two 64 bit floats
REBOOL RE2DAlgorithm::isEqualFloat64(const REFloat64 firstValue, const REFloat64 secondValue)
{
	return (fabs(firstValue - secondValue) < DBL_EPSILON);
}

/// Rounds 32 bit float value to number of significant digits: 3.14159(2) => 3.14000
REFloat32 RE2DAlgorithm::roundNumSignificantDigitsFloat32(const REFloat32 float32Value, const REInt32 numberOfDigits)
{
	const REFloat32 p = powf(10.0f, numberOfDigits);	
	return floorf(float32Value * p + 0.5f) / p;
}

/// Rounds 64 bit float value to number of significant digits: 3.14159(2) => 3.14000
REFloat64 RE2DAlgorithm::roundNumSignificantDigitsFloat64(const REFloat64 float64Value, const REInt32 numberOfDigits)
{
	const REFloat64 p = pow(10.0, numberOfDigits);
	return floor(float64Value * p + 0.5) / p;
}

/// Return length of line that presented with two points.
/// ['x1', 'y1'] - start line point and ['x2', 'y2'] - end line point.
REFloat32 RE2DAlgorithm::getLineLengthFloat32(const REFloat32 x1, 
											  const REFloat32 y1, 
											  const REFloat32 x2, 
											  const REFloat32 y2)
{
	const REFloat32 mulX = x1 - x2;
	const REFloat32 mulY = y1 - y2;
	return sqrtf(((mulX * mulX) + (mulY * mulY)));
}

/// Return length of line that presented with two points.
/// 'firstPoint' - start line point and 'secondPoint' - end line point.
REFloat32 RE2DAlgorithm::getLineLengthFloat32(const REPoint2 & firstPoint, const REPoint2 & secondPoint)
{
	return RE2DAlgorithm::getLineLengthFloat32(firstPoint.x, firstPoint.y, secondPoint.x, secondPoint.y);
}

REPoint2 RE2DAlgorithm::getLinesCrossPoint(const REPoint2 & p11, 
										   const REPoint2 & p12, 
										   const REPoint2 & p21, 
										   const REPoint2 & p22)
{
	const REFloat64 Z  = ((p12.y - p11.y) * (p21.x - p22.x)) - ((p21.y - p22.y) * (p12.x - p11.x));
	if (Z == 0) 
	{
		return REPoint2(FLT_MAX, FLT_MAX);
	}
	//double Ca = ((p12.y-p11.y)*(p21.x-p11.x))-((p21.y-p11.y)*(p12.x-p11.x));
	const REFloat64 Cb = ((p21.y - p11.y) * (p21.x - p22.x)) - ((p21.y - p22.y) * (p21.x - p11.x));
	/*
	 if( (Z == 0) && (Ca == 0) && (Cb == 0) ) // прямые совпадают
	 {
	 return cross;
	 }
	 if( Z == 0 ) // прямые параллельны
	 {
	 return cross;
	 }
	 */
	//const double Ua = Ca/Z;
	const REFloat64 Ub = Cb / Z;
	REPoint2 cross;
	cross.x = (REFloat32)(p11.x + ((p12.x - p11.x) * Ub));
	cross.y = (REFloat32)(p11.y + ((p12.y - p11.y) * Ub));
	/*
	 if( (0 <= Ua) && (Ua <= 1) && (0 <= Ub) && (Ub <= 1) )
	 {
	 if ( (Ua == 0)||(Ua == 1)||(Ub == 0)||(Ub == 1) )
	 {
	 //прямые пересекаются в конечных точках отрезков
	 }
	 else
	 {
	 //прямые пересекаются в пределах отрезков
	 }
	 }
	 else
	 {
	 //точка пересечения за пределами отрезков
	 }
	 */
	return cross;
}



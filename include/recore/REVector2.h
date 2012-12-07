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


#ifndef __REVECTOR2_H__
#define __REVECTOR2_H__

#include "RECommonHeader.h"
#include "REPoint2.h"
#include "REMath.h"

#if defined(__ARM_NEON__)
#include <arm_neon.h>
#endif

/// Class of 2D Vector. Starts from [0.0f, 0.0f] and to [x, y]
class __RE_PUBLIC_CLASS_API__ REVector2 
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
#if defined(__ARM_NEON__)
		/// used for arm optimizations
		float32x2_t armNeonVector;
#endif		
		/// array of 'x' and 'y'
		REFloat32 coords[2];
	};
	
	
	/// Vector product.
	/// The result of the vector product of two vectors is a vector perpendicular to these vectors.
	/// That's only in 2D space, this vector is not seen as He [0.0f, 0.0f, Z] - this is the point of intersection in the center and goes.
	///
	/// Векторное произведение векторов.
	/// Результатом векторного произведения двух векторов будет вектор перпендикулярный этим векторам.		
	/// Вот только в 2D пространстве этот вектор не видно т.к. он [0.0f, 0.0f, Z] - это точка в центре пересечения и уходит.
	static REVector2 CrossProduct(const REVector2 & firstVector, const REVector2 & secondVector)
	{
		return REVector2();
	}
	
	REVector2 GetCrossProduct() const
	{
		return REVector2();
	}
	
    static REFloat32 DotProduct(const REVector2 & firstVector, const REVector2 & secondVector)
	{
#if defined(__ARM_NEON__)
		float32x2_t v = vmul_f32(*(float32x2_t *)&firstVector.armNeonVector, *(float32x2_t *)&secondVector.armNeonVector);
		v = vpadd_f32(v, v);
		return vget_lane_f32(v, 0);
#else
		return ( (firstVector.x * secondVector.x) + (firstVector.y * secondVector.y) );
#endif
	}
	
	REFloat32 GetDotProduct(const REVector2 & secondVector) const
	{
		return REVector2::DotProduct((*this), secondVector);
	}
	
	/// Angle between two vectors.
	/// value between 0 and π (in radians) which is 0° and 180°.
	static REFloat32 GetAngleUnsigned(const REVector2 & firstVector, const REVector2 & secondVector)
	{
		REVector2 v1(firstVector);
		v1.Normalize();
		REVector2 v2(secondVector);
		v2.Normalize();
		return acosf(REVector2::DotProduct(v1, v2));
	}

	/// Angle between two vectors.
	/// angle of secondVector relative to firstVector.
	static REFloat32 GetAngleSigned(const REVector2 & firstVector, const REVector2 & secondVector)
	{
		REVector2 v1(firstVector);
		v1.Normalize();
		REVector2 v2(secondVector);
		v2.Normalize();
		return atan2f(v2.y, v2.x) - atan2f(v1.y, v1.x);
	}

	/// Getting a new vector in the opposite direction.
	///
	/// Получение нового вектора с противоположным направлением.
	REVector2 GetInverseVector() const
	{
#if defined(__ARM_NEON__)
		return REVector2(vneg_f32(*(float32x2_t *)&armNeonVector));
#else
		return REVector2(-x, -y);
#endif		
	}
	
	/// Changes in the opposite direction of the vector.
	///
	/// Изменения направление вектора на противоположное
	REVector2 & Inverse()
	{
#if defined(__ARM_NEON__)
		armNeonVector = vneg_f32(*(float32x2_t *)&armNeonVector);
#else
		x = -x;
		y = -y;
#endif		
		return (*this);
	}
	
	/// Converts the vector to vector in the same direction, but with unit length.
	///
	/// Преобразование заданного вектора в вектор в том же направлении, но с единичной длиной.
	REVector2 & Normalize()
	{
#if defined(__ARM_NEON__)
		const float32_t invMag = (1.0f / this->GetMagnitude());
		armNeonVector = vmul_f32(*(float32x2_t *)&armNeonVector, vdup_n_f32(invMag));
#else
		const float invMag = ( 1.0f / this->GetMagnitude() );
		x *= invMag;
		y *= invMag;
#endif		
		return (*this);
	}
	
	/// The length of the vector (or module)
	///
	/// Длина вектора (или модуль)
	const REFloat32 GetMagnitude() const
	{
#if defined(__ARM_NEON__) 
		float32x2_t v = vmul_f32(*(float32x2_t *)&armNeonVector, *(float32x2_t *)&armNeonVector);
		v = vpadd_f32(v, v);
		return sqrtf(vget_lane_f32(v, 0)); 
#else 
		return sqrtf( (x * x) + (y * y) );
#endif 
	}
	
	/// Square of the length of the vector (or module)
	///
	/// Квадрат длины вектора (или модуля)
	const REFloat32 GetSquareMagnitude() const
	{
		return ( (x * x) + (y * y) );
	}
	
	/// Return 'x' coordinate
	const REFloat32 GetX() const { return x; }
	
	/// Return 'y' coordinate
	const REFloat32 GetY() const { return y; }
	
	/// Seting 'x' coordinate
	void SetX(const REFloat32 newX) { x = newX; }
	
	/// Seting 'y' coordinate
	void SetY(const REFloat32 newY) { y = newY; }
	
	/// "this vector" + another vector
	REVector2 & Add(const REVector2 & anotherVector2D)
	{
#if defined(__ARM_NEON__)
		armNeonVector = vadd_f32(*(float32x2_t *)&armNeonVector,
								 *(float32x2_t *)&anotherVector2D.armNeonVector);
#else
		x += anotherVector2D.x;
		y += anotherVector2D.y;
#endif
		return (*this);
	}
	
	/// "this vector" - another vector
	REVector2 & Subtract(const REVector2 & anotherVector2D)
	{
#if defined(__ARM_NEON__)
		armNeonVector = vsub_f32(*(float32x2_t *)&armNeonVector,
								 *(float32x2_t *)&anotherVector2D.armNeonVector);
#else
		x -= anotherVector2D.x;
		y -= anotherVector2D.y;
#endif
		return (*this);
	}
	
	/// "this vector" * another vector
	REVector2 & Multiply(const REVector2 & anotherVector2D)
	{
#if defined(__ARM_NEON__)
		armNeonVector = vmul_f32(*(float32x2_t *)&armNeonVector,
								 *(float32x2_t *)&anotherVector2D.armNeonVector);
#else
		x *= anotherVector2D.x;
		y *= anotherVector2D.y;
#endif		
		return (*this);
	}
	
	/// "this vector" / another vector
	REVector2 & Divide(const REVector2 & anotherVector2D)
	{
		if ((anotherVector2D.x == 0.0f) || (anotherVector2D.y == 0.0f)) 
		{
			x = y = FLT_MAX;
		}
		else
		{
#if defined(__ARM_NEON__)
			float32x2_t *vLeft = (float32x2_t *)&armNeonVector;
			float32x2_t *vRight = (float32x2_t *)&anotherVector2D.armNeonVector;
			float32x2_t estimate = vrecpe_f32(*vRight);    
			estimate = vmul_f32(vrecps_f32(*vRight, estimate), estimate);
			estimate = vmul_f32(vrecps_f32(*vRight, estimate), estimate);
			armNeonVector = vmul_f32(*vLeft, estimate);
#else
			x /= anotherVector2D.x;
			y /= anotherVector2D.y;
#endif
		}
		return (*this);
	}
	
	/// "this vector" + float value (scalar)
	REVector2 & AddScalar(const REFloat32 scalar)
	{
#if defined(__ARM_NEON__) 
		armNeonVector = vadd_f32(*(float32x2_t *)&armNeonVector,
								 vdup_n_f32((float32_t)scalar));
#else 
		x += scalar;
		y += scalar;
#endif	
		return (*this);
	}
	
	/// "this vector" - float value (scalar)
	REVector2 & SubtractScalar(const REFloat32 scalar)
	{
#if defined(__ARM_NEON__)
		armNeonVector = vsub_f32(*(float32x2_t *)&armNeonVector,
								 vdup_n_f32((float32_t)scalar));
#else    
		x -= scalar;
		y -= scalar;
#endif	
		return (*this);
	}
	
	/// "this vector" * float value (scalar)
	REVector2 & MultiplyScalar(const REFloat32 scalar)
	{
#if defined(__ARM_NEON__)
		armNeonVector = vmul_f32(*(float32x2_t *)&armNeonVector,
								 vdup_n_f32((float32_t)scalar));
#else
		x *= scalar;
		y *= scalar;
#endif		
		return (*this);
	}
	
	/// "this vector" / float value (scalar)
	REVector2 & DivideScalar(const REFloat32 scalar)
	{
		if (scalar == 0.0f) 
		{
			x = y = FLT_MAX;
		}
		else
		{
#if defined(__ARM_NEON__)
			float32x2_t values = vdup_n_f32((float32_t)scalar);
			float32x2_t estimate = vrecpe_f32(values);    
			estimate = vmul_f32(vrecps_f32(values, estimate), estimate);
			estimate = vmul_f32(vrecps_f32(values, estimate), estimate);
			armNeonVector = vmul_f32(*(float32x2_t *)&armNeonVector, estimate);
#else
			x /= scalar;
			y /= scalar;
#endif				
		}
		return (*this);
	}
	
	REVector2 & operator+=(const REVector2 & anotherVector2D)
	{
		return this->Add(anotherVector2D);
	}
	REVector2 & operator+=(const REFloat32 scalar)
	{
		return this->AddScalar(scalar);
	}
	
	REVector2 & operator-=(const REVector2 & anotherVector2D)
	{
		return this->Subtract(anotherVector2D);
	}
	REVector2 & operator-=(const REFloat32 scalar)
	{
		return this->SubtractScalar(scalar);
	}
	
	REVector2 & operator*=(const REVector2 & anotherVector2D)
	{
		return this->Multiply(anotherVector2D);
	}
	REVector2 & operator*=(const REFloat32 scalar)
	{
		return this->MultiplyScalar(scalar);
	}
	
	REVector2 & operator/=(const REVector2 & anotherVector2D)
	{
		return this->Divide(anotherVector2D);
	}
	REVector2 & operator/=(const REFloat32 scalar)
	{
		return this->DivideScalar(scalar);
	}
	
	REVector2 operator+(const REVector2 & anotherVector2D) const
	{
		REVector2 resultVector(*this);
		resultVector.Add(anotherVector2D);
		return resultVector;
	}
	REVector2 operator+(const REFloat32 scalar) const
	{
		REVector2 resultVector(*this);
		resultVector.AddScalar(scalar);
		return resultVector;
	}
	
	REVector2 operator-(const REVector2 & anotherVector2D) const
	{
		REVector2 resultVector(*this);
		resultVector.Subtract(anotherVector2D);
		return resultVector;
	}
	REVector2 operator-(const REFloat32 scalar) const
	{
		REVector2 resultVector(*this);
		resultVector.SubtractScalar(scalar);
		return resultVector;
	}
	
	REVector2 operator*(const REVector2 & anotherVector2D) const
	{
		REVector2 resultVector(*this);
		resultVector.Multiply(anotherVector2D);
		return resultVector;
	}
	REVector2 operator*(const REFloat32 scalar) const
	{
		REVector2 resultVector(*this);
		resultVector.MultiplyScalar(scalar);
		return resultVector;
	}
	
	REVector2 operator/(const REVector2 & anotherVector2D) const
	{
		REVector2 resultVector(*this);
		resultVector.Divide(anotherVector2D);
		return resultVector;
	}
	REVector2 operator/(const REFloat32 scalar) const
	{
		REVector2 resultVector(*this);
		resultVector.DivideScalar(scalar);
		return resultVector;
	}
	
	REVector2 operator-() const
	{
		return REVector2(-x, -y);
	}
	
	bool operator==(const REVector2 & anotherVector2D) const
	{
		return (IS_FLOATS_EQUAL(x, anotherVector2D.x) && 
				IS_FLOATS_EQUAL(y, anotherVector2D.y));
	}
	
	bool operator!=(const REVector2 & anotherVector2D) const
	{
		return ((!IS_FLOATS_EQUAL(x, anotherVector2D.x)) || 
				(!IS_FLOATS_EQUAL(y, anotherVector2D.y)));
	}
	
	REVector2 & operator=(const REVector2 & anotherVector2D)
	{
#if defined(__ARM_NEON__)
		armNeonVector = anotherVector2D.armNeonVector;
#else 
		x = anotherVector2D.x;
		y = anotherVector2D.y;
#endif			
		return (*this);
	}
	
	/// Settes vector value from one point coords to another coords.
	/// Can used for creating direction vector from one object coords to another object coords.
	REVector2 & SetFromCoordinates(const REPoint2 & fromPoint, const REPoint2 & toPoint)
	{
#if defined(__ARM_NEON__)
		armNeonVector = vsub_f32(*(float32x2_t *)&toPoint.armNeonPoint,
								 *(float32x2_t *)&fromPoint.armNeonPoint);
#else
		x = (toPoint.x - fromPoint.x);
		y = (toPoint.y - fromPoint.y);
#endif
		return (*this);
	}
	
	/// Settes vector value from one point coords to another coords.
	/// Can used for creating direction vector from one object coords to another object coords.
	REVector2 & SetFromCoordinates(const REFloat32 fromPointX,
								   const REFloat32 fromPointY,
								   const REFloat32 toPointX,
								   const REFloat32 toPointY)
	{
#if defined(__ARM_NEON__)
		REPoint2 to(toPointX, toPointY);
		REPoint2 from(fromPointX, fromPointY);
		armNeonVector = vsub_f32(*(float32x2_t *)&to.armNeonPoint,
								 *(float32x2_t *)&from.armNeonPoint);
#else
		x = (toPointX - fromPointX);
		y = (toPointY - fromPointY);
#endif		
		return (*this);
	}
	
	REVector2 & Set(const REFloat32 newX, const REFloat32 newY)
	{
		x = newX;
		y = newY;
		return (*this);
	}
	
#if defined(__ARM_NEON__)
	REVector2(const float32x2_t & a) : armNeonVector(a) { }
#endif	
	REVector2() : x(0.0f), y(0.0f) { }
	REVector2(const REFloat32 newX, const REFloat32 newY) : x(newX), y(newY) { }
	REVector2(const REVector2 & anotherVector2D) : 
#if defined(__ARM_NEON__)
	armNeonVector(anotherVector2D.armNeonVector) { }
#else 
	x(anotherVector2D.x), y(anotherVector2D.y) { }
#endif	
	~REVector2() { }
};


#endif /* __REVECTOR2_H__ */


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


#ifndef __REVECTOR3_H__
#define __REVECTOR3_H__

#include "RECommonHeader.h"
#include "REPoint3.h"
#include "REMath.h"

#if defined(__ARM_NEON__)
#include <arm_neon.h>
#endif


class __RE_PUBLIC_CLASS_API__ REVector3
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
			
			/// Z coordinate.
			REFloat32 z;
			
#if defined(__ARM_NEON__)
			/// unused variable for NEON vector 4 align
			REFloat32 _unusedZeroValueVariableForArmNeonVector;
#endif			
		};
		
#if defined(__ARM_NEON__) 
		float32x4_t armNeonVector;
		
		/// array of 'x', 'y', 'z'
		REFloat32 coords[4];
#else	
		/// array of 'x', 'y', 'z'
		REFloat32 coords[3];
#endif	
	};
	
	
	/*
	 Векторное произведение векторов
	 Результатом векторного произведения двух векторов будет вектор перпендикулярный этим векторам.		*/
	static REVector3 CrossProduct(const REVector3 & firstVector, const REVector3 & secondVector)
	{
#if defined(__ARM_NEON__)   
		float32x4_t lyzx = { firstVector.y, firstVector.z, firstVector.x, 0.0f };
		float32x4_t lzxy = { firstVector.z, firstVector.x, firstVector.y, 0.0f };
		float32x4_t ryzx = { secondVector.y, secondVector.z, secondVector.x, 0.0f };
		float32x4_t rzxy = { secondVector.z, secondVector.x, secondVector.y, 0.0f };
		return REVector3(vmlsq_f32(vmulq_f32(*(float32x4_t *)&lyzx, *(float32x4_t *)&rzxy), *(float32x4_t *)&lzxy, *(float32x4_t *)&ryzx));
#else		
		return REVector3(( (firstVector.y * secondVector.z) - (firstVector.z * secondVector.y) ), 
						  ( (firstVector.z * secondVector.x) - (firstVector.x * secondVector.z) ), 
						  ( (firstVector.x * secondVector.y) - (firstVector.y * secondVector.x) ));
#endif		
	}
	
	REVector3 GetCrossProduct(const REVector3 & anotherVector3D) const
	{
		return REVector3::CrossProduct((*this), anotherVector3D);
	}
	
	/*
	 dotProduct
	 */
    static REFloat32 DotProduct(const REVector3 & firstVector, const REVector3 & secondVector)
	{
		// NEON is slower than c++ in this case
#if 0//defined(__ARM_NEON__)   
		float32x4_t v = vmulq_f32(*(float32x4_t *)&firstVector.armNeonVector,
								  *(float32x4_t *)&secondVector.armNeonVector);
		float32x2_t v2 = vpadd_f32(vget_low_f32(v), vget_high_f32(v));
		v2 = vpadd_f32(v2, v2);
		return vget_lane_f32(v2, 0);
#else		
		return ( (firstVector.x * secondVector.x) + (firstVector.y * secondVector.y) + (firstVector.z * secondVector.z) );
#endif		
	}
	
	const REFloat32 GetDotProduct(const REVector3 & anotherVector3D) const
	{
		return REVector3::DotProduct((*this), anotherVector3D);
	}
	
	/*
	 Получение нового вектора с противоположным направлением		*/
	REVector3 GetInverseVector() const
	{
#if defined(__ARM_NEON__)   		
		return REVector3(vnegq_f32(*(float32x4_t *)&armNeonVector));
#else		
		return REVector3(-x, -y, -z);
#endif		
	}
	
	/*
	 Изменения направление вектора на противоположное		*/
	REVector3 & Inverse()
	{
#if defined(__ARM_NEON__)   		
		armNeonVector = vnegq_f32(*(float32x4_t *)&armNeonVector);
#else		
		x = -x;
		y = -y;
		z = -z;
#endif		
		return (*this);
	}
	
	/*
	 Преобразование заданного вектора в вектор в том же направлении,
	 но с единичной длиной.			*/
	REVector3 & Normalize()
	{
		return this->MultiplyScalar((1.0f / this->GetMagnitude()));
	}
	
	/*
	 Длина вектора (или модуль)			*/
	const REFloat32 GetMagnitude() const
	{
#if defined(__ARM_NEON__)   		
		float32x4_t v = vmulq_f32(*(float32x4_t *)&armNeonVector,
								  *(float32x4_t *)&armNeonVector);
		float32x2_t v2 = vpadd_f32(vget_low_f32(v), vget_high_f32(v));
		v2 = vpadd_f32(v2, v2);
		return sqrtf(vget_lane_f32(v2, 0));
#else		
		return sqrtf( (x * x) + (y * y) + (z * z) );
#endif		
	}
	
	/*
	 Установить новую длину вектора (или модуль)			*/
	REVector3 & SetMagnitude(const REFloat32 newMagnitude)
	{
		return this->Normalize().MultiplyScalar(newMagnitude);
	}
	
	/*
	 Квадрат длины вектора (или модуля)			*/
	const REFloat32 GetSquareMagnitude() const
	{
#if defined(__ARM_NEON__)   		
		float32x4_t v = vmulq_f32(*(float32x4_t *)&armNeonVector,
								  *(float32x4_t *)&armNeonVector);
		float32x2_t v2 = vpadd_f32(vget_low_f32(v), vget_high_f32(v));
		v2 = vpadd_f32(v2, v2);
		return vget_lane_f32(v2, 0);
#else		
		return ( (x * x) + (y * y) + (z * z) );
#endif		
	}
	
	const REFloat32 GetX() const { return x; }
	const REFloat32 GetY() const { return y; }
	const REFloat32 GetZ() const { return z; }
	
	void SetX(const REFloat32 newX) { x = newX; }
	void SetY(const REFloat32 newY) { y = newY; }
	void SetZ(const REFloat32 newZ) { z = newZ; }
	
	/// "this vector" + another vector
	REVector3 & Add(const REVector3 & anotherVector3D)
	{
#if defined(__ARM_NEON__) 
		armNeonVector = vaddq_f32(*(float32x4_t *)&armNeonVector,
								  *(float32x4_t *)&anotherVector3D.armNeonVector);
#else		
		x += anotherVector3D.x;
		y += anotherVector3D.y;
		z += anotherVector3D.z;
#endif		
		return (*this);
	}
	REVector3 & operator+=(const REVector3 & anotherVector3D)
	{
		return this->Add(anotherVector3D);
	}
	REVector3 & AddScalar(const REFloat32 scalar)
	{
#if defined(__ARM_NEON__) 
		armNeonVector = vaddq_f32(*(float32x4_t *)&armNeonVector,
								  vdupq_n_f32((float32_t)scalar));
#else		
		x += scalar;
		y += scalar;
		z += scalar;
#endif		
		return (*this);
	}
	REVector3 & operator+=(const REFloat32 scalar)
	{
		return this->AddScalar(scalar);
	}
	
	/// "this vector" - another vector
	REVector3 & Subtract(const REVector3 & anotherVector3D)
	{
#if defined(__ARM_NEON__) 
		armNeonVector = vsubq_f32(*(float32x4_t *)&armNeonVector,
								  *(float32x4_t *)&anotherVector3D.armNeonVector);
#else
		x -= anotherVector3D.x;
		y -= anotherVector3D.y;
		z -= anotherVector3D.z;
#endif		
		return (*this);
	}
	REVector3 & operator-=(const REVector3 & anotherVector3D)
	{
		return this->Subtract(anotherVector3D);
	}
	/// "this vector" - float value (scalar)
	REVector3 & SubtractScalar(const REFloat32 scalar)
	{
#if defined(__ARM_NEON__) 
		armNeonVector = vsubq_f32(*(float32x4_t *)&armNeonVector,
								  vdupq_n_f32((float32_t)scalar));
#else		
		x -= scalar;
		y -= scalar;
		z -= scalar;
#endif		
		return (*this);
	}
	REVector3 & operator-=(const REFloat32 scalar)
	{
		return this->SubtractScalar(scalar);
	}
	
	/// "this vector" * another vector
	REVector3 & Multiply(const REVector3 & anotherVector3D)
	{
#if defined(__ARM_NEON__) 
		armNeonVector = vmulq_f32(*(float32x4_t *)&armNeonVector,
								  *(float32x4_t *)&anotherVector3D.armNeonVector);
#else
		x *= anotherVector3D.x;
		y *= anotherVector3D.y;	
		z *= anotherVector3D.z;	
#endif		
		return (*this);
	}
	REVector3 & operator*=(const REVector3 & anotherVector3D)
	{
		return this->Multiply(anotherVector3D);
	}
	/// "this vector" * float value (scalar)
	REVector3 & MultiplyScalar(const REFloat32 scalar)
	{
#if defined(__ARM_NEON__)
		armNeonVector = vmulq_f32(*(float32x4_t *)&armNeonVector,
								  vdupq_n_f32((float32_t)scalar));
#else
		x *= scalar;
		y *= scalar;	
		z *= scalar;
#endif		
		return (*this);
	}
	REVector3 & operator*=(const REFloat32 scalar)
	{
		return this->MultiplyScalar(scalar);
	}
	
	/// "this vector" / another vector
	REVector3 & Divide(const REVector3 & anotherVector3D)
	{
		if ((anotherVector3D.x == 0.0f) || (anotherVector3D.y == 0.0f) || (anotherVector3D.z == 0.0f)) 
		{
			x = y = z = FLT_MAX;
		}
		else
		{
			x /= anotherVector3D.x;
			y /= anotherVector3D.y;
			z /= anotherVector3D.z;
		}
		return (*this);
	}
	REVector3 & operator/=(const REVector3 & anotherVector3D)
	{
		return this->Divide(anotherVector3D);
	}
	/// "this vector" / float value (scalar)
	REVector3 & DivideScalar(const REFloat32 scalar)
	{
		if (scalar == 0.0f) 
		{
			x = y = z = FLT_MAX;
		}
		else
		{
			x /= scalar;
			y /= scalar;
			z /= scalar;
		}
		return (*this);
	}
	REVector3 & operator/=(const REFloat32 scalar)
	{
		return this->DivideScalar(scalar);
	}
	
	REVector3 operator+(const REFloat32 scalar) const
	{
		REVector3 v(*this);
		v.AddScalar(scalar);
		return v;
	}
	
	REVector3 operator+(const REVector3 & anotherVector3D) const
	{
		REVector3 v(*this);
		v.Add(anotherVector3D);
		return v;
	}
	
	REVector3 operator-(const REFloat32 scalar) const
	{
		REVector3 v(*this);
		v.SubtractScalar(scalar);
		return v;
	}
	
	REVector3 operator-(const REVector3 & anotherVector3D) const
	{
		REVector3 v(*this);
		v.Subtract(anotherVector3D);
		return v;
	}
	
	REVector3 operator*(const REVector3 & anotherVector3D) const
	{
		REVector3 v(*this);
		v.Multiply(anotherVector3D);
		return v;
	}
	
	REVector3 operator*(const REFloat32 scalar) const
	{
		REVector3 v(*this);
		v.MultiplyScalar(scalar);
		return v;
	}
	
	REVector3 operator/(const REVector3 & anotherVector3D) const
	{
		REVector3 v(*this);
		v.Divide(anotherVector3D);
		return v;
	}
	
	REVector3 operator/(const REFloat32 scalar) const
	{
		REVector3 v(*this);
		v.DivideScalar(scalar);
		return v;
	}
		
	bool operator==(const REVector3 & anotherVector3D) const
	{
		return (IS_FLOATS_EQUAL(x, anotherVector3D.x) &&
				IS_FLOATS_EQUAL(y, anotherVector3D.y) &&
				IS_FLOATS_EQUAL(z, anotherVector3D.z) );
	}
	
	bool operator!=(const REVector3 & anotherVector3D) const
	{
		return ((!IS_FLOATS_EQUAL(x, anotherVector3D.x)) ||
				(!IS_FLOATS_EQUAL(y, anotherVector3D.y)) ||
				(!IS_FLOATS_EQUAL(z, anotherVector3D.z)));
	}
	
	REVector3 & operator=(const REVector3 & anotherVector3D)
	{
#if defined(__ARM_NEON__) 	
		armNeonVector = anotherVector3D.armNeonVector;
#else		
		x = anotherVector3D.x;
		y = anotherVector3D.y;
		z = anotherVector3D.z;
#endif		
		return (*this);
	}
	
	REVector3 operator-() const
	{
		return REVector3(-x, -y, -z);
	}
	
	REVector3 & Rotate(const REVector3 & arroundVector, const REFloat32 clockwiseRadianAngle)
	{
		const REFloat32 negativeHalfAngle = (-clockwiseRadianAngle / 2.0f);
		const REFloat32 s = sinf(negativeHalfAngle);
	
		REVector3 normalizedVector(arroundVector);
		normalizedVector.Normalize();
		
		REFloat32 x = normalizedVector.x * s;
		REFloat32 y = normalizedVector.y * s;
		REFloat32 z = normalizedVector.z * s;
		
		const REFloat32 w = cosf(negativeHalfAngle);
		const REFloat32 w2 = w * w;
		const REFloat32 x2 = x * x;
		const REFloat32 y2 = y * y;
		const REFloat32 z2 = z * z;
	
		const REFloat32 resX = (x*(w2+x2-y2-z2)) + (y*2.0f*((x*y)+(w*z))) + (z*2.0f*((x*z)-(w*y)));
		const REFloat32 resY = (x*2.0f*((x*y)-(w*z))) + (y*(w2-x2+y2-z2)) + (z*2.0f*((y*z)+(w*x)));
		const REFloat32 resZ = (x*2.0f*((x*z)+(w*y))) + (y*2.0f*((y*z)-(w*x))) + (z*(w2-x2-y2+z2));
		
		x = resX;
		y = resY;
		z = resZ;
		
		return (*this);
	}
	
	/// Settes vector value from one point coords to another coords.
	/// Can used for creating direction vector from one object coords to another object coords.
	REVector3 & SetFromCoordinates(const REPoint3 & fromPoint, const REPoint3 & toPoint)
	{
		x = (toPoint.x - fromPoint.x);
		y = (toPoint.y - fromPoint.y);
		z = (toPoint.z - fromPoint.z);
		return (*this);
	}
	
	/// Settes vector value from one point coords to another coords.
	/// Can used for creating direction vector from one object coords to another object coords.
	REVector3 & SetFromCoordinates(const REFloat32 fromPointX,
								   const REFloat32 fromPointY,
								   const REFloat32 fromPointZ,
								   const REFloat32 toPointX,
								   const REFloat32 toPointY,
								   const REFloat32 toPointZ)
	{
		x = (toPointX - fromPointX);
		y = (toPointY - fromPointY);
		z = (toPointZ - fromPointZ);
		return (*this);
	}
	
	REVector3 & Set(const REFloat32 newX, const REFloat32 newY, const REFloat32 newZ)
	{
		x = newX;
		y = newY;
		z = newZ;	
		return (*this);
	}
	
	REVector3() : x(0.0f), y(0.0f), z(0.0f)
#if defined(__ARM_NEON__) 	
	, _unusedZeroValueVariableForArmNeonVector(0.0f)
#endif	
	{ }
	
	REVector3(const REFloat32 newX, const REFloat32 newY, const REFloat32 newZ) : x(newX), y(newY), z(newZ)
#if defined(__ARM_NEON__) 
	, _unusedZeroValueVariableForArmNeonVector(0.0f)
#endif	
	{ }
	
	REVector3(const REVector3 & anotherVector3D) : x(anotherVector3D.x), y(anotherVector3D.y), z(anotherVector3D.z) 
#if defined(__ARM_NEON__) 	
	, _unusedZeroValueVariableForArmNeonVector(0.0f)
#endif	
	{ }
	
#if defined(__ARM_NEON__) 
	REVector3(const float32x4_t & a) : armNeonVector(a) { _unusedZeroValueVariableForArmNeonVector = 0.0f; }
#endif	
	
	~REVector3() { }
};


#endif /* __REVECTOR3_H__ */ 


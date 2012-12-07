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


#ifndef __REAFFINETRANSFORM_H__
#define __REAFFINETRANSFORM_H__

#include "RECommonHeader.h"
#include "REMath.h"
#include "REMem.h"
#include "IREAffineTransformed.h"

#if defined (__ARM_NEON__)
#include <arm_neon.h>
#endif

class REPoint2;
class RETetragon;

/// Affine transform object
class __RE_PUBLIC_CLASS_API__ REAffineTransform
{
public:
	union
	{
		struct
		{
			/// Value at position [1, 1] in the matrix.
			REFloat32 a;
			
			/// Value at position [1, 2] in the matrix.
			REFloat32 b;
			
			/// Value at position [2, 1] in the matrix.
			REFloat32 c;
			
			/// Value at position [2, 2] in the matrix.
			REFloat32 d;
			
			/// Value at position [3, 1] in the matrix.
			REFloat32 tx;
			
			/// Value at position [3, 2] in the matrix.
			REFloat32 ty;
		};	
		/// Array with transform values. [a, b, c, d, tx, ty]
		REFloat32 arr[6];
	};
	
	/// Return transformed point from 'x', 'y' values of source point.
	REPoint2 GetTransformedPoint(const REFloat32 x, const REFloat32 y) const;
	
	/// Return transformed point from 'point' source point.
	REPoint2 GetTransformedPoint(const REPoint2 & point) const;
	
	/// Transforms 'point'.
	REBOOL TransformPoint(REPoint2 * point) const;
	
	/// Return transformed tetragon from 'tetr' source tetragon.
	RETetragon GetTransformedTetragon(const RETetragon & tetr) const;
	
	/// Transforms tetragon 'tetr'.
	REBOOL TransformTetragon(RETetragon * tetr) const;
	
	/// Concatenetes this transform with another 't2' transform.
	/// A new affine transformation matrix. That is, t’ = this * t2.
	REAffineTransform & Concat(const REAffineTransform & t2)
	{
		REFloat32 arr1[6];
		arr1[0] = (t2.a * a) + (t2.b * c);
        arr1[1] = (t2.a * b) + (t2.b * d);
        arr1[2] = (t2.c * a) + (t2.d * c);
        arr1[3] = (t2.c * b) + (t2.d * d);
        arr1[4] = (t2.tx * a) + (t2.ty * c) + tx;
        arr1[5] = (t2.tx * b) + (t2.ty * d) + ty;
		REMem::Memcpy(arr, arr1, 6 * sizeof(REFloat32));
		return (*this);
	}
	
	/// Concatenetes this transform with another transform values.
	/// A new affine transformation matrix. That is, t’ = this * t2. 
	REAffineTransform & Concat(const REFloat32 tra,
							   const REFloat32 trb,
							   const REFloat32 trc,
							   const REFloat32 trd,
							   const REFloat32 trtx,
							   const REFloat32 trty)
	{
		REFloat32 arr1[6];
		arr1[0] = (tra * a) + (trb * c);
        arr1[1] = (tra * b) + (trb * d);
        arr1[2] = (trc * a) + (trd * c);
        arr1[3] = (trc * b) + (trd * d);
        arr1[4] = (trtx * a) + (trty * c) + tx;
        arr1[5] = (trtx * b) + (trty * d) + ty;
		REMem::Memcpy(arr, arr1, 6 * sizeof(REFloat32));
		return (*this);
	}
	
	/// Translates this for 'x' and 'y' values.
	REAffineTransform & Translate(const REFloat32 x, const REFloat32 y)
	{
		return this->Concat(1.0f, 0.0f, 0.0f, 1.0f, x, y);
	}
	
	/// Rotates this for 'radianAngle' angle in radians.
	REAffineTransform & Rotate(const REFloat32 radianAngle)
	{
		const REFloat32 sinusValue = sinf(radianAngle);
		const REFloat32 cosinusValue = cosf(radianAngle);
		return this->Concat(cosinusValue, sinusValue, -sinusValue, cosinusValue, 0.0f, 0.0f);
	}
	
	/// Rotates this arround center by 'radianAngle' in radians arround 'anchorPointX' and 'anchorPointY' coordinates.
	REAffineTransform & Rotate(const REFloat32 radianAngle, const REFloat32 anchorPointX, const REFloat32 anchorPointY)
	{
		this->Translate(anchorPointX, anchorPointY);
		const REFloat32 sinusValue = sinf(radianAngle);
		const REFloat32 cosinusValue = cosf(radianAngle);
		this->Concat(cosinusValue, sinusValue, -sinusValue, cosinusValue, 0.0f, 0.0f);
		return this->Translate(-anchorPointX, -anchorPointY);
	}
	
	/// Scale object by 'sx' and 'sy' values.
	REAffineTransform & Scale(const REFloat32 sx, const REFloat32 sy)
	{
		return this->Concat(sx, 0.0f, 0.0f, sy, 0.0f, 0.0f);
	}
	
	/// Set transform values to identity.
	REAffineTransform & ToIdentity()
	{
		b = c = tx = ty = 0.0f;
		a = d = 1.0f;
		return (*this);
	}
	
	/// Assign operator
	REAffineTransform & operator=(const REAffineTransform & anotherTransform)
	{
		REMem::Memcpy(arr, anotherTransform.arr, 6 * sizeof(REFloat32));
		return (*this);
	}
	
	/// Concatenetes this transform with another transform.
	REAffineTransform operator*(const REAffineTransform & anotherTransform) const
	{
		REAffineTransform t(*this);
		t.Concat(anotherTransform);
		return t;
	}
	
	/// Returns rotation value in radians.
	const REFloat32 GetRotationRadAngle() const { return atan2f(b, a); }
	
	/// Returns scale 'x' value.
	const REFloat32 GetScaleX() const { return sqrtf(a*a + c*c); }
	
	/// Returns scale 'y' value.
	const REFloat32 GetScaleY() const { return sqrtf(b*b + d*d); }
	
	/// Returns translation 'x' value.
	const REFloat32 GetTranslationX() const { return tx; }
	
	/// Returns translation 'y' value.
	const REFloat32 GetTranslationY() const { return ty; }
	
	/*
	REAffineTransformParamsStruct GetTransformParams() const
	{
		REAffineTransformParamsStruct s;
		s.translateX = this->GetTranslationX();
		s.translateY = this->GetTranslationY();
		s.scaleX = this->GetScaleX();
		s.scaleY = this->GetScaleY();
		s.rotateAngle = this->GetRotationRadAngle();
		return s;
	}
	
	REAffineTransform & SetTransformParams(const REAffineTransformParamsStruct & params)
	{
		this->ToIdentity();
		this->Translate(params.translateX, params.translateY);
		this->Scale(params.scaleX, params.scaleY);
		return this->Rotate(params.rotateAngle);
	}
	*/
	
	/// Contructs transform from existed transform.
	REAffineTransform(const REAffineTransform & tr)
	{
		REMem::Memcpy(arr, tr.arr, 6 * sizeof(REFloat32));	
	}
	
	/// Contructs transform from existed transform values.
	REAffineTransform(const REFloat32 newA,
					  const REFloat32 newB,
					  const REFloat32 newC,
					  const REFloat32 newD,
					  const REFloat32 newTX,
					  const REFloat32 newTY) : 
	a(newA), 
	b(newB), 
	c(newC), 
	d(newD), 
	tx(newTX), 
	ty(newTY) 
	{ }
	
	/// Contructs transform with identity.
	REAffineTransform() : a(1.0f), b(0.0f), c(0.0f), d(1.0f), tx(0.0f), ty(0.0f) { }
	
	/// Desctructor.
	~REAffineTransform() { }
	
#ifdef __RE_DEBUG_MODE__
	void Log() const;
#endif	
	
};


#endif /* __REAFFINETRANSFORM_H__ */


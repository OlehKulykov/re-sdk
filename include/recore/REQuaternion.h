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


#ifndef __REQUATERNION_H__
#define __REQUATERNION_H__

#include "RECommonHeader.h"
#include "REVector3.h"
#include "REMath.h"
#include "REMem.h"

#if defined(__ARM_NEON__)
#include <arm_neon.h>
#endif

class __RE_PUBLIC_CLASS_API__ REQuaternion
{
public:
	union
	{
		struct
		{
			REFloat32 x;
			REFloat32 y;
			REFloat32 z;
			REFloat32 w;
		};
		
#if defined(__ARM_NEON__)
		float32x4_t armNeonQuaternion;
#endif
		
		REFloat32 q[4];
	};
	
	/// length
	const REFloat32 getMagnitude() const
	{
#if defined(__ARM_NEON__)
		float32x4_t v = vmulq_f32(*(float32x4_t *)&armNeonQuaternion, *(float32x4_t *)&armNeonQuaternion);
		float32x2_t v2 = vpadd_f32(vget_low_f32(v), vget_high_f32(v));
		v2 = vpadd_f32(v2, v2);
		return sqrtf(vget_lane_f32(v2, 0)); 
#else
		return sqrtf((q[0] * q[0]) + (q[1] * q[1]) + (q[2] * q[2]) + (q[3] * q[3]));
#endif
	}
	
	REQuaternion & normalize()
	{
		const REFloat32 scale = 1.0f / this->getMagnitude();
#if defined(__ARM_NEON__)
		armNeonQuaternion = vmulq_f32(*(float32x4_t *)&armNeonQuaternion, vdupq_n_f32((float32_t)scale));
#else
		q[0] *= scale;
		q[1] *= scale;
		q[2] *= scale;
		q[3] *= scale;
#endif
		return (*this);
	}
	
	REQuaternion getConjugateQuaternion() const
	{
#if defined(__ARM_NEON__)
		float32x4_t *q = (float32x4_t *)&armNeonQuaternion;
		uint32_t signBit = 0x80000000;
		uint32_t zeroBit = 0x0;
		uint32x4_t mask = vdupq_n_u32(signBit);
		mask = vsetq_lane_u32(zeroBit, mask, 3);
		return REQuaternion(vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(*q), mask)));
#else
		return REQuaternion(-q[0], -q[1], -q[2], q[3]);
#endif
	}
	
	REQuaternion getInverseQuaternion() const
	{
#if defined(__ARM_NEON__)
		float32x4_t * q = (float32x4_t *)&armNeonQuaternion;
		float32x4_t v = vmulq_f32(*q, *q);
		float32x2_t v2 = vpadd_f32(vget_low_f32(v), vget_high_f32(v));
		v2 = vpadd_f32(v2, v2);
		float32_t scale = 1.0f / vget_lane_f32(v2, 0);
		v = vmulq_f32(*q, vdupq_n_f32(scale));
		uint32_t signBit = 0x80000000;
		uint32_t zeroBit = 0x0;
		uint32x4_t mask = vdupq_n_u32(signBit);
		mask = vsetq_lane_u32(zeroBit, mask, 3);
		return REQuaternion(vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(v), mask)));
#else
		const REFloat32 scale = 1.0f / ((q[0] * q[0]) + (q[1] * q[1]) + (q[2] * q[2]) + (q[3] * q[3]));
		return REQuaternion(q[0] * -scale, q[1] * -scale, q[2] * -scale, q[3] * scale);
#endif
	}
	
	REQuaternion & rotate(const REVector3 & vector)
	{
		REQuaternion rotatedQuaternion(vector.x, vector.y, vector.z, 0.0f);
		REQuaternion multiplied(*this);
		multiplied.multiply(rotatedQuaternion);
		multiplied.multiply(this->getInverseQuaternion());
#if defined(__ARM_NEON__)
		armNeonQuaternion = multiplied.armNeonQuaternion;
#else
		REMem::Memcpy(q, multiplied.q, sizeof(REFloat32) * 4);
#endif
		return (*this);
	}
	
	REQuaternion & multiply(const REQuaternion & aq)
	{
		const REFloat32 x1 = (q[3] * aq.q[0]) + (q[0] * aq.q[3]) + (q[1] * aq.q[2]) - (q[2] * aq.q[1]);
		const REFloat32 y1 = (q[3] * aq.q[1]) + (q[1] * aq.q[3]) + (q[2] * aq.q[0]) - (q[0] * aq.q[2]);
		const REFloat32 z1 = (q[3] * aq.q[2]) + (q[2] * aq.q[3]) + (q[0] * aq.q[1]) - (q[1] * aq.q[0]);
		w = (q[3] * aq.q[3]) - (q[0] * aq.q[0]) - (q[1] * aq.q[1]) - (q[2] * aq.q[2]);
		x = x1;
		y = y1;
		z = z1;
		return (*this);
	}
	
	REQuaternion & subtract(const REQuaternion & aq)
	{
#if defined(__ARM_NEON__)
		armNeonQuaternion = vsubq_f32(*(float32x4_t *)&armNeonQuaternion, *(float32x4_t *)&aq.armNeonQuaternion);
#else
		q[0] -= aq.q[0];
		q[1] -= aq.q[1];
		q[2] -= aq.q[2];
		q[3] -= aq.q[3];
#endif	
		return (*this);
	}
	
	REQuaternion & add(const REQuaternion & aq)
	{
#if defined(__ARM_NEON__)
		armNeonQuaternion = vaddq_f32(*(float32x4_t *)&armNeonQuaternion, *(float32x4_t *)&aq.armNeonQuaternion);
#else
		q[0] += aq.q[0];
		q[1] += aq.q[1];
		q[2] += aq.q[2];
		q[3] += aq.q[3];
#endif	
		return (*this);
	}
	
	REQuaternion & operator=(const REQuaternion & aq)
	{
#if defined(__ARM_NEON__)
		armNeonQuaternion = aq.armNeonQuaternion;
#else
		REMem::Memcpy(q, aq.q, sizeof(REFloat32) * 4);
#endif
		return (*this);
	}
	
#if defined(__ARM_NEON__)
	REQuaternion(const float32x4_t & aq) : armNeonQuaternion(aq) {  }
#endif

	REQuaternion(const REQuaternion & aq)
#if defined(__ARM_NEON__)
	: armNeonQuaternion(aq.armNeonQuaternion) { }
#else
	{ REMem::Memcpy(q, aq.q, sizeof(REFloat32) * 4); }
#endif
	
	REQuaternion(const REFloat32 inx, const REFloat32 iny, const REFloat32 inz, const REFloat32 inw) : 
		x(inx), y(iny), z(inz), w(inw) { }
	REQuaternion() { memset(q, 0, sizeof(REFloat32) * 4); }
	~REQuaternion() { }
};


#endif /* __REQUATERNION_H__ */


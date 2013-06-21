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


#ifndef __REMATRIX4_H__
#define __REMATRIX4_H__

#include "RECommonHeader.h"
#include "REMath.h"
#include "REVector3.h"
#include "REMem.h"

#if defined(__ARM_NEON__)
#include <arm_neon.h>
#endif

class REQuaternion;

class __RE_PUBLIC_CLASS_API__ REMatrix4
{
public:
	union 
	{
		struct
		{
			REFloat32 m00, m01, m02, m03;
			REFloat32 m10, m11, m12, m13;
			REFloat32 m20, m21, m22, m23;
			REFloat32 m30, m31, m32, m33;
		};
#if defined(__ARM_NEON__)			
		float32x4x4_t armNeonMatrix;
#endif	
		REFloat32 square[4][4];
		
		/// column-major order
		REFloat32 line[16];
	};
	
	REMatrix4 & multiply(const REMatrix4 & am)
	{
#if defined(__ARM_NEON__)
		float32x4x4_t iMatrixLeft = *(float32x4x4_t *)&armNeonMatrix;
		float32x4x4_t iMatrixRight = *(float32x4x4_t *)&am.armNeonMatrix;
		
		armNeonMatrix.val[0] = vmulq_n_f32(iMatrixLeft.val[0], vgetq_lane_f32(iMatrixRight.val[0], 0));
		armNeonMatrix.val[1] = vmulq_n_f32(iMatrixLeft.val[0], vgetq_lane_f32(iMatrixRight.val[1], 0));
		armNeonMatrix.val[2] = vmulq_n_f32(iMatrixLeft.val[0], vgetq_lane_f32(iMatrixRight.val[2], 0));
		armNeonMatrix.val[3] = vmulq_n_f32(iMatrixLeft.val[0], vgetq_lane_f32(iMatrixRight.val[3], 0));
		
		armNeonMatrix.val[0] = vmlaq_n_f32(armNeonMatrix.val[0], iMatrixLeft.val[1], vgetq_lane_f32(iMatrixRight.val[0], 1));
		armNeonMatrix.val[1] = vmlaq_n_f32(armNeonMatrix.val[1], iMatrixLeft.val[1], vgetq_lane_f32(iMatrixRight.val[1], 1));
		armNeonMatrix.val[2] = vmlaq_n_f32(armNeonMatrix.val[2], iMatrixLeft.val[1], vgetq_lane_f32(iMatrixRight.val[2], 1));
		armNeonMatrix.val[3] = vmlaq_n_f32(armNeonMatrix.val[3], iMatrixLeft.val[1], vgetq_lane_f32(iMatrixRight.val[3], 1));
		
		armNeonMatrix.val[0] = vmlaq_n_f32(armNeonMatrix.val[0], iMatrixLeft.val[2], vgetq_lane_f32(iMatrixRight.val[0], 2));
		armNeonMatrix.val[1] = vmlaq_n_f32(armNeonMatrix.val[1], iMatrixLeft.val[2], vgetq_lane_f32(iMatrixRight.val[1], 2));
		armNeonMatrix.val[2] = vmlaq_n_f32(armNeonMatrix.val[2], iMatrixLeft.val[2], vgetq_lane_f32(iMatrixRight.val[2], 2));
		armNeonMatrix.val[3] = vmlaq_n_f32(armNeonMatrix.val[3], iMatrixLeft.val[2], vgetq_lane_f32(iMatrixRight.val[3], 2));
		
		armNeonMatrix.val[0] = vmlaq_n_f32(armNeonMatrix.val[0], iMatrixLeft.val[3], vgetq_lane_f32(iMatrixRight.val[0], 3));
		armNeonMatrix.val[1] = vmlaq_n_f32(armNeonMatrix.val[1], iMatrixLeft.val[3], vgetq_lane_f32(iMatrixRight.val[1], 3));
		armNeonMatrix.val[2] = vmlaq_n_f32(armNeonMatrix.val[2], iMatrixLeft.val[3], vgetq_lane_f32(iMatrixRight.val[2], 3));
		armNeonMatrix.val[3] = vmlaq_n_f32(armNeonMatrix.val[3], iMatrixLeft.val[3], vgetq_lane_f32(iMatrixRight.val[3], 3));
#else
		REMatrix4 m;
		
		m.line[0]  = line[0] * am.line[0]  + line[4] * am.line[1]  + line[8] * am.line[2]   + line[12] * am.line[3];
		m.line[4]  = line[0] * am.line[4]  + line[4] * am.line[5]  + line[8] * am.line[6]   + line[12] * am.line[7];
		m.line[8]  = line[0] * am.line[8]  + line[4] * am.line[9]  + line[8] * am.line[10]  + line[12] * am.line[11];
		m.line[12] = line[0] * am.line[12] + line[4] * am.line[13] + line[8] * am.line[14]  + line[12] * am.line[15];
		
		m.line[1]  = line[1] * am.line[0]  + line[5] * am.line[1]  + line[9] * am.line[2]   + line[13] * am.line[3];
		m.line[5]  = line[1] * am.line[4]  + line[5] * am.line[5]  + line[9] * am.line[6]   + line[13] * am.line[7];
		m.line[9]  = line[1] * am.line[8]  + line[5] * am.line[9]  + line[9] * am.line[10]  + line[13] * am.line[11];
		m.line[13] = line[1] * am.line[12] + line[5] * am.line[13] + line[9] * am.line[14]  + line[13] * am.line[15];
		
		m.line[2]  = line[2] * am.line[0]  + line[6] * am.line[1]  + line[10] * am.line[2]  + line[14] * am.line[3];
		m.line[6]  = line[2] * am.line[4]  + line[6] * am.line[5]  + line[10] * am.line[6]  + line[14] * am.line[7];
		m.line[10] = line[2] * am.line[8]  + line[6] * am.line[9]  + line[10] * am.line[10] + line[14] * am.line[11];
		m.line[14] = line[2] * am.line[12] + line[6] * am.line[13] + line[10] * am.line[14] + line[14] * am.line[15];
		
		m.line[3]  = line[3] * am.line[0]  + line[7] * am.line[1]  + line[11] * am.line[2]  + line[15] * am.line[3];
		m.line[7]  = line[3] * am.line[4]  + line[7] * am.line[5]  + line[11] * am.line[6]  + line[15] * am.line[7];
		m.line[11] = line[3] * am.line[8]  + line[7] * am.line[9]  + line[11] * am.line[10] + line[15] * am.line[11];
		m.line[15] = line[3] * am.line[12] + line[7] * am.line[13] + line[11] * am.line[14] + line[15] * am.line[15];
		
		(*this) = m;
#endif
		return (*this);
	}
	
	REMatrix4 & add(const REMatrix4 & am)
	{
#if defined(__ARM_NEON__)
		float32x4x4_t iMatrixLeft = *(float32x4x4_t *)&armNeonMatrix;
		float32x4x4_t iMatrixRight = *(float32x4x4_t *)&am.armNeonMatrix;
		
		armNeonMatrix.val[0] = vaddq_f32(iMatrixLeft.val[0], iMatrixRight.val[0]);
		armNeonMatrix.val[1] = vaddq_f32(iMatrixLeft.val[1], iMatrixRight.val[1]);
		armNeonMatrix.val[2] = vaddq_f32(iMatrixLeft.val[2], iMatrixRight.val[2]);
		armNeonMatrix.val[3] = vaddq_f32(iMatrixLeft.val[3], iMatrixRight.val[3]);
#else		
		line[0] += am.line[0];
		line[1] += am.line[1];
		line[2] += am.line[2];
		line[3] += am.line[3];
		line[4] += am.line[4];
		line[5] += am.line[5];
		line[6] += am.line[6];
		line[7] += am.line[7];
		line[8] += am.line[8];
		line[9] += am.line[9];
		line[10] += am.line[10];
		line[11] += am.line[11];
		line[12] += am.line[12];
		line[13] += am.line[13];
		line[14] += am.line[14];
		line[15] += am.line[15];
#endif
		return (*this);
	}
	
	REMatrix4 & subtract(const REMatrix4 & am)
	{
#if defined(__ARM_NEON__)
		float32x4x4_t iMatrixLeft = *(float32x4x4_t *)&armNeonMatrix;
		float32x4x4_t iMatrixRight = *(float32x4x4_t *)&am.armNeonMatrix;
		
		armNeonMatrix.val[0] = vsubq_f32(iMatrixLeft.val[0], iMatrixRight.val[0]);
		armNeonMatrix.val[1] = vsubq_f32(iMatrixLeft.val[1], iMatrixRight.val[1]);
		armNeonMatrix.val[2] = vsubq_f32(iMatrixLeft.val[2], iMatrixRight.val[2]);
		armNeonMatrix.val[3] = vsubq_f32(iMatrixLeft.val[3], iMatrixRight.val[3]);
#else
		line[0] -= am.line[0];
		line[1] -= am.line[1];
		line[2] -= am.line[2];
		line[3] -= am.line[3];
		line[4] -= am.line[4];
		line[5] -= am.line[5];
		line[6] -= am.line[6];
		line[7] -= am.line[7];
		line[8] -= am.line[8];
		line[9] -= am.line[9];
		line[10] -= am.line[10];
		line[11] -= am.line[11];
		line[12] -= am.line[12];
		line[13] -= am.line[13];
		line[14] -= am.line[14];
		line[15] -= am.line[15];
#endif
		return (*this);
	}
	
	REMatrix4 & translate(const REFloat32 tx, const REFloat32 ty, const REFloat32 tz)
	{
		REMatrix4 m(*this);
		line[12] = m.line[0] * tx + m.line[4] * ty + m.line[8] * tz + m.line[12];
		line[13] = m.line[1] * tx + m.line[5] * ty + m.line[9] * tz + m.line[13];
		line[14] = m.line[2] * tx + m.line[6] * ty + m.line[10] * tz + m.line[14];
		return (*this);
	}
	
	REMatrix4 & scale(const REFloat32 sx, const REFloat32 sy, const REFloat32 sz)
	{
#if defined(__ARM_NEON__)
		float32x4x4_t iMatrix = *(float32x4x4_t *)&armNeonMatrix;
		
		armNeonMatrix.val[0] = vmulq_n_f32(iMatrix.val[0], (float32_t)sx);
		armNeonMatrix.val[1] = vmulq_n_f32(iMatrix.val[1], (float32_t)sy);
		armNeonMatrix.val[2] = vmulq_n_f32(iMatrix.val[2], (float32_t)sz);
		armNeonMatrix.val[3] = iMatrix.val[3];
#else
		REMatrix4 m(*this);
		
		line[0] = m.line[0] * sx;
		line[1] = m.line[1] * sx;
		line[2] = m.line[2] * sx;
		line[3] = m.line[3] * sx;
		
		line[4] = m.line[4] * sy;
		line[5] = m.line[5] * sy;
		line[6] = m.line[6] * sy;
		line[7] = m.line[7] * sy;
		
		line[8] = m.line[8] * sz;
		line[9] = m.line[9] * sz;
		line[10] = m.line[10] * sz;
		line[11] = m.line[11] * sz;
#endif
		return (*this);
	}
	
	REMatrix4 & rotate(const REFloat32 radians, const REFloat32 x, const REFloat32 y, const REFloat32 z)
	{
		return this->multiply(REMatrix4::createRotation(radians, x, y, z));
	}
	
	REMatrix4 & rotate(const REFloat32 radians, const REVector3 & axisVector)
	{
		return this->multiply(REMatrix4::createRotation(radians, axisVector.x, axisVector.y, axisVector.z));
	}
	
	REMatrix4 & operator=(const REMatrix4 & anotherMatrix3D)
	{
#if defined(__ARM_NEON__)	
		armNeonMatrix = anotherMatrix3D.armNeonMatrix;
#else		
		REMem::Memcpy(line, anotherMatrix3D.line, sizeof(REFloat32) * 16);
#endif		
		return (*this);
	}
	
	REMatrix4 & toIdentity()
	{
		REMem::Memset(line, 0, sizeof(REFloat32) * 16);
		m00 = m11 = m22 = m33 = 1.0f;
		return (*this);
	}
	
#if defined(__ARM_NEON__)	
	REMatrix4(const float32x4x4_t & am) : armNeonMatrix(am) { }
#endif
	
#if defined(__ARM_NEON__)	
	REMatrix4(const REMatrix4 & am) : armNeonMatrix(am.armNeonMatrix) { }
#else
	REMatrix4(const REMatrix4 & am) { REMem::Memcpy(line, am.line, sizeof(REFloat32) * 16); }
#endif	
	
	REMatrix4() { REMem::Memset(line, 0, sizeof(REFloat32) * 16); }
	~REMatrix4() { }
	
	REMatrix4 & toPerspective(const REFloat32 fovyRadians, 
							   const REFloat32 aspect, 
							   const REFloat32 nearZ, 
							   const REFloat32 farZ)
	{
		REMem::Memset(line, 0, sizeof(REFloat32) * 16);
		const REFloat32 cotan = 1.0f / tanf(fovyRadians / 2.0f);
		m00 = cotan / aspect;
		m11 = cotan;
		const REFloat32 nearZMinFarZ = nearZ - farZ;
		m22 = (farZ + nearZ) / nearZMinFarZ;
		m23 = -1.0f;
		m32 = (2.0f * farZ * nearZ) / nearZMinFarZ;
		return (*this);
	}
	
	static REMatrix4 createPerspective(const REFloat32 fovyRadians, 
										const REFloat32 aspect, 
										const REFloat32 nearZ, 
										const REFloat32 farZ)
	{
		REMatrix4 m;
		m.toPerspective(fovyRadians, aspect, nearZ, farZ);
		return m;
	}
	
	
	REMatrix4 & toFrustum(const REFloat32 left, 
						   const REFloat32 right,
						   const REFloat32 bottom, 
						   const REFloat32 top,
						   const REFloat32 nearZ, 
						   const REFloat32 farZ)
	{
		const REFloat32 ral = right + left;
		const REFloat32 rsl = right - left;
		const REFloat32 tsb = top - bottom;
		const REFloat32 tab = top + bottom;
		const REFloat32 fan = farZ + nearZ;
		const REFloat32 fsn = farZ - nearZ;
		REMem::Memset(line, 0, sizeof(REFloat32) * 16);
		m00 = 2.0f * nearZ / rsl;
		m11 = 2.0f * nearZ / tsb;
		m20 = ral / rsl;
		m21 = tab / tsb;
		m22 = -fan / fsn;
		m23 = -1.0f;
		m32 = (-2.0f * farZ * nearZ) / fsn;
		return (*this);
	}
	
	static REMatrix4 createFrustum(const REFloat32 left, 
									const REFloat32 right,
									const REFloat32 bottom, 
									const REFloat32 top,
									const REFloat32 nearZ, 
									const REFloat32 farZ)
	{
		REMatrix4 m;
		m.toFrustum(left, right, bottom, top, nearZ, farZ);
		return m;
	}
	
	REMatrix4 & toOrtho(const REFloat32 left, 
						 const REFloat32 right,
						 const REFloat32 bottom, 
						 const REFloat32 top,
						 const REFloat32 nearZ, 
						 const REFloat32 farZ)
	{
		REMem::Memset(line, 0, sizeof(REFloat32) * 16);
		const REFloat32 ral = right + left;
		const REFloat32 rsl = right - left;
		const REFloat32 tab = top + bottom;
		const REFloat32 tsb = top - bottom;
		const REFloat32 fan = farZ + nearZ;
		const REFloat32 fsn = farZ - nearZ;
		m00 = 2.0f / rsl;
		m11 = 2.0f / tsb;
		m22 = -2.0f / fsn;
		m30 = -ral / rsl;
		m31 = -tab / tsb;
		m32 = -fan / fsn;
		m33 = 1.0f;
		return (*this);
	}
	
	static REMatrix4 createOrtho(const REFloat32 left, 
								  const REFloat32 right,
								  const REFloat32 bottom, 
								  const REFloat32 top,
								  const REFloat32 nearZ, 
								  const REFloat32 farZ)
	{
		REMatrix4 m;
		m.toOrtho(left, right, bottom, top, nearZ, farZ);
		return m;
	}
	
	REMatrix4 & toLookAt(const REFloat32 eyeX, 
						  const REFloat32 eyeY,
						  const REFloat32 eyeZ,
						  const REFloat32 centerX, 
						  const REFloat32 centerY, 
						  const REFloat32 centerZ,
						  const REFloat32 upX, 
						  const REFloat32 upY, 
						  const REFloat32 upZ)
	{
		REMem::Memset(line, 0, sizeof(REFloat32) * 16);
		REVector3 ev(eyeX, eyeY, eyeZ);
		REVector3 cv(centerX, centerY, centerZ);
		REVector3 uv(upX, upY, upZ);
		
		REVector3 n(ev);
		n += -cv;
		n.normalize();
		
        REVector3 u(REVector3::crossProduct(uv, n));
		u.normalize();
		
        REVector3 v(REVector3::crossProduct(n, u));
		
		m00 = u.x;
		m01 = v.x;
		m02 = n.x;
		
		m10 = u.y;
		m11 = v.y;
		m12 = n.y;
		
		m20 = u.z;
		m21 = v.z;
		m22 = n.z;
		
        m30 = REVector3::dotProduct(-u, ev);
        m31 = REVector3::dotProduct(-v, ev);
        m32 = REVector3::dotProduct(-n, ev);
		m33 = 1.0f;
		return (*this);
	}
	
	static REMatrix4 createLookAt(const REFloat32 eyeX, 
								   const REFloat32 eyeY,
								   const REFloat32 eyeZ,
								   const REFloat32 centerX, 
								   const REFloat32 centerY, 
								   const REFloat32 centerZ,
								   const REFloat32 upX, 
								   const REFloat32 upY, 
								   const REFloat32 upZ)
	{
		REMatrix4 m;
		m.toLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
		return m;
	}
	
	static REMatrix4 createFromQuaternion(const REQuaternion & aq);
	
	static REMatrix4 createRotation(const REFloat32 radians, const REFloat32 x, const REFloat32 y, const REFloat32 z)
	{
		const REFloat32 cosV = cosf(radians);
		const REFloat32 cospV = 1.0f - cosV;
		const REFloat32 sinV = sinf(radians);
		REVector3 n(x,y,z);
		n.normalize();
		REMatrix4 m;
		m.line[0] = cosV + cospV * n.x * n.x;
		m.line[1] = cospV * n.x * n.y + n.z * sinV;
		m.line[2] = cospV * n.x * n.z - n.y * sinV;
		m.line[4] = cospV * n.x * n.y - n.z * sinV;
		m.line[5] = cosV + cospV * n.y * n.y;
		m.line[6] = cospV * n.y * n.z + n.x * sinV;
		m.line[7] = cospV * n.x * n.z + n.y * sinV;
		m.line[8] = cospV * n.y * n.z - n.x * sinV;
		m.line[10] = cosV + cospV * n.z * n.z;
		m.line[15] = 1.0f;
		return m;
	}
	
#ifdef __RE_DEBUG_MODE__
	void log() const;
#endif	
	
};

#endif /* __REMATRIX4_H__ */



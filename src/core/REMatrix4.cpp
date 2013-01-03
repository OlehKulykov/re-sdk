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


#include "../../include/recore/REMatrix4.h"
#include "../../include/recore/REVector3.h"
#include "../../include/recore/REQuaternion.h"
#include "../../include/recore/REMath.h"
#include "../../include/recore/RELog.h"

REMatrix4 REMatrix4::CreateFromQuaternion(const REQuaternion & aq)
{
	REQuaternion n(aq);
	n.Normalize();
	
	const REFloat32 tx = n.x + n.x;
	const REFloat32 ty = n.y + n.y;
	const REFloat32 tz = n.z + n.z;
	const REFloat32 tw = n.w + n.w;

	const REFloat32 tx_nx = tx * n.x;
	const REFloat32 tx_ny = tx * n.y;
	const REFloat32 tx_nz = tx * n.z;
	
	const REFloat32 ty_ny = ty * n.y;
	const REFloat32 ty_nz = ty * n.z;
	
	const REFloat32 tw_nx = tw * n.x;
	const REFloat32 tw_ny = tw * n.y;
	const REFloat32 tw_nz = tw * n.z;
	
	const REFloat32 tz_nz = tz * n.z;
	
	REMatrix4 m;
	
	m.m00 = 1.0f - ty_ny - tz_nz;
	m.m01 = tx_ny + tw_nz;
	m.m02 = tx_nz - tw_ny;

	m.m10 = tx_ny - tw_nz;
	m.m11 = 1.0f - tx_nx - tz_nz;
	m.m12 = ty_nz + tw_nx;

	m.m20 = tx_nz + tw_ny;
	m.m21 = ty_nz - tw_nx;
	m.m22 = 1.0f - tx_nx - ty_ny;

	m.m33 = 1.0f;
	
	return m;
}

#ifdef __RE_DEBUG_MODE__
void REMatrix4::Log() const
{
	RELog::Log("REMatrix4: \n[%f\t %f\t %f\t %f\t]\n[%f\t %f\t %f\t %f\t]\n[%f\t %f\t %f\t %f\t]\n[%f\t %f\t %f\t %f\t]",
			   m00, m01, m02, m03,
			   m10, m11, m12, m13,
			   m20, m21, m22, m23,
			   m30, m31, m32, m33 );
}
#endif	




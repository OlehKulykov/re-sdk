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


#include "../../include/recore/REAffineTransform.h"
#include "../../include/recore/REPoint2.h"
#include "../../include/recore/RETetragon.h"
#include "../../include/recore/RELog.h"


REPoint2 REAffineTransform::GetTransformedPoint(const REFloat32 x, const REFloat32 y) const
{
	const REFloat32 newX = (a * x) + (c * y) + tx;
	const REFloat32 newY = (b * x) + (d * y) + ty;
	return REPoint2(newX, newY);
}

REPoint2 REAffineTransform::GetTransformedPoint(const REPoint2 & point) const
{
	const REFloat32 newX = (a * point.x) + (c * point.y) + tx;
	const REFloat32 newY = (b * point.x) + (d * point.y) + ty;
	return REPoint2(newX, newY);
}

REBOOL REAffineTransform::TransformPoint(REPoint2 * point) const
{
	if (point) 
	{
		const REFloat32 newX = (a * point->x) + (c * point->y) + tx;
		const REFloat32 newY = (b * point->x) + (d * point->y) + ty; 
		point->x = newX;
		point->y = newY;
		return true;
	}
	return false;
}

RETetragon REAffineTransform::GetTransformedTetragon(const RETetragon & tetr) const
{
	RETetragon newTextr;
	for (REUInt32 i = 0; i < 8; i += 2) 
	{
		newTextr.arr[i] = (a * tetr.arr[i]) + (c * tetr.arr[i + 1]) + tx;
		newTextr.arr[i + 1] = (b * tetr.arr[i]) + (d * tetr.arr[i + 1]) + ty;
	}
	return newTextr;
}

REBOOL REAffineTransform::TransformTetragon(RETetragon * tetr) const
{
	if (tetr) 
	{
		for (REUInt32 i = 0; i < 8; i += 2) 
		{
			const REFloat32 newX = (a * tetr->arr[i]) + (c * tetr->arr[i + 1]) + tx;
			const REFloat32 newY = (b * tetr->arr[i]) + (d * tetr->arr[i + 1]) + ty;
			tetr->arr[i] = newX;
			tetr->arr[i + 1] = newY;
		}
		return true;
	}
	return false;
}


#ifdef __RE_DEBUG_MODE__
void REAffineTransform::Log() const
{
	/*
	 REFloat32 a;
	 REFloat32 b;
	 REFloat32 c;
	 REFloat32 d;
	 REFloat32 tx;
	 REFloat32 ty;
	 */
	RELog::Log("REAffineTransform: \na: %f, \tb: %f, \tc: %f, \td: %f, \ttx: %f, \tty: %f", a, b, c, d, tx, ty);
}
#endif	



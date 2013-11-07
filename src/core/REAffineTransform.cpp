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
#include "../../include/recore/RERect.h"
#include "../../include/recore/RETetragon.h"
#include "../../include/recore/RELog.h"
#include "../../include/recore/REString.h"
#include "../../include/recore/private/REStringUtilsPrivate.h"


REPoint2 REAffineTransform::transformedPoint(const REFloat32 x, const REFloat32 y) const
{
	const REFloat32 newX = (a * x) + (c * y) + tx;
	const REFloat32 newY = (b * x) + (d * y) + ty;
	return REPoint2(newX, newY);
}

REPoint2 REAffineTransform::transformedPoint(const REPoint2 & point) const
{
	const REFloat32 newX = (a * point.x) + (c * point.y) + tx;
	const REFloat32 newY = (b * point.x) + (d * point.y) + ty;
	return REPoint2(newX, newY);
}

RETetragon REAffineTransform::transformedTetragon(const RETetragon & tetr) const
{
	RETetragon newTextr;
	for (REUInt32 i = 0; i < 8; i += 2) 
	{
		newTextr.arr[i] = (a * tetr.arr[i]) + (c * tetr.arr[i + 1]) + tx;
		newTextr.arr[i + 1] = (b * tetr.arr[i]) + (d * tetr.arr[i + 1]) + ty;
	}
	return newTextr;
}

RETetragon REAffineTransform::transformedRect(const RERect & rect) const
{
	RETetragon newTextr;
	RETetragon tetr(rect);
	for (REUInt32 i = 0; i < 8; i += 2) 
	{
		newTextr.arr[i] = (a * tetr.arr[i]) + (c * tetr.arr[i + 1]) + tx;
		newTextr.arr[i + 1] = (b * tetr.arr[i]) + (d * tetr.arr[i + 1]) + ty;
	}
	return newTextr;
}

#ifdef __RE_DEBUG_MODE__
void REAffineTransform::log() const
{
	RELog::log("REAffineTransform: \na: %f, \tb: %f, \tc: %f, \td: %f, \ttx: %f, \tty: %f", a, b, c, d, tx, ty);
}
#endif	

REAffineTransform REAffineTransform::fromString(const char * string)
{
	if (string) 
	{
		REAffineTransform t;
		if (REStringUtilsPrivate::readArrayF32(string, t.arr, 6, ';') == 6) 
		{
			return t;
		}
	}
	return REAffineTransform();
}

REAffineTransform REAffineTransform::fromString(const REString & string)
{
	if (string.length() > 0) 
	{
		REAffineTransform t;
		if (REStringUtilsPrivate::readArrayF32(string.UTF8String(), t.arr, 6, ';') == 6) 
		{
			return t;
		}
	}
	return REAffineTransform();
}

REString REAffineTransform::toString(const REAffineTransform & transform)
{
	char buff[128];
	const REUInt32 len = REStringUtilsPrivate::writeArrayF32(transform.arr, buff, 6, ';');
	return REString(buff, len);
}


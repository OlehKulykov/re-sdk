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


#include "../../include/recore/RETetragon.h"
#include "../../include/recore/RERect.h"
#include "../../include/recore/REMath.h"
#include "../../include/recore/REMem.h"
#include "../../include/recore/REString.h"
#include "../../include/recore/private/REStringUtilsPrivate.h"

REPoint2 RETetragon::center() const
{
	const REFloat32 x = topLeftX + ((bottomRightX - topLeftX) / 2.0f);	
	const REFloat32 y = topLeftY + ((bottomRightY - topLeftY) / 2.0f);	
	return REPoint2(x, y);
}

RERect RETetragon::boundRect() const
{
	const REFloat32 x = REMath::min<REFloat32>(topLeftX, topRightX, bottomLeftX, bottomRightX);
	const REFloat32 y = REMath::min<REFloat32>(topLeftY, topRightY, bottomLeftY, bottomRightY);
	
	return RERect(x, 
				  y,
				  REMath::max<REFloat32>(topLeftX, topRightX, bottomLeftX, bottomRightX) - x,
				  REMath::max<REFloat32>(topLeftY, topRightY, bottomLeftY, bottomRightY) - y);
}

RETetragon & RETetragon::operator=(const RERectStruct & anotherRect)
{
	this->set(anotherRect);
	return (*this);
}

RETetragon & RETetragon::operator=(const RERect & anotherRect)
{
	this->set(anotherRect);
	return (*this);
}

RETetragon & RETetragon::operator=(const RETetragon & tetr)
{
	this->set(tetr);
	return (*this);
}

void RETetragon::set(const RERectStruct & rect)
{
	topLeftX = bottomLeftX = rect.x;
	topLeftY = topRightY = rect.y;
	
	topRightX = bottomRightX = (rect.x + rect.width);
	bottomLeftY = bottomRightY = (rect.y + rect.height);
}

void RETetragon::set(const RERect & rect)
{	
	topLeftX = bottomLeftX = rect.x;
	topLeftY = topRightY = rect.y;
	
	topRightX = bottomRightX = (rect.x + rect.width);
	bottomLeftY = bottomRightY = (rect.y + rect.height);
}

void RETetragon::set(const RETetragon & tetr)
{
#if defined(__ARM_NEON__) || defined(HAVE_ARM_NEON_H) 
	armNeonTop = tetr.armNeonTop;
	armNeonBottom = tetr.armNeonBottom;
#else
	memcpy(arr, tetr.arr, 8 * sizeof(REFloat32));
#endif
}

RETetragon RETetragon::fromString(const char * string)
{
	if (string) 
	{
		RETetragon t;
		if (REStringUtilsPrivate::readArrayF32(string, t.arr, 8) == 8) 
		{
			return t;
		}
	}
	return RETetragon();
}

RETetragon RETetragon::fromString(const REString & string)
{
	if (string.length() > 0) 
	{
		RETetragon t;
		if (REStringUtilsPrivate::readArrayF32(string.UTF8String(), t.arr, 8) == 8) 
		{
			return t;
		}
	}
	return RETetragon();
}

REString RETetragon::toString(const RETetragon & tetragon)
{
	char buff[128];
	const REUInt32 len = REStringUtilsPrivate::writeArrayF32(tetragon.arr, buff, 8, ';');
	return REString(buff, len);
}



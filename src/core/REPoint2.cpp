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


#include "../../include/recore/REPoint2.h"
#include "../../include/recore/REString.h"
#include "../../include/recore/private/REStringUtilsPrivate.h"


REPoint2 REPoint2::fromString(const char * string)
{
	if (string) 
	{
		REPoint2 p;
		if (REStringUtilsPrivate::readArrayF32(string, p.coords, 2, ';') == 2) 
		{
			return p;
		}
	}
	return REPoint2();
}

REPoint2 REPoint2::fromString(const REString & string)
{
	if (string.length() > 0) 
	{
		REPoint2 p;
		if (REStringUtilsPrivate::readArrayF32(string.UTF8String(), p.coords, 2, ';') == 2) 
		{
			return p;
		}
	}
	return REPoint2();
}

REString REPoint2::toString(const REPoint2 & point)
{
	char buff[32];
	const REUInt32 len = REStringUtilsPrivate::writeArrayF32(point.coords, buff, 2, ';');
	return REString(buff, len);
}


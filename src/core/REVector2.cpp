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


#include "../../include/recore/REVector2.h"
#include "../../include/recore/REString.h"
#include "../../include/recore/private/REStringUtilsPrivate.h"


REVector2 REVector2::fromString(const char * string)
{
	if (string) 
	{
		REVector2 v;
		if (REStringUtilsPrivate::readArrayF32(string, v.coords, 2, ';') == 2) 
		{
			return v;
		}
	}
	return REVector2();
}

REVector2 REVector2::fromString(const REString & string)
{
	if (string.length() > 0) 
	{
		REVector2 v;
		if (REStringUtilsPrivate::readArrayF32(string.UTF8String(), v.coords, 2, ';') == 2) 
		{
			return v;
		}
	}
	return REVector2();
}

REString REVector2::toString(const REVector2 & vector2)
{
	char buff[32];
	const REUInt32 len = REStringUtilsPrivate::writeArrayF32(vector2.coords, buff, 2, ';');
	return REString(buff, len);
}


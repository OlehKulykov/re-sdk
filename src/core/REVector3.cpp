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


#include "../../include/recore/REVector3.h"
#include "../../include/recore/REString.h"
#include "../../include/recore/private/REStringUtilsPrivate.h"



REVector3 REVector3::fromString(const char * string)
{
	if (string) 
	{
		REVector3 v;
		if (REStringUtilsPrivate::readArrayF32(string, v.coords, 3, ';') == 3) 
		{
			return v;
		}
	}
	return REVector3();
}

REVector3 REVector3::fromString(const REString & string)
{
	if (string.length() > 0) 
	{
		REVector3 v;
		if (REStringUtilsPrivate::readArrayF32(string.UTF8String(), v.coords, 3, ';') == 3) 
		{
			return v;
		}
	}
	return REVector3();
}

REString REVector3::toString(const REVector3 & vector3)
{
	char buff[54];
	const REUInt32 len = REStringUtilsPrivate::writeArrayF32(vector3.coords, buff, 3, ';');
	return REString(buff, len);
}


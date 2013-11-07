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


#include "../../include/recore/RESize.h"
#include "../../include/recore/REString.h"
#include "../../include/recore/private/REStringUtilsPrivate.h"

RESize RESize::fromString(const char * string)
{
	if (string) 
	{
		RESize s;
		if (REStringUtilsPrivate::readArrayF32(string, s.size, 2, ';') == 2) 
		{
			return s;
		}
	}
	return RESize();
}

RESize RESize::fromString(const REString & string)
{
	if (string.length() > 0) 
	{
		RESize s;
		if (REStringUtilsPrivate::readArrayF32(string.UTF8String(), s.size, 2, ';') == 2) 
		{
			return s;
		}
	}
	return RESize();
}

REString RESize::toString(const RESize & size)
{
	char buff[32];
	const REUInt32 len = REStringUtilsPrivate::writeArrayF32(size.size, buff, 2, ';');
	return REString(buff, len);
}



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


#include "../../include/recore/REEdgeInsets.h"
#include "../../include/recore/REString.h"
#include "../../include/recore/private/REStringUtilsPrivate.h"

REEdgeInsets REEdgeInsets::fromString(const char * string)
{
	if (string) 
	{
		REEdgeInsets e;
		if (REStringUtilsPrivate::readArrayF32(string, e.tlbr, 4, ';') == 4) 
		{
			return e;
		}
	}
	return REEdgeInsets();
}

REEdgeInsets REEdgeInsets::fromString(const REString & string)
{
	if (string.length() > 0) 
	{
		REEdgeInsets e;
		if (REStringUtilsPrivate::readArrayF32(string.UTF8String(), e.tlbr, 4, ';') == 4) 
		{
			return e;
		}
	}
	return REEdgeInsets();
}

REString REEdgeInsets::toString(const REEdgeInsets & edgeInsets)
{
	char buff[64];
	const REUInt32 len = REStringUtilsPrivate::writeArrayF32(edgeInsets.tlbr, buff, 4, ';');
	return REString(buff, len);		
}


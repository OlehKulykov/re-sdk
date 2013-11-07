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


#include "../../include/recore/REQuaternion.h"
#include "../../include/recore/REString.h"
#include "../../include/recore/private/REStringUtilsPrivate.h"


REQuaternion REQuaternion::fromString(const char * string)
{
	if (string) 
	{
		REQuaternion q;
		if (REStringUtilsPrivate::readArrayF32(string, q.q, 4, ';') == 4) 
		{
			return q;
		}
	}
	return REQuaternion();
}

REQuaternion REQuaternion::fromString(const REString & string)
{
	if (string.length() > 0) 
	{
		REQuaternion q;
		if (REStringUtilsPrivate::readArrayF32(string.UTF8String(), q.q, 4, ';') == 4) 
		{
			return q;
		}
	}
	return REQuaternion();
}

REString REQuaternion::toString(const REQuaternion & quaternion)
{
	char buff[64];
	const REUInt32 len = REStringUtilsPrivate::writeArrayF32(quaternion.q, buff, 4, ';');
	return REString(buff, len);
}

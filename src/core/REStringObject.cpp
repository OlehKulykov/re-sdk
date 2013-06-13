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


#include "../../include/recore/REStringObject.h"

#if defined(__RE_OS_BADA__) || defined(__RE_OS_LINUX__)
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <wctype.h>
#endif /* __RE_OS_BADA__ */

const REUInt32 REStringObject::GetClassIdentifier() const
{
	return REStringObject::ClassIdentifier();
}

const REUInt32 REStringObject::ClassIdentifier()
{
	static const REUInt32 clasIdentif = REMD5Generator::GenerateFromString("REStringObject");
	return clasIdentif;
}

REBOOL REStringObject::IsImplementsClass(const REUInt32 classIdentifier) const
{
	return ((REStringObject::ClassIdentifier() == classIdentifier) || REObject::IsImplementsClass(classIdentifier) ||
			(REMD5Generator::GenerateFromString("REString") == classIdentifier));
}

REBOOL REStringObject::IsEqual(REObject * anotherObject)
{
	if (REObject::IsEqual(anotherObject)) { return true; }
	
	if (anotherObject) 
	{
		if (anotherObject->GetClassIdentifier() == REStringObject::ClassIdentifier()) 
		{
			REStringObject * anotherString = (REStringObject*)anotherObject;
			return REString::IsEqual((*anotherString));
		}
	}
	return false;
}

REStringObject::REStringObject(const wchar_t * wideStringValue) : REString(wideStringValue), REObject()
{
	
}

REStringObject::REStringObject(const char * charsStringValue) : REString(charsStringValue), REObject()
{
	
}

REStringObject::REStringObject(const char * charsStringValue, const REUInt32 stringLength) : REString(charsStringValue, stringLength), REObject()
{
	
}

REStringObject::REStringObject(const REString & anotherString) : REString(anotherString), REObject()
{
	
}

REStringObject::REStringObject(const REStringObject & anotherString) : REString(anotherString.UTF8String()), REObject()
{
	
}

REStringObject::REStringObject() : REString(), REObject()
{
	
}

REStringObject::~REStringObject()
{
	
}



REStringObject * REStringObject::CreateWithWideChars(const wchar_t * wideStringValue)
{
	REStringObject * newStr = new REStringObject(wideStringValue);
	return newStr;
}

REStringObject * REStringObject::CreateWithChars(const char * charsStringValue)
{
	REStringObject * newStr = new REStringObject(charsStringValue);
	return newStr;
}

REStringObject * REStringObject::CreateWithCharsAndLen(const char * charsStringValue, const REUInt32 stringLength)
{
	REStringObject * newStr = new REStringObject(charsStringValue, stringLength);
	return newStr;
}

REStringObject * REStringObject::CreateWithString(const REString & anotherString)
{
	REStringObject * newStr = new REStringObject(anotherString);
	return newStr;
}

REStringObject * REStringObject::CreateWithStringObject(const REStringObject * anotherString)
{
	REStringObject * newStr = NULL;
	if (anotherString) 
	{
		newStr = new REStringObject(*anotherString);
	}
	else
	{
		newStr = new REStringObject();
	}
	return newStr;
}


REStringObject * REStringObject::CreateWithFormatChars(const char * format, ...)
{
	if (format) 
	{
		REStringObject * newStr = new REStringObject();
		if (newStr) 
		{
			va_list args;
			va_start(args, format);
			char strBuff[512];
			int writed = vsprintf(strBuff, format, args);
			if (writed > 0)
			{
				newStr->AppendWithLen(strBuff, (REUInt32)writed);
			}
			va_end(args);
			
			return newStr;
		}
	}
	return NULL;
}


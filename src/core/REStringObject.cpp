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

const REUInt32 REStringObject::getClassIdentifier() const
{
	return REStringObject::classIdentifier();
}

const REUInt32 REStringObject::classIdentifier()
{
	static const REUInt32 clasIdentif = REMD5Generator::generateFromString("REStringObject");
	return clasIdentif;
}

REBOOL REStringObject::isImplementsClass(const REUInt32 classIdentifier) const
{
	return ((REStringObject::classIdentifier() == classIdentifier) || REObject::isImplementsClass(classIdentifier) ||
			(REMD5Generator::generateFromString("REString") == classIdentifier));
}

REBOOL REStringObject::isEqual(REObject * anotherObject)
{
	if (REObject::isEqual(anotherObject)) { return true; }
	
	if (anotherObject) 
	{
		if (anotherObject->getClassIdentifier() == REStringObject::classIdentifier()) 
		{
			REStringObject * anotherString = (REStringObject*)anotherObject;
			return REMutableString::isEqual((*anotherString));
		}
	}
	return false;
}

REStringObject::REStringObject(const wchar_t * wideStringValue) : REMutableString(wideStringValue), REObject()
{
	
}

REStringObject::REStringObject(const char * charsStringValue) : REMutableString(charsStringValue), REObject()
{
	
}

REStringObject::REStringObject(const char * charsStringValue, const REUInt32 stringLength) : REMutableString(charsStringValue, stringLength), REObject()
{
	
}

REStringObject::REStringObject(const REString & anotherString) : REMutableString(anotherString), REObject()
{
	
}

REStringObject::REStringObject(const REStringObject & anotherString) : REMutableString(anotherString.getChars(), anotherString.getLength()), REObject()
{
	
}

REStringObject::REStringObject() : REMutableString(), REObject()
{
	
}

REStringObject::~REStringObject()
{
	
}



REStringObject * REStringObject::createWithWideChars(const wchar_t * wideStringValue)
{
	REStringObject * newStr = new REStringObject(wideStringValue);
	return newStr;
}

REStringObject * REStringObject::createWithChars(const char * charsStringValue)
{
	REStringObject * newStr = new REStringObject(charsStringValue);
	return newStr;
}

REStringObject * REStringObject::createWithCharsAndLen(const char * charsStringValue, const REUInt32 stringLength)
{
	REStringObject * newStr = new REStringObject(charsStringValue, stringLength);
	return newStr;
}

REStringObject * REStringObject::createWithString(const REString & anotherString)
{
	REStringObject * newStr = new REStringObject(anotherString);
	return newStr;
}

REStringObject * REStringObject::createWithStringObject(const REStringObject * anotherString)
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


REStringObject * REStringObject::createWithFormatChars(const char * format, ...)
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
				newStr->append(strBuff, (REUInt32)writed);
			}
			va_end(args);
			
			return newStr;
		}
	}
	return NULL;
}


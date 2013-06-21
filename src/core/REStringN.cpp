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


#include "../../include/recore/REStringN.h"
#include "../../include/recore/REWideStringN.h"
#include "../../include/recore/REMutableStringN.h"

#include "../../include/recore/private/REStringUtilsPrivate.h"

REStringN & REStringN::operator=(const char * utf8String)
{
	this->setFromUTF8String(utf8String,
							RENotFound,
							REStringTypeUTF8);
	
	return (*this);
}

REStringN & REStringN::operator=(const wchar_t * wideString)
{
	this->setFromWideString(wideString, 
							RENotFound,
							REStringTypeUTF8);
	
	return (*this);
}

REStringN & REStringN::operator=(const REWideStringN & anotherString)
{
	this->setFromWideString(anotherString.getWideChars(), 
							anotherString.getLength(), 
							REStringTypeUTF8);
	return (*this);
}

REStringN & REStringN::operator=(const REStringN & anotherString)
{
	_p = anotherString._p;
	return (*this);
}

REStringN & REStringN::operator=(const REMutableStringN & anotherString)
{
	_p = REStringUtilsPrivate::makeCopy(anotherString._p);
	return (*this);
}

REMutableStringN REStringN::getMutableString() const
{
	return REMutableStringN( REStringUtilsPrivate::makeCopy(_p) );
}

REWideStringN REStringN::getWideString() const
{
	return REWideStringN( REStringUtilsPrivate::getWideFromUTF8(_p) );
}

const char * REStringN::getChars() const
{
	const REBuffer * b = _p;
	return b ? (const char *)b->getBuffer() : NULL;
}

const REUInt32 REStringN::getLength() const
{
	return REStringUtilsPrivate::stringLengthFromUTF8Buffer(_p);
}

REBOOL REStringN::isContainsNonASCII() const
{
	const char * ch = (const char *)this->getChars();
	if (ch)
	{
		while (*ch) 
		{
			const wchar_t u = (*ch);
			if ( u >= ((char)0x80) )
			{
				return true;
			}
			ch++;
		}
	}
	return false;
}

REBOOL REStringN::isContaines(const char * utf8String) const
{
	if (utf8String)
	{
		const char * thisUTF8String = this->getChars();
		if (thisUTF8String)
		{
			return (strstr(thisUTF8String, utf8String) != 0);
		}
	}
	return false;
}

REBOOL REStringN::isContaines(const wchar_t * wideString) const
{
	if (wideString)
	{
		REPtr<REBuffer> utf8Buff = REStringUtilsPrivate::getUTF8FromWide(wideString, 
																		 RENotFound);
		
		const REUInt32 len = REStringUtilsPrivate::stringLengthFromUTF8Buffer(utf8Buff);
		if (len)
		{
			return this->isContaines((const char *)utf8Buff->getBuffer());
		}
	}
	return false;
}

REBOOL REStringN::isDigit() const
{
	const char * ch = this->getChars();
	if (ch)
	{
		REUInt32 processed = 0;
		while (*ch)
		{
			const int dig = *ch++;
			processed++;
			if (!isdigit(dig))
			{
				if ((dig != '+') || (dig != '-') || (dig != '.'))
				{
					return false;
				}
			}
		}
		return (processed > 0);
	}
	return false;
}

REStringN::REStringN() : REStringBase()
{

}

REStringN::REStringN(const wchar_t * wideString, const REUInt32 wideStringLength) :
	REStringBase(wideString, wideStringLength, REStringTypeUTF8)
{
	
}

REStringN::REStringN(const char * utf8String, const REUInt32 utf8StringLength) :
	REStringBase(utf8String, utf8StringLength, REStringTypeUTF8)
{
	
}

REStringN::REStringN(const REWideStringN & anotherString) : 
	REStringBase(anotherString.getWideChars(), anotherString.getLength(), REStringTypeUTF8)
{
	
}

REStringN::REStringN(const REStringN & anotherString) : 
	REStringBase(anotherString._p)
{
	
}

REStringN::REStringN(const REMutableStringN & anotherString) :
	REStringBase(anotherString.getChars(), anotherString.getLength(), REStringTypeUTF8)
{
	
}

REStringN::REStringN(const REPtr<REBuffer> & utf8StringBuffer) :
	REStringBase(utf8StringBuffer)
{
	
}

REStringN::~REStringN()
{
	
}

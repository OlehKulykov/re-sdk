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


#include "../../include/recore/REMutableStringN.h"
#include "../../include/recore/REWideStringN.h"
#include "../../include/recore/REStringBase.h"

#include "../../include/recore/private/REStringUtilsPrivate.h"

#include <stdarg.h>

REMutableStringN & REMutableStringN::operator=(const char * utf8String)
{
	_p = REStringUtilsPrivate::newBuffForUTF8String(utf8String);
	return (*this);
}

REMutableStringN & REMutableStringN::operator=(const wchar_t * wideString)
{
	_p = REStringUtilsPrivate::getUTF8FromWide(wideString, 
											   REStringUtilsPrivate::wideStringLength(wideString));
	return (*this);
}

REMutableStringN & REMutableStringN::operator=(const REWideStringN & anotherString)
{
	_p = REStringUtilsPrivate::getUTF8FromWide(anotherString.getWideChars(),
											   anotherString.getLength());
	return (*this);
}

REMutableStringN & REMutableStringN::operator=(const REStringN & anotherString)
{
	_p = REStringUtilsPrivate::newBuffForUTF8String(anotherString.getChars(),
													anotherString.getLength());
	return (*this);
}

REMutableStringN & REMutableStringN::operator=(const REMutableStringN & anotherString)
{
	_p = REStringUtilsPrivate::makeCopy(anotherString._p);
	return (*this);
}

/// Conserts string ti it's lower presentation.
REMutableStringN & REMutableStringN::toLower()
{
	if (_p.isNotEmpty())
	{
		if (this->isContainsNonASCII())
		{
			REWideStringN wide(*this);
			const REUInt32 len = wide.getLength();
			if (len > 0)
			{
				wchar_t * s = (wchar_t *)wide.getWideChars();
				while (*s) 
				{
					*s = (wchar_t)towlower(*s);
					s++;
				}
				this->setFromWideString(s, len, REStringTypeUTF8);
			}
		}
		else
		{
			char * s = (char *)this->getChars();
			while (*s) 
			{
				*s = (char)tolower(*s);
				s++;
			}
		}
	}
	return (*this);
}

/// Conserts string ti it's uper presentation.
REMutableStringN & REMutableStringN::toUpper()
{
	if (_p.isNotEmpty())
	{
		if (this->isContainsNonASCII())
		{
			REWideStringN wide(*this);
			const REUInt32 len = wide.getLength();
			if (len > 0)
			{
				wchar_t * s = (wchar_t *)wide.getWideChars();
				while (*s) 
				{
					*s = (wchar_t)towupper(*s);
					s++;
				}
				this->setFromWideString(s, len, REStringTypeUTF8);
			}
		}
		else
		{
			char * s = (char *)this->getChars();
			while (*s) 
			{
				*s = (char)toupper(*s);
				s++;
			}
		}
	}
	return (*this);
}

/// Appends another UTF8 string.
REMutableStringN & REMutableStringN::append(const char * utf8String, 
											const REUInt32 utf8StringLength)
{
	const REUInt32 len = REStringUtilsPrivate::actualUTF8StringLength(utf8String, utf8StringLength);
	if (len)
	{
		const REUInt32 thisLen = this->getLength();
		const REUInt32 newLen = len + thisLen;
		REBuffer * newBuffer = new REBuffer();
		if (newBuffer)
		{
			if (newBuffer->resize(newLen + 1, false))
			{
				char * newData = (char *)newBuffer->getBuffer();
				if (thisLen > 0)
				{
					memcpy(newData, this->getChars(), thisLen);
				}
				memcpy(&newData[thisLen], utf8String, len);
				newData[newLen] = 0;
				_p = REPtr<REBuffer>(newBuffer);
				return (*this);
			}
			delete newBuffer;
		}
	}
	return (*this);
}

/// Appends another wide char string.
REMutableStringN & REMutableStringN::append(const wchar_t * wideString, 
											const REUInt32 wideStringLength)
{
	REStringN utf8(wideString, wideStringLength);
	if (utf8.isNotEmpty())
	{
		return this->append((const char *)utf8.getChars(), utf8.getLength());
	}
	return (*this);
}

REMutableStringN & REMutableStringN::appendFormat(const char * format, ...)
{
	if (format)
	{
		va_list args;
		va_start(args, format);
		char strBuff[512];
		const int writed = vsprintf(strBuff, format, args);
		if (writed > 0)
		{
			strBuff[writed] = 0;
			this->append((const char *)strBuff, (REUInt32)writed);
		}
		va_end(args);
	}
	return (*this);
}

void REMutableStringN::replaceWithLen(const char * charsStringValue, 
									  const char * withCharsStringValue, 
									  const REUInt32 firstLen, 
									  const REUInt32 secondLen)
{
	if (firstLen) 
	{
		const REUInt32 length = this->getLength();
		REUInt32 ocurencesCount = 0;
		char * mainString = (char *)_p->getBuffer();
		while ( (mainString = strstr(mainString, charsStringValue)) ) 
		{
			ocurencesCount++;
			mainString += firstLen;
		}
		
		if (ocurencesCount) 
		{
			const REUInt32 resultLen = (length + (ocurencesCount * secondLen)) - (ocurencesCount * firstLen);
			REBuffer * newBuffer = new REBuffer();
			if (!newBuffer) return;
			if (!newBuffer->resize(resultLen + 1, false)) return;
			
			char * newData = (char *)newBuffer->getBuffer();
			mainString = (char *)_p->getBuffer();
			char * srcString = (char *)_p->getBuffer();
			char * dstString = newData;
			while ( (mainString = strstr(mainString, charsStringValue)) ) 
			{
				while (srcString != mainString) 
				{
					*dstString++ = *srcString++;
				}
				REUByte * addString = (REUByte *)withCharsStringValue;
				REUInt32 addLen = secondLen;
				while (addLen) 
				{
					*dstString++ = *addString++;
					addLen--;
				}
				mainString += firstLen;
				srcString += firstLen;
			}
			while (*srcString) 
			{
				*dstString++ = *srcString++;
			}
			newData[resultLen] = 0;
			_p = REPtr<REBuffer>(newBuffer);
		}
	}
}


REMutableStringN & REMutableStringN::replace(const char * utf8String, 
											 const char * withUTF8StringOrNULL)
{
	if (this->isNotEmpty())
	{
		const REUInt32 firstLen = REStringUtilsPrivate::UTF8StringLength(utf8String);
		const REUInt32 secondLen = REStringUtilsPrivate::UTF8StringLength(withUTF8StringOrNULL);
		this->replaceWithLen(utf8String, withUTF8StringOrNULL, firstLen, secondLen);
	}
	return (*this);
}

REMutableStringN & REMutableStringN::replace(const wchar_t * wideString, 
											 const wchar_t * withWideStringOrNULL)
{
	if (this->isNotEmpty())
	{
		REStringN s1(wideString);
		REStringN s2(withWideStringOrNULL);
		this->replaceWithLen(s1.getChars(), s2.getChars(), s1.getLength(), s2.getLength());
	}
	return (*this);
}

REMutableStringN & REMutableStringN::appendPathComponent(const char * pComponent)
{
	_p = REStringUtilsPrivate::getAppendedWithPathComponent(_p, pComponent, RENotFound);
	return (*this);
}

REMutableStringN::REMutableStringN() : 
	REStringN()
{
	
}

REMutableStringN::REMutableStringN(const char * utf8String, 
								   const REUInt32 utf8StringLength) :
	REStringN(utf8String, utf8StringLength)
{
	
}

REMutableStringN::REMutableStringN(const wchar_t * wideString, 
								   const REUInt32 wideStringLength) :
	REStringN(wideString, wideStringLength)
{
	
}

REMutableStringN::REMutableStringN(const REWideStringN & anotherString) : 
	REStringN(anotherString.getWideChars(), anotherString.getLength())
{
	
}

REMutableStringN::REMutableStringN(const REStringN & anotherString) : 
	REStringN(anotherString.getChars(), anotherString.getLength())
{
	
}

REMutableStringN::REMutableStringN(const REMutableStringN & anotherString) : 
	REStringN(anotherString.getChars(), anotherString.getLength())
{

}

REMutableStringN::REMutableStringN(const REPtr<REBuffer> & utf8StringBuffer) : 
	REStringN(utf8StringBuffer)
{
	
}

REMutableStringN::~REMutableStringN()
{
	
}


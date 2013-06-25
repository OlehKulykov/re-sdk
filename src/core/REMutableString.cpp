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


#include "../../include/recore/REMutableString.h"
#include "../../include/recore/REWideString.h"
#include "../../include/recore/REStringBase.h"

#include "../../include/recore/private/REStringUtilsPrivate.h"

#include <stdarg.h>

REMutableString & REMutableString::operator=(const char * utf8String)
{
	_p = REStringUtilsPrivate::newBuffForUTF8String(utf8String);
	return (*this);
}

REMutableString & REMutableString::operator=(const wchar_t * wideString)
{
	_p = REStringUtilsPrivate::getUTF8FromWide(wideString, 
											   REStringUtilsPrivate::wideStringLength(wideString));
	return (*this);
}

REMutableString & REMutableString::operator=(const REWideString & anotherString)
{
	_p = REStringUtilsPrivate::getUTF8FromWide(anotherString.getWideChars(),
											   anotherString.getLength());
	return (*this);
}

REMutableString & REMutableString::operator=(const REString & anotherString)
{
	_p = REStringUtilsPrivate::newBuffForUTF8String(anotherString.getChars(),
													anotherString.getLength());
	return (*this);
}

REMutableString & REMutableString::operator=(const REMutableString & anotherString)
{
	_p = REStringUtilsPrivate::makeCopy(anotherString._p);
	return (*this);
}

/// Conserts string ti it's lower presentation.
REMutableString & REMutableString::toLower()
{
	if (_p.isNotEmpty())
	{
		if (this->isContainsNonASCII())
		{
			REWideString wide(*this);
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
REMutableString & REMutableString::toUpper()
{
	if (_p.isNotEmpty())
	{
		if (this->isContainsNonASCII())
		{
			REWideString wide(*this);
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
REMutableString & REMutableString::append(const char * utf8String, 
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
REMutableString & REMutableString::append(const wchar_t * wideString, 
											const REUInt32 wideStringLength)
{
	REString utf8(wideString, wideStringLength);
	if (utf8.isNotEmpty())
	{
		return this->append((const char *)utf8.getChars(), utf8.getLength());
	}
	return (*this);
}

REMutableString & REMutableString::appendFormat(const char * format, ...)
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

void REMutableString::replaceWithLen(const char * charsStringValue, 
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


REMutableString & REMutableString::replace(const char * utf8String, 
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

REMutableString & REMutableString::replace(const wchar_t * wideString, 
											 const wchar_t * withWideStringOrNULL)
{
	if (this->isNotEmpty())
	{
		REString s1(wideString);
		REString s2(withWideStringOrNULL);
		this->replaceWithLen(s1.getChars(), s2.getChars(), s1.getLength(), s2.getLength());
	}
	return (*this);
}

REMutableString & REMutableString::appendPathComponent(const char * pComponent)
{
	_p = REStringUtilsPrivate::getAppendedWithPathComponent(_p, pComponent);
	return (*this);
}

REMutableString & REMutableString::removeLastPathComponent()
{
	_p = REStringUtilsPrivate::getRemovedLastPathComponent(_p);
	return (*this);
}

REMutableString & REMutableString::removePathExtension()
{
	_p = REStringUtilsPrivate::getRemovedPathExtension(_p);
	return (*this);
}

REMutableString::REMutableString() : 
	REString()
{
	
}

REMutableString::REMutableString(const char * utf8String, 
								   const REUInt32 utf8StringLength) :
	REString(utf8String, utf8StringLength)
{
	
}

REMutableString::REMutableString(const wchar_t * wideString, 
								   const REUInt32 wideStringLength) :
	REString(wideString, wideStringLength)
{
	
}

REMutableString::REMutableString(const REWideString & anotherString) : 
	REString(anotherString.getWideChars(), anotherString.getLength())
{
	
}

REMutableString::REMutableString(const REString & anotherString) : 
	REString(anotherString.getChars(), anotherString.getLength())
{
	
}

REMutableString::REMutableString(const REMutableString & anotherString) : 
	REString(anotherString.getChars(), anotherString.getLength())
{

}

REMutableString::REMutableString(const REPtr<REBuffer> & utf8StringBuffer) : 
	REString(utf8StringBuffer)
{
	
}

REMutableString::~REMutableString()
{
	
}


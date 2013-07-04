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


#ifndef __REString_H__ 
#define __REString_H__ 

#include "RECommonHeader.h"
#include "REPtr.h"
#include "REBuffer.h"
#include "REStringBase.h"

class REMutableString;
class REWideString;

class __RE_PUBLIC_CLASS_API__ REString : public REStringBase
{
public:
	REMutableString getMutableString() const;
	REWideString getWideString() const;
	
	const char * getChars() const;
	const REUInt32 getLength() const;
	
	REBOOL isContainsNonASCII() const;
	
	REBOOL isContaines(const char * utf8String) const;
	
	REBOOL isContaines(const wchar_t * wideString) const;
	
	REBOOL isDigit() const;
	
	REBOOL isEqual(const REWideString & anotherString) const;
	REBOOL isEqual(const REString & anotherString) const;
	REBOOL isEqual(const REMutableString & anotherString) const;
	REBOOL isEqual(const char * utf8String, const REUInt32 utf8StringLength = RENotFound) const;
	REBOOL isEqual(const wchar_t * wideString, const REUInt32 wideStringLength = RENotFound) const;
	
	REString & operator=(const char * utf8String);
	REString & operator=(const wchar_t * wideString);
	
	REString & operator=(const REWideString & anotherString);
	REString & operator=(const REString & anotherString);
	REString & operator=(const REMutableString & anotherString);
	
	operator const char* () { return this->getChars(); }
	operator const char* () const { return this->getChars(); }
	
	/// Returns path extension.
	REString getPathExtension() const;
	
	REString();
	REString(const char * utf8String, const REUInt32 utf8StringLength = RENotFound);
	REString(const wchar_t * wideString, const REUInt32 wideStringLength = RENotFound);
	REString(const REWideString & anotherString);
	REString(const REString & anotherString);
	REString(const REMutableString & anotherString);
	
	REString(const REPtr<REBuffer> & utf8StringBuffer);
	
	virtual ~REString();
	
#if defined(CFSTR) 
	
	CFStringRef getCFString() const
	{
		if (this->isNotEmpty())
		{
			return CFStringCreateWithCString(kCFAllocatorDefault, 
											 this->getChars(), 
											 kCFStringEncodingUTF8);
		}
		return (CFStringRef)0;
	}
	
	REString(CFStringRef cfString) : REStringBase()
	{
		if (cfString)
		{
			const CFIndex len = CFStringGetLength(cfString);
			if (len > 0)
			{
				REBOOL isOk = false;
				REBuffer * newBuffer = new REBuffer();
				if (newBuffer)
				{
					if (newBuffer->resize((len + 1) * sizeof(REUInt32), false))
					{
						char * buff = (char *)newBuffer->getBuffer();
						if (CFStringGetCString(cfString,
											   (char *)buff,
											   (CFIndex)newBuffer->getSize(),
											   kCFStringEncodingUTF8))
						{
							const size_t resBuffSize = strlen(buff);
							if (resBuffSize > 0)
							{
								if (newBuffer->resize(resBuffSize + 1, true))
								{
									buff = (char *)newBuffer->getBuffer();
									buff[resBuffSize] = 0;
									_p = REPtr<REBuffer>(newBuffer);
									isOk = true;
								}
							}
						}
					}
					if (!isOk)
					{
						delete newBuffer;
					}
				}
			}
		}
	}
	
#endif
	
};

#endif /* __REString_H__  */


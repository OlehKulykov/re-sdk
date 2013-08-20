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


#ifndef __REWideString_H__ 
#define __REWideString_H__ 

#include "RECommonHeader.h"
#include "REStringBase.h"

class REString;
class REMutableString;

class __RE_PUBLIC_CLASS_API__ REWideString : public REStringBase
{
public:
	REString string() const;
	REMutableString mutableString() const;
	
	const wchar_t * wideChars() const;
	const REUInt32 length() const;
	
	REWideString & operator=(const char * utf8String);
	REWideString & operator=(const wchar_t * wideString);
	
	REWideString & operator=(const REWideString & anotherString);
	REWideString & operator=(const REString & anotherString);
	REWideString & operator=(const REMutableString & anotherString);
	
	operator const wchar_t* () { return this->wideChars(); }
	operator const wchar_t* () const { return this->wideChars(); }
	
	REWideString();
	REWideString(const char * utf8String, const REUInt32 utf8StringLength = RENotFound);
	REWideString(const wchar_t * wideString, const REUInt32 wideStringLength = RENotFound);
	REWideString(const REWideString & anotherString);
	REWideString(const REString & anotherString);
	REWideString(const REMutableString & anotherString);
	
	REWideString(const REPtr<REBuffer> & wideStringBuffer);
	
	virtual ~REWideString();
	
#if defined(CFSTR) 
	
	CFStringRef getCFString() const
	{
		const REUInt32 len = this->length();
		if (len)
		{
			return CFStringCreateWithBytes(kCFAllocatorDefault,
										   (const UInt8 *)_p->buffer(),
										   (CFIndex)(_p->size() - sizeof(wchar_t)),
										   (CFByteOrderGetCurrent() == CFByteOrderLittleEndian) ? kCFStringEncodingUTF32LE : kCFStringEncodingUTF32BE,
										   false);
		}
		return (CFStringRef)0;
	}
	
	REWideString(CFStringRef cfString) : REStringBase()
	{
		if (cfString)
		{
			const CFIndex len = CFStringGetLength(cfString);
			if (len > 0)
			{
				REBuffer * newBuffer = new REBuffer();
				if (newBuffer)
				{
					if (newBuffer->resize((len + 1) * sizeof(REUInt32), false))
					{
						char * buff = (char *)newBuffer->buffer();
						if (CFStringGetCString(cfString,
											   (char *)buff,
											   (CFIndex)newBuffer->size(),
											   kCFStringEncodingUTF8))
						{
							const size_t resBuffSize = strlen(buff);
							buff[resBuffSize] = 0;
							this->setFromUTF8String(buff, 
													(REUInt32)resBuffSize,
													REStringTypeWide);
						}
					}
					delete newBuffer;
				}
			}
		}
	}
	
#endif
	
};

#endif /* __REWideString_H__  */


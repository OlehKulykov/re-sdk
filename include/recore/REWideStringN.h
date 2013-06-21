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


#ifndef __REWIDESTRINGN_H__ 
#define __REWIDESTRINGN_H__ 

#include "RECommonHeader.h"
#include "REStringBase.h"

class REStringN;
class REMutableStringN;

class __RE_PUBLIC_CLASS_API__ REWideStringN : public REStringBase
{
public:
	REStringN getString() const;
	REMutableStringN getMutableString() const;
	
	const wchar_t * getWideChars() const;
	const REUInt32 getLength() const;
	
	REWideStringN & operator=(const char * utf8String);
	REWideStringN & operator=(const wchar_t * wideString);
	
	REWideStringN & operator=(const REWideStringN & anotherString);
	REWideStringN & operator=(const REStringN & anotherString);
	REWideStringN & operator=(const REMutableStringN & anotherString);
	
	operator const wchar_t* () { return this->getWideChars(); }
	operator const wchar_t* () const { return this->getWideChars(); }
	
	REWideStringN();
	REWideStringN(const char * utf8String, const REUInt32 utf8StringLength = RENotFound);
	REWideStringN(const wchar_t * wideString, const REUInt32 wideStringLength = RENotFound);
	REWideStringN(const REWideStringN & anotherString);
	REWideStringN(const REStringN & anotherString);
	REWideStringN(const REMutableStringN & anotherString);
	
	REWideStringN(const REPtr<REBuffer> & wideStringBuffer);
	
	virtual ~REWideStringN();
	
#if defined(CFSTR) 
	
	CFStringRef getCFString() const
	{
		const REUInt32 len = this->getLength();
		if (len)
		{
			return CFStringCreateWithBytes(kCFAllocatorDefault,
										   (const UInt8 *)_p->getBuffer(),
										   (CFIndex)(_p->getSize() - sizeof(wchar_t)),
										   (CFByteOrderGetCurrent() == CFByteOrderLittleEndian) ? kCFStringEncodingUTF32LE : kCFStringEncodingUTF32BE,
										   false);
		}
		return (CFStringRef)0;
	}
	
	REWideStringN(CFStringRef cfString) : REStringBase()
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
						char * buff = (char *)newBuffer->getBuffer();
						if (CFStringGetCString(cfString,
											   (char *)buff,
											   (CFIndex)newBuffer->getSize(),
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

#endif /* __REWIDESTRINGN_H__  */


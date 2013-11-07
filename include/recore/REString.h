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


#ifndef __RESTRING_H__ 
#define __RESTRING_H__ 

#include "RECommonHeader.h"
#include "REPtr.h"
#include "REBuffer.h"
#include "REStringBase.h"
#include "RERange.h"

class REMutableString;
class REWideString;

class __RE_PUBLIC_CLASS_API__ REString : public REStringBase
{
public:
	REMutableString mutableString() const;
	REWideString wideString() const;
	
	const char * UTF8String() const;
	const REUInt32 length() const;
	
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
	
	operator const char* () { return this->UTF8String(); }
	operator const char* () const { return this->UTF8String(); }
	
	/// Returns path extension.
	REString pathExtension() const;
	
	/// Try find first integer value from the string.
	/// If can't find result will be zero.
	REInt64 integerValue(REBOOL * isOk = NULL) const;
	
	/// Try find first integer value from the string.
	/// If can't find result will be zero.
	REFloat64 floatValue(REBOOL * isOk = NULL) const;
	
	REString();
	REString(const char * utf8String, const REUInt32 utf8StringLength = RENotFound);
	REString(const wchar_t * wideString, const REUInt32 wideStringLength = RENotFound);
	REString(const char * utf8String, const RERange & range);
	REString(const wchar_t * wideString, const RERange & range);
	REString(const REWideString & anotherString);
	REString(const REString & anotherString);
	REString(const REMutableString & anotherString);
	
	REString(const REPtr<REBuffer> & utf8StringBuffer);
	
	virtual ~REString();
	
	static REString createWithFormat(const char * format, ...);
	
#if (defined(__RE_OS_IPHONE__) || defined(__RE_OS_MACOSX__)) && (defined(FOUNDATION_STATIC_INLINE) || defined(FOUNDATION_EXTERN_INLINE))
	NSString * getNSString() const
	{
		const char * s = this->UTF8String();
		return s ? [NSString stringWithUTF8String:s] : nil;
	}
	
	void setNSString(NSString * str)
	{
		if (str) 
		{
			const char * s = [str UTF8String];
			const NSUInteger len = [str lengthOfBytesUsingEncoding:NSUTF8StringEncoding];
			if (s && len > 0) 
			{
				this->setFromUTF8String(s, (REUInt32)len);
			}
		}
	}
#endif
};

#endif /* __RESTRING_H__  */


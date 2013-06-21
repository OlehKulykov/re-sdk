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


#ifndef __REMUTABLESTRINGN_H__ 
#define __REMUTABLESTRINGN_H__ 

#include "RECommonHeader.h"
#include "REStringN.h"

class REStringN;
class REWideStringN;

class __RE_PUBLIC_CLASS_API__ REMutableStringN : public REStringN
{
protected:
	void replaceWithLen(const char * charsStringValue, 
						const char * withCharsStringValue, 
						const REUInt32 firstLen, 
						const REUInt32 secondLen);
public:
	REMutableStringN & operator=(const char * utf8String);
	REMutableStringN & operator=(const wchar_t * wideString);
	
	REMutableStringN & operator=(const REWideStringN & anotherString);
	REMutableStringN & operator=(const REStringN & anotherString);
	REMutableStringN & operator=(const REMutableStringN & anotherString);
	
	/// Conserts string ti it's lower presentation.
	REMutableStringN & toLower();
	
	/// Conserts string ti it's uper presentation.
	REMutableStringN & toUpper();
	
	/// Appends another UTF8 string.
	REMutableStringN & append(const char * utf8String, const REUInt32 utf8StringLength = RENotFound);
	
	/// Appends another wide char string.
	REMutableStringN & append(const wchar_t * wideString, const REUInt32 wideStringLength = RENotFound);
	
	/// Appends another UTF8 string using format.
	REMutableStringN & appendFormat(const char * format, ...);
	
	/// Replaces UTF8 string with another UTF8 string.
	REMutableStringN & replace(const char * utf8String, const char * withUTF8StringOrNULL = NULL);
	
	/// Replaces wide chars string with another wide chars string.
	REMutableStringN & replace(const wchar_t * wideString, const wchar_t * withWideStringOrNULL = NULL);
	
	REMutableStringN & appendPathComponent(const char * pComponent);
	
	REMutableStringN();
	REMutableStringN(const char * utf8String, const REUInt32 utf8StringLength = RENotFound);
	REMutableStringN(const wchar_t * wideString, const REUInt32 wideStringLength = RENotFound);
	REMutableStringN(const REWideStringN & anotherString);
	REMutableStringN(const REStringN & anotherString);
	REMutableStringN(const REMutableStringN & anotherString);
	
	REMutableStringN(const REPtr<REBuffer> & utf8StringBuffer);
	
	virtual ~REMutableStringN();
};

#endif /* __REMUTABLESTRINGN_H__  */


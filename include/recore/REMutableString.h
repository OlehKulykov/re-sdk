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


#ifndef __REMutableString_H__ 
#define __REMutableString_H__ 

#include "RECommonHeader.h"
#include "REString.h"

class REString;
class REWideString;

class __RE_PUBLIC_CLASS_API__ REMutableString : public REString
{
protected:
	void replaceWithLen(const char * charsStringValue, 
						const char * withCharsStringValue, 
						const REUInt32 firstLen, 
						const REUInt32 secondLen);
public:
	REMutableString & operator=(const char * utf8String);
	REMutableString & operator=(const wchar_t * wideString);
	
	REMutableString & operator=(const REWideString & anotherString);
	REMutableString & operator=(const REString & anotherString);
	REMutableString & operator=(const REMutableString & anotherString);
	
	/// Conserts string ti it's lower presentation.
	REMutableString & toLower();
	
	/// Conserts string ti it's uper presentation.
	REMutableString & toUpper();
	
	/// Appends another UTF8 string.
	REMutableString & append(const char * utf8String, const REUInt32 utf8StringLength = RENotFound);
	
	/// Appends another wide char string.
	REMutableString & append(const wchar_t * wideString, const REUInt32 wideStringLength = RENotFound);
	
	/// Appends another UTF8 string using format.
	REMutableString & appendFormat(const char * format, ...);
	
	/// Replaces UTF8 string with another UTF8 string.
	REMutableString & replace(const char * utf8String, const char * withUTF8StringOrNULL = NULL);
	
	/// Replaces needed char with another char.
	REMutableString & replace(const char needChar, const char targetChar);
	
	/// Replaces wide chars string with another wide chars string.
	REMutableString & replace(const wchar_t * wideString, const wchar_t * withWideStringOrNULL = NULL);
	
	REMutableString & appendPathComponent(const char * pComponent);
	
	REMutableString & removeLastPathComponent();
	
	REMutableString & removePathExtension();
	
	REMutableString();
	REMutableString(const char * utf8String, const REUInt32 utf8StringLength = RENotFound);
	REMutableString(const wchar_t * wideString, const REUInt32 wideStringLength = RENotFound);
	REMutableString(const REWideString & anotherString);
	REMutableString(const REString & anotherString);
	REMutableString(const REMutableString & anotherString);
	
	REMutableString(const REPtr<REBuffer> & utf8StringBuffer);
	
	virtual ~REMutableString();
};

#endif /* __REMutableString_H__  */


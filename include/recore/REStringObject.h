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


#ifndef __RESTRINGOBJECT_H__
#define __RESTRINGOBJECT_H__


#include "RECommonHeader.h"
#include "REString.h"
#include "REObject.h"


class __RE_PUBLIC_CLASS_API__ REStringObject : public REString, public REObject
{
protected:
	REStringObject(const wchar_t * wideStringValue);
	REStringObject(const char * charsStringValue);
    REStringObject(const char * charsStringValue, const REUInt32 stringLength);
	REStringObject(const REString & anotherString);
	REStringObject(const REStringObject & anotherString);
	REStringObject();
	virtual ~REStringObject();
public:
	/* REObject */
	virtual const REUInt32 GetClassIdentifier() const;
	static const REUInt32 ClassIdentifier();
	virtual REBOOL IsImplementsClass(const REUInt32 classIdentifier) const;
	virtual REBOOL IsEqual(REObject * anotherObject);
	
	/// Creates and return new string object with wide string value or NULL on error.
	/// After using this string object call Release().
	static REStringObject * CreateWithWideChars(const wchar_t * wideStringValue);
	
	/// Creates and return new string object with UTF8 string value or NULL on error.
	/// After using this string object call Release().
	static REStringObject * CreateWithChars(const char * charsStringValue);
	
	/// Creates and return new string object with UTF8 string value with string length or NULL on error.
	/// After using this string object call Release().
	static REStringObject * CreateWithCharsAndLen(const char * charsStringValue, const REUInt32 stringLength);
	
	/// Creates and return new string object with another string value or NULL on error.
	/// After using this string object call Release().
	static REStringObject * CreateWithString(const REString & anotherString);
	
	/// Creates and return new string object with another string value or NULL on error.
	/// After using this string object call Release().
	static REStringObject * CreateWithStringObject(const REStringObject * anotherString);
	
	/// Creates and return new string object with formated UTF8 string value or NULL on error.
	/// After using this string call Release().
	static REStringObject * CreateWithFormatChars(const char * format, ...);
};

#endif /* __RESTRING_H__ */


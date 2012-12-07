/*
 *   Copyright 2012 Kulykov Oleh
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
#include "RERange.h"

class REString;

/// Presentation of REString object
class __RE_PUBLIC_CLASS_API__ REStringPresentation
{
private:
	char * _c;
	wchar_t * _w;
	REUInt32 _cLen;
	REUInt32 _wLen;
public:
	/// Converts wide char string to UTF8 string.
	static int WideToChars(const wchar_t * wideString, int wideStringLength, char * charsString);
	
	/// Converts UTF8 string to it's wide char presentation.
	static int CharsToWide(const char * charsString, int charsStringLengthOrNegative, wchar_t * wideString);
	
	/// Sets from UTF8 string.
	void SetC(const char * stringValue);
	
	/// Sets from wide char string.
	void SetW(const wchar_t * stringValue);
	
	/// Sets from UTF8 string with length.
	void SetCWithLen(const char * stringValue, const REUInt32 len);
	
	/// Sets from wide char string with length.
	void SetWWithLen(const wchar_t * stringValue, const REUInt32 len);
	
	/// Clears presentation.
	void Clear();
	
	/// Returns pointer to UTF8 string.
	const char * UTF8String() const;
	
	/// Returns UTF8 string length.
	const REUInt32 GetUTF8Length() const;
	
	/// Returns pointer to wide char string.
	const wchar_t * WideString() const;
	
	/// Returns wide char string length.
	const REUInt32 GetWideLength() const;
	
	/// Assign operator from another string presentation object.
	REStringPresentation & operator=(const REStringPresentation & stringValue);
	
	/// Assign operator from string object.
	REStringPresentation & operator=(const REString & stringValue);
	
	/// Constructs presentation with UTF8 string.
	REStringPresentation(const char * stringValue);
	
	/// Constructs presentation with wide char string value.
	REStringPresentation(const wchar_t * stringValue);
	
	/// Constructs presentation string presentation object.
	REStringPresentation(const REStringPresentation & stringValue);
	
	/// Constructs presentation with string object.
	REStringPresentation(const REString & stringValue);
	
	/// Constructs empty presentation.
	REStringPresentation();
	
	/// Destructor.
	virtual ~REStringPresentation();
};

/// String class.
class __RE_PUBLIC_CLASS_API__ REString
{
protected:
	char * _data;
	REUInt32 _length;
	
	
	REString & SetWithLen(const char * charsStringValue, const REUInt32 stringLength);
	void AppendWithLen(const char * charsStringValue, const REUInt32 len);
	void ReplaceWithLen(const char * charsStringValue, const char * withCharsStringValue, const REUInt32 firstLen, const REUInt32 secondLen);
	static REBOOL IsDigit(const char * charsStringValue);
	static void AppendPathComponentToString(const char * pComponent, REString & string);
	static void ExtractLastPathComponent(const REString & source, REString & lastPathComponent);
	static void ExtractPathExtension(const REString & source, REString & pathExtension);
	
public:
	/// Returns pointer to UTF8 string.
	const char * UTF8String() const;
	
	/// Sets string from UTF8 string.
	REString & Set(const char * charsStringValue);
	
	/// Sets string from wide char string.
	REString & Set(const wchar_t * wideStringValue);
	
	/// Sets string from another string object.
	REString & Set(const REString & anotherString);
	
	/// Sets string from UTF8 string with length.
	REString & Set(const char * charsStringValue, const REUInt32 stringLength);
	
	/// Sets string from wide char string with length.
	REString & Set(const wchar_t * wideStringValue, const REUInt32 stringLength);
	
	/// Sets string from string object with length.
	REString & Set(const REString & anotherString, const REUInt32 stringLength);
	
	/// Assign operator from UTF8 string.
    REString & operator=(const char * charsStringValue);
	
	/// Assign operator from wide char string.
	REString & operator=(const wchar_t * wideStringValue);
	
	/// Assign operator from string object
	REString & operator=(const REString & anotherString);
	
	/// Assign operator from string presentation.
	REString & operator=(const REStringPresentation & anotherStringPresentation);
	
	/// Conserts string ti it's lower presentation.
	REString & ToLower();
	
	/// Conserts string ti it's uper presentation.
	REString & ToUpper();
	
	/// Check is string empty.
	REBOOL IsEmpty() const;
	
	/// Checks if string has digit value.
	REBOOL IsDigit() const;
	
	/// Checks string for non ascii characters.
	REBOOL IsNonASCIICharsPresent() const;
	
	/// Returns length of string.
	const REUInt32 Length() const;
	
	/// Clears string object.
	REString & Clear();
	
	/// Check is string equal to another UTF8 string. Case sensitive.
	REBOOL IsEqual(const char * charsStringValue) const;
	
	/// Check is string equal to another wide char string. Case sensitive.
	REBOOL IsEqual(const wchar_t * wideStringValue) const;
	
	/// Check is string equal to another string object. Case sensitive.
	REBOOL IsEqual(const REString & anotherString) const;
	
	/// Check is string equal to another UTF8 string ignoring case.
	REBOOL IsEqualIgnoreCase(const char * charsStringValue) const;
	
	/// Check is string equal to another wide char string ignoring case.
	REBOOL IsEqualIgnoreCase(const wchar_t * wideStringValue) const;
	
	/// Check is string equal to another string object ignoring case.
	REBOOL IsEqualIgnoreCase(const REString & anotherString) const;
	
	/// Appends another UTF8 string.
	REString & Append(const char * charsStringValue);
	
	/// Appends another wide char string.
	REString & Append(const wchar_t * wideStringValue);
	
	/// Appends another UTF8 string using format.
	REString & AppendFormat(const char * format, ...);
	
	/// Check is string containes UTF8 string.
	REBOOL IsContaines(const char * charsStringValue) const;
	
	/// Check is string containes wide char string.
	REBOOL IsContaines(const wchar_t * wideStringValue) const;
	
	/// Replaces UTF8 string with another UTF8 string.
	REString & Replace(const char * charsStringValue, const char * withCharsStringValueOrNULL = NULL);
	
	/// Replaces wide chars string with another wide chars string.
	REString & Replace(const wchar_t * wideStringValue, const wchar_t * withWideStringValueOrNULL = NULL);

	/// Constructs string with wide char string.
	REString(const wchar_t * wideStringValue);
	
	/// Constructs string with wide char string and it's length.
	REString(const wchar_t * wideStringValue, const REUInt32 stringLength);
	
	/// Constructs string with UTF8 string.
	REString(const char * charsStringValue);
	
	/// Constructs string with UTF8 string and it's length.
    REString(const char * charsStringValue, const REUInt32 stringLength);
	
	/// Constructs string with UTF8 string using range.
	REString(const char * charsStringValue, const RERange & rangeFromString);
	
	/// Constructs string with another string object.
	REString(const REString & anotherString);
	
	/// Constructs string with string presentation.
	REString(const REStringPresentation & anotherStringPresentation);
	
	/// Constructs empty string.
	REString();
	virtual ~REString();
	
	/// Appends another path component.
	REString & AppendPathComponent(const char * pComponent);
	
	/// Returns last path component.
	REString GetLastPathComponent() const;
	
	/// Returns path extension.
	REString GetPathExtension() const;
	
	/// Removes path extension.
	REString & RemovePathExtension();
	
	/// This functionality avaiable only with RENetwork and
	/// in config file must be defined __RE_RECORE_CAN_INITIALIZE_FROM_URL_STRING__ 
	/// Initializing data object from URL String.
	/// Example: http://example.com/index.html for downloading from web.
	/// Example: file:///Volumes/Data/file.txt for reading from from file. See file url scemes.
	REBOOL InitFromURLString(const REString & urlString);
	
	/// Replaces special HTML characters(strings) with it's symbol presentation.
	/// Example: string &quot; will be replaced with ' symbol and ets.
	REString & DecodeSpecialHTMLCharacters();
	
	/// Replaces symbols to special HTML characters strings.
	/// Example: ' symbol will be replaces with &quot; string and ets.
	REString & EncodeWithSpecialHTMLCharacters();
	
	/// Check C string for containing of wide character.
	static REBOOL IsContainesNonASCIIChararacters(const char * someString);
	
	/// Compare two C strings ignore case. While checking strings not tested for wide chars avaiability.
	/// Both strings shoudl containe only ascii chars.
	/// Return 0 if equal or number of first ocurence of incompatible char.
	static REUInt32 CompareIgnoreCase(const char * firstString, const char * secondString);
	
	/// Compare two wide chars strings ignoring case.
	static REUInt32 CompareIgnoreCase(const wchar_t * firstString, const wchar_t * secondString);
};

#endif /* __RESTRING_H__ */


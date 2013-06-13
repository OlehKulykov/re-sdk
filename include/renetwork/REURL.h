/*
 *   Copyright 2012 Kulikov Oleg
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


#ifndef __REURL_H__
#define __REURL_H__


#include "../recore/RECommonHeader.h"
#include "../recore/REString.h"
#include "../recore/RERange.h"
#include "../recore/REBuffer.h"
#include "../recore/RENumber.h"

/// http://en.wikipedia.org/wiki/URI_scheme

class __RE_PUBLIC_CLASS_API__ REURL
{
private:	
	static void ParseUserNameAndPassword(void * info, const char * stringValue, const REUInt32 location, const REUInt32 lenght);
	static void ParseQuery(void * info, const char * stringValue, const REUInt32 location, const REUInt32 lenght);
	static void ParseFragment(void * info, const char * stringValue, const REUInt32 location, const REUInt32 lenght);
	static void ParseFileNameAndExtension(void * info, const char * stringValue, const REUInt32 location, const REUInt32 lenght);
	static void ParseUserInfo(void * info, const char * stringValue, const REUInt32 location, const REUInt32 lenght);
	static void ParseHostName(void * info, const char * stringValue, const REUInt32 location, const REUInt32 lenght);
	static void ParsePort(void * info, const char * stringValue, const REUInt32 location, const REUInt32 lenght);
	static void ParseAuthority(void * info, const char * stringValue, const REUInt32 location, const REUInt32 lenght);
	static void ParsePath(void * info, const char * stringValue, const REUInt32 location, const REUInt32 lenght);
	static void ParseHierarchicalPart(void * info, const char * stringValue, const REUInt32 location, const REUInt32 lenght);
	static void ParseSchemeName(void * info, const char * stringValue, const REUInt32 location, const REUInt32 lenght);	
	
protected:
	REBuffer _buffer;
	void * _ranges;
	
	void SetURL(const REURL & url);
	void SetURLString(const char * stringValue, const REUInt32 length = RENotFound);
	void SetWithLen(const char * stringValue, const REUInt32 lenght);
	void Parse();
	
	static REBOOL Parse(void * info, const char * stringValue, const REUInt32 lenght);
	
public:
	REString GetSchemeName() const;	
	REString GetUserInfo() const;
	REString GetHostName() const;
	RENumber GetPort() const;
	REString GetQuery() const;
	REString GetFragment() const;
	REString GetAuthority() const;
	REString GetPath() const;
	REString GetHierarchicalPart() const;
	REString GetFileName() const;
	REString GetExtension() const;
	REString GetFileNameWithExtension() const;
	REString GetUserName() const;
	REString GetPassword() const;
	
	const char * UTF8String() const;
	const REUInt32 Length() const;
	REBOOL IsFileURL() const;
	
	REURL & operator=(const REURL & urlValue);
	REURL & operator=(const REString & stringValue);
	REURL & operator=(const wchar_t * stringValue);
	REURL & operator=(const char * stringValue);
	
	REURL(const REURL & urlValue);
	REURL(const REString & stringValue);
	REURL(const wchar_t * stringValue);
	REURL(const char * stringValue);
	REURL();
	virtual ~REURL();
};


#endif /* __REURL_H__ */


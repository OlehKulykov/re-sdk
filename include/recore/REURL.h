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


#include "RECommonHeader.h"
#include "REString.h"
#include "RERange.h"
#include "REBuffer.h"
#include "RENumber.h"

/// http://en.wikipedia.org/wiki/URI_scheme

class __RE_PUBLIC_CLASS_API__ REURL
{
private:	
	static void parseUserNameAndPassword(void * info, const char * stringValue, const REUInt32 location, const REUInt32 lenght);
	static void parseQuery(void * info, const char * stringValue, const REUInt32 location, const REUInt32 lenght);
	static void parseFragment(void * info, const char * stringValue, const REUInt32 location, const REUInt32 lenght);
	static void parseFileNameAndExtension(void * info, const char * stringValue, const REUInt32 location, const REUInt32 lenght);
	static void parseUserInfo(void * info, const char * stringValue, const REUInt32 location, const REUInt32 lenght);
	static void parseHostName(void * info, const char * stringValue, const REUInt32 location, const REUInt32 lenght);
	static void parsePort(void * info, const char * stringValue, const REUInt32 location, const REUInt32 lenght);
	static void parseAuthority(void * info, const char * stringValue, const REUInt32 location, const REUInt32 lenght);
	static void parsePath(void * info, const char * stringValue, const REUInt32 location, const REUInt32 lenght);
	static void parseHierarchicalPart(void * info, const char * stringValue, const REUInt32 location, const REUInt32 lenght);
	static void parseSchemeName(void * info, const char * stringValue, const REUInt32 location, const REUInt32 lenght);	
	
protected:
	REBuffer _buffer;
	void * _ranges;
	
	void setURL(const REURL & url);
	void setURLString(const char * stringValue, const REUInt32 length = RENotFound);
	void setWithLen(const char * stringValue, const REUInt32 lenght);
	void parse();
	
	static REBOOL parse(void * info, const char * stringValue, const REUInt32 lenght);
	
public:
	REString schemeName() const;	
	REString userInfo() const;
	REString hostName() const;
	RENumber port() const;
	REString query() const;
	REString fragment() const;
	REString authority() const;
	REString path() const;
	REString hierarchicalPart() const;
	REString fileName() const;
	REString extension() const;
	REString fileNameWithExtension() const;
	REString userName() const;
	REString password() const;
	
	const char * UTF8String() const;
	const REUInt32 length() const;
	REBOOL isFileURL() const;
	
	REURL & operator=(const REURL & urlValue);
	REURL & operator=(const REString & stringValue);
	REURL & operator=(const wchar_t * stringValue);
	REURL & operator=(const char * stringValue);
	
	REURL(const REURL & urlValue);
	REURL(const REString & stringValue);
	REURL(const wchar_t * wideString, const REUInt32 wideStringLength = RENotFound);
	REURL(const char * utf8String, const REUInt32 utf8StringLength = RENotFound);
	REURL();
	virtual ~REURL();
};


#endif /* __REURL_H__ */


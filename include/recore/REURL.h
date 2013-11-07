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


#ifndef __REURL_H__
#define __REURL_H__


#include "RECommonHeader.h"
#include "REString.h"
#include "RENumber.h"
#include "REPtr.h"

/// http://en.wikipedia.org/wiki/URI_scheme

class REURLInternal;

class __RE_PUBLIC_CLASS_API__ REURL
{
private:
	void setURLString(const char * stringValue, const REUInt32 length = RENotFound);
	void setWithLen(const char * stringValue, const REUInt32 lenght);
	void parse();
	
protected:
	REPtr<REURLInternal> _i;
	
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


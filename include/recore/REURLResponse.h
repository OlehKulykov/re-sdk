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


#ifndef __REURLRESPONCE_H__
#define __REURLRESPONCE_H__


#include "REURL.h"
#include "REString.h"
#include "REPtr.h"

class __RE_PUBLIC_CLASS_API__ REURLResponse
{
protected:
	REString _suggestedFileName;
	REString _MIMEType;
	REURL _url;
	REUInt32 _expectedContentSize;
	REInt32 _statusCode;
	REFloat32 _httpVersion;
	REBOOL _isExpectedContentSizeDetermined;

	REURLResponse();
public:	
	const REURL & url() const;
	const REUInt32 expectedContentSize() const;
	REBOOL isExpectedContentSizeDetermined() const;
	const REInt32 statusCode() const;
	const REFloat32 httpVersion() const;
	const REString & MIMEType() const;
	const REString & suggestedFileName() const;
	
	virtual ~REURLResponse();
};


#endif /* __REURLRESPONCE_H__ */


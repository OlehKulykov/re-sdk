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


#ifndef __REMUTABLEURLRESPONCE_H__
#define __REMUTABLEURLRESPONCE_H__


#include "../REURLResponse.h"

class __RE_PUBLIC_CLASS_API__ REMutableURLResponse : public REURLResponse
{
public:
	void setExpectedContentSize(REUInt32 v)
	{
		_expectedContentSize = v;
	}
	
	void setIsExpectedContentSizeDetermined(REBOOL v)
	{
		_isExpectedContentSizeDetermined = v;
	}
	
	void setMIMEType(const char * v)
	{
		_MIMEType = v;
	}
	
	void setStatusCode(REInt32 v) 
	{
		_statusCode = v;
	}
	
	void setHttpVersion(REFloat32 v)
	{
		_httpVersion = v;
	}
	
	void setURL(const REURL & v)
	{
		_url = v;
	}
	
	REMutableURLResponse() : REURLResponse()
	{
		
	}
	
	virtual ~REMutableURLResponse()
	{
		
	}
};


#endif /* __REMUTABLEURLRESPONCE_H__ */


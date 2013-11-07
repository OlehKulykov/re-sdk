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


#ifndef __REURLREQUEST_H__
#define __REURLREQUEST_H__


#include "RECommonHeader.h"
#include "REURL.h"
#include "REPtr.h"

typedef enum _reURLRequestType
{
	REURLRequestGET = 0,
	REURLRequestPOST = 1,
	
	REURLRequestPUT = 2,
	REURLRequestDELETE = 3
} REURLRequestType;

class REURLRequestInternal;

class __RE_PUBLIC_CLASS_API__ REURLRequest
{
private:
	REPtr<REURLRequestInternal> _r;
	
protected:	
	REURLRequest(const REURL & url);
	
public:
	REBOOL isFollowRedirects() const;
	void setFollowRedirects(REBOOL isFollowRedirectsFlag);
	const REURL & url() const;
	const REURLRequestType HTTPMethod() const;
	void setHTTPMethod(const REURLRequestType method);
	const RETimeInterval connectingTimeoutInterval() const;
	void setConnectingTimeoutInterval(const RETimeInterval timeInterval);
	
	virtual ~REURLRequest();
	
	static REPtr<REURLRequest> createWithURL(const REURL & url);
};


#endif /* __REURLREQUEST_H__ */


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


#include "../../include/recore/REURLRequest.h"
#include "../../include/recore/REMD5Generator.h"

#if defined(HAVE_RECORE_SDK_CONFIG_H) 
#include "recore_sdk_config.h"
#endif



#if !defined(__RE_RECORE_NO_LOAD_FROM_URL__)

#if defined(HAVE_CURL_CURL_H)
/* HAVE_CURL_CURL_H */
#include <curl/curl.h>

#ifndef __RE_USING_CURL_LIBRARY__
#define __RE_USING_CURL_LIBRARY__ 1
#endif

#else 

#include "../addlibs/curl.h"
#ifndef __RE_USING_CURL_LIBRARY__
#define __RE_USING_CURL_LIBRARY__ 1
#endif

#endif 


#endif /* !defined(__RE_RECORE_NO_LOAD_FROM_URL__) */


class REURLRequestInternal 
{
public:
	REURL url;
	RETimeInterval connectingTimeoutInterval;
	REUByte httpMethod;
	REBOOL isFollowRedirects;
	
	REURLRequestInternal() :
		connectingTimeoutInterval(30.0),
		httpMethod(0),
		isFollowRedirects(true)
	{
		
	}
};

REBOOL REURLRequest::isFollowRedirects() const
{
	return _r->isFollowRedirects;
}

void REURLRequest::setFollowRedirects(REBOOL isFollowRedirectsFlag)
{
	_r->isFollowRedirects = isFollowRedirectsFlag;
}

const REURL & REURLRequest::url() const
{
	return _r->url;
}

const REURLRequestType REURLRequest::HTTPMethod() const
{
	return (REURLRequestType)_r->httpMethod;
}

void REURLRequest::setHTTPMethod(const REURLRequestType method)
{
	_r->httpMethod = (REUByte)method;
}

const RETimeInterval REURLRequest::connectingTimeoutInterval() const
{
	return _r->connectingTimeoutInterval;
}

void REURLRequest::setConnectingTimeoutInterval(const RETimeInterval timeInterval)
{
	_r->connectingTimeoutInterval = timeInterval;
}

REURLRequest::REURLRequest(const REURL & url)
{
	REPtr<REURLRequestInternal> internal(new REURLRequestInternal());
	if (internal.isNotEmpty()) 
	{
		internal->url = url;
		_r = REPtr<REURLRequestInternal>(internal);
	}
}

REURLRequest::~REURLRequest()
{
	_r.release();
}

REPtr<REURLRequest> REURLRequest::createWithURL(const REURL & url)
{
	REPtr<REURLRequest> r(new REURLRequest(url));
	return r;
}



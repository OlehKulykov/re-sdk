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


#include "../../include/renetwork/REURLRequestObject.h"
#include "../../include/recore/REMD5Generator.h"

#ifdef __RE_USING_ADITIONAL_CURL_LIBRARY__
#include "../addlibs/curl.h"
#endif

#ifdef __RE_USING_SYSTEM_CURL_LIBRARY__
#include <curl/curl.h>
#endif

/* REObject */
const REUInt32 REURLRequestObject::GetClassIdentifier() const
{
	return REURLRequestObject::ClassIdentifier();
}

const REUInt32 REURLRequestObject::ClassIdentifier()
{
	static const REUInt32 clasIdentif = REMD5Generator::GenerateFromString("REURLRequestObject");
	return clasIdentif;
}

REURLObject * REURLRequestObject::GetURL() const
{
	return _url;
}

#if 0
#ifndef __RE_OS_IPHONE__

//TODO:
//REURLRequestObject::REURLRequestObject(const REURL & url);
//REURLRequestObject::~REURLRequestObject();
//void SetHTTPMethod(const REString & method);

#endif /* __RE_OS_IPHONE__  */
#endif /* 0 */ 

REBOOL REURLRequestObject::IsFollowRedirects() const
{
	return _isFollowRedirects;
}

void REURLRequestObject::SetFollowRedirects(REBOOL isFollowRedirectsFlag)
{
	_isFollowRedirects = isFollowRedirectsFlag;
}

const REURLRequestType REURLRequestObject::GetHTTPMethod() const
{
	return ((REURLRequestType)_httpMethod);
}

void REURLRequestObject::SetHTTPMethod(const REURLRequestType method)
{
	_httpMethod = (REUByte)method;
}

const RETimeInterval REURLRequestObject::GetConnectingTimeoutInterval() const
{
	return _connectingTimeoutInterval;
}

void REURLRequestObject::SetConnectingTimeoutInterval(const RETimeInterval timeInterval)
{
	_connectingTimeoutInterval = timeInterval;
}

REURLRequestObject::REURLRequestObject(const REURL & url) : REObject(),
	_url(NULL),
	_connectingTimeoutInterval(60.0),
	_httpMethod(0),
	_isFollowRedirects(true)
{
	_url = REURLObject::CreateWithURL(url);
}

REURLRequestObject::~REURLRequestObject()
{
	if (_url) 
	{
		_url->Release();
	}
}

REURLRequestObject * REURLRequestObject::CreateWithURL(const REURL & url)
{
	if (url.Length()) 
	{
		REURLRequestObject * r = new REURLRequestObject(url);
		return r;
	}
	return NULL;
}





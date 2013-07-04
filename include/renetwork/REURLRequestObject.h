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


#ifndef __REURLREQUESTOBJECT_H__
#define __REURLREQUESTOBJECT_H__


#include "REURLObject.h"
#include "../recore/RECommonHeader.h"
#include "../recore/REObject.h"

typedef enum _reURLRequestType
{
	REURLRequestGET = 0,
	REURLRequestPOST = 1,
	
	REURLRequestPUT = 2,
	REURLRequestDELETE = 3
} REURLRequestType;

class __RE_PUBLIC_CLASS_API__ REURLRequestObject : public REObject
{
private:	
#ifdef __RE_OS_IPHONE__
	void * _iphonePtr;
#endif	
	
	
protected:
	REURLObject * _url;
	RETimeInterval _connectingTimeoutInterval;
	REUByte _httpMethod;
	REBOOL _isFollowRedirects;
	REURLRequestObject(const REURL & url);
	
	
public:
#ifdef __RE_OS_IPHONE__
	void * GetIphonePtr() const { return _iphonePtr; }
#endif
	/* REObject */
	virtual const REUInt32 GetClassIdentifier() const;
	static const REUInt32 ClassIdentifier();
	
	REBOOL IsFollowRedirects() const;
	void SetFollowRedirects(REBOOL isFollowRedirectsFlag);
	REURLObject * GetURL() const;
	const REURLRequestType GetHTTPMethod() const;
	void SetHTTPMethod(const REURLRequestType method);
	const RETimeInterval GetConnectingTimeoutInterval() const;
	void SetConnectingTimeoutInterval(const RETimeInterval timeInterval);
	
	virtual ~REURLRequestObject();
	
	static REURLRequestObject * CreateWithURL(const REURL & url);
};


#endif /* __REURLREQUESTOBJECT_H__ */


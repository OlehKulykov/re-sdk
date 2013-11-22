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

/**
 @brief Options for URL request typing.
 */
typedef enum _reURLRequestType
{
	/**
	 @brief The option for indicating GET request.
	 */
	REURLRequestGET = 0,
	
	
	/**
	 @brief The option for indicating POST request.
	 */
	REURLRequestPOST = 1,
	
	
	/**
	 @brief The option for indicating PUT request.
	 */
	REURLRequestPUT = 2,
	
	
	/**
	 @brief The option for indicating DELETE request.
	 */
	REURLRequestDELETE = 3
} 


/**
 @brief Options for URL request typing.
 */
REURLRequestType;


class REURLRequestInternal;


/**
 @brief Class presented URL request.
 */
class __RE_PUBLIC_CLASS_API__ REURLRequest
{
private:
	REPtr<REURLRequestInternal> _r;
	
protected:	
	/**
	 @brief Contructs request object with URL.
	 */
	REURLRequest(const REURL & url);
	
public:
	/**
	 @brief Indicating that request should follow redirections.
	 @return True if should follow redirects, othervice false.
	 */
	REBOOL isFollowRedirects() const;
	
	
	/**
	 @brief Set request should follow redirections.
	 @param isFollowRedirectsFlag Flag indicating following redirects
	 */
	void setFollowRedirects(REBOOL isFollowRedirectsFlag);
	
	
	/**
	 @brief Request URL.
	 @return Address of the URL object.
	 */
	const REURL & url() const;
	
	
	/**
	 @brief Type of the request.
	 @return One of the REURLRequestType options.
	 */
	const REURLRequestType HTTPMethod() const;
	
	
	/**
	 @brief Set type of the request.
	 @param method Method of the request. One of the REURLRequestType options
	 */
	void setHTTPMethod(const REURLRequestType method);
	
	
	/**
	 @brief Time of the maximum connection time.
	 @return Time interval in seconds of the timeout interval.
	 */
	const RETimeInterval connectingTimeoutInterval() const;
	
	
	/**
	 @brief Set maximum connection time.
	 @param timeInterval Time interval in seconds of the timeout interval.
	 */
	void setConnectingTimeoutInterval(const RETimeInterval timeInterval);
	
	
	/**
	 @brief Default destructor.
	 */
	virtual ~REURLRequest();
	
	
	/**
	 @brief Creates new URL request object.
	 @param url The URL object of the request.
	 @return Autopointer with request object.
	 */
	static REPtr<REURLRequest> createWithURL(const REURL & url);
};


#endif /* __REURLREQUEST_H__ */


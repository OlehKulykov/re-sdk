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


#if 0

#include "../../include/renetwork/REURLRequestObject.h"


#ifdef __RE_OS_IPHONE__

#include <Foundation/Foundation.h>

REURLRequestObject::REURLRequestObject(const REURL & url) : REObject(),
	_iphonePtr(NULL),
	_url(NULL)
{
	_httpMethod.Set("GET");
	if (url.Length()) 
	{
		_url = REURLObject::CreateWithURL(url);
	}
	
	if (_url)
	{
		NSString * urlString = [NSString stringWithUTF8String:_url->UTF8String()];
		if (urlString) 
		{
			NSURL * url = [NSURL URLWithString:urlString];
			if (url) 
			{
				NSMutableURLRequest * request = [[NSMutableURLRequest alloc] initWithURL:url];
				if (request) 
				{
					_iphonePtr = request;
					[request setHTTPMethod:[NSString stringWithUTF8String:_httpMethod.UTF8String()]];
				}
			}
		}
	}
}

REURLRequestObject::~REURLRequestObject()
{
	if (_url)
	{
		_url->Release();
	}
	
	if (_iphonePtr) 
	{
		NSMutableURLRequest * request = (NSMutableURLRequest *)_iphonePtr;
		[request release];
	}
}

#endif /* __RE_OS_IPHONE__  */

#endif /* 0 */

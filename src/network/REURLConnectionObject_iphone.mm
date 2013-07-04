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

#include "../../include/renetwork/REURLConnectionObject.h"
#include "../../include/renetwork/private/REURLResponseObjectPrivate.h"

#ifdef __RE_OS_IPHONE__

#include <Foundation/Foundation.h>

REURLResponseObjectPrivate * CreateREURLResponseObjectPrivateFromResp(REURLRequestObject * requestPTR,
																	  NSURLResponse * response);

REURLResponseObjectPrivate * CreateREURLResponseObjectPrivateFromResp(REURLRequestObject * requestPTR,
																	  NSURLResponse * response)
{
	if (response) 
	{
		REURLResponseObjectPrivate * resp = new REURLResponseObjectPrivate();
		if (resp) 
		{
			if ([response expectedContentLength] != NSURLResponseUnknownLength)
			{
				resp->SetIsExpectedContentSizeDetermined(true);
				resp->SetExpectedContentSize((REUInt32)[response expectedContentLength]);
			}
			else
			{
				resp->SetIsExpectedContentSizeDetermined(false);
			}
			
			if ([response respondsToSelector:@selector(statusCode)]) 
			{
				resp->SetStatusCode((REInt32)[response statusCode]);
			}
			
			if (requestPTR) 
			{
				resp->SetURL(requestPTR->GetURL());
			}
			
			NSString * someStr = [response MIMEType];
			if (someStr) 
			{
				resp->SetMIMEType([someStr UTF8String]);
			}
			
			someStr = [response suggestedFilename];
			if (someStr) 
			{
				resp->SetSuggestedFileName([someStr UTF8String]);
			}
			
		}
		return resp;
	}
	return NULL;
}

@interface REURLConnectionObjectDelegate_iphonePrivate : NSObject <NSURLConnectionDelegate>
{
@public
	IREURLConnectionDelegate * delegatePTR;
	REBufferObject * receiveDataBufferPTR;
	REURLConnectionObject * connectionPTR;
	BOOL isRedirectResponse;
}
@end

@implementation REURLConnectionObjectDelegate_iphonePrivate

// Tells the delegate that the connection will send a request for an authentication challenge.
- (void)connection:(NSURLConnection *)connection 
willSendRequestForAuthenticationChallenge:(NSURLAuthenticationChallenge *)challenge
{
	
}

// Sent to determine whether the delegate is able to respond to a protection space’s 
// form of authentication.
- (BOOL)connection:(NSURLConnection *)connection 
canAuthenticateAgainstProtectionSpace:(NSURLProtectionSpace *)protectionSpace
{
	BOOL isTrust = [protectionSpace.authenticationMethod isEqualToString:NSURLAuthenticationMethodServerTrust];
	if ( !isTrust ) 
	{
		isTrust = YES;
	}
	return isTrust;
}

// Sent when a connection cancels an authentication challenge.
- (void)connection:(NSURLConnection *)connection 
didCancelAuthenticationChallenge:(NSURLAuthenticationChallenge *)challenge
{
	
}

// Sent when a connection must authenticate a challenge in order to download its request.
- (void)connection:(NSURLConnection *)connection 
didReceiveAuthenticationChallenge:(NSURLAuthenticationChallenge *)challenge
{
	if ([challenge.protectionSpace.authenticationMethod isEqualToString:NSURLAuthenticationMethodServerTrust])
	{
		[challenge.sender useCredential:[NSURLCredential credentialForTrust:challenge.protectionSpace.serverTrust] forAuthenticationChallenge:challenge];
	}
	[challenge.sender continueWithoutCredentialForAuthenticationChallenge:challenge];
}

/*
// Sent to determine whether the URL loader should consult 
// the credential storage for authenticating the connection.
- (BOOL)connectionShouldUseCredentialStorage:(NSURLConnection *)connection
{
	return NO;
}
*/

// Sent when a connection fails to load its request successfully.
- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error
{
	if (receiveDataBufferPTR) 
	{
		receiveDataBufferPTR->Clear();
	}
	
	if (delegatePTR && connectionPTR) 
	{
		delegatePTR->URLConnectionFail(connectionPTR);
	}
}

- (void)connectionDidFinishLoading:(NSURLConnection *)connection
{
	if (delegatePTR && connectionPTR) 
	{
		delegatePTR->URLConnectionFinishLoading(connectionPTR);
	}
}

- (NSURLRequest *)connection:(NSURLConnection *)connection 
			 willSendRequest:(NSURLRequest *)request 
			redirectResponse:(NSURLResponse *)redirectResponse
{
	if (isRedirectResponse && redirectResponse) 
	{
		NSMutableURLRequest * r = [[request mutableCopy] autorelease];
		if (r)
		{
			if ([r isKingOfClass:[NSMutableURLRequest class]])
			{
				[r setURL:[redirectResponse URL]];
				return r;
			}
		}
	}
	return request;
}

- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data
{
	REUInt32 s = 0; 
	const void * d = NULL;
	if (data) 
	{
		s = (REUInt32)[data length];
		d = [data bytes];
	}
	
	if (d && s)
	{
		if (receiveDataBufferPTR)
		{
			receiveDataBufferPTR->Append(d, s); 
		}
		
		if (delegatePTR)
		{
			delegatePTR->URLConnectionReceiveData(connectionPTR, d , s);
		}
	}
}

- (void)connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)response
{
	if (delegatePTR && connectionPTR && response) 
	{
		REURLResponseObjectPrivate * resp = CreateREURLResponseObjectPrivateFromResp(connectionPTR->GetRequest(), 
																					 response);
		delegatePTR->URLConnectionReceiveResponse(connectionPTR, resp);
		if (resp) 
		{
			resp->Release();
		}
	}
}

@end


REBOOL REURLConnectionObject::Start()
{
	if (_iphonePtr3) 
	{
		NSURLConnection * connection = (NSURLConnection *)_iphonePtr3;
		[connection start];
		return true;
	}
	return false;
}

REBOOL REURLConnectionObject::Stop()
{
	if (_iphonePtr3) 
	{
		NSURLConnection * connection = (NSURLConnection *)_iphonePtr3;
		[connection cancel];
		_iphonePtr3 = NULL;
	}
	return true;
}

REURLConnectionObject::REURLConnectionObject(REURLRequestObject * request, IREURLConnectionDelegate * delegate) : REObject(),
	_iphonePtr2(NULL),
	_iphonePtr3(NULL),
	_request(request),
	_delegate(delegate),
	_receivedDataBuffer(NULL)
{
	if (_request) 
	{
		_request->Retain();
		
		REURLConnectionObjectDelegate_iphonePrivate * iphDel = [[REURLConnectionObjectDelegate_iphonePrivate alloc] init];
		if (iphDel) 
		{
			iphDel->delegatePTR = delegate;
			_receivedDataBuffer = new REBufferObject();
			iphDel->receiveDataBufferPTR = _receivedDataBuffer;
			iphDel->connectionPTR = this;
			iphDel->isRedirectResponse = request->IsFollowRedirects() ? YES : NO;
			_iphonePtr2 = iphDel;
		}
		
		NSMutableURLRequest * urlRequest = (NSMutableURLRequest *)_request->GetIphonePtr();
		if (urlRequest) 
		{
			NSURLConnection * connection = [NSURLConnection connectionWithRequest:urlRequest 
																		 delegate:iphDel];
			if (connection) 
			{
				_iphonePtr3 = connection;
			}
		}
	}
}

REURLConnectionObject::~REURLConnectionObject()
{
	if (_request) 
	{
		_request->Release();
	}
	
	if (_iphonePtr2)
	{
		REURLConnectionObjectDelegate_iphonePrivate * iphDel = (REURLConnectionObjectDelegate_iphonePrivate *)_iphonePtr2;
		[iphDel release];
	}
	
	if (_iphonePtr3) 
	{

	}
	
	if (_receivedDataBuffer) 
	{
		_receivedDataBuffer->Release();
	}
}

REBOOL REURLConnectionObject::SendRequest(REURLRequestObject * request, REBuffer * downloadedBufferData, REURLResponseObject ** response)
{
	if (request) 
	{
		NSMutableURLRequest * urlRequest = (NSMutableURLRequest *)request->GetIphonePtr();
		if (urlRequest) 
		{
			NSURLResponse * r = nil;
			NSError * e = nil;
			NSData * d = [NSURLConnection sendSynchronousRequest:urlRequest returningResponse:&r error:&e];
			if (e) 
			{
				return false;
			}
			
			if (downloadedBufferData) 
			{
				downloadedBufferData->Clear();
				if (d) 
				{
					const REUInt32 s = (REUInt32)[d length];
					if (s) 
					{
						downloadedBufferData->Set([d bytes], s);
					}
				}
			}
			
			if (response) 
			{
				REURLResponseObjectPrivate * resp = CreateREURLResponseObjectPrivateFromResp(request, r);
				*response = resp;
			}
			
			return true;
		}
	}
	return false;
}

#endif /* __RE_OS_IPHONE__  */


#endif /* 0 */



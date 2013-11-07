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


#ifndef __REURLCONNECTION_H__
#define __REURLCONNECTION_H__


#include "REURL.h"
#include "REURLRequest.h"
#include "IREURLConnectionDelegate.h"
#include "REURLResponse.h"
#include "REBuffer.h"
#include "REPtr.h"


class __RE_PUBLIC_CLASS_API__ REURLConnection
{
private:
	
protected:
	REPtr<REURLConnection> _connection;
	REPtr<REURLRequest> _request;
	REPtr<REURLResponse> _responce;
	REPtr<REBuffer> _receivedBuffer;
	
	IREURLConnectionDelegate * _delegate;
	
//	void startConnection();
//	void startConnection(REBOOL isThreaded);
//	
//	virtual void ThreadBody();
//	
//	void clean();
//	REURLConnection(REPtr<REURLRequest> request, IREURLConnectionDelegate * delegate);
	REURLConnection();
public:	
	IREURLConnectionDelegate * delegate() const;
	
	void setDelegate(IREURLConnectionDelegate * newDelegate);
	
	virtual REBOOL startConnection();
	
	const REPtr<REURLRequest> & request() const;
	const REPtr<REURLResponse> & responce() const;
	const REPtr<REBuffer> & receivedBuffer() const;

	virtual ~REURLConnection();
	
	static REPtr<REURLConnection> createWithRequestAndDelegate(REPtr<REURLRequest> request,
															   IREURLConnectionDelegate * delegate);
	
	static REBOOL sendRequest(REPtr<REURLRequest> request, 
							  REPtr<REBuffer> * receivedBuffer, 
							  REPtr<REURLResponse> * responce);
};


#endif /* __REURLCONNECTION_H__ */


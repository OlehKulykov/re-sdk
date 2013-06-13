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


#ifndef __REURLCONNECTIONOBJECT_H__
#define __REURLCONNECTIONOBJECT_H__


#include "REURLObject.h"
#include "REURLRequestObject.h"
#include "IREURLConnectionDelegate.h"
#include "REURLResponseObject.h"
#include "../recore/REObject.h"
#include "../recore/REBufferObject.h"
#include "../recore/REThread.h"


class __RE_PUBLIC_CLASS_API__ REURLConnectionObject : public REThread
{
private:
#ifdef __RE_OS_IPHONE__
	void * _iphonePtr2;
	void * _iphonePtr3;
#endif	
	
protected:
	REURLRequestObject * _request;
	IREURLConnectionDelegate * _delegate;
	REBufferObject * _receivedDataBuffer;
	void * _reserved;
	REURLResponseObject * _responce;
	
	void StartConnection();
	void StartConnection(REBOOL isThreaded);
	
	virtual void ThreadBody();
	
	void Clean();
	REURLConnectionObject(REURLRequestObject * request, IREURLConnectionDelegate * delegate);
public:
	/* REObject */
	virtual const REUInt32 GetClassIdentifier() const;
	static const REUInt32 ClassIdentifier();
	
	REBufferObject * GetReceivedDataBuffer() const;
	REURLRequestObject * GetRequest() const;
	

	virtual ~REURLConnectionObject();
	
	static REURLConnectionObject * CreateWithRequestAndDelegate(REURLRequestObject * request, IREURLConnectionDelegate * delegate);
	
	static REBOOL SendRequest(REURLRequestObject * request, REBuffer * downloadedBufferData, REURLResponseObject ** response);
};


#endif /* __REURLCONNECTIONOBJECT_H__ */


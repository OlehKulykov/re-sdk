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


#ifndef __IREURLCONNECTIONDELEGATE_H__
#define __IREURLCONNECTIONDELEGATE_H__


#include "../recore/RECommonHeader.h"

class REURLResponseObject;
class REURLConnectionObject;
class REBufferObject;

class __RE_PUBLIC_CLASS_API__ IREURLConnectionDelegate
{
public:
	virtual void URLConnectionReceiveResponse(REURLConnectionObject * connection, 
											  REURLResponseObject * response) = 0;
	
	virtual void URLConnectionReceiveData(REURLConnectionObject * connection, 
										  const void * data, 
										  const REUInt32 dataSize) = 0;
	
	virtual void URLConnectionFinishLoading(REURLConnectionObject * connection) = 0;
	
	virtual void URLConnectionFail(REURLConnectionObject * connection) = 0;
	
	virtual ~IREURLConnectionDelegate() { }
};


#endif /* __IREURLCONNECTIONDELEGATE_H__ */


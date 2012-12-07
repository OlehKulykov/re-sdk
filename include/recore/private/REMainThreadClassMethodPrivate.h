/*
 *   Copyright 2012 Kulykov Oleh
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


#ifndef __REMAINTHREADCLASSMETHODPRIVATE_H__
#define __REMAINTHREADCLASSMETHODPRIVATE_H__

#include "../RECommonHeader.h"
#include "../REThread.h"
#include "../REMainLoopUpdatable.h"


class REMainThreadClassMethodPrivate : public REObject, public REMainLoopUpdatable
{
protected:
	REClassMethod * _mainThreadMethodForInvoke;
	REObject * _mainThreadMethodObject;
	
public:
	/* REMainLoopUpdatable */ 
	virtual void Update(const RETimeInterval currentTime)
	{	
		_mainThreadMethodForInvoke->InvokeWithObject(_mainThreadMethodObject);
		this->RemoveFromMainLoop();
		REAutoReleasePool::GetDefaultPool()->AddObject(this);
	}
	
	virtual const REUIdentifier GetMainLoopUpdatableIdentifier() const { return this->GetObjectIdentifier(); }
	
	REMainThreadClassMethodPrivate(REClassMethod * methodForInvoke, REObject * methodObject) : REObject(),
		_mainThreadMethodForInvoke(methodForInvoke),
		_mainThreadMethodObject(methodObject)
	{ 
		if (_mainThreadMethodObject) 
		{
			_mainThreadMethodObject->Retain();
		}
		
		if ( _mainThreadMethodForInvoke ) 
		{
			this->AddToMainLoop();
		}
		else 
		{
			REAutoReleasePool::GetDefaultPool()->AddObject(this);
		}
	}
	
	virtual ~REMainThreadClassMethodPrivate() 
	{
		if (_mainThreadMethodObject) 
		{
			_mainThreadMethodObject->Release();
		}
		
		if ( _mainThreadMethodForInvoke ) 
		{
			delete _mainThreadMethodForInvoke;
		}
	}
};


#endif /* __REMAINTHREADCLASSMETHODPRIVATE_H__ */ 



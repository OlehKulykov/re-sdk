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


#ifndef __REDETACHABLETHREADPRIVATE_H__
#define __REDETACHABLETHREADPRIVATE_H__

#include "../RECommonHeader.h"
#include "../REThread.h"
#include "../REMainLoopUpdatable.h"
#include "../RETime.h"

class REDetachableThreadPrivate : public REThread, public REMainLoopUpdatable 
{
private:
	REClassMethod * _reThreadClassMethod;
	REObject * _reThreadClassMethodObject;
	RETimeInterval _reThreadStartTime;
	
protected:
	virtual void ThreadBody()
	{
		if (_reThreadClassMethod)
		{
			_reThreadClassMethod->InvokeWithObject(_reThreadClassMethodObject);
		}
	}
	
public:
	/* REMainLoopUpdatable */
	virtual void Update(const RETimeInterval currentTime)
	{
		if (currentTime >= _reThreadStartTime) 
		{
			this->RemoveFromMainLoop();
			
			this->Start();
		}
	}
	
	virtual const REUIdentifier GetMainLoopUpdatableIdentifier() const { return this->GetObjectIdentifier(); }
	
	REBOOL StartWithDelay(const RETimeInterval delayTime)
	{
		if (delayTime > 0.0)
		{
			_reThreadStartTime = RETime::Time() + delayTime;
			
			this->AddToMainLoop();
		}
		else 
		{
			return this->Start();
		}
		
		return true;
	}
	
	REDetachableThreadPrivate(REClassMethod * classMethod, REObject * methodObject) : 
		REThread(),
		_reThreadClassMethod(classMethod),
		_reThreadClassMethodObject(methodObject),
		_reThreadStartTime(0.0)
	{
		if (_reThreadClassMethodObject) 
		{
			_reThreadClassMethodObject->Retain();
		}
	}
	
	virtual ~REDetachableThreadPrivate()
	{
		if (_reThreadClassMethod)
		{
			delete _reThreadClassMethod;
		}
		if (_reThreadClassMethodObject) 
		{
			_reThreadClassMethodObject->Release();
		}
	}
};

#endif


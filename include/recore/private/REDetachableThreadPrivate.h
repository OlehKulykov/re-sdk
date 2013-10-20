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
	virtual void threadBody()
	{
		if (_reThreadClassMethod)
		{
			_reThreadClassMethod->invokeWithObject(_reThreadClassMethodObject);
		}
	}
	
public:
	/* REMainLoopUpdatable */
	virtual void update(const RETimeInterval currentTime)
	{
		if (currentTime >= _reThreadStartTime) 
		{
			this->removeFromMainLoop();
			
			this->start();
		}
	}
	
	REBOOL startWithDelay(const RETimeInterval delayTime)
	{
		if (delayTime > 0.0)
		{
			_reThreadStartTime = RETime::time() + delayTime;
			
			this->addToMainLoop();
		}
		else 
		{
			return this->start();
		}
		
		return true;
	}
	
	REDetachableThreadPrivate(REClassMethod * classMethod, REObject * methodObject) : REThread(), REMainLoopUpdatable(),
		_reThreadClassMethod(classMethod),
		_reThreadClassMethodObject(methodObject),
		_reThreadStartTime(0.0)
	{
		if (_reThreadClassMethodObject) 
		{
			_reThreadClassMethodObject->retain();
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
			_reThreadClassMethodObject->release();
		}
	}
};

#endif


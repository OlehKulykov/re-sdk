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


#ifndef __REMAINTHREADCLASSMETHODWAITEDPRIVATE_H__
#define __REMAINTHREADCLASSMETHODWAITEDPRIVATE_H__

#include "../RECommonHeader.h"
#include "../REThread.h"
#include "../REMainLoopUpdatable.h"


class REMainThreadClassMethodWaitedPrivate : public REObject, public REMainLoopUpdatable
{
protected:
	REClassMethod * _mainThreadMethodForInvoke;
	REObject * _mainThreadMethodObject;
	REBOOL _isWaiting;
public:
	/* REMainLoopUpdatable */
	virtual void update(const RETimeInterval currentTime)
	{	
		_mainThreadMethodForInvoke->invokeWithObject(_mainThreadMethodObject);
		this->removeFromMainLoop();
		_isWaiting = false;
	}
	
	const REBOOL isWaiting() const 
	{
		return _isWaiting; 
	}
	
	REMainThreadClassMethodWaitedPrivate(REClassMethod * methodForInvoke, REObject * methodObject) : REObject(), REMainLoopUpdatable(),
		_mainThreadMethodForInvoke(methodForInvoke),
		_mainThreadMethodObject(methodObject),
		_isWaiting(true)
	{ 
		if (_mainThreadMethodObject) 
		{
			_mainThreadMethodObject->retain();
		}
		
		if (_mainThreadMethodForInvoke)
		{
			this->addToMainLoop();
		}
		else
		{
			_isWaiting = false;
		}
	}
	
	virtual ~REMainThreadClassMethodWaitedPrivate() 
	{
		if (_mainThreadMethodObject) 
		{
			_mainThreadMethodObject->release();
		}
		
		if ( _mainThreadMethodForInvoke ) 
		{
			delete _mainThreadMethodForInvoke;
		}
	}
};


#endif /* __REMAINTHREADCLASSMETHODWAITEDPRIVATE_H__ */ 



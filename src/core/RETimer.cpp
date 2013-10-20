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


#include "../../include/recore/RETimer.h"
#include "../../include/recore/RETime.h"

/* REObject */
const REUInt32 RETimer::getClassIdentifier() const 
{
	return RETimer::classIdentifier();
}

const REUInt32 RETimer::classIdentifier() 
{
	static const REUInt32 clasIdentif = REObject::generateClassIdentifierFromClassName("RETimer");
	return clasIdentif;
}

REBOOL RETimer::isImplementsClass(const REUInt32 classIdentifier) const
{
	return ((RETimer::classIdentifier() == classIdentifier) ||
			(REObject::generateClassIdentifierFromClassName("REMainLoopUpdatable") == classIdentifier) ||
			REObject::isImplementsClass(classIdentifier));
}

void RETimer::update(const RETimeInterval currentTime)
{
	if (_isActive)
	{
		if ( (currentTime - _startTime) >= _time )
		{
			if (_targetMethod) 
			{ 
				_targetMethod->invokeWithObject(this); 
			}
			
			if (_loopsCount == 0) 
			{
				_isActive = false;
				this->removeFromMainLoop();
			}
			else if (_loopsCount > 0)
			{
				_doneLoopsCount++;
				if (_loopsCount == _doneLoopsCount) 
				{
					_isActive = false;
					this->removeFromMainLoop();
				}
				else
				{
					_startTime = currentTime;
				}
			}
		}
	}
}

void RETimer::setTrigerTargetMethod(REClassMethod * targetMethod) 
{
	if (_targetMethod) 
	{
		delete _targetMethod;
	}
	
	_targetMethod = targetMethod; 
}

void RETimer::start()
{
	if (_time > 0.0)
	{
		_isActive = true;
		_doneLoopsCount = 0;
		_startTime = RETime::time();
		this->addToMainLoop();
	}
}

void RETimer::stop()
{
	_isActive = false;
	this->removeFromMainLoop();
}

REBOOL RETimer::isActive() const
{
	return _isActive;
}

void RETimer::setTime(const RETimeInterval time)
{
	_time = time;
}

const RETimeInterval RETimer::time() const
{
	return _time;
}

void RETimer::setLoopsCount(const REInt32 loopsCount)
{
	_loopsCount = loopsCount;
}

const REInt32 RETimer::loopsCount() const
{
	return _loopsCount;
}

RETimer::RETimer() : REObject(), REMainLoopUpdatable(),
	_targetMethod(NULL),
	_startTime(0.0),
	_time(0.0),
	_loopsCount(0),
	_doneLoopsCount(0),
	_isActive(false)
{
	
}

RETimer::~RETimer()
{
	_isActive = false;
	this->removeFromMainLoop();
	if (_targetMethod)
	{
		delete _targetMethod;
	}
}



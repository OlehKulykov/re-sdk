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
const REUInt32 RETimer::GetClassIdentifier() const 
{
	return RETimer::ClassIdentifier();
}

const REUInt32 RETimer::ClassIdentifier() 
{
	static const REUInt32 clasIdentif = REObject::GenerateClassIdentifierFromClassName("RETimer");
	return clasIdentif;
}

REBOOL RETimer::IsImplementsClass(const REUInt32 classIdentifier) const
{
	return ((RETimer::ClassIdentifier() == classIdentifier) ||
			(REObject::GenerateClassIdentifierFromClassName("REMainLoopUpdatable") == classIdentifier) ||
			REObject::IsImplementsClass(classIdentifier));
}

void RETimer::Update(const RETimeInterval currentTime)
{
	if (_isActive)
	{
		if ( (currentTime - _startTime) >= _time )
		{
			if (_targetMethod) 
			{ 
				_targetMethod->InvokeWithObject(this); 
			}
			
			if (_loopsCount == 0) 
			{
				_isActive = false;
				this->RemoveFromMainLoop();
			}
			else if (_loopsCount > 0)
			{
				_doneLoopsCount++;
				if (_loopsCount == _doneLoopsCount) 
				{
					_isActive = false;
					this->RemoveFromMainLoop();
				}
				else
				{
					_startTime = currentTime;
				}
			}
		}
	}
}

const REUIdentifier RETimer::GetMainLoopUpdatableIdentifier() const
{
	return this->GetObjectIdentifier();
}

void RETimer::SetTrigerTargetMethod(REClassMethod * targetMethod) 
{
	if (_targetMethod) 
	{
		delete _targetMethod;
	}
	
	_targetMethod = targetMethod; 
}

void RETimer::Start()
{
	if (_time > 0.0)
	{
		_isActive = true;
		_doneLoopsCount = 0;
		_startTime = RETime::Time();
		this->AddToMainLoop();
	}
}

void RETimer::Stop()
{
	_isActive = false;
	this->RemoveFromMainLoop();
}

REBOOL RETimer::IsActive() const
{
	return _isActive;
}

void RETimer::SetTime(const RETimeInterval time)
{
	_time = time;
}

const RETimeInterval RETimer::GetTime() const
{
	return _time;
}

void RETimer::SetLoopsCount(const REInt32 loopsCount)
{
	_loopsCount = loopsCount;
}

const REInt32 RETimer::GetLoopsCount() const
{
	return _loopsCount;
}

RETimer::RETimer() : REObject(),
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
	this->RemoveFromMainLoop();
	if (_targetMethod)
	{
		delete _targetMethod;
	}
}



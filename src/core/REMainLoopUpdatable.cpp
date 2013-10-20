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


#include "../../include/recore/REMainLoopUpdatable.h"
#include "../../include/recore/REApplication.h"
#include "../../include/recore/RELog.h"

REBOOL REMainLoopUpdatable::isInMainLoop() const
{
	return (_mainLoopUpdatableIdentifier != 0);
}

const REUIdentifier REMainLoopUpdatable::mainLoopUpdatableIdentifier() const
{
	return _mainLoopUpdatableIdentifier;
}

REMainLoopUpdatable::REMainLoopUpdatable() : 
	_mainLoopUpdatableIdentifier(0)
{
	
}

REMainLoopUpdatable::~REMainLoopUpdatable()
{
	if (_mainLoopUpdatableIdentifier) 
	{
		REApplication * currApp = REApplication::currentApplication();
		if (currApp) 
		{
			currApp->removeFromMainLoop(this);
		}
		else
		{
			RELog::log("Need create application");
		}
	}
}

REBOOL REMainLoopUpdatable::addToMainLoop()
{
	if (_mainLoopUpdatableIdentifier)
	{
		return true;
	}
	REApplication * currApp = REApplication::currentApplication();
	if (currApp) 
	{
		_mainLoopUpdatableIdentifier = currApp->removeFromMainLoop(this);
	}
	else
	{
		RELog::log("Need create application");
	}
	return (_mainLoopUpdatableIdentifier != 0);
}

REBOOL REMainLoopUpdatable::removeFromMainLoop()
{
	if (_mainLoopUpdatableIdentifier)
	{
		REApplication * currApp = REApplication::currentApplication();
		if (currApp) 
		{
			const REUIdentifier removedID = currApp->removeFromMainLoop(this);
			if (removedID == _mainLoopUpdatableIdentifier) 
			{
				_mainLoopUpdatableIdentifier = 0;
				return true;
			}
		}
		else
		{
			RELog::log("Need create application");
		}
		return false;
	}
	return true;
}


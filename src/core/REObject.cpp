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


#include "../../include/recore/REObject.h"
#include "../../include/recore/REAutoReleasePool.h"
#include "../../include/recore/RELog.h"
#include "../../include/recore/private/REAutoReleasePoolPrivate.h"
#include "../../include/recore/REApplication.h"

void REObject::onReleased() 
{
	
}

const REUIdentifier REObject::objectIdentifier() const
{
	return _reObjectIdentifier;
}

REBOOL REObject::isEqual(REObject * anotherObject)
{
	if (anotherObject) 
	{
		return (_reObjectIdentifier == anotherObject->_reObjectIdentifier);
	}
	return false;
}

REObject & REObject::retain() 
{
	_reObjectRetainCount++;
	return (*this); 
}

void REObject::release() 
{
	REApplication * currApp = REApplication::currentApplication();
	if (!currApp) 
	{
		RELog::log("Need create application");
	}
	
	if (_reObjectRetainCount)
	{
		_reObjectRetainCount--;
		if (_reObjectRetainCount == 0) 
		{
			if (currApp->autoReleaseObject(this))
			{
				this->onReleased();
			}
			else
			{
				RELog::log("ERROR: object %p released but not added to pool.", this);
			}
		}
	}
}

const REUInt32 REObject::retainCount() const
{
	return _reObjectRetainCount;
}

REObject::REObject() :
	_reObjectIdentifier(0),
	_reObjectRetainCount(1)
{
	union
	{
		REUIdentifier objID;
		REObject * obj;
	} u1;
	u1.obj = this;
	_reObjectIdentifier = u1.objID;
}

REObject::~REObject() 
{
	
}

void REObject::deleteObject(REObject * object)
{
    if (object) 
	{
		delete object;
	}
}




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

void * REObject::getVoidPointer()
{
	return _reObjectVoidPointer;
}

const void * REObject::getVoidPointer() const
{
	return _reObjectVoidPointer;
}

const REUIdentifier REObject::getObjectIdentifier() const
{
	return _reObjectIdentifier;
}

const REUInt32 REObject::getClassIdentifier() const
{
	return REObject::classIdentifier();	
}

REBOOL REObject::isImplementsClass(const REUInt32 classIdentifier) const
{
	return (REObject::classIdentifier() == classIdentifier);
}

const REUInt32 REObject::classIdentifier()
{	
	static const REUInt32 clasIdentif = REObject::generateClassIdentifierFromClassName("REObject");
	return clasIdentif;
}

const REUInt32 REObject::generateClassIdentifierFromClassName(const char * className)
{
	return REMD5Generator::generateFromString(className);
}

REBOOL REObject::isEqual(REObject * anotherObject)
{
	if (anotherObject) 
	{
		return (this->getObjectIdentifier() == anotherObject->getObjectIdentifier());
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
	if (_reObjectRetainCount)
	{
		_reObjectRetainCount--;
		if (_reObjectRetainCount == 0) 
		{
			/*
			REAutoReleasePoolPrivate * pool = REAutoReleasePoolPrivate::GetCurrentThreadPool();
			if (pool)
			{
				if (pool->AddObject(this))
				{
					this->OnReleased();
				}
				else
				{
					this->OnReleased();
					REObject::Delete(this);
				}
			}
			else
			{
				this->OnReleased();
				REObject::Delete(this);
			}
			*/
			if (REAutoReleasePool::getDefaultPool()->addObject(this))
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

REObject::REObject() :
	_reObjectIdentifier(0),
	_reObjectRetainCount(1)
{
	_reObjectVoidPointer = REPtrCast<void, void>(this);
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




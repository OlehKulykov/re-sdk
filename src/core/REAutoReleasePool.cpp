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


#include "../../include/recore/REAutoReleasePool.h"
#include "../../include/recore/REThread.h"

__RE_PUBLIC_CLASS_API__ REAutoReleasePool * REAutoReleasePool::_defaultPool = NULL;

#define REAUTORELEASEPOOL_DEFAULT_POOL_CAPACITY (32)

/* REObject */
const REUInt32 REAutoReleasePool::getClassIdentifier() const
{
	return REAutoReleasePool::classIdentifier();
}

const REUInt32 REAutoReleasePool::classIdentifier()
{
	static const REUInt32 clasIdentif = REObject::generateClassIdentifierFromClassName("REAutoReleasePool");
	return clasIdentif;
}

REBOOL REAutoReleasePool::isImplementsClass(const REUInt32 classIdentifier) const
{
	return ((REAutoReleasePool::classIdentifier() == classIdentifier) ||
			(REObject::generateClassIdentifierFromClassName("REMainLoopUpdatable") == classIdentifier) ||
			REObject::isImplementsClass(classIdentifier));
}

void REAutoReleasePool::update(const RETimeInterval currentTime)
{
	if (_isBusy)
	{
		return;
	}
	
	_updateMutex.lock();
	
	REBOOL isAllDeleted = true;
	_index = 0;
	const REUInt32 count = _pool.count();
	for (REUInt32 i = 0; i < count; i++) 
	{
		REObject * autoReleasableObject = _pool.at(i);
		_index++;
		if (autoReleasableObject) 
		{
			if (autoReleasableObject->getRetainCount()) 
			{
				isAllDeleted = false;
			}
			else
			{
				_pool.setAt(i, NULL);
                REObject::deleteObject(autoReleasableObject);
			}
		}
	}
	
	if ((count == _pool.count()) && isAllDeleted) 
	{
		_pool.clear();
		if (_pool.capacity() > REAUTORELEASEPOOL_DEFAULT_POOL_CAPACITY)
		{
			_pool.setCapacity(REAUTORELEASEPOOL_DEFAULT_POOL_CAPACITY);
		}
	}
	
	_index = 0;
	
	_updateMutex.unlock();
}

const REUIdentifier REAutoReleasePool::getMainLoopUpdatableIdentifier() const
{
	return this->getObjectIdentifier();
}

REBOOL REAutoReleasePool::addObject(REObject * autoReleasableObject)
{
	REBOOL addResult = false;
	_updateMutex.lock();
	_isBusy++;
	
	if (autoReleasableObject)
	{
		const REUIdentifier oid = autoReleasableObject->getObjectIdentifier();
		REUInt32 indexOfNull = RENotFound;
		const REUInt32 count = _pool.count();
		for (REUInt32 i = _index; i < count; i++) 
		{
			REObject * obj = _pool.at(i);
			if (obj) 
			{
				if (oid == obj->getObjectIdentifier()) 
				{
					_isBusy--;
					_updateMutex.unlock();
					return true; 
				}
			}
			else
			{
				indexOfNull = i;
			}
		}
		
		if (indexOfNull != RENotFound) 
		{
			addResult = _pool.setAt(indexOfNull, autoReleasableObject);
		}
		else
		{
			addResult = _pool.add(autoReleasableObject);
		}		
	}
	
	_isBusy--;
	_updateMutex.unlock();
	return addResult;
}

void REAutoReleasePool::onReleased()
{
	this->update(0.0);
}

REAutoReleasePool::REAutoReleasePool() : REObject(),
_index(0),
_isBusy(0)
{
	_updateMutex.init(REMutexTypeRecursive);
	
	_updateMutex.lock();
	_pool.setCapacity(REAUTORELEASEPOOL_DEFAULT_POOL_CAPACITY);
	_updateMutex.unlock();
}

REAutoReleasePool::~REAutoReleasePool()
{
	this->update(0.0);
}

REAutoReleasePool * REAutoReleasePool::getDefaultPool()
{
	if (_defaultPool == NULL)
	{
		_defaultPool = new REAutoReleasePool();
	}
	return _defaultPool;
}

void REAutoReleasePool::releaseDefaultPool()
{
	RE_SAFE_DELETE(_defaultPool);
}




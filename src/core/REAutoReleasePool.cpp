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


#include "../../include/recore/REAutoReleasePool.h"
#include "../../include/recore/REThread.h"

__RE_PUBLIC_CLASS_API__ REAutoReleasePool * REAutoReleasePool::_defaultPool = NULL;

#define REAUTORELEASEPOOL_DEFAULT_POOL_CAPACITY (32)

/* REObject */
const REUInt32 REAutoReleasePool::GetClassIdentifier() const
{
	return REAutoReleasePool::ClassIdentifier();
}

const REUInt32 REAutoReleasePool::ClassIdentifier()
{
	static const REUInt32 clasIdentif = REObject::GenerateClassIdentifierFromClassName("REAutoReleasePool");
	return clasIdentif;
}

REBOOL REAutoReleasePool::IsImplementsClass(const REUInt32 classIdentifier) const
{
	return ((REAutoReleasePool::ClassIdentifier() == classIdentifier) ||
			(REObject::GenerateClassIdentifierFromClassName("REMainLoopUpdatable") == classIdentifier) ||
			REObject::IsImplementsClass(classIdentifier));
}

void REAutoReleasePool::Update(const RETimeInterval currentTime)
{
	if (_isBusy)
	{
		return;
	}
	
	_updateMutex.Lock();
	
	REBOOL isAllDeleted = true;
	_index = 0;
	const REUInt32 count = _pool.Count();
	for (REUInt32 i = 0; i < count; i++) 
	{
		REObject * autoReleasableObject = _pool.At(i);
		_index++;
		if (autoReleasableObject) 
		{
			if (autoReleasableObject->GetRetainCount()) 
			{
				isAllDeleted = false;
			}
			else
			{
				_pool.SetAt(i, NULL);
                REObject::Delete(autoReleasableObject);
			}
		}
	}
	
	if ((count == _pool.Count()) && isAllDeleted) 
	{
		_pool.Clear();
		if (_pool.Capacity() > REAUTORELEASEPOOL_DEFAULT_POOL_CAPACITY)
		{
			_pool.SetCapacity(REAUTORELEASEPOOL_DEFAULT_POOL_CAPACITY);
		}
	}
	
	_index = 0;
	
	_updateMutex.Unlock();
}

const REUIdentifier REAutoReleasePool::GetMainLoopUpdatableIdentifier() const
{
	return this->GetObjectIdentifier();
}

REBOOL REAutoReleasePool::AddObject(REObject * autoReleasableObject)
{
	REBOOL addResult = false;
	_updateMutex.Lock();
	_isBusy++;
	
	if (autoReleasableObject)
	{
		const REUIdentifier oid = autoReleasableObject->GetObjectIdentifier();
		REUInt32 indexOfNull = RENotFound;
		const REUInt32 count = _pool.Count();
		for (REUInt32 i = _index; i < count; i++) 
		{
			REObject * obj = _pool.At(i);
			if (obj) 
			{
				if (oid == obj->GetObjectIdentifier()) 
				{
					_isBusy--;
					_updateMutex.Unlock();
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
			addResult = _pool.SetAt(indexOfNull, autoReleasableObject);
		}
		else
		{
			addResult = _pool.Add(autoReleasableObject);
		}		
	}
	
	_isBusy--;
	_updateMutex.Unlock();
	return addResult;
}

void REAutoReleasePool::OnReleased()
{
	this->Update(0.0);
}

REAutoReleasePool::REAutoReleasePool() : REObject(),
	_index(0),
	_isBusy(0)
{
	_updateMutex.Init(REMutexTypeRecursive);
	
	_updateMutex.Lock();
	_pool.SetCapacity(REAUTORELEASEPOOL_DEFAULT_POOL_CAPACITY);
	_updateMutex.Unlock();
}

REAutoReleasePool::~REAutoReleasePool()
{
	this->Update(0.0);
}

REAutoReleasePool * REAutoReleasePool::GetDefaultPool()
{
	if (_defaultPool == NULL)
	{
		_defaultPool = new REAutoReleasePool();
	}
	return _defaultPool;
}

void REAutoReleasePool::ReleaseDefaultPool()
{
	RE_SAFE_DELETE(_defaultPool);
}




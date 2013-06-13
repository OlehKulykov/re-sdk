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


#include "../../include/recore/private/REAutoReleasePoolPrivate.h"
#include "../../include/recore/RELog.h"


#define REAUTORELEASEPOOL_DEFAULT_POOL_CAPACITY (32)

/*
REArray<REAutoReleasePoolPrivate::ThreadPoolStruct> REAutoReleasePoolPrivate::_threadPools;

void REAutoReleasePoolPrivate::UpdatePool()
{
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

REBOOL REAutoReleasePoolPrivate::AddObject(REObject * autoReleasableObject)
{
	REBOOL addResult = false;
	_updateMutex.Lock();
	
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
	
	_updateMutex.Unlock();
	return addResult;
}

REAutoReleasePoolPrivate::REAutoReleasePoolPrivate() : REObject(),
	_index(0)
{
	_updateMutex.Init(REMutexTypeRecursive);
}

REAutoReleasePoolPrivate::~REAutoReleasePoolPrivate()
{
	
}

void REAutoReleasePoolPrivate::LastUpdate()
{
	REAutoReleasePoolPrivate * pool = REAutoReleasePoolPrivate::GetPoolForThread(REThread::GetMainThreadIdentifier());
	if (pool)
	{
		if (pool->GetObjectIdentifier() == this->GetObjectIdentifier())
		{
			pool = NULL;
		}
	}
	
	for (REUInt32 i = 0; i < _pool.Count(); i++)
	{
		REObject * o = _pool.At(i);
		if (o)
		{
			if (o->GetRetainCount())
			{
				if (pool)
				{
					if (!pool->AddObject(o))
					{
						RELog::Log("ERROR: object %p retained and thread %u finished. Can't add object to main thread pool.", o, REThread::GetCurrentThreadIdentifier());
						//REObject::Delete(o);
					}
				}
				else
				{
					RELog::Log("ERROR: object %p retained and thread %u finished. Can't add object to main thread pool because it's not found.", o, REThread::GetCurrentThreadIdentifier());
				}
			}
			else
			{
				REObject::Delete(o);
			}
		}
	}
}




void REAutoReleasePoolPrivate::Update()
{
	for (REUInt32 i = 0; i < _threadPools.Count(); i++)
	{
		REAutoReleasePoolPrivate * pool = _threadPools[i].pool;
		if (pool)
		{
			pool->UpdatePool();
		}
		else
		{
			// try clean prev error
			if (_threadPools[i].threadId == 0)
			{
				if (_threadPools.RemoveAt(i))
				{
					return;
				}
			}
		}
	}
}


REAutoReleasePoolPrivate * REAutoReleasePoolPrivate::GetCurrentThreadPool()
{
	return REAutoReleasePoolPrivate::GetPoolForThread(REThread::GetCurrentThreadIdentifier());
}

REAutoReleasePoolPrivate * REAutoReleasePoolPrivate::GetPoolForThread(const REUIdentifier threadIdentifier)
{
	if (threadIdentifier)
	{
		for (REUInt32 i = 0; i < _threadPools.Count(); i++)
		{
			if (threadIdentifier == _threadPools[i].threadId)
			{
				return _threadPools[i].pool;
			}
		}
		
		REAutoReleasePoolPrivate * newPool = new REAutoReleasePoolPrivate();
		if (newPool)
		{
			REAutoReleasePoolPrivate::ThreadPoolStruct s;
			s.threadId = threadIdentifier;
			s.pool = newPool;
			if (_threadPools.Add(s))
			{
				return newPool;
			}
			delete newPool;
		}
	}
	return NULL;
}

void REAutoReleasePoolPrivate::ThreadFinished(const REUIdentifier threadIdentifier)
{
	if (threadIdentifier)
	{
		for (REUInt32 i = 0; i < _threadPools.Count(); i++)
		{
			if (threadIdentifier == _threadPools[i].threadId)
			{
				REAutoReleasePoolPrivate * pool = _threadPools[i].pool;
				if (_threadPools.RemoveAt(i))
				{
					if ((_threadPools.Count() <= 1) && (_threadPools.Capacity() > REAUTORELEASEPOOL_DEFAULT_POOL_CAPACITY))
					{
						_threadPools.SetCapacity(4);
					}
				}
				else
				{
					/// error removing
					_threadPools[i].pool = NULL;
					_threadPools[i].threadId = 0;
				}
				pool->LastUpdate();
				delete pool;
				break;
			}
		}
	}
}
*/


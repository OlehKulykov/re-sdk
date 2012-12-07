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


#include "../../include/recore/private/REMainLoopsObjectsStoragePrivate.h"

REMainLoopsObjectsStoragePrivate * REMainLoopsObjectsStoragePrivate::_storage = NULL;

REUInt32 REMainLoopsObjectsStoragePrivate::Index(REArray<REMainLoopUpdatable *> * arr, const REUIdentifier objectId)
{
	REUInt32 i = 0;
	while (i < arr->Count())
	{
		REMainLoopUpdatable * o = arr->At(i);
		if (o)
		{
			if (objectId == o->GetMainLoopUpdatableIdentifier())
			{
				return i;
			}
		}
		i++;
	}
	return RENotFound;
}

REBOOL REMainLoopsObjectsStoragePrivate::Add(REMainLoopUpdatable * object)
{
	_updateMutex.Lock();
	
	const REUInt32 index = REMainLoopsObjectsStoragePrivate::Index(&_objects, object->GetMainLoopUpdatableIdentifier());
	if (index == RENotFound)
	{
		const REBOOL r = _objects.Add(object);
		_updateMutex.Unlock();
		return r;
	}
	
	_updateMutex.Unlock();
	return false;
}

REBOOL REMainLoopsObjectsStoragePrivate::Remove(REMainLoopUpdatable * object)
{
	_updateMutex.Lock();
	
	const REUInt32 index = REMainLoopsObjectsStoragePrivate::Index(&_objects, object->GetMainLoopUpdatableIdentifier());
	if (index != RENotFound)
	{
		const REBOOL r = _objects.RemoveAt(index);
		_updateMutex.Unlock();
		return r;
	}
	
	_updateMutex.Unlock();
	return false;
}

void REMainLoopsObjectsStoragePrivate::Update(const RETimeInterval time)
{
	_updateMutex.Lock();
	
	for (REUInt32 i = 0; i < _objects.Count(); i++)
	{
		REMainLoopUpdatable * o = _objects.At(i);
		if (o)
		{
			o->Update(time);
		}
	}
	
	_updateMutex.Unlock();
}

REBOOL REMainLoopsObjectsStoragePrivate::IsEmptyAndIDLE() const
{
	if (_updateMutex.IsInitialized())
	{
		return (_objects.IsEmpty() && !_updateMutex.IsLocked());
	}
	return _objects.IsEmpty();
}

REMainLoopsObjectsStoragePrivate::REMainLoopsObjectsStoragePrivate()
{
	_updateMutex.Init(REMutexTypeRecursive);
}

REMainLoopsObjectsStoragePrivate::~REMainLoopsObjectsStoragePrivate()
{
	
}

REMainLoopsObjectsStoragePrivate * REMainLoopsObjectsStoragePrivate::GetStorage()
{
	if (_storage)
	{
		return _storage;
	}
	
	_storage = new REMainLoopsObjectsStoragePrivate();
	return _storage;
}

void REMainLoopsObjectsStoragePrivate::ReleaseStorage()
{
	if (_storage)
	{
		REMainLoopsObjectsStoragePrivate * s = _storage;
		_storage = NULL;
		delete s;
	}
}

REBOOL REMainLoopsObjectsStoragePrivate::AddObject(REMainLoopUpdatable * object)
{
	if (object)
	{
		REMainLoopsObjectsStoragePrivate * storage = REMainLoopsObjectsStoragePrivate::GetStorage();
		if (storage)
		{
			return storage->Add(object);
		}
	}
	return false;
}

REBOOL REMainLoopsObjectsStoragePrivate::RemoveObject(REMainLoopUpdatable * object)
{
	if (object)
	{
		if (_storage)
		{
			if (_storage->Remove(object))
			{
				if (_storage->IsEmptyAndIDLE())
				{
					REMainLoopsObjectsStoragePrivate::ReleaseStorage();
				}
				return true;
			}
			else
			{
				return false;
			}
		}
		return true;
	}
	return false;
}

void REMainLoopsObjectsStoragePrivate::UpdateStorage(const RETimeInterval time)
{
	if (_storage)
	{
		_storage->Update(time);
	}
}

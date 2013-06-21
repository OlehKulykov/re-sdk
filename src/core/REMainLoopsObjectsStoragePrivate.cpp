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


#include "../../include/recore/private/REMainLoopsObjectsStoragePrivate.h"

REMainLoopsObjectsStoragePrivate * REMainLoopsObjectsStoragePrivate::_storage = NULL;

REUInt32 REMainLoopsObjectsStoragePrivate::index(REArray<REMainLoopUpdatable *> * arr, const REUIdentifier objectId)
{
	REUInt32 i = 0;
	while (i < arr->count())
	{
		REMainLoopUpdatable * o = arr->at(i);
		if (o)
		{
			if (objectId == o->getMainLoopUpdatableIdentifier())
			{
				return i;
			}
		}
		i++;
	}
	return RENotFound;
}

REBOOL REMainLoopsObjectsStoragePrivate::add(REMainLoopUpdatable * object)
{
	_updateMutex.lock();
	
	const REUInt32 index = REMainLoopsObjectsStoragePrivate::index(&_objects, object->getMainLoopUpdatableIdentifier());
	if (index == RENotFound)
	{
		const REBOOL r = _objects.add(object);
		_updateMutex.unlock();
		return r;
	}
	
	_updateMutex.unlock();
	return false;
}

REBOOL REMainLoopsObjectsStoragePrivate::remove(REMainLoopUpdatable * object)
{
	_updateMutex.lock();
	
	const REUInt32 index = REMainLoopsObjectsStoragePrivate::index(&_objects, object->getMainLoopUpdatableIdentifier());
	if (index != RENotFound)
	{
		const REBOOL r = _objects.removeAt(index);
		_updateMutex.unlock();
		return r;
	}
	
	_updateMutex.unlock();
	return false;
}

void REMainLoopsObjectsStoragePrivate::update(const RETimeInterval time)
{
	_updateMutex.lock();
	
	for (REUInt32 i = 0; i < _objects.count(); i++)
	{
		REMainLoopUpdatable * o = _objects.at(i);
		if (o)
		{
			o->update(time);
		}
	}
	
	_updateMutex.unlock();
}

REBOOL REMainLoopsObjectsStoragePrivate::isEmptyAndIDLE() const
{
	if (_updateMutex.isInitialized())
	{
		return (_objects.isEmpty() && !_updateMutex.isLocked());
	}
	return _objects.isEmpty();
}

REMainLoopsObjectsStoragePrivate::REMainLoopsObjectsStoragePrivate()
{
	_updateMutex.init(REMutexTypeRecursive);
}

REMainLoopsObjectsStoragePrivate::~REMainLoopsObjectsStoragePrivate()
{
	
}

REMainLoopsObjectsStoragePrivate * REMainLoopsObjectsStoragePrivate::getStorage()
{
	if (_storage)
	{
		return _storage;
	}
	
	_storage = new REMainLoopsObjectsStoragePrivate();
	return _storage;
}

void REMainLoopsObjectsStoragePrivate::releaseStorage()
{
	if (_storage)
	{
		REMainLoopsObjectsStoragePrivate * s = _storage;
		_storage = NULL;
		delete s;
	}
}

REBOOL REMainLoopsObjectsStoragePrivate::addObject(REMainLoopUpdatable * object)
{
	if (object)
	{
		REMainLoopsObjectsStoragePrivate * storage = REMainLoopsObjectsStoragePrivate::getStorage();
		if (storage)
		{
			return storage->add(object);
		}
	}
	return false;
}

REBOOL REMainLoopsObjectsStoragePrivate::removeObject(REMainLoopUpdatable * object)
{
	if (object)
	{
		if (_storage)
		{
			if (_storage->remove(object))
			{
				if (_storage->isEmptyAndIDLE())
				{
					REMainLoopsObjectsStoragePrivate::releaseStorage();
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

void REMainLoopsObjectsStoragePrivate::updateStorage(const RETimeInterval time)
{
	if (_storage)
	{
		_storage->update(time);
	}
}

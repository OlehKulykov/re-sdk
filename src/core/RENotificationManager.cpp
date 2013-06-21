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


#include "../../include/recore/RENotificationManager.h"
#include "../../include/recore/REMutex.h"
#include "../../include/recore/REArray.h"


class RENotificationManagerObserverPrivate
{
public:
	REObject * observer;
	REClassMethod * observerMethod;
	REString name;
	void TryInvoke(const REString & n, REObject * obj)
	{
		if (name.isEqual(n) && observerMethod)
		{
			observerMethod->invokeWithObject(obj);
		}
	}
	RENotificationManagerObserverPrivate(REObject * o,
										 REClassMethod * om,
										 const REString & n) :
	observer(o),
	observerMethod(om)
	{ name.set(n); }
	~RENotificationManagerObserverPrivate()
	{
		if (observerMethod) { delete observerMethod; }
	}
};

class RENotificationManagerPrivate
{
private:
	REArray<RENotificationManagerObserverPrivate *> _observers;
	REMutex _updateMutex;
	
	void Clear();
	RENotificationManagerPrivate();
	~RENotificationManagerPrivate();
public:
	REBOOL IsEmpty() const { return _observers.isEmpty(); }
	REBOOL AddObserverForNotificationName(REObject * observerObject, const REString & notificationName, REClassMethod * observerMethod);
	REBOOL RemoveObserver(REObject * observerObject);
	REBOOL RemoveObserverForNotificationName(REObject * observerObject, const REString & notificationName);
	REBOOL PostNotificationName(const REString & notificationName);
	REBOOL PostNotificationNameWithObject(const REString & notificationName, REObject * object);
	
	static RENotificationManagerPrivate * manager;
	static RENotificationManagerPrivate * GetManager();
	static void ReleaseManager();
};

RENotificationManagerPrivate * RENotificationManagerPrivate::manager = NULL;

REBOOL RENotificationManagerPrivate::AddObserverForNotificationName(REObject * observerObject,
																	const REString & notificationName,
																	REClassMethod * observerMethod)
{
	_updateMutex.lock();
	RENotificationManagerObserverPrivate * newObserver = new RENotificationManagerObserverPrivate(observerObject,
																								  observerMethod,
																								  notificationName);
	if (newObserver)
	{
		const REBOOL r = _observers.add(newObserver);
		if (!r) { delete newObserver; }
		_updateMutex.unlock();
		return r;
	}
	_updateMutex.unlock();
	return false;
}

REBOOL RENotificationManagerPrivate::RemoveObserver(REObject * observerObject)
{
	_updateMutex.lock();
	const REUIdentifier objID = observerObject->getObjectIdentifier();
	REArray<RENotificationManagerObserverPrivate *>::Iterator i = _observers.getIterator();
	while (i.next())
	{
		RENotificationManagerObserverPrivate * o = i.object();
		if (objID == o->observer->getObjectIdentifier())
		{
			const REUInt32 index = i.index();
			if (!i.removeObject()) { _observers.setAt(index, NULL); }
			delete o;
		}
	}
	_updateMutex.unlock();
	return false;
}

REBOOL RENotificationManagerPrivate::RemoveObserverForNotificationName(REObject * observerObject, const REString & notificationName)
{
	_updateMutex.lock();
	const REUIdentifier objID = observerObject->getObjectIdentifier();
	REArray<RENotificationManagerObserverPrivate *>::Iterator i = _observers.getIterator();
	while (i.next())
	{
		RENotificationManagerObserverPrivate * o = i.object();
		if (objID == o->observer->getObjectIdentifier())
		{
			if (o->name.isEqual(notificationName))
			{
				const REUInt32 index = i.index();
				if (!i.removeObject()) { _observers.setAt(index, NULL); }
				delete o;
			}
		}
	}
	_updateMutex.unlock();
	return false;
}

REBOOL RENotificationManagerPrivate::PostNotificationName(const REString & notificationName)
{
	_updateMutex.lock();
	const REBOOL r = this->PostNotificationNameWithObject(notificationName, NULL);
	_updateMutex.unlock();
	return r;
}

REBOOL RENotificationManagerPrivate::PostNotificationNameWithObject(const REString & notificationName, REObject * object)
{
	_updateMutex.lock();
	for (REUInt32 i = 0; i < _observers.count(); i++)
	{
		RENotificationManagerObserverPrivate * o = _observers.at(i);
		if (o) { o->TryInvoke(notificationName, object); }
	}
	_updateMutex.unlock();
	return false;
}


RENotificationManagerPrivate * RENotificationManagerPrivate::GetManager()
{
	if (RENotificationManagerPrivate::manager) { return RENotificationManagerPrivate::manager; }
	RENotificationManagerPrivate::manager = new RENotificationManagerPrivate();
	return RENotificationManagerPrivate::manager;
}

void RENotificationManagerPrivate::ReleaseManager()
{
	RENotificationManagerPrivate * m = RENotificationManagerPrivate::manager;
	RENotificationManagerPrivate::manager = NULL;
	if (m) { delete m; }
}

RENotificationManagerPrivate::RENotificationManagerPrivate()
{
	_updateMutex.init(REMutexTypeRecursive);
}

void RENotificationManagerPrivate::Clear()
{
	_updateMutex.lock();
	for (REUInt32 i = 0; i < _observers.count(); i++)
	{
		RENotificationManagerObserverPrivate * o = _observers.at(i);
		if (o) { _observers.setAt(i, NULL); delete o; }
	}
	_updateMutex.unlock();
}

RENotificationManagerPrivate::~RENotificationManagerPrivate()
{
	this->Clear();
}


REBOOL RENotificationManager::addObserverForNotificationName(REObject * observerObject,
															 const REString & notificationName,
															 REClassMethod * observerMethod)
{
	if (observerObject && observerMethod && (!notificationName.isEmpty()))
	{
		RENotificationManagerPrivate * m = RENotificationManagerPrivate::GetManager();
		if (m)
		{
			return m->AddObserverForNotificationName(observerObject,
													 notificationName,
													 observerMethod);
		}
	}
	return false;
}

REBOOL RENotificationManager::removeObserver(REObject * observerObject)
{
	if (observerObject)
	{
		RENotificationManagerPrivate * m = RENotificationManagerPrivate::manager;
		if (m)
		{
			if (m->RemoveObserver(observerObject))
			{
				if (m->IsEmpty())
				{
					RENotificationManagerPrivate::ReleaseManager();
				}
				return true;
			}
		}
	}
	return false;
}

REBOOL RENotificationManager::removeObserverForNotificationName(REObject * observerObject,
																const REString & notificationName)
{
	if (observerObject && (!notificationName.isEmpty()))
	{
		RENotificationManagerPrivate * m = RENotificationManagerPrivate::manager;
		if (m)
		{
			if (m->RemoveObserverForNotificationName(observerObject,
													 notificationName))
			{
				if (m->IsEmpty())
				{
					RENotificationManagerPrivate::ReleaseManager();
				}
				return true;
			}
		}
	}
	return false;
}

REBOOL RENotificationManager::postNotificationName(const REString & notificationName)
{
	if (!notificationName.isEmpty())
	{
		RENotificationManagerPrivate * m = RENotificationManagerPrivate::manager;
		if (m)
		{
			m->PostNotificationName(notificationName);
		}
	}
	return false;
}

REBOOL RENotificationManager::postNotificationNameWithObject(const REString & notificationName,
															 REObject * object)
{
	if (!notificationName.isEmpty())
	{
		RENotificationManagerPrivate * m = RENotificationManagerPrivate::manager;
		if (m)
		{
			return m->PostNotificationNameWithObject(notificationName,
													 object);
		}
	}
	return false;
}


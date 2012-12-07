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
		if (name.IsEqual(n) && observerMethod)
		{
			observerMethod->InvokeWithObject(obj);
		}
	}
	RENotificationManagerObserverPrivate(REObject * o,
										 REClassMethod * om,
										 const REString & n) :
	observer(o),
	observerMethod(om)
	{ name.Set(n); }
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
	REBOOL IsEmpty() const { return _observers.IsEmpty(); }
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
	_updateMutex.Lock();
	RENotificationManagerObserverPrivate * newObserver = new RENotificationManagerObserverPrivate(observerObject,
																								  observerMethod,
																								  notificationName);
	if (newObserver)
	{
		const REBOOL r = _observers.Add(newObserver);
		if (!r) { delete newObserver; }
		_updateMutex.Unlock();
		return r;
	}
	_updateMutex.Unlock();
	return false;
}

REBOOL RENotificationManagerPrivate::RemoveObserver(REObject * observerObject)
{
	_updateMutex.Lock();
	const REUIdentifier objID = observerObject->GetObjectIdentifier();
	REArray<RENotificationManagerObserverPrivate *>::Iterator i = _observers.GetIterator();
	while (i.Next())
	{
		RENotificationManagerObserverPrivate * o = i.Object();
		if (objID == o->observer->GetObjectIdentifier())
		{
			const REUInt32 index = i.Index();
			if (!i.RemoveObject()) { _observers.SetAt(index, NULL); }
			delete o;
		}
	}
	_updateMutex.Unlock();
	return false;
}

REBOOL RENotificationManagerPrivate::RemoveObserverForNotificationName(REObject * observerObject, const REString & notificationName)
{
	_updateMutex.Lock();
	const REUIdentifier objID = observerObject->GetObjectIdentifier();
	REArray<RENotificationManagerObserverPrivate *>::Iterator i = _observers.GetIterator();
	while (i.Next())
	{
		RENotificationManagerObserverPrivate * o = i.Object();
		if (objID == o->observer->GetObjectIdentifier())
		{
			if (o->name.IsEqual(notificationName))
			{
				const REUInt32 index = i.Index();
				if (!i.RemoveObject()) { _observers.SetAt(index, NULL); }
				delete o;
			}
		}
	}
	_updateMutex.Unlock();
	return false;
}

REBOOL RENotificationManagerPrivate::PostNotificationName(const REString & notificationName)
{
	_updateMutex.Lock();
	const REBOOL r = this->PostNotificationNameWithObject(notificationName, NULL);
	_updateMutex.Unlock();
	return r;
}

REBOOL RENotificationManagerPrivate::PostNotificationNameWithObject(const REString & notificationName, REObject * object)
{
	_updateMutex.Lock();
	for (REUInt32 i = 0; i < _observers.Count(); i++)
	{
		RENotificationManagerObserverPrivate * o = _observers.At(i);
		if (o) { o->TryInvoke(notificationName, object); }
	}
	_updateMutex.Unlock();
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
	_updateMutex.Init(REMutexTypeRecursive);
}

void RENotificationManagerPrivate::Clear()
{
	_updateMutex.Lock();
	for (REUInt32 i = 0; i < _observers.Count(); i++)
	{
		RENotificationManagerObserverPrivate * o = _observers.At(i);
		if (o) { _observers.SetAt(i, NULL); delete o; }
	}
	_updateMutex.Unlock();
}

RENotificationManagerPrivate::~RENotificationManagerPrivate()
{
	this->Clear();
}


REBOOL RENotificationManager::AddObserverForNotificationName(REObject * observerObject,
															 const REString & notificationName,
															 REClassMethod * observerMethod)
{
	if (observerObject && observerMethod && (!notificationName.IsEmpty()))
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

REBOOL RENotificationManager::RemoveObserver(REObject * observerObject)
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

REBOOL RENotificationManager::RemoveObserverForNotificationName(REObject * observerObject,
																const REString & notificationName)
{
	if (observerObject && (!notificationName.IsEmpty()))
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

REBOOL RENotificationManager::PostNotificationName(const REString & notificationName)
{
	if (!notificationName.IsEmpty())
	{
		RENotificationManagerPrivate * m = RENotificationManagerPrivate::manager;
		if (m)
		{
			m->PostNotificationName(notificationName);
		}
	}
	return false;
}

REBOOL RENotificationManager::PostNotificationNameWithObject(const REString & notificationName,
															 REObject * object)
{
	if (!notificationName.IsEmpty())
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


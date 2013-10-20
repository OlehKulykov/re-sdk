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


#include "../../include/recore/REApplication.h"
#include "../../include/recore/REThread.h"
#include "../../include/recore/RECoreC.h"
#include "../../include/recore/private/REMainLoopsObjectsStoragePrivate.h"
#include "../../include/recore/private/REAutoReleasePoolPrivate.h"
#include "../../include/recore/RELog.h"
#include "../../include/recore/private/REMainLoopsObjectsStoragePrivate.h"

class REApplicationInternalAutoreleaseThread : public REThread
{
private:
	REAutoReleasePoolPrivate * _pool;
protected:	
	virtual void threadBody()
	{
		_pool->update();
		delete _pool;
	}
public:
	REApplicationInternalAutoreleaseThread(REAutoReleasePoolPrivate * pool) : REThread(),
		_pool(pool) 
	{ 
		this->setAutoReleaseWhenDone(true);
	}
};

class REApplicationInternal 
{
public:
	REAutoReleasePool * autoReleasePool;
	REAutoReleasePoolPrivate * arp;
	REMainLoopsObjectsStoragePrivate * mainLoop;
	REThread * autoreleaseThread;
	REMutex mainLoopUpdatableMutex;
	REMutex autoreleaseMutex;
	
	REBOOL isOk() const
	{
		if (!autoReleasePool) return false;
		if (!mainLoop) return false;
		if (!mainLoopUpdatableMutex.isInitialized()) return false;
		if (!autoreleaseMutex.isInitialized()) return false;
		return true;
	}
	
	REApplicationInternal() :
		autoReleasePool(NULL),
		arp(NULL),
		mainLoop(NULL),
		autoreleaseThread(NULL)
	{
		mainLoopUpdatableMutex.init(REMutexTypeRecursive);
		autoreleaseMutex.init(REMutexTypeRecursive);
		
		autoReleasePool = new REAutoReleasePool();
		mainLoop = new REMainLoopsObjectsStoragePrivate();
	}
	
	~REApplicationInternal()
	{
		if (autoReleasePool) delete autoReleasePool;
		if (mainLoop) delete mainLoop;
	}
	
	static REApplication * currentApplication;
};

REApplication * REApplicationInternal::currentApplication = NULL;

REString REApplication::name() const
{
	return REString("REApplication");
}

REFloat32 REApplication::version() const
{
	return 0.2f;
}

REString REApplication::description() const
{
	return REString("This is base application");
}

REBOOL REApplication::isOK() const
{
	return (_a != NULL);
}

REBOOL REApplication::pause()
{
	return RETime::pause();
}

REBOOL REApplication::resume()
{
	return RETime::resume();
}

REBOOL REApplication::isPaused() const
{
	return RETime::isPaused();
}

void REApplication::update()
{
	if (_a->arp) 
	{
		_a->autoreleaseMutex.lock();
		REAutoReleasePoolPrivate * pool = _a->arp;
		_a->arp = NULL;
		if (pool) 
		{
			REApplicationInternalAutoreleaseThread * t = new REApplicationInternalAutoreleaseThread(pool);
			if (t) { t->start(); }
			else { delete pool; }
		}
		_a->autoreleaseMutex.unlock();
	}
	
	_a->mainLoopUpdatableMutex.lock();
	_a->mainLoop->update(RETime::time());
	_a->mainLoopUpdatableMutex.unlock();
}

REBOOL REApplication::reapplicationInit()
{	
	if (_a)
	{
		return true;
	}
	
	REThread::isMainThread();
		
	if (RECore::isCorrectTypes() == 0)
	{
		RELog::log("ERROR: RECore types is incorrect. Check type defines.");
		return false;
	}

	_a = new REApplicationInternal();
	if (!_a)
	{
		RELog::log("ERROR: REApplication can't initialize internal object.");
		return false;
	}
	
	if (!_a->isOk()) 
	{
		delete _a;
		_a = NULL;
		RELog::log("ERROR: REApplication can't initialize internal logic.");
		return false;
	}
	
	return true;
}

REBOOL REApplication::initialize()
{
	if (_a) 
	{
		return true;
	}
	return this->reapplicationInit();
}

REBOOL REApplication::autoReleaseObject(REObject * object)
{
	_a->autoreleaseMutex.lock();
	if (!_a->arp) 
	{
		_a->arp = new REAutoReleasePoolPrivate(); 
	}
	const REBOOL r = (object && _a->arp) ? _a->arp->addObject(object) : false;
	_a->autoreleaseMutex.unlock();
	return r;
}

REUIdentifier REApplication::addToMainLoop(REMainLoopUpdatable * object)
{
	_a->mainLoopUpdatableMutex.lock();
	const REUIdentifier identifier = object ? _a->mainLoop->add(object) : 0;
	_a->mainLoopUpdatableMutex.unlock();
	return identifier;
}

REUIdentifier REApplication::removeFromMainLoop(REMainLoopUpdatable * object)
{
	_a->mainLoopUpdatableMutex.lock();
	const REUIdentifier identifier = object ? _a->mainLoop->remove(object) : 0;
	_a->mainLoopUpdatableMutex.unlock();
	return identifier;
}

REApplication::REApplication() :
	_a(NULL)
{	
	if (this->reapplicationInit())
	{
		if (!REApplicationInternal::currentApplication) 
		{
			REApplicationInternal::currentApplication = this;
		}
	}
}

REApplication::~REApplication()
{
	if (REApplicationInternal::currentApplication == this) 
	{
		REApplicationInternal::currentApplication = NULL;
	}
	
	if (_a) 
	{
		delete _a;
	}
}

REApplication * REApplication::currentApplication()
{
	return REApplicationInternal::currentApplication;
}



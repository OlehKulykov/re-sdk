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
#include "../../include/recore/RELog.h"
#include "../../include/recore/RECRC32Generator.h"
#include "../../include/recore/private/REMainLoopsObjectsStoragePrivate.h"
#include "../../include/recore/private/REAutoReleasePoolPrivate.h"
#include "../../include/recore/private/REMainLoopsObjectsStoragePrivate.h"


class REApplicationInternalNewClassCallback
{
public:
	REApplicationNewClassForNameCallback callback;
	REUInt32 nameCRC32;
	REApplicationInternalNewClassCallback & operator=(const REApplicationInternalNewClassCallback & c)
	{
		callback = c.callback;
		nameCRC32 = c.nameCRC32;
		return (*this); 
	}
	REApplicationInternalNewClassCallback(const REApplicationInternalNewClassCallback & c) :
	callback(c.callback), nameCRC32(c.nameCRC32) { }
	REApplicationInternalNewClassCallback(const REApplicationNewClassForNameCallback c, const REUInt32 ncrc32) :
	 callback(c), nameCRC32(ncrc32) { }
};


class REApplicationInternal 
{
public:
	REAutoReleasePoolPrivate pool;
	REUInt32 poolItems;
	
	REMainLoopsObjectsStoragePrivate * mainLoop;
	REThread * autoreleaseThread;
	REMutex mainLoopUpdatableMutex;
	REMutex autoreleaseMutex;
	REList<REApplicationInternalNewClassCallback> classCallbacks;
	
	REBOOL isOk() const
	{
		if (!mainLoop) return false;
		if (!mainLoopUpdatableMutex.isInitialized()) return false;
		if (!autoreleaseMutex.isInitialized()) return false;
		return true;
	}
	
	REApplicationInternal() :
		poolItems(0),
		mainLoop(NULL),
		autoreleaseThread(NULL)
	{
		mainLoopUpdatableMutex.init(REMutexTypeRecursive);
		autoreleaseMutex.init(REMutexTypeRecursive);
		
		mainLoop = new REMainLoopsObjectsStoragePrivate();
	}
	
	~REApplicationInternal()
	{
		pool.update();
		if (mainLoop) delete mainLoop;
	}
	
	static REApplication * currentApplication;
};

REApplication * REApplicationInternal::currentApplication = NULL;



REBOOL REApplication::registerNewClassForNameCallback(const REApplicationNewClassForNameCallback callback, const char * className)
{
	if (callback) 
	{
		const REUInt32 nCRC32 = RECRC32Generator::generateFromString(className);
		if (nCRC32 > 0) 
		{
			REApplicationInternalNewClassCallback c(callback, nCRC32);
			return _a->classCallbacks.add(c);
		}
	}
	return false;
}

void * REApplication::createObjectWithClassName(const char * className)
{
	const REUInt32 nCRC32 = RECRC32Generator::generateFromString(className);
	if (nCRC32 > 0) 
	{
		REList<REApplicationInternalNewClassCallback>::Iterator iter = _a->classCallbacks.iterator();
		while (iter.next()) 
		{
			if (iter.value().nameCRC32 == nCRC32) 
			{
				return iter.value().callback();
			}
		}
	}
	return NULL;
}

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
	if (_a->poolItems) 
	{
		_a->autoreleaseMutex.lock();
		if (_a->poolItems) 
		{
			_a->pool.update(); 
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
	const REBOOL r = _a->pool.addObject(object);
	_a->poolItems++;
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



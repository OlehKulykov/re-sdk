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
#include "../../include/recore/RECPUFeatures.h"
#include "../../include/recore/private/REMainLoopsObjectsStoragePrivate.h"
#include "../../include/recore/private/REAutoReleasePoolPrivate.h"


/* REObject */
const REUInt32 REApplication::getClassIdentifier() const
{
	return REApplication::classIdentifier();
}

const REUInt32 REApplication::classIdentifier()
{
	static const REUInt32 clasIdentif = REObject::generateClassIdentifierFromClassName("REApplication");
	return clasIdentif;
}

REBOOL REApplication::isImplementsClass(const REUInt32 classIdentifier) const
{
	return ((REApplication::classIdentifier() == classIdentifier) || REObject::isImplementsClass(classIdentifier));
}

const REString & REApplication::getErrorDescriptionString() const
{
	return _errorDescriptionString;
}

void REApplication::addToErrorDescription(const REString & errorString)
{
	const char * errStr = errorString.getChars();
	if (errStr && (!errorString.isEmpty())) 
	{
		if (_errorDescriptionString.isEmpty()) 
		{
			_errorDescriptionString = errStr;
		}
		else
		{
			_errorDescriptionString.appendFormat("\n%s", errStr);
		}
	}
}

void REApplication::clearErrorDescription()
{
	_errorDescriptionString.clear();
}

REString REApplication::getName() const
{
	return REString("REApplication");
}

REFloat32 REApplication::getVersion() const
{
	return 0.0f;
}

REString REApplication::getDescription() const
{
	return REString("This is base application");
}

REBOOL REApplication::isOK() const
{
	return true;
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
	//REAutoReleasePoolPrivate::Update();
	
	_appAutoReleasePool->update(0);
	
	REMainLoopsObjectsStoragePrivate::updateStorage(RETime::time());
}

REBOOL REApplication::reapplicationInit()
{
	REThread::isMainThread();
	
	this->clearErrorDescription();
	
	if (_appAutoReleasePool)
	{
		/// allready initialized.
		return true;
	}
	
	REAutoReleasePool * pool = REAutoReleasePool::getDefaultPool();
	if (pool) 
	{
		_appAutoReleasePool = pool;
		return true;
	}
	else
	{
		this->addToErrorDescription(REString("Can't initialize application default REAutoReleasePool."));
	}
	return false;
}

REBOOL REApplication::initialize()
{
	return reapplicationInit();
}

REApplication::REApplication() : REObject(),
	_appAutoReleasePool(NULL)
{
	this->reapplicationInit();
}

void REApplication::onReleased()
{
	this->update();
	
	REAutoReleasePool::releaseDefaultPool();
}

REApplication::~REApplication()
{
	
}




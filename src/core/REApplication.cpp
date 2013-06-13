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
const REUInt32 REApplication::GetClassIdentifier() const
{
	return REApplication::ClassIdentifier();
}

const REUInt32 REApplication::ClassIdentifier()
{
	static const REUInt32 clasIdentif = REObject::GenerateClassIdentifierFromClassName("REApplication");
	return clasIdentif;
}

REBOOL REApplication::IsImplementsClass(const REUInt32 classIdentifier) const
{
	return ((REApplication::ClassIdentifier() == classIdentifier) || REObject::IsImplementsClass(classIdentifier));
}

const REString & REApplication::GetErrorDescriptionString() const
{
	return _errorDescriptionString;
}

void REApplication::AddToErrorDescription(const REString & errorString)
{
	const char * errStr = errorString.UTF8String();
	if (errStr && (!errorString.IsEmpty())) 
	{
		if (_errorDescriptionString.IsEmpty()) 
		{
			_errorDescriptionString.Set(errStr);
		}
		else
		{
			_errorDescriptionString.AppendFormat("\n%s", errStr);
		}
	}
}

void REApplication::ClearErrorDescription()
{
	_errorDescriptionString.Clear();
}

REString REApplication::GetName() const
{
	return REString("REApplication");
}

REFloat32 REApplication::GetVersion() const
{
	return 0.0f;
}

REString REApplication::GetDescription() const
{
	return REString("This is base application");
}

REBOOL REApplication::IsOK() const
{
	return true;
}

REBOOL REApplication::Pause()
{
	return RETime::Pause();
}

REBOOL REApplication::Resume()
{
	return RETime::Resume();
}

REBOOL REApplication::IsPaused() const
{
	return RETime::IsPaused();
}

void REApplication::Update()
{
	//REAutoReleasePoolPrivate::Update();
	
	_appAutoReleasePool->Update(0);
	
	REMainLoopsObjectsStoragePrivate::UpdateStorage(RETime::Time());
}

REBOOL REApplication::REApplicationInit()
{
	REThread::IsMainThread();
	
	this->ClearErrorDescription();
	
	if (_appAutoReleasePool)
	{
		/// allready initialized.
		return true;
	}
	
	REAutoReleasePool * pool = REAutoReleasePool::GetDefaultPool();
	if (pool) 
	{
		_appAutoReleasePool = pool;
		return true;
	}
	else
	{
		this->AddToErrorDescription(REString("Can't initialize application default REAutoReleasePool."));
	}
	return false;
}

REBOOL REApplication::Initialize()
{
	return REApplicationInit();
}

REApplication::REApplication() : REObject(),
	_appAutoReleasePool(NULL)
{
	this->REApplicationInit();
}

void REApplication::OnReleased()
{
	this->Update();
	
	REAutoReleasePool::ReleaseDefaultPool();
}

REApplication::~REApplication()
{
	
}




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


#include "../../include/recore/REObject.h"
#include "../../include/recore/REAutoReleasePool.h"
#include "../../include/recore/RELog.h"
#include "../../include/recore/private/REAutoReleasePoolPrivate.h"

const REUInt32 REObject::GetClassIdentifier() const
{
	return REObject::ClassIdentifier();	
}

REBOOL REObject::IsImplementsClass(const REUInt32 classIdentifier) const
{
	return (REObject::ClassIdentifier() == classIdentifier);
}

const REUInt32 REObject::ClassIdentifier()
{	
	static const REUInt32 clasIdentif = REObject::GenerateClassIdentifierFromClassName("REObject");
	return clasIdentif;
}

const REUInt32 REObject::GenerateClassIdentifierFromClassName(const char * className)
{
	return REMD5Generator::GenerateFromString(className);
}

REBOOL REObject::IsEqual(REObject * anotherObject)
{
	if (anotherObject) 
	{
		return (this->GetObjectIdentifier() == anotherObject->GetObjectIdentifier());
	}
	return false;
}

REObject & REObject::Retain() 
{
	_reObjectRetainCount++;
	return (*this); 
}

void REObject::Release() 
{
	if (_reObjectRetainCount)
	{
		_reObjectRetainCount--;
		if (_reObjectRetainCount == 0) 
		{
			/*
			REAutoReleasePoolPrivate * pool = REAutoReleasePoolPrivate::GetCurrentThreadPool();
			if (pool)
			{
				if (pool->AddObject(this))
				{
					this->OnReleased();
				}
				else
				{
					this->OnReleased();
					REObject::Delete(this);
				}
			}
			else
			{
				this->OnReleased();
				REObject::Delete(this);
			}
			*/
			if (REAutoReleasePool::GetDefaultPool()->AddObject(this))
			{
				this->OnReleased();
			}
			else
			{
				RELog::Log("ERROR: object %p released but not added to pool.", this);
			}
		}
	}
}

REObject::REObject() :
	_reObjectIdentifier(0),
	_reObjectRetainCount(1)
{
	_reObjectIdentifier = (REUIdentifier)this;
}

REObject::~REObject() 
{
	
}

void REObject::Delete(REObject * object)
{
    if (object) 
	{
		delete object;
	}
}




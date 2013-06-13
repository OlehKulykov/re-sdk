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


#include "../../include/recore/REBufferObject.h"

const REUInt32 REBufferObject::GetClassIdentifier() const
{
	return REBufferObject::ClassIdentifier();
}

const REUInt32 REBufferObject::ClassIdentifier()
{
	static const REUInt32 clasIdentif = REObject::GenerateClassIdentifierFromClassName("REBufferObject");
	return clasIdentif;
}

REBOOL REBufferObject::IsImplementsClass(const REUInt32 classIdentifier) const
{
	return ((REBufferObject::ClassIdentifier() == classIdentifier) || REObject::IsImplementsClass(classIdentifier) ||
			(REObject::GenerateClassIdentifierFromClassName("REBuffer") == classIdentifier));
}

REBOOL REBufferObject::IsEqual(REObject * anotherObject)
{
	if (REObject::IsEqual(anotherObject)) { return true; }
	
	if (anotherObject) 
	{
		if (anotherObject->GetClassIdentifier() == REBufferObject::ClassIdentifier()) 
		{
			REBufferObject * buff = (REBufferObject*)anotherObject;
			if ( buff->GetSize() == this->GetSize() ) 
			{
				return (memcmp(buff->GetBuffer(), this->GetBuffer(), this->GetSize()) == 0);
			}
		}
	}
	return false;
}

REBufferObject::REBufferObject() : REBuffer(), REObject() 
{
	
}

REBufferObject::REBufferObject(const REUInt32 buffSize) : REBuffer(buffSize), REObject() 
{
	
}

REBufferObject::REBufferObject(const void * buff, const REUInt32 buffSize) : REBuffer(buff, buffSize), REObject()
{
	
}

REBufferObject::~REBufferObject() 
{
	
}

REBufferObject * REBufferObject::Create()
{
	REBufferObject * newBuff = new REBufferObject();
	return newBuff;
}

REBufferObject * REBufferObject::CreateWithSize(const REUInt32 buffSize)
{
	REBufferObject * newBuff = new REBufferObject(buffSize);
	if (newBuff) 
	{
		if (newBuff->GetSize() == buffSize) 
		{
			return newBuff;
		}
		delete newBuff;
	}
	return NULL;
}

REBufferObject * REBufferObject::CreateWithStringObject(REStringObject * str)
{
	if (str) 
	{
		if (str->Length()) 
		{
			REBufferObject * newBuff = new REBufferObject();
			if (newBuff) 
			{
				if (newBuff->Set(str->UTF8String(), str->Length()))
				{
					return newBuff;
				}
				
				delete newBuff;
			}
		}
	}
	return NULL;
}

REBufferObject * REBufferObject::CreateWithString(const REString & str)
{
	if (str.Length()) 
	{
		REBufferObject * newBuff = new REBufferObject();
		if (newBuff) 
		{
			if (newBuff->Set(str.UTF8String(), str.Length()))
			{
				return newBuff;
			}
			
			delete newBuff;
		}
	}
	return NULL;
}

REBufferObject * REBufferObject::CreateWithMemory(const void * memoryBuff, const REUInt32 memoryBuffSize)
{
	if (memoryBuff && memoryBuffSize) 
	{
		REBufferObject * newBuff = new REBufferObject();
		if (newBuff) 
		{
			if (newBuff->Set(memoryBuff, memoryBuffSize))
			{
				return newBuff;
			}
			
			delete newBuff;
		}
	}
	return NULL;
}




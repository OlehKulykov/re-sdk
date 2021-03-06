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

REBOOL REBufferObject::isEqual(REObject * anotherObject)
{	
	if (anotherObject) 
	{
		if (REObject::isEqual(anotherObject)) 
		{
			return true; 
		}
		
		REBuffer * buff = dynamic_cast<REBuffer *>(anotherObject);
		if (buff) 
		{
			if (buff->size() == this->size()) 
			{
				return (memcmp(buff->buffer(), this->buffer(), this->size()) == 0);
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

REBufferObject * REBufferObject::create()
{
	REBufferObject * newBuff = new REBufferObject();
	return newBuff;
}

REBufferObject * REBufferObject::createWithSize(const REUInt32 buffSize)
{
	REBufferObject * newBuff = new REBufferObject(buffSize);
	if (newBuff) 
	{
		if (newBuff->size() == buffSize) 
		{
			return newBuff;
		}
		delete newBuff;
	}
	return NULL;
}

REBufferObject * REBufferObject::createWithStringObject(REStringObject * str)
{
	if (str) 
	{
		if (str->length()) 
		{
			REBufferObject * newBuff = new REBufferObject();
			if (newBuff) 
			{
				if (newBuff->set(str->UTF8String(), str->length()))
				{
					return newBuff;
				}
				
				delete newBuff;
			}
		}
	}
	return NULL;
}

REBufferObject * REBufferObject::createWithString(const REString & str)
{
	if (str.isNotEmpty()) 
	{
		REBufferObject * newBuff = new REBufferObject();
		if (newBuff) 
		{
			if (newBuff->set(str.UTF8String(), str.length()))
			{
				return newBuff;
			}
			
			delete newBuff;
		}
	}
	return NULL;
}

REBufferObject * REBufferObject::createWithMemory(const void * memoryBuff, const REUInt32 memoryBuffSize)
{
	if (memoryBuff && memoryBuffSize) 
	{
		REBufferObject * newBuff = new REBufferObject();
		if (newBuff) 
		{
			if (newBuff->set(memoryBuff, memoryBuffSize))
			{
				return newBuff;
			}
			
			delete newBuff;
		}
	}
	return NULL;
}




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


#include "../../include/recore/RETypedPtr.h"
#include "../../include/recore/REDictionary.h"
#include "../../include/recore/REObject.h"
#include "../../include/recore/RETypedArray.h"

class RETypedPtrPrivate 
{
public:
	static REBOOL compareStrings(REString * obj1, REString * obj2)
	{
		return obj1->isEqual(*obj2);
	}
	
	static REBOOL compareNumbers(RENumber * obj1, RENumber * obj2)
	{
		return obj1->isEqualToNumber(*obj2);
	}
	
	static REBOOL compareBuffers(REBuffer * obj1, REBuffer * obj2)
	{
		return obj1->isEqualToBuffer(*obj2);
	}
	
	static REBOOL compareArrays(RETypedArray * obj1, RETypedArray * obj2)
	{
		if (obj1->count() == obj2->count())
		{
			for (REUInt32 i = 0; i < obj1->count(); i++) 
			{
				if (!((*obj1)[i]).isEqualToTypedPointer((*obj2)[i]))
				{
					return false;
				}
			}
			return true;
		}
		return false;
	}
	
	static REBOOL compareDictionaries(REDictionary * obj1, REDictionary * obj2)
	{
		return obj1->isEqualToDictionary(*obj2);
	}
	
	static REBOOL compareDates(REDate * obj1, REDate * obj2)
	{
		return obj1->isEqualToDate(*obj2);
	}
};

REBOOL RETypedPtr::isEqualToTypedPointer(const RETypedPtr & anotherPtr) const
{
	if (_type != anotherPtr._type)
	{
		/// not the same types
		return false;
	}
	
	if (_object && anotherPtr._object)
	{
		/// pointers is same
		if (_object == anotherPtr._object)
		{
			return true;
		}
		
		switch (_type) 
		{
			case REPtrTypeString:
				return RETypedPtrPrivate::compareStrings((REString *)_object, (REString *)anotherPtr._object);
				break;
				
			case REPtrTypeNumber:
				return RETypedPtrPrivate::compareNumbers((RENumber *)_object, (RENumber *)anotherPtr._object);
				break;
				
			case REPtrTypeNull:
				return true;
				break;
				
			case REPtrTypeBuffer:
				return RETypedPtrPrivate::compareBuffers((REBuffer *)_object, (REBuffer *)anotherPtr._object);
				break;
				
			case REPtrTypeArray:
				return RETypedPtrPrivate::compareArrays((RETypedArray *)_object, (RETypedArray *)anotherPtr._object);
				break;
				
			case REPtrTypeDictionary:	
				return RETypedPtrPrivate::compareDictionaries((REDictionary *)_object, (REDictionary *)anotherPtr._object);
				break;
			
			case REPtrTypeDate:
				return RETypedPtrPrivate::compareDates((REDate *)_object, (REDate *)anotherPtr._object);
				break;
				
			default:
				break;
		}
	}
	
	return false;
}

bool RETypedPtr::operator==(const RETypedPtr & anotherPtr) const
{
	return this->isEqualToTypedPointer(anotherPtr);
}

bool RETypedPtr::operator!=(const RETypedPtr & anotherPtr) const
{
	return (!this->isEqualToTypedPointer(anotherPtr));
}

REBOOL RETypedPtr::isEmpty() const 
{
	return (_object == (void *)0);
}

REBOOL RETypedPtr::isNotEmpty() const 
{
	return (_object != (void *)0); 
}

void RETypedPtr::retain()
{
	if (_referenceCount)
	{
		*_referenceCount = (*_referenceCount) + 1;
	}
}

void RETypedPtr::release()
{
	if (_referenceCount)
	{
		*_referenceCount = (*_referenceCount) - 1;
		if ((*_referenceCount) <= 0)
		{
			this->deleteObject();
			
			free(_referenceCount);
			_referenceCount = (REInt32 *)0;
		}
	}
	
	_object = (void *)0;
	_referenceCount = (REInt32 *)0;
	_type = REPtrTypeNone;
}

void RETypedPtr::deleteObject()
{
	if (_object)
	{
		switch (_type) 
		{
			case REPtrTypeString:
			{
				REString * str = (REString *)_object;
				delete str;
				_object = (void *)0;
			}
				break;
				
			case REPtrTypeNumber:
			{
				RENumber * num = (RENumber *)_object;
				delete num;
				_object = (void *)0;
			}
				break;
				
			case REPtrTypeArray:
			{
				REArray<RETypedPtr> * arr = (REArray<RETypedPtr> *)_object;
				delete arr;
				_object = (void *)0;
			}
				break;
			
			case REPtrTypeNull:	
			{
				_object = (void *)0;
				
			}
				break;
				
			case REPtrTypeBuffer:
			{
				REBuffer * buff = (REBuffer *)_object;
				delete buff;
				_object = (void *)0;
			}
				break;
				
			case REPtrTypeDictionary:
			{
				REDictionary * dict = (REDictionary *)_object;
				delete dict;
				_object = (void *)0;
			}
				break;
			
			case REPtrTypeVoidPointer:
				_object = (void *)0;
				break;
				
			case REPtrTypeDate:
			{
				REDate * date = (REDate *)_object;
				delete date;
				_object = (void *)0;
			}
				break;
			default:
				break;
		}
	}
}

RETypedPtr & RETypedPtr::operator=(const RETypedPtr & anotherPtr)
{
	if (this != &anotherPtr)
	{
		this->release();
		
		if (anotherPtr.isNotEmpty())
		{
			_object = anotherPtr._object;
			_referenceCount = anotherPtr._referenceCount;
			_type = anotherPtr._type;
			this->retain();
		}
	}
	return (*this);
}

REString * RETypedPtr::getString() const
{
	return (_type == REPtrTypeString) ? (REString *)_object : (REString *)0;
}

RENumber * RETypedPtr::getNumber() const
{
	return (_type == REPtrTypeNumber) ? (RENumber *)_object : (RENumber *)0;
}

RETypedArray * RETypedPtr::getArray() const
{
	return (_type == REPtrTypeArray) ? (RETypedArray *)_object : (RETypedArray *)0;
}

RENULLObject * RETypedPtr::getNULL() const
{
	return (_type == REPtrTypeNull) ? (RENULLObject *)_object : (RENULLObject *)0;
}

REBuffer * RETypedPtr::getBuffer() const
{
	return (_type == REPtrTypeBuffer) ? (REBuffer *)_object : (REBuffer *)0;
}

REDictionary * RETypedPtr::getDictionary() const
{
	return (_type == REPtrTypeDictionary) ? (REDictionary *)_object : (REDictionary *)0;
}

void * RETypedPtr::getVoidPointer() const
{
	return (_type == REPtrTypeVoidPointer) ? _object : 0;
}

REDate * RETypedPtr::getDate() const
{
	return (_type == REPtrTypeDate) ? (REDate *)_object : (REDate *)0;
}

const REPtrType RETypedPtr::getType() const
{
	return _type;
}

RETypedPtr::RETypedPtr(const RETypedPtr & anotherPtr) :
	_object((void *)0),
	_referenceCount((REInt32 *)0),
	_type(REPtrTypeNone)
{
	if (this != &anotherPtr)
	{
		if (anotherPtr.isNotEmpty())
		{
			_object = anotherPtr._object;
			_referenceCount = anotherPtr._referenceCount;
			_type = anotherPtr._type;
			this->retain();
		}
	}
}

RETypedPtr::RETypedPtr(void * object, const REPtrType type) :
	_object(object),
	_referenceCount((REInt32 *)0),
	_type(type)
{
	if (_object)
	{
		REInt32 * count = (REInt32 *)malloc(sizeof(REInt32 *));
		if (count)
		{
			*count = 1;
			_referenceCount = count;
		}
		else
		{
			_object = (void *)0;
		}
	}
}

RETypedPtr::RETypedPtr() :
	_object((void *)0),
	_referenceCount((REInt32 *)0),
	_type(REPtrTypeNone)
{
	
}

RETypedPtr::~RETypedPtr()
{
	this->release();
}

REBOOL RETypedPtr::isNotEmpty(RETypedPtr * ptr)
{
	return ptr ? (ptr->_object != (void *)0) : false;
}





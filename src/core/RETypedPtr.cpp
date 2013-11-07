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
#include "../../include/recore/REString.h"
#include "../../include/recore/RENumber.h"
#include "../../include/recore/RETypedArray.h"
#include "../../include/recore/RENULL.h"
#include "../../include/recore/REBuffer.h"
#include "../../include/recore/REDictionary.h"
#include "../../include/recore/REDate.h"


class RETypedPtrPrivate 
{
public:
	static REBOOL compareStrings(REString * obj1, REString * obj2);
	static REBOOL compareNumbers(RENumber * obj1, RENumber * obj2);
	static REBOOL compareBuffers(REBuffer * obj1, REBuffer * obj2);	
	static REBOOL compareArrays(RETypedArray * obj1, RETypedArray * obj2);	
	static REBOOL compareDictionaries(REDictionary * obj1, REDictionary * obj2);	
	static REBOOL compareDates(REDate * obj1, REDate * obj2);
	static void deleteObject(void * obj1, const REPtrType type);
};

REBOOL RETypedPtrPrivate::compareStrings(REString * obj1, REString * obj2)
{
	return obj1->isEqual(*obj2);
}

REBOOL RETypedPtrPrivate::compareNumbers(RENumber * obj1, RENumber * obj2)
{
	return obj1->isEqualToNumber(*obj2);
}

REBOOL RETypedPtrPrivate::compareBuffers(REBuffer * obj1, REBuffer * obj2)
{
	return obj1->isEqualToBuffer(*obj2);
}

REBOOL RETypedPtrPrivate::compareArrays(RETypedArray * obj1, RETypedArray * obj2)
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

REBOOL RETypedPtrPrivate::compareDictionaries(REDictionary * obj1, REDictionary * obj2)
{
	return obj1->isEqualToDictionary(*obj2);
}

REBOOL RETypedPtrPrivate::compareDates(REDate * obj1, REDate * obj2)
{
	return obj1->isEqualToDate(*obj2);
}

void RETypedPtrPrivate::deleteObject(void * obj1, const REPtrType type)
{
	switch (type) 
	{
		case REPtrTypeString:
			delete REPtrCast<REString, void>(obj1);
			break;
			
		case REPtrTypeNumber:
			delete REPtrCast<RENumber, void>(obj1);
			break;
			
		case REPtrTypeArray:
			delete REPtrCast<REArray<RETypedPtr>, void>(obj1);
			break;
			
		case REPtrTypeNull:	
			delete REPtrCast<RENULL, void>(obj1);
			break;
			
		case REPtrTypeBuffer:
			delete REPtrCast<REBuffer, void>(obj1);
			break;
			
		case REPtrTypeDictionary:
			delete REPtrCast<REDictionary, void>(obj1);
			break;
			
		case REPtrTypeDate:
			delete REPtrCast<REDate, void>(obj1);
			break;
						
		default:
			break;
	}
}


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
				return RETypedPtrPrivate::compareStrings(REPtrCast<REString, void>(_object), 
														 REPtrCast<REString, void>(anotherPtr._object));
				break;
				
			case REPtrTypeNumber:
				return RETypedPtrPrivate::compareNumbers(REPtrCast<RENumber, void>(_object), 
														 REPtrCast<RENumber, void>(anotherPtr._object));
				break;
				
			case REPtrTypeNull:
				return (REPtrCast<RENULL, void>(_object))->isEqualToNULL(*(REPtrCast<RENULL, void>(anotherPtr._object)));
				break;
				
			case REPtrTypeBuffer:
				return RETypedPtrPrivate::compareBuffers(REPtrCast<REBuffer, void>(_object), 
														 REPtrCast<REBuffer, void>(anotherPtr._object));
				break;
				
			case REPtrTypeArray:
				return RETypedPtrPrivate::compareArrays(REPtrCast<RETypedArray, void>(_object), 
														REPtrCast<RETypedArray, void>(anotherPtr._object));
				break;
				
			case REPtrTypeDictionary:	
				return RETypedPtrPrivate::compareDictionaries(REPtrCast<REDictionary, void>(_object), 
															  REPtrCast<REDictionary, void>(anotherPtr._object));
				break;
			
			case REPtrTypeDate:
				return RETypedPtrPrivate::compareDates(REPtrCast<REDate, void>(_object), 
													   REPtrCast<REDate, void>(anotherPtr._object));
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

REInt32 RETypedPtr::referenceCount() const
{
	return _referenceCount ? (*_referenceCount) : 0;
}

REBOOL RETypedPtr::isEmpty() const 
{
	return (_object == NULL);
}

REBOOL RETypedPtr::isNotEmpty() const 
{
	return (_object != NULL); 
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
			_referenceCount = NULL;
		}
	}
	
	_object = NULL;
	_referenceCount = NULL;
	_type = REPtrTypeNone;
}

void RETypedPtr::deleteObject()
{
	if (_object)
	{
		RETypedPtrPrivate::deleteObject(_object, _type);
		_object = NULL;
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

REString * RETypedPtr::string() const
{
	return (_type == REPtrTypeString) ? REPtrCast<REString, void>(_object) : NULL;
}

RENumber * RETypedPtr::number() const
{
	return (_type == REPtrTypeNumber) ? REPtrCast<RENumber, void>(_object) : NULL;
}

RETypedArray * RETypedPtr::array() const
{
	return (_type == REPtrTypeArray) ? REPtrCast<RETypedArray, void>(_object) : NULL;
}

RENULL * RETypedPtr::getNULL() const
{
	return (_type == REPtrTypeNull) ? REPtrCast<RENULL, void>(_object) : NULL;
}

REBuffer * RETypedPtr::buffer() const
{
	return (_type == REPtrTypeBuffer) ? REPtrCast<REBuffer, void>(_object) : NULL;
}

REDictionary * RETypedPtr::dictionary() const
{
	return (_type == REPtrTypeDictionary) ? REPtrCast<REDictionary, void>(_object) : NULL;
}

void * RETypedPtr::voidPointer() const
{
	return (_type == REPtrTypeVoidPointer) ? _object : NULL;
}

REDate * RETypedPtr::date() const
{
	return (_type == REPtrTypeDate) ? REPtrCast<REDate, void>(_object) : NULL;
}

const REPtrType RETypedPtr::type() const
{
	return _type;
}

RETypedPtr::RETypedPtr(const RETypedPtr & anotherPtr) :
	_object(NULL),
	_referenceCount(NULL),
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
	_referenceCount(NULL),
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
			_object = NULL;
		}
	}
}

RETypedPtr::RETypedPtr() :
	_object(NULL),
	_referenceCount(NULL),
	_type(REPtrTypeNone)
{
	
}

RETypedPtr::~RETypedPtr()
{
	this->release();
}

REBOOL RETypedPtr::isNotEmpty(const RETypedPtr * ptr)
{
	return ptr ? (ptr->_object != NULL) : false;
}

REBOOL RETypedPtr::isNotEmpty(const RETypedPtr * ptr, const REPtrType type)
{
	return ptr ? ((ptr->_object != NULL) && (ptr->_type == type)) : false;
}




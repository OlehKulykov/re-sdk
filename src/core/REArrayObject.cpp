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


#include "../../include/recore/REArrayObject.h"

const REUInt32 REArrayObject::getClassIdentifier() const
{
	return REArrayObject::classIdentifier();
}

const REUInt32 REArrayObject::classIdentifier()
{
	static const REUInt32 clasIdentif = REObject::generateClassIdentifierFromClassName("REArrayObject");
	return clasIdentif;
}

REBOOL REArrayObject::isImplementsClass(const REUInt32 classIdentifier) const
{
	return ((REArrayObject::classIdentifier() == classIdentifier) || REObject::isImplementsClass(classIdentifier));
}

const REObjectsIterator REArrayObject::getIterator() const
{
	REArray<REObject*> * a = const_cast<REArray<REObject*>*>(&_arr);
	return REArray<REObject*>::Iterator(a);
}

REBOOL REArrayObject::isEqualWithObjectsArray(REObject * anotherObject, REArray<REObject *> * arr)
{
	if (anotherObject)
	{
		if (anotherObject->getClassIdentifier() == REArrayObject::classIdentifier())
		{
			REArrayObject * anotherArray = (REArrayObject *)anotherObject;
			
			if (anotherArray->_arr.count() == arr->count())
			{
				for (REUInt32 i = 0; i < arr->count(); i++)
				{
					if ( !(*arr)[i]->isEqual((REObject*)anotherArray->_arr[i]) )
					{
						return false;
					}
				}
				return true;
			}
		}
	}
	return false;
}

REBOOL REArrayObject::isEqual(REObject * anotherObject)
{
	if (REObject::isEqual(anotherObject)) 
	{
		return true; 
	}
	
	return REArrayObject::isEqualWithObjectsArray(anotherObject, &_arr);
}

/*
 Linear search for object
 Return object index or RENotFound if not found */
const REUInt32 REArrayObject::search(const REObject * object) const
{
	REObject * o = const_cast<REObject *>(object);
	REArray<REObject *> * arr = const_cast<REArray<REObject *> *>(&_arr);
	for (REUInt32 i = 0; i < _arr.count(); i++) 
	{
		if ((*arr)[i]->isEqual(o))
		{
			return i;
		}
	}
	return RENotFound;
}

REBOOL REArrayObject::setCapacity(const REUInt32 newCapacity)
{
	return _arr.setCapacity(newCapacity);
}

const REUInt32 REArrayObject::capacity() const 
{
	return _arr.capacity(); 
}

/// Returns objects count of array
const REUInt32 REArrayObject::count() const 
{
	return _arr.count(); 
}

/// Check is array empty
REBOOL REArrayObject::isEmpty() const 
{
	return _arr.isEmpty(); 
}

REBOOL REArrayObject::swap(const REUInt32 firstIndex, const REUInt32 secondIndex)
{
	const REUInt32 count = _arr.count();
	if ( (firstIndex < count) && (secondIndex < count) )
	{
		REObject * o = _arr[firstIndex];
		_arr[firstIndex] = _arr[secondIndex];
		_arr[secondIndex] = o;
		return true;
	}
	return NULL;
}

REObject * REArrayObject::at(const REUInt32 index) const
{
	return _arr.at(index);
}

REObject * REArrayObject::lastObject() const
{
	const REUInt32 count = _arr.count();
	return (count > 0) ? _arr.at(count - 1) : NULL;
}

REObject * REArrayObject::firstObject() const
{
	return (_arr.count() > 0) ? _arr.at(0) : NULL;
}

REBOOL REArrayObject::add(REObject * object)
{
	if (object) 
	{
		if (_arr.add(object)) 
		{
			object->retain();
			return true;
		}
	}
	return false;
}

REBOOL REArrayObject::add(const REArrayObject & anotherArray)
{
	return this->add(anotherArray._arr);
}

REBOOL REArrayObject::add(const REArray<REObject *> & anotherArray)
{
	if (anotherArray.count()) 
	{
		if (_arr.setCapacity( _arr.count() + anotherArray.count() + 1 )) 
		{
			REUInt32 arrIndex = _arr.count();
			for (REUInt32 i = 0; i < anotherArray.count(); i++) 
			{
				_arr[arrIndex] = anotherArray.at(i);
				_arr[arrIndex]->retain();
				arrIndex++;
			}
		}
		else
		{
			return false;
		}
	}
	return true;
}

REBOOL REArrayObject::removeAt(const REUInt32 index)
{
	if (index < _arr.count()) 
	{
		REObject * o = _arr[index];
		if (_arr.removeAt(index)) 
		{
			o->release();
			return true;
		}
	}
	return false;
}

REBOOL REArrayObject::remove(REObject * object)
{
	if (object) 
	{
		for (REUInt32 i = 0; i < _arr.count(); i++) 
		{
			if ( _arr[i]->isEqual(object) )
			{
				const REBOOL isRemoved = _arr.removeAt(i);
				if (isRemoved) 
				{
					object->release();
				}
				return isRemoved;
			}
		} 
	}
	return false;
}

REBOOL REArrayObject::remove(const REArrayObject & anotherArray)
{
	return this->remove(anotherArray._arr);
}

REBOOL REArrayObject::remove(const REArray<REObject *> & anotherArray)
{
	for (REUInt32 i = 0; i < anotherArray.count(); i++) 
	{
		this->remove(anotherArray.at(i));
	}
	return true;
}

REBOOL REArrayObject::insert(const REUInt32 insertAtIndex, REObject * object)
{
	if (object)
	{
		if (_arr.insert(insertAtIndex, object)) 
		{
			object->retain();
			return true;
		}
	}
	return false;
}

REBOOL REArrayObject::replace(REObject * object, REObject * withObject)
{
	if (object && withObject) 
	{
		for (REUInt32 i = 0; i < _arr.count(); i++) 
		{
			if (_arr[i]->isEqual(object))
			{
				object->release();
				_arr[i] = withObject;
				withObject->retain();
				return true;
			}
		} 
	}
	return false;
}

REBOOL REArrayObject::replace(const REUInt32 replaceAtIndex, REObject * withObject)
{
	if (withObject) 
	{
		if (replaceAtIndex < _arr.count())
		{
			_arr[replaceAtIndex]->release();
			
			_arr[replaceAtIndex] = withObject;
			withObject->retain();
			return true;
		}
	}
	
	return false;
}

void REArrayObject::clear()
{
	for (REUInt32 i = 0; i < _arr.count(); i++) 
	{
		_arr[i]->release();
	}
	_arr.clear();
}

REArrayObject & REArrayObject::set(const REArrayObject & anotherArray)
{
	this->set(anotherArray._arr);
	return (*this);
}

REArrayObject & REArrayObject::set(const REArray<REObject *> & anotherArray)
{
	this->clear();
	
	if (_arr.setCapacity(anotherArray.count() + 1))
	{
		for (REUInt32 i = 0; i < anotherArray.count(); i++) 
		{
			REObject * o = anotherArray.at(i);
			_arr.add(o);
			o->retain();
		}
	}
	
	return (*this);
}
/*
 REObject* & REArrayObject::operator[](const REUInt32 & index)
 {
 return _arr[index];
 }
 */
REArrayObject & REArrayObject::operator=(const REArrayObject & anotherArray)
{
	this->set(anotherArray);
	return (*this);
}

REArrayObject & REArrayObject::operator=(const REArray<REObject *> & anotherArray)
{
	this->set(anotherArray);
	return (*this);
}

REArrayObject::REArrayObject(const REUInt32 newCapacity) : REObject()
{
	_arr.setCapacity(newCapacity);
}

REArrayObject::REArrayObject(const REArrayObject & anotherArray) : REObject()
{
	this->set(anotherArray);
}

REArrayObject::REArrayObject(const REArray<REObject *> & anotherArray) : REObject()
{
	this->set(anotherArray);
}

REArrayObject::REArrayObject() : REObject()
{
	
}

void REArrayObject::onReleased()
{
	this->clear();
}

REArrayObject::~REArrayObject()
{
	
}

REArrayObject * REArrayObject::create()
{
	REArrayObject * newArr = new REArrayObject();
	return newArr;	
}

REArrayObject * REArrayObject::createWithCapacity(const REUInt32 newCapacity)
{
	REArrayObject * newArr = new REArrayObject(newCapacity);
	return newArr;
}

REArrayObject * REArrayObject::createWithObjectsArray(const REArrayObject * anotherArray)
{
	if (anotherArray) 
	{
		REArrayObject * newArr = new REArrayObject(*anotherArray);
		if (newArr) 
		{
			if (newArr->count() == anotherArray->count()) 
			{
				return newArr;
			}
			delete newArr;
		}
	}
	return NULL;
}

REArrayObject * REArrayObject::createWithArray(const REArray<REObject *> & anotherArray)
{
	REArrayObject * newArr = new REArrayObject(anotherArray);
	if (newArr)
	{
		if (newArr->count() == anotherArray.count())
		{
			return newArr;
		}
		delete newArr;
	}
	return NULL;
}





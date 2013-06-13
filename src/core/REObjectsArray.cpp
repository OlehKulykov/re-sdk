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


#include "../../include/recore/REObjectsArray.h"

const REUInt32 REObjectsArray::GetClassIdentifier() const
{
	return REObjectsArray::ClassIdentifier();
}

const REUInt32 REObjectsArray::ClassIdentifier()
{
	static const REUInt32 clasIdentif = REObject::GenerateClassIdentifierFromClassName("REObjectsArray");
	return clasIdentif;
}

REBOOL REObjectsArray::IsImplementsClass(const REUInt32 classIdentifier) const
{
	return ((REObjectsArray::ClassIdentifier() == classIdentifier) || REObject::IsImplementsClass(classIdentifier));
}

const REObjectsIterator REObjectsArray::GetIterator() const
{
	REArray<REObject*> * a = const_cast<REArray<REObject*>*>(&_arr);
	return REArray<REObject*>::Iterator(a);
}

REBOOL REObjectsArray::IsEqualWithObjectsArray(REObject * anotherObject, REArray<REObject *> * arr)
{
	if (anotherObject)
	{
		if (anotherObject->GetClassIdentifier() == REObjectsArray::ClassIdentifier())
		{
			REObjectsArray * anotherArray = (REObjectsArray *)anotherObject;
			
			if (anotherArray->_arr.Count() == arr->Count())
			{
				for (REUInt32 i = 0; i < arr->Count(); i++)
				{
					if ( !(*arr)[i]->IsEqual((REObject*)anotherArray->_arr[i]) )
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

REBOOL REObjectsArray::IsEqual(REObject * anotherObject)
{
	if (REObject::IsEqual(anotherObject)) { return true; }
	
	return REObjectsArray::IsEqualWithObjectsArray(anotherObject, &_arr);
}

/*
 Linear search for object
 Return object index or RENotFound if not found */
const REUInt32 REObjectsArray::Search(const REObject * object) const
{
	REObject * o = const_cast<REObject *>(object);
	REArray<REObject *> * arr = const_cast<REArray<REObject *> *>(&_arr);
	for (REUInt32 i = 0; i < _arr.Count(); i++) 
	{
		if ((*arr)[i]->IsEqual(o))
		{
			return i;
		}
	}
	return RENotFound;
}

REBOOL REObjectsArray::SetCapacity(const REUInt32 newCapacity)
{
	return _arr.SetCapacity(newCapacity);
}
/*
 const REUInt32 REObjectsArray::Count() const
 {
 return _arr.Count();
 }
 */
/*
 REBOOL REObjectsArray::IsEmpty() const
 {
 return _arr.IsEmpty();
 }
 */
REBOOL REObjectsArray::Swap(const REUInt32 firstIndex, const REUInt32 secondIndex)
{
	const REUInt32 count = _arr.Count();
	if ( (firstIndex < count) && (secondIndex < count) )
	{
		REObject * o = _arr[firstIndex];
		_arr[firstIndex] = _arr[secondIndex];
		_arr[secondIndex] = o;
		return true;
	}
	return NULL;
}

REObject * REObjectsArray::At(const REUInt32 index) const
{
	if (index < _arr.Count()) 
	{
		return _arr.At(index);
	}
	return NULL;
}

REObject * REObjectsArray::LastObject() const
{
	const REUInt32 count = _arr.Count();
	if (count)
	{
		return _arr.At(count - 1);
	}
	return NULL;
}

REObject * REObjectsArray::FirstObject() const
{
	if (_arr.Count()) 
	{
		return _arr.At(0);
	}
	return NULL;
}

REBOOL REObjectsArray::Add(REObject * object)
{
	if (object) 
	{
		if (_arr.Add(object)) 
		{
			object->Retain();
			return true;
		}
	}
	return false;
}

REBOOL REObjectsArray::Add(const REObjectsArray & anotherArray)
{
	return this->Add(anotherArray._arr);
}

REBOOL REObjectsArray::Add(const REArray<REObject *> & anotherArray)
{
	if (anotherArray.Count()) 
	{
		if (_arr.SetCapacity( _arr.Count() + anotherArray.Count() + 1 )) 
		{
			REUInt32 arrIndex = _arr.Count();
			for (REUInt32 i = 0; i < anotherArray.Count(); i++) 
			{
				_arr[arrIndex] = anotherArray.At(i);
				_arr[arrIndex]->Retain();
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

REBOOL REObjectsArray::RemoveAt(const REUInt32 index)
{
	if (index < _arr.Count()) 
	{
		REObject * o = _arr[index];
		if (_arr.RemoveAt(index)) 
		{
			o->Release();
			return true;
		}
	}
	return false;
}

REBOOL REObjectsArray::Remove(REObject * object)
{
	if (object) 
	{
		for (REUInt32 i = 0; i < _arr.Count(); i++) 
		{
			if ( _arr[i]->IsEqual(object) )
			{
				const REBOOL isRemoved = _arr.RemoveAt(i);
				if (isRemoved) 
				{
					object->Release();
				}
				return isRemoved;
			}
		} 
	}
	return false;
}

REBOOL REObjectsArray::Remove(const REObjectsArray & anotherArray)
{
	return this->Remove(anotherArray._arr);
}

REBOOL REObjectsArray::Remove(const REArray<REObject *> & anotherArray)
{
	for (REUInt32 i = 0; i < anotherArray.Count(); i++) 
	{
		this->Remove(anotherArray.At(i));
	}
	return true;
}

REBOOL REObjectsArray::Insert(const REUInt32 insertAtIndex, REObject * object)
{
	if (object)
	{
		if (_arr.Insert(insertAtIndex, object)) 
		{
			object->Retain();
			return true;
		}
	}
	return false;
}

REBOOL REObjectsArray::Replace(REObject * object, REObject * withObject)
{
	if (object && withObject) 
	{
		for (REUInt32 i = 0; i < _arr.Count(); i++) 
		{
			if (_arr[i]->IsEqual(object))
			{
				object->Release();
				_arr[i] = withObject;
				withObject->Retain();
				return true;
			}
		} 
	}
	return false;
}

REBOOL REObjectsArray::Replace(const REUInt32 replaceAtIndex, REObject * withObject)
{
	if (withObject) 
	{
		if (replaceAtIndex < _arr.Count())
		{
			_arr[replaceAtIndex]->Release();
			
			_arr[replaceAtIndex] = withObject;
			withObject->Retain();
			return true;
		}
	}
	
	return false;
}

void REObjectsArray::Clear()
{
	for (REUInt32 i = 0; i < _arr.Count(); i++) 
	{
		_arr[i]->Release();
	}
	_arr.Clear();
}

REObjectsArray & REObjectsArray::Set(const REObjectsArray & anotherArray)
{
	this->Set(anotherArray._arr);
	return (*this);
}

REObjectsArray & REObjectsArray::Set(const REArray<REObject *> & anotherArray)
{
	this->Clear();
	
	if (_arr.SetCapacity(anotherArray.Count() + 1))
	{
		for (REUInt32 i = 0; i < anotherArray.Count(); i++) 
		{
			REObject * o = anotherArray.At(i);
			_arr.Add(o);
			o->Retain();
		}
	}
	
	return (*this);
}
/*
 REObject* & REObjectsArray::operator[](const REUInt32 & index)
 {
 return _arr[index];
 }
 */
REObjectsArray & REObjectsArray::operator=(const REObjectsArray & anotherArray)
{
	this->Set(anotherArray);
	return (*this);
}

REObjectsArray & REObjectsArray::operator=(const REArray<REObject *> & anotherArray)
{
	this->Set(anotherArray);
	return (*this);
}

REObjectsArray::REObjectsArray(const REUInt32 newCapacity) : REObject()
{
	_arr.SetCapacity(newCapacity);
}

REObjectsArray::REObjectsArray(const REObjectsArray & anotherArray) : REObject()
{
	this->Set(anotherArray);
}

REObjectsArray::REObjectsArray(const REArray<REObject *> & anotherArray) : REObject()
{
	this->Set(anotherArray);
}

REObjectsArray::REObjectsArray() : REObject()
{
	
}

void REObjectsArray::OnReleased()
{
	this->Clear();
}

REObjectsArray::~REObjectsArray()
{
	
}

REObjectsArray * REObjectsArray::Create()
{
	REObjectsArray * newArr = new REObjectsArray();
	return newArr;	
}

REObjectsArray * REObjectsArray::CreateWithCapacity(const REUInt32 newCapacity)
{
	REObjectsArray * newArr = new REObjectsArray(newCapacity);
	return newArr;
}

REObjectsArray * REObjectsArray::CreateWithObjectsArray(const REObjectsArray * anotherArray)
{
	if (anotherArray) 
	{
		REObjectsArray * newArr = new REObjectsArray(*anotherArray);
		if (newArr) 
		{
			if (newArr->Count() == anotherArray->Count()) 
			{
				return newArr;
			}
			delete newArr;
		}
	}
	return NULL;
}

REObjectsArray * REObjectsArray::CreateWithArray(const REArray<REObject *> & anotherArray)
{
	REObjectsArray * newArr = new REObjectsArray(anotherArray);
	if (newArr)
	{
		if (newArr->Count() == anotherArray.Count())
		{
			return newArr;
		}
		delete newArr;
	}
	return NULL;
}





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


#include "../../include/recore/RETypedArray.h"
#include "../../include/recore/RETypedPtr.h"

void RETypedArray::clearTyped()
{
	for (REUInt32 i = 0; i < _count; i++) 
	{
		_arr[i].release();
	}
	REArray<RETypedPtr>::clear();
}

void RETypedArray::clear()
{
	this->clearTyped();
}

RETypedArray & RETypedArray::operator=(const RETypedArray & anotherArray)
{
	this->clearTyped();
	for (REUInt32 i = 0; i < anotherArray.count(); i++) 
	{
		this->add(anotherArray[i]);
	}
	return (*this);
}

RETypedArray & RETypedArray::operator=(const REArray<RETypedPtr> & anotherArray)
{
	this->clearTyped();
	for (REUInt32 i = 0; i < anotherArray.count(); i++) 
	{
		this->add(anotherArray[i]);
	}
	return (*this);
}

RETypedArray::RETypedArray(REUInt32 newCapacity) : REArray<RETypedPtr>(newCapacity)
{
	
}

RETypedArray::RETypedArray(const RETypedArray & anotherArray) :  REArray<RETypedPtr>(anotherArray.count())
{
	for (REUInt32 i = 0; i < anotherArray.count(); i++) 
	{
		this->add(anotherArray[i]);
	}
}

RETypedArray::RETypedArray(const REArray<RETypedPtr> & anotherArray) : REArray<RETypedPtr>(anotherArray.count())
{
	for (REUInt32 i = 0; i < anotherArray.count(); i++) 
	{
		this->add(anotherArray[i]);
	}
}

RETypedArray::~RETypedArray()
{
	this->clearTyped();
}



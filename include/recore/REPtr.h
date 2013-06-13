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


#ifndef __REPTR_H__
#define __REPTR_H__

#include "RECommonHeader.h"
#include <stdlib.h>
#include <inttypes.h>

template <typename PointerType>
class REPtr 
{
private:
	PointerType* _object;
	int32_t* _referenceCount;
	
	void Retain()
	{
		if (_referenceCount)
		{
			*_referenceCount = (*_referenceCount) + 1;
		}
	}
	
public:
	bool IsEmpty() const 
	{
		return (_object == (PointerType*)0);
	}
	
	bool IsNotEmpty() const 
	{
		return (_object != (PointerType*)0); 
	}
	
	void Release()
	{
		if (_referenceCount)
		{
			*_referenceCount = (*_referenceCount) - 1;
			if ((*_referenceCount) <= 0)
			{
				if (_object)
				{
					delete _object;
				}
				_object = (PointerType*)0;
				
				free(_referenceCount);
				_referenceCount = (int32_t*)0;
			}
		}
		
		_object = (PointerType*)0;
		_referenceCount = (int32_t*)0;
	}
	
	REPtr<PointerType> & operator=(const REPtr<PointerType> & anotherPtr)
	{
		this->Release();
		if (anotherPtr.IsNotEmpty())
		{
			_object = anotherPtr._object;
			_referenceCount = anotherPtr._referenceCount;
			this->Retain();
		}
		return (*this);
	}
	
	PointerType* operator->()
	{
		return _object;
	}
	
	const PointerType* operator->() const
	{
		return _object;
	}
	
	operator PointerType* ()
	{
		return _object;
	}
	
	operator const PointerType* () const
	{
		return _object;
	}
	
	REPtr() : 
		_object((PointerType*)0),
		_referenceCount((int32_t*)0) 
	{
		
	}
	
	REPtr(PointerType* object) : 
		_object(object),
		_referenceCount((int32_t*)0) 
	{
		if (_object)
		{
			int32_t* count = (int32_t*)malloc(sizeof(int32_t*));
			if (count)
			{
				*count = 1;
				_referenceCount = count;
			}
			else
			{
				_object = (PointerType*)0;
			}
		}
	}
	
	REPtr(const REPtr<PointerType> & anotherPtr) :
		_object((PointerType*)0),
		_referenceCount((int32_t*)0) 
	{
		if (anotherPtr.IsNotEmpty())
		{
			_object = anotherPtr._object;
			_referenceCount = anotherPtr._referenceCount;
			this->Retain();
		}
	}
	
	~REPtr()
	{
		this->Release();
	}
};


#endif

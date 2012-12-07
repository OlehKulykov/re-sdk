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


#include "../../include/recore/REBuffer.h"
#include "../../include/recore/REMath.h"


#ifdef __RE_RECORE_CAN_INITIALIZE_FROM_URL_STRING__
#include "../../include/renetwork.h"
#include "../../include/recore/REData.h"
#endif

void * REBuffer::GetBuffer() const
{
	return _buff;
}

const REUInt32 REBuffer::GetSize() const
{
	return _size;
}

REBOOL REBuffer::Resize(const REUInt32 newSize, REBOOL isCopyPrevData)
{
	if (_size == newSize) { return true; }
	
	void * newBuff = malloc((size_t)newSize);
	if (newBuff) 
	{
		if (_buff) 
		{
			if (isCopyPrevData) 
			{
				const REUInt32 copySize = MIN(newSize, _size);
				if (copySize) 
				{
					memcpy(newBuff, _buff, (size_t)copySize);
				}
			}
			
			free(_buff);
		}
		
		_buff = newBuff;
		_size = newSize;
		
		return true;
	}
	
	return false;
}

void REBuffer::Clear()
{
	if (_buff) 
	{
		free(_buff);
		_buff = NULL;
		_size = 0;
	}
}

REBOOL REBuffer::Set(const void * buff, const REUInt32 buffSize)
{
	this->Clear();
	
	if (buff && buffSize) 
	{
		void * newBuff = malloc((size_t)buffSize);
		if (newBuff) 
		{
			memcpy(newBuff, buff, (size_t)buffSize);
			_buff = newBuff;
			_size = buffSize;
			return true;
		}
	}
	
	return false;
}

REBOOL REBuffer::Append(const void * buff, const REUInt32 buffSize)
{
	if (_size && _buff)
	{
		const REUInt32 newSize = _size + buffSize;
		char * newBuff = (char *)malloc((size_t)newSize);
		if (newBuff) 
		{
			memcpy(newBuff, _buff, (size_t)_size);
			memcpy(&newBuff[_size], buff, (size_t)buffSize);
			free(_buff);
			_buff = newBuff;
			_size = newSize;
			return true;
		}
	}
	
	return this->Set(buff, buffSize);
}

REBOOL REBuffer::Append(const REBuffer & anotherBuff)
{
	return this->Append(anotherBuff.GetBuffer(), anotherBuff.GetSize());
}

REBuffer & REBuffer::operator+=(const REBuffer & anotherBuff)
{
	this->Append(anotherBuff.GetBuffer(), anotherBuff.GetSize());
	return (*this);
}

REBuffer & REBuffer::operator=(const REBuffer & anotherBuff)
{
	this->Set(anotherBuff._buff, anotherBuff._size);
	return (*this);
}

REBuffer::REBuffer(const REBuffer & anotherBuff) : 
	_buff(NULL), 
	_size(0)
{
	this->Set(anotherBuff._buff, anotherBuff._size);
}

REBuffer::REBuffer(const void * buff, const REUInt32 buffSize) : 
	_buff(NULL), 
	_size(0)
{
	this->Set(buff, buffSize);
}

REBuffer::REBuffer(const REUInt32 buffSize) : 
	_buff(NULL), 
	_size(0)
{
	if (buffSize) 
	{
		void * newBuff = malloc((size_t)buffSize);
		if (newBuff) 
		{
			_buff = newBuff;
			_size = buffSize;
		}
	}
}

REBuffer::REBuffer(): 
	_buff(NULL), 
	_size(0)
{
	
}

REBuffer::~REBuffer()
{
	if (_buff) 
	{
		free(_buff);
	}
}

/// __RE_RECORE_CAN_INITIALIZE_FROM_URL_STRING__
REBOOL REBuffer::InitFromURLString(const REString & urlString)
{
	this->Clear();
	
#ifdef __RE_RECORE_CAN_INITIALIZE_FROM_URL_STRING__
	REURL url(urlString);
	if (url.IsFileURL()) 
	{
		REString hiPart(url.GetHierarchicalPart());
		REData data;
		if (data.InitFromPath(hiPart)) 
		{
			return this->Set(data.GetBytes(), data.GetSize());
		}
	}
	else
	{
		REURLRequestObject * request = REURLRequestObject::CreateWithURL(url);
		if (request) 
		{
			REBuffer downlBuff;
			const REBOOL isSended = REURLConnectionObject::SendRequest(request, &downlBuff, NULL);
			request->Release();
			if (isSended)
			{
				if (downlBuff.GetSize()) 
				{
					return this->Set(downlBuff.GetBuffer(), downlBuff.GetSize());
				}
				return true;
			}
		}
	}
#endif	
	return false;
}








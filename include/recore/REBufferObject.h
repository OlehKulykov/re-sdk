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


#ifndef __REBUFFEROBJECT_H__
#define __REBUFFEROBJECT_H__


#include "RECommonHeader.h"
#include "REBuffer.h"
#include "REObject.h"
#include "REStringObject.h"

/// REObject class of data buffer object.
class __RE_PUBLIC_CLASS_API__ REBufferObject : public REBuffer, public REObject
{
protected:
	/// Default constructor. Creates zero sized buffer.
	REBufferObject();
	
	/// Constructs buffer with 'buffSize' data buffer size.
	REBufferObject(const REUInt32 buffSize);
	
	/// Constructs buffer with data from 'buff' data with 'buffSize' data size. 
	REBufferObject(const void * buff, const REUInt32 buffSize);
	
	/// Destructor.
	virtual ~REBufferObject();
public:
	/* REObject */
	virtual const REUInt32 getClassIdentifier() const;
	static const REUInt32 classIdentifier();
	virtual REBOOL isImplementsClass(const REUInt32 classIdentifier) const;
	virtual REBOOL isEqual(REObject * anotherObject);
	
	/// Creates and return pointer to new buffer object with zero size or NULL on error.
	/// After using call Release().
	static REBufferObject * create();
	
	/// Creates and return pointer to new buffer object with 'buffSize' data size or NULL on error.
	/// After using call Release().
	static REBufferObject * createWithSize(const REUInt32 buffSize);
	
	/// Creates and return pointer to new buffer object with content of string object 'str' or NULL on error or string is empty of NULL.
	/// After using call Release().
	static REBufferObject * createWithStringObject(REStringObject * str);
	
	/// Creates and return pointer to new buffer object with content of string object or NULL on error or string is empty.
	/// After using call Release().
	static REBufferObject * createWithString(const REString & str);
	
	/// Creates and return pointer to new buffer object with content of 'memoryBuff' buffer data with 'memoryBuffSize' size or NULL on error.
	/// After using call Release().
	static REBufferObject * createWithMemory(const void * memoryBuff, const REUInt32 memoryBuffSize);
};



#endif /* __REBUFFEROBJECT_H__ */


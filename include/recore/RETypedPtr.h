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


#ifndef __RETYPEDPTR_H__
#define __RETYPEDPTR_H__

#include "RECommonHeader.h"
#include "REPtr.h"
#include "REString.h"
#include "RENumber.h"
#include "REArray.h"
#include "RENULLObject.h"
#include "REBuffer.h"
#include "REDate.h"

typedef enum _rePtrType
{
	REPtrTypeNone = 0,
	REPtrTypeString = 1,
	REPtrTypeNumber = 2,
	REPtrTypeArray = 3,
	REPtrTypeNull = 4,
	REPtrTypeBuffer = 5,
	REPtrTypeDictionary = 6,
	REPtrTypeVoidPointer = 7,
	REPtrTypeDate = 8
} REPtrType;

class REDictionary;
class REObject;
class RETypedArray;

class __RE_PUBLIC_CLASS_API__ RETypedPtr
{
protected:
	void * _object;
	REInt32 * _referenceCount;
	REPtrType _type;
	
	void retain();
	
	void deleteObject();
	
public:
	REBOOL isEmpty() const;
	REBOOL isNotEmpty() const;
	void release();
	
	REBOOL isEqualToTypedPointer(const RETypedPtr & anotherPtr) const;
	
	RETypedPtr & operator=(const RETypedPtr & anotherPtr);
	bool operator==(const RETypedPtr & anotherPtr) const;
	bool operator!=(const RETypedPtr & anotherPtr) const;
	
	REString * getString() const;
	RENumber * getNumber() const;
	RETypedArray * getArray() const;
	RENULLObject * getNULL() const;
	REBuffer * getBuffer() const;
	REDictionary * getDictionary() const;
	void * getVoidPointer() const;
	REDate * getDate() const;
	
	const REPtrType getType() const;
	
	RETypedPtr(const RETypedPtr & anotherPtr);
	RETypedPtr(void * object, const REPtrType type);
	RETypedPtr();
	~RETypedPtr();
};

#endif

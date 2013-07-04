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
<<<<<<< HEAD
#include "RENULL.h"
=======
#include "RENULLObject.h"
>>>>>>> 5cb88de87e4cabcd359f1f70380107622060d639
#include "REBuffer.h"
#include "REDate.h"

typedef enum _rePtrType
{
	REPtrTypeNone = 0,
<<<<<<< HEAD
	
	REPtrTypeString = 1,
	REPtrTypeNumber = 1 << 1,
	REPtrTypeArray = 1 << 2,
	REPtrTypeNull = 1 << 3,
	REPtrTypeBuffer = 1 << 4,
	REPtrTypeDictionary = 1 << 5,
	REPtrTypeVoidPointer = 1 << 6,
	REPtrTypeDate = 1 << 7,
	
	REPtrTypeREObject = 1 << 8,
	REPtrTypeStringObject = (REPtrTypeREObject | REPtrTypeString),
	REPtrTypeNumberObject = (REPtrTypeREObject | REPtrTypeNumber),
	REPtrTypeArrayObject = (REPtrTypeREObject | REPtrTypeArray),
	REPtrTypeNullObject = (REPtrTypeREObject | REPtrTypeNull),
	REPtrTypeBufferObject = (REPtrTypeREObject | REPtrTypeBuffer),
	REPtrTypeDictionaryObject = (REPtrTypeREObject | REPtrTypeDictionary),
	REPtrTypeDateObject = (REPtrTypeREObject | REPtrTypeDate)
	
=======
	REPtrTypeString = 1,
	REPtrTypeNumber = 2,
	REPtrTypeArray = 3,
	REPtrTypeNull = 4,
	REPtrTypeBuffer = 5,
	REPtrTypeDictionary = 6,
	REPtrTypeVoidPointer = 7,
	REPtrTypeDate = 8
>>>>>>> 5cb88de87e4cabcd359f1f70380107622060d639
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
<<<<<<< HEAD
	static void deleteREObject(void * obj, const REPtrType type);
=======
>>>>>>> 5cb88de87e4cabcd359f1f70380107622060d639
	
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
<<<<<<< HEAD
	RENULL * getNULL() const;
=======
	RENULLObject * getNULL() const;
>>>>>>> 5cb88de87e4cabcd359f1f70380107622060d639
	REBuffer * getBuffer() const;
	REDictionary * getDictionary() const;
	void * getVoidPointer() const;
	REDate * getDate() const;
	
	const REPtrType getType() const;
	
	RETypedPtr(const RETypedPtr & anotherPtr);
	RETypedPtr(void * object, const REPtrType type);
	RETypedPtr();
	~RETypedPtr();
	
<<<<<<< HEAD
	template <typename reObjectTypeName> 
	reObjectTypeName * getREObject() const
	{
		return (_type & REPtrTypeREObject) ? REPtrCast<reObjectTypeName, void>(_object) : (reObjectTypeName*)0;
	}
	
	template <typename reObjectTypeName> 
	void setREObject(reObjectTypeName * reObject, const REPtrType type)
	{
		this->release();
		if (reObject && (type & REPtrTypeREObject))
		{
			REInt32 * count = (REInt32 *)malloc(sizeof(REInt32 *));
			if (count)
			{
				*count = 1;
				_referenceCount = count;
				_object = REPtrCast<void, reObjectTypeName>(reObject);
				_type = type;
			}
		}
	}
	
=======
>>>>>>> 5cb88de87e4cabcd359f1f70380107622060d639
	static REBOOL isNotEmpty(RETypedPtr * ptr);
};

#endif

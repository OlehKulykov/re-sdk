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


#ifndef __REARRAYOBJECT_H__
#define __REARRAYOBJECT_H__

#include "RECommonHeader.h"
#include "REObject.h"
#include "REArray.h"

#if (defined(__BUILDING_RECORE_DYNAMIC_LIBRARY__) || defined(__USING_RECORE_DYNAMIC_LIBRARY__)) 
/// Warning message 4251: Class 'REArray<T>' needs to have dll-interface to be used by clients of class.
/// Why disable 4251: Class 'REArray<T>' defined as private field and no clents can access it.
/// To access must use public methods.
#ifdef _MSC_VER
#pragma warning(disable:4251)
#endif
#endif

typedef REArray<REObject *>::Iterator REObjectsIterator;

/// Array class used for storing object than implements REObject class.
class __RE_PUBLIC_CLASS_API__ REArrayObject : public REObject
{
private:
	REArray<REObject *> _arr;

	static REBOOL isEqualWithObjectsArray(REObject * anotherObject, REArray<REObject *> * arr);
	
protected:
	/// Constructs array with 'newCapacity' capacity.
	REArrayObject(const REUInt32 newCapacity);
	
	/// Constructs array with objects from 'anotherArray' another array.
	REArrayObject(const REArrayObject & anotherArray);
	
	/// Constructs array with objects from 'anotherArray' another array.
	REArrayObject(const REArray<REObject *> & anotherArray);
	
	/// Constructs empty array.
	REArrayObject();
	
	/// Desctructor.
	virtual ~REArrayObject();
public:
	/* REObject */
	virtual REBOOL isEqual(REObject * anotherObject);
	virtual void onReleased();
	
	/// Returns array iterator object
	const REObjectsIterator getIterator() const;
	
	/// Linear search for object. Return object index or RENotFound if not found.
	const REUInt32 search(const REObject * object) const;
	
	/// Setter for capacity of array
	REBOOL setCapacity(const REUInt32 newCapacity);
	
	/// Returns capacity of array
	const REUInt32 capacity() const;
	
	/// Returns objects count of array
	const REUInt32 count() const;
	
	/// Check is array empty
	REBOOL isEmpty() const;
	
	/// Swaps object at 'firstIndex' index with object at 'secondIndex' index.
	/// Returns true if both indexes exists and objects is swaped.
	REBOOL swap(const REUInt32 firstIndex, const REUInt32 secondIndex);
	
	/// Returns object pointer at 'index' index ot NULL if 'index' don't exists.
	REObject * at(const REUInt32 index) const;
	
	/// Returns last object in array or NULL if array is empty.
	REObject * lastObject() const;
	
	/// Returns first object in array or NULL if array is empty.
	REObject * firstObject() const;
	
	/// Adds 'object' to array. If object added than called Retain().
	/// Returns true if added and false if not added of object if NULL.
	REBOOL add(REObject * object);
	
	/// Adds objects from another array.
	REBOOL add(const REArrayObject & anotherArray);
	
	/// Adds objects from another array.
	REBOOL add(const REArray<REObject *> & anotherArray);
	
	/// Removes object at 'index' index. 
	/// Returns true if index exists and object is removed or false if index not exists.
	/// After removing objects call Release().
	REBOOL removeAt(const REUInt32 index);
	
	/// Removes object from array. 
	/// Returns true if object founded and removed or false if not found exists.
	/// After removing objects call Release().
	REBOOL remove(REObject * object);
	
	/// Removes objects from array that containes in another array. 
	/// Returns true if object founded and removed or false if not found exists.
	/// After removing objects call Release().
	REBOOL remove(const REArrayObject & anotherArray);
	
	/// Removes objects from array that containes in another array. 
	/// Returns true if object founded and removed or false if not found exists.
	/// After removing objects call Release().
	REBOOL remove(const REArray<REObject *> & anotherArray);
	
	/// Insert 'object' at 'insertAtIndex' index.
	/// Returns true if index exists and object not NULL or false if object is NULL or index not found.
	/// After inserts object will be Retain()'ed.
	REBOOL insert(const REUInt32 insertAtIndex, REObject * object);
	
	/// Replaces 'object' with another object 'withObject'.
	/// Returns true if both objects not NULL, 'object' is found and replaced, otherwise returns false.
	/// After replacing 'object' will be Release()'ed and 'withObject' will be Retain()'ed.
	REBOOL replace(REObject * object, REObject * withObject);
	
	/// Replaces object at 'replaceAtIndex' index with another object 'withObject'.
	/// Returns true if index exists and 'withObject' not NULL, otherwise returns false.
	/// After replacing object at index 'replaceAtIndex'will be Release()'ed and 'withObject' will be Retain()'ed.
	REBOOL replace(const REUInt32 replaceAtIndex, REObject * withObject);
	
	/// Removes all objects from array. Each object will be Release()'ed.
	void clear();
	
	/// Setts object objects from 'anotherArray'.
	/// Prev objects is Release()'ed and new objects if Retain()'ed.
	REArrayObject & set(const REArrayObject & anotherArray);
	
	/// Setts object objects from 'anotherArray'.
	/// Prev objects is Release()'ed and new objects if Retain()'ed.
	REArrayObject & set(const REArray<REObject *> & anotherArray);
	
	/// Returns object at index or NULL if 'index' index not exists.
	REObject* & operator[](const REUInt32 & index) { return _arr[index]; }
	
	/// Setts object objects from 'anotherArray'.
	/// Prev objects is Release()'ed and new objects if Retain()'ed.
	REArrayObject & operator=(const REArrayObject & anotherArray);
	
	/// Setts object objects from 'anotherArray'.
	/// Prev objects is Release()'ed and new objects if Retain()'ed.
	REArrayObject & operator=(const REArray<REObject *> & anotherArray);
	
	/// Creates and return new empty array.
	static REArrayObject * create();
	
	/// Creates and return new array object with 'newCapacity' capacity or NULL on error.
	/// After using this array call Release().
	static REArrayObject * createWithCapacity(const REUInt32 newCapacity);
	
	/// Creates and return new array object with object from 'anotherArray' another array or NULL on error.
	/// After using this array call Release().
	static REArrayObject * createWithObjectsArray(const REArrayObject * anotherArray);	
	
	/// Creates and return new array object with object from 'anotherArray' another array or NULL on error.
	/// After using this array call Release().
	static REArrayObject * createWithArray(const REArray<REObject *> & anotherArray);
};

#endif /* __REARRAYOBJECT_H__ */ 



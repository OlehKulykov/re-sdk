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


#ifndef __REARRAY_H__
#define __REARRAY_H__

#include "RECommonHeader.h"
#include "REMem.h"
#include "REMath.h"

#define REARRAY_CAPACITY_INCREMENT (4)


/// Class template of array.
template<class T> class REArray
{
protected:
	T * _arr;
	REUInt32 _count;
	REUInt32 _capacity;
	T * newMemory(const REUInt32 capacity)
	{
		return (T*)REMem::mallocAlignZeros(sizeof(T) * capacity, sizeof(T));
	}
	
	REBOOL resize(const REUInt32 newCapacity)
	{
		T * newArray = this->newMemory(newCapacity);
		if (newArray)
		{
			T * prevArray = _arr;
			const REUInt32 prevCapacity = _capacity;
			_arr = newArray;
			_capacity = newCapacity;
			if (prevArray)
			{
				memcpy(_arr, prevArray, (MIN(prevCapacity, newCapacity)) * sizeof(T));
				free(prevArray);
			}
			return true;
		}
		return false;
	}

	REBOOL qSort(int (*compareFunction)(T*, T*), REUInt32 left, REUInt32 right)
	{
		if (right > left + 1)
		{
			T * piv = &_arr[left];
			REUInt32 l = left + 1;
			REUInt32 r = right;

			if (compareFunction) 
			{
				while (l < r)
				{
					if ( compareFunction(&_arr[l], piv) > 0 )
					{
						this->swap(l, --r);
					}
					else
					{
						l++;
					}
				}
			}
			else 
			{
				while (l < r)
				{
					if (_arr[l] > (*piv))
					{
						this->swap(l, --r);
					}
					else
					{
						l++;
					}
				}
			}

			this->swap(--l, left);
			this->qSort(compareFunction, left, l);
			this->qSort(compareFunction, r, right);
		}
		return true;
	}
	
public:
	/// Array iterator struct.
	typedef struct _iterator
	{
	private:
		REArray<T> * _arrPtr;
		T * _cur;
		REUInt32 _nextIndex;
	public:
		/// Go to next object.
		/// If can go next returns true, otherwice return false.
		REBOOL next()
		{
			if ( _nextIndex < _arrPtr->_count )
			{
				_cur = _arrPtr->_arr + _nextIndex;
				_nextIndex++;
				return true;
			}
			return false;
		}
		/// Returns address of current object.
		T & object()
		{
			return (*_cur);
		}
		/// Removes current object.
		/// Returns true if removed, otherwice returns false if can't remove or index not exists.
		REBOOL removeObject()
		{
			REUInt32 removeIndex = 0;
			if (_nextIndex) 
			{ 
				removeIndex = (_nextIndex - 1); 
			}
			if (_arrPtr->removeAt(removeIndex)) 
			{
				_nextIndex = removeIndex;
				_cur = _arrPtr->_arr + _nextIndex;
				return true;
			}
			return false;
		}
		
		/// Replaces current object with another object.
		/// Returns true if replaced, otherwice returns false if can't replace or index not exists.
		REBOOL replaceWithObject(const T & withObject)
		{
			REUInt32 replaceIndex = 0;
			if (_nextIndex)
			{
				replaceIndex = (_nextIndex - 1);
			}
			return _arrPtr->replace(replaceIndex, withObject);
		}
		
		/// Cancels iteration.
		void breakIteration() { _nextIndex = _arrPtr->_count; }
		
		/// Returns current index.
		REUInt32 index() const
		{
			if (_nextIndex) { return (_nextIndex - 1); }
			return 0;
		}
		/// Construct iterator object with array pointer.
		_iterator(REArray<T> * arrPtr) : _arrPtr(arrPtr), _cur(0), _nextIndex(0) { }
	} 
	/// Array iterator struct.
	Iterator;

	/// Returns array iterator.
	Iterator getIterator()
	{
		return Iterator(this);
	}
	
	/// Set object at index.
	/// Returns true if setted, otherwice returns false if index not exists.
	REBOOL setAt(const REUInt32 index, const T & obj)
	{
		if (index < _count)
		{
			_arr[index] = obj;
			return true;
		}
		return false;
	}
	
	/// Returns object address at index. 
	/// Unsecure because no checking for correct index.
	T & operator [](const REUInt32 index)
	{
		return _arr[index];
	}
	
	/// Returns object address at index. 
	/// Unsecure because no checking for correct index.
	T & operator [](const REUInt32 index) const
	{
		return _arr[index];
	}
	
	/// Returns address of object at index or if index not exists address of static object memset()'ed with 0 value.
	T & at(const REUInt32 index)
	{
		if ( index >= _count )
		{
			static T err;
			memset(&err, 0, sizeof(T));
			return err;
		}
		return _arr[index];
	}
	
	/// Returns address of object at index or if index not exists address of static object memset()'ed with 0 value.
	T & at(const REUInt32 index) const
	{
		if ( index >= _count )
		{
			static T err;
			memset(&err, 0, sizeof(T));
			return err;
		}
		return _arr[index];
	}
	
	/// Returns address of last object or if index not exists address of static object memset()'ed with 0 value.
	T & lastObject() const
	{
		if ( _count == 0 )
		{
			static T err;
			memset(&err, 0, sizeof(T));
			return err;
		}
		return this->at(_count - 1);
	}

	/// Returns address of first object or if index not exists address of static object memset()'ed with 0 value.
	T & firstObject() const
	{
		return this->at(0);
	}

	/// Adds object to the endof array.
	/// Returns true if added, otherwice returns false if can't add new object.
	REBOOL add(const T & object)
	{
		if (_count == _capacity)
		{
			if ( !this->resize(_capacity + REARRAY_CAPACITY_INCREMENT) ) 
			{
				return false;
			}
		}
		_arr[_count] = object;
		_count++;
		return true;
	}

	/// Removes object at index.
	/// Returns true if removed or false if index not exists.
	REBOOL removeAt(const REUInt32 index)
	{
		if (index >= _count) 
		{ 
			return false; 
		}
		_count--;
		memcpy(&_arr[index], &_arr[index + 1], (_count - index) * sizeof(T));
		return true;
	}

	/// Removes last object in array.
	/// Returns true if rempved or false on error or array is empty.
	REBOOL removeLast() 
	{
		REUInt32 removeIndex = 0;
		if (_count) 
		{ 
			removeIndex = (_count - 1); 
		} 
		return this->removeAt(removeIndex);
	}

	/// Removes first object in array.
	/// Returns true if removed or false on error or array is empty.
	REBOOL removeFirst()
	{
		return this->removeAt(0);
	}

	/// swaps two objects by it's indexes.
	/// Returns true if swaped or false if indexes not exists or on error.
	/// Indexes not cheched for equation.
	REBOOL swap(const REUInt32 firstIndex, const REUInt32 secondIndex)
	{
		if ( (firstIndex < _count) && (secondIndex < _count) ) 
		{
			T * tmp = NULL;
			const bool newMem = (_count == _capacity);
			if (newMem) { tmp = (T*)malloc(sizeof(T)); }
			else        { tmp = &_arr[_count]; }

			if (tmp)
			{
				memcpy(tmp, &_arr[firstIndex], sizeof(T));
				memcpy(&_arr[firstIndex], &_arr[secondIndex], sizeof(T));
				memcpy(&_arr[secondIndex], tmp, sizeof(T));
				if (newMem) { free(tmp); }
				return true;
			}
		}
		return false;
	}

	/// Sort array using comparator function.
	/// Example: 
	/// static void INTCompareFunction(int * leftValue, int * rightValue) 
	/// {
	///    // for this example where type is integer can be used: return (*leftValue - *rightValue); but see bellow for not integers type.
	///    if (*leftValue > *rightValue) return 1; // any positive value if left bigger.
	///    else if (*leftValue > *rightValue) return -1; // any negative value if left smaler.
	///    return 0; // if left and right equal 
	/// }
	///  
	/// REArray<int> array; ....; array.Sort(INTCompareFunction);
	REBOOL sort(int (*compareFunction)(T*, T*))
	{
		if (compareFunction != NULL) 
		{
			return this->qSort(compareFunction, 0, _count);
		}
		return false;
	}

	
	/// Sort array
	/// T must implement operator '>'
	REBOOL sort()
	{
		return this->qSort(0, 0, _count);
	}
	

	/// Binary search for object, array must be sorted
	/// T must implement operator '==' and operator '>'
	/// Return object index or RENotFound if not found.
	REUInt32 binarySearch(const T & object)
	{
		if (_count)
		{
			REUInt32 left = 0;
			REUInt32 right = _count - 1;
			while (left <= right) 
			{
				REUInt32 middle = (left + right) / 2;
				T * middleValue = &_arr[middle];
				if ( (*middleValue) == object )
				{
					return middle;
				}
				else if ( (*middleValue) > object )
				{
					if (middle) 
					{
						right = middle - 1;
					}
					else
					{
						break;
					}
				}
				else
				{
					left = middle + 1;
				}
			}
		}
		return RENotFound;
	}
	

	/// Linear search for object
	/// T must implement operator '=='
	/// Return object index or RENotFound if not found
	REUInt32 search(const T & object)
	{
		for (REUInt32 index = 0; index < _count; index++) 
		{
			if (_arr[index] == object) 
			{
				return index;
			}
		}
		return RENotFound;
	}
	
	/// Inserts object at index.
	/// Returns true if inserted or false if index not exists
	REBOOL insert(const REUInt32 insertAtIndex, const T & object)
	{
		if (insertAtIndex < _count)
		{
			if (_count == _capacity)
			{
				if (!this->resize(_capacity + REARRAY_CAPACITY_INCREMENT)) 
				{
					return false;
				}
			}
			memmove(&_arr[insertAtIndex + 1], &_arr[insertAtIndex], (sizeof(T) * (_count - insertAtIndex)));
			_arr[insertAtIndex] = object;
			_count++;
			return true;
		}
		else if ((insertAtIndex == 0) && (_count == 0))
		{
			return this->add(object);
		}
		return false;
	}

	/// Replaces existed object at index with another object.
	/// Returns true if replaces or false if index not exists.
	REBOOL replace(const REUInt32 replaceAtIndex, const T & withObject)
	{
		if (replaceAtIndex < _count)
		{
			_arr[replaceAtIndex] = withObject;
			return true;
		}
		return false;
	}

	/// Sets capacity of array. 
	/// If new capacity is equal current capacity the return true and do nothing.
	/// Be careful: if you provide new capacity les than current capacity you can lost data.
	REBOOL setCapacity(const REUInt32 newCapacity)
	{
		if (_capacity == newCapacity) { return true; }
		const REUInt32 newCap = (newCapacity > 0) ? newCapacity : REARRAY_CAPACITY_INCREMENT;
		const REUInt32 newCnt = (_count > newCap) ? newCap : _count;
		if (this->resize(newCap))
		{
			_count = newCnt;
			return true;
		}
		return false;
	}
	
	/// Returns count of objects in array.
	const REUInt32 count() const { return _count; }
	
    /// Returns capacity of array.
    const REUInt32 capacity() const { return _capacity; }

	/// Check is array empty.
	REBOOL isEmpty() const { return (_count == 0); }
	
	/// Setes count of array to zero. 
	virtual void clear() { _count = 0; }

	/// Setes object from another array.
	REArray<T> & operator=(const REArray<T> & anotherArray)
	{
		if (_arr) 
		{ 
			free(_arr); 
			_arr = 0; 
			_count = 0; 
			_capacity = 0; 
		}
		
		if (anotherArray._arr) 
		{ 
			_capacity = anotherArray._capacity; 
			_count = anotherArray._count; 
		}
		
		if (_capacity == 0) { _capacity = REARRAY_CAPACITY_INCREMENT; }
		_arr = this->newMemory(_capacity);
		if (_arr) 
		{ 
			if (_count > 0) { memcpy(_arr, anotherArray._arr, _count * sizeof(T)); } 
		}
		else 
		{ 
			_capacity = 0;
			_count = 0;
		}
		
		return (*this);
	}
	
	/// Constructs array with capacity or with default capacity.
	REArray(REUInt32 newCapacity = REARRAY_CAPACITY_INCREMENT) : _arr(0), _count(0), _capacity(newCapacity)
	{
		if (_capacity == 0) { _capacity = REARRAY_CAPACITY_INCREMENT; }
		_arr = this->newMemory(_capacity);
		if ( _arr == 0 ) { _capacity = 0; }
	}
	
	/// Constructs array with objects from another array.
	REArray(const REArray<T> & anotherArray) : _arr(0), _count(anotherArray._count), _capacity(anotherArray._capacity)
	{
		if (_capacity == 0) { _capacity = REARRAY_CAPACITY_INCREMENT; }
		_arr = this->newMemory(_capacity);
		if (_arr) 
		{
			if (_count > 0) { memcpy(_arr, anotherArray._arr, _count * sizeof(T)); }
		}
		else
		{ 
			_capacity = 0; 
		}
	}
	
	virtual ~REArray() 
	{ 
		if (_arr)
		{
			free(_arr);
		} 
	}
	
	/// Check is array pointer exists and array not empty.
	/// Returns true if pointer is not NULL and array has objects, otherwice returns false.
	static REBOOL isNotEmpty(const REArray<T> * array) 
	{
		return array ? (array->_count > 0) : false;
	}
	
	/// Check is array empty.
	/// Returns true if pointer is NULL or array has no objects.
	static REBOOL isEmpty(const REArray<T> * array) 
	{
		return array ? array->isEmpty() : true;
	}
};



#endif /* __REARRAY_H__ */


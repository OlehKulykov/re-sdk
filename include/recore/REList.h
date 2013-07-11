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


#ifndef __RELIST_H__
#define __RELIST_H__

#include "RECommonHeader.h"
#include "REMem.h"

/// Template class of linked list.
template<class T>
class REList
{
private:
	typedef struct _listElementStruct
	{
		_listElementStruct * next;
		T value;
	} ListElementStruct;
	ListElementStruct * _head;
	REUInt32 _count;
	
	ListElementStruct * getElementStructAt(const REUInt32 index) const
	{
		REUInt32 i = 0;
		ListElementStruct * curr = _head;
		while (curr) 
		{
			if (index == i) return curr;
			curr = curr->next;
			i++;
		}
		return NULL;
	}
	
public:
	/// Return count of elements in list
	const REUInt32 count() const { return _count; }
	
	/// Checks if list has objects
	const REBOOL isEmpty() const { return (_count == 0); }
	
	/// Try remove object at index. Return 'true' if index exists and removed, otherwise return 'false'.
	REBOOL removeAt(const REUInt32 index)
	{
		REUInt32 i = 0;
		ListElementStruct * curr = _head;
		ListElementStruct * prev = NULL;
		while (curr) 
		{
			if (index == i) 
			{
				if (prev) prev->next = curr->next;
				else _head = curr->next;
				_count--;
				free(curr);
				return true;
			}
			prev = curr;
			curr = curr->next;
			i++;
		}
		return false;
	}
	
	/// Try remove last object. Return 'true' if last object exists and removed, otherwise return 'false'.
	REBOOL removeLast()
	{
		return (_count) ? this->removeAt(_count - 1) : false;
	}
	
	/// Try remove object by it's value. Objects must have equation operator.
	/// Return 'true' if found and removed, otherwise return 'false'.
	REBOOL removeValue(const T & value)
	{
		ListElementStruct * curr = _head;
		ListElementStruct * prev = NULL;
		while (curr) 
		{
			if (value == curr->value) 
			{
				if (prev) prev->next = curr->next;
				else _head = curr->next;
				_count--;
				free(curr);
				return true;
			}
			prev = curr;
			curr = curr->next;
		}
		return false;
	}
	
	/// Adds 'value' as last element of the list.
	/// Return 'true' if object added, otherwise return 'false'.
	REBOOL add(const T & value)
	{
		ListElementStruct * newStruct = (ListElementStruct *)malloc(sizeof(ListElementStruct));
		if (newStruct) 
		{
			if (_head && _count) 
			{
				ListElementStruct * last = this->getElementStructAt(_count - 1);
				last->next = newStruct;
			}
			else
			{
				_head = newStruct; 
			}
			newStruct->next = NULL;
			newStruct->value = value;
			_count++;
			return true;
		}
		return false;
	}
	
	/// Adds 'value' as first(head) element of the list.
	/// Return 'true' if added as first, otherwise return 'false'.
	REBOOL addFirst(const T & value)
	{
		ListElementStruct * newStruct = (ListElementStruct *)malloc(sizeof(ListElementStruct));
		if (newStruct) 
		{
			newStruct->value = value;
			newStruct->next = _head;
			_head = newStruct;
			_count++;
			return true;
		}
		return false;
	}
	
	/// Returns object address at index.
	/// If index not exists returns address of static object filled with zeros.
	T & at(const REUInt32 index) const
	{
		ListElementStruct * elem = this->getElementStructAt(index);
		if (elem) return elem->value;
		static T empty;
		memset(&empty, 0, sizeof(T));
		return empty;
	}
	
	/// Overrided operator 'Array subscript'. 
	/// Returns object address at index.
	/// If index not exists returns address of static object filled with zeros.
	T & operator[](const REUInt32 index)
	{
		return this->at(index);
	}
	
	/// Returns object address of last object.
	/// If index not exists returns address of static object filled with zeros.
	T & lastObject() const
	{
		if (_count) 
		{
			ListElementStruct * elem = this->getElementStructAt(_count - 1);
			if (elem) return elem->value;
		}
		static T empty;
		memset(&empty, 0, sizeof(T));
		return empty;
	}
	
	/// Removes all object of the list. Objects not frred and not deleted.
	void clear()
	{
		if (_head) 
		{
			ListElementStruct * next = _head->next;
			free(_head);
			_head = NULL;
			while (next) 
			{
				ListElementStruct * prev = next;
				next = next->next;
				free(prev);
			}
		}
	}
	
	/// Constructor of empty list.
	REList() : _head(NULL), _count(0)
	{
		
	}
	
	/// Desctructor.
	~REList()
	{
		this->clear();
	}
};



#endif /* __RELIST_H__ */


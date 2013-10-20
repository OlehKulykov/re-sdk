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

template <typename T>
class REList 
{
public:
	class Node;
private:
	class NodeBase
	{
	public:
		Node * next;
		Node * previous;
		NodeBase() : next(NULL), previous(NULL) { }
	};
	
public:	
	class Node : public NodeBase
	{
	public:
		T value;
		Node(const T & newValue) : NodeBase(), value(newValue) { }
	};
	
	typedef enum 
	{
		Descending = -1,
		Same = 0,
		Ascending = 1
	} ValueCompareResult;
	
	typedef Node * NodePtr;
	typedef NodePtr (*CreateNodeCallback)(const T & newValue);
	typedef void (*ReleaseNodeCallback)(NodePtr);
	typedef ValueCompareResult (*NodeValuesComparator)(const T * left, const T * right);
	typedef ValueCompareResult (*CustomNodeValueComparator)(const T * nodeValue, void * customValue);
private:	
	CreateNodeCallback _createNode;
	ReleaseNodeCallback _releaseNode;
	union
	{
		NodeBase * _head;
		Node * _castHead;
	};
	
public:
	static NodePtr newNode(const T & newValue)
	{
		return (new Node(newValue));
	}
	
	static void deleteNode(NodePtr node)
	{
		delete node;
	}
	
	static NodePtr allocateNode(const T & newValue)
	{
		NodePtr node = (NodePtr)malloc(sizeof(Node));
		if (node) 
		{
			node->value = newValue;
		}
		return node;
	}
	
	static void freeNode(NodePtr node)
	{
		free(node);
	}
public:
	class Iterator
	{
	private:
		Node * _head;
		Node * _node;
	public:
		bool next() const
		{
			Node ** n = const_cast<Node **>(&_node);
			*n = _node ? _node->next : this->_head->next;
			return _node != this->_head;
		}
		
		Node * node() const
		{
			return _node;
		}
		
		const T & value() const
		{
			return _node->value;
		}
		
		Iterator(const REList<T>::Iterator & it) : 
			_head(it._head),
			_node(NULL)
		{
			
		}
		
		Iterator(Node * listHead) : 
			_head(listHead),
			_node(NULL)
		{
			
		}
	};
	
	REList<T>::Iterator iterator() const
	{
		return REList<T>::Iterator(this->_castHead);
	}
	
	REBOOL isEmpty() const 
	{
		return (this->_head->next == this->_head);
	}
	
	void clear() 
	{
		Node * node = this->_head->next;
		while (node != this->_head) 
		{
			node = this->removeNode(node);
		}
	}
	
	Node * findNode(void * customValue, CustomNodeValueComparator comparator) const
	{
		Node * next = this->_head->next;
		while (next != this->_head)
		{
			if (comparator(&next->value, customValue) == REList<T>::Same) 
			{
				return next;
			}
			next = next->next;
		}
		return NULL;
	}
	
	Node * findNode(const T & value, NodeValuesComparator comparator) const
	{
		Node * next = this->_head->next;
		while (next != this->_head)
		{
			if (comparator(&next->value, &value) == REList<T>::Same) 
			{
				return next;
			}
			next = next->next;
		}
		return NULL;
	}
	
	Node * findNode(const T & value) const
	{
		Node * next = this->_head->next;
		while (next != this->_head)
		{
			if (next->value == value) 
			{
				return next;
			}
			next = next->next;
		}
		return NULL;
	}
	
	REBOOL isContaines(const T & value)
	{
		return this->findNode(value) ? true : false;
	}
	
	Node * removeNode(Node * node) 
	{
		if (node != this->_head) 
		{			
			Node * next = node->next;
			node->previous->next = node->next;
			node->next->previous = node->previous;
			_releaseNode(node);
			return next;
		}
		return this->_castHead;
	}
	
	REBOOL add(const T & newValue) 
	{
		Node * newNode = _createNode(newValue);
		if (newNode) 
		{
			newNode->previous = _head->previous;
			newNode->previous->next = newNode;
			_head->previous = newNode;
			newNode->next = _castHead;
			return true;
		}
		return false;
	}
	
	REList(CreateNodeCallback nodeCreator = &REList<T>::newNode, 
		   ReleaseNodeCallback nodeReleaser = &REList<T>::deleteNode) :
	_createNode(nodeCreator),
	_releaseNode(nodeReleaser),
	_head(NULL)
	{
		NodeBase * newHead = (NodeBase *)malloc(sizeof(NodeBase));
		if (newHead) 
		{
			this->_head = newHead;
			this->_head->next = this->_castHead;
			this->_head->previous = this->_castHead;
		}
	}
	
	~REList()
	{
		this->clear();
		if (this->_head)
		{
			free(this->_head);
		}
	}
};

#if 0
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
#endif // #if 0


#endif /* __RELIST_H__ */


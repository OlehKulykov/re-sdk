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


#ifndef __REMAINLOOPSOBJECTSSTORAGEPRIVATE_H__
#define __REMAINLOOPSOBJECTSSTORAGEPRIVATE_H__

#include "../RECommonHeader.h"
#include "../REThread.h"
#include "../REMutex.h"
#include "../REMainLoopUpdatable.h"
#include "../REList.h"

class REMainLoopsObjectsStoragePrivate
{
private:
	class MLObject
	{
	public:	
		REMainLoopUpdatable * object;
		REUIdentifier identifier;
		MLObject & operator=(const MLObject & o)
		{
			object = o.object;
			identifier = o.identifier;
			return (*this);
		}
		MLObject(const MLObject & o) :
			object(o.object),
			identifier(o.identifier)
		{ }
		MLObject(REMainLoopUpdatable * o = NULL, const REUIdentifier i = 0) : 
			object(o), 
			identifier(i)
		{ }
	};
	typedef REList<REMainLoopsObjectsStoragePrivate::MLObject> ListType;
	static ListType::ValueCompareResult compareObjectByID(const MLObject * nodeValue, void * customValue)
	{
		return (nodeValue->identifier == *((REUIdentifier*)customValue)) ? ListType::Same : ListType::Ascending;
	}
	REList<MLObject> _list;
public:
	REUIdentifier add(REMainLoopUpdatable * object);
	REUIdentifier remove(REMainLoopUpdatable * object);
	void update(const RETimeInterval time);
	REMainLoopsObjectsStoragePrivate();
	~REMainLoopsObjectsStoragePrivate();
};


#endif /* __REMAINLOOPSOBJECTSSTORAGEPRIVATE_H__ */



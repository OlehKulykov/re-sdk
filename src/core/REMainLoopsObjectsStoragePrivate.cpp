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


#include "../../include/recore/private/REMainLoopsObjectsStoragePrivate.h"

REUIdentifier REMainLoopsObjectsStoragePrivate::add(REMainLoopUpdatable * object)
{
	union
	{
		REMainLoopUpdatable * object;
		REUIdentifier identifier;
	} u1;
	
	u1.object = object;
	REUIdentifier objectIdentifier = u1.identifier;
	ListType::Node * node = _list.findNode(&objectIdentifier, REMainLoopsObjectsStoragePrivate::compareObjectByID);
	if (node)
	{
		return objectIdentifier;
	}
	else
	{
		MLObject o(NULL, objectIdentifier);
		if (_list.add(o)) 
		{
			o.object = object;
			return objectIdentifier;
		}
	}
	
	return 0;
}

REUIdentifier REMainLoopsObjectsStoragePrivate::remove(REMainLoopUpdatable * object)
{
	REUIdentifier identifier = object->mainLoopUpdatableIdentifier();
	ListType::Node * node = _list.findNode(&identifier, REMainLoopsObjectsStoragePrivate::compareObjectByID);
	if (node) 
	{
		node->value.object = NULL;
		return identifier;
	}	
	return 0;
}

void REMainLoopsObjectsStoragePrivate::update(const RETimeInterval time)
{	
	ListType::Iterator iter = _list.iterator();
	while (iter.next()) 
	{
		if (iter.value().object) 
		{
			iter.value().object->update(time);
		}
		else
		{
			_list.removeNode(iter.node());
		}
	}
}

REMainLoopsObjectsStoragePrivate::REMainLoopsObjectsStoragePrivate()
{

}

REMainLoopsObjectsStoragePrivate::~REMainLoopsObjectsStoragePrivate()
{
	
}

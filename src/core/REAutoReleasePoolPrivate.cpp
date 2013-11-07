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


#include "../../include/recore/private/REAutoReleasePoolPrivate.h"

REBOOL REAutoReleasePoolPrivate::addObject(REObject * autoReleasableObject)
{
	if (autoReleasableObject)
	{
		return _list.add(autoReleasableObject);
	}
	return false;
}

REBOOL REAutoReleasePoolPrivate::isEmpty() const
{
	return _list.isEmpty();
}

void REAutoReleasePoolPrivate::update()
{
	ListType::Iterator iter = _list.iterator();
	while (iter.next()) 
	{
		REObject * o = iter.value();
		REObjectRemover::deleteObject(o);
		_list.removeNode(iter.node());
	}
}

REAutoReleasePoolPrivate::REAutoReleasePoolPrivate()
{
	
}

REAutoReleasePoolPrivate::~REAutoReleasePoolPrivate()
{
	this->update();
}


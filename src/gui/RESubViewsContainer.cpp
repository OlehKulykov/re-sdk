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


#include "../../include/regui/RESubViewsContainer.h"
#include "../../include/regui/REView.h"

RESubviewsContainer * RESubviewsContainer::parentSubviewsContainer() const
{
	return _parentSubviewsContainer;
}

REBOOL RESubviewsContainer::isHasSubviews() const
{
	return ( !_subviews.isEmpty() );
}

REList<REView *>::Iterator RESubviewsContainer::subviewsIterator() const
{
	return _subviews.iterator();
}

REView * RESubviewsContainer::subviewWithTag(const REInt32 viewTag)
{
	REList<REView *>::Iterator iter(_subviews.iterator());
	while (iter.next()) 
	{
		if (iter.value()->tag() == viewTag) 
		{
			return iter.value();
		}
		else
		{
			REView * sv = iter.value()->subviewWithTag(viewTag);
			if (sv) 
			{
				return sv; 
			}
		}
	}
	return NULL;
}

REBOOL RESubviewsContainer::removeSubview(REView * v)
{
	if (v) 
	{
		REList<REView *>::Iterator iter(_subviews.iterator());
		while (iter.next()) 
		{
			if (iter.value()->isEqual(v)) 
			{
				return _subviews.removeNode(iter.node());
			}
		}
	}
	return false;
}

REBOOL RESubviewsContainer::addSubview(REView * v)
{	
	if (v) 
	{
		if (_subviews.add(v)) 
		{
			v->_parentSubviewsContainer = this;
			return true;
		}
	}
	return false;
}

void RESubviewsContainer::removeAllSubviews()
{
	_subviews.clear();
}

REList<REView *>::NodePtr RESubviewsContainer::newNode(REView* const & newValue)
{
	REList<REView *>::NodePtr node = new REList<REView *>::Node(newValue);
	if (node) 
	{
		node->value->retain();
	}
	return node;
}

void RESubviewsContainer::deleteNode(REList<REView *>::NodePtr node)
{
	if (node) 
	{
		node->value->_parentSubviewsContainer = NULL;
		node->value->release();
		delete node;
	}
}

RESubviewsContainer::RESubviewsContainer() :
	_subviews(RESubviewsContainer::newNode, RESubviewsContainer::deleteNode),
	_parentSubviewsContainer(NULL)
{
	
}

RESubviewsContainer::~RESubviewsContainer()
{
	
}



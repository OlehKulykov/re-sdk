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
#include "../../include/regui/REGUIApplication.h"

#include "../../include/recore/REStaticString.h"
#include "../../include/recore/RETypedPtr.h"
#include "../../include/recore/RETypedArray.h"
#include "../../include/recore/RELog.h"


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

RESubviewsContainer::RESubviewsContainer() : RESerializable(),
_subviews(RESubviewsContainer::newNode, RESubviewsContainer::deleteNode),
_parentSubviewsContainer(NULL)
{
	
}

RESubviewsContainer::~RESubviewsContainer()
{
	
}

#define TYPED_STATIC_STRING(s) RETypedPtr(new REStaticString(s), REPtrTypeString) 

RETypedPtr RESubviewsContainer::serializeToDictionary() const
{
	RETypedPtr d(RESerializable::serializeToDictionary());
	if (!_subviews.isEmpty()) 
	{
		REDictionary * dict = d.dictionary();
		if (dict) 
		{
			RETypedPtr arr(new RETypedArray(), REPtrTypeArray);
			RETypedArray * subviews = arr.array();
			if (subviews) 
			{
				REList<REView *>::Iterator iter(_subviews.iterator());
				while (iter.next()) 
				{
					RETypedPtr subDict(iter.value()->serializeToDictionary());
					if (subDict.isNotEmpty()) 
					{
						subviews->add(subDict);
					}
				}
				if (subviews->count() > 0) 
				{
					dict->setValue(arr, TYPED_STATIC_STRING("subviews"));
				}
			}
		}
	}
	return d;
}

void RESubviewsContainer::deserializeWithDictionary(const RETypedPtr & dictionary)
{
	REDictionary * dict = dictionary.dictionary();
	if (dict) 
	{
		RETypedArray * subviews = dict->valueForKey("subviews").array();
		if (subviews) 
		{
			for (REUInt32 i = 0; i < subviews->count(); i++) 
			{
				RETypedPtr subDict(subviews->at(i));
				if (RETypedPtr::isNotEmpty(&subDict, REPtrTypeDictionary)) 
				{
					REGUIApplication * app = REGUIApplication::currentApplication();
					if (app) 
					{
						REView * v = app->createSerializableClassWithDictionary<REView>(subDict);
						if (v) 
						{
							v->deserializeWithDictionary(subDict);
							this->addSubview(v);
						}
					}
					else { RELog::log("ERROR: REGUIApplication not initialized"); }
				}
			}
		}
	}
}



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

REView * RESubViewsContainer::getParentSubViewsContainer() const
{
	return (REView *)_parentSubViewsContainer;
}

REBOOL RESubViewsContainer::isHasSubViews() const
{
	if (_subViewsArray) 
	{
		return (_subViewsArray->count() > 0);
	}
	return false;
}

REView * RESubViewsContainer::findSubViewWithTag(const REInt32 viewTag)
{
	_updateMutex.lock();
	
	if (_subViewsArray) 
	{
		for (REUInt32 i = 0; i < _subViewsArray->count(); i++) 
		{
			REView * v = (REView *)(*_subViewsArray)[i];
			if (v->getTag() == viewTag) 
			{
				_updateMutex.unlock();
				return v;
			}
			else
			{
				REView * sv = v->findSubViewWithTag(viewTag);
				if (sv) 
				{
					_updateMutex.unlock();
					return sv;
				}
			}
		}
	}

	_updateMutex.unlock();
	return NULL;
}

REView * RESubViewsContainer::findSubViewWithClassName(const char * className)
{
	_updateMutex.lock();
	if (_subViewsArray && className) 
	{
		const REUInt32 classNameIdentifier = REMD5Generator::generateFromString(className);
		for (REUInt32 i = 0; i < _subViewsArray->count(); i++) 
		{
			REView * v = (REView *)(*_subViewsArray)[i];
			if (v->getObjectIdentifier() == classNameIdentifier) 
			{
				_updateMutex.unlock();
				return v;
			}
			else
			{
				REView * sv = v->findSubViewWithClassName(className);
				if (sv) 
				{
					_updateMutex.unlock();
					return sv;
				}
			}
		}
	}
	_updateMutex.unlock();
	return NULL;
}

REBOOL RESubViewsContainer::removeSubView(REView * v)
{
	REBOOL r = false;
	_updateMutex.lock();
	if (v) 
	{
		if (_subViewsArray) 
		{
			v->retain();
			if (_subViewsArray->remove(v)) 
			{
				v->_parentSubViewsContainer = NULL;
				this->onViewDidRemoved(v);
				r = true;
			}
			v->release();
		}
	}
	_updateMutex.unlock();
	return r;
}

REBOOL RESubViewsContainer::replaceSubView(REView * v, REView * withView)
{
	REBOOL r = false;
	_updateMutex.lock();
	
	if (v && withView) 
	{
		if (_subViewsArray) 
		{
			v->retain();
			if (_subViewsArray->replace(v, withView)) 
			{
				v->_parentSubViewsContainer = NULL;
				withView->_parentSubViewsContainer = this;
				this->onViewDidRemoved(v);
				this->onViewDidAdded(withView);
				r = true;
			}
			v->release();
		}
	}
	
	_updateMutex.unlock();
	return r;
}

REBOOL RESubViewsContainer::addSubView(REView * v)
{
	_updateMutex.lock();
	
	if (v) 
	{
<<<<<<< HEAD
		REArrayObject * a = this->getOrCreateAndGetSubViewsArray();
=======
		REObjectsArray * a = this->getOrCreateAndGetSubViewsArray();
>>>>>>> 5cb88de87e4cabcd359f1f70380107622060d639
		if (a) 
		{
			if ( a->add(v) ) 
			{
				v->_parentSubViewsContainer = this;
				this->onViewDidAdded(v);
				_updateMutex.unlock();
				return true;
			}
		}
	}
	
	_updateMutex.unlock();
	return false;
}

/// Inserts view above 'targetView'.
/// After adding view Retain will call.
/// Returns false if one of params is NULL or 'targetView' not containes.
REBOOL RESubViewsContainer::insertSubViewAbove(REView * v, REView * targetView)
{
	_updateMutex.lock();
	
	if (v && targetView) 
	{
<<<<<<< HEAD
		REArrayObject * a = this->getOrCreateAndGetSubViewsArray();
=======
		REObjectsArray * a = this->getOrCreateAndGetSubViewsArray();
>>>>>>> 5cb88de87e4cabcd359f1f70380107622060d639
		if (a) 
		{
			REUInt32 insertIndex = a->search(targetView);
			if (insertIndex != RENotFound) 
			{
				insertIndex++;
				const REUInt32 lastIndex = a->count() - 1;
				REBOOL isAdded = false;
				if (insertIndex < lastIndex) 
				{
					isAdded = a->insert(insertIndex, v);
				}
				else 
				{
					isAdded = a->add(v);
				}
				
				if (isAdded) 
				{
					v->_parentSubViewsContainer = this;
					this->onViewDidAdded(v);
					_updateMutex.unlock();
					return true;
				}
			}
		}
	}
	
	_updateMutex.unlock();
	return false;
}

/// Inserts view below 'targetView'.
/// After adding view Retain will call.
/// Returns false if one of params is NULL or 'targetView' not containes.
REBOOL RESubViewsContainer::insertSubViewBelow(REView * v, REView * targetView)
{
	_updateMutex.lock();
	
	if (v && targetView) 
	{
<<<<<<< HEAD
		REArrayObject * a = this->getOrCreateAndGetSubViewsArray();
=======
		REObjectsArray * a = this->getOrCreateAndGetSubViewsArray();
>>>>>>> 5cb88de87e4cabcd359f1f70380107622060d639
		if (a) 
		{
			REUInt32 insertIndex = a->search(targetView);
			if (insertIndex != RENotFound) 
			{
				if (a->insert(insertIndex, v)) 
				{
					v->_parentSubViewsContainer = this;
					this->onViewDidAdded(v);
					_updateMutex.unlock();
					return true;
				}
			}
		}
	}
	
	_updateMutex.unlock();
	return false;
}

void RESubViewsContainer::lockUpdate()
{
	_updateMutex.lock();
}

void RESubViewsContainer::unLockUpdate()
{
	_updateMutex.unlock();
}

void RESubViewsContainer::removeAllSubViews()
{
	_updateMutex.lock();
	
	if (_subViewsArray) 
	{
		for (REUInt32 i = 0; i < _subViewsArray->count(); i++) 
		{
			REView * v = (REView *)(*_subViewsArray)[i];
			v->_parentSubViewsContainer = NULL;
			this->onViewDidRemoved(v);
		}
		_subViewsArray->clear();
	}

	_updateMutex.unlock();
}

<<<<<<< HEAD
REArrayObject * RESubViewsContainer::getOrCreateAndGetSubViewsArray()
=======
REObjectsArray * RESubViewsContainer::getOrCreateAndGetSubViewsArray()
>>>>>>> 5cb88de87e4cabcd359f1f70380107622060d639
{
	if (_subViewsArray) 
	{
		return _subViewsArray;
	}
	
	_updateMutex.lock();
	
<<<<<<< HEAD
	_subViewsArray = REArrayObject::create();
=======
	_subViewsArray = REObjectsArray::create();
>>>>>>> 5cb88de87e4cabcd359f1f70380107622060d639
	
	_updateMutex.unlock();
		
	return _subViewsArray;
}

RESubViewsContainer::RESubViewsContainer() :
	_subViewsArray(NULL),
	_parentSubViewsContainer(NULL)
{
	_updateMutex.init(REMutexTypeRecursive);
}

RESubViewsContainer::~RESubViewsContainer()
{
	this->removeAllSubViews();
	
	if (_subViewsArray) 
	{
		_subViewsArray->release();
	}
	
}



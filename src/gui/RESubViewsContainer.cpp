/*
 *   Copyright 2012 Kulykov Oleh
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

REView * RESubViewsContainer::GetParentSubViewsContainer() const
{
	return (REView*)_parentSubViewsContainer;
}

REBOOL RESubViewsContainer::IsHasSubViews() const
{
	if (_subViewsArray) 
	{
		return (_subViewsArray->Count() > 0);
	}
	return false;
}

REView * RESubViewsContainer::FindSubViewWithTag(const REInt32 viewTag)
{
	_updateMutex.Lock();
	
	if (_subViewsArray) 
	{
		for (REUInt32 i = 0; i < _subViewsArray->Count(); i++) 
		{
			REView * v = (REView *)(*_subViewsArray)[i];
			if (v->GetTag() == viewTag) 
			{
				_updateMutex.Unlock();
				return v;
			}
			else
			{
				REView * sv = v->FindSubViewWithTag(viewTag);
				if (sv) 
				{
					_updateMutex.Unlock();
					return sv;
				}
			}
		}
	}

	_updateMutex.Unlock();
	return NULL;
}

REView * RESubViewsContainer::FindSubViewWithClassName(const char * className)
{
	_updateMutex.Lock();
	if (_subViewsArray && className) 
	{
		const REUInt32 classNameIdentifier = REMD5Generator::GenerateFromString(className);
		for (REUInt32 i = 0; i < _subViewsArray->Count(); i++) 
		{
			REView * v = (REView *)(*_subViewsArray)[i];
			if (v->GetObjectIdentifier() == classNameIdentifier) 
			{
				_updateMutex.Unlock();
				return v;
			}
			else
			{
				REView * sv = v->FindSubViewWithClassName(className);
				if (sv) 
				{
					_updateMutex.Unlock();
					return sv;
				}
			}
		}
	}
	_updateMutex.Unlock();
	return NULL;
}

REBOOL RESubViewsContainer::RemoveSubView(REView * v)
{
	REBOOL r = false;
	_updateMutex.Lock();
	if (v) 
	{
		if (_subViewsArray) 
		{
			v->Retain();
			if (_subViewsArray->Remove(v)) 
			{
				v->_parentSubViewsContainer = NULL;
				this->OnViewDidRemoved(v);
				r = true;
			}
			v->Release();
		}
	}
	_updateMutex.Unlock();
	return r;
}

REBOOL RESubViewsContainer::ReplaceSubView(REView * v, REView * withView)
{
	REBOOL r = false;
	_updateMutex.Lock();
	
	if (v && withView) 
	{
		if (_subViewsArray) 
		{
			v->Retain();
			if (_subViewsArray->Replace(v, withView)) 
			{
				v->_parentSubViewsContainer = NULL;
				withView->_parentSubViewsContainer = this;
				this->OnViewDidRemoved(v);
				this->OnViewDidAdded(withView);
				r = true;
			}
			v->Release();
		}
	}
	
	_updateMutex.Unlock();
	return r;
}

REBOOL RESubViewsContainer::AddSubView(REView * v)
{
	_updateMutex.Lock();
	
	if (v) 
	{
		REObjectsArray * a = this->GetOrCreateAndGetSubViewsArray();
		if (a) 
		{
			if ( a->Add(v) ) 
			{
				v->_parentSubViewsContainer = this;
				this->OnViewDidAdded(v);
				_updateMutex.Unlock();
				return true;
			}
		}
	}
	
	_updateMutex.Unlock();
	return false;
}

/// Inserts view above 'targetView'.
/// After adding view Retain will call.
/// Returns false if one of params is NULL or 'targetView' not containes.
REBOOL RESubViewsContainer::InsertSubViewAbove(REView * v, REView * targetView)
{
	_updateMutex.Lock();
	
	if (v && targetView) 
	{
		REObjectsArray * a = this->GetOrCreateAndGetSubViewsArray();
		if (a) 
		{
			REUInt32 insertIndex = a->Search(targetView);
			if (insertIndex != RENotFound) 
			{
				insertIndex++;
				const REUInt32 lastIndex = a->Count() - 1;
				REBOOL isAdded = false;
				if (insertIndex < lastIndex) 
				{
					isAdded = a->Insert(insertIndex, v);
				}
				else 
				{
					isAdded = a->Add(v);
				}
				
				if (isAdded) 
				{
					v->_parentSubViewsContainer = this;
					this->OnViewDidAdded(v);
					_updateMutex.Unlock();
					return true;
				}
			}
		}
	}
	
	_updateMutex.Unlock();
	return false;
}

/// Inserts view below 'targetView'.
/// After adding view Retain will call.
/// Returns false if one of params is NULL or 'targetView' not containes.
REBOOL RESubViewsContainer::InsertSubViewBelow(REView * v, REView * targetView)
{
	_updateMutex.Lock();
	
	if (v && targetView) 
	{
		REObjectsArray * a = this->GetOrCreateAndGetSubViewsArray();
		if (a) 
		{
			REUInt32 insertIndex = a->Search(targetView);
			if (insertIndex != RENotFound) 
			{
				if (a->Insert(insertIndex, v)) 
				{
					v->_parentSubViewsContainer = this;
					this->OnViewDidAdded(v);
					_updateMutex.Unlock();
					return true;
				}
			}
		}
	}
	
	_updateMutex.Unlock();
	return false;
}

void RESubViewsContainer::LockUpdate()
{
	_updateMutex.Lock();
}

void RESubViewsContainer::UnLockUpdate()
{
	_updateMutex.Unlock();
}

void RESubViewsContainer::RemoveAllSubViews()
{
	_updateMutex.Lock();
	
	if (_subViewsArray) 
	{
		for (REUInt32 i = 0; i < _subViewsArray->Count(); i++) 
		{
			REView * v = (REView *)(*_subViewsArray)[i];
			v->_parentSubViewsContainer = NULL;
			this->OnViewDidRemoved(v);
		}
		_subViewsArray->Clear();
	}

	_updateMutex.Unlock();
}

REObjectsArray * RESubViewsContainer::GetOrCreateAndGetSubViewsArray()
{
	if (_subViewsArray) 
	{
		return _subViewsArray;
	}
	
	_updateMutex.Lock();
	
	_subViewsArray = REObjectsArray::Create();
	
	_updateMutex.Unlock();
		
	return _subViewsArray;
}

RESubViewsContainer::RESubViewsContainer() :
	_subViewsArray(NULL),
	_parentSubViewsContainer(NULL)
{
	_updateMutex.Init(REMutexTypeRecursive);
}

RESubViewsContainer::~RESubViewsContainer()
{
	this->RemoveAllSubViews();
	
	if (_subViewsArray) 
	{
		_subViewsArray->Release();
	}
	
}



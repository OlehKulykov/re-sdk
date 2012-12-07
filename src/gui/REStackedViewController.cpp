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


#include "../../include/regui/REStackedViewController.h"

const REUInt32 REStackedViewController::GetClassIdentifier() const
{
	return REStackedViewController::ClassIdentifier();
}

const REUInt32 REStackedViewController::ClassIdentifier()
{
	static const REUInt32 classIdentif = REObject::GenerateClassIdentifierFromClassName("REStackedViewController");
	return classIdentif;
}

REBOOL REStackedViewController::IsImplementsClass(const REUInt32 classIdentifier) const
{
	return ((REStackedViewController::ClassIdentifier() == classIdentifier) ||
			REViewController::IsImplementsClass(classIdentifier));
}

REBOOL REStackedViewController::PushSubViews(const REBOOL isRemoveCurrentSubViews)
{
	this->LockUpdate();
	REObjectsArray * subViews = this->GetOrCreateAndGetSubViewsArray(); 
	if (subViews) 
	{
		if (_stackedSubViewsArray == NULL) 
		{
			_stackedSubViewsArray = new REArray<REObjectsArray *>(); 
		}
		
		if (_stackedSubViewsArray) 
		{
			REObjectsArray * newSubViews = REObjectsArray::CreateWithObjectsArray(subViews);
			if (newSubViews) 
			{
				if (_stackedSubViewsArray->Add(newSubViews)) 
				{
					if (isRemoveCurrentSubViews) 
					{
						subViews->Clear();
					}
					this->UnLockUpdate();
					return true;
				}
				newSubViews->Release();
			}
		}
	}
	this->UnLockUpdate();
	return false;
}

REBOOL REStackedViewController::PopSubViews()
{
	this->LockUpdate();
	REObjectsArray * subViews = this->GetOrCreateAndGetSubViewsArray(); 
	if (_stackedSubViewsArray && subViews) 
	{
		if (_stackedSubViewsArray->Count()) 
		{
			subViews->Clear();
			REObjectsArray * subViewsArray = _stackedSubViewsArray->LastObject();
			subViews->Set(*subViewsArray);
			_stackedSubViewsArray->RemoveLast();
			if (_stackedSubViewsArray->IsEmpty()) 
			{
				delete _stackedSubViewsArray;
				_stackedSubViewsArray = NULL;
			}
			subViewsArray->Release();
			this->UnLockUpdate();
			return true;
		}
	}
	this->UnLockUpdate();
	return false;
}

REStackedViewController::REStackedViewController() : REViewController(),
	_stackedSubViewsArray(NULL)
{
	
}

void REStackedViewController::OnReleased()
{
	if (_stackedSubViewsArray) 
	{
		for (REUInt32 i = 0; i < _stackedSubViewsArray->Count(); i++) 
		{
			REObjectsArray * subViewsArray = (*_stackedSubViewsArray)[i];
			subViewsArray->Release();
		}
		delete _stackedSubViewsArray;
		_stackedSubViewsArray = NULL;
	}
	
	REViewController::OnReleased();
}

REStackedViewController::~REStackedViewController()
{
	
}

REStackedViewController * REStackedViewController::Create()
{
	REStackedViewController * newStackedViewController = new REStackedViewController();
	return newStackedViewController;
}


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


#include "../../include/regui/REStackedViewController.h"

REBOOL REStackedViewController::pushSubViews(const REBOOL isRemoveCurrentSubViews)
{
//	REArrayObject * subViews = this->getOrCreateAndGetSubViewsArray(); 
//	if (subViews) 
//	{
//		if (_stackedSubViewsArray == NULL) 
//		{
//			_stackedSubViewsArray = new REArray<REArrayObject *>(); 
//		}
//		
//		if (_stackedSubViewsArray) 
//		{
//			REArrayObject * newSubViews = REArrayObject::createWithObjectsArray(subViews);
//			if (newSubViews) 
//			{
//				if (_stackedSubViewsArray->add(newSubViews)) 
//				{
//					if (isRemoveCurrentSubViews) 
//					{
//						subViews->clear();
//					}
//					return true;
//				}
//				newSubViews->release();
//			}
//		}
//	}
	return false;
}

REBOOL REStackedViewController::popSubViews()
{
//	REArrayObject * subViews = this->getOrCreateAndGetSubViewsArray(); 
//	if (_stackedSubViewsArray && subViews) 
//	{
//		if (_stackedSubViewsArray->count()) 
//		{
//			subViews->clear();
//			REArrayObject * subViewsArray = _stackedSubViewsArray->lastObject();
//			subViews->set(*subViewsArray);
//			_stackedSubViewsArray->removeLast();
//			if (_stackedSubViewsArray->isEmpty()) 
//			{
//				delete _stackedSubViewsArray;
//				_stackedSubViewsArray = NULL;
//			}
//			subViewsArray->release();
//			return true;
//		}
//	}
	return false;
}

REStackedViewController::REStackedViewController() : REViewController(),
	_stackedSubViewsArray(NULL)
{
	
}

void REStackedViewController::onReleased()
{
	if (_stackedSubViewsArray) 
	{
		for (REUInt32 i = 0; i < _stackedSubViewsArray->count(); i++) 
		{
			REArrayObject * subViewsArray = (*_stackedSubViewsArray)[i];
			subViewsArray->release();
		}
		delete _stackedSubViewsArray;
		_stackedSubViewsArray = NULL;
	}
	
	REViewController::onReleased();
}

REStackedViewController::~REStackedViewController()
{
	
}

REStackedViewController * REStackedViewController::create()
{
	REStackedViewController * newStackedViewController = new REStackedViewController();
	return newStackedViewController;
}


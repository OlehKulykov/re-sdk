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

const REUInt32 REStackedViewController::getClassIdentifier() const
{
	return REStackedViewController::classIdentifier();
}

const REUInt32 REStackedViewController::classIdentifier()
{
	static const REUInt32 classIdentif = REObject::generateClassIdentifierFromClassName("REStackedViewController");
	return classIdentif;
}

REBOOL REStackedViewController::isImplementsClass(const REUInt32 classIdentifier) const
{
	return ((REStackedViewController::classIdentifier() == classIdentifier) ||
			REViewController::isImplementsClass(classIdentifier));
}

REBOOL REStackedViewController::pushSubViews(const REBOOL isRemoveCurrentSubViews)
{
	this->lockUpdate();
<<<<<<< HEAD
	REArrayObject * subViews = this->getOrCreateAndGetSubViewsArray(); 
=======
	REObjectsArray * subViews = this->getOrCreateAndGetSubViewsArray(); 
>>>>>>> 5cb88de87e4cabcd359f1f70380107622060d639
	if (subViews) 
	{
		if (_stackedSubViewsArray == NULL) 
		{
			_stackedSubViewsArray = new REArray<REArrayObject *>(); 
		}
		
		if (_stackedSubViewsArray) 
		{
<<<<<<< HEAD
			REArrayObject * newSubViews = REArrayObject::createWithObjectsArray(subViews);
=======
			REObjectsArray * newSubViews = REObjectsArray::createWithObjectsArray(subViews);
>>>>>>> 5cb88de87e4cabcd359f1f70380107622060d639
			if (newSubViews) 
			{
				if (_stackedSubViewsArray->add(newSubViews)) 
				{
					if (isRemoveCurrentSubViews) 
					{
						subViews->clear();
					}
					this->unLockUpdate();
					return true;
				}
				newSubViews->release();
			}
		}
	}
	this->unLockUpdate();
	return false;
}

REBOOL REStackedViewController::popSubViews()
{
	this->lockUpdate();
<<<<<<< HEAD
	REArrayObject * subViews = this->getOrCreateAndGetSubViewsArray(); 
=======
	REObjectsArray * subViews = this->getOrCreateAndGetSubViewsArray(); 
>>>>>>> 5cb88de87e4cabcd359f1f70380107622060d639
	if (_stackedSubViewsArray && subViews) 
	{
		if (_stackedSubViewsArray->count()) 
		{
			subViews->clear();
<<<<<<< HEAD
			REArrayObject * subViewsArray = _stackedSubViewsArray->lastObject();
=======
			REObjectsArray * subViewsArray = _stackedSubViewsArray->lastObject();
>>>>>>> 5cb88de87e4cabcd359f1f70380107622060d639
			subViews->set(*subViewsArray);
			_stackedSubViewsArray->removeLast();
			if (_stackedSubViewsArray->isEmpty()) 
			{
				delete _stackedSubViewsArray;
				_stackedSubViewsArray = NULL;
			}
			subViewsArray->release();
			this->unLockUpdate();
			return true;
		}
	}
	this->unLockUpdate();
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
<<<<<<< HEAD
			REArrayObject * subViewsArray = (*_stackedSubViewsArray)[i];
=======
			REObjectsArray * subViewsArray = (*_stackedSubViewsArray)[i];
>>>>>>> 5cb88de87e4cabcd359f1f70380107622060d639
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


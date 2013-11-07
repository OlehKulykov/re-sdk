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


#include "../../include/regui/private/REAnimationController.h"

__RE_PUBLIC_CLASS_API__ REAnimationController * REAnimationController::_defaultController = NULL;

REEditableAnimationInfo * REAnimationController::lastInfo() 
{
	return _setuping.lastObject();
}

void REAnimationController::trySelfRelease()
{
	if (_setuping.isEmpty() && _working.isEmpty()) 
	{
		_defaultController = NULL;
		this->release();
	}
}

REBOOL REAnimationController::stopAllAnimationForView(REObject * view, const REAnimationStopType stopType, REBOOL isNeedCallDelegate)
{
	for (REUInt32 i = 0; i < _working.count(); i++) 
	{
		REEditableAnimationInfo * info = _working.at(i);
		if (info)
		{
			info->stopAllAnimationForView(view, stopType);
			if (info->isEmpty() && (!isNeedCallDelegate)) 
			{
				if (_working.removeAt(i))
				{
					info->release();
				}
			}
		}
	}
	return true;
}

void REAnimationController::animationFinishedSuccessfully(REAnimationInfo * animationInfo)
{
	const REUIdentifier animationID = animationInfo->objectIdentifier();
	for (REUInt32 i = 0; i < _working.count(); i++) 
	{
		REEditableAnimationInfo * info = _working.at(i);
		if (info)
		{
			if (animationID == info->objectIdentifier()) 
			{
				if (!_working.removeAt(i)) { _working.setAt(i, NULL); }
				info->decrementCounterAndReleaseAllViews();
				break;
			}
		}
	}
	
	if (animationInfo->getStopMethod()) 
	{
		animationInfo->getStopMethod()->invokeWithObject(animationInfo);
	}
	animationInfo->release();
	
	this->trySelfRelease();
}

REBOOL REAnimationController::addFloatParam(REObject * view, 
											REUInt16 * animationsCounter,
											REFloat32 * param, 
											const REFloat32 startValue, 
											const REFloat32 endValue)
{
	REEditableAnimationInfo * lastInfo = _setuping.lastObject();
	if (lastInfo && view && param) 
	{
		return lastInfo->addFloatParam(view, animationsCounter, param, startValue, endValue);
	}
	return false;
}

REUIdentifier REAnimationController::startSetupNewAnimation(void * data)
{	
	REEditableAnimationInfo * newInfo = REEditableAnimationInfo::createWithCustomData(data);
	if (newInfo) 
	{
		if (_setuping.add(newInfo))
		{
			return newInfo->objectIdentifier();
		}
		else
		{
			newInfo->release();
		}
	}
	return 0;
}

REBOOL REAnimationController::executeLastAnimation()
{
	REEditableAnimationInfo * lastInfo = _setuping.lastObject();
	if (lastInfo) 
	{
		if (lastInfo->executeAnimation())
		{
			_working.add(lastInfo);
			_setuping.removeLast();
			return true;
		}
	}
	return false;
}

void REAnimationController::pause()
{
	if (!_isPaused) 
	{
		for (REUInt32 i = 0; i < _working.count(); i++)
		{
			REEditableAnimationInfo * info = _working.at(i);
			if (info) 
			{
				info->pause();
			}
		}
	}
}

void REAnimationController::Continue()
{
	if (_isPaused) 
	{
		for (REUInt32 i = 0; i < _working.count(); i++)
		{
			REEditableAnimationInfo * info = _working.at(i);
			if (info) 
			{
				info->Continue();
			}
		}	
	}
}

const REFloat32 REAnimationController::getProgress(REObject * animatedObject)
{
	REUInt32 i = 0;
	while (i < _working.count()) 
	{
		REEditableAnimationInfo * info = _working.at(i);
		if (info) 
		{
			const REFloat32 p = info->getProgress(animatedObject);
			if (p < 0.0f) { i++; }
			else { return p; }
		}
		else
		{
			i++;
		}
	}
	return -1.0f;
}

REAnimationController::REAnimationController() : REObject(),
	_isPaused(false)
{
	
}

REAnimationController::~REAnimationController()
{
	for (REUInt32 i = 0; i < _working.count(); i++) 
	{
		REEditableAnimationInfo * info = _working.at(i);
		if (info) { _working.setAt(i, NULL); info->release(); };
	}
	
	for (REUInt32 i = 0; i < _setuping.count(); i++) 
	{
		REEditableAnimationInfo * info = _setuping.at(i);
		if (info) { _setuping.setAt(i, NULL); info->release(); }
	}
}

REAnimationController * REAnimationController::defaultController()
{
	if (_defaultController)
	{
		return _defaultController;
	}
	
	_defaultController = new REAnimationController();
	return _defaultController;
}

void REAnimationController::releaseDefaultController()
{
	REAnimationController * c = _defaultController;
	_defaultController = NULL;
	if (c)
	{
		delete c;
	}
}



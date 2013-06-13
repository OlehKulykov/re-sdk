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

REEditableAnimationInfo * REAnimationController::LastInfo() 
{
	return _setuping.LastObject();
}

void REAnimationController::TrySelfRelease()
{
	if (_setuping.IsEmpty() && _working.IsEmpty()) 
	{
		_defaultController = NULL;
		this->Release();
	}
}

REBOOL REAnimationController::StopAllAnimationForView(REObject * view, const REAnimationStopType stopType, REBOOL isNeedCallDelegate)
{
	for (REUInt32 i = 0; i < _working.Count(); i++) 
	{
		REEditableAnimationInfo * info = _working.At(i);
		if (info)
		{
			info->StopAllAnimationForView(view, stopType);
			if (info->IsEmpty() && (!isNeedCallDelegate)) 
			{
				if (_working.RemoveAt(i))
				{
					info->Release();
				}
			}
		}
	}
	return true;
}

void REAnimationController::AnimationFinishedSuccessfully(REAnimationInfo * animationInfo)
{
	const REUIdentifier animationID = animationInfo->GetObjectIdentifier();
	for (REUInt32 i = 0; i < _working.Count(); i++) 
	{
		REEditableAnimationInfo * info = _working.At(i);
		if (info)
		{
			if (animationID == info->GetObjectIdentifier()) 
			{
				if (!_working.RemoveAt(i)) { _working.SetAt(i, NULL); }
				info->DecrementCounterAndReleaseAllViews();
				break;
			}
		}
	}
	
	if (animationInfo->GetStopMethod()) 
	{
		animationInfo->GetStopMethod()->InvokeWithObject(animationInfo);
	}
	animationInfo->Release();
	
	this->TrySelfRelease();
}

REBOOL REAnimationController::AddFloatParam(REObject * view, 
											REUInt16 * animationsCounter,
											REFloat32 * param, 
											const REFloat32 startValue, 
											const REFloat32 endValue)
{
	REEditableAnimationInfo * lastInfo = _setuping.LastObject();
	if (lastInfo && view && param) 
	{
		return lastInfo->AddFloatParam(view, animationsCounter, param, startValue, endValue);
	}
	return false;
}

REUIdentifier REAnimationController::StartSetupNewAnimation(void * data)
{	
	REEditableAnimationInfo * newInfo = REEditableAnimationInfo::CreateWithCustomData(data);
	if (newInfo) 
	{
		if (_setuping.Add(newInfo))
		{
			return newInfo->GetObjectIdentifier();
		}
		else
		{
			newInfo->Release();
		}
	}
	return 0;
}

REBOOL REAnimationController::ExecuteLastAnimation()
{
	REEditableAnimationInfo * lastInfo = _setuping.LastObject();
	if (lastInfo) 
	{
		if (lastInfo->ExecuteAnimation())
		{
			_working.Add(lastInfo);
			_setuping.RemoveLast();
			return true;
		}
	}
	return false;
}

void REAnimationController::Pause()
{
	if (!_isPaused) 
	{
		for (REUInt32 i = 0; i < _working.Count(); i++)
		{
			REEditableAnimationInfo * info = _working.At(i);
			if (info) 
			{
				info->Pause();
			}
		}
	}
}

void REAnimationController::Continue()
{
	if (_isPaused) 
	{
		for (REUInt32 i = 0; i < _working.Count(); i++)
		{
			REEditableAnimationInfo * info = _working.At(i);
			if (info) 
			{
				info->Continue();
			}
		}	
	}
}

const REFloat32 REAnimationController::GetProgress(REObject * animatedObject)
{
	REUInt32 i = 0;
	while (i < _working.Count()) 
	{
		REEditableAnimationInfo * info = _working.At(i);
		if (info) 
		{
			const REFloat32 p = info->GetProgress(animatedObject);
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
	for (REUInt32 i = 0; i < _working.Count(); i++) 
	{
		REEditableAnimationInfo * info = _working.At(i);
		if (info) { _working.SetAt(i, NULL); info->Release(); };
	}
	
	for (REUInt32 i = 0; i < _setuping.Count(); i++) 
	{
		REEditableAnimationInfo * info = _setuping.At(i);
		if (info) { _setuping.SetAt(i, NULL); info->Release(); }
	}
}

REAnimationController * REAnimationController::GetDefaultController()
{
	if (_defaultController)
	{
		return _defaultController;
	}
	
	_defaultController = new REAnimationController();
	return _defaultController;
}

void REAnimationController::ReleaseDefaultController()
{
	REAnimationController * c = _defaultController;
	_defaultController = NULL;
	if (c)
	{
		delete c;
	}
}



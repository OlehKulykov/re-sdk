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


#include "../../include/regui/REAnimation.h"
#include "../../include/regui/private/REAnimationController.h"


REUIdentifier REAnimation::Setup(void * customDataOrNull)
{
	return REAnimationController::GetDefaultController()->StartSetupNewAnimation(customDataOrNull);
}

REBOOL REAnimation::IsSetuping()
{
	return (REAnimationController::GetDefaultController()->LastInfo() != NULL);
}

REBOOL REAnimation::AddFloatParam(REObject * animatedObject, 
								  REUInt16 * animationsCounterOrNULL,
								  REFloat32 * param, 
								  const REFloat32 startValue, 
								  const REFloat32 endValue)
{
	return REAnimationController::GetDefaultController()->AddFloatParam(animatedObject,
																		animationsCounterOrNULL,
																		param,
																		startValue,
																		endValue);
}

void REAnimation::SetTime(const RETimeInterval animationTime)
{
	REEditableAnimationInfo * info = REAnimationController::GetDefaultController()->LastInfo();
	if (info) { info->SetTime(animationTime); }
}

void REAnimation::SetStartDelay(const RETimeInterval delayTime)
{
	REEditableAnimationInfo * info = REAnimationController::GetDefaultController()->LastInfo();
	if (info) { info->SetStartDelay(delayTime); }
}

void REAnimation::SetType(const REAnimationType animationType)
{
	REEditableAnimationInfo * info = REAnimationController::GetDefaultController()->LastInfo();
	if (info) { info->SetAnimationType(animationType); }
}

void REAnimation::SetLoopType(const REAnimationLoopType loopType, const REUInt32 loopsCount)
{
	REEditableAnimationInfo * info = REAnimationController::GetDefaultController()->LastInfo();
	if (info) { info->SetLoopType(loopType); info->SetLoopsCount(loopsCount); }
}

void REAnimation::SetDidStartMethod(REClassMethod * startMethod)
{
	REEditableAnimationInfo * info = REAnimationController::GetDefaultController()->LastInfo();
	if (info) { info->SetStartMethod(startMethod); }
}

void REAnimation::SetDidStopMethod(REClassMethod * stopMethod)
{
	REEditableAnimationInfo * info = REAnimationController::GetDefaultController()->LastInfo();
	if (info) { info->SetStopMethod(stopMethod); }
}

REBOOL REAnimation::Execute()
{
	return REAnimationController::GetDefaultController()->ExecuteLastAnimation();
}

void REAnimation::PauseAllAnimations()
{
	REAnimationController::GetDefaultController()->Pause();
}

void REAnimation::ContinueAllAnimations()
{
	REAnimationController::GetDefaultController()->Continue();
}

REBOOL REAnimation::StopAllAnimations(REObject * animatedObject,
									  const REAnimationStopType stopType, 
									  REBOOL isNeedCallDelegate)
{
	return REAnimationController::GetDefaultController()->StopAllAnimationForView(animatedObject, stopType, isNeedCallDelegate);
}

const REFloat32 REAnimation::GetProgress(REObject * animatedObject)
{
	if (animatedObject) 
	{
		return REAnimationController::GetDefaultController()->GetProgress(animatedObject);
	}
	return -1.0f;
}




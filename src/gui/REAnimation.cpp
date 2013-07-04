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


#include "../../include/regui/REAnimation.h"
#include "../../include/regui/private/REAnimationController.h"


REUIdentifier REAnimation::setup(void * customDataOrNull)
{
	return REAnimationController::getDefaultController()->startSetupNewAnimation(customDataOrNull);
}

REBOOL REAnimation::isSetuping()
{
	return (REAnimationController::getDefaultController()->lastInfo() != NULL);
}

REBOOL REAnimation::addFloatParam(REObject * animatedObject, 
								  REUInt16 * animationsCounterOrNULL,
								  REFloat32 * param, 
								  const REFloat32 startValue, 
								  const REFloat32 endValue)
{
	return REAnimationController::getDefaultController()->addFloatParam(animatedObject,
																		animationsCounterOrNULL,
																		param,
																		startValue,
																		endValue);
}

void REAnimation::setTime(const RETimeInterval animationTime)
{
	REEditableAnimationInfo * info = REAnimationController::getDefaultController()->lastInfo();
	if (info) { info->setTime(animationTime); }
}

void REAnimation::setStartDelay(const RETimeInterval delayTime)
{
	REEditableAnimationInfo * info = REAnimationController::getDefaultController()->lastInfo();
	if (info) { info->setStartDelay(delayTime); }
}

void REAnimation::setType(const REAnimationType animationType)
{
	REEditableAnimationInfo * info = REAnimationController::getDefaultController()->lastInfo();
	if (info) { info->setAnimationType(animationType); }
}

void REAnimation::setLoopType(const REAnimationLoopType loopType, const REUInt32 loopsCount)
{
	REEditableAnimationInfo * info = REAnimationController::getDefaultController()->lastInfo();
	if (info) { info->setLoopType(loopType); info->setLoopsCount(loopsCount); }
}

void REAnimation::setDidStartMethod(REClassMethod * startMethod)
{
	REEditableAnimationInfo * info = REAnimationController::getDefaultController()->lastInfo();
	if (info) { info->setStartMethod(startMethod); }
}

void REAnimation::setDidStopMethod(REClassMethod * stopMethod)
{
	REEditableAnimationInfo * info = REAnimationController::getDefaultController()->lastInfo();
	if (info) { info->setStopMethod(stopMethod); }
}

REBOOL REAnimation::execute()
{
	return REAnimationController::getDefaultController()->executeLastAnimation();
}

void REAnimation::pauseAllAnimations()
{
	REAnimationController::getDefaultController()->pause();
}

void REAnimation::continueAllAnimations()
{
	REAnimationController::getDefaultController()->Continue();
}

REBOOL REAnimation::stopAllAnimations(REObject * animatedObject,
									  const REAnimationStopType stopType, 
									  REBOOL isNeedCallDelegate)
{
	return REAnimationController::getDefaultController()->stopAllAnimationForView(animatedObject, stopType, isNeedCallDelegate);
}

const REFloat32 REAnimation::getProgress(REObject * animatedObject)
{
	if (animatedObject) 
	{
		return REAnimationController::getDefaultController()->getProgress(animatedObject);
	}
	return -1.0f;
}




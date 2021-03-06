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


#ifndef __REANIMATION_H__
#define __REANIMATION_H__

#include "../recore/RECommonHeader.h"
#include "../recore/REClassMethod.h"
#include "IREAnimation.h"
#include "REAnimationInfo.h"
#include "IREAnimationRespondent.h"


/// Class using for managing objects animation.
class __RE_PUBLIC_CLASS_API__ REAnimation 
{
public:
	/// Start new animation block.
	/// Returns 0 - on error and non zero value of animation identifier.
	/// 'customDataOrNull' - pointer to any custom data. Don't forget clean up memory of custom data after animation finished.
	static REUIdentifier setup(void * customDataOrNull);
	
	/// Returns value indicating that animation is setuping.
	static REBOOL isSetuping();
	
	/// Adding float param for animation.
	/// 'animationsCounterOrNULL' - may be NULL.
	static REBOOL addFloatParam(REObject * animatedObject, 
								REUInt16 * animationsCounterOrNULL,
								REFloat32 * param, 
								const REFloat32 startValue, 
								const REFloat32 endValue);
	
	/// Setting time of animation.
	static void setTime(const RETimeInterval animationTime);
	
	/// Setting start delay of animation.
	static void setStartDelay(const RETimeInterval delayTime);
	
	/// Setting type of animation.
	static void setType(const REAnimationType animationType);
	
	/// Setting animation looping.
	static void setLoopType(const REAnimationLoopType loopType, const REUInt32 loopsCount = RENotFound);
	
	/// Setting method called when animation did start.
	static void setDidStartMethod(REClassMethod * startMethod);
	
	/// Setting method called when animation did end.
	static void setDidStopMethod(REClassMethod * stopMethod);
	
	/// Executing/starting animation.
	static REBOOL execute();
	
	/// Globaly pause all application animation.
	static void pauseAllAnimations();
	
	/// Globaly continue all application animation.
	static void continueAllAnimations();
	
	/// Stops all assigned object animations.
	static REBOOL stopAllAnimations(REObject * animatedObject,
									const REAnimationStopType stopType, 
									REBOOL isNeedCallDelegate);
	
	/// Returns animation progress in range [0.0f, 1.0f] of 'animatedObject'.
	/// If 'animatedObject' is NULL or not currently animating a negative value will return(usially -1.0f).
	static const REFloat32 getProgress(REObject * animatedObject);
};


#endif /* __REANIMATION_H__ */



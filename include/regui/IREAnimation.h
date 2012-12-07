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


#ifndef __IREANIMATION_H__
#define __IREANIMATION_H__

#include "../recore/RECommonHeader.h"

/// Types of animations.
typedef enum _reAnimationType
{
	/// Animation is linear. All params animated evenly(равномерно). Default type of animation.
	REAnimationTypeLinear = 0,
	 
	REAnimationTypeQuadEaseIn = 1,
	REAnimationTypeQuadEaseOut = 2,
	REAnimationTypeQuadEaseInOut = 3,
	REAnimationTypeQuadEaseOutIn = 4,
	REAnimationTypeCubicEaseIn = 5,
	REAnimationTypeCubicEaseOut = 6,
	REAnimationTypeCubicEaseInOut = 7,
	REAnimationTypeCubicEaseOutIn = 8,
	REAnimationTypeQuarticEaseIn = 9,
	REAnimationTypeQuarticEaseOut = 10,
	REAnimationTypeQuarticEaseInOut = 11,
	REAnimationTypeQuarticEaseOutIn = 12,
	REAnimationTypeQuinticEaseIn = 13,
	REAnimationTypeQuinticEaseOut = 14,
	REAnimationTypeQuinticEaseInOut = 15,
	REAnimationTypeQuinticEaseOutIn = 16,
	REAnimationTypeSinusoidalEaseIn = 17,
	REAnimationTypeSinusoidalEaseOut = 18,
	REAnimationTypeSinusoidalEaseInOut = 19,
	REAnimationTypeSinusoidalEaseOutIn = 20,
	REAnimationTypeExponentialEaseIn = 21,
	REAnimationTypeExponentialEaseOut = 22,
	REAnimationTypeExponentialEaseInOut = 23,
	REAnimationTypeExponentialEaseOutIn = 24,
	REAnimationTypeCircularEaseIn = 25,
	REAnimationTypeCircularEaseOut = 26,
	REAnimationTypeCircularEaseInOut = 27,
	REAnimationTypeCircularEaseOutIn = 28,
	REAnimationTypeElasticEaseIn = 29,
	REAnimationTypeElasticEaseOut = 30,
	REAnimationTypeElasticEaseInOut = 31,
	REAnimationTypeElasticEaseOutIn = 32,
	REAnimationTypeBackEaseIn = 33,
	REAnimationTypeBackEaseOut = 34,
	REAnimationTypeBackEaseInOut = 35,
	REAnimationTypeBackEaseOutIn = 36,
	REAnimationTypeBounceEaseIn = 37,
	REAnimationTypeBounceEaseOut = 38,
	REAnimationTypeBounceEaseInOut = 39,
	REAnimationTypeBounceEaseOutIn = 40
} REAnimationType;


/// Loping type of animation.
typedef enum _reAnimationLoop 
{
	/// Animation not looped. Default type.
	REAnimationLoopTypeNone = 0,
	
	/// Animation at next loop will start from start values.
	REAnimationLoopTypeFromStartToEnd = 1,
	
	/// Animation at next loop will start from end values.
	REAnimationLoopTypeFromEndToStart = 2
} REAnimationLoopType;


/// Stop type of animation.
typedef enum _reAnimationStopType
{
	/// Animation stops as is. Default type.
	REAnimationStopTypeImmediately = 0,
	
	/// Animation stops with setting params to it's start values.
	REAnimationStopTypeImmediatelyToStart = 1,
	
	/// Animation stops with setting params to it's end values. 
	REAnimationStopTypeImmediatelyToEnd = 2,
} REAnimationStopType;


/// Abstract class interface using for animation objects. 
class __RE_PUBLIC_CLASS_API__ IREAnimation
{
public:
	/// Return progress of animation in range [0.0f, 1.0f].
	virtual const REFloat32 GetProgress() const = 0;
	
	/// Starts animation.
	virtual void Start() = 0;
	
	/// Pausing animation.
	virtual void Pause() = 0;
	
	/// Stops animation.
	virtual void Stop(const REAnimationStopType stopType) = 0;
	
	/// Destructor.
	virtual ~IREAnimation() { }
};


#endif /* __IREANIMATION_H__ */



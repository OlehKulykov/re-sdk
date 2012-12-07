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


#ifndef __RETYPEDANIMATION_H__
#define __RETYPEDANIMATION_H__

#include "REAnimationBase.h"
#include "../../recore/REInterpolation.h"
#include "../../recore/REMainLoopUpdatable.h"

/// Constructs class depends of animation type. All logic in REAnimationBase and here
/// just inline classes without runtime selecting interpolation algorithm.
/// Each animation type has own class realized just it's functionality. Ugly code. 
#define ANIMATIONCLASS(AnimationClassByType) \
class __RE_PUBLIC_CLASS_API__ AnimationClassByType : public REObject, public REAnimationBase, public REMainLoopUpdatable \
{ \
protected: \
	virtual void ToMainLoop() \
	{ \
		this->AddToMainLoop(); \
	} \
	\
	virtual void FromMainLoop() \
	{ \
		this->RemoveFromMainLoop(); \
	} \
	\
public: \
	virtual void Update(const RETimeInterval currentTime); \
	virtual const REUIdentifier GetMainLoopUpdatableIdentifier() const \
	{ \
		return this->GetObjectIdentifier(); \
	} \
	\
	virtual const REUIdentifier GetAnimationIdentifier() const \
	{ \
		return this->GetObjectIdentifier(); \
	} \
	AnimationClassByType(REAnimationInfo * info, REArray<REAnimationFloatParams *> * params) : \
		REObject(), REAnimationBase(info, params) { } \
	virtual ~AnimationClassByType() \
	{ \
		this->RemoveFromMainLoop(); \
	} \
}; \
 \


ANIMATIONCLASS(RELinearAnimation) 
ANIMATIONCLASS(REQuadEaseInAnimation) 
ANIMATIONCLASS(REQuadEaseOutAnimation) 
ANIMATIONCLASS(REQuadEaseInOutAnimation)
ANIMATIONCLASS(REQuadEaseOutInAnimation)
ANIMATIONCLASS(RECubicEaseInAnimation)
ANIMATIONCLASS(RECubicEaseOutAnimation)
ANIMATIONCLASS(RECubicEaseInOutAnimation)
ANIMATIONCLASS(RECubicEaseOutInAnimation)
ANIMATIONCLASS(REQuarticEaseInAnimation)
ANIMATIONCLASS(REQuarticEaseOutAnimation)
ANIMATIONCLASS(REQuarticEaseInOutAnimation)
ANIMATIONCLASS(REQuarticEaseOutInAnimation)
ANIMATIONCLASS(REQuinticEaseInAnimation)
ANIMATIONCLASS(REQuinticEaseOutAnimation)
ANIMATIONCLASS(REQuinticEaseInOutAnimation)
ANIMATIONCLASS(REQuinticEaseOutInAnimation)
ANIMATIONCLASS(RESinusoidalEaseInAnimation)
ANIMATIONCLASS(RESinusoidalEaseOutAnimation)
ANIMATIONCLASS(RESinusoidalEaseInOutAnimation)
ANIMATIONCLASS(RESinusoidalEaseOutInAnimation)
ANIMATIONCLASS(REExponentialEaseInAnimation)
ANIMATIONCLASS(REExponentialEaseOutAnimation)
ANIMATIONCLASS(REExponentialEaseInOutAnimation)
ANIMATIONCLASS(REExponentialEaseOutInAnimation)
ANIMATIONCLASS(RECircularEaseInAnimation)
ANIMATIONCLASS(RECircularEaseOutAnimation)
ANIMATIONCLASS(RECircularEaseInOutAnimation)
ANIMATIONCLASS(RECircularEaseOutInAnimation)
ANIMATIONCLASS(REElasticEaseInAnimation)
ANIMATIONCLASS(REElasticEaseOutAnimation)
ANIMATIONCLASS(REElasticEaseInOutAnimation)
ANIMATIONCLASS(REElasticEaseOutInAnimation)
ANIMATIONCLASS(REBackEaseInAnimation)
ANIMATIONCLASS(REBackEaseOutAnimation)
ANIMATIONCLASS(REBackEaseInOutAnimation)
ANIMATIONCLASS(REBackEaseOutInAnimation)
ANIMATIONCLASS(REBounceEaseInAnimation)
ANIMATIONCLASS(REBounceEaseOutAnimation)
ANIMATIONCLASS(REBounceEaseInOutAnimation)
ANIMATIONCLASS(REBounceEaseOutInAnimation)


#endif /* __RETYPEDANIMATION_H__ */



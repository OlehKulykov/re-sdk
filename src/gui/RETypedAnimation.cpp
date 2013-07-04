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


#include "../../include/regui/private/RETypedAnimation.h"


#define ANIMATION_BODY(mulLenghtParam) \
if ( ( currentTime >= _startTime ) && ( currentTime < _endTime ) ) \
{	if (_isNeedCallStartMethod) \
	{ \
		if (_info->getStartMethod()) { _info->getStartMethod()->invokeWithObject(_info); } \
		_isNeedCallStartMethod = false; \
		for (REUInt32 i = 0; i < _params->count(); i++) \
		{ \
			REAnimationFloatParams * p = (*_params)[i]; \
			if (p->respondent) { p->respondent->REAnimationRespondentAnimationDidStarted(this->getAnimationIdentifier()); } \
		} \
	} \
	_progress = (REFloat32)((currentTime - _startTime) / _workingTime); \
	for (REUInt32 i = 0; i < _params->count(); i++) \
	{ \
		REAnimationFloatParams * p = (*_params)[i]; \
		for (REUInt32 j = 0; j < p->params.count(); j++) \
		{ \
			REAnimationFloatParams::ParamStruct * s = &(p->params[j]); \
			(*s->paramPtr) = (s->start + (s->length * mulLenghtParam)); \
		} \
		\
		if (p->respondent) { p->respondent->REAnimationRespondentAnimationDidStep(_progress); } \
	} \
} \
else if ( currentTime >= _endTime ) { this->timeIsOver(); }


void RELinearAnimation::update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(_progress);
}

void REQuadEaseInAnimation::update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::quadEaseIn(_progress));
}

void REQuadEaseOutAnimation::update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::quadEaseOut(_progress));
}

void REQuadEaseInOutAnimation::update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::quadEaseInOut(_progress));
}

void REQuadEaseOutInAnimation::update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::quadEaseOutIn(_progress));
}

void RECubicEaseInAnimation::update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::cubicEaseIn(_progress));
}

void RECubicEaseOutAnimation::update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::cubicEaseOut(_progress));
}

void RECubicEaseInOutAnimation::update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::cubicEaseInOut(_progress));
}

void RECubicEaseOutInAnimation::update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::cubicEaseOutIn(_progress));
}

void REQuarticEaseInAnimation::update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::quarticEaseIn(_progress));
}

void REQuarticEaseOutAnimation::update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::quarticEaseOut(_progress));
}

void REQuarticEaseInOutAnimation::update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::quarticEaseInOut(_progress));
}

void REQuarticEaseOutInAnimation::update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::quarticEaseOutIn(_progress));
}

void REQuinticEaseInAnimation::update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::quinticEaseIn(_progress));
}

void REQuinticEaseOutAnimation::update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::quinticEaseOut(_progress));
}

void REQuinticEaseInOutAnimation::update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::quinticEaseInOut(_progress));
}

void REQuinticEaseOutInAnimation::update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::quinticEaseOutIn(_progress));
}

void RESinusoidalEaseInAnimation::update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::sinusoidalEaseIn(_progress));
}

void RESinusoidalEaseOutAnimation::update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::sinusoidalEaseOut(_progress));
}

void RESinusoidalEaseInOutAnimation::update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::sinusoidalEaseInOut(_progress));
}

void RESinusoidalEaseOutInAnimation::update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::sinusoidalEaseOutIn(_progress));
}

void REExponentialEaseInAnimation::update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::exponentialEaseIn(_progress));
}

void REExponentialEaseOutAnimation::update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::exponentialEaseOut(_progress));
}

void REExponentialEaseInOutAnimation::update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::exponentialEaseInOut(_progress));
}

void REExponentialEaseOutInAnimation::update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::exponentialEaseOutIn(_progress));
}

void RECircularEaseInAnimation::update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::circularEaseIn(_progress));
}

void RECircularEaseOutAnimation::update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::circularEaseOut(_progress));
}

void RECircularEaseInOutAnimation::update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::circularEaseInOut(_progress));
}

void RECircularEaseOutInAnimation::update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::circularEaseOutIn(_progress));
}

void REElasticEaseInAnimation::update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::elasticEaseIn(_progress));
}

void REElasticEaseOutAnimation::update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::elasticEaseOut(_progress));
}

void REElasticEaseInOutAnimation::update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::elasticEaseInOut(_progress));
}

void REElasticEaseOutInAnimation::update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::elasticEaseOutIn(_progress));
}

void REBackEaseInAnimation::update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::backEaseIn(_progress));
}

void REBackEaseOutAnimation::update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::backEaseOut(_progress));
}

void REBackEaseInOutAnimation::update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::backEaseInOut(_progress));
}

void REBackEaseOutInAnimation::update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::backEaseOutIn(_progress));
}

void REBounceEaseInAnimation::update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::bounceEaseIn(_progress));
}

void REBounceEaseOutAnimation::update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::bounceEaseOut(_progress));
}

void REBounceEaseInOutAnimation::update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::bounceEaseInOut(_progress));
}

void REBounceEaseOutInAnimation::update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::bounceEaseOutIn(_progress));
}








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
		if (_info->GetStartMethod()) { _info->GetStartMethod()->InvokeWithObject(_info); } \
		_isNeedCallStartMethod = false; \
		for (REUInt32 i = 0; i < _params->Count(); i++) \
		{ \
			REAnimationFloatParams * p = (*_params)[i]; \
			if (p->respondent) { p->respondent->REAnimationRespondentAnimationDidStarted(this->GetAnimationIdentifier()); } \
		} \
	} \
	_progress = (REFloat32)((currentTime - _startTime) / _workingTime); \
	for (REUInt32 i = 0; i < _params->Count(); i++) \
	{ \
		REAnimationFloatParams * p = (*_params)[i]; \
		for (REUInt32 j = 0; j < p->params.Count(); j++) \
		{ \
			REAnimationFloatParams::ParamStruct * s = &(p->params[j]); \
			(*s->paramPtr) = (s->start + (s->length * mulLenghtParam)); \
		} \
		\
		if (p->respondent) { p->respondent->REAnimationRespondentAnimationDidStep(_progress); } \
	} \
} \
else if ( currentTime >= _endTime ) { this->TimeIsOver(); }


void RELinearAnimation::Update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(_progress);
}

void REQuadEaseInAnimation::Update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::QuadEaseIn(_progress));
}

void REQuadEaseOutAnimation::Update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::QuadEaseOut(_progress));
}

void REQuadEaseInOutAnimation::Update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::QuadEaseInOut(_progress));
}

void REQuadEaseOutInAnimation::Update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::QuadEaseOutIn(_progress));
}

void RECubicEaseInAnimation::Update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::CubicEaseIn(_progress));
}

void RECubicEaseOutAnimation::Update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::CubicEaseOut(_progress));
}

void RECubicEaseInOutAnimation::Update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::CubicEaseInOut(_progress));
}

void RECubicEaseOutInAnimation::Update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::CubicEaseOutIn(_progress));
}

void REQuarticEaseInAnimation::Update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::QuarticEaseIn(_progress));
}

void REQuarticEaseOutAnimation::Update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::QuarticEaseOut(_progress));
}

void REQuarticEaseInOutAnimation::Update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::QuarticEaseInOut(_progress));
}

void REQuarticEaseOutInAnimation::Update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::QuarticEaseOutIn(_progress));
}

void REQuinticEaseInAnimation::Update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::QuinticEaseIn(_progress));
}

void REQuinticEaseOutAnimation::Update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::QuinticEaseOut(_progress));
}

void REQuinticEaseInOutAnimation::Update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::QuinticEaseInOut(_progress));
}

void REQuinticEaseOutInAnimation::Update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::QuinticEaseOutIn(_progress));
}

void RESinusoidalEaseInAnimation::Update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::SinusoidalEaseIn(_progress));
}

void RESinusoidalEaseOutAnimation::Update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::SinusoidalEaseOut(_progress));
}

void RESinusoidalEaseInOutAnimation::Update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::SinusoidalEaseInOut(_progress));
}

void RESinusoidalEaseOutInAnimation::Update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::SinusoidalEaseOutIn(_progress));
}

void REExponentialEaseInAnimation::Update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::ExponentialEaseIn(_progress));
}

void REExponentialEaseOutAnimation::Update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::ExponentialEaseOut(_progress));
}

void REExponentialEaseInOutAnimation::Update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::ExponentialEaseInOut(_progress));
}

void REExponentialEaseOutInAnimation::Update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::ExponentialEaseOutIn(_progress));
}

void RECircularEaseInAnimation::Update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::CircularEaseIn(_progress));
}

void RECircularEaseOutAnimation::Update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::CircularEaseOut(_progress));
}

void RECircularEaseInOutAnimation::Update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::CircularEaseInOut(_progress));
}

void RECircularEaseOutInAnimation::Update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::CircularEaseOutIn(_progress));
}

void REElasticEaseInAnimation::Update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::ElasticEaseIn(_progress));
}

void REElasticEaseOutAnimation::Update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::ElasticEaseOut(_progress));
}

void REElasticEaseInOutAnimation::Update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::ElasticEaseInOut(_progress));
}

void REElasticEaseOutInAnimation::Update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::ElasticEaseOutIn(_progress));
}

void REBackEaseInAnimation::Update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::BackEaseIn(_progress));
}

void REBackEaseOutAnimation::Update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::BackEaseOut(_progress));
}

void REBackEaseInOutAnimation::Update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::BackEaseInOut(_progress));
}

void REBackEaseOutInAnimation::Update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::BackEaseOutIn(_progress));
}

void REBounceEaseInAnimation::Update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::BounceEaseIn(_progress));
}

void REBounceEaseOutAnimation::Update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::BounceEaseOut(_progress));
}

void REBounceEaseInOutAnimation::Update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::BounceEaseInOut(_progress));
}

void REBounceEaseOutInAnimation::Update(const RETimeInterval currentTime)
{
	ANIMATION_BODY(REInterpolation::BounceEaseOutIn(_progress));
}








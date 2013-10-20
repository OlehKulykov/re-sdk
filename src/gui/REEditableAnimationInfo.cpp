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


#include "../../include/regui/private/REEditableAnimationInfo.h"

REAnimationFloatParams * REEditableAnimationInfo::findParamForObjectInParams(REObject * view, REArray<REAnimationFloatParams *> * params)
{
	const REUIdentifier oid = view->objectIdentifier();
	for (REUInt32 i = 0; i < params->count(); i++)
	{
		REAnimationFloatParams * p = params->at(i);
		if (p)
		{
			if (oid == p->object->objectIdentifier())
			{
				return p;
			}
		}
		else
		{
			return NULL;
		}
	}
	return NULL;
}

REFloat32 REEditableAnimationInfo::getProgressForObjectAnimationInParams(REObject * animatedObject,
																		 REAnimationBase * animation,
																		 REArray<REAnimationFloatParams *> * params)
{
	const REFloat32 progress = animation->getProgress();
	const REUIdentifier oid = animatedObject->objectIdentifier();
	for (REUInt32 i = 0; i < params->count(); i++)
	{
		REAnimationFloatParams * p = params->at(i);
		if (p)
		{
			if (oid == p->object->objectIdentifier())
			{
				return progress;
			}
		}
		else
		{
			return -1.0f;
		}
	}
	return -1.0f;
}

const REFloat32 REEditableAnimationInfo::getProgress(REObject * animatedObject)
{
	if (_animation) 
	{
		return REEditableAnimationInfo::getProgressForObjectAnimationInParams(animatedObject,
																			  _animation,
																			  &_viewParams);
	}
	return -1.0f;
}

REBOOL REEditableAnimationInfo::stopAllAnimationForObjectWithStopType(REObject * animatedObject,
																	  const REAnimationStopType stopType,
																	  REArray<REAnimationFloatParams *> * params)
{
	REBOOL isFoundedAndStoped = false;
	const REUIdentifier oid = animatedObject->objectIdentifier();
	for (REUInt32 i = 0; i < params->count(); i++)
	{
		REAnimationFloatParams * p = params->at(i);
		if (p)
		{
			if (oid == p->object->objectIdentifier())
			{
				if (params->removeAt(i))
				{
					p->setStopValues(stopType);
					p->decCounter();
					p->object->release();
					p->release();
					isFoundedAndStoped = true;
				}
			}
		}
		else
		{
			return isFoundedAndStoped;
		}
	}
	return isFoundedAndStoped;
}

REBOOL REEditableAnimationInfo::stopAllAnimationForView(REObject * view, const REAnimationStopType stopType)
{
	return REEditableAnimationInfo::stopAllAnimationForObjectWithStopType(view,
																		  stopType,
																		  &_viewParams);
}

void REEditableAnimationInfo::pause() 
{
	if (_animation) 
	{
		_animation->pause(); 
	} 
}

void REEditableAnimationInfo::Continue() 
{ 
	if (_animation) 
	{ 
		_animation->start(); 
	}
}

REAnimationBase * REEditableAnimationInfo::createAnimationForTypeWithInfoAndParams(const REAnimationType type,
																				   REEditableAnimationInfo * info,
																				   REArray<REAnimationFloatParams *> * params)
{
	switch (type)
	{
		case REAnimationTypeLinear:
			return new RELinearAnimation(info, params);
			break;
		case REAnimationTypeQuadEaseIn:
			return new REQuadEaseInAnimation(info, params);
			break;
		case REAnimationTypeQuadEaseOut:
			return new REQuadEaseOutAnimation(info, params);
			break;
		case REAnimationTypeQuadEaseInOut:
			return new REQuadEaseInOutAnimation(info, params);
			break;
		case REAnimationTypeQuadEaseOutIn:
			return new REQuadEaseOutInAnimation(info, params);
			break;
		case REAnimationTypeCubicEaseIn:
			return new RECubicEaseInAnimation(info, params);
			break;
		case REAnimationTypeCubicEaseOut:
			return new RECubicEaseOutAnimation(info, params);
			break;
		case REAnimationTypeCubicEaseInOut:
			return new RECubicEaseInOutAnimation(info, params);
			break;
		case REAnimationTypeCubicEaseOutIn:
			return new RECubicEaseOutInAnimation(info, params);
			break;
		case REAnimationTypeQuarticEaseIn:
			return new REQuarticEaseInAnimation(info, params);
			break;
		case REAnimationTypeQuarticEaseOut:
			return new REQuarticEaseOutAnimation(info, params);
			break;
		case REAnimationTypeQuarticEaseInOut:
			return new REQuarticEaseInOutAnimation(info, params);
			break;
		case REAnimationTypeQuarticEaseOutIn:
			return new REQuarticEaseOutInAnimation(info, params);
			break;
		case REAnimationTypeQuinticEaseIn:
			return new REQuinticEaseInAnimation(info, params);
			break;
		case REAnimationTypeQuinticEaseOut:
			return new REQuinticEaseOutAnimation(info, params);
			break;
		case REAnimationTypeQuinticEaseInOut:
			return new REQuinticEaseInOutAnimation(info, params);
			break;
		case REAnimationTypeQuinticEaseOutIn:
			return new REQuinticEaseOutInAnimation(info, params);
			break;
		case REAnimationTypeSinusoidalEaseIn:
			return new RESinusoidalEaseInAnimation(info, params);
			break;
		case REAnimationTypeSinusoidalEaseOut:
			return new RESinusoidalEaseOutAnimation(info, params);
			break;
		case REAnimationTypeSinusoidalEaseInOut:
			return new RESinusoidalEaseInOutAnimation(info, params);
			break;
		case REAnimationTypeSinusoidalEaseOutIn:
			return new RESinusoidalEaseOutInAnimation(info, params);
			break;
		case REAnimationTypeExponentialEaseIn:
			return new REExponentialEaseInAnimation(info, params);
			break;
		case REAnimationTypeExponentialEaseOut:
			return new REExponentialEaseOutAnimation(info, params);
			break;
		case REAnimationTypeExponentialEaseInOut:
			return new REExponentialEaseInOutAnimation(info, params);
			break;
		case REAnimationTypeExponentialEaseOutIn:
			return new REExponentialEaseOutInAnimation(info, params);
			break;
		case REAnimationTypeCircularEaseIn:
			return new RECircularEaseInAnimation(info, params);
			break;
		case REAnimationTypeCircularEaseOut:
			return new RECircularEaseOutAnimation(info, params);
			break;
		case REAnimationTypeCircularEaseInOut:
			return new RECircularEaseInOutAnimation(info, params);
			break;
		case REAnimationTypeCircularEaseOutIn:
			return new RECircularEaseOutInAnimation(info, params);
			break;
		case REAnimationTypeElasticEaseIn:
			return new REElasticEaseInAnimation(info, params);
			break;
		case REAnimationTypeElasticEaseOut:
			return new REElasticEaseOutAnimation(info, params);
			break;
		case REAnimationTypeElasticEaseInOut:
			return new REElasticEaseInOutAnimation(info, params);
			break;
		case REAnimationTypeElasticEaseOutIn:
			return new REElasticEaseOutInAnimation(info, params);
			break;
		case REAnimationTypeBackEaseIn:
			return new REBackEaseInAnimation(info, params);
			break;
		case REAnimationTypeBackEaseOut:
			return new REBackEaseOutAnimation(info, params);
			break;
		case REAnimationTypeBackEaseInOut:
			return new REBackEaseInOutAnimation(info, params);
			break;
		case REAnimationTypeBackEaseOutIn:
			return new REBackEaseOutInAnimation(info, params);
			break;
		case REAnimationTypeBounceEaseIn:
			return new REBounceEaseInAnimation(info, params);
			break;
		case REAnimationTypeBounceEaseOut:
			return new REBounceEaseOutAnimation(info, params);
			break;
		case REAnimationTypeBounceEaseInOut:
			return new REBounceEaseInOutAnimation(info, params);
			break;
		case REAnimationTypeBounceEaseOutIn:
			return new REBounceEaseOutInAnimation(info, params);
			break;
		default:
			break;
	}
	return NULL;
}

REBOOL REEditableAnimationInfo::executeAnimation()
{
	if (_viewParams.count())
	{
		REAnimationBase * animation = REEditableAnimationInfo::createAnimationForTypeWithInfoAndParams(this->getAnimationType(),
																									   this,
																									   &_viewParams);
		if (animation) 
		{
			_animation = animation;
			animation->start();
			return true;
		}
	}
	return false;
}

REBOOL REEditableAnimationInfo::addFloatParam(REObject * view, 
											  REUInt16 * animationsCounter,
											  REFloat32 * param, 
											  const REFloat32 startValue, 
											  const REFloat32 endValue)
{
	REBOOL isNewViewParam = false;
	REAnimationFloatParams * p = REEditableAnimationInfo::findParamForObjectInParams(view, &_viewParams);
	if (p == NULL) 
	{
		p = new REAnimationFloatParams();
		if (p == NULL) { return false; }
		isNewViewParam = true;
	}
	
	REAnimationFloatParams::ParamStruct s;
	s.paramPtr = param;
	s.start = startValue;
	s.end = endValue;
	s.length = endValue - startValue;
	
	if (p->params.add(s)) 
	{
		if (isNewViewParam)
		{
			if (_viewParams.add(p)) 
			{
				p->object = view;
				p->objectAnimationsCounter = animationsCounter;
                static const REUInt32 respondentClassID = REObject::generateClassIdentifierFromClassName("IREAnimationRespondent");
				if (view->isImplementsClass(respondentClassID))
				{
					p->respondent = (IREAnimationRespondent *)view;
				}
				else
				{
					p->respondent = NULL;
				}
				p->incCounter();
				view->retain();
			}
			else
			{
				p->params.removeLast();
			}
		}
		return true;
	}
	else if (isNewViewParam)
	{
		delete p;
	}
	return false;
}

const REUInt32 REEditableAnimationInfo::getObjectsCount() const
{
	return _viewParams.count();
}

void REEditableAnimationInfo::getObjects(REArray<REObject *> * views) const
{
	if (views && _viewParams.count()) 
	{
		views->clear();
		views->setCapacity(_viewParams.count() + 1);
		for (REUInt32 i = 0; i < _viewParams.count(); i++) 
		{
			REAnimationFloatParams * p = _viewParams.at(i);
			if (p) 
			{
				views->add(p->object);
			}
		}
	}
}

REBOOL REEditableAnimationInfo::isEmpty() const
{
	return _viewParams.isEmpty();
}

void REEditableAnimationInfo::decrementCounterAndReleaseAllViews()
{
	for (REUInt32 i = 0; i < _viewParams.count(); i++) 
	{
		REAnimationFloatParams * p = _viewParams.at(i);
		if (p) 
		{
			p->object->release();
			p->decCounter();
		}
	}
}

void REEditableAnimationInfo::setStartMethod(REClassMethod * m) 
{
	if (_startMethod) { delete _startMethod; }
	_startMethod = m;
}

void REEditableAnimationInfo::setStopMethod(REClassMethod * m) 
{
	if (_stopMethod) { delete _stopMethod; }
	_stopMethod = m;
}

REEditableAnimationInfo::REEditableAnimationInfo(void * customDataOrNull) : REAnimationInfo(),
	_animation(NULL)
{
	_customData = customDataOrNull;
}

void REEditableAnimationInfo::onReleased()
{
	for (REUInt32 i = 0; i < _viewParams.count(); i++) 
	{
		REAnimationFloatParams * p = _viewParams.at(i);
		if (p) 
		{
			p->release();
		}
	}
	_viewParams.clear();
	
	RE_SAFE_DELETE(_animation);
	
	REAnimationInfo::onReleased();
}

REEditableAnimationInfo::~REEditableAnimationInfo() 
{
	
}

REEditableAnimationInfo * REEditableAnimationInfo::createWithCustomData(void * customDataOrNull)
{
	REEditableAnimationInfo * newInfo = new REEditableAnimationInfo(customDataOrNull);
	return newInfo;
}



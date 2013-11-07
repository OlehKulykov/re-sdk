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


#include "../../include/regui/private/REAnimationBase.h"
#include "../../include/regui/private/REAnimationController.h"
#include "../../include/recore/RETime.h"
#include "../../include/recore/REInterpolation.h"

/* REMainLoopUpdatable */
/*
void REAnimationBase::Update(const RETimeInterval currentTime)
{
	if ( ( currentTime >= _startTime ) && ( currentTime < _endTime ) )
	{
		if (_isNeedCallStartMethod) 
		{
			_info->GetStartMethod()->invokeWithObject(_info);
			_isNeedCallStartMethod = false;
		}
		
		const REFloat32 progress = (REFloat32)((currentTime - _startTime) / _workingTime);
		for (REUInt32 i = 0; i < _params->Count(); i++) 
		{
			REAnimationFloatParams * p = (*_params)[i];
			for (REUInt32 j = 0; j < p->params.Count(); j++) 
			{
				REAnimationFloatParams::ParamStruct * s = &(p->params[j]);
				(*s->paramPtr) = (s->start + (s->length * progress));
			}
		}
	}
	else if ( currentTime >= _endTime )
	{
		this->TimeIsOver();
	}
}
*/
void REAnimationBase::toStartValues()
{
	for (REUInt32 i = 0; i < _params->count(); i++) 
	{
		((*_params)[i])->toStartValues();
	}
}

void REAnimationBase::toEndValues()
{
	for (REUInt32 i = 0; i < _params->count(); i++) 
	{
		((*_params)[i])->toEndValues();
	}
}

void REAnimationBase::finish()
{
	this->toEndValues();
	
	this->fromMainLoop();
	
	for (REUInt32 i = 0; i < _params->count(); i++) 
	{ 
		REAnimationFloatParams * p = (*_params)[i]; 
		if (p->respondent) { p->respondent->animationDidFinished(this->getAnimationIdentifier()); } 
	} 
	
	REAnimationController::defaultController()->animationFinishedSuccessfully(_info);
}

void REAnimationBase::loopTypeFromStartToEnd()
{
	if (_info->getLoopsCount() != RENotFound)
	{
		//not looped
		if (_info->getLoopsCount() == _loopsDone) 
		{
			this->finish();
			return;
		}
		_loopsDone++;
	}
	
	this->toStartValues();
	
	_startTime = RETime::time();
	_endTime = _startTime + _workingTime;
}

void REAnimationBase::loopTypeFromEndToStart()
{
	if (_info->getLoopsCount() != RENotFound)
	{
		//not looped
		if (_info->getLoopsCount() == _loopsDone) 
		{
			this->finish();
			return;
		}
		_loopsDone++;
	}
	
	for (REUInt32 i = 0; i < _params->count(); i++) 
	{
		REAnimationFloatParams * p = (*_params)[i];
		for (REUInt32 j = 0; j < p->params.count(); j++) 
		{
			REAnimationFloatParams::ParamStruct * s = &(p->params[j]);
			const REFloat32 temp = s->end;
			s->end = s->start;
			s->start = temp;
			s->length *= -1.0f;
			(*s->paramPtr) = temp;
		}
	}
	_startTime = RETime::time();
	_endTime = _startTime + _workingTime;
}

void REAnimationBase::timeIsOver()
{
	_progress = 1.0f;
	switch (_info->getLoopType()) 
	{
		case REAnimationLoopTypeNone:
			this->finish();
			break;
		case REAnimationLoopTypeFromStartToEnd:
			this->loopTypeFromStartToEnd();
			break;
		case REAnimationLoopTypeFromEndToStart:
			this->loopTypeFromEndToStart();
			break;
		default:
			break;
	}
}

void REAnimationBase::start()
{
	if (_isPaused)
	{
		const RETimeInterval pauseTime = RETime::time() - _endTime;
		_startTime += pauseTime;
		_endTime = _startTime + _workingTime;
		_isPaused = false;
	}
	else
	{
		_startTime = RETime::time() + _info->getStartDelay();
		_endTime = _startTime + _workingTime;
	}
	
	this->toMainLoop();
}

void REAnimationBase::pause()
{
	if ( !_isPaused )
	{
		this->fromMainLoop();
		
		_endTime = RETime::time();
		_isPaused = true;
	}
}

void REAnimationBase::stop(const REAnimationStopType stopType)
{
	switch (stopType) 
	{
		case REAnimationStopTypeImmediatelyToStart:
			this->toStartValues();
			break;
		case REAnimationStopTypeImmediatelyToEnd:
			this->toEndValues();
			break;
		default:
			break;
	}
	
	this->fromMainLoop();
	REAnimationController::defaultController()->animationFinishedSuccessfully(_info);
}

REAnimationBase::REAnimationBase(REAnimationInfo * info, REArray<REAnimationFloatParams *> * params) : 
	_info(info),
	_params(params),
	_startTime(0.0),
	_endTime(0.0),
	_workingTime(0.0),
	_progress(0.0f),
	_loopsDone(0),
	_isPaused(false),
	_isNeedCallStartMethod(false)
{
	_workingTime = _info->getTime();
	if (_info->getStartMethod()) 
	{
		_isNeedCallStartMethod = true;
	}
	for (REUInt32 i = 0; i < _params->count(); i++) 
	{ 
		REAnimationFloatParams * p = (*_params)[i]; 
		if (p->respondent) 
		{
			_isNeedCallStartMethod = true;
			break;
		} 
	} 
}

REAnimationBase::~REAnimationBase()
{
	/// !!! removed in typed animation destructor
	//this->RemoveFromMainLoop();
}





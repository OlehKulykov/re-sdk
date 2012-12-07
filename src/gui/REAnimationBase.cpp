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
			_info->GetStartMethod()->InvokeWithObject(_info);
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
void REAnimationBase::ToStartValues()
{
	for (REUInt32 i = 0; i < _params->Count(); i++) 
	{
		((*_params)[i])->ToStartValues();
	}
}

void REAnimationBase::ToEndValues()
{
	for (REUInt32 i = 0; i < _params->Count(); i++) 
	{
		((*_params)[i])->ToEndValues();
	}
}

void REAnimationBase::Finish()
{
	this->ToEndValues();
	
	this->FromMainLoop();
	
	for (REUInt32 i = 0; i < _params->Count(); i++) 
	{ 
		REAnimationFloatParams * p = (*_params)[i]; 
		if (p->respondent) { p->respondent->REAnimationRespondentAnimationDidFinished(this->GetAnimationIdentifier()); } 
	} 
	
	REAnimationController::GetDefaultController()->AnimationFinishedSuccessfully(_info);
}

void REAnimationBase::LoopTypeFromStartToEnd()
{
	if (_info->GetLoopsCount() != RENotFound)
	{
		//not looped
		if (_info->GetLoopsCount() == _loopsDone) 
		{
			this->Finish();
			return;
		}
		_loopsDone++;
	}
	
	this->ToStartValues();
	
	_startTime = RETime::Time();
	_endTime = _startTime + _workingTime;
}

void REAnimationBase::LoopTypeFromEndToStart()
{
	if (_info->GetLoopsCount() != RENotFound)
	{
		//not looped
		if (_info->GetLoopsCount() == _loopsDone) 
		{
			this->Finish();
			return;
		}
		_loopsDone++;
	}
	
	for (REUInt32 i = 0; i < _params->Count(); i++) 
	{
		REAnimationFloatParams * p = (*_params)[i];
		for (REUInt32 j = 0; j < p->params.Count(); j++) 
		{
			REAnimationFloatParams::ParamStruct * s = &(p->params[j]);
			const REFloat32 temp = s->end;
			s->end = s->start;
			s->start = temp;
			s->length *= -1.0f;
			(*s->paramPtr) = temp;
		}
	}
	_startTime = RETime::Time();
	_endTime = _startTime + _workingTime;
}

void REAnimationBase::TimeIsOver()
{
	_progress = 1.0f;
	switch (_info->GetLoopType()) 
	{
		case REAnimationLoopTypeNone:
			this->Finish();
			break;
		case REAnimationLoopTypeFromStartToEnd:
			this->LoopTypeFromStartToEnd();
			break;
		case REAnimationLoopTypeFromEndToStart:
			this->LoopTypeFromEndToStart();
			break;
		default:
			break;
	}
}

void REAnimationBase::Start()
{
	if (_isPaused)
	{
		const RETimeInterval pauseTime = RETime::Time() - _endTime;
		_startTime += pauseTime;
		_endTime = _startTime + _workingTime;
		_isPaused = false;
	}
	else
	{
		_startTime = RETime::Time() + _info->GetStartDelay();
		_endTime = _startTime + _workingTime;
	}
	
	this->ToMainLoop();
}

void REAnimationBase::Pause()
{
	if ( !_isPaused )
	{
		this->FromMainLoop();
		
		_endTime = RETime::Time();
		_isPaused = true;
	}
}

void REAnimationBase::Stop(const REAnimationStopType stopType)
{
	switch (stopType) 
	{
		case REAnimationStopTypeImmediatelyToStart:
			this->ToStartValues();
			break;
		case REAnimationStopTypeImmediatelyToEnd:
			this->ToEndValues();
			break;
		default:
			break;
	}
	
	this->FromMainLoop();
	REAnimationController::GetDefaultController()->AnimationFinishedSuccessfully(_info);
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
	_workingTime = _info->GetTime();
	if (_info->GetStartMethod()) 
	{
		_isNeedCallStartMethod = true;
	}
	for (REUInt32 i = 0; i < _params->Count(); i++) 
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





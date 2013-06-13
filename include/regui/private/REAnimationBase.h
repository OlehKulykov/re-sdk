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


#ifndef __REANIMATIONBASE_H__
#define __REANIMATIONBASE_H__

#include "../../recore/RECommonHeader.h"
#include "../../recore/REArray.h"
#include "../../recore/REMainLoopUpdatable.h"
#include "../../recore/REObject.h"
#include "../IREAnimation.h"
#include "../REAnimationInfo.h"
#include "REAnimationFloatParams.h"

class __RE_PUBLIC_CLASS_API__ REAnimationBase : public IREAnimation
{
protected:
	REAnimationInfo * _info;
	REArray<REAnimationFloatParams *> * _params;
	RETimeInterval _startTime;
	RETimeInterval _endTime;
	RETimeInterval _workingTime;
	REFloat32 _progress;
	REUInt32 _loopsDone;
	REBOOL _isPaused;
	REBOOL _isNeedCallStartMethod;
	
	void TimeIsOver();
	
	void ToStartValues();
	void ToEndValues();
	void Finish();
	void LoopTypeFromStartToEnd();
 	void LoopTypeFromEndToStart();
	
	virtual void ToMainLoop() = 0;
	virtual void FromMainLoop() = 0;
public:	
	/* IREViewAnimation */
	virtual const REFloat32 GetProgress() const { return _progress; }
	virtual void Start();
	virtual void Pause();
	virtual void Stop(const REAnimationStopType stopType);
	
	virtual const REUIdentifier GetAnimationIdentifier() const = 0;
	
	REAnimationBase(REAnimationInfo * info, REArray<REAnimationFloatParams *> * params);
	virtual ~REAnimationBase();
};


#endif /* __REANIMATIONBASE_H__ */



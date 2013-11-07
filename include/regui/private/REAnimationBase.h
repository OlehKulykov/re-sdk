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
	
	void timeIsOver();
	
	void toStartValues();
	void toEndValues();
	void finish();
	void loopTypeFromStartToEnd();
 	void loopTypeFromEndToStart();
	
	virtual void toMainLoop() = 0;
	virtual void fromMainLoop() = 0;
public:	
	/* IREViewAnimation */
	virtual const REFloat32 getProgress() const { return _progress; }
	virtual void start();
	virtual void pause();
	virtual void stop(const REAnimationStopType stopType);
	
	virtual const REUIdentifier getAnimationIdentifier() const = 0;
	
	REAnimationBase(REAnimationInfo * info, REArray<REAnimationFloatParams *> * params);
	virtual ~REAnimationBase();
};


#endif /* __REANIMATIONBASE_H__ */



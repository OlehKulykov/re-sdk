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


#ifndef __RETIMER_H__
#define __RETIMER_H__

#include "RECommonHeader.h"
#include "REObject.h"
#include "REMainLoopUpdatable.h"
#include "REClassMethod.h"

/// Class of timer.
class __RE_PUBLIC_CLASS_API__ RETimer : public REObject, public REMainLoopUpdatable
{
private:
	REClassMethod * _targetMethod;
	RETimeInterval _startTime;
	RETimeInterval _time;
	REInt32 _loopsCount;
	REInt32 _doneLoopsCount;
	REBOOL _isActive;
	
public:
	/* REObject */
	virtual const REUInt32 getClassIdentifier() const;
	static const REUInt32 classIdentifier();
	virtual REBOOL isImplementsClass(const REUInt32 classIdentifier) const;
	
	/// Sets method wich will invoke in timer tick.
	void setTrigerTargetMethod(REClassMethod * targetMethod);
	
	/// Sets time interval.
	void setTime(const RETimeInterval time);
	
	/// Returns time interval.
	const RETimeInterval time() const;
	
	/// Sets loops count.
	/// "0" - No loop(default);  "> 0" - loopsCount; "< 0" - looped.
	void setLoopsCount(const REInt32 loopsCount);
	
	/// Returns count of loops.
	const REInt32 loopsCount() const;
	
	/// Check is timer active.
	REBOOL isActive() const;
	
	/// Starts timer.
	void start();
	
	/// Stops timer.
	void stop();
	
	/* REMainLoopUpdatable */
	virtual void Update(const RETimeInterval currentTime);
	virtual const REUIdentifier GetMainLoopUpdatableIdentifier() const;

	/// Counstructs timer.
	RETimer();
	
	/// Destructor.
	virtual ~RETimer();
};

 
#endif /* __RETIMER_H__ */


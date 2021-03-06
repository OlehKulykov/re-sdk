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


#ifndef __RETIME_H__
#define __RETIME_H__

#include "RECommonHeader.h"

/// Class using for getting time.
/// No matter how many instances of the class will still be used static methods.
class __RE_PUBLIC_CLASS_API__ RETime
{
public:
	/// Returns current time.
	/// Result value will be same as RETime::time();
	const RETimeInterval getTime() const;
	
	/// Pausing time functionality.
	/// Returns true if paused otherwise returns false on error of if already paused.
	/// Result value will be same as RETime::pause();
	REBOOL pauseTime();
	
	/// Resuming time functionality.
	/// Returns true if resumed otherwise returns false on error of if already resumed or not paused.
	/// Result value will be same as RETime::resume();
	REBOOL resumeTime();
	
	/// Checks is time functionality paused.
	/// Result value will be same as RETime::isPaused();
	REBOOL isTimePaused() const;
	
	/// Returns current time.
	static const RETimeInterval time();
	
	/// Pausing time functionality.
	/// Returns true if paused otherwise returns false on error of if already paused.
	static REBOOL pause();
	
	/// Resuming time functionality.
	/// Returns true if resumed otherwise returns false on error of if already resumed or not paused.
	static REBOOL resume();
	
	/// Checks is time functionality paused.
	static REBOOL isPaused();
	
	/// Converts seconds to milliseconds.
	static const REUInt64 convertSecondsToMilliseconds(const RETimeInterval seconds);
	
	/// Converts seconds to microseconds.
	static const REUInt64 convertSecondsToMicroseconds(const RETimeInterval seconds);
};


#endif /* __RETIME_H__ */


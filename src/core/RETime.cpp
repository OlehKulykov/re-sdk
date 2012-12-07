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


#include "../../include/recore/RETime.h"

#if (defined(__RE_OS_MACOSX__) || defined(__RE_OS_IPHONE__)) 
#include <mach/mach_time.h>
#elif defined(__RE_OS_WINDOWS__) 

#elif defined(__RE_OS_LINUX__) || defined(__RE_OS_ANDROID__)
#include <sys/time.h>
#include <time.h>
#endif

class RETimePrivate 
{
private:
#if (defined(__RE_OS_MACOSX__) || defined(__RE_OS_IPHONE__)) 
	uint64_t _absoluteAppStartTime;
	uint64_t _absoluteLastCallTime;
	double _nanoSecond;
#elif defined(__RE_OS_WINDOWS__) 
	INT64 _performanceCounterFrequency;
	double _performanceCounterTimeScale;
#elif (defined(__RE_OS_LINUX__) || defined(__RE_OS_ANDROID__))
	//long _startSeconds;
#endif	
	RETimeInterval _lastPauseTime;
	RETimeInterval _unusedTime;
	REUInt32 _isPaused;
	RETimeInterval Get();
	void UpdatePauseTime();
public:
	static RETimePrivate time;
	RETimePrivate();
	~RETimePrivate();
	RETimeInterval Time();
	REBOOL Pause();
	REBOOL Resume();
	REBOOL IsPaused() const;
};

RETimePrivate RETimePrivate::time;

RETimeInterval RETimePrivate::Get() 
{
#if (defined(__RE_OS_MACOSX__) || defined(__RE_OS_IPHONE__)) 
	const double seconds = (_nanoSecond * (mach_absolute_time() - _absoluteAppStartTime));
	return (RETimeInterval)seconds;
#elif defined(__RE_OS_WINDOWS__) 
	INT64 counterTime = 0;
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&counterTime));
	const double returnTime = ((double)counterTime) * _performanceCounterTimeScale;
	return (RETimeInterval)returnTime;
#elif defined(__RE_OS_LINUX__) || defined(__RE_OS_ANDROID__)
	struct timespec now;
	if (clock_gettime(CLOCK_MONOTONIC, &now) == 0)
	{
		return (RETimeInterval)(((double)now.tv_nsec * 1e-9) + now.tv_sec);
	}
	else
	{
		return (RETimeInterval)((double)clock() / (double)CLOCKS_PER_SEC);
	}
#else
	return (RETimeInterval)((double)clock() / (double)CLOCKS_PER_SEC);
#endif
}

void RETimePrivate::UpdatePauseTime()
{
	const RETimeInterval curTime = this->Get();
	_unusedTime += (curTime - _lastPauseTime);
	_lastPauseTime = curTime;
}

RETimeInterval RETimePrivate::Time()
{
	if (_isPaused) 
	{
		this->UpdatePauseTime();
	}
	
	return (this->Get() - _unusedTime);
}

REBOOL RETimePrivate::Pause()
{
	if (_isPaused == 0)
	{
		_isPaused = 1;
		_lastPauseTime = this->Get();
		return true;
	}
	return false;
}

REBOOL RETimePrivate::Resume()
{
	if (_isPaused) 
	{
		this->UpdatePauseTime();
		_isPaused = 0;
		return true;
	}
	return false;
}

REBOOL RETimePrivate::IsPaused() const
{
	return ((_isPaused == 1) ? true : false);
}

RETimePrivate::RETimePrivate() :
#if (defined(__RE_OS_MACOSX__) || defined(__RE_OS_IPHONE__)) 
	_absoluteAppStartTime(0),
	_absoluteLastCallTime(0),
	_nanoSecond((double)0),
#elif defined(__RE_OS_WINDOWS__) 
	_performanceCounterFrequency(0),
	_performanceCounterTimeScale((double)0),
#elif (defined(__RE_OS_LINUX__) || defined(__RE_OS_ANDROID__))
	//_startSeconds(0),
#endif	
	_lastPauseTime(0.0),
	_unusedTime(0.0),
	_isPaused(0)
{
#if (defined(__RE_OS_MACOSX__) || defined(__RE_OS_IPHONE__)) 
	_absoluteAppStartTime = mach_absolute_time();
	_absoluteLastCallTime = _absoluteAppStartTime;
	mach_timebase_info_data_t info;
    mach_timebase_info(&info);
    _nanoSecond = (double)(((long double)info.numer) * 1e-9)  / ((long double)info.denom);
#elif defined(__RE_OS_WINDOWS__) 
	QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&_performanceCounterFrequency));
	_performanceCounterTimeScale = ((long double)1.0) / ((long double)_performanceCounterFrequency);
#elif defined(__RE_OS_LINUX__) || defined(__RE_OS_ANDROID__)
	/*
	struct timeval now;
	if (gettimeofday(&now, NULL) == 0) 
	{
		_startSeconds = now.tv_sec;
	}
	*/
#endif
	
	_unusedTime = this->Get();
}

RETimePrivate::~RETimePrivate()
{
	
}

const REUInt64 RETime::ConvertSecondsToMilliseconds(const RETimeInterval seconds)
{
	double fullSeconds = 0.0;
	const double fractSecond = modf(seconds, &fullSeconds);
	const REUInt64 fullIntMilisec = (REUInt64)fullSeconds * 1000;
	const REUInt64 fractIntMilisec = (REUInt64)fractSecond * 1000;
	return (fullIntMilisec + fractIntMilisec);
}

const REUInt64 RETime::ConvertSecondsToMicroseconds(const RETimeInterval seconds)
{
	double fullSeconds = 0.0;
	const double fractSecond = modf(seconds, &fullSeconds);
	const REUInt64 fullIntMicrosec = (REUInt64)fullSeconds * 1000000;
	const REUInt64 fractIntMicrosec = (REUInt64)fractSecond * 1000000;
	return (fullIntMicrosec + fractIntMicrosec);
}

REBOOL RETime::PauseTime()
{
	return RETime::Pause();
}

REBOOL RETime::ResumeTime()
{
	return RETime::Resume();
}

REBOOL RETime::Pause()
{
	return RETimePrivate::time.Pause();
}

REBOOL RETime::Resume()
{
	return RETimePrivate::time.Resume();
}

REBOOL RETime::IsTimePaused() const
{
	return RETime::IsPaused();
}

REBOOL RETime::IsPaused()
{
	return RETimePrivate::time.IsPaused();
}

const RETimeInterval RETime::GetTime() const
{
	return RETime::Time();
}

const RETimeInterval RETime::Time()
{
	return RETimePrivate::time.Time();
}




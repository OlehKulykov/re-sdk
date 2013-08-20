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


#include "../../include/recore/REThread.h"
#include "../../include/recore/private/REDetachableThreadPrivate.h"
#include "../../include/recore/private/REMainThreadClassMethodPrivate.h"
#include "../../include/recore/private/REMainThreadClassMethodWaitedPrivate.h"
#include "../../include/recore/private/REAutoReleasePoolPrivate.h"
#include "../../include/recore/REMath.h"


#if defined(HAVE_PTHREAD_H)
#include <pthread.h>
#endif

#if defined(HAVE_SYS_TIME_H)
#include <sys/time.h>
#endif

#if defined(HAVE_UNISTD_H)
#include <unistd.h>
#endif

#ifndef __RE_TRY_USE_PTHREADS__
#ifdef __RE_OS_WINDOWS__
#ifndef __RE_USING_WINDOWS_THREADS__
#define __RE_USING_WINDOWS_THREADS__
#endif
#include <Windows.h>
#endif
#endif

class REThreadPrivate 
{
public:
#if defined(HAVE_PTHREAD_H)   
	static pthread_t mainPThread;
#elif defined(__RE_USING_WINDOWS_THREADS__)
	static DWORD mainThreadID;
#endif	
	static REBOOL isMultiThreaded;
};

#if defined(HAVE_PTHREAD_H)  
pthread_t REThreadPrivate::mainPThread = 0;
#elif defined(__RE_USING_WINDOWS_THREADS__)
DWORD REThreadPrivate::mainThreadID = (DWORD)0;
#endif	
REBOOL REThreadPrivate::isMultiThreaded = false;


const REUInt32 REThread::getClassIdentifier() const
{
	return REThread::classIdentifier();
}

const REUInt32 REThread::classIdentifier()
{
	static const REUInt32 classIdentif = REObject::generateClassIdentifierFromClassName("REThread");
	return classIdentif;
}

REBOOL REThread::isImplementsClass(const REUInt32 classIdentifier) const
{
	return ((REThread::classIdentifier() == classIdentifier) ||
			REObject::isImplementsClass(classIdentifier));
}

void REThread::_threadBody()
{
	this->addState(REThreadStateWillStartThreadBody);

	this->threadBody();
	
	this->removeState(REThreadStateWillStartThreadBody);
	this->addState(REThreadStateDidEndThreadBody);
}

void REThread::addState(const REThreadState & state)
{
	const REUInt16 st = _reThreadStates | (REUInt16)state;
	_reThreadStates = st;
}

void REThread::removeState(const REThreadState & state)
{
	const REUInt16 st = _reThreadStates ^ (REUInt16)state;
	_reThreadStates = st;
}

REBOOL REThread::isHasState(const REThreadState & state) const
{
	const REUInt16 st = _reThreadStates;
	return (st & ((REUInt16)state)) ? true : false;
}

REBOOL REThread::start()
{
#if defined(HAVE_PTHREAD_H)  
	
	if (_reThreadThread) 
	{
		return false;
	}
	
	pthread_t th = 0;
	struct rlimit limit = { 0 };
	size_t stack_size = 0;
	int rc = getrlimit(RLIMIT_STACK, &limit);
	if (rc == 0) 
	{
		if (limit.rlim_cur != 0) 
		{
			stack_size = (size_t)limit.rlim_cur;
		}
	}
	
	pthread_attr_t thread_attr;
	pthread_attr_init(&thread_attr);
	pthread_attr_setscope(&thread_attr, PTHREAD_SCOPE_SYSTEM);
	pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_JOINABLE);
	if (stack_size > 0) 
	{
		pthread_attr_setstacksize(&thread_attr, stack_size);
	}
	
	REBOOL isCreated = false;
	if ( pthread_create(&th, &thread_attr, REThread::threadFunction, REPtrCast<void, void>(this)) == 0 ) 
	{
		isCreated = true;
		REThreadPrivate::isMultiThreaded = true;
        _reThreadThread = th;
	}
	
	pthread_attr_destroy(&thread_attr);
	
	return isCreated;
	
#elif defined(__RE_USING_WINDOWS_THREADS__) 
	
	HANDLE hThread = CreateThread(NULL, 0, REThread::threadProc, (LPVOID)this, 0, NULL);
	if (hThread)
	{
		REThreadPrivate::isMultiThreaded = true;
		_reThreadThread = hThread;
		this->addState(REThreadStateCreated);
		return true;
	}
	return false;
	
#else

	this->_threadBody();
	if ( this->isAutoReleaseWhenDone() ) 
	{
		this->release();
	}
	return true;
	
#endif	
}

REBOOL REThread::stop()
{
#if defined(HAVE_PTHREAD_H)  
	if (_reThreadThread) 
	{
		REBOOL res = false;
		pthread_t th = _reThreadThread;
		if (pthread_cancel(th) == 0) 
		{
			void * r = NULL;
			if (pthread_join(th, &r) == 0) 
			{
				res = (r == PTHREAD_CANCELED);
			}
		}

		_reThreadThread = NULL;
		_reThreadStates = 0;
		return res;
	}
	
#elif defined (__RE_USING_WINDOWS_THREADS__) 
	
	if (_reThreadThread) 
	{
		bool isAlive = false;
		DWORD dwExitCode = 0;
		if (GetExitCodeThread(_reThreadThread, &dwExitCode))
		{
			// if return code is STILL_ACTIVE,
			// then thread is live.
			isAlive = (dwExitCode == STILL_ACTIVE);
		}
		if (isAlive)
		{
			TerminateThread(_reThreadThread, 0);
		}
		CloseHandle(_reThreadThread);
		_reThreadThread = (HANDLE)0;
		_reThreadStates = 0;
		return true;
	}
	
#endif
	
	return false;
}

REBOOL REThread::isWorking() const 
{
	if (this->isHasState(REThreadStateCreated) && this->isHasState(REThreadStateDidEnterThreadFunc))
	{
		return true;
	}
	return false; 
}

REFloat32 REThread::priority() const
{
#if defined(HAVE_PTHREAD_H)  
	if ( _reThreadThread ) 
	{
		const int maxPrior = sched_get_priority_max(SCHED_RR);
		if (maxPrior > 0) 
		{
			struct sched_param p = { 0 };
			
			int policy = SCHED_RR;
			pthread_t thisThread = _reThreadThread;
			if ( pthread_getschedparam(thisThread, &policy, &p) == 0 )
			{
				return ((REFloat32)p.sched_priority / (REFloat32)maxPrior);
			}
		}
	}
#elif defined(__RE_USING_WINDOWS_THREADS__) 
	if (_reThreadThread) 
	{
		const int prior = GetThreadPriority(_reThreadThread);
		const int minPriority = MIN(THREAD_PRIORITY_IDLE, THREAD_PRIORITY_TIME_CRITICAL);
		const int range = MAX(THREAD_PRIORITY_IDLE, THREAD_PRIORITY_TIME_CRITICAL) - minPriority;
		return ( (REFloat32)(prior - minPriority) / (REFloat32)range );
	}
#endif	
	
	return 0.0f;
}

REBOOL REThread::setPriority(const REFloat32 newPriority)
{
	if ( (newPriority <= 0.0f) || (newPriority > 1.0f) ) 
	{
		return false;
	}
#if defined(HAVE_PTHREAD_H)  
	if (_reThreadThread) 
	{
		const int maxPrior = sched_get_priority_max(SCHED_RR);
		if ( maxPrior > 0 ) 
		{
			struct sched_param p = { 0 };
			p.sched_priority = (int)(newPriority * maxPrior);
			pthread_t thisThread = _reThreadThread;
			return ( pthread_setschedparam(thisThread, SCHED_RR, &p) == 0 );
		}
	}
#elif defined(__RE_USING_WINDOWS_THREADS__) 
	if (_reThreadThread) 
	{
		const int minPriority = MIN(THREAD_PRIORITY_IDLE, THREAD_PRIORITY_TIME_CRITICAL);
		const int range = MAX(THREAD_PRIORITY_IDLE, THREAD_PRIORITY_TIME_CRITICAL) - minPriority;
		const int prior = (int)(minPriority + (newPriority * (REFloat32)range));
		if (SetThreadPriority(_reThreadThread, prior))
		{
			return true;
		}
	}
#endif	
	
	return false;
}

REThread::REThread() : 
#if defined(HAVE_PTHREAD_H)  
_reThreadThread((pthread_t)0),
#elif defined(__RE_USING_WINDOWS_THREADS__) 
_reThreadThread((HANDLE)0),
#endif	
_reThreadStates(0),
_reThreadIsAutoreleaseWhenDone(false)
{
#if defined(HAVE_PTHREAD_H)  
	if (REThreadPrivate::mainPThread == (pthread_t)0) 
	{
		REThreadPrivate::mainPThread = pthread_self();
	}
#elif defined(__RE_USING_WINDOWS_THREADS__) 
	if (REThreadPrivate::mainThreadID == 0)
	{
		REThreadPrivate::mainThreadID = GetCurrentThreadId();
	}
#endif	
}

REThread::~REThread()
{
	this->stop();
}

REBOOL REThread::isMultiThreaded()
{
	return REThreadPrivate::isMultiThreaded;
}

REUIdentifier REThread::mainThreadIdentifier()
{
	REUIdentifier thID = 0;
#if defined(HAVE_PTHREAD_H)  
	if (REThreadPrivate::mainPThread == (pthread_t)0)
	{
		REThreadPrivate::mainPThread = pthread_self();
	}
	thID = ((REUIdentifier)REThreadPrivate::mainPThread);
#elif defined(__RE_USING_WINDOWS_THREADS__)
	if (REThreadPrivate::mainThreadID == 0)
	{
		REThreadPrivate::mainThreadID = GetCurrentThreadId();
	}
	thID = ((REUIdentifier)REThreadPrivate::mainPThread);
#endif
	return thID;
}

REUIdentifier REThread::currentThreadIdentifier()
{
	REUIdentifier thID = 0;
#if defined(HAVE_PTHREAD_H)  
	thID = ((REUIdentifier)pthread_self());
#elif defined(__RE_USING_WINDOWS_THREADS__)
	thID = ((REUIdentifier)GetCurrentThreadId());
#endif
	return thID;
}

void REThread::uSleep(const REUInt32 microseconds)
{
#if defined(__RE_OS_WINDOWS__) 
	__int64 time1 = 0, time2 = 0, sysFreq = 0;

	QueryPerformanceCounter((LARGE_INTEGER *)&time1);
	QueryPerformanceFrequency((LARGE_INTEGER *)&sysFreq);
	do 
	{
		QueryPerformanceCounter((LARGE_INTEGER *)&time2);
	} 
	while((time2 - time1) < microseconds);
#endif
	
#if defined(HAVE_FUNCTION_USLEEP) 
	usleep(microseconds);
#endif
}

/// Sleps current thread for time in micro seconds.
void REThread::uSleepInSeconds(const RETimeInterval seconds)
{
	REThread::uSleep(RETime::convertSecondsToMicroseconds(seconds));
}

REBOOL REThread::isMainThread()
{
#if defined(HAVE_PTHREAD_H)  
	pthread_t curThread = pthread_self();
	if (REThreadPrivate::mainPThread) 
	{
		return (pthread_equal(curThread, REThreadPrivate::mainPThread) != 0);
	}
	else
	{
		REThreadPrivate::mainPThread = curThread;
	}
#elif defined(__RE_USING_WINDOWS_THREADS__) 
	const DWORD curTHID = GetCurrentThreadId();
	if (REThreadPrivate::mainThreadID)
	{
		return (REThreadPrivate::mainThreadID == curTHID);
	}
	else
	{
		REThreadPrivate::mainThreadID = curTHID;
	}
#endif	
	
	return true;
}

REFloat32 REThread::mainThreadPriority()
{
#if defined(HAVE_PTHREAD_H)  
	if (REThread::isMainThread()) 
	{
		const int maxPrior = sched_get_priority_max(SCHED_RR);
		if (maxPrior > 0) 
		{
			struct sched_param p = { 0 };
			int policy = SCHED_RR;
			pthread_t thisThread = REThreadPrivate::mainPThread;
			if ( pthread_getschedparam(thisThread, &policy, &p) == 0 )
			{
				return ((REFloat32)p.sched_priority / (REFloat32)maxPrior);
			}
		}
	}
#elif defined(__RE_USING_WINDOWS_THREADS__) 
	/*
	if (REThread::IsMainThread()) 
	{
		if (_reThreadMainThreadHANDLE) 
		{
			const int prior = GetThreadPriority(_reThreadMainThreadHANDLE);
			const int minPriority = MIN(THREAD_PRIORITY_IDLE, THREAD_PRIORITY_TIME_CRITICAL);
			const int range = MAX(THREAD_PRIORITY_IDLE, THREAD_PRIORITY_TIME_CRITICAL) - minPriority;
			return ( (REFloat32)(prior - minPriority) / (REFloat32)range );
		}
	}
	*/
#endif	
	
	return -1.0f;
}

REBOOL REThread::setMainThreadPriority(const REFloat32 newPriority)
{
	if ( (newPriority <= 0.0f) || (newPriority > 1.0f) ) 
	{
		return false;
	}
#if defined(HAVE_PTHREAD_H)  
	if ( REThread::isMainThread() ) 
	{
		const int maxPrior = sched_get_priority_max(SCHED_RR);
		if ( maxPrior > 0 ) 
		{
			struct sched_param p = { 0 };
			p.sched_priority = (int)(newPriority * maxPrior);
			pthread_t thisThread = REThreadPrivate::mainPThread;
			return ( pthread_setschedparam(thisThread, SCHED_RR, &p) == 0 );
		}
	}
#elif defined(__RE_USING_WINDOWS_THREADS__) 
	/*
	if ( REThread::IsMainThread() ) 
	{
		if (_reThreadMainThreadHANDLE) 
		{
			const int minPriority = MIN(THREAD_PRIORITY_IDLE, THREAD_PRIORITY_TIME_CRITICAL);
			const int range = MAX(THREAD_PRIORITY_IDLE, THREAD_PRIORITY_TIME_CRITICAL) - minPriority;
			const int prior = (int)(minPriority + (newPriority * (REFloat32)range));
			if (SetThreadPriority(_reThreadMainThreadHANDLE, prior))
			{
				return true;
			}
		}
	}
	*/
#endif
	
	return false;
}

#if defined(HAVE_PTHREAD_H)  
void * REThread::threadFunction(void * th) 
{ 
	if (th)
	{
		REThread * reThread = REPtrCast<REThread, void>(th);
		reThread->addState(REThreadStateDidEnterThreadFunc);
		reThread->_threadBody();
		if (reThread->isAutoReleaseWhenDone())
		{
			reThread->release();
		}
		reThread->removeState(REThreadStateDidEnterThreadFunc);
		reThread->addState(REThreadStateWillExitThreadFunc);
		//REAutoReleasePoolPrivate::ThreadFinished(REThread::GetCurrentThreadIdentifier());
	}
	return th;
}
#elif defined(__RE_USING_WINDOWS_THREADS__) 
DWORD REThread::threadProc(LPVOID lpParameter)
{
	if (lpParameter)
	{
		REThread * reThread = (REThread *)lpParameter;
		reThread->addState(REThreadStateDidEnterThreadFunc);
		reThread->_threadBody();
		if (reThread->isAutoReleaseWhenDone())
		{
			reThread->release();
		}
		reThread->removeState(REThreadStateDidEnterThreadFunc);
		reThread->addState(REThreadStateWillExitThreadFunc);
		//REAutoReleasePoolPrivate::ThreadFinished(REThread::GetCurrentThreadIdentifier());
	}
	return 0;
}
#endif	

void REThread::detachNewThreadWithMethod(REClassMethod * method, REObject * methodObjectOrNULL)
{
	if (method)
	{
		REDetachableThreadPrivate * newThread = new REDetachableThreadPrivate(method, methodObjectOrNULL);
		if (newThread)
		{
			newThread->setAutoReleaseWhenDone(true);
			newThread->start();
		}
	}
}

void REThread::detachNewThreadWithMethodAfterDelay(REClassMethod * method, REObject * methodObjectOrNULL, RETimeInterval delayTime)
{
	if (method) 
	{
		REDetachableThreadPrivate * newThread = new REDetachableThreadPrivate(method, methodObjectOrNULL);
		if (newThread)
		{
			newThread->setAutoReleaseWhenDone(true);
			newThread->startWithDelay(delayTime);
		}
	}
}

void REThread::performMethodOnMainThread(REClassMethod * method, REObject * methodObjectOrNULL)
{
	if ( method ) 
	{
		REBOOL isNeedDeleteInMethod = true;

		if ( REThread::isMainThread() ) 
		{
			if (methodObjectOrNULL) 
			{
				methodObjectOrNULL->retain();
			}
			method->invokeWithObject(methodObjectOrNULL);
			if (methodObjectOrNULL) 
			{
				methodObjectOrNULL->release();
			}
		}
		else 
		{
			REMainThreadClassMethodPrivate * methodWrapper = new REMainThreadClassMethodPrivate(method, methodObjectOrNULL);
			if ( methodWrapper ) 
			{
				isNeedDeleteInMethod = false;
			}
		}
		
		if ( isNeedDeleteInMethod ) 
		{
			delete method;
		}
	}
}

void REThread::performMethodOnMainThreadAndWaitUntilDone(REClassMethod * method, REObject * methodObjectOrNULL)
{
	if ( method ) 
	{	
		if ( REThread::isMainThread() ) 
		{
			if (methodObjectOrNULL) 
			{
				methodObjectOrNULL->retain();
			}
			method->invokeWithObject(methodObjectOrNULL);
			if (methodObjectOrNULL) 
			{
				methodObjectOrNULL->release();
			}
			delete method;
		}
		else 
		{
			REMainThreadClassMethodWaitedPrivate * methodWrapper = new REMainThreadClassMethodWaitedPrivate(method, methodObjectOrNULL);
			if ( methodWrapper ) 
			{
				while ( methodWrapper->isWaiting() ) 
				{
					REThread::uSleep(10);
				}
				delete methodWrapper;
			}
		}
	}
}



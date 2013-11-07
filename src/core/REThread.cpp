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
#include "../../include/recore/REMutex.h"
#include "../../include/recore/private/REDetachableThreadPrivate.h"
#include "../../include/recore/private/REMainThreadClassMethodPrivate.h"
#include "../../include/recore/private/REMainThreadClassMethodWaitedPrivate.h"
#include "../../include/recore/private/REAutoReleasePoolPrivate.h"
#include "../../include/recore/REMath.h"
#include "../../include/recore/REList.h"

#ifdef __RE_DEBUG_MODE__
#include "../../include/recore/RELog.h"
#endif


#if defined(HAVE_RECORE_SDK_CONFIG_H) 
#include "recore_sdk_config.h"
#endif

#if defined(HAVE_PTHREAD_H)
#include <pthread.h>
#elif defined(__RE_OS_WINDOWS__)
/* Use Windows threading */
#ifndef __RE_USING_WINDOWS_THREADS__
#define __RE_USING_WINDOWS_THREADS__
#endif
#include <Windows.h>
#endif /* __RE_OS_WINDOWS__ */

#if defined(HAVE_SYS_TIME_H)
#include <sys/time.h>
#endif

#if defined(HAVE_UNISTD_H)
#include <unistd.h>
#endif

#if defined(HAVE_DISPATCH_DISPATCH_H)
#include <dispatch/dispatch.h>
#endif

#if !defined(HAVE_FUNCTION_PTHREAD_CANCEL)		
#include <signal.h>
#endif

typedef void(*REThreadInvokeThreadBodyFunction)(REThread *);

class REThreadInternal 
{
private:
	REBOOL _isTaskFinished;
public:
	REBOOL isTaskFinished() const { return _isTaskFinished; }
#if defined(HAVE_PTHREAD_H) 
	pthread_t _reThreadThread;
#elif defined(__RE_USING_WINDOWS_THREADS__)	
	HANDLE _reThreadThread;
#endif
	REThread * parent;
	REThreadInvokeThreadBodyFunction invokeFunction;
	
	void threadBody()
	{		
		REThread * p = parent;
		REThreadInvokeThreadBodyFunction f = invokeFunction;
		if (p && f) 
		{
			f(p); 
		}
		_isTaskFinished = true;
	}
	
	REThreadInternal(REThread * p) :
	_isTaskFinished(false),
#if defined(HAVE_PTHREAD_H) 
	_reThreadThread((pthread_t)0),
#elif defined(__RE_USING_WINDOWS_THREADS__)	
	_reThreadThread((HANDLE)0),
#endif
	parent(p),
	invokeFunction(NULL)
	{
		
#if !defined(HAVE_FUNCTION_PTHREAD_CANCEL)		
		static bool needSetup = true;
		if (needSetup) 
		{
			needSetup = false;
			struct sigaction actions;
			memset(&actions, 0, sizeof(actions)); 
			sigemptyset(&actions.sa_mask);
			actions.sa_flags = 0; 
			actions.sa_handler = thread_exit_handler;
			sigaction(SIGUSR2,&actions,NULL);
		}
#endif		
		if (!_internalsMutex.isInitialized()) 
		{
			_internalsMutex.init(REMutexTypeRecursive);
		}
	}
	
	~REThreadInternal()
	{
		this->cancel();
	}
	
	REBOOL start()
	{
#if defined(HAVE_PTHREAD_H)  
		if (_reThreadThread) { return false; }
		
		size_t stackSize = 0;
		
#if defined(HAVE_FUNCTION_GETRLIMIT)		
		struct rlimit limit = { 0 };
		const int rc = getrlimit(RLIMIT_STACK, &limit);
		if (rc == 0) 
		{
			if (limit.rlim_cur != 0) { stackSize = (size_t)limit.rlim_cur; }
		}
#endif		
		
		pthread_attr_t threadAttr = { 0 };
		pthread_attr_init(&threadAttr);
		
#if defined( HAVE_FUNCTION_PTHREAD_ATTR_SETSCOPE) 
		pthread_attr_setscope(&threadAttr, PTHREAD_SCOPE_SYSTEM);
#endif		
		
		pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_JOINABLE);
		if (stackSize > 0) 
		{
#if defined(HAVE_FUNCTION_PTHREAD_ATTR_SETSTACKSIZE)			
			pthread_attr_setstacksize(&threadAttr, stackSize);
#endif			
		}
		
		REBOOL isCreated = false;
		pthread_t newThread = (pthread_t)0;
		if (pthread_create(&newThread, &threadAttr, REThreadInternal::threadFunction, REPtrCast<void, REThreadInternal>(this)) == 0) 
		{
			isCreated = true;
			REThreadInternal::isMultiThreaded = true;
			_reThreadThread = newThread;
		}
		
		pthread_attr_destroy(&threadAttr);
		return isCreated;
		
#elif defined(__RE_USING_WINDOWS_THREADS__) 
		HANDLE hThread = CreateThread(NULL, 0, REThreadInternal::threadProc, REPtrCast<LPVOID, REThreadInternal>(this), 0, NULL);
		if (hThread)
		{
			REThreadInternal::isMultiThreaded = true;
			_reThreadThread = hThread;
			this->addState(REThreadStateCreated);
			return true;
		}
#endif	
		
		return false;
	}
		
#if !defined(HAVE_FUNCTION_PTHREAD_CANCEL)	
	static void thread_exit_handler(int sig)
	{ 
		pthread_exit(0);
	}
#endif
	
	REBOOL cancel()
	{
#if defined(HAVE_PTHREAD_H)  
		if (_reThreadThread) 
		{
			REBOOL res = false;
			pthread_t th = _reThreadThread;
			
			pthread_kill(th, SIGUSR2);
			
#if defined(HAVE_FUNCTION_PTHREAD_CANCEL)						
			if (pthread_cancel(th) == 0) 
#else
			pthread_kill(th, SIGUSR2);
#endif				
			{
				void * r = NULL;
				if (pthread_join(th, &r) == 0) 
				{
#if defined(HAVE_FUNCTION_PTHREAD_CANCEL)											
					res = (r == PTHREAD_CANCELED);
#else
					res = true;
#endif
				}
			}
			_reThreadThread = (pthread_t)0;
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
		return true;
	}
	
	REFloat32 priority() const
	{
#if defined(HAVE_PTHREAD_H)  
		if (_reThreadThread) 
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
	
	REBOOL setPriority(const REFloat32 newPriority)
	{
		if ((newPriority <= 0.0f) || (newPriority > 1.0f)) return false;
#if defined(HAVE_PTHREAD_H)  
		if (_reThreadThread) 
		{
			const int maxPrior = sched_get_priority_max(SCHED_RR);
			if (maxPrior > 0) 
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

	
#if defined(HAVE_PTHREAD_H)  
	static pthread_t mainPThread;
	static void * threadFunction(void * th);
#elif defined(__RE_USING_WINDOWS_THREADS__)
	static DWORD mainThreadID;
	static DWORD WINAPI threadProc(LPVOID lpParameter);
#endif	
	static REBOOL isMultiThreaded;
	static void onDone(REThreadInternal * t);
	
private:
	static REList<REThreadInternal *> _internalsList;
	static REMutex _internalsMutex;
};

REList<REThreadInternal *> REThreadInternal::_internalsList;
REMutex REThreadInternal::_internalsMutex;

void REThreadInternal::onDone(REThreadInternal * t)
{
	_internalsMutex.lock();
	REList<REThreadInternal *>::Iterator iter = _internalsList.iterator();
	while (iter.next()) 
	{
		REThreadInternal * ti = iter.value();
		if (ti->isTaskFinished()) 
		{
			if (_internalsList.removeNode(iter.node())) 
			{
				ti->cancel();
				delete ti;
			}
		}
	}
	_internalsList.add(t);
	_internalsMutex.unlock();
}

#if defined(HAVE_PTHREAD_H)  
pthread_t REThreadInternal::mainPThread = (pthread_t)0;
void * REThreadInternal::threadFunction(void * th)
{
	if (th)
	{
		REThreadInternal * reThread = REPtrCast<REThreadInternal, void>(th);
		reThread->threadBody();
	}
	return th;
}
#elif defined(__RE_USING_WINDOWS_THREADS__)
DWORD REThreadInternal::mainThreadID = (DWORD)0;
DWORD REThreadInternal::threadProc(LPVOID lpParameter)
{
	if (lpParameter)
	{
		REThreadInternal * reThread = REPtrCast<REThreadInternal, LPVOID>(lpParameter);
		reThread->threadBody();
	}
	return 0;
}
#endif	
REBOOL REThreadInternal::isMultiThreaded = false;


REBOOL REThread::start()
{
	return (_t) ? _t->start() : false;
}

REBOOL REThread::cancel()
{
	if (_t) 
	{
		const REBOOL r = _t->cancel();
		delete _t;
		_t = NULL;
		return r;
	}
	return true;
}

REBOOL REThread::isTaskFinished() const
{
	return _isTaskFinished;
}

REFloat32 REThread::priority() const
{
	return (_t) ? _t->priority() : 0.0f;
}

REBOOL REThread::setPriority(const REFloat32 newPriority)
{
	return (_t) ? _t->setPriority(newPriority) : false;
}

/// Return flag is thread placed to autorelease pool when it's work done.
REBOOL REThread::isAutoReleaseWhenDone() const 
{
	return _isAutoreleaseWhenDone;
}

/// Mark thread that it must be placed to autorelease pool when it's work done.
void REThread::setAutoReleaseWhenDone(const REBOOL isAutorelease)
{
	_isAutoreleaseWhenDone = isAutorelease;
}

void REThread::invokeThreadBody(REThread * thread)
{
	if (thread)
	{
		REThreadInternal * ti = thread->_t;
		thread->_t = NULL;
		
		ti->parent = NULL;
		ti->invokeFunction = NULL;
		
		thread->threadBody();
		thread->_isTaskFinished = true;
		if (thread->_isAutoreleaseWhenDone) 
		{
			delete thread;	
		}
		REThreadInternal::onDone(ti);
	}
}

REThread::REThread() : REObject(),
	_isTaskFinished(false),
	_isAutoreleaseWhenDone(false)
{
	_t = new REThreadInternal(this);
	if (_t) 
	{
		_t->invokeFunction = &REThread::invokeThreadBody;
	}
	
#if defined(HAVE_PTHREAD_H)  
	if (REThreadInternal::mainPThread == (pthread_t)0) 
	{
		REThreadInternal::mainPThread = pthread_self();
	}
#elif defined(__RE_USING_WINDOWS_THREADS__) 
	if (REThreadInternal::mainThreadID == (HANDLE)0)
	{
		REThreadInternal::mainThreadID = GetCurrentThreadId();
	}
#endif	
}

REThread::~REThread()
{
	this->cancel();
}

REBOOL REThread::isMultiThreaded()
{
	return REThreadInternal::isMultiThreaded;
}

REUIdentifier REThread::mainThreadIdentifier()
{
	REUIdentifier thID = 0;
#if defined(HAVE_PTHREAD_H)  
	if (REThreadInternal::mainPThread == (pthread_t)0)
	{
		REThreadInternal::mainPThread = pthread_self();
	}
	thID = ((REUIdentifier)REThreadInternal::mainPThread);
#elif defined(__RE_USING_WINDOWS_THREADS__)
	if (REThreadInternal::mainThreadID == 0)
	{
		REThreadInternal::mainThreadID = GetCurrentThreadId();
	}
	thID = ((REUIdentifier)REThreadInternal::mainPThread);
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
	LARGE_INTEGER time1 = 0, time2 = 0, sysFreq = 0;

	QueryPerformanceCounter(&time1);
	QueryPerformanceFrequency(&sysFreq);
	do 
	{
		QueryPerformanceCounter(&time2);
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
	REThread::uSleep((REUInt32)RETime::convertSecondsToMicroseconds(seconds));
}

REBOOL REThread::isMainThread()
{
#if defined(HAVE_PTHREAD_H)  
	pthread_t curThread = pthread_self();
	if (REThreadInternal::mainPThread) 
	{
		return (pthread_equal(curThread, REThreadInternal::mainPThread) != 0);
	}
	else
	{
		REThreadInternal::mainPThread = curThread;
	}
#elif defined(__RE_USING_WINDOWS_THREADS__) 
	const DWORD curTHID = GetCurrentThreadId();
	if (REThreadInternal::mainThreadID)
	{
		return (REThreadInternal::mainThreadID == curTHID);
	}
	else
	{
		REThreadInternal::mainThreadID = curTHID;
	}
#endif	
	return true;
}

void REThread::detachNewThreadWithMethod(REClassMethod * method, REObject * methodObjectOrNULL)
{
	if (method)
	{
#if defined(HAVE_FUNCTION_DISPATCH_ASYNC) && defined(HAVE_FUNCTION_DISPATCH_GET_GLOBAL_QUEUE) 
		if (methodObjectOrNULL) { methodObjectOrNULL->retain(); }
		dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^(void){
			method->invokeWithObject(methodObjectOrNULL);
			delete method;
			if (methodObjectOrNULL) { methodObjectOrNULL->release(); }
		});
#else		
		REDetachableThreadPrivate * newThread = new REDetachableThreadPrivate(method, methodObjectOrNULL);
		if (newThread)
		{
			newThread->setAutoReleaseWhenDone(true);
			newThread->start();
		}
#endif		
	}
}

void REThread::detachNewThreadWithMethodAfterDelay(REClassMethod * method, REObject * methodObjectOrNULL, const RETimeInterval delayTime)
{
	if (method) 
	{
#if defined(HAVE_FUNCTION_DISPATCH_AFTER) && defined(HAVE_FUNCTION_DISPATCH_GET_GLOBAL_QUEUE)
		if (methodObjectOrNULL) { methodObjectOrNULL->retain(); }
		const dispatch_time_t popTime = dispatch_time(DISPATCH_TIME_NOW, (int64_t)(delayTime * NSEC_PER_SEC));
		dispatch_after(popTime, dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^(void){
			method->invokeWithObject(methodObjectOrNULL);
			delete method;
			if (methodObjectOrNULL) { methodObjectOrNULL->release(); }
		});
#else		
		REDetachableThreadPrivate * newThread = new REDetachableThreadPrivate(method, methodObjectOrNULL);
		if (newThread)
		{
			newThread->setAutoReleaseWhenDone(true);
			newThread->startWithDelay(delayTime);
		}
#endif		
	}
}

void REThread::performMethodOnMainThread(REClassMethod * method, REObject * methodObjectOrNULL)
{
	if (method) 
	{
		if (REThread::isMainThread()) 
		{
			if (methodObjectOrNULL) { methodObjectOrNULL->retain(); }
			method->invokeWithObject(methodObjectOrNULL);
			delete method;
			if (methodObjectOrNULL) { methodObjectOrNULL->release(); }
		}
		else 
		{
#if defined(HAVE_FUNCTION_DISPATCH_ASYNC)
			if (methodObjectOrNULL) { methodObjectOrNULL->retain(); }
			dispatch_async(dispatch_get_main_queue(), ^(void){
				method->invokeWithObject(methodObjectOrNULL);
				delete method;
				if (methodObjectOrNULL) { methodObjectOrNULL->release(); }
			});
#else
			REMainThreadClassMethodPrivate * methodWrapper = new REMainThreadClassMethodPrivate(method, methodObjectOrNULL);
			if (!methodWrapper) { delete method; }
#endif			
		}
	}
}

void REThread::performMethodOnMainThreadAndWaitUntilDone(REClassMethod * method, REObject * methodObjectOrNULL)
{
	if (method) 
	{	
		if (REThread::isMainThread()) 
		{
			if (methodObjectOrNULL) { methodObjectOrNULL->retain(); }
			method->invokeWithObject(methodObjectOrNULL);
			delete method;
			if (methodObjectOrNULL) { methodObjectOrNULL->release(); }
		}
		else 
		{
#if defined(HAVE_FUNCTION_DISPATCH_SYNC)	
			if (methodObjectOrNULL) { methodObjectOrNULL->retain(); }
			dispatch_sync(dispatch_get_main_queue(), ^(void){
				method->invokeWithObject(methodObjectOrNULL);
			});
			delete method;
			if (methodObjectOrNULL) { methodObjectOrNULL->release(); }
#else			
			REMainThreadClassMethodWaitedPrivate * methodWrapper = new REMainThreadClassMethodWaitedPrivate(method, methodObjectOrNULL);
			if (methodWrapper) 
			{
				while (methodWrapper->isWaiting()) 
				{
					REThread::uSleep(10);
				}
				delete methodWrapper;
			}
#endif			
		}
	}
}


void REThread::detachNewThreadWithFunction(REThread::PerformFunction function, void * userData)
{
	if (function) 
	{
#if defined(HAVE_FUNCTION_DISPATCH_ASYNC) && defined(HAVE_FUNCTION_DISPATCH_GET_GLOBAL_QUEUE) 
		dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^(void){
			function(userData);
		});
#else		
		REDetachableThreadPrivate * newThread = new REDetachableThreadPrivate(function, userData);
		if (newThread)
		{
			newThread->setAutoReleaseWhenDone(true);
			newThread->start();
		}
#endif		
	}
}


void REThread::detachNewThreadWithFunctionAfterDelay(REThread::PerformFunction function, void * userData, const RETimeInterval delayTime)
{
	if (function) 
	{
#if defined(HAVE_FUNCTION_DISPATCH_AFTER) && defined(HAVE_FUNCTION_DISPATCH_GET_GLOBAL_QUEUE)
		const dispatch_time_t popTime = dispatch_time(DISPATCH_TIME_NOW, (int64_t)(delayTime * NSEC_PER_SEC));
		dispatch_after(popTime, dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^(void){
			function(userData);
		});
#else		
		REDetachableThreadPrivate * newThread = new REDetachableThreadPrivate(function, userData);
		if (newThread)
		{
			newThread->setAutoReleaseWhenDone(true);
			newThread->startWithDelay(delayTime);
		}
#endif	
	}
}


void REThread::performFunctionOnMainThread(REThread::PerformFunction function, void * userData)
{
	if (function) 
	{
		if (REThread::isMainThread()) 
		{
			function(userData);
		}
		else 
		{
#if defined(HAVE_FUNCTION_DISPATCH_ASYNC)
			dispatch_async(dispatch_get_main_queue(), ^(void){
				function(userData);
			});
#else
			REMainThreadClassMethodPrivate * methodWrapper = new REMainThreadClassMethodPrivate(function, userData);
			if (methodWrapper) { return; }
#endif			
		}
	}
}


void REThread::performFunctionOnMainThreadAndWaitUntilDone(REThread::PerformFunction function, void * userData)
{
	if (function) 
	{
		if (REThread::isMainThread()) 
		{
			function(userData);
		}
		else 
		{
#if defined(HAVE_FUNCTION_DISPATCH_SYNC)	
			dispatch_sync(dispatch_get_main_queue(), ^(void){
				function(userData);
			});
#else			
			REMainThreadClassMethodWaitedPrivate * methodWrapper = new REMainThreadClassMethodWaitedPrivate(function, userData);
			if (methodWrapper) 
			{
				while (methodWrapper->isWaiting()) 
				{
					REThread::uSleep(10);
				}
				delete methodWrapper;
			}
#endif			
		}
	}
}



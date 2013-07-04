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


#ifndef __RETHREAD_H__
#define __RETHREAD_H__

#include "RECommonHeader.h"
#include "REObject.h"
#include "REClassMethod.h"
#include "REAutoReleasePool.h"

#if defined(__RE_TRY_USE_PTHREADS__) && defined(__RE_HAVE_SYSTEM_PTHREAD_H__) 
#include <pthread.h>
#endif

#ifndef __RE_TRY_USE_PTHREADS__
#ifdef __RE_OS_WINDOWS__
#ifndef __RE_USING_WINDOWS_THREADS__
#define __RE_USING_WINDOWS_THREADS__
#endif
#include <Windows.h>
#endif
#endif

typedef enum _reThreadState
{
	REThreadStateNone = 0,
	REThreadStateCreated = 1,
	REThreadStateDidEnterThreadFunc = (1 << 1),
	REThreadStateWillExitThreadFunc = (1 << 2),
	REThreadStateWillStartThreadBody = (1 << 3),
	REThreadStateDidEndThreadBody = (1 << 4)
}
REThreadState;

// http://locklessinc.com/articles/pthreads_on_windows/

/// Thread class for aditional work
class __RE_PUBLIC_CLASS_API__ REThread : public REObject
{
private:
#if defined(__RE_TRY_USE_PTHREADS__) && defined(__RE_HAVE_SYSTEM_PTHREAD_H__) 
	pthread_t _reThreadThread;
#elif defined(__RE_USING_WINDOWS_THREADS__)	
	HANDLE _reThreadThread;
#endif
	
	REUInt16 _reThreadStates;
	REBOOL _reThreadIsAutoreleaseWhenDone;

	void _threadBody();
	void addState(const REThreadState & state);
	void removeState(const REThreadState & state);
	REBOOL isHasState(const REThreadState & state) const;
	
#if defined(__RE_TRY_USE_PTHREADS__) && defined(__RE_HAVE_SYSTEM_PTHREAD_H__) 
	static void * threadFunction(void * th);
#elif defined(__RE_USING_WINDOWS_THREADS__)
	static DWORD WINAPI threadProc(LPVOID lpParameter);
#endif	
	
protected:	
	virtual void threadBody() = 0;
	
public:
	/* REObject */
	virtual const REUInt32 getClassIdentifier() const;
	static const REUInt32 classIdentifier();
	virtual REBOOL isImplementsClass(const REUInt32 classIdentifier) const;
	
	/// Returns working thread priority. Value in range: [0.0f, 1.0f]
	REFloat32 getPriority() const;
	
	/// Setting working thread priority. Value must be in range: [0.0f, 1.0f]
	REBOOL setPriority(const REFloat32 newPriority);
	
	/// Return flag is thread placed to autorelease pool when it's work done.
	REBOOL isAutoReleaseWhenDone() const { return _reThreadIsAutoreleaseWhenDone; }
	
	/// Mark thread that it must be placed to autorelease pool when it's work done.
	void setAutoReleaseWhenDone(REBOOL isAutorelease) { _reThreadIsAutoreleaseWhenDone = isAutorelease; }
	
	/// Start thread's work
	REBOOL start();
	
	/// Stop thread
	REBOOL stop();
	
	/// Check thread working or not.
	REBOOL isWorking() const;
	
	REThread();
	virtual ~REThread();
	
	/// Checking is executing in main thread.
	static REBOOL isMainThread();
	
	/// Return main thread priority. Range [0.0f, 1.0f]
	static REFloat32 getMainThreadPriority();
	
	/// Setting main thread priority. Range [0.0f, 1.0f] 
	static REBOOL setMainThreadPriority(const REFloat32 newPriority);
	
	/// If at least one aditional thread is created.
	static REBOOL isMultiThreaded();
	
	/// Returns identifier of main thread.
	static REUIdentifier getMainThreadIdentifier();
	
	/// Returns identifier of current thread. Mean thread from which was called this method.
	static REUIdentifier getCurrentThreadIdentifier();
	
	/// Sleps current thread for time in micro seconds.
	static void uSleep(const REUInt32 microseconds);
	
	/// Invokes class method in separate thread.
	/// Sending object will be ratained on start and released on the end of work.
	static void detachNewThreadWithMethod(REClassMethod * method, REObject * methodObjectOrNULL);
	
	/// After delay in seconds invokes class method in separate thread.
	/// Sending object will be ratained on start and released on the end of work.
	static void detachNewThreadWithMethodAfterDelay(REClassMethod * method, REObject * methodObjectOrNULL, RETimeInterval delayTime);
	
	/// Invokes class method in main thread.
	/// Sending object will be ratained on start and released on the end of work.
	static void performMethodOnMainThread(REClassMethod * method, REObject * methodObjectOrNULL);
	
	/// Invokes class method in main thread and waiting while thread finished.
	/// Sending object will be ratained on start and released on the end of work.
	static void performMethodOnMainThreadAndWaitUntilDone(REClassMethod * method, REObject * methodObjectOrNULL);
};


#endif /* __RETHREAD_H__ */

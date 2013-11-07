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
#include "REPtr.h"

// http://locklessinc.com/articles/pthreads_on_windows/

class REThreadInternal;

/// Thread class for aditional work
class __RE_PUBLIC_CLASS_API__ REThread : public REObject
{
private:
	REThreadInternal * _t;
	REBOOL _isTaskFinished;
	REBOOL _isAutoreleaseWhenDone;
	
	static void invokeThreadBody(REThread * thread);
	
protected:
	virtual void threadBody() = 0;
	
public:
	typedef void(*PerformFunction)(void * userData);
	
	REBOOL isTaskFinished() const;
	
	/// Returns working thread priority. Value in range: [0.0f, 1.0f]
	REFloat32 priority() const;
	
	/// Setting working thread priority. Value must be in range: [0.0f, 1.0f]
	REBOOL setPriority(const REFloat32 newPriority);
	
	/// Return flag is thread placed to autorelease pool when it's work done.
	REBOOL isAutoReleaseWhenDone() const;
	
	/// Mark thread that it must be placed to autorelease pool when it's work done.
	void setAutoReleaseWhenDone(const REBOOL isAutorelease);
	
	/// Start thread's work
	REBOOL start();
	
	/// Cancels thread
	REBOOL cancel();
	
	REThread();
	virtual ~REThread();
	
	/// Checking is executing in main thread.
	static REBOOL isMainThread();
	
	/// If at least one aditional thread is created.
	static REBOOL isMultiThreaded();
	
	/// Returns identifier of main thread.
	static REUIdentifier mainThreadIdentifier();
	
	/// Returns identifier of current thread. Mean thread from which was called this method.
	static REUIdentifier currentThreadIdentifier();
	
	/// Sleps current thread for time in micro seconds.
	static void uSleep(const REUInt32 microseconds);
	
	/// Sleps current thread for time in micro seconds.
	static void uSleepInSeconds(const RETimeInterval seconds);
	
	/// Invokes class method in separate thread.
	/// Sending object will be ratained on start and released on the end of work.
	static void detachNewThreadWithMethod(REClassMethod * method, REObject * methodObjectOrNULL);
	
	/// After delay in seconds invokes class method in separate thread.
	/// Sending object will be ratained on start and released on the end of work.
	static void detachNewThreadWithMethodAfterDelay(REClassMethod * method, REObject * methodObjectOrNULL, const RETimeInterval delayTime);
	
	/// Invokes class method in main thread.
	/// Sending object will be ratained on start and released on the end of work.
	static void performMethodOnMainThread(REClassMethod * method, REObject * methodObjectOrNULL);
	
	/// Invokes class method in main thread and waiting while thread finished.
	/// Sending object will be ratained on start and released on the end of work.
	static void performMethodOnMainThreadAndWaitUntilDone(REClassMethod * method, REObject * methodObjectOrNULL);
	
	/// Invokes function in separate thread.
	/// Sending user data pointer to function.
	static void detachNewThreadWithFunction(REThread::PerformFunction function, void * userData);
	
	/// After delay in seconds invokes function in separate thread.
	/// Sending user data pointer to function.
	static void detachNewThreadWithFunctionAfterDelay(REThread::PerformFunction function, void * userData, const RETimeInterval delayTime);
	
	/// Invokes function in main thread.
	/// Sending user data pointer to function.
	static void performFunctionOnMainThread(REThread::PerformFunction function, void * userData);
	
	/// Invokes function in main thread and waiting while thread finished.
	/// Sending user data pointer to function.
	static void performFunctionOnMainThreadAndWaitUntilDone(REThread::PerformFunction function, void * userData);
};


#endif /* __RETHREAD_H__ */
